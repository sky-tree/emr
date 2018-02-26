#include "SetMenstruationWidgetAction.h"
#include "ConstantsID.h"
#include "iostream"
#include "ViewObj.h"
#include "MenstruationTemplete.h"
#include "GlobalTemplate.h"
#include "SetTxtWidgetElementAction.h"
SetMenstruationWidgetAction::SetMenstruationWidgetAction(StepActionManger * stepActonManger) : SetWidgetElementPropertyAction(stepActonManger, ConstantsID::ClassId::SET_MENSTRUATION_TEMPLETE_ELEMENT_ACTION)
, mBeforTopObjList(NULL)
, mBeforDownObjList(NULL)
, mAfterTopObjList(NULL)
, mAfterDownObjList(NULL)
, mSetTemplete(NULL)
{
	mBeforTopObjList = new QList<ViewObj *>();
	mBeforDownObjList = new QList<ViewObj *>();
	mAfterTopObjList = new QList<ViewObj *>();
	mAfterDownObjList = new QList<ViewObj *>();
	isUseBefor = false;
}


SetMenstruationWidgetAction::~SetMenstruationWidgetAction()
{
	if (mSetTemplete)
	{
		mSetTemplete = NULL;
	}
	bool isSameTopObjList = false;
	if (mBeforTopObjList&&mAfterTopObjList)
	{
		isSameTopObjList = SetTxtWidgetElementAction::isSameObjInList(mBeforTopObjList, mAfterTopObjList);
	}
	delete_before_List(mBeforTopObjList, isSameTopObjList);
	delete_after_list(mAfterTopObjList, isSameTopObjList);
	bool isSameDownObjList = false;
	if (mBeforDownObjList&&mAfterDownObjList)
	{
		isSameDownObjList = SetTxtWidgetElementAction::isSameObjInList(mBeforDownObjList, mAfterDownObjList);
	}
	delete_before_List(mBeforDownObjList, isSameDownObjList);
	delete_after_list(mAfterDownObjList, isSameDownObjList);
}
void SetMenstruationWidgetAction::unDo()
{
	isUseBefor = true;
	mSetTemplete->setPropertyOnActionRedoOrUndo(mBeforTopObjList, mBeforDownObjList);
	mBeforFocus->setFocus(mBeforDir, false);
}
void SetMenstruationWidgetAction::reDo()
{
	isUseBefor = false;
	mSetTemplete->setPropertyOnActionRedoOrUndo(mAfterTopObjList, mAfterDownObjList);
	mAfterFocus->setFocus(mAfterDir, false);
}
void SetMenstruationWidgetAction::initBeforObjList(MenstruationTemplete* setTemplete, QList<ViewObj *> * beforTopList, QList<ViewObj *> * beforDownStrList)
{
	mSetTemplete = setTemplete;
	SetTxtWidgetElementAction::copyObjListToObjList(beforTopList, mBeforTopObjList, true);
	SetTxtWidgetElementAction::copyObjListToObjList(beforDownStrList, mBeforDownObjList, true);
}
void SetMenstruationWidgetAction::initAfterObjList(QList<ViewObj *> * afterTopList, QList<ViewObj *> * afterDownStrList)
{
	SetTxtWidgetElementAction::copyObjListToObjList(afterTopList, mAfterTopObjList, true);
	SetTxtWidgetElementAction::copyObjListToObjList(afterDownStrList, mAfterDownObjList, true);
}