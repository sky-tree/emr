#include "ExcelMouseInputManager.h"
#include "BaseViewParam.h"
#include "ConstantsID.h"
#include "UIConstants.h"
#include "qdebug.h"
#include "Cursor.h"
#include "exception"
#include "FlagParam.h"
#include "BaseMath.h"
#include "OperateMgr.h"
#include "ExcelTemplete.h"
#include "RowViewParam.h"
#include "LineMoveAction.h"
#include "ExcelCellChangeLineAction.h"
#include "ExcelRelayoutAction.h"
//#include "ObjMouseManager.h"
//#include "ExcelCellPageMouse.h"
using namespace std;
ExcelMouseInputManager::ExcelMouseInputManager(ExcelTemplete* templeteOwner) :BaseTempleteMouse(templeteOwner, ConstantsID::ClassId::EXCEL_MOUSE)
, mOwnerViewLayout(NULL)
, mSelectLineObj(NULL)
, mTemptViewObjList(NULL)
, mInputType(InputType::Nomal)
, mViewOwner(NULL)
, mPressPage(NULL)
{
	mOwnerExcelTemplete = templeteOwner;
	ExcelLayout *  layout = templeteOwner->getExcelLayout();
	if (layout)
	{
		mOwnerViewLayout = mOwnerExcelTemplete->getExcelLayout();
		mViewOwner = mOwnerExcelTemplete->getVirtulExcel();
	}
	else
	{
	}
	mSelectLineObj = new SelectView();
	mIsPressOnExcel = false;
	mTemptViewObjList = new QList<ViewObj *>();
}


ExcelMouseInputManager::~ExcelMouseInputManager()
{
	if (mSelectLineObj)
	{
		delete mSelectLineObj;
		mSelectLineObj = NULL;
	}
	if (mOwnerViewLayout)
	{
		mOwnerViewLayout = NULL;
	}
	if (mTemptViewObjList)
	{
		mTemptViewObjList->clear();
		delete mTemptViewObjList;
		mTemptViewObjList = NULL;
	}
	if (mViewOwner)
	{
		mViewOwner = NULL;
	}
	if (mPressPage)
	{
		mPressPage = NULL;
	}
}
void ExcelMouseInputManager::init()
{
	mOwnerViewLayout = mOwnerExcelTemplete->getExcelLayout();
	mViewOwner = mOwnerExcelTemplete->getVirtulExcel();
}
int ExcelMouseInputManager::mousePress(int type, int x, int y, BaseElement * obj)
{
	if (!mOwnerExcelTemplete->mCanDragExcel)
	{
		return 0;
	}
	int returnType = BaseConstants::NONE;
	if (!BaseMath::isInsideTotalArea(x, y, mViewOwner))
	{
		mIsPressOnExcel = false;
		mIsPressMove = false;
		isInOneCell = false;
		return returnType;
	}

	if (!canMousePressInTitleBottom())
	{
		return 0;
	}
	ViewObj* pressView = (ViewObj*)obj;
	ViewObj*  txtWindow = pressView->getParentViewById(ConstantsID::ContainerId::TXT_WIN_VIEW);
	mIsPressOnExcel = true;
	OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
	if (mInputType == InputType::Nomal)
	{
		//operateMgr->clearChooseList(true);
		Line * selectLine = mSelectLineObj->getSelectLine();
		if (selectLine)
		{
			txtWindow->changeStatus(BaseConstants::ObjStatus::EXCEL, false);
			if (mSelectLineObj->mSideType == SelectView::Side::Left)
			{
				mOwnerExcelTemplete->setFocusOnSelf();
				mSelectLineObj->mSelectObj->setMaskColor(UIConstants::MyColor::CHOOSEN_COLOR);
				operateMgr->addToChooseList(mSelectLineObj->mSelectObj);
				//记录按下事件，方便接下来有坑能拖动
				mMouseDown.setX(x);
				mMouseDown.setY(y);
				isPressOnCellLeft = true;
			}
			else if (mSelectLineObj->mSideType == SelectView::Side::Right)
			{

				QList<BaseContainer *> *  chooseList = operateMgr->getChooseList();
				bool isSelectOnCellLOrR = isSelectOnChooseCellLeftOrRight(chooseList, mOwnerViewLayout->mTemptCellDataMergeReult, y);
				if (isSelectOnCellLOrR)
				{
					bool canMooveSelectLOrR = isAbleToMooveCellLOrR(mOwnerViewLayout->mTemptCellDataMergeReult, selectLine);
					if (canMooveSelectLOrR)
					{
						//拖动选中单元格
						mSelectLineDragPosition = mSelectLineObj->mSelectLine->getPosition();
						setDragLineShow(true);
						mInputType = InputType::SelectOnCellLOrR;
					}
				}
				else
				{
					//普通拖动线条
					mSelectLineDragPosition = mSelectLineObj->mSelectLine->getPosition();
					setDragLineShow(true);
					mInputType = InputType::SelectLine;
				}
				mOwnerExcelTemplete->setFocusOnSelf();
			}
			else
			{
				operateMgr->clearChooseList(true);
				//普通拖动线条
				mSelectLineDragPosition = mSelectLineObj->mSelectLine->getPosition();
				setDragLineShow(true);
				mInputType = InputType::SelectLine;
				mOwnerExcelTemplete->setFocusOnSelf();
			}
			setPressPage(NULL);
			returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		}
		else
		{
			//只有在阅读模式下才会走这里
			ViewObj * cell = mOwnerViewLayout->seekCell(x, y);
			if (!cell)
			{
				setPressPage(NULL);
				mSelectLineObj->init();
				Cursor::slotArrow();
			}
			else
			{
				setPressPage(cell);
				Cursor::slotArrow();
				mSelectLineObj->SetSelect(cell, SelectView::Side::Middle);
				mSelectLineObj->mSelectObj->changeStatus(BaseConstants::ObjStatus::FREE, false);
			}
			//记录按下事件，方便接下来有可能拖动
			operateMgr->clearChooseList(true);
			mMouseDown.setX(x);
			mMouseDown.setY(y);
			isPressOnCellLeft = false;
			isInOneCell = true;
			mInputType = InputType::Nomal;
			txtWindow->changeStatus(BaseConstants::ObjStatus::FREE, false);
		}
		return returnType;
	}

	setPressPage(NULL);
	return returnType;
}
int ExcelMouseInputManager::mouseRelease(int type, int x, int y, BaseElement * obj)
{
	if (!mOwnerExcelTemplete->mCanDragExcel)
	{
		return 0;
	}
	
	if (mIsPressOnExcel)
	{
	}
	else
	{
		OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
		operateMgr->clearChooseList(true);
		mInputType = InputType::Nomal;
		setDragLineShow(false);
		return 1;
	}
	if (!canMousePressInTitleBottom())
	{
		OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
		BaseContainer* focus = operateMgr->getPressStart()->getFocusObj();
		BaseContainer * excelLayer = focus->findViewFromParent(ConstantsID::ContainerId::EXCEL_TXT_LAYER, true);
		return 0;
	}
	int returnType = BaseConstants::NONE;

	if (mInputType == InputType::SelectLine)
	{
		handleReleaseDragLine(type, x, y);
		mInputType = InputType::Nomal;
		setPressPage(NULL);
		returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	else if (mInputType == InputType::SelectOnCellLOrR)
	{
		handleReleaseDragOnSelectCellLOrRNew(type, x, y);
		mInputType = InputType::Nomal;
		setPressPage(NULL);
		returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	else
	{
		if (caculateMarquee(type, x, y))
		{
			setPressPage(NULL);
			returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		}
		else
		{
			OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
			BaseContainer* focus = operateMgr->getPressStart()->getFocusObj();
			BaseContainer * excelLayer = focus->findViewFromParent(ConstantsID::ContainerId::EXCEL_TXT_LAYER, true);
			if (!excelLayer)
			{
				int a = 0;
			}
		}
		mInputType = InputType::Nomal;
	}
	Cursor::slotIbeam();
	mIsPressOnExcel = false;
	mIsPressMove = false;
	isInOneCell = false;
	if (returnType == 0)
	{
		OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
		BaseContainer* focus = operateMgr->getPressStart()->getFocusObj();
		BaseContainer * excelLayer = focus->findViewFromParent(ConstantsID::ContainerId::EXCEL_TXT_LAYER, true);
		if (!excelLayer)
		{
			int a = 0;
		}
	}
	return returnType;
}
int ExcelMouseInputManager::mouseMove(int type, int x, int y, BaseElement * obj)
{
	if (!mOwnerExcelTemplete->mCanDragExcel)
	{
		return 0;
	}
	int returnType = BaseConstants::NONE;
	if (!BaseMath::isInsideTotalArea(x, y, mViewOwner))
	{
		Cursor::slotIbeam();
		return returnType;
	}
	if (!canMousePressInTitleBottom())
	{
		return 0;
	}
	if (mInputType == InputType::Nomal)
	{

		ViewObj * cell = mOwnerViewLayout->seekCell(x, y);
		if (!cell)
		{
			mSelectLineObj->init();
			Cursor::slotArrow();
			return returnType;
		}
		ExcelCellData * cellData = mOwnerViewLayout->getCellDataFromCell(cell);

		if (cellData)
		{
			//先判断上下线附近，再判断左右线附近
			int tempt = 0;
			tempt = cellData->mTop->getPosition();
			if (BaseMath::isValueInArea(y, tempt, SLOT_CHANGE_DISTANCE))
			{
				returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
				Cursor::slotVSplit();
				mSelectLineObj->SetSelect(cell, SelectView::Side::Top);
				return returnType;
			}
			tempt = cellData->mBottom->getPosition();
			if (BaseMath::isValueInArea(y, tempt - SLOT_CHANGE_DISTANCE, SLOT_CHANGE_DISTANCE))
			{
				returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
				Cursor::slotVSplit();
				mSelectLineObj->SetSelect(cell, SelectView::Side::Bottom);
				return returnType;

			}
			tempt = cellData->mLeft->getPosition();
			if (BaseMath::isValueInArea(x, tempt, SLOT_CHANGE_DISTANCE + 2))
			{
				returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
				Cursor::slotSizeAll();
				mSelectLineObj->SetSelect(cell, SelectView::Side::Left);
				return returnType;

			}
			tempt = cellData->mRight->getPosition();
			if (BaseMath::isValueInArea(x, tempt - SLOT_CHANGE_DISTANCE, SLOT_CHANGE_DISTANCE))
			{
				returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
				Cursor::slotHSplit();
				mSelectLineObj->SetSelect(cell, SelectView::Side::Right);
				return returnType;

			}
			Cursor::slotArrow();
			mSelectLineObj->SetSelect(cell, SelectView::Side::Middle);
			return 0;
		}
	}

	return returnType;
}
int ExcelMouseInputManager::mouseDrag(int type, int x, int y, BaseElement * obj)
{
	if (!mOwnerExcelTemplete->mCanDragExcel)
	{
		setPressPage(NULL);
		return 0;
	}
	if (!mIsPressOnExcel)
	{
		setPressPage(NULL);
		return BaseConstants::NONE;
	}
	mIsPressMove = true;
	int returnType = BaseConstants::NONE;
	//保证不能拖动最左边的线和最上边的线
	if (x - mOwnerViewLayout->mViewParam->getInsideLeft() < 0 || y - mOwnerViewLayout->mViewParam->getInsideTop() < 0)
	{
		setPressPage(NULL);
		return returnType;
	}
	if (!canMousePressInTitleBottom())
	{
		return 0;
	}
	if (mInputType == InputType::SelectLine)
	{
		handleDragLine(type, x, y);
		setPressPage(NULL);
		returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		return returnType;
	}
	else if (mInputType == InputType::SelectOnCellLOrR)
	{
		handleDragOnSelectCellLOrR(type, x, y);
		setPressPage(NULL);
		returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		return returnType;
	}
	else
	{
		if (caculateMarquee(type, x, y))
		{
			setPressPage(NULL);
			returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		}
		else
		{
			OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
			BaseContainer* focus = operateMgr->getPressStart()->getFocusObj();
			BaseContainer * excelLayer = focus->findViewFromParent(ConstantsID::ContainerId::EXCEL_TXT_LAYER, true);
			if (!excelLayer)
			{
				int a = 0;
			}
		}
	}

	if (returnType == 0)
	{
		OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
		BaseContainer* focus = operateMgr->getPressStart()->getFocusObj();
		BaseContainer * excelLayer = focus->findViewFromParent(ConstantsID::ContainerId::EXCEL_TXT_LAYER, true);
		if (!excelLayer)
		{
			int a = 0;
		}
	}
	return returnType;
}
int ExcelMouseInputManager::mouseDoublePress(int type, int x, int y, BaseElement * obj)
{

	return mousePress(type, x, y, obj);
}
int ExcelMouseInputManager::mouseScroll(int scroll, BaseElement * obj)
{
	return 0;
}
bool ExcelMouseInputManager::isSelectOnChooseCellLeftOrRight(QList<BaseContainer *> *  chooseList, ExcelCellData* resultCell, int y)
{
	if (!resultCell)
	{
		return false;
	}
	resultCell->initData();
	if (!ExcelTemplete::isMyCell(chooseList, mOwnerExcelTemplete))
	{
		return false;
	}
	int chooseListSize = chooseList->size();
	if (chooseListSize == 0)
	{
		return false;
	}
	/*else if (chooseListSize == 1)
	{
	ViewObj * tempObj = (ViewObj *)chooseList->at(0);
	ExcelCellData * cellData = ExcelLayout::getCellDataFromCell(tempObj);
	resultCell->initFromOther(cellData);
	return true;
	}*/
	if (mTemptViewObjList->size() > 0)
	{
		mTemptViewObjList->clear();
	}
	ViewObj * tempObj = NULL;
	for (int i = 0; i < chooseListSize; i++)
	{
		tempObj = (ViewObj *)chooseList->at(i);
		mTemptViewObjList->push_back(tempObj);
	}
	mOwnerViewLayout->getCellDatasFormCellObjs(mTemptViewObjList, mOwnerViewLayout->mCellsTempt1);
	if (mOwnerViewLayout->mCellsTempt1->size() == 1)
	{
		resultCell->initFromOther(mOwnerViewLayout->mCellsTempt1->at(0));
	}
	else
	{
		mOwnerViewLayout->canMerge(mOwnerViewLayout->mCellsTempt1, resultCell);
	}

	if (resultCell->mLeft)
	{
		Line * selectLine = mSelectLineObj->getSelectLine();
		if (selectLine == resultCell->mRight || selectLine == resultCell->mLeft)
		{
			if (y >= resultCell->mTop->getPosition() && y <= resultCell->mBottom->getPosition())
			{
				//bool haveSeeked = findSelectCellLAndRCellData(cellData);
				return true;
			}
			else
			{
				return false;
			}

		}
		return false;
	}
	else
	{
		return false;
	}
}
bool ExcelMouseInputManager::isAbleToMooveCellLOrR(ExcelCellData * chooseCellData, Line * selectedLine)
{
	if (selectedLine == mOwnerViewLayout->mColLines->at(0) || selectedLine == mOwnerViewLayout->mColLines->at(mOwnerViewLayout->mColLines->size() - 1))
	{
		return false;
	}
	if (mOwnerViewLayout->mCellsTempt1->size())
	{
		mOwnerViewLayout->mCellsTempt1->clear();
	}
	int resultTopSize = 0;
	int resultBottomSize = 0;
	if (chooseCellData->mLeft == selectedLine)
	{
		mOwnerViewLayout->seekCellsHaveLine(BaseConstants::Direction::RIGHT, chooseCellData->mLeft, mOwnerViewLayout->mCells, mOwnerViewLayout->mCellsTempt1);
		if (mOwnerViewLayout->mCellsTempt1->size() == 0)
		{
			return false;
		}
	}
	else if (chooseCellData->mRight == selectedLine)
	{
		mOwnerViewLayout->seekCellsHaveLine(BaseConstants::Direction::LEFT, chooseCellData->mRight, mOwnerViewLayout->mCells, mOwnerViewLayout->mCellsTempt1);
		if (mOwnerViewLayout->mCellsTempt1->size() == 0)
		{
			return false;
		}

	}
	if (mOwnerViewLayout->mCellsTempt2->size())
	{
		mOwnerViewLayout->mCellsTempt2->clear();
	}
	mOwnerViewLayout->seekCellsHaveLine(BaseConstants::Direction::UP, chooseCellData->mTop, mOwnerViewLayout->mCellsTempt1, mOwnerViewLayout->mCellsTempt2);
	resultTopSize = mOwnerViewLayout->mCellsTempt2->size();
	if (mOwnerViewLayout->mCellsTempt2->size())
	{
		mOwnerViewLayout->mCellsTempt2->clear();
	}
	mOwnerViewLayout->seekCellsHaveLine(BaseConstants::Direction::DOWN, chooseCellData->mBottom, mOwnerViewLayout->mCellsTempt1, mOwnerViewLayout->mCellsTempt2);
	resultBottomSize = mOwnerViewLayout->mCellsTempt2->size();
	bool returnValue = false;
	if (resultTopSize == 1)
	{
		if (resultBottomSize == 1)
		{
			//最后去返回。
			returnValue = true;

		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	//mCellsTempt2 mCellsTempt3 已经被使用。
	if (mOwnerViewLayout->mCellsTempt2->size())
	{
		mOwnerViewLayout->mCellsTempt2->clear();
	}
	int chooseSize = mTemptViewObjList->size();
	ViewObj * tempObj;
	ExcelCellData* tempCellData;
	for (int i = 0; i < chooseSize; i++)
	{
		tempObj = mTemptViewObjList->at(i);
		BaseContentData * contentParam = tempObj->getContentData();
		tempCellData = (ExcelCellData *)contentParam;
		mOwnerViewLayout->mCellsTempt2->push_back(tempCellData);

	}
	//计算相邻的cellData
	int tempViewListSize = mOwnerViewLayout->mCellsTempt2->size();
	int minTop = INT_MAX;
	int maxBottom = -1;
	int tempTop;
	int tempBottom;
	for (int i = 0; i < tempViewListSize; i++)
	{
		tempCellData = mOwnerViewLayout->mCellsTempt2->at(i);
		tempTop = tempCellData->mTop->getPosition();
		if (tempTop<minTop)
		{
			minTop = tempTop;
		}
		tempBottom = tempCellData->mBottom->getPosition();
		if (tempBottom>maxBottom)
		{
			maxBottom = tempBottom;
		}
	}
	if (mOwnerViewLayout->mCellsTempt4->size())
	{
		mOwnerViewLayout->mCellsTempt4->clear();
	}
	int sizeOfTemp1List = mOwnerViewLayout->mCellsTempt1->size();
	for (int i = 0; i < sizeOfTemp1List; i++)
	{
		tempCellData = mOwnerViewLayout->mCellsTempt1->at(i);
		if (tempCellData->mTop->getPosition() >= minTop && tempCellData->mBottom->getPosition() <= maxBottom)
		{
			mOwnerViewLayout->mCellsTempt4->push_back(tempCellData);
		}
	}
	return true;
}
bool ExcelMouseInputManager::isClickOnSelectCellLeftOrRight(ViewObj * viewObj, int y)
{
	bool returnValue = false;
	if (mSelectLineObj->mSideType == SelectView::Side::Right)
	{
		Line * selectLine = mSelectLineObj->getSelectLine();
		BaseContentData * contentParam = viewObj->getContentData();
		ExcelCellData * cellData = NULL;
		try
		{
			cellData = (ExcelCellData *)contentParam;
			if (selectLine == cellData->mRight || selectLine == cellData->mLeft)
			{
				if (y >= cellData->mTop->getPosition() && y <= cellData->mBottom->getPosition())
				{
					//bool haveSeeked = findSelectCellLAndRCellData(cellData);
					if (findSelectCellLAndRCellData(cellData))
					{
						returnValue = true;
					}
				}

			}
		}
		catch (exception& e)
		{
			return false;
		}

	}
	return returnValue;
}
bool ExcelMouseInputManager::findSelectCellLAndRCellData(ExcelCellData * selectCellData)
{
	if (mOwnerViewLayout->mCellsTempt1->size())
	{
		mOwnerViewLayout->mCellsTempt1->clear();
	}
	mSelectLineLeftCellData = selectCellData;
	Line * selectLine = mSelectLineObj->getSelectLine();
	if (selectLine == selectCellData->mLeft)
	{
		mSelectLineLeftCellData = (ExcelCellData *)mSelectLineObj->mSelectObj->getContentData();
		mSelectLineRightLeftData = selectCellData;
		return true;
	}
	else if (selectLine == selectCellData->mRight)
	{
		mSelectLineLeftCellData = selectCellData;
		mOwnerViewLayout->seekCellsHaveLine(BaseConstants::Direction::LEFT, selectCellData->mRight, mOwnerViewLayout->mCells, mOwnerViewLayout->mCellsTempt1);
		if (mOwnerViewLayout->mCellsTempt1->size() == 0)
		{
			mSelectLineRightLeftData = NULL;
			return false;
		}
		if (mOwnerViewLayout->mCellsTempt2->size())
		{
			mOwnerViewLayout->mCellsTempt2->clear();
		}
		mOwnerViewLayout->seekCellsHaveLine(BaseConstants::Direction::UP, selectCellData->mTop, mOwnerViewLayout->mCellsTempt1, mOwnerViewLayout->mCellsTempt2);

		if (mOwnerViewLayout->mCellsTempt1->size())
		{
			mOwnerViewLayout->mCellsTempt1->clear();
		}
		mOwnerViewLayout->seekCellsHaveLine(BaseConstants::Direction::DOWN, selectCellData->mBottom, mOwnerViewLayout->mCellsTempt2, mOwnerViewLayout->mCellsTempt1);
		int size = mOwnerViewLayout->mCellsTempt1->size();
		//应该继续寻找bottom线相同  才行

		if (size == 0)
		{
			mSelectLineRightLeftData = NULL;
			return false;;
		}
		else if (size == 1)
		{
			ExcelCellData *temptdata = mOwnerViewLayout->mCellsTempt2->at(0);
			mSelectLineRightLeftData = temptdata;
			return true;
		}
		else if (size > 1)
		{
			mSelectLineRightLeftData = NULL;
			return false;
		}
	}
	return false;
}
void ExcelMouseInputManager::handleDragOnSelectCellLOrR(int type, int x, int y)
{
	if (mSelectLineObj->mSideType == SelectView::Side::Right)
	{
		Line * selectLine = mSelectLineObj->getSelectLine();
		int dragDistance = mSelectLineObj->mSelectLine->getPosition() - x;
		if (BaseMath::getAbs(dragDistance) > DRAG_CHANGE_MIN)
		{
			QList<ExcelCellData *>* cellsTemptList = NULL;
			if (dragDistance < 0)
			{
				if (mOwnerViewLayout->mTemptCellDataMergeReult->mLeft == selectLine)
				{
					cellsTemptList = mOwnerViewLayout->mCellsTempt2;
					/*int chooseSize = mOwnerViewLayout->mCellsTempt2->size();
					for (int i = 0; i < chooseSize; i++)
					{
					tempCellData = mOwnerViewLayout->mCellsTempt2->at(i);

					}*/
				}
				else
				{
					cellsTemptList = mOwnerViewLayout->mCellsTempt4;
				}
				//变窄后，单元格如果不能容纳最大的视图时，直接返回
				if (!mOwnerViewLayout->isAbleToSetX(BaseConstants::Direction::LEFT, x, cellsTemptList))
				{
					return;
				}
				ExcelCellData* tempCellData = cellsTemptList->at(0);
				int cellWidth = tempCellData->mRight->getPosition() - x;
				if (cellWidth < ExcelLayout::defaultCellW)
				{
					mSelectLineDragPosition = tempCellData->mRight->getPosition() - ExcelLayout::defaultCellW;
				}
				else
				{
					mSelectLineDragPosition = x;
				}
			}
			else
			{
				if (mOwnerViewLayout->mTemptCellDataMergeReult->mLeft == selectLine)
				{
					cellsTemptList = mOwnerViewLayout->mCellsTempt4;

				}
				else
				{
					cellsTemptList = mOwnerViewLayout->mCellsTempt2;
				}
				//变窄后，单元格如果不能容纳最大的视图时，直接返回
				if (!mOwnerViewLayout->isAbleToSetX(BaseConstants::Direction::RIGHT, x, cellsTemptList))
				{
					return;
				}
				ExcelCellData* tempCellData = cellsTemptList->at(0);
				int cellWidth = x - tempCellData->mLeft->getPosition();
				if (cellWidth < ExcelLayout::defaultCellW)
				{
					mSelectLineDragPosition = tempCellData->mLeft->getPosition() + ExcelLayout::defaultCellW;
				}
				else
				{
					mSelectLineDragPosition = x;
				}
			}
			//if (selectLine == mSelectCellData->mLeft){
			//if (dragDistance<0)
			//{
			//	if (mSelectLineRightLeftData){
			//		Line * nextLine = mSelectLineRightLeftData->mRight;
			//		//判断当前移动的位置，与下一条线之间的宽度 是否小于最小宽度
			//		int cellWidth = nextLine->getPosition() - x;
			//		if (cellWidth<ExcelLayout::defaultCellW){
			//			//dragDistance = nextLine->mPosition - mSelectObj->mSelectLine->mPosition - ExcelLayout::defaultCellW;
			//			mSelectLineDragPosition = nextLine->getPosition() - ExcelLayout::defaultCellW;
			//		}
			//		else{
			//			mSelectLineDragPosition = x;
			//		}
			//	}
			//	else{
			//		mSelectLineDragPosition = x;
			//	}
			//	
			//}
			//else
			//{
			//	//ExcelCellData * lastCellData = (ExcelCellData *)mSelectLineObj->mSelectObj->getContentParam();
			//	Line * lastLine = mSelectLineLeftCellData->mLeft;
			//	//判断当前移动的位置，与上一条线之间的宽度 是否小于最小宽度
			//	int cellWidth = x - lastLine->getPosition();
			//	if (cellWidth < ExcelLayout::defaultCellW){
			//		mSelectLineDragPosition = lastLine->getPosition() + ExcelLayout::defaultCellW;
			//	}
			//	else{
			//		mSelectLineDragPosition = x;
			//	}
			//}


		}
		else
		{
			mSelectLineDragPosition = mSelectLineObj->mSelectLine->getPosition();
		}
		setDragLineShow(true);
	}
}
void ExcelMouseInputManager::handleDragLine(int type, int x, int y)
{
	if (mSelectLineObj->mSideType == SelectView::Side::Right)
	{
		int dragDistance = mSelectLineObj->mSelectLine->getPosition() - x;
		if (BaseMath::getAbs(dragDistance) > DRAG_CHANGE_MIN)
		{
			//mSelectLineDragDistance = dragDistance;
			if (dragDistance < 0)
			{
				if (mOwnerViewLayout->mCellsTempt1->size())
				{
					mOwnerViewLayout->mCellsTempt1->clear();
				}
				mOwnerViewLayout->seekCellsHaveLine(BaseConstants::Direction::LEFT, mSelectLineObj->mSelectLine, mOwnerViewLayout->mCells, mOwnerViewLayout->mCellsTempt1);
				//变窄后，单元格如果不能容纳最大的视图时，直接返回
				if (!mOwnerViewLayout->isAbleToSetX(BaseConstants::Direction::LEFT, x, mOwnerViewLayout->mCellsTempt1))
				{
					return;
				}

				Line* nextLine = mOwnerViewLayout->getLineNextOrLastValidLine(mOwnerViewLayout->mCellsTempt1, false, false);
				if (nextLine)
				{
					//判断当前移动的位置，与下一条线之间的宽度 是否小于最小宽度
					int cellWidth = nextLine->getPosition() - x;
					if (cellWidth < ExcelLayout::defaultCellW)
					{
						//dragDistance = nextLine->mPosition - mSelectObj->mSelectLine->mPosition - ExcelLayout::defaultCellW;
						mSelectLineDragPosition = nextLine->getPosition() - ExcelLayout::defaultCellW;
					}
					else
					{
						mSelectLineDragPosition = x;
					}
				}
				else
				{
					//mSelectLineDragPosition = mSelectLineObj->mSelectLine->getPosition();
					//不能大于当前所在行的宽度
					ViewObj * excelViewParent = mOwnerExcelTemplete->getStartRow();
					if (!excelViewParent)
					{
						return;
					}
					RowViewParam *  viewParam = (RowViewParam*)excelViewParent->getViewParam();
					int rowWidth = viewParam->getInsideW();
					mSelectLineDragPosition = x;
					if (mSelectLineDragPosition > rowWidth)
					{
						mSelectLineDragPosition = rowWidth;
					}

				}

			}
			else
			{
				//第一条竖线不能左拉动
				if (mOwnerViewLayout->getIndexOfLine(mSelectLineObj->mSelectLine, false) == 0)
				{
					mSelectLineDragPosition = mSelectLineObj->mSelectLine->getPosition();
					return;
				}
				if (mOwnerViewLayout->mCellsTempt1->size())
				{
					mOwnerViewLayout->mCellsTempt1->clear();
				}
				mOwnerViewLayout->seekCellsHaveLine(BaseConstants::Direction::RIGHT, mSelectLineObj->mSelectLine, mOwnerViewLayout->mCells, mOwnerViewLayout->mCellsTempt1);
				//变窄后，单元格如果不能容纳最大的视图时，直接返回
				if (!mOwnerViewLayout->isAbleToSetX(BaseConstants::Direction::RIGHT, x, mOwnerViewLayout->mCellsTempt1))
				{
					return;
				}

				Line* lastLine = mOwnerViewLayout->getLineNextOrLastValidLine(mOwnerViewLayout->mCellsTempt1, false, true);
				if (lastLine)
				{
					//判断当前移动的位置，与上一条线之间的宽度 是否小于最小宽度
					int cellWidth = x - lastLine->getPosition();
					if (cellWidth < ExcelLayout::defaultCellW)
					{
						mSelectLineDragPosition = lastLine->getPosition() + ExcelLayout::defaultCellW;
					}
					else
					{
						mSelectLineDragPosition = x;
					}
				}
				else
				{
					mSelectLineDragPosition = mSelectLineObj->mSelectLine->getPosition();
				}
			}
		}
		else
		{
			mSelectLineDragPosition = mSelectLineObj->mSelectLine->getPosition();
			//mSelectLineDragDistance = 0;
		}
	}
	else if (mSelectLineObj->mSideType == SelectView::Side::Top || mSelectLineObj->mSideType == SelectView::Side::Bottom)
	{
		//第一条横线不能拉动
		if (mOwnerViewLayout->getIndexOfLine(mSelectLineObj->mSelectLine, true) == 0)
		{
			mSelectLineDragPosition = mSelectLineObj->mSelectLine->getPosition();
			return;
		}
		int dragDistance = mSelectLineObj->mSelectLine->getPosition() - y;
		if (BaseMath::getAbs(dragDistance) >= DRAG_CHANGE_MIN)
		{
			if (dragDistance < 0)
			{
				mSelectLineDragPosition = y;
				//Line* nextLine = mOwnerViewLayout->getLineNextOrLastLine(mSelectObj->mSelectLine, true, false);
				//if (nextLine){
				//	//判断当前移动的位置，与下一条线之间的宽度 是否小于最小宽度
				//	int cellHight = nextLine->mPosition - y;
				//	if (cellHight<ExcelLayout::defaultCellH){
				//		//dragDistance = nextLine->mPosition - mSelectObj->mSelectLine->mPosition - ExcelLayout::defaultCellW;
				//		mSelectLineDragPosition = nextLine->mPosition - ExcelLayout::defaultCellH;
				//	}
				//	else{
				//		mSelectLineDragPosition = y;
				//	}
				//}
				//else{
				//	mSelectLineDragPosition = mSelectObj->mSelectLine->mPosition;
				//}
			}
			else
			{
				//往上拉，不能小于最小高度
				if (mOwnerViewLayout->mCellsTempt1->size())
				{
					mOwnerViewLayout->mCellsTempt1->clear();
				}
				mOwnerViewLayout->seekCellsHaveLine(BaseConstants::Direction::DOWN, mSelectLineObj->mSelectLine, mOwnerViewLayout->mCells, mOwnerViewLayout->mCellsTempt1);
				Line* lastLine = mOwnerViewLayout->getLineNextOrLastValidLine(mOwnerViewLayout->mCellsTempt1, true, true);
				if (lastLine)
				{
					int cellHight = y - lastLine->getPosition();
					if (cellHight < ExcelLayout::defaultCellH)
					{
						mSelectLineDragPosition = lastLine->getPosition() + ExcelLayout::defaultCellH;
					}
					else
					{
						mSelectLineDragPosition = y;
					}
				}
				else
				{
					mSelectLineDragPosition = mSelectLineObj->mSelectLine->getPosition();
				}
			}
		}
		else
		{
			mSelectLineDragPosition = mSelectLineObj->mSelectLine->getPosition();
		}
	}
	setDragLineShow(true);
	/*else if (mSelectObj->mSideType == SelectView::Side::Bottom){
		int dragDistance = mSelectObj->mSelectLine->mPosition - y;
		if (BaseMath::getAbs(dragDistance) >= DRAG_CHANGE_MIN){
		mSelectLineDragDistance = dragDistance;
		}
		else{
		mSelectLineDragPosition = mSelectObj->mSelectLine->mPosition;
		}
		}*/

}
void ExcelMouseInputManager::handleReleaseDragLine(int type, int x, int y)
{
	// 如果没有移动
	if (mSelectLineDragPosition == mSelectLineObj->mSelectLine->getPosition())
	{
		setDragLineShow(false);
		return;
	}

	if (mSelectLineObj->mSideType == SelectView::Side::Right)
	{
		OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
		operateMgr->createNewAction();

		//撤销 前进


		Line * nearestLine = mOwnerViewLayout->findNearestLine(mOwnerViewLayout->mColLines, mSelectLineDragPosition, 3);
		if (nearestLine)
		{
			//换掉所有的右边线
			if (mOwnerViewLayout->mCellsTempt1->size())
			{
				mOwnerViewLayout->mCellsTempt1->clear();
			}
			mOwnerViewLayout->seekCellsHaveLine(BaseConstants::Direction::RIGHT, mSelectLineObj->mSelectLine, mOwnerViewLayout->mCells, mOwnerViewLayout->mCellsTempt1);
			bool isSucceed = mOwnerViewLayout->changeCellsPositionLine(mOwnerViewLayout->mCellsTempt1, nearestLine, BaseConstants::Direction::RIGHT);
			if (isSucceed)
			{
				ExcelCellChangeLineAction *moveLineAction = (ExcelCellChangeLineAction *)operateMgr->getLocalActionLastAction();
				moveLineAction->setRelayout(true);
			}
			if (mOwnerViewLayout->mCellsTempt1->size())
			{
				mOwnerViewLayout->mCellsTempt1->clear();
			}
			mOwnerViewLayout->seekCellsHaveLine(BaseConstants::Direction::LEFT, mSelectLineObj->mSelectLine, mOwnerViewLayout->mCells, mOwnerViewLayout->mCellsTempt1);
			isSucceed = mOwnerViewLayout->changeCellsPositionLine(mOwnerViewLayout->mCellsTempt1, nearestLine, BaseConstants::Direction::LEFT);
			if (isSucceed)
			{
				ExcelCellChangeLineAction *moveLineAction = (ExcelCellChangeLineAction *)operateMgr->getLocalActionLastAction();
				moveLineAction->setRelayout(true);
			}
		}
		else
		{
			int prePosition = mSelectLineObj->mSelectLine->getPosition();
			int preDragPosition = mSelectLineObj->mSelectLine->getDragPosition();
			mSelectLineObj->mSelectLine->setAllPosition(mSelectLineDragPosition);
			LineMoveAction *moveLineAction = new LineMoveAction(operateMgr->getCurStepMgr(), mOwnerExcelTemplete);
			moveLineAction->addMoveLine(mSelectLineObj->mSelectLine, prePosition, preDragPosition, mSelectLineDragPosition, mSelectLineDragPosition);
			moveLineAction->setRelayout(true);
			operateMgr->addToLocalAction(moveLineAction);
		}
		mOwnerViewLayout->removeUseLessColLine();
		mOwnerExcelTemplete->requestLayout();
		//winData->refreshFlagPlace(winData->getPressEnd());
	}
	else if (mSelectLineObj->mSideType == SelectView::Side::Top || mSelectLineObj->mSideType == SelectView::Side::Bottom)
	{

		int minHieght = mOwnerViewLayout->getRowCellMinHieght(mSelectLineObj->mSelectLine);
		if (mSelectLineDragPosition <= minHieght)
		{
			mSelectLineDragPosition = minHieght;
		}
		OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
		operateMgr->createNewAction();
		int start = mOwnerViewLayout->getIndexOfLine(mSelectLineObj->mSelectLine, true);
		int end = mOwnerViewLayout->mRowLines->size();
		mOwnerViewLayout->moveSomeLines(mOwnerViewLayout->mRowLines, start, end, mSelectLineDragPosition - mSelectLineObj->mSelectLine->getPosition());
		LineMoveAction *moveLineAction = (LineMoveAction *)operateMgr->getLocalActionLastAction();
		if (moveLineAction)
		{
			moveLineAction->setRelayout(true);
		}
		mOwnerExcelTemplete->requestLayout();
		//winData->refreshFlagPlace(winData->getPressEnd());
	}
	setDragLineShow(false);
	mOwnerExcelTemplete->checkIsNeedUndo();
}
void ExcelMouseInputManager::handleReleaseDragOnSelectCellLOrR(int type, int x, int y)
{
	// 如果没有移动
	if (mSelectLineDragPosition == mSelectLineObj->mSelectLine->getPosition())
	{
		setDragLineShow(false);
		return;
	}
	if (mSelectLineObj->mSideType == SelectView::Side::Right)
	{
		bool isTopAndBottomH = false;
		if (mSelectLineLeftCellData->mTop == mSelectLineRightLeftData->mTop&&mSelectLineLeftCellData->mBottom == mSelectLineRightLeftData->mBottom)
		{
			isTopAndBottomH = true;
		}
		if (isTopAndBottomH)
		{
			OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
			operateMgr->createNewAction();

			Line * nearestLine = mOwnerViewLayout->findNearestLine(mOwnerViewLayout->mColLines, mSelectLineDragPosition, 3);
			if (!nearestLine)
			{
				nearestLine = new Line(mSelectLineDragPosition);
				nearestLine->setDragPosition(mSelectLineDragPosition);
				mOwnerViewLayout->insertLine(nearestLine, mOwnerViewLayout->mColLines);
			}
			ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mOwnerExcelTemplete);
			operateMgr->addToLocalAction(changeLineAction);
			changeLineAction->addChangeLine(mSelectLineLeftCellData, BaseConstants::Direction::RIGHT, mSelectLineLeftCellData->mRight, nearestLine);
			mSelectLineLeftCellData->mRight = nearestLine;
			if (mSelectLineRightLeftData)
			{
				changeLineAction->addChangeLine(mSelectLineRightLeftData, BaseConstants::Direction::LEFT, mSelectLineRightLeftData->mLeft, nearestLine);
				mSelectLineRightLeftData->mLeft = nearestLine;
			}
			mOwnerViewLayout->removeUseLessColLine();
			ExcelRelayoutAction* relayoutAction = new ExcelRelayoutAction(operateMgr->getCurStepMgr(), mOwnerExcelTemplete);
			operateMgr->addToLocalActionAt(relayoutAction, 1);
			mOwnerExcelTemplete->requestLayout();
		}

		setDragLineShow(false);
		mOwnerExcelTemplete->checkIsNeedUndo();
	}
}
void ExcelMouseInputManager::handleReleaseDragOnSelectCellLOrRNew(int type, int x, int y)
{
	// 如果没有移动
	if (mSelectLineDragPosition == mSelectLineObj->mSelectLine->getPosition())
	{
		setDragLineShow(false);
		return;
	}
	if (mSelectLineObj->mSideType == SelectView::Side::Right)
	{
		int besideCellSize = mOwnerViewLayout->mCellsTempt4->size();
		int chooseCellSize = mOwnerViewLayout->mCellsTempt2->size();
		if (besideCellSize == 0 || chooseCellSize == 0)
		{
			setDragLineShow(false);
			return;
		}
		OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
		operateMgr->createNewAction();

		Line * nearestLine = mOwnerViewLayout->findNearestLine(mOwnerViewLayout->mColLines, mSelectLineDragPosition, 3);
		if (!nearestLine)
		{
			nearestLine = new Line(mSelectLineDragPosition);
			nearestLine->setAllPosition(mSelectLineDragPosition);
			mOwnerViewLayout->insertLine(nearestLine, mOwnerViewLayout->mColLines);
		}

		selectCellLOrRChangeLine(mOwnerExcelTemplete->getOperateMgr(), nearestLine);
		/*ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(winData, mOwnerExcelTemplete);
		winData->addToLocalAction(changeLineAction);
		changeLineAction->addChangeLine(mSelectLineLeftCellData, BaseConstants::Direction::RIGHT, mSelectLineLeftCellData->mRight, nearestLine);
		mSelectLineLeftCellData->mRight = nearestLine;
		if (mSelectLineRightLeftData){
		changeLineAction->addChangeLine(mSelectLineRightLeftData, BaseConstants::Direction::LEFT, mSelectLineRightLeftData->mLeft, nearestLine);
		mSelectLineRightLeftData->mLeft = nearestLine;
		}*/


		mOwnerViewLayout->removeUseLessColLine();
		ExcelRelayoutAction* relayoutAction = new ExcelRelayoutAction(operateMgr->getCurStepMgr(), mOwnerExcelTemplete);
		operateMgr->addToLocalActionAt(relayoutAction, 1);
		mOwnerExcelTemplete->requestLayout();

		setDragLineShow(false);
	}
}
void ExcelMouseInputManager::selectCellLOrRChangeLine(OperateMgr * operateMgr, Line* changeLine)
{
	char isSelectLeft = -10;
	if (mOwnerViewLayout->mTemptCellDataMergeReult->mLeft == mSelectLineObj->mSelectLine)
	{
		isSelectLeft = BaseConstants::Direction::LEFT;
	}
	else if (mOwnerViewLayout->mTemptCellDataMergeReult->mRight == mSelectLineObj->mSelectLine)
	{
		isSelectLeft = BaseConstants::Direction::RIGHT;
	}
	if (isSelectLeft < BaseConstants::Direction::LEFT)
	{
		return;
	}
	int chooseSize = mOwnerViewLayout->mCellsTempt2->size();
	ViewObj * tempObj;
	ExcelCellData* tempCellData;
	for (int i = 0; i < chooseSize; i++)
	{
		tempCellData = mOwnerViewLayout->mCellsTempt2->at(i);
		ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mOwnerExcelTemplete);
		operateMgr->addToLocalAction(changeLineAction);

		if (isSelectLeft == BaseConstants::Direction::LEFT)
		{
			changeLineAction->addChangeLine(tempCellData, BaseConstants::Direction::LEFT, tempCellData->mLeft, changeLine);
			tempCellData->mLeft = changeLine;
		}
		else
		{
			changeLineAction->addChangeLine(tempCellData, BaseConstants::Direction::RIGHT, tempCellData->mRight, changeLine);
			tempCellData->mRight = changeLine;
		}
	}
	//mCellsTempt1  在函数这个里已经赋值 isAbleToMooveCellLOrR（）


	int besideCellSize = mOwnerViewLayout->mCellsTempt4->size();
	for (int i = 0; i < besideCellSize; i++)
	{
		ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mOwnerExcelTemplete);
		operateMgr->addToLocalAction(changeLineAction);

		tempCellData = mOwnerViewLayout->mCellsTempt4->at(i);

		if (isSelectLeft == BaseConstants::Direction::LEFT)
		{
			changeLineAction->addChangeLine(tempCellData, BaseConstants::Direction::RIGHT, tempCellData->mRight, changeLine);
			tempCellData->mRight = changeLine;
		}
		else
		{
			changeLineAction->addChangeLine(tempCellData, BaseConstants::Direction::LEFT, tempCellData->mLeft, changeLine);
			tempCellData->mLeft = changeLine;
		}
	}
	mOwnerViewLayout->mCellsTempt2->clear();
	mOwnerViewLayout->mCellsTempt4->clear();
}
void ExcelMouseInputManager::setDragLineShow(bool isShow)
{
	int width = 1;
	if (isShow)
	{
		if (mSelectLineObj->mSelectLine)
		{
			if (mSelectLineObj->mSideType == SelectView::Side::Right || mSelectLineObj->mSideType == SelectView::Side::Left)
			{
				mOwnerViewLayout->setSelectLineVisability(true, mSelectLineDragPosition, mOwnerViewLayout->mViewParam->getInsideTop(), width, mOwnerViewLayout->mViewParam->getSelfH());
			}
			else if (mSelectLineObj->mSideType == SelectView::Side::Top || mSelectLineObj->mSideType == SelectView::Side::Bottom)
			{
				mOwnerViewLayout->setSelectLineVisability(true, mOwnerViewLayout->mViewParam->getInsideLeft(), mSelectLineDragPosition, mOwnerViewLayout->mViewParam->getSelfW(), width);
			}
		}
	}
	else
	{
		mOwnerViewLayout->setSelectLineVisability(false, 10, 0, 100, 100);
	}
}
bool ExcelMouseInputManager::caculateMarquee(int type, int x, int y)
{
	if (!mSelectLineObj->mSelectObj)
	{
		return false;
	}
	//ExcelCellPageMouse * cellPageMouse = (ExcelCellPageMouse*)mSelectLineObj->mSelectObj->getMouseMgr()->getMouse();
	//cellPageMouse
	//是否超出一个单元格
	OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();

	/*ViewObj * viewParent = mSelectLineObj->mSelectObj->getViewParent();
	BaseViewParam * parentViewParam = viewParent->getViewParam();
	if (BaseMath::isInsideTotalArea(x - parentViewParam->getX(), y - parentViewParam->getY(), mSelectLineObj->mSelectObj))*/
	ViewObj * parentRow = mSelectLineObj->mSelectObj->getParentViewById(ConstantsID::ContainerId::NORMAL_ROW);
	ViewObj * window = parentRow->getParentViewById(ConstantsID::ContainerId::TXT_WIN_VIEW);
	//if (parentRow)
	//{
	//	float temp = parentRow->exchangeCrossParentY(0);
	//	y += temp;
	//}
	if (BaseMath::isInsideTotalArea(x, y, mSelectLineObj->mSelectObj))
	{
		if (!isInOneCell)
		{
			isInOneCell = true;
			operateMgr->clearChooseList(true);
		}
		if (isPressOnCellLeft)
		{
			mSelectLineObj->mSelectObj->setMaskColor(UIConstants::MyColor::CHOOSEN_COLOR);
			operateMgr->addToChooseList(mSelectLineObj->mSelectObj);
			mOwnerExcelTemplete->setPageRepaint(mSelectLineObj->mSelectObj);
			return true;
		}

		mOwnerExcelTemplete->setPageRepaint(mSelectLineObj->mSelectObj);
		//short status = window->getStatus();
		if (type == BaseConstants::MousePressType::RELEASE)
		{
			window->changeStatus(BaseConstants::ObjStatus::DRAG_CHOOSE_RELEASE, false);
		}
		else
			window->changeStatus(BaseConstants::ObjStatus::DRAG_CHOOSE, false);
		mSelectLineObj->mSelectObj->changeStatus(BaseConstants::ObjStatus::DRAG_CHOOSE, false);
		return false;
	}
	else
	{
		//哪些单元格与鼠标拖动的的矩形相交
		if (mOwnerViewLayout->mCellsTempt1->size())
		{
			mOwnerViewLayout->mCellsTempt1->clear();
		}
		if (isInOneCell)
		{
			isInOneCell = false;
		}
		operateMgr->clearChooseList(true);
		window->changeStatus(BaseConstants::ObjStatus::EXCEL, false);

		int minx = 0;
		int miny = 0;
		int maxx = 0;
		int maxy = 0;
		if (mMouseDown.x() >= x)
		{
			minx = x;
			maxx = mMouseDown.x();
		}
		else
		{
			minx = mMouseDown.x();
			maxx = x;
		}
		if (mMouseDown.y() >= y)
		{
			miny = y;
			maxy = mMouseDown.y();
		}
		else
		{
			miny = mMouseDown.y();
			maxy = y;
		}
		int cellDataSize = mOwnerViewLayout->mCells->size();
		ExcelCellData* temptCellData;
		for (int i = 0; i < cellDataSize; i++)
		{
			temptCellData = mOwnerViewLayout->mCells->at(i);
			ViewObj* tempCellPage;
			if (BaseMath::isRectCross(minx, miny, maxx, maxy, temptCellData->mLeft->getPosition(), temptCellData->mTop->getPosition()
				, temptCellData->mRight->getPosition(), temptCellData->mBottom->getPosition()))
			{
				tempCellPage = temptCellData->getViewOwner();
				tempCellPage->changeStatus(BaseConstants::ObjStatus::FREE, false);
				operateMgr->addToChooseList(tempCellPage);
			}
		}
		return true;
	}
}
bool ExcelMouseInputManager::getIsPreeOnExcel()
{
	return mIsPressOnExcel;
}
void ExcelMouseInputManager::setIsPressOnExcel(bool isPressOnExcel)
{
	mIsPressOnExcel = isPressOnExcel;
}

bool ExcelMouseInputManager::canMousePressInTitleBottom()
{
	ViewObj * viewTitleOrBottom = NULL;
	OperateMgr * mgr = mOwnerExcelTemplete->getOperateMgr();
	ViewObj * window = mgr->getWindow();
	BaseContainer * temp = mOwnerExcelTemplete->findTempFromParent(ConstantsID::ContainerId::TITLE_TEMP, true);
	if (temp)
	{
		viewTitleOrBottom = (ViewObj *)window->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false)->getChild(0)
			->findChildrenByID(ConstantsID::ContainerId::PAGE_TITLE, false);
	}
	else
	{
		temp = mOwnerExcelTemplete->findTempFromParent(ConstantsID::ContainerId::BOTTOM_TEMP, true);
		if (temp)
		{
			viewTitleOrBottom = (ViewObj *)window->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false)->getChild(0)
				->findChildrenByID(ConstantsID::ContainerId::PAGE_BOTTOM, false);
		}
	}
	if (!viewTitleOrBottom)
	{
		return true;
	}
	int status = viewTitleOrBottom->getStatus();
	if (status == BaseConstants::ObjStatus::ACTIVE)
	{
		return true;
	}
	return false;
}
void ExcelMouseInputManager::setPressPage(ViewObj * page)
{
	mPressPage = page;
}
ViewObj * ExcelMouseInputManager::getPressPage()
{
	return mPressPage;
}