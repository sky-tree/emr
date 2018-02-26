#include "ExcelCellData.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "ViewObj.h"
#include "BaseLayout.h"
#include "BaseViewParam.h"
#include "BaseIOTools.h"
#include "ObjDrawManager.h"
#include "VerticalViewParam.h"
#include "RowLayout.h"

#include "ContentDataFactory.h"
#include "ViewFactory.h"
#include "TempContainerMgr.h"
#include "DrawRect.h"
#include "OperateMgr.h"
#include "ExcelTemplete.h"
#include "WriterManager.h"
#include "FlagParam.h"
#include "TempParagraphMgrData.h"
#include "ExcelLayout.h"
#include "TempContainerMgr.h"
#include "ParagraphManager.h"
#include "BaseConstants.h"
ExcelCellData::ExcelCellData(ViewObj * ownerView, BaseTemplate* templeteOwner, CodeParams * codeParams) :
CodeTempData(templeteOwner, ConstantsID::ClassId::EXCEL_CELL_DATA, codeParams), ExcelBaseLayout()
, mLeft(NULL)
, mRight(NULL)
, mTop(NULL)
, mBottom(NULL)
, mRootTemplete(NULL)
//, mWindObj(NULL)
, mViewOwner(ownerView)
, mCanPrint(true)
, mCanEdit(true)
, mVerticalPosition(BaseConstants::Direction::MIDDLE)
, mCellStatus(OnNomal)
{
	//mComposingHeightType = BaseConstants::ComposingType::AT_MOST;
	//mComposingWidthType = BaseConstants::ComposingType::EXACTLY;
	setNeedLoadSelfChildren(false);
}


ExcelCellData::~ExcelCellData()
{
	if (mLeft){
		mLeft = NULL;
	}
	if (mRight){
		mRight = NULL;
	}
	if (mTop){
		mTop = NULL;
	}
	if (mBottom){
		mBottom = NULL;
	}
	if (mRootTemplete)
	{
		delete mRootTemplete;
		mRootTemplete = NULL;
	}
	/*if (mWindObj)
	{
		mWindObj = NULL;
	}*/
	if (mViewOwner)
	{
		mViewOwner = NULL;
	}
}
void ExcelCellData::initData(){
	if (mLeft){
		mLeft = NULL;
	}
	if (mRight){
		mRight = NULL;
	}
	if (mTop){
		mTop = NULL;
	}
	if (mBottom){
		mBottom = NULL;
	}
	mVerticalPosition = BaseConstants::Direction::MIDDLE;
}
void ExcelCellData ::initLines(Line * left, Line * top, Line * right, Line * bottom){
	mLeft = left;
	mRight = right;
	mTop = top;
	mBottom = bottom;
}
void ExcelCellData::initFromOther(ExcelCellData* otherCellData)
{
	mLeft = otherCellData->mLeft;
	mRight = otherCellData->mRight;
	mTop = otherCellData->mTop;
	mBottom = otherCellData->mBottom;
	mVerticalPosition = otherCellData->mVerticalPosition;
}
void ExcelCellData::onMeasure(MeasureWH * measureWH, bool isFreshFromInnerChange){
	mCellStatus = OnMeause;
	//if (mComposingWidthType == BaseConstants::ComposingType::EXACTLY)
	{
		int width = mRight->getPosition() - mLeft->getPosition();
		//int preHeight = mBottom->getDragPosition() - mTop->getDragPosition();
		/*MeasureWH temptMeasur;
		temptMeasur.measureWidth = width;
		temptMeasur.measureHeight = INT_MAX;
		temptMeasur.measureHeightMode = mComposingHeightType;
		temptMeasur.measureWidthMode = mComposingWidthType;*/
		//测量子内容的宽高
		VerticalViewParam * viewParam = (VerticalViewParam*)mViewOwner->getViewParam();
		int preWidth = viewParam->getTotalW();
		int dis = width - preWidth;
		viewParam->setSelfW(width);
		int preHeight = viewParam->getTotalH();
		//QList<ViewObj *> * layers = (QList<ViewObj *> *)mViewOwner->getChildren();
		//for (int i = 0; i < layers->size(); i ++)
		{
			//layers->at(i)->getViewParam()->refreshWHByParent();
		}
		
		ViewObj * txtLayer = (ViewObj *)mViewOwner->findChildrenByID(ConstantsID::ContainerId::EXCEL_TXT_LAYER, false);
		VerticalViewParam * layerViewParam = (VerticalViewParam *)txtLayer->getViewParam();
		replaceExcelPage(txtLayer, layerViewParam, dis, isFreshFromInnerChange);
		//cur = layerViewParam->getTotalH();
		//if (pre != cur)
		//{
		//	mViewOwner->getWinData()->addToReplaceViewList(txtLayer);
		//}
		//但调用这个函数会自动潜逃调用你的函数，要死循环
		//txtLayer->getViewParam()->refreshAllChildrenWHByParent();
		//txtLayer->getLayout()->refreshChildrenPlace();

		//调用排版，刷新页的高度
		//viewParam->set
		//int height = layerViewParam->getTotalH() + viewParam->getInsideTop() + viewParam->getInsideBottom();
		int height = layerViewParam->getChildrenRowH();
		height = height+viewParam->getInsideTop() + viewParam->getInsideBottom();
		if (ExcelLayout::defaultCellH>height)
		{
			setMeasureWH(width, ExcelLayout::defaultCellH, height);
		}
		else
		{
			setMeasureWH(width, height, height);
		}
		
	}
}
void ExcelCellData::replaceExcelPage(ViewObj * txtLayer, VerticalViewParam * layerViewParam, int disInWidth, bool isFreshFromInnerChange)
{
	
	/*layerViewParam->setMinH(-1);
	layerViewParam->refreshWHByParent();
	txtLayer->getLayout()->refreshChildrenPlace();*/
	layerViewParam->refreshWByParent(true);
	QList<BaseContainer *> * rowList = txtLayer->getChildren();
	ViewObj * rowTempt;
	BaseLayout * rowLayout;
	for (int i = 0; i < rowList->size(); i++)
	{
		rowTempt = (ViewObj*)rowList->at(i);
		rowLayout = rowTempt->getLayout();
		rowLayout->openReplace();
	}
	if (rowList)
	{
		ViewObj* lastChild = mRootTemplete->getLastChildView();
		if (rowList->size()>0)
		{
			ViewObj* firstRow = (ViewObj*)rowList->at(0);
			RowLayout*rowLayout = (RowLayout*)firstRow->getLayout();
			if (disInWidth < 0)
			{
				rowLayout->judgeOverFlowCircleUntilObj(lastChild);
				//firstRow->getViewParent()->getLayout()->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
			}
			else if (disInWidth > 0)
			{
				rowLayout->judgeLessThanCircleUntilObj(lastChild);
				//firstRow->getViewParent()->getLayout()->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
			}
			else
			{
				if (isFreshFromInnerChange)
				{
					rowLayout->judgeOverFlowCircleUntilObj(lastChild);
					rowLayout->judgeLessThanCircleUntilObj(lastChild);
				}

			}

		}
	}
	txtLayer->getLayout()->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
	layerViewParam->refreshHByChildren(true);
}
void ExcelCellData::onLayout(bool isChanged, int l, int t, int r, int b){
	mCellStatus = OnLayout;
	VerticalViewParam * viewParam = (VerticalViewParam *)mViewOwner->getViewParam();
	viewParam->setX(l);
	viewParam->setY(t);
	/*int currentTotalH = mBottom->getPosition() - mTop->getPosition();
	int currentH = currentTotalH - viewParam->getInsideTop() - viewParam->getInsideBottom();
	ViewObj * txtLayer = (ViewObj *)mViewOwner->getChildren()->at(ConstantsID::IndexId::LAYER_TXT);
	LinearVHViewParam * layerViewParam = (LinearVHViewParam *)txtLayer->getViewParam();
	if (layerViewParam->getTotalH()<currentH)
	{
		layerViewParam->setMinH(currentH);
		layerViewParam->setSelfH(currentH);
	}
	viewParam->setSelfH(currentTotalH);*/

	ViewObj * txtLayer = (ViewObj *)mViewOwner->findChildrenByID(ConstantsID::ContainerId::EXCEL_TXT_LAYER, false);
	VerticalViewParam * layerViewParam = (VerticalViewParam *)txtLayer->getViewParam();
	int currentTotalH = mBottom->getPosition() - mTop->getPosition();
	int currentInsideH = currentTotalH - viewParam->getInsideTop() - viewParam->getInsideBottom();

	//layerViewParam->setMinH(currentInsideH);
	layerViewParam->setSelfH(currentInsideH);
	viewParam->setSelfH(currentTotalH);
	txtLayer->getLayout()->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
	mCellStatus = OnNomal;
}

void ExcelCellData::saveSelfData(BaseIOTools *iOTools)
{
	//return;
	mOwnerObjId = mRootTemplete->getObjID();
	if (mOwnerObjId == ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER)
	{
		//BaseIOTools::writeBool(mTempleteOwner->mIsShowBorder);
		iOTools->writeBool(mCanEdit);
		iOTools->writeBool(mCanPrint);
		ViewObj* cellView = (ViewObj*)getViewOwner();
		ObjDrawManager * drawManger = mViewOwner->getDrawMgr();
		DrawRect *  draw = (DrawRect *)drawManger->getBottomDraw();
		draw->saveSelfData(iOTools);
		
		VerticalViewParam  * viewParam = (VerticalViewParam *)mViewOwner->getViewParam();
		int insideWidth = viewParam->getInsideLeft();
		iOTools->writeInt(insideWidth);
		iOTools->writeInt(mVerticalPosition);
		mRootTemplete->getContentData()->doSave(iOTools);
	}

}

void ExcelCellData::loadSelfData(BaseIOTools *iOTools)
{
	//return;
	//int i = 200;
	//if (mOwnerObjId == ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER)
	{
		mCanEdit = iOTools->readBool();
		mCanPrint = iOTools->readBool();
		
	}

}
BaseContainer * ExcelCellData::createOwner(BaseContainer * tempParent1, BaseIOTools *iOTools, bool isWillDelete)
{
	BaseTemplate * excelTemplete;
	//if (mOwnerObjId == ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER)
	{
		DrawRect* drawRect = new DrawRect(NULL,0,0,0,0,true);
		drawRect->loadSelfData(iOTools);
		mInsideWidth = iOTools->readInt();
		mVerticalPosition = iOTools->readInt();
		excelTemplete = (BaseTemplate *)ContentDataFactory::loadData(tempParent1->getWriterMgr(), iOTools, tempParent1);
		
		mRootTemplete = excelTemplete;

		ViewObj * cellPage = ViewFactory::createExcelPage(tempParent1->getWriterMgr(), mLeft->getPosition(), mTop->getPosition()
			, mRight->getPosition() - mLeft->getPosition(), mBottom->getPosition() - mTop->getPosition(), (ExcelTemplete*)tempParent1,true);
		//设置边框
		ObjDrawManager * drawManger = cellPage->getDrawMgr();
		drawManger->setBottomDraw(drawRect);
		drawRect->setViewOwner(cellPage);
		setViewOwner(cellPage);
		cellPage->initContentData(this);
		excelTemplete->setExcelCellData(this);
		ExcelLayout::setExcelInsideProperty(cellPage,mInsideWidth);
		//上一步新建page时重置了mOwner，所以这里需要重新再设置一下。
		setCellTempleteOwner(excelTemplete);

		OperateMgr * operateMgr = tempParent1->getOperateMgr();
		operateMgr->removeObjAfterLastEnter(mRootTemplete->getChildrenMgr());
		//加入window
		if (isWillDelete)
		{

		}
		else
		{
			operateMgr->setFocusOfEndPress(NULL, -1);

			((TempContainerMgr*)mRootTemplete->getChildrenMgr())->addAllChildrenToParagraphAndView();
			operateMgr->setFocusOfEndPress(NULL, -1);
		}
		return mOwner;
	}
	
	return NULL;
}
void ExcelCellData::createCellPageByOther(WriterManager * wMgr, ExcelCellData *otherCellData, ExcelTemplete* excelTemplete)
{
		mCanEdit = otherCellData->mCanEdit;
		mCanPrint = otherCellData->mCanPrint;
		mVerticalPosition = otherCellData->mVerticalPosition;
	    BaseTemplate * otherCellRoot = otherCellData->mRootTemplete;
		TempParagraphMgrData * otherData = (TempParagraphMgrData *)otherCellRoot->getContentData();
		TempContainerMgr * otherChildrenMgr = (TempContainerMgr *) otherCellRoot->getChildrenMgr();
		ViewObj * otherCellPage = otherCellData->getViewOwner();
		ObjDrawManager *otherDrawManger = otherCellPage->getDrawMgr();
		DrawRect *  otherDraw = (DrawRect *)otherDrawManger->getBottomDraw();
		VerticalViewParam * otherViewParam = (VerticalViewParam *)otherCellPage->getViewParam();

		//CodeParams * codeParams = new CodeParams("rootTempID", "rootTemp");
		mRootTemplete = new BaseTemplate(wMgr, otherCellRoot, false);
		TempParagraphMgrData * rootData = new TempParagraphMgrData(mRootTemplete, otherData);
		mRootTemplete->initContentData(rootData);
		ParagraphManager * rootParagraphMgr = rootData->getParagraphMgr();
		rootParagraphMgr->setExcelParagrphManager();
		TempContainerMgr * rootChildrenMgr = new TempContainerMgr(mRootTemplete,rootParagraphMgr,otherChildrenMgr);
		QList<BaseContainer *> * tempChildrenList = rootChildrenMgr->getChildren();
		BaseContainer * tempObj = NULL;
		for (int i = 0; i < tempChildrenList->size();)
		{
			tempObj = tempChildrenList->at(i);
			if (tempObj)
			{
				tempObj->setTempleteParent(mRootTemplete);
				i++;
			}
			else
			{
				tempChildrenList->removeAt(i);
			}
		
		}
		mRootTemplete->setTempleteParent(excelTemplete);
		mRootTemplete->refreshChildrenIndex(0);
		ViewObj * cellPage = ViewFactory::createExcelPage(wMgr, mLeft->getPosition(), mTop->getPosition()
			, mRight->getPosition() - mLeft->getPosition(), mBottom->getPosition() - mTop->getPosition(), excelTemplete,true);
	//设置边框
		ObjDrawManager *drawManger = cellPage->getDrawMgr();
		DrawRect *  draw = new DrawRect(cellPage, otherDraw);
		drawManger->initDraws(draw, NULL);
		VerticalViewParam * viewParam = (VerticalViewParam *)cellPage->getViewParam();
	int insideLeft = otherViewParam->getInsideLeft();
	int insideRight = otherViewParam->getInsideRight();
	viewParam->setInsideLeft(insideLeft);
	viewParam->setInsideRight(insideRight);
	/*viewParam->setInsideLeft(insideLeft);
	viewParam->setInsideRight(insideRight);*/
	ExcelLayout::setExcelInsideProperty(cellPage, insideLeft);
	//上一步新建page时重置了mOwner，所以这里需要重新再设置一下。
	cellPage->initContentData(this);
	mRootTemplete->setExcelCellData(this);
	setCellTempleteOwner(mRootTemplete);
	setViewOwner(cellPage);
	
	//生成页段落
	ViewObj* preFocus = NULL;
	char preDir = 0;
	excelTemplete->getFocusObjAndDir(preFocus, preDir);
	excelTemplete->savePressStartAndEnd();
	excelTemplete->getOperateMgr()->setFocusOfEndPress(NULL, -1);
	//mRootTemplete->getChildrenMgr()->addChild(enter, -1, true);
	rootChildrenMgr->addAllChildrenToParagraphAndView();
	ViewObj* firstObj = mRootTemplete->getFirstChildView();
	firstObj->getViewParent()->getViewParam()->refreshHByChildren(true);
	if (preFocus)
	{
		excelTemplete->backSetPressStartAndEnd(true);
	}
}
int ExcelCellData::getMaxWidthOfChildView()
{
	return mRootTemplete->getMaxWidthOfChildView();
}
ViewObj * ExcelCellData::getViewOwner()
{
	return mViewOwner;
}
void ExcelCellData::setViewOwner(ViewObj * viewOwner)
{
	mViewOwner = viewOwner;
}
void ExcelCellData::setTemplateOwner(BaseTemplate *templateOwner)
{
	setCellTempleteOwner(templateOwner);
}
void ExcelCellData::setCellTempleteOwner(BaseTemplate * templeteOwner)
{
	mRootTemplete = templeteOwner;
	mOwner = templeteOwner;
}
bool ExcelCellData::getCanPrint()
{
	return mCanPrint;
}
bool ExcelCellData::getCanEdit()
{
	return mCanEdit;
}
int ExcelCellData::getCellLineHeight()
{
	return mBottom->getPosition() - mTop->getPosition();
}
ParagraphManager * ExcelCellData::getParagraphMgr()
{
	TempParagraphMgrData * data = (TempParagraphMgrData *)mRootTemplete->getContentData();
	return data->getParagraphMgr();
}
void ExcelCellData::setVerticalPosition(int position)
{
	mVerticalPosition = position;
}
int ExcelCellData::getVerticalPosition()
{
	return mVerticalPosition;
}
int ExcelCellData::getIntegrityRowByMaxH(int bottomH)
{
	VerticalViewParam * viewParam = (VerticalViewParam*)mViewOwner->getViewParam();
	int beginH = viewParam->getInsideTop() + viewParam->getY();
	ViewObj * txtLayer = (ViewObj *)mViewOwner->findChildrenByID(ConstantsID::ContainerId::EXCEL_TXT_LAYER, false);
	VerticalViewParam * layerViewParam = (VerticalViewParam *)txtLayer->getViewParam();
	beginH = beginH+layerViewParam->getInsideTop();
	QList<BaseContainer *> * rowList = txtLayer->getChildren();
	ViewObj * rowTempt;
	int rowH = 0;
	for (int i = 0; i < rowList->size(); i++)
	{
		rowTempt = (ViewObj*)rowList->at(i);
		rowH = rowTempt->getViewParam()->getTotalH();
		if (rowH + beginH>bottomH)
		{
			return beginH;
		}
		else
		{
			beginH = beginH + rowH;
		}
	}
	if (beginH + layerViewParam->getInsideBottom() > bottomH)
	{
		return beginH;
	}
	else
	{
		return beginH + layerViewParam->getInsideBottom();
	}
	if (beginH + viewParam->getInsideBottom() > bottomH)
	{
		return beginH;
	}
	else
	{
		//return beginH + viewParam->getInsideBottom();
		return  bottomH;
	}
}