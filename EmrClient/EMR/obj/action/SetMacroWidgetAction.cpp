#include "SetMacroWidgetAction.h"
#include "ConstantsID.h"
#include "iostream"
#include "ViewObj.h"
#include "MacroTemplete.h"
#include "GlobalTemplate.h"
SetMacroWidgetAction::SetMacroWidgetAction(StepActionManger * mgr) : SetWidgetElementPropertyAction(mgr, ConstantsID::ClassId::SET_MACRO_TEMPLETE_ELEMENT_ACTION)
, mSetTemplete(NULL)
, mBeforContantDefaultObjList(NULL)
, mBeforContantStrObjList(NULL)
, mAfterContantDefaultObjList(NULL)
, mAfterContantStrObjList(NULL)
, mBeforTitleDefaultObjList(NULL)
, mBeforTitleStrObjList(NULL)
, mAfterTitleDefaultObjList(NULL)
, mAfterTitleStrObjList(NULL)
{
	mBeforContantDefaultObjList = new QList<ViewObj *>();
	mBeforContantStrObjList = new QList<ViewObj *>();
	mAfterContantDefaultObjList = new QList<ViewObj *>();
	mAfterContantStrObjList = new QList<ViewObj *>();
	mBeforTitleDefaultObjList = new QList<ViewObj *>();
	mBeforTitleStrObjList = new QList<ViewObj *>();
	mAfterTitleDefaultObjList = new QList<ViewObj *>();
	mAfterTitleStrObjList = new QList<ViewObj *>();
	isUseBefor = false;
}


SetMacroWidgetAction::~SetMacroWidgetAction()
{
	if (mSetTemplete)
	{
		mSetTemplete = NULL;
	}
	if (mBeforContantDefaultObjList)
	{
		if (isUseBefor)
		{
			mBeforContantDefaultObjList->clear();
		}
		else
		{
			deleteVector(mBeforContantDefaultObjList);
		}
		delete mBeforContantDefaultObjList;
		mBeforContantDefaultObjList = NULL;
	}
	if (mBeforContantStrObjList)
	{
		if (isUseBefor)
		{
			mBeforContantStrObjList->clear();
		}
		else
		{
			deleteVector(mBeforContantStrObjList);
		}
		delete mBeforContantStrObjList;
		mBeforContantStrObjList = NULL;
	}
	if (mAfterContantDefaultObjList)
	{
		if (isUseBefor)
		{
			deleteVector(mAfterContantDefaultObjList);
		}
		else
		{
			mAfterContantDefaultObjList->clear();
		}
		delete mAfterContantDefaultObjList;
		mAfterContantDefaultObjList = NULL;
	}
	if (mAfterContantStrObjList)
	{
		if (isUseBefor)
		{
			deleteVector(mAfterContantStrObjList);
		}
		else
		{
			mAfterContantStrObjList->clear();
		}
		delete mAfterContantStrObjList;
		mAfterContantStrObjList = NULL;
	}
	if (mBeforTitleDefaultObjList)
	{
		if (isUseBefor)
		{
			mBeforTitleDefaultObjList->clear();
		}
		else
		{
			deleteVector(mBeforTitleDefaultObjList);
		}
		delete mBeforTitleDefaultObjList;
		mBeforTitleDefaultObjList = NULL;
	}
	if (mBeforTitleStrObjList)
	{
		if (isUseBefor)
		{
			mBeforTitleStrObjList->clear();
		}
		else
		{
			deleteVector(mBeforTitleStrObjList);
		}
		delete mBeforTitleStrObjList;
		mBeforTitleStrObjList = NULL;
	}
	if (mAfterTitleDefaultObjList)
	{
		if (isUseBefor)
		{
			deleteVector(mAfterTitleDefaultObjList);
		}
		else
		{
			mAfterTitleDefaultObjList->clear();
		}
		delete mAfterTitleDefaultObjList;
		mAfterTitleDefaultObjList = NULL;
	}
	if (mAfterTitleStrObjList)
	{
		if (isUseBefor)
		{
			deleteVector(mAfterTitleStrObjList);
		}
		else
		{
			mAfterTitleStrObjList->clear();
		}
		delete mAfterTitleStrObjList;
		mAfterTitleStrObjList = NULL;
	}
}
void SetMacroWidgetAction::initAction(MacroTemplete * setTemplete, QMap<QString, QString> setBeforMap, QMap<QString, QString> setAfterMap)
{
	mSetTemplete = setTemplete;
	mSetBeforMap = setBeforMap;
	mSetAfterMap = setAfterMap;
	isUseBefor = false;
}
void SetMacroWidgetAction::initBeforObjList(QList<ViewObj *> * beforContantDefaultList, QList<ViewObj *> * beforContantStrList, QList<ViewObj *> * beforTitleDefaultList, QList<ViewObj *> * beforTitleStrList)
{
	copyObjListToObjList(beforContantDefaultList, mBeforContantDefaultObjList, true);
	copyObjListToObjList(beforContantStrList, mBeforContantStrObjList, true);
	copyObjListToObjList(beforTitleDefaultList, mBeforTitleDefaultObjList, true);
	copyObjListToObjList(beforTitleStrList, mBeforTitleStrObjList, true);
}
void SetMacroWidgetAction::initAfterObjList(QList<ViewObj *> * afterContantDefaultList, QList<ViewObj *> * afteContantrStrList, QList<ViewObj *> * afteTitleDefaultList, QList<ViewObj *> * afteTitleStrList)
{
	copyObjListToObjList(afterContantDefaultList, mAfterContantDefaultObjList, true);
	copyObjListToObjList(afteContantrStrList, mAfterContantStrObjList, true);
	copyObjListToObjList(afteTitleDefaultList, mAfterTitleDefaultObjList, true);
	copyObjListToObjList(afteTitleStrList, mAfterTitleStrObjList, true);
}
void SetMacroWidgetAction::copyObjListToObjList(QList<ViewObj *> * srcList, QList<ViewObj *> * destList, bool isClearDestList)
{
	if ((!destList) || (!srcList))
	{
		return;
	}
	if (isClearDestList)
	{
		if (destList->size()>0)
		{
			destList->clear();
		}
	}
	int srcSize = srcList->size();
	for (int i = 0; i < srcSize; i++)
	{
		destList->push_back(srcList->at(i));
	}
}
void SetMacroWidgetAction::unDo()
{
	isUseBefor = true;
	mSetTemplete->setPropertyOnActionRedoOrUndo(mSetBeforMap, mBeforContantStrObjList, mBeforContantDefaultObjList, mBeforTitleStrObjList, mBeforTitleDefaultObjList);
	if (BaseConstants::Direction::RIGHT == mBeforDir)
	{
		mBeforFocus->setFocus(true);
	}
	else
	{
		mBeforFocus->setFocus(false);
	}
	
}
void SetMacroWidgetAction::reDo()
{
	isUseBefor = false;
	mSetTemplete->setPropertyOnActionRedoOrUndo(mSetAfterMap, mAfterContantStrObjList, mAfterContantDefaultObjList, mAfterTitleStrObjList, mAfterTitleDefaultObjList);
	if (BaseConstants::Direction::RIGHT == mAfterDir)
	{
		mAfterFocus->setFocus(true);
	}
	else
	{
		mAfterFocus->setFocus(false);
	}
}