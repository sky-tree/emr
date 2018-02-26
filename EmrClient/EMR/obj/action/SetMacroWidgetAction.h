#ifndef _SETMACROWIDGETACTION_H_
#define _SETMACROWIDGETACTION_H_
#include "SetWidgetElementPropertyAction.h"
#include "qmap.h"
class MacroTemplete;
class ViewObj;
class SetMacroWidgetAction :
	public SetWidgetElementPropertyAction
{
public:
	SetMacroWidgetAction(StepActionManger * mgr);
	~SetMacroWidgetAction();
	void unDo();
	void reDo();
	void initAction(MacroTemplete * setTemplete, QMap<QString, QString> setBeforMap, QMap<QString, QString> setAfterMap);
	void initBeforObjList(QList<ViewObj *> * beforContantDefaultList, QList<ViewObj *> * beforContantStrList, QList<ViewObj *> * beforTitleDefaultList, QList<ViewObj *> * beforTitleStrList);
	void initAfterObjList(QList<ViewObj *> * afterContantDefaultList, QList<ViewObj *> * afteContantrStrList, QList<ViewObj *> * afteTitleDefaultList, QList<ViewObj *> * afteTitleStrList);
	static void copyObjListToObjList(QList<ViewObj *> * srcList, QList<ViewObj *> * destList, bool isClearDestList);
	MacroTemplete * mSetTemplete;
	QMap<QString, QString> mSetBeforMap;
	QMap<QString, QString> mSetAfterMap;
	QList<ViewObj *> * mBeforContantDefaultObjList;//默认显示文字列表
	QList<ViewObj *> * mBeforContantStrObjList;//默认显示文字列表
	QList<ViewObj *> * mAfterContantDefaultObjList;//默认显示文字列表
	QList<ViewObj *> * mAfterContantStrObjList;//默认显示文字列表
	QList<ViewObj *> * mBeforTitleDefaultObjList;//默认标题文字列表
	QList<ViewObj *> * mBeforTitleStrObjList;//默认标题文字列表
	QList<ViewObj *> * mAfterTitleDefaultObjList;//默认标题文字列表
	QList<ViewObj *> * mAfterTitleStrObjList;//默认标题文字列表
	bool isUseBefor;
};
#endif
