#include "AddTempletWidgetAction.h"
#include "ConstantsID.h"
#include "iostream"
#include "BaseTemplate.h"
#include "ViewObj.h"
#include "ExcelTemplete.h"
#include "OperateMgr.h"
AddTempletWidgetAction::AddTempletWidgetAction(StepActionManger * stepActonManger) : BaseAction(stepActonManger, ConstantsID::ClassId::ADD_TEMPLETE_WIDGET_ACTION)
, mAddObjTempParent(NULL)
, mAddBeforeFocus(NULL)
, mAddAfterFocus(NULL)
{
	isAddAction = false;
	mIsUse = false;
	//mStartFocus = NULL;
	//mEndFocus = NULL;
}


AddTempletWidgetAction::~AddTempletWidgetAction()
{
	mAddObjTempParent = NULL;
	mAddBeforeFocus = NULL;
	mAddAfterFocus = NULL;
	if (mAddTempleteWedgit)
	{
		if (mIsUse)
		{
			
		}
		else
		{
			//mAddTempleteWedgit->clearViewParent();
			//mAddTempleteWedgit->clearTempParent();
			delete mAddTempleteWedgit;
		}
		mAddTempleteWedgit = NULL;
	}
	
}
void AddTempletWidgetAction::initAddAction(BaseTemplate*addTempleteWedgit, BaseTemplate * addObjTempParent, int addIndex, ViewObj * addBeforeFocus, char addBeforeDir, ViewObj * addAfterFocus, char addAfterDir)
{
	isAddAction = true;
	mIsUse = true;
	mAddTempleteWedgit= addTempleteWedgit ;
	mAddObjTempParent = addObjTempParent;
	mAddIndex = addIndex;
	mAddBeforeFocus = addBeforeFocus;
	mAddBeforeDir = addBeforeDir;
	mAddAfterFocus = addAfterFocus;
	mAddAfterDir = addAfterDir;
}
void AddTempletWidgetAction::initDeleteAction(BaseTemplate*deleteTempleteWedgit, BaseTemplate * deleteObjTempParent, int deleteIndex, ViewObj * deleteBeforeFocus, char deleteBeforeDir, ViewObj * deleteAfterFocus, char deleteAfterDir)
{
	isAddAction = false;
	mIsUse = false;
	mAddTempleteWedgit = deleteTempleteWedgit;
	mAddObjTempParent = deleteObjTempParent;
	mAddIndex = deleteIndex;
	mAddBeforeFocus = deleteBeforeFocus;
	mAddBeforeDir = deleteBeforeDir;
	mAddAfterFocus = deleteAfterFocus;
	mAddAfterDir = deleteAfterDir;
}
void AddTempletWidgetAction::unDo()
{
	if (isAddAction)
	{
		deleteTemplete(mAddBeforeFocus, mAddBeforeDir);
	}
	else
	{
		addTemplete(mAddAfterFocus, mAddAfterDir, mAddBeforeFocus, mAddBeforeDir);
	}
	
}
void AddTempletWidgetAction::reDo()
{
	if (isAddAction)
	{
		addTemplete(mAddBeforeFocus,mAddBeforeDir,mAddAfterFocus,mAddAfterDir);
	}
	else
	{
		deleteTemplete(mAddAfterFocus, mAddAfterDir);
	}
	
}
void AddTempletWidgetAction::deleteTemplete(ViewObj * deleteFocusObj, char focusDir)
{
	
	//mAddObjTempParent->delChild(mAddTempleteWedgit, true);
	OperateMgr * operateMgr = mAddObjTempParent->getOperateMgr();
	operateMgr->clearChooseList(true);
	mAddObjTempParent->deleteChild(mAddTempleteWedgit, true);
	bool isRight;
	if (focusDir == BaseConstants::Direction::RIGHT)
	{
		isRight = true;
	}
	else
	{
		isRight = false;
	}
	mIsUse = false;
	deleteFocusObj->setFocus(isRight);
	//winData->replaceNeedReplaceList();
}
void AddTempletWidgetAction::addTemplete(ViewObj * addFocusObj, char focusDir, ViewObj * addAfterObj, char addAfterObjDir)
{
	bool isRight;
	int disIdx = 0;
	if (focusDir == BaseConstants::Direction::RIGHT)
	{
		isRight = true;
		disIdx++;
	}
	else
	{
		isRight = false;
	}
	addFocusObj->setFocus(isRight);
	mAddObjTempParent->addChild(mAddTempleteWedgit, mAddIndex, false);
	//mAddTempleteWedgit->setIsShowed(false);
	//if (mAddTempleteWedgit->getClassId() == ConstantsID::ClassId::EXCEL_TEMPLETE)
	//{
	//	/*ViewObj * row = addFocusObj->getViewParent();
	//	((ExcelTemplete*)mAddTempleteWedgit)->addWidgetToRow(row, addFocusObj->getViewIndexFromParent() + disIdx);*/
	//}
	//else
	//{
	//	mAddTempleteWedgit->addWidgetToRow();
	//}

	if (addAfterObjDir == BaseConstants::Direction::RIGHT)
	{
		isRight = true;
	}
	else
	{
		isRight = false;
	}
	mIsUse = true;
	addAfterObj->setFocus(isRight);
}