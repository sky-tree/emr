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
	QList<ViewObj *> * mBeforContantDefaultObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mBeforContantStrObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mAfterContantDefaultObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mAfterContantStrObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mBeforTitleDefaultObjList;//Ĭ�ϱ��������б�
	QList<ViewObj *> * mBeforTitleStrObjList;//Ĭ�ϱ��������б�
	QList<ViewObj *> * mAfterTitleDefaultObjList;//Ĭ�ϱ��������б�
	QList<ViewObj *> * mAfterTitleStrObjList;//Ĭ�ϱ��������б�
	bool isUseBefor;
};
#endif
