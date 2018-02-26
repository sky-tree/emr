#ifndef _ACTION_FACTORY_H_
#define _ACTION_FACTORY_H_


#include <QList>

class BaseContainer;
class ViewObj;
class BaseParagraph;
class ParagraphData;
class ActionMgr;
class BaseViewParam;

class ActionFactory
{
public:
	ActionFactory();
	~ActionFactory();

public:

	static void pushAddObjAction(ActionMgr * mgr, BaseContainer * obj, ViewObj * beforeFocus, char beforeDir);
	static void pushAddObjListAction(ActionMgr * mgr, QList<BaseContainer *> * objList, ViewObj * beforeFocus, char beforeDir);
	static void pushDelObjAction(ActionMgr * mgr, BaseParagraph * nextParagraph, ViewObj * preEffectLogicPage, BaseContainer *objList
		, ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir, ViewObj * afterFocus, char afterDir, bool upsidedown);
	static void pushDelObjListAction(ActionMgr * mgr, BaseParagraph * nextParagraph, ViewObj * preEffectLogicPage, QList<BaseContainer *> * objList, bool needCreateNewList
		, ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir, ViewObj * afterFocus, char afterDir, bool upsidedown);

	static void pushSetTxtNumParamAction(ActionMgr * mgr, QList<BaseContainer *> * objList, char type, int value);
	static void pushSetTxtMutipleNumParamAction(ActionMgr * mgr, QList<BaseContainer *> * objList, char type, int valueNum, int * value);
	static void pushSetTxtStrParamAction(ActionMgr * mgr, QList<BaseContainer *> * objList, char type, QString value);

	static void pushSetParagraphIntParamAction(ActionMgr * mgr, QList<BaseContainer *> * chooseList, char type, int value, ViewObj * startFocus, char startDir
		, ViewObj * endFocus, char endDir, bool isUpsidedown);
	static void pushSetParagraphFloatParamAction(ActionMgr * mgr, QList<BaseContainer *> * list, char type, float value, ViewObj * startFocus, char startDir
		, ViewObj * endFocus, char endDir, bool isUpsidedown);
	static void pushSetParagraphIndentParamAction(ActionMgr * mgr, QList<BaseContainer *> * chooseList, int inL, int inR, char indentType, int indentValue
		, ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir, bool isUpsidedown);

	static void pushSetPageParamsAction(ActionMgr * mgr, bool isHorizontal, short paperType, int inT, int inB, int inL, int inR, int titleMaxH, int bottomMaxH);

	static void pushSetSysBoolParamAction(ActionMgr * mgr, char type, bool value);
	static void pushSetSysIntParamAction(ActionMgr * mgr, char type, int value);
	static void pushZoomImageParamAction(ActionMgr * mgr, BaseViewParam * viewParam,int w, int h);


};
#endif
