#include "LineMoveAction.h"
#include "ConstantsID.h"
#include "ViewObj.h"
#include "iostream"
#include "Line.h"
#include "ExcelTemplete.h"

#include "OperateMgr.h"


#include "BaseConstants.h"

LineMoveAction::LineMoveAction(StepActionManger * stepActonManger, ExcelTemplete* excelTemplete) : BaseAction(stepActonManger, ConstantsID::ClassId::MOVE_EXCEL_LINE_ACTION)
{
	mMoveLineList = new QList<Line *>;
	mPrePositionList = new QList<int>;
	mPreDragPositionList = new QList<int>;
	mSetPositionList = new QList<int>;
	mSetDragPositionList = new QList<int>;
	mNeedRelayout = false;
	mExcelTemplete=excelTemplete;
}
LineMoveAction::~LineMoveAction()
{
	if (mMoveLineList){
	delete	mMoveLineList;
		mMoveLineList = NULL;
	}
	if (mPrePositionList){
		delete	mPrePositionList;
		mPrePositionList = NULL;
	}
	if (mPreDragPositionList){
		delete	mPreDragPositionList;
		mPreDragPositionList = NULL;
	}
	if (mSetPositionList){
		delete	mSetPositionList;
		mSetPositionList = NULL;
	}
	if (mSetDragPositionList){
		delete	mSetDragPositionList;
		mSetDragPositionList = NULL;
	}
	if (mExcelTemplete){
		mExcelTemplete = NULL;
	}
}
void LineMoveAction::addMoveLine(Line * line, int preposition, int preDragposition, int setPosition, int setDragPosition)
{
	mMoveLineList->push_back(line);
	mPrePositionList->push_back(preposition);
	mSetPositionList->push_back(setPosition);
	mPreDragPositionList->push_back(preDragposition);
	mSetDragPositionList->push_back(setDragPosition);
}
void LineMoveAction::setRelayout(bool relayout)
{
	mNeedRelayout = relayout;
}
void LineMoveAction::unDo()
{
	Line * temporary;
	int size = mMoveLineList->size();
	for (int i = size - 1; i >= 0;i--)
	{
		temporary = mMoveLineList->at(i);
		temporary->setPosition(mPrePositionList->at(i));
		temporary->setDragPosition(mPreDragPositionList->at(i));
	}
	mExcelTemplete->sortLine();
	mOperateMgr->getPagePanel()->changeStatus(BaseConstants::ObjStatus::FREE, false);
	if (mNeedRelayout)
	{
		mExcelTemplete->requestLayout();
	}
}
void LineMoveAction::reDo()
{
	Line * temporary;
	int size = mMoveLineList->size();
	for (int i = size - 1; i >= 0; i--)
	{
		temporary = mMoveLineList->at(i);
		temporary->setPosition(mSetPositionList->at(i));
		temporary->setDragPosition(mSetDragPositionList->at(i));
	}
	mExcelTemplete->sortLine();
	if (mNeedRelayout)
	{
		mExcelTemplete->requestLayout();
	}
}