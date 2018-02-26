#ifndef _SETUNFOLDCHOICEWIDGETACTION_H_
#define _SETUNFOLDCHOICEWIDGETACTION_H_
#include "SetWidgetElementPropertyAction.h"
#include "qmap.h"
#include "qlist.h"
class UnfoldChoiceWidgetTemplete;
class ViewObj;
class ChoiceItemTextWidget;
class SetUnfoldChoiceWidgetAction :public SetWidgetElementPropertyAction
{
public:
	SetUnfoldChoiceWidgetAction(StepActionManger * stepActonManger);
	~SetUnfoldChoiceWidgetAction();
	void unDo();
	void reDo();
	void initAction(UnfoldChoiceWidgetTemplete * setTemplete, QMap<QString, QString> setBeforMap, QMap<QString, QString> setAfterMap, QList<int> beforChooseList, QList<int> afterChooseList);
	void initBeforObjList(QList<ViewObj *> * beforDefaultList, QList<ViewObj *> * beforStrList, QList<ChoiceItemTextWidget*>* beforeOptionTempleteList);
	void initAfterObjList(QList<ViewObj *> * afterDefaultList, QList<ViewObj *> * afterStrList, QList<ChoiceItemTextWidget*>* afterOptionTempleteList);
	//void initHaveAndNotHaveObjList(QList<ViewObj *> * beforHaveList, QList<ViewObj *> * beforNotHaveList, QList<ViewObj *> * afterHaveList, QList<ViewObj *> * afterNotHaveList);
	//static void copyObjListToObjList(QList<ViewObj *> * srcList, QList<ViewObj *> * destList, bool isClearDestList);
private:
	UnfoldChoiceWidgetTemplete * mSetTemplete;
	QMap<QString, QString> mSetBeforMap;
	QMap<QString, QString> mSetAfterMap;
	QList<ViewObj *> * mBeforDefaultObjList;//默认显示文字列表
	QList<ViewObj *> * mBeforStrObjList;//默认显示文字列表
	QList<ViewObj *> * mAfterDefaultObjList;//默认显示文字列表
	QList<ViewObj *> * mAfterStrObjList;//默认标题文字列表
	//QList<ViewObj *> * mBeforHaveStrObjList;//有无类型，有显示文字列表
	//QList<ViewObj *> * mAfterHaveStrObjList;//有无类型，有显示文字列表
	//QList<ViewObj *> * mBeforNotHaveStrObjList;//有无类型，无 显示文字列表
	//QList<ViewObj *> * mAfterNotHaveStrObjList;//有无类型，无 显示文字列表
	QList<ChoiceItemTextWidget*>* mBeforOptionTempleteList;
	QList<ChoiceItemTextWidget*>* mAfterOptionTempleteList;
	QList<int>  mBeforChooseList;    //选中列表
	QList<int>  mAfterChooseList;    //选中列表
	bool isUseBefor;
};
#endif