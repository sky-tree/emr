#include "SetTxtWidgetElementAction.h"
#include "ConstantsID.h"
#include "iostream"
#include "TextWidget.h"
#include "ViewObj.h"
#include "GlobalTemplate.h"
#include "qdebug.h"
SetTxtWidgetElementAction::SetTxtWidgetElementAction(StepActionManger * stepActonManger) : SetWidgetElementPropertyAction(stepActonManger, ConstantsID::ClassId::SET_TXT_TEMPLETE_ELEMENT_ACTION)
, mSetTemplete(NULL)
, mBeforDefaultObjList(NULL)
, mBeforStrObjList(NULL)
, mAfterDefaultObjList(NULL)
, mAfterStrObjList(NULL)
{
	mBeforDefaultObjList = new QList<ViewObj *>();
	mBeforStrObjList = new QList<ViewObj *>();
	mAfterDefaultObjList = new QList<ViewObj *>();
	mAfterStrObjList = new QList<ViewObj *>();
	isUseBefor = false;
}


SetTxtWidgetElementAction::~SetTxtWidgetElementAction()
{
	if (mSetTemplete)
	{
		mSetTemplete = NULL;
	}
	//有可能是相同的显示内容
	bool isSameDefaultObjList = false;
	if (mBeforDefaultObjList&&mAfterDefaultObjList)
	{
		isSameDefaultObjList = isSameObjInList(mBeforDefaultObjList, mAfterDefaultObjList);
	}
	if (mBeforDefaultObjList)
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
	}
	bool isSameStrObjList = false;
	if (mBeforStrObjList&&mAfterStrObjList)
	{
		isSameStrObjList = isSameObjInList(mBeforStrObjList, mAfterStrObjList);
	}
	if (mBeforStrObjList)
	{
		if (isUseBefor)
		{
			mBeforStrObjList->clear();
		}
		else
		{
			if (isSameStrObjList)
			{
				mBeforStrObjList->clear();
			}
			else
			{
				deleteVector(mBeforStrObjList);
			}
			
		}
		delete mBeforStrObjList;
		mBeforStrObjList = NULL;
	}
	
	if (mAfterStrObjList)
	{
		if (isUseBefor)
		{
			if (isSameStrObjList)
			{
				mAfterStrObjList->clear();
			}
			else
			{
				deleteVector(mAfterStrObjList);
			}
			
		}
		else
		{
			mAfterStrObjList->clear();
		}
		delete mAfterStrObjList;
		mAfterStrObjList = NULL;
	}
}
void SetTxtWidgetElementAction::initAction(TextWidget * setTemplete, QMap<QString, QString> setBeforMap, QMap<QString, QString> setAfterMap)
{
	mSetTemplete = setTemplete;
	mSetBeforMap = setBeforMap;
	mSetAfterMap = setAfterMap;
	isUseBefor = false;
}
void SetTxtWidgetElementAction::initBeforObjList(QList<ViewObj *> * beforDefaultList, QList<ViewObj *> * beforStrList)
{
	copyObjListToObjList(beforDefaultList, mBeforDefaultObjList,true);
	copyObjListToObjList(beforStrList, mBeforStrObjList, true);
}
void SetTxtWidgetElementAction::initAfterObjList(QList<ViewObj *> * afterDefaultList, QList<ViewObj *> * afterStrList)
{
	copyObjListToObjList(afterDefaultList, mAfterDefaultObjList, true);
	copyObjListToObjList(afterStrList, mAfterStrObjList, true);
}
void SetTxtWidgetElementAction::copyObjListToObjList(QList<ViewObj *> * srcList, QList<ViewObj *> * destList, bool isClearDestList)
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
	for (int i = 0; i < srcSize;i++)
	{
		destList->push_back(srcList->at(i));
	}
}
bool SetTxtWidgetElementAction::isSameObjInList(QList<ViewObj *> * srcList, QList<ViewObj *> * destList)
{
	if ((!destList) || (!srcList))
	{
		return false;
	}
	int srcSize = srcList->size();
	int destSize = destList->size();
	if (destSize == srcSize)
	{
		if (destSize==0)
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	for (int i = 0; i < srcSize; i++)
	{
		if (srcList->at(i) != destList->at(i))
		{
			return false;
		}
	}
	return true;
}
bool SetTxtWidgetElementAction::isSameObjInTwoList(QList<BaseContainer *> * srcList, QList<ViewObj *> * destList)
{
	if ((!destList) || (!srcList))
	{
		return false;
	}
	int srcSize = srcList->size();
	int destSize = destList->size();
	if (destSize == srcSize)
	{
		if (destSize == 0)
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	for (int i = 0; i < srcSize; i++)
	{
		if (srcList->at(i) != destList->at(i))
		{
			return false;
		}
	}
	return true;
}
void SetTxtWidgetElementAction::unDo()
{
	isUseBefor = true;
	mSetTemplete->setPropertyOnActionRedoOrUndo(mSetBeforMap,mBeforStrObjList,mBeforDefaultObjList);
	if (mBeforDir == BaseConstants::Direction::RIGHT)
	{
		mBeforFocus->setFocus(true);
	}
	else
	{
		mBeforFocus->setFocus(false);
	}
	
}
void SetTxtWidgetElementAction::reDo()
{
	isUseBefor = false;
	mSetTemplete->setPropertyOnActionRedoOrUndo(mSetAfterMap,mAfterStrObjList,mAfterDefaultObjList);
	if (mAfterDir == BaseConstants::Direction::RIGHT)
	{
		mAfterFocus->setFocus(true);
	}
	else
	{
		mAfterFocus->setFocus(false);
	}
}