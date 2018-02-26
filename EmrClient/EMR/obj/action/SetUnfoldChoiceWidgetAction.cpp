#include "SetUnfoldChoiceWidgetAction.h"
#include "ConstantsID.h"
#include "iostream"
#include "UnfoldChoiceWidgetTemplete.h"
#include "GlobalTemplate.h"
#include "SetTxtWidgetElementAction.h"
//#include "TextWidget.h"
#include "ViewObj.h"
SetUnfoldChoiceWidgetAction::SetUnfoldChoiceWidgetAction(StepActionManger * stepActonManger) : SetWidgetElementPropertyAction(stepActonManger, ConstantsID::ClassId::SET_UNFOLD_CHOICE_TEMPLETE_ELEMENT_ACTION)
, mSetTemplete(NULL)
, mBeforDefaultObjList(NULL)
, mBeforStrObjList(NULL)
, mAfterDefaultObjList(NULL)
, mAfterStrObjList(NULL)
, mBeforOptionTempleteList(NULL)
, mAfterOptionTempleteList(NULL)
//,mBeforHaveStrObjList(NULL)
//, mAfterHaveStrObjList(NULL)
//, mBeforNotHaveStrObjList(NULL)
//, mAfterNotHaveStrObjList(NULL)
{
	mBeforDefaultObjList = new QList<ViewObj *>();
	mBeforStrObjList = new QList<ViewObj *>();
	mAfterDefaultObjList = new QList<ViewObj *>();
	mAfterStrObjList = new QList<ViewObj *>();
	//mBeforHaveStrObjList = new QList<ViewObj *>();
	//mAfterHaveStrObjList = new QList<ViewObj *>();
	//mBeforNotHaveStrObjList = new QList<ViewObj *>();
	//mAfterNotHaveStrObjList = new QList<ViewObj *>();
	isUseBefor = false;
}
SetUnfoldChoiceWidgetAction::~SetUnfoldChoiceWidgetAction()
{
	if (mSetTemplete)
	{
		mSetTemplete = NULL;
	}
	bool isSameDefaultObjList = false;
	if (mBeforDefaultObjList&&mAfterDefaultObjList)
	{
		isSameDefaultObjList = SetTxtWidgetElementAction::isSameObjInList(mBeforDefaultObjList, mAfterDefaultObjList);
	}
	delete_before_List(mBeforDefaultObjList, isSameDefaultObjList);
	delete_after_list(mAfterDefaultObjList, isSameDefaultObjList);
	/*if (mBeforDefaultObjList)
	{
		if (isUseBefor)
		{
			mBeforDefaultObjList->clear();
		}
		else
		{
			if (isSameDefaultObjList)
			{
				mBeforDefaultObjList->clear();
			}
			else
			{
				deleteVector(mBeforDefaultObjList);
			}
		}
		delete mBeforDefaultObjList;
		mBeforDefaultObjList = NULL;
	}
	if (mAfterDefaultObjList)
	{
		if (isUseBefor)
		{
			if (isSameDefaultObjList)
			{
				mAfterDefaultObjList->clear();
			}
			else
			{
				deleteVector(mAfterDefaultObjList);
			}
		}
		else
		{
			mAfterDefaultObjList->clear();
		}
		delete mAfterDefaultObjList;
		mAfterDefaultObjList = NULL;
	}*/
	bool isSameStrObjList = false;
	if (mBeforStrObjList&&mAfterStrObjList)
	{
		isSameStrObjList = SetTxtWidgetElementAction::isSameObjInList(mBeforStrObjList, mAfterStrObjList);
	}
	delete_before_List(mBeforStrObjList, isSameStrObjList);
	delete_after_list(mAfterStrObjList, isSameStrObjList);
	/*bool isSameHaveStrObjList = false;
	if (mBeforHaveStrObjList&&mAfterHaveStrObjList)
	{
		isSameHaveStrObjList = SetTxtWidgetElementAction::isSameObjInList(mBeforHaveStrObjList, mAfterHaveStrObjList);
	}
	delete_before_List(mBeforHaveStrObjList, isSameHaveStrObjList);
	delete_after_list(mAfterHaveStrObjList, isSameHaveStrObjList);
	bool isSameNotHaveStrObjList = false;
	if (mBeforNotHaveStrObjList&&mAfterNotHaveStrObjList)
	{
		isSameNotHaveStrObjList = SetTxtWidgetElementAction::isSameObjInList(mBeforNotHaveStrObjList, mAfterNotHaveStrObjList);
	}
	delete_before_List(mBeforNotHaveStrObjList, isSameNotHaveStrObjList);
	delete_after_list(mAfterNotHaveStrObjList, isSameNotHaveStrObjList);
*/
	if (mBeforOptionTempleteList == mAfterOptionTempleteList)
	{
		mBeforOptionTempleteList = NULL;
		mAfterOptionTempleteList = NULL;
	}
	else
	{
		if (mBeforOptionTempleteList)
		{
			if (isUseBefor)
			{
				mBeforOptionTempleteList = NULL;
			}
			else
			{
				deleteVector(mBeforOptionTempleteList);
				delete mBeforOptionTempleteList;
				mBeforOptionTempleteList = NULL;
			}

		}
		if (mAfterOptionTempleteList)
		{
			if (isUseBefor)
			{
				deleteVector(mAfterOptionTempleteList);
				delete mAfterOptionTempleteList;
				mAfterOptionTempleteList = NULL;
			}
			else
			{
				mAfterOptionTempleteList = NULL;
			}

		}
	}
	
}
void SetUnfoldChoiceWidgetAction::initAction(UnfoldChoiceWidgetTemplete * setTemplete, QMap<QString, QString> setBeforMap, QMap<QString, QString> setAfterMap, QList<int> beforChooseList, QList<int> afterChooseList)
{
	mSetTemplete = setTemplete;
	mSetBeforMap = setBeforMap;
	mSetAfterMap = setAfterMap;
	mBeforChooseList = beforChooseList;
	mAfterChooseList = afterChooseList;
	isUseBefor = false;
}
void SetUnfoldChoiceWidgetAction::initBeforObjList(QList<ViewObj *> * beforTitleDefaultList, QList<ViewObj *> * beforTitleStrList, QList<ChoiceItemTextWidget*>* beforeOptionTempleteList)
{
	SetTxtWidgetElementAction::copyObjListToObjList(beforTitleDefaultList, mBeforDefaultObjList, true);
	SetTxtWidgetElementAction::copyObjListToObjList(beforTitleStrList, mBeforStrObjList, true);
	mBeforOptionTempleteList = beforeOptionTempleteList;
}
void SetUnfoldChoiceWidgetAction::initAfterObjList(QList<ViewObj *> * afterTitleDefaultList, QList<ViewObj *> * afterTitleStrList, QList<ChoiceItemTextWidget*>* afterOptionTempleteList)
{
	SetTxtWidgetElementAction::copyObjListToObjList(afterTitleDefaultList, mAfterDefaultObjList, true);
	SetTxtWidgetElementAction::copyObjListToObjList(afterTitleStrList, mAfterStrObjList, true);
	mAfterOptionTempleteList = afterOptionTempleteList;
}
//void SetChoiceWidgetAction::initHaveAndNotHaveObjList(QList<ViewObj *> * beforHaveList, QList<ViewObj *> * beforNotHaveList, QList<ViewObj *> * afterHaveList, QList<ViewObj *> * afterNotHaveList)
//{
//	SetTxtWidgetElementAction::copyObjListToObjList(beforHaveList, mBeforHaveStrObjList, true);
//	SetTxtWidgetElementAction::copyObjListToObjList(beforNotHaveList, mBeforNotHaveStrObjList, true);
//	SetTxtWidgetElementAction::copyObjListToObjList(afterHaveList, mAfterHaveStrObjList, true);
//	SetTxtWidgetElementAction::copyObjListToObjList(afterNotHaveList, mAfterNotHaveStrObjList, true);
//}
void SetUnfoldChoiceWidgetAction::unDo()
{
	isUseBefor = true;
	mSetTemplete->setPropertyOnActionRedoOrUndo(mSetBeforMap, mBeforStrObjList, mBeforDefaultObjList, mBeforOptionTempleteList, mBeforChooseList);
	mBeforFocus->setFocus(mBeforDir, false);
}
void SetUnfoldChoiceWidgetAction::reDo()
{
	isUseBefor = false;
	mSetTemplete->setPropertyOnActionRedoOrUndo(mSetAfterMap, mAfterStrObjList, mAfterDefaultObjList, mAfterOptionTempleteList, mAfterChooseList);
	mAfterFocus->setFocus(mAfterDir, false);
}