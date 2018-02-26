#ifndef _SETCHOICEWIDGETACTION_H_
#define _SETCHOICEWIDGETACTION_H_
#include "SetWidgetElementPropertyAction.h"
#include "qmap.h"
#include "qlist.h"
class ChoiceWidgetTemplete;
class ViewObj;
class TextWidget;
class SetChoiceWidgetAction :public SetWidgetElementPropertyAction
{
public:
	SetChoiceWidgetAction(StepActionManger * stepActonManger);
	~SetChoiceWidgetAction();
	void unDo();
	void reDo();
	void initAction(ChoiceWidgetTemplete * setTemplete, QMap<QString, QString> setBeforMap, QMap<QString, QString> setAfterMap, QList<int> beforChooseList, QList<int> afterChooseList);
	void initBeforObjList(QList<ViewObj *> * beforDefaultList, QList<ViewObj *> * beforStrList, QList<TextWidget*>* beforeOptionTempleteList);
	void initAfterObjList(QList<ViewObj *> * afterDefaultList, QList<ViewObj *> * afterStrList, QList<TextWidget*>* afterOptionTempleteList);
	void initHaveAndNotHaveObjList(QList<ViewObj *> * beforHaveList, QList<ViewObj *> * beforNotHaveList, QList<ViewObj *> * afterHaveList, QList<ViewObj *> * afterNotHaveList);
	static void copyObjListToObjList(QList<ViewObj *> * srcList, QList<ViewObj *> * destList, bool isClearDestList);
private:
	ChoiceWidgetTemplete * mSetTemplete;
	QMap<QString, QString> mSetBeforMap;
	QMap<QString, QString> mSetAfterMap;
	QList<ViewObj *> * mBeforDefaultObjList;//默认显示文字列表
	QList<ViewObj *> * mBeforStrObjList;//默认显示文字列表
	QList<ViewObj *> * mAfterDefaultObjList;//默认显示文字列表
	QList<ViewObj *> * mAfterStrObjList;//默认标题文字列表
	QList<ViewObj *> * mBeforHaveStrObjList;//有无类型，有显示文字列表
	QList<ViewObj *> * mAfterHaveStrObjList;//有无类型，有显示文字列表
	QList<ViewObj *> * mBeforNotHaveStrObjList;//有无类型，无 显示文字列表
	QList<ViewObj *> * mAfterNotHaveStrObjList;//有无类型，无 显示文字列表
	QList<TextWidget*>* mBeforOptionTempleteList;
	QList<TextWidget*>* mAfterOptionTempleteList;
	QList<int>  mBeforChooseList;    //选中列表
	QList<int>  mAfterChooseList;    //选中列表
	bool isUseBefor;
};
#endif