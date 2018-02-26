#include "ExcelTemplete.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "ViewFactory.h"
//#include "BaseWinData.h"
#include "ViewObj.h"
#include "FlagParam.h"
#include "ExcelLayout.h"
//#include "NormalRowParam.h"
#include "RowLayout.h"
#include "ExcelCellDataRow.h"
#include "QMessageBox.h"
#include "ExcelMouseInputManager.h"
#include "WriterManager.h"
#include "TempleteTypeEnumeration.h"
#include "ExcelTempleteParamData.h"
#include "TempleteViewObj.h"
#include "ExcelCellData.h"
#include "GlobalTemplate.h"
#include "VerticalViewParam.h"
#include "RowViewParam.h"
#include "ObjMouseManager.h"
#include "StrConstants.h"
#include "OperateMgr.h"
#include "BaseTempleteKeyboard.h"
#include "ExcelKeyboard.h"
#include "qdebug.h"
#include "BaseTools.h"
#include "StrContentData.h"
ExcelTemplete::ExcelTemplete(WriterManager * wMgr, BaseTemplate * templeteParent, int objId, QString codeId)
: BaseTemplate(wMgr, ConstantsID::ClassId::EXCEL_TEMPLETE, objId, codeId)
, mVirtulExcel(NULL)
, mExcelLayout(NULL)
, mLayoutRowObjList(NULL)
, mTemptViewFocus(NULL)
, mExcelInputMouseManager(NULL)
{
	setTempleteParent(templeteParent);
	BaseChildrenManager* childMgr = new BaseChildrenManager(this);
	mLayoutRowObjList = new QList<ViewObj*>();
	isInit = false;
	mLayoutRowObjIndex = -1;
	mIsShowBorder = false;
	mIsOnLayout = false;
	ExcelMouseInputManager* mouseManager = new ExcelMouseInputManager(this);
	this->setMouse(mouseManager);
	ExcelKeyboard * keyboard = new ExcelKeyboard(this);
	this->setKeyboard(keyboard);
	mCanDragExcel = true;
	mCanOperateExcelCell = true;
}
ExcelTemplete::ExcelTemplete(WriterManager * wMgr, ExcelTemplete* otherExcelTemplete) : BaseTemplate(wMgr, otherExcelTemplete, true)
, mVirtulExcel(NULL)
, mExcelLayout(NULL)
, mLayoutRowObjList(NULL)
, mTemptViewFocus(NULL)
, mExcelInputMouseManager(NULL)
{
	BaseChildrenManager* childMgr = new BaseChildrenManager(this);
	ExcelMouseInputManager* mouseManager = new ExcelMouseInputManager(this);
	this->setMouse(mouseManager);
	ExcelKeyboard * keyboard = new ExcelKeyboard(this);
	this->setKeyboard(keyboard);
	mLayoutRowObjList = new QList<ViewObj*>();
	isInit = false;
	mLayoutRowObjIndex = -1;
	mIsShowBorder = false;
	mIsOnLayout = false;
	initExcelTemplete(otherExcelTemplete->getIsShowBorader(), false);
	ExcelTempleteParamData * excelData = new ExcelTempleteParamData(this, NULL);
	initContentData(excelData);
	mExcelLayout->initByOther(wMgr, otherExcelTemplete->mExcelLayout);
	setInitOver();
	addWidgetToChildMgr();
	mCanDragExcel = otherExcelTemplete->mCanDragExcel;
	mCanOperateExcelCell = otherExcelTemplete->mCanOperateExcelCell;
	//addWidgetToRow();
}


ExcelTemplete::~ExcelTemplete()
{
	mExcelLayout->clearAllCellTxtLayerForWinData(mWriterMgr);
	getChildrenMgr()->delChildren(0, -1, true);
	if (mVirtulExcel)
	{
		//mVirtulExcel->initSelfTemplete(NULL);
		//mVirtulExcel->clearViewParent();
		delete mVirtulExcel;
		mVirtulExcel = NULL;
	}
	if (mExcelLayout)
	{
		mExcelLayout = NULL;
	}
	if (mStartRow)
	{
		mStartRow = NULL;
	}
	if (mTemptViewFocus)
	{
		mTemptViewFocus = NULL;
	}
	if (mLayoutRowObjList)
	{
		int size = mLayoutRowObjList->size();
		//for (int i = 0; i < size; i++)
		//{
			//mLayoutRowObjList->at(i)->setIsJudgeViewParenOnDecontruction(mIsJudgeViewParenOnDecontruction);
		//}
		deleteVector(mLayoutRowObjList);
		//delete mLayoutRowObjList;
		mLayoutRowObjList = NULL;
	}
	if (mExcelInputMouseManager)
	{
		mExcelInputMouseManager = NULL;
	}
}
void ExcelTemplete::initExcelTemplete(int row, int col, int w, int h, ViewObj *initRow, bool isShowBorder)
{
	//ViewObj * window = initRow->getParentWindow();
	mIsShowBorder = isShowBorder;
	mVirtulExcel = ViewFactory::createExcel(this, initRow, 0, 0, w, h, row, col, false);
	mExcelLayout = (ExcelLayout *)mVirtulExcel->getLayout();
	((ExcelMouseInputManager*)mInputMouse)->init();
	((ExcelKeyboard*)mInputKeyboard)->init();
	//mStartRow = initRow;
	isInit = true;
	addWidgetToChildMgr();
}
void ExcelTemplete::initExcelTemplete(bool isShowBorder, bool isFromCreate)
{
	mIsShowBorder = isShowBorder;
	mVirtulExcel = ViewFactory::createExcel(this, NULL, 0, 0, 0, 0, 0, 0, true);
	//mVirtulExcel->setViewParent(getViewParent());
	mExcelLayout = (ExcelLayout *)mVirtulExcel->getLayout();
	if (isFromCreate)
	{
		ExcelTempleteParamData * excelTemplete = (ExcelTempleteParamData *)getContentData();
		if (!mExcelLayout->mCells)
		{
			mExcelLayout->mCells = excelTemplete->mCells;
		}
		if (!mExcelLayout->mRowLines)
		{
			mExcelLayout->mRowLines = excelTemplete->mRowLines;
		}
		if (!mExcelLayout->mColLines)
		{
			mExcelLayout->mColLines = excelTemplete->mColLines;
		}
	}
	((ExcelMouseInputManager*)mInputMouse)->init();
	((ExcelKeyboard*)mInputKeyboard)->init();
	//isInit = true;
	if (isFromCreate)
	{
		addWidgetToChildMgr();
	}

}
void ExcelTemplete::setInitOver()
{
	isInit = true;
}
/*光标要设置到当前表格上*/
void ExcelTemplete::requestLayoutByCell(ViewObj * currentRow, int index)
{
	resetExcelRect();
	int totalCellSize = mExcelLayout->mCells->size();
	if (totalCellSize <= 0)
	{
		this->deleteSelf();
		return;
	}
	if (mExcelLayout->mColLines->size() <= 1)
	{
		return;
	}
	//算起点到页底的高
	ExcelLayoutViewParam * excelViewParam = (ExcelLayoutViewParam *)mVirtulExcel->getViewParam();
	/*if (currentRow)
	{
	ViewObj * txtLayer = currentRow->getViewParent();
	VerticalViewParam * layoutViewParam = (VerticalViewParam *)txtLayer->getViewParam();
	int layoutBottom = layoutViewParam->getInsideTop() + layoutViewParam->getInsideH();
	RowViewParam * rowParam = (RowViewParam *)currentRow->getViewParam();
	int rowMaxH = layoutBottom - rowParam->getY() - rowParam->getInsideTop() - rowParam->getInsideBottom();
	}else
	{
	ViewObj * txtLayer = getPageLayer();
	}*/
	int currentRowCanHaveThroughROW = mThroughRowIndex;
	int endThroughRow = -1;
	int totalRow = mExcelLayout->mRowLines->size() - 1;
	if (totalRow <= 0)
	{
		this->deleteSelf(false);
		return;
	}
	ExcelCellDataRow* temptCelDataRow = NULL;
	int startLinePosition = mExcelLayout->getCellDataList(mThroughRowIndex)->getCellDataList()->at(0)->mTop->getPosition();
	Line* currentLine = NULL;
	int totalH = 0;
	while (currentRowCanHaveThroughROW < totalRow)
	{
		temptCelDataRow = mExcelLayout->getCellDataList(currentRowCanHaveThroughROW);
		if (temptCelDataRow->isThrough)
		{
			currentLine = temptCelDataRow->getCellDataList()->at(0)->mBottom;
			totalH = currentLine->getPosition() - startLinePosition;
			/*if (totalH <= rowMaxH){
				endThroughRow = currentRowCanHaveThroughROW;
				currentRowCanHaveThroughROW++;
				}
				else{

				break;
				}*/
			endThroughRow = currentRowCanHaveThroughROW;
			currentRowCanHaveThroughROW++;

		}
		else
		{
			currentRowCanHaveThroughROW++;
		}
	}
	//如果一个通行都装不下，判断当前行是否是在页首行，如果是，就直接装载，如果不是，就把该行跳到下一页。跳下页是addchild函数自己处理。
	//如果装的下一个通行或以上。装载这些通行。获取下一个行，继续layout；
	int layoutSize = 0;
	if (endThroughRow == -1)
	{
		if (currentRowCanHaveThroughROW == totalRow)
		{//整个表格不是一个通行
			layoutSize = currentRowCanHaveThroughROW;
		}
		else
		{//整个表格是一个通行
			layoutSize = currentRowCanHaveThroughROW + 1;
		}
	}
	else
	{
		layoutSize = endThroughRow + 1;
	}
	//生成一个外壳
	ViewObj * containerObj = getRowObj(mLayoutRowObjIndex);

	int cellDataListSize = 0;
	ExcelCellData *tempCellData;
	int startCellP = 0;
	if (mThroughRowIndex == 0)
	{
		startCellP = startLinePosition - excelViewParam->getInsideTop();
	}
	int cellX, cellY, cellRight, cellBottom;
	ViewObj* cellPageView;
	
	for (int i = mThroughRowIndex; i < layoutSize; i++)
	{
		temptCelDataRow = mExcelLayout->getCellDataList(i);
		cellDataListSize = temptCelDataRow->mCellsList->size();
		for (int m = 0; m < cellDataListSize; m++)
		{
			tempCellData = temptCelDataRow->mCellsList->at(m);
			cellX = tempCellData->mLeft->getPosition();
			cellY = tempCellData->mTop->getPosition() - startCellP;
			cellRight = tempCellData->mRight->getPosition();
			cellBottom = tempCellData->mBottom->getPosition() - startCellP;
			tempCellData->onLayout(0, cellX, cellY, cellRight, cellBottom);
			resizeRect(cellX, cellY, cellRight, cellBottom);
			cellPageView = tempCellData->getViewOwner();
			containerObj->getChildrenMgr()->addChild(cellPageView, -1, true);
		}
	}
	//检测没显示的就去除掉
	QList<BaseContainer *> * showList = containerObj->getChildren();
	mExcelLayout->removeNotShowCell(showList);
	temptCelDataRow = mExcelLayout->getCellDataList(layoutSize - 1);
	totalH = temptCelDataRow->getCellDataList()->at(0)->mBottom->getPosition() - startLinePosition;
	BaseViewParam * contentParam = containerObj->getViewParam();

	if (layoutSize == totalRow)
	{

		totalH += excelViewParam->getInsideBottom();
	}
	if (mThroughRowIndex == 0)
	{
		totalH += excelViewParam->getInsideTop();
	}
	contentParam->setSelfH(mExcelRect.height());
	contentParam->setSelfW(mExcelRect.width());


}
void ExcelTemplete::addContainerToPage(bool isNeedAddToShow)
{

	//为了解决重排版，当前行没有数据，当前行被删除问题，第一行的包装view 保留。所以不在重复加入
	/*ViewObj * containerObj = NULL;
	BaseChildrenManager * childMgr = getChildrenMgr();
	for (int i = mLayoutRowObjIndex; i >= 0; i--)
	{
	containerObj = mLayoutRowObjList->at(i);
	childMgr->addChild(containerObj,-1,true);
	}*/
	if (isNeedAddToShow)
	{
		QList<BaseContainer *> * children = getChildren();
		addViewObjOnShow(children, true);
	}
}
void ExcelTemplete::addWidgetToChildMgr()
{
	if (!isInit)
	{
		return;
	}
	if (mIsOnLayout)
		return;
	mIsOnLayout = true;
	ViewObj * preFocus = NULL;
	char preDir = 0;
	getFocusObjAndDir(preFocus, preDir);
	if (preFocus)
	{
		getOperateMgr()->savePressStartAndEnd2();
	}
	ViewObj * preFocus2 = NULL;
	char preDir2 = 0;
	layoutAgent(NULL, 0, preFocus, preDir, preFocus2, preDir2, false);
	if (preFocus2)
	{
		preFocus2->setFocus1(preDir2);
	}

	addContainerToPage(false);
	if (preFocus)
	{
		//preFocus->setFocus1(preDir);
		getOperateMgr()->backSetPressStartAndEnd2(true);
	}

	mIsOnLayout = false;
}
void ExcelTemplete::layoutAgent(ViewObj *row, int index, ViewObj *preFocus, char direction, ViewObj *&preFocus2, char& direction2, bool isFreshFromInnerChange)
{

	refreshRowObj();
	getFocusObjAndDir(preFocus2, direction2);
	if (preFocus)
	{
		//preFocus->setFocus1(direction);
		backSetPressStartAndEnd(true);
	}
	mExcelLayout->composing(isFreshFromInnerChange);
	mLayoutRowObjIndex = 0;
	mThroughRowIndex = 0;
	if (preFocus2)
	{
		mStartRow = preFocus2->getViewParent();
	}
	else
	{
		mStartRow = NULL;
	}

	requestLayoutByCell(mStartRow, index);
}
void ExcelTemplete::requestLayout(bool needResetFlag, bool isFreshFromInnerChange)
{
	if (!isInit)
	{
		return;
	}
	if (mIsOnLayout)
		return;
	mIsOnLayout = true;
	/*if (true)
	{
	int a = 0;
	}*/
	ViewObj * preFocus = NULL;
	char preDir = 0;
	getFocusObjAndDir(preFocus, preDir);
	if (preFocus)
	{
		getOperateMgr()->savePressStartAndEnd2();
	}
	ViewObj * tempRow = NULL;
	int addIndex = -1;
	ViewObj* firstContainer = mLayoutRowObjList->at(0);
	tempRow = firstContainer->getViewParent();
	if (tempRow)
	{
		//getRowAndIndexAtuo(tempRow,addIndex);
		firstContainer->setFocus(false);
		ViewObj * preFocus2 = NULL;
		char preDir2 = 0;
		layoutAgent(tempRow, addIndex, preFocus, preDir, preFocus2, preDir2, isFreshFromInnerChange);
		if (preFocus2)
		{
			preFocus2->setFocus1(preDir2);
		}

		addContainerToPage(true);
		if (needResetFlag)
		{
			

			/*if (preDir == BaseConstants::Direction::RIGHT)
			{
			preFocus->setFocus(true);
			}
			else
			{
			preFocus->setFocus(false);
			}*/

		}
	}
	if (preFocus)
	{
		//preFocus->setFocus1(preDir);
		OperateMgr * operateMgr = getOperateMgr();
		operateMgr->backSetPressStartAndEnd2(true);

	}
	//BaseWinData * winData = getWinData();
	//winData->replaceNeedReplaceList();
	mIsOnLayout = false;
}
/*void ExcelTemplete::addAllChildToView(BaseWinData * winData, bool needResetFlag)
{
ViewObj * window = getParentWindow();
//BaseWinData * winData = (BaseWinData *)window->getContentData();
//BaseWinData * winData = winData;
FlagParam * pressEnd = winData->getPressEnd();
ViewObj * focus = pressEnd->getFocus();
int idx = winData->getDestAddIndexByFocus();
// 初始化时，无焦点
if (idx == -1)
{
ViewObj * pageParent = (ViewObj *)window->getChild(0);
if (pageParent->getChildren()->size() > 0)
{
ViewObj * page = (ViewObj *)pageParent->getChild(0);
ViewObj * row = (ViewObj *)page->findContainerById(ConstantsID::ContainerId::PAGE_CONTENT, false)
->getChild(BaseConstants::LayerId::TXT_LAYER)->getChild(0);
layoutAgent(row, 0);
}
}
// 正常光标
else
{
//refreshRowObj();
//char dir = pressEnd->getDirection();
//ViewObj * focusParentView = focus->getViewParent();
//if (focus->isRow())
//{
//	idx = 0;
//	focusParentView = focus;
//}
//QList<BaseContainer *> * children = focusParentView->getChildren();
//BaseContainer * rightObj = NULL;
////BaseContainer * destFocus = child;
//if (children && children->size() > idx)
//{
//	rightObj = children->at(idx);
//}
//addWidgetToRow(focusParentView, idx);
ViewObj* row = NULL;
int viewIndex = 0;
if ((!mLayoutRowObjList) || mLayoutRowObjList->size() == 0)
{
getRowAndViewIndex(this, row, viewIndex);
mStartRow = row;

}
else
{
ViewObj* firstRowObj = mLayoutRowObjList->at(0);
ViewObj * parentRow = firstRowObj->getViewParent();
if (parentRow)
{
ViewObj * txtLayer = parentRow->getViewParent();
if (txtLayer->haveChild(parentRow))
{
mStartRow = firstRowObj->getViewParent();
row = mStartRow;
viewIndex = firstRowObj->getViewIndexFromParent();
if (viewIndex < 0)
{
getRowAndViewIndex(this, row, viewIndex);
mStartRow = row;
}
}
else
{
getRowAndViewIndex(this, row, viewIndex);
mStartRow = row;
}
}
else
{
getRowAndViewIndex(this, row, viewIndex);
mStartRow = row;
}

}
layoutAgent(mStartRow, viewIndex);
//focusParentView->addChild(child, idx);
//if (needResetFlag)
//{
//	// 如果当前焦点为回车，并且方向为左，则无需更换焦点
//	if (focus->isEnter() && dir == BaseConstants::Direction::LEFT)
//		return;
//	// 如果加入回车
//	if (child->isEnter())
//	{
//		// 如果当前方向向右，则将方向设为向左
//		if (dir == BaseConstants::Direction::RIGHT)
//		{
//			// 如果插入前，右侧有对象，则将焦点对象设为回车右侧的对象
//			if (rightObj)
//				destFocus = rightObj;
//			dir = BaseConstants::Direction::LEFT;
//		}
//	}
//	else
//	{
//		dir = BaseConstants::Direction::RIGHT;
//	}
//	winData->setPressStartOrEnd((ViewObj *)destFocus, dir, false);
//}
}
}*/
//void ExcelTemplete::addAllChildToBufList(QList<BaseContainer *> * viewList)
//{
//	ViewObj * window = getParentWindow();
//	BaseWinData * winData = (BaseWinData *)window->getContentData();
//	ViewObj* tempRow=NULL;
//	int indexView = 0;
//	getRowAndIndexAtuo(tempRow,indexView);
//	layoutAgent(tempRow, indexView);
//	ViewObj * containerObj;
//	for (int i = 0; i <= mLayoutRowObjIndex;i++)
//	{
//		 containerObj = mLayoutRowObjList->at(i);
//		 viewList->push_back(containerObj);
//	}
//}
//void ExcelTemplete::addAllChildToView(BaseWinData * winData, bool needResetFlag)
//{
//	requestLayout(needResetFlag);
//}
//void ExcelTemplete::addAllChildToBufList(QList<BaseContainer *> * children, int start, int num, QList<BaseContainer *> * destViewList)
//{
//	ViewObj * window = getParentWindow();
//	BaseWinData * winData = (BaseWinData *)window->getContentData();
//	ViewObj* tempRow = NULL;
//	int indexView = 0;
//	//getRowAndIndexAtuo(tempRow, indexView);
//	layoutAgent(tempRow, indexView);
//	ViewObj * containerObj;
//	for (int i = 0; i <= mLayoutRowObjIndex; i++)
//	{
//		containerObj = mLayoutRowObjList->at(i);
//		//children->push_back(containerObj);
//		destViewList->push_back(containerObj);
//	}
//}
//void ExcelTemplete::getRowAndIndexAtuo(ViewObj*& returnRow, int& returnViewIndex)
//{
//	ViewObj * window = getParentWindow();
//	BaseWinData * winData = (BaseWinData *)window->getContentData();
//	FlagParam * pressEnd = winData->getPressEnd();
//	ViewObj * focus = pressEnd->getFocus();
//	int idx = winData->getDestAddIndexByFocus();
//	// 初始化时，无焦点
//	if (idx == -1)
//	{
//		if (mLayoutRowObjList->size() < 1)
//		{
//			ViewObj * pageParent = (ViewObj *)window->getChild(0);
//			if (pageParent->getChildren()->size() > 0)
//			{
//				ViewObj * page = (ViewObj *)pageParent->getChild(0);
//				ViewObj * row1 = (ViewObj *)page->findContainerById(ConstantsID::ContainerId::PAGE_CONTENT, false)
//					->getChild(BaseConstants::LayerId::TXT_LAYER)->getChild(0);
//				returnRow = row1;
//				returnViewIndex = 0;
//			}
//		}
//		else
//		{
//			ViewObj* containerFist = mLayoutRowObjList->at(0);
//			returnRow = containerFist->getViewParent();
//			returnViewIndex = containerFist->getViewIndexFromParent();
//		}
//	}
//	// 正常光标
//	else
//	{
//		ViewObj* row = NULL;
//		int viewIndex = 0;
//		if ((!mLayoutRowObjList) || mLayoutRowObjList->size() == 0)
//		{
//			getRowAndViewIndex(this, row, viewIndex);
//			mStartRow = row;
//		}
//		else
//		{
//			ViewObj* firstRowObj = mLayoutRowObjList->at(0);
//			ViewObj * parentRow = firstRowObj->getViewParent();
//			if (parentRow)
//			{
//				ViewObj * txtLayer = parentRow->getViewParent();
//				if (txtLayer->haveChild(parentRow))
//				{
//					mStartRow = firstRowObj->getViewParent();
//					row = mStartRow;
//					viewIndex = firstRowObj->getViewIndexFromParent();
//					if (viewIndex < 0)
//					{
//						getRowAndViewIndex(this, row, viewIndex);
//						mStartRow = row;
//					}
//				}
//				else
//				{
//					getRowAndViewIndex(this, row, viewIndex);
//					mStartRow = row;
//				}
//			}
//			else
//			{
//				getRowAndViewIndex(this, row, viewIndex);
//				mStartRow = row;
//			}
//
//		}
//		returnRow = mStartRow;
//		returnViewIndex = viewIndex;
//
//		//focusParentView->addChild(child, idx);
//		//if (needResetFlag)
//		//{
//		//	// 如果当前焦点为回车，并且方向为左，则无需更换焦点
//		//	if (focus->isEnter() && dir == BaseConstants::Direction::LEFT)
//		//		return;
//		//	// 如果加入回车
//		//	if (child->isEnter())
//		//	{
//		//		// 如果当前方向向右，则将方向设为向左
//		//		if (dir == BaseConstants::Direction::RIGHT)
//		//		{
//		//			// 如果插入前，右侧有对象，则将焦点对象设为回车右侧的对象
//		//			if (rightObj)
//		//				destFocus = rightObj;
//		//			dir = BaseConstants::Direction::LEFT;
//		//		}
//		//	}
//		//	else
//		//	{
//		//		dir = BaseConstants::Direction::RIGHT;
//		//	}
//		//	winData->setPressStartOrEnd((ViewObj *)destFocus, dir, false);
//		//}
//	}
//}
ViewObj * ExcelTemplete::getRowObj(int index)
{
	int size = mLayoutRowObjList->size();
	if (index >= size)
	{
		ViewObj* temptRow;
		for (int i = size; i <= index; i++)
		{
			temptRow = ViewFactory::createExcelRowContainer(this, NULL);
			mLayoutRowObjList->push_back(temptRow);
		}
	}
	ViewObj * excelRowContainer = mLayoutRowObjList->at(index);
	BaseChildrenManager * childMgr = getChildrenMgr();
	if (index == 0)
	{
		//为了解决重排版，当前行没有数据，当前行被删除问题，第一行的包装view 保留。所以不在重复加入
		QList<BaseContainer *> * 	children = getChildren();

		if (children)
		{
			int indexFirst = children->indexOf(excelRowContainer);

			if (indexFirst < 0)
			{
				childMgr->addChild(excelRowContainer, -1, true);
			}
		}
		else
		{
			childMgr->addChild(excelRowContainer, -1, true);
		}

	}
	else
	{
		childMgr->addChild(excelRowContainer, -1, true);
	}
	excelRowContainer->getChildrenMgr()->delChildren(0, -1, true);
	excelRowContainer->setChooseObjId(mObjID);
	//OperateMgr * operateMgr = excelRowContainer->getOperateMgr();
	//BaseTemplete* parentTemplete = getTempleteParent();
	//if (!parentTemplete)
	//{
	//	//粘贴时没有parentTemplete
	//	operateMgr->setCanAddObjParentId(ConstantsID::ContainerId::NORMAL_PARAGRAPH_TEMPLETE);
	//}
	//else
	//{
	//	operateMgr->setCanAddObjParentId(parentTemplete->getObjId());
	//}
	//
	//operateMgr->setCanBeChoosenObjId(getObjId());
	return excelRowContainer;
}
void ExcelTemplete::refreshRowObj()
{
	int size = mLayoutRowObjList->size();
	ViewObj* tempRow;
	for (int i = 0; i < size; i++)
	{
		tempRow = mLayoutRowObjList->at(i);
		deleteChild(0, -1, true);
		//if (i == 0){
		//	//为了解决重排版，当前行没有数据，当前行被删除问题，第一行的包装view 保留。
		//}
		//else{
		//	delChild(temptRow, true);
		//}
		//newDelChild(tempRow, true, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF, false, false);
		//delChild(tempRow, true);
		//temptRow->getViewParent()->delChild(temptRow, true);
	}
}
ViewObj * ExcelTemplete::getWindowObj()
{
	return	mLayoutRowObjList->at(0)->getParentViewById(ConstantsID::ContainerId::TXT_WIN_VIEW);
}
int ExcelTemplete::mousePress(int type, int x, int y, BaseContainer * obj)
{
	if (type == Qt::RightButton)
	{
		return 0;
	}
	//if (!canEditTempleteProperty())
	//{//表格特殊，左键也能改变属性。所以这里特殊处理
	//	return 0;
	//}
	bool isMyRowContainer = false;
	int size = mLayoutRowObjList->size();
	for (int i = 0; i < size; i++)
	{
		if (mLayoutRowObjList->at(i) == obj)
		{
			isMyRowContainer = true;
			break;
		}
	}
	TempleteViewObj* cellViewRow = (TempleteViewObj*)obj;
	//BaseViewParam *  viewParam = cellViewRow->getViewParam();
	if (isMyRowContainer)
	{
		ViewObj* windowView = (ViewObj*)cellViewRow->getParentViewById(ConstantsID::ContainerId::TXT_WIN_VIEW);
		mTemptViewFocus = cellViewRow;
		windowView->getMouseMgr()->setFocus(mTemptViewFocus);
		//return mVirtulExcel->getInputManager()->mousePress(type, viewParam->getX() + x, viewParam->getY() + y);
		return mInputMouse->mousePress(type, x, y, obj);
	}
	return BaseConstants::NONE;
	/*ExcelCellData * cellData = mExcelLayout->getCellDataFromCell(obj);
	if (!cellData)
	{
	return BaseConstants::NONE;
	}
	return mVirtulExcel->getInputManager()->mousePress(type, cellData->mLeft->getPosition() + x, cellData->mTop->getPosition() + y);*/
}
int ExcelTemplete::mouseRelease(int type, int x, int y, BaseContainer * obj)
{
	if (type == Qt::RightButton)
	{
		return 0;
	}
	/*if (!canEditTempleteProperty())
	{
	return 0;
	}*/
	bool isMyRowContainer = false;
	int size = mLayoutRowObjList->size();
	for (int i = 0; i < size; i++)
	{
		if (mLayoutRowObjList->at(i) == obj)
		{
			isMyRowContainer = true;
		}
	}
	TempleteViewObj* cellViewRow = (TempleteViewObj*)obj;
	BaseViewParam *  viewParam = cellViewRow->getViewParam();
	if (isMyRowContainer)
	{
		int returnValue = 0;
		ViewObj* windowView = (ViewObj*)cellViewRow->getParentViewById(ConstantsID::ContainerId::TXT_WIN_VIEW);
		if (mTemptViewFocus)
		{
			//BaseViewParam *  viewParam1 = mTemptViewFocus->getViewParam();
			//returnValue = mVirtulExcel->getInputManager()->mouseRelease(type, viewParam1->getX() + x, viewParam1->getY() + y);
			returnValue = mInputMouse->mouseRelease(type, x, y, obj);
			windowView->getMouseMgr()->setFocus(NULL);
			mTemptViewFocus = NULL;
		}
		else
		{
			returnValue = mInputMouse->mouseRelease(type, viewParam->getX() + x, viewParam->getY() + y, obj);
		}
		return returnValue;
	}
	return 0;
	//ExcelCellData * cellData = mExcelLayout->getCellDataFromCell(obj);
	//if (!cellData)
	//{
	//	return BaseConstants::NONE;
	//}
	//return mVirtulExcel->getInputManager()->mouseRelease(type, cellData->mLeft->getPosition() + x, cellData->mTop->getPosition() + y);
}
int ExcelTemplete::mouseMove(int type, int x, int y, BaseContainer * obj)
{
	/*if (!canEditTempleteProperty())
	{
	return 0;
	}*/
	ExcelCellData * cellData = mExcelLayout->getCellDataFromCell(obj);
	if (!cellData)
	{
		return BaseConstants::NONE;
	}
	return mInputMouse->mouseMove(type, cellData->mLeft->getPosition() + x, cellData->mTop->getPosition() + y, obj);
}
int ExcelTemplete::mouseDrag(int type, int x, int y, BaseContainer * obj)
{
	bool isMyRowContainer = false;
	int size = mLayoutRowObjList->size();
	for (int i = 0; i < size; i++)
	{
		if (mLayoutRowObjList->at(i) == obj)
		{
			isMyRowContainer = true;
		}
	}
	TempleteViewObj* cellViewRow = (TempleteViewObj*)obj;
	BaseViewParam *  viewParam = cellViewRow->getViewParam();
	if (isMyRowContainer)
	{
		return mInputMouse->mouseDrag(type, x, y, obj);
	}
	if (cellViewRow->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
	{
		return BaseConstants::NONE;
	}
	ExcelCellData * cellData = mExcelLayout->getCellDataFromCell(obj);
	if (!cellData)
	{
		return BaseConstants::NONE;
	}
	return mInputMouse->mouseDrag(type, cellData->mLeft->getPosition() + x, cellData->mTop->getPosition() + y, obj);
}
int ExcelTemplete::mouseDoublePress(int type, int x, int y, BaseContainer * obj)
{
	bool isMyRowContainer = false;
	int size = mLayoutRowObjList->size();
	for (int i = 0; i < size; i++)
	{
		if (mLayoutRowObjList->at(i) == obj)
		{
			isMyRowContainer = true;
		}
	}
	TempleteViewObj* cellViewRow = (TempleteViewObj*)obj;
	BaseViewParam *  viewParam = cellViewRow->getViewParam();
	if (isMyRowContainer)
	{
		return mInputMouse->mouseDoublePress(type, x, y, obj);
	}
	ExcelCellData * cellData = mExcelLayout->getCellDataFromCell(obj);
	if (!cellData)
	{
		return BaseConstants::NONE;
	}
	return mInputMouse->mouseDoublePress(type, cellData->mLeft->getPosition() + x, cellData->mTop->getPosition() + y, obj);
}
int ExcelTemplete::mouseScroll(int angle, BaseContainer * obj)
{
	ExcelCellData * cellData = mExcelLayout->getCellDataFromCell(obj);
	if (!cellData)
	{
		return BaseConstants::NONE;
	}
	return mInputMouse->mouseScroll(angle, obj);
}
int ExcelTemplete::keyPress(QString str, BaseContainer * obj)
{

	ViewObj* lastRowObj = mLayoutRowObjList->at(mLayoutRowObjIndex);
	ViewObj* focus = NULL;
	char dir = 0;
	OperateMgr *operateMgr = getOperateMgr();
	getFocusObjAndDir(focus, dir);
	if (lastRowObj)	//表格跨页时，以最后一页的对象来删除整个表格
	{
		if (obj == lastRowObj)
		{

			if (isFocousObj(obj))
			{
				if (str.compare(StrConstants::MyKeyBoard::BACK_SPACE) == 0)
				{
					if (dir == BaseConstants::Direction::RIGHT)
					{
						if (operateMgr->isChoosed(this) || operateMgr->getChoosedSize() > 0)
						{
							return 0;
						}
						return 1;
					}
				}
				else if (str.compare(StrConstants::MyKeyBoard::DEL) == 0)
				{
					if (dir == BaseConstants::Direction::LEFT)
					{
						if (operateMgr->isChoosed(this) || operateMgr->getChoosedSize() > 0)
						{
							return 0;
						}
						return 1;
					}

				}
			}
		}
	}

	ExcelCellData * cellData = mExcelLayout->getCellDataFromCell(obj);
	if (cellData)//这里判断通过了，只表明信号传递者是本表格的表页
	{
		//处理拆分列行
		bool isDividSign = str.contains(TempleteTypeEnumeration::SISTEM_SIGN);
		if (isDividSign)
		{
			ViewObj * excelCellView = focus->getParentViewById(ConstantsID::ContainerId::EXCEL_PAGE);
			if (excelCellView&&excelCellView == obj)//真，光标在当前表格页
			{
				if (str.contains(TempleteTypeEnumeration::EXCEL_DEVIDE_H_V))
				{
					if (!mCanOperateExcelCell)
					{
						return 1;
					}
					QStringList strs = str.split(":");
					int row = strs.at(1).toInt();
					int col = strs.at(2).toInt();
					if (row <= 0 || col <= 0)
					{
						return 1;
					}

					mExcelLayout->dividCell(cellData, row, col);
					return 1;
				}
				else if (str.contains(TempleteTypeEnumeration::EXCEL_ADD_ROW_OR_COL))
				{
					if (!mCanOperateExcelCell)
					{
						return 1;
					}
					QStringList strs = str.split(":");
					int direction = strs.at(1).toInt();
					if (direction <= 0)
					{
						return 1;
					}
					mExcelLayout->addRowOrCol(cellData, direction);
					return 1;
				}
				else if (str.contains(TempleteTypeEnumeration::EXCEL_DEL_ROW_OR_COL))
				{
					if (!mCanOperateExcelCell)
					{
						return 1;
					}
					QStringList strs = str.split(":");
					QVariant tempValue = strs.at(1);
					bool isRow = tempValue.toBool();
					mExcelLayout->deleteRowOrCol(cellData, isRow);

					getOperateMgr()->clearChooseList(false);
					return 1;
				}
				else if (str.contains(TempleteTypeEnumeration::EXCEL_PROPERTY))
				{
					QStringList strs = str.split(":");
					QVariant tempValue = strs.at(1);
					bool isExelCellProperty = tempValue.toBool();
					if (isExelCellProperty)
					{
						//单元格属性
						mExcelLayout->showSetCellProperty(cellData);
					}
					else
					{//表格属性
						mExcelLayout->showSetExcelProperty(cellData);
					}
					return 1;
				}
				//正常输入到表格里面的信号
				if (!cellData->getCanEdit())
				{
					return 1;
				}
			}
		}

		return BaseConstants::NONE;
	}
	//信息会先传到包裹层
	bool isMyRowContainer = false;
	int size = mLayoutRowObjList->size();
	for (int i = 0; i < size; i++)
	{
		if (mLayoutRowObjList->at(i) == obj)
		{
			isMyRowContainer = true;
		}
	}
	if (isMyRowContainer)
	{
		int returnType = BaseConstants::NONE;
		returnType = mInputKeyboard->keyPress(str, obj);
		if (returnType == BaseConstants::NONE)
		{
			ViewObj * focus = NULL;
			char dir = 0;
			getFocusObjAndDir(focus, dir);
			ViewObj * tempObj1 = focus->getParentViewById(ConstantsID::ContainerId::EXCEL_PAGE);
			if (mExcelLayout->isMyCell(tempObj1))
			{
				//在拖选表格单元格时，不应该让输入有效
				ViewObj * window = mLayoutRowObjList->at(0)->getParentViewById(ConstantsID::ContainerId::TXT_WIN_VIEW);

				if (window&&window->getStatus() == BaseConstants::ObjStatus::EXCEL)
				{
					return 1;
				}
			}
			
		}
		
		return returnType;
		
	}
	return BaseConstants::NONE;
}
int ExcelTemplete::keyRelease(QString str, BaseContainer * obj)
{
	ExcelCellData * cellData = mExcelLayout->getCellDataFromCell(obj);
	if (!cellData)
	{
		return BaseConstants::NONE;
	}
	return mInputKeyboard->keyRelease(str, obj);
}
bool ExcelTemplete::getIsShowBorader()
{
	return mIsShowBorder;
}
ViewObj *  ExcelTemplete::getStartRow()
{
	if (mStartRow)
	{
		return mStartRow;
	}
	if (mLayoutRowObjList->size()>0)
	{
		ViewObj * excelContainer = mLayoutRowObjList->at(0);
		return excelContainer->getViewParent();
	}
	ViewObj* preView = BaseContainer::getPreView();
	if (preView&&!preView->isEnter())
	{
			return preView->getViewParent();
	}
	ViewObj* nextView = BaseContainer::getNextView();
	if (nextView)
	{
		return nextView->getViewParent();
	}
	
	return NULL;
}
void ExcelTemplete::showTidy()
{
	if (mIsShowTidy)
	{
		mIsShowTidy = false;
	}
	else
	{
		mIsShowTidy = true;
	}
	mExcelLayout->showTidy();
}
void ExcelTemplete::addExcelDragLineToContainer(ViewObj* line)
{
	ViewObj * excelRowContainer;
	int size = mLayoutRowObjList->size();
	for (int i = 0; i < size; i++)
	{
		excelRowContainer = mLayoutRowObjList->at(i);
		QList<BaseContainer *> * children = excelRowContainer->getChildren();
		int preIndex = children->indexOf(line);
		if (preIndex >= 0)
		{
			int lastIndex = children->size() - 1;
			if (preIndex != lastIndex)
			{
				children->swap(preIndex, lastIndex);
			}
		}
		else
		{
			children->push_back(line);
		}
	}
}
void ExcelTemplete::removeExcelDragLineFromContainer(ViewObj* line)
{
	ViewObj * excelRowContainer;
	int size = mLayoutRowObjList->size();
	for (int i = 0; i < size; i++)
	{
		excelRowContainer = mLayoutRowObjList->at(i);
		excelRowContainer->getChildren()->removeAll(line);
	}
}
void ExcelTemplete::resetExcelRect()
{
	mExcelRect.setX(0);
	mExcelRect.setY(0);
	mExcelRect.setRight(0);
	mExcelRect.setBottom(0);
}
void ExcelTemplete::resizeRect(int l, int t, int r, int b)
{
	if (mExcelRect.x() > l)
	{
		mExcelRect.setX(l);
	}
	if (mExcelRect.y() > t)
	{
		mExcelRect.setY(t);
	}
	if (mExcelRect.right() < r)
	{
		mExcelRect.setWidth(r);
	}
	if (mExcelRect.bottom() < b)
	{
		mExcelRect.setHeight(b);
	}
}
bool ExcelTemplete::isMyCell(QList<BaseContainer *> *  & chooseListT, ExcelTemplete * excelTemp)
{

	//判断是不是自己的表格操作
	int chooseListSize = chooseListT->size();
	if (chooseListSize == 0)
	{
		return false;
	}
	ViewObj * tempObj1 = NULL;
	int myCellSize = excelTemp->mExcelLayout->mCells->size();
	bool isMyCell = false;
	for (int i = 0; i < chooseListSize; i++)
	{
		tempObj1 = (ViewObj *)chooseListT->at(i);
		isMyCell = false;
		for (int m = 0; m < myCellSize; m++)
		{
			if (excelTemp->mExcelLayout->mCells->at(m)->getViewOwner() == tempObj1)
			{
				isMyCell = true;
				break;
			}
		}
		if (!isMyCell)
		{
			return false;
		}
	}
	return true;
}
bool ExcelTemplete::isMyCell(QList<ExcelCellData *>* & chooseListT, ExcelTemplete * excelTemp)
{
	//判断是不是自己的表格操作
	int chooseListSize = chooseListT->size();
	if (chooseListSize == 0)
	{
		return false;
	}
	ExcelCellData * tempObj1 = NULL;
	int myCellSize = excelTemp->mExcelLayout->mCells->size();
	bool isMyCell = false;
	for (int i = 0; i < chooseListSize; i++)
	{
		tempObj1 = chooseListT->at(i);
		isMyCell = false;
		for (int m = 0; m < myCellSize; m++)
		{
			if (excelTemp->mExcelLayout->mCells->at(m) == tempObj1)
			{
				isMyCell = true;
				break;
			}
		}
		if (!isMyCell)
		{
			return false;
		}
	}
	return true;
}
int ExcelTemplete::getCellSize()
{
	return mExcelLayout->mCells->size();
}
QList<ExcelCellData *>* ExcelTemplete::getCellList()
{
	if (mExcelLayout)
	{
		return mExcelLayout->mCells;
	}
	return NULL;
}
ExcelLayout *ExcelTemplete::getExcelLayout()
{
	return mExcelLayout;
}
void ExcelTemplete::sortLine()
{
	mExcelLayout->sortLines(mExcelLayout->mRowLines);
	mExcelLayout->sortLines(mExcelLayout->mColLines);
}
ViewObj* ExcelTemplete::getVirtulExcel()
{
	return mVirtulExcel;
}
void ExcelTemplete::setFocusOnSelf()
{
	ViewObj* firstContainer = mLayoutRowObjList->at(0);
	firstContainer->setFocus(true);
}
void ExcelTemplete::getWillShowObjList(QList<ViewObj *> * container)
{
	if (!mLayoutRowObjList)
	{
		return;
	}
	int size = mLayoutRowObjList->size();
	ViewObj * tempView = getRowObj(0);
	BaseViewParam * viewParam = tempView->getViewParam();
	viewParam->setSelfW(mExcelLayout->mColLines->at(mExcelLayout->mColLines->size() - 1)->getPosition() - mExcelLayout->mColLines->at(0)->getPosition());
	//获取普通行高。因为表格没有嵌套表格
	ViewObj* tempPage = mExcelLayout->mCells->at(0)->getViewOwner();
	ViewObj* txtLayer = (ViewObj*)tempPage->findChildrenByID(ConstantsID::ContainerId::EXCEL_TXT_LAYER, false);
	ViewObj* fistRow = (ViewObj*)txtLayer->getChildrenMgr()->getChild(0);
	int rowHeight = fistRow->getViewParam()->getSelfH();
	int rowNum;
	int totaolH = mExcelLayout->mRowLines->at(mExcelLayout->mRowLines->size() - 1)->getPosition() - mExcelLayout->mRowLines->at(0)->getPosition();
	viewParam->setSelfH(totaolH);
	rowNum = totaolH / rowHeight;
	for (int m = 0; m < rowNum; m++)
	{
		container->append(tempView);
	}
}
ViewObj* ExcelTemplete::getPageLayer()
{
	ViewObj * txtLayer = NULL;;
	ViewObj * window = mWriterMgr->getRootView()->getWindow();
	txtLayer = (ViewObj *)window->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false)
		->findChildrenByID(ConstantsID::ContainerId::NORMAL_PAGE, false)->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
		->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
	return txtLayer;
}
bool ExcelTemplete::getIsPressOnExcel()
{
	return mExcelInputMouseManager->getIsPreeOnExcel();
}
void ExcelTemplete::setIsPressOnExcel(bool isPressOnExcel)
{
	mExcelInputMouseManager->setIsPressOnExcel(isPressOnExcel);
}
void ExcelTemplete::setMouse(BaseTempleteMouse * mouse)
{
	if (!mInputMouse)
	{
		mInputMouse = mouse;
	}
	if (!mExcelInputMouseManager)
	{
		mExcelInputMouseManager = (ExcelMouseInputManager*)mouse;
	}
}
bool ExcelTemplete::setDrawTxtParam(StrContentData* focusData)
{
	//StrContentData* preData = new StrContentData(NULL, NULL);
	//preData->refreshTxtParamBySourceParam(focusData, false);
	bool isSuccess = true;
	int successIndex = 0;
	for (successIndex; successIndex < mExcelLayout->mCells->size(); successIndex++)
	{
		if (!mExcelLayout->mCells->at(successIndex)->mRootTemplete->setDrawTxtParam(focusData))
		{
			isSuccess = false;
			break;
		}
	}
	/*if (!isSuccess)
	{
		for (int i = 0; i <= successIndex; i++)
		{
			mExcelLayout->mCells->at(i)->mRootTemplete->setDrawTxtParam(preData);
		}
	}
	delete preData;
	preData = NULL;*/
	addExcelTempelateToOperate(this);
	return isSuccess;
}
bool ExcelTemplete::setExcelTemplateVerticalType(QList<int> positionList, QList<ExcelCellData *>* changeCells)
{
	if ((!changeCells) || changeCells->size() == 0)
	{
		QMessageBox::information(NULL, QString("表格设置属性报错"), QString("ExcelTemplete set vertical layout error"));
		return false;
	}
	bool isSetSuccess = false;
	ExcelCellData* tempCellData = NULL;
	int positionTemp;
	QList<ExcelTemplete *> excelTempList;
	for (int i = 0; i < changeCells->size(); i++)
	{
		positionTemp = positionList.at(i);
		tempCellData = changeCells->at(i);
		if (positionTemp == tempCellData->getVerticalPosition())
		{
			continue;
		}
		BaseContainer * excelTemp = tempCellData->getViewOwner()->findTempFromParent(ConstantsID::ContainerId::EXCEL_TEMPLETE, true);
		if (excelTemp)
		{
			excelTempList.push_back((ExcelTemplete*)excelTemp);
		}
		tempCellData->setVerticalPosition(positionTemp);
		isSetSuccess = true;
	}
	if (isSetSuccess)
	{
		int temptSize = excelTempList.size();
		for (int t = 0; t < temptSize; t++)
		{
			excelTempList.at(t)->requestLayout();
		}
	}
	return isSetSuccess;
}
int ExcelTemplete::getVerticalLayoutType()
{
	if ((!mExcelLayout) || (!mExcelLayout->mCells) || mExcelLayout->mCells->size() == 0)
	{
		QMessageBox::information(NULL, QString("表格设置属性报错"), QString("ExcelTemplete set vertical layout error"));
		return BaseConstants::Direction::MIDDLE;
	}
	ExcelCellData* tempCellData = mExcelLayout->mCells->at(0);
	return tempCellData->getVerticalPosition();
}
void ExcelTemplete::getCellPageRootTemplate(QList<BaseContainer*>& cellRootContainerList)
{
	for (int i = 0; i < mExcelLayout->mCells->size(); i++)
	{
		cellRootContainerList.push_back(mExcelLayout->mCells->at(i)->mRootTemplete);
	}
}
int ExcelTemplete::getWholeRowCrossPageH(int curH)
{
	return -1;
}
ViewObj * ExcelTemplete::getPressPage()
{
	if (mExcelInputMouseManager)
	{
		return mExcelInputMouseManager->getPressPage();
	}
	return NULL;
}
void ExcelTemplete::setPressPage(ViewObj * page)
{
	if (mExcelInputMouseManager)
	{
		mExcelInputMouseManager->setPressPage(page);
	}
}
void ExcelTemplete::setWriteManager(WriterManager* writeManger, bool isCircle)
{
	BaseContainer::setWriteManager(writeManger,false);
	if (isCircle)
	{
		if (mExcelLayout)
		{
			mExcelLayout->setWriteManager(writeManger, isCircle);
		}
	}
}