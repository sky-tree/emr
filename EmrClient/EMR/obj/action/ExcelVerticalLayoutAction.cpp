#include "ExcelVerticalLayoutAction.h"
#include "ConstantsID.h"
#include "ExcelTemplete.h"
#include "ViewObj.h"
#include "OperateMgr.h"
#include "FlagParam.h"
ExcelVerticalLayoutAction::ExcelVerticalLayoutAction(StepActionManger * stepActonManger) :BaseAction(stepActonManger, ConstantsID::ClassId::SET_EXCEL_VERTICAL_LAYOUT_ACTION)
//,mExcelTemplete(NULL)
, mFoucus(NULL)
, mChangeCells(NULL)
{
	mChangeCells = new 	QList<ExcelCellData *>();
	//mUndoVerticalLayout = 0;
	//mRedoVerticalLayout = 0;
}


ExcelVerticalLayoutAction::~ExcelVerticalLayoutAction()
{
	/*if (mExcelTemplete){
		mExcelTemplete = NULL;
	}*/
	if (mChangeCells)
	{
		mChangeCells->clear();
		delete mChangeCells;
		mChangeCells = NULL;
	}
}
void ExcelVerticalLayoutAction::InitAction( ViewObj* focus, char direction)
{
	//mExcelTemplete = excelTemplete;
	mFoucus = focus;
	mDirection = direction;
	/*int index = mChangeCells->indexOf(changCell);
	if (index<0)
	{
		mChangeCells->push_back(changCell);
	}*/
}
void ExcelVerticalLayoutAction::addChangeCells(QList<ExcelCellData *>* cellList, QList<int> unDoPositionList, QList<int> redoPositionList)
{
	int size = cellList->size();
	ExcelCellData* changCell;
	for (int i = 0; i < size;i++)
	{
		changCell = cellList->at(i);
		int index = mChangeCells->indexOf(changCell);
		if (index<0)
		{
			mChangeCells->push_back(changCell);
			mUndoVerticalLayoutList.push_back(unDoPositionList.at(i));
			mRedoVerticalLayoutList.push_back(redoPositionList.at(i));
		}
	}
}
void ExcelVerticalLayoutAction::addChangeCell(ExcelCellData* cellData, int undPostion, int reduPosition)
{
	int index = mChangeCells->indexOf(cellData);
	if (index<0)
	{
		mChangeCells->push_back(cellData);
		mUndoVerticalLayoutList.push_back(undPostion);
		mRedoVerticalLayoutList.push_back(reduPosition);
	}
}
void ExcelVerticalLayoutAction::unDo()
{
	/*if (BaseConstants::Direction::RIGHT == mDirection)
	{
		mFoucus->setFocus(true);
	}
	else
	{
		mFoucus->setFocus(false);
	}*/
	if (mSavedPressStart)
	{
		mOperateMgr->setPress(BaseConstants::MousePressType::PRESS, mSavedPressStart->getFocusObj(), mSavedPressStart->getDirection());
		mOperateMgr->setPress(BaseConstants::MousePressType::RELEASE, mSavedPressEnd->getFocusObj(), mSavedPressEnd->getDirection());
	}
	ExcelTemplete::setExcelTemplateVerticalType(mUndoVerticalLayoutList, mChangeCells);
	if (mSavedPressStart)
	{
		mOperateMgr->setPress(BaseConstants::MousePressType::PRESS, mSavedPressStart->getFocusObj(), mSavedPressStart->getDirection());
		mOperateMgr->setPress(BaseConstants::MousePressType::RELEASE, mSavedPressEnd->getFocusObj(), mSavedPressEnd->getDirection());
	}
}
void ExcelVerticalLayoutAction::reDo()
{
	/*if (BaseConstants::Direction::RIGHT == mDirection)
	{
		mFoucus->setFocus(true);
	}
	else
	{
		mFoucus->setFocus(false);
	}*/
	if (mSavedPressStart)
	{
		mOperateMgr->setPress(BaseConstants::MousePressType::PRESS, mSavedPressStart->getFocusObj(), mSavedPressStart->getDirection());
		mOperateMgr->setPress(BaseConstants::MousePressType::RELEASE, mSavedPressEnd->getFocusObj(), mSavedPressEnd->getDirection());
	}
	ExcelTemplete::setExcelTemplateVerticalType(mRedoVerticalLayoutList, mChangeCells);
	if (mSavedPressStart)
	{
		mOperateMgr->setPress(BaseConstants::MousePressType::PRESS, mSavedPressStart->getFocusObj(), mSavedPressStart->getDirection());
		mOperateMgr->setPress(BaseConstants::MousePressType::RELEASE, mSavedPressEnd->getFocusObj(), mSavedPressEnd->getDirection());
	}
}