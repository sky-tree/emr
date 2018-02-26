#include "SelectView.h"
#include "ConstantsID.h"
#include "ExcelCellData.h"
SelectView::SelectView() :BaseElement(ConstantsID::ClassId::SELECT_VIEW_OBJ)
, mSideType(Middle)
, mSelectObj(NULL)
, mSelectLine(NULL)
, mSelectCellData(NULL)
{
}


SelectView::~SelectView()
{
	if (mSelectCellData){
		mSelectCellData = NULL;
	}
	if (mSelectObj){
		mSelectObj = NULL;
	}
	if (mSelectLine){
		mSelectLine = NULL;
	}
}
void SelectView::init(){
	mSideType = Middle;
	mSelectObj = NULL;
	mSelectLine = NULL;
	mSelectCellData = NULL;
}
void SelectView::SetSelect(ViewObj * selectObj, Side side){
	mSideType = side;
	mSelectObj = selectObj;
}
Line* SelectView::getSelectLine(){
	if (!mSelectObj){
		mSelectLine = NULL;
		return NULL;
	}
	mSelectLine = NULL;
	BaseContentData *  contentParam = mSelectObj->getContentData();
	mSelectCellData = (ExcelCellData*)contentParam;
	switch (mSideType){
	case SelectView::Side::Left:
		mSelectLine = mSelectCellData->mLeft;
		break;
	case SelectView::Side::Right:
		mSelectLine = mSelectCellData->mRight;
		break;
	case SelectView::Side::Top:
		mSelectLine = mSelectCellData->mTop;
		break;
	case SelectView::Side::Bottom:
		mSelectLine = mSelectCellData->mBottom;
		break;
	case SelectView::Side::Middle:
		mSelectLine = NULL;
		break;
	}
	return mSelectLine;
}

