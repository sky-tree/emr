#include "ActionFactory.h"




#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseParagraph.h"


#include "ParagraphData.h"

#include "ActionMgr.h"
#include "AddObjListAction.h"
#include "DelObjListAction.h"
#include "SetParagraphIntParamAction.h"
#include "SetParagraphFloatParamAction.h"
#include "SetParagraphIndentParamAction.h"
#include "SetTxtNumParamAction.h"
#include "SetTxtMultipleNumParamAction.h"
#include "SetTxtStrParamAction.h"
#include "SetSysBoolParamAction.h"
#include "SetSysIntParamAction.h"
#include "SetPagePropertyAction.h"
#include "ImageScaleAction.h"


#include "ConstantsID.h"
#include "BaseIOTools.h"

ActionFactory::ActionFactory()
{
}

ActionFactory::~ActionFactory()
{
}
void ActionFactory::pushAddObjAction(ActionMgr * mgr, BaseContainer * obj, ViewObj * beforeFocus, char beforeDir)
{
	AddObjListAction * action = new AddObjListAction(mgr->getCurStepMgr());
	QList<BaseContainer *> * addList = new QList<BaseContainer *>();
	addList->push_back(obj);
	action->doAction(addList, beforeFocus, beforeDir);
	mgr->addAction(action);
}
void ActionFactory::pushAddObjListAction(ActionMgr * mgr, QList<BaseContainer *> * objList, ViewObj * beforeFocus, char beforeDir)
{
	AddObjListAction * action = new AddObjListAction(mgr->getCurStepMgr());
	action->doAction(objList, beforeFocus, beforeDir);
	mgr->addAction(action);
}
void ActionFactory::pushDelObjAction(ActionMgr * mgr, BaseParagraph * nextParagraph, ViewObj * preEffectLogicPage, BaseContainer *obj
	, ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir, ViewObj * afterFocus, char afterDir, bool upsidedown)
{
	QList<BaseContainer *> * list = new QList<BaseContainer *>();
	list->push_back(obj);
	pushDelObjListAction(mgr, nextParagraph, preEffectLogicPage, list, false, startFocus, startDir, endFocus, endDir, afterFocus, afterDir, upsidedown);
}
void ActionFactory::pushDelObjListAction(ActionMgr * mgr, BaseParagraph * nextParagraph, ViewObj * preEffectLogicPage, QList<BaseContainer *> * objList, bool needCreateNewList
	, ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir, ViewObj * afterFocus, char afterDir, bool upsidedown)
{
	DelObjListAction * action = new DelObjListAction(mgr->getCurStepMgr());
	QList<BaseContainer *> * destList = objList;
	if (needCreateNewList)
	{
		destList = new QList<BaseContainer *>();
		int size = objList->size();
		for (int i = 0; i < size; i ++)
		{
			destList->push_back(objList->at(i));
		}
	}
	action->doAction(nextParagraph, preEffectLogicPage, destList, startFocus, startDir, endFocus, endDir, afterFocus, afterDir, upsidedown);
	mgr->addAction(action);
}
void ActionFactory::pushSetParagraphIntParamAction(ActionMgr * mgr, QList<BaseContainer *> * chooseList, char type, int value, ViewObj * startFocus, char startDir
	, ViewObj * endFocus, char endDir, bool isUpsidedown)
{
	SetParagraphIntParamAction * action = new SetParagraphIntParamAction(mgr->getCurStepMgr());
	action->doAction(chooseList, type, value, startFocus, startDir, endFocus, endDir, isUpsidedown);
	mgr->addAction(action);
}
void ActionFactory::pushSetParagraphFloatParamAction(ActionMgr * mgr, QList<BaseContainer *> * list, char type, float value, ViewObj * startFocus, char startDir
	, ViewObj * endFocus, char endDir, bool isUpsidedown)
{
	SetParagraphFloatParamAction * action = new SetParagraphFloatParamAction(mgr->getCurStepMgr());
	action->doAction(list, type, value, startFocus, startDir, endFocus, endDir, isUpsidedown);
	mgr->addAction(action);
}
void ActionFactory::pushSetParagraphIndentParamAction(ActionMgr * mgr, QList<BaseContainer *> * chooseList, int inL, int inR, char indentType, int indentValue
	, ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir, bool isUpsidedown)
{
	SetParagraphIndentParamAction * action = new SetParagraphIndentParamAction(mgr->getCurStepMgr());
	action->doAction(chooseList, inL, inR, indentType, indentValue, startFocus, startDir, endFocus, endDir, isUpsidedown);
	mgr->addAction(action);
}
void ActionFactory::pushSetTxtNumParamAction(ActionMgr * mgr, QList<BaseContainer *> * objList, char type, int value)
{
	SetTxtNumParamAction * action = new SetTxtNumParamAction(mgr->getCurStepMgr());
	action->doAction(objList, type, value);
	mgr->addAction(action);
}
void ActionFactory::pushSetTxtMutipleNumParamAction(ActionMgr * mgr, QList<BaseContainer *> * objList, char type, int valueNum, int * value)
{
	SetTxtMultipleNumParamAction * action = new SetTxtMultipleNumParamAction(mgr->getCurStepMgr());
	action->doAction(objList, type, valueNum, value);
	mgr->addAction(action);
}
void ActionFactory::pushSetTxtStrParamAction(ActionMgr * mgr, QList<BaseContainer *> * objList, char type, QString value)
{
	SetTxtStrParamAction * action = new SetTxtStrParamAction(mgr->getCurStepMgr());
	action->doAction(objList, type, value);
	mgr->addAction(action);
}
void ActionFactory::pushSetSysBoolParamAction(ActionMgr * mgr, char type, bool value)
{
	SetSysBoolParamAction * action = new SetSysBoolParamAction(mgr->getCurStepMgr());
	action->doAction(type, value);
	mgr->addAction(action);
}
void ActionFactory::pushSetSysIntParamAction(ActionMgr * mgr, char type, int value)
{
	SetSysIntParamAction * action = new SetSysIntParamAction(mgr->getCurStepMgr());
	action->doAction(type, value);
	mgr->addAction(action);
}
void ActionFactory::pushSetPageParamsAction(ActionMgr * mgr, bool isHorizontal, short paperType, int inT, int inB, int inL, int inR, int titleMaxH, int bottomMaxH)
{
	SetPagePropertyAction * action = new SetPagePropertyAction(mgr->getCurStepMgr());
	action->doAction(isHorizontal, paperType, inT, inB, inL, inR, titleMaxH, bottomMaxH);
	mgr->addAction(action);
}

void ActionFactory::pushZoomImageParamAction(ActionMgr * mgr, BaseViewParam * viewParam, int w, int h)
{
	ImageScaleAction * action = new ImageScaleAction(mgr->getCurStepMgr());
	action->doAction(viewParam, w, h);
	mgr->addAction(action);
}