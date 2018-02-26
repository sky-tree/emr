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
	QList<ViewObj *> * mBeforDefaultObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mBeforStrObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mAfterDefaultObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mAfterStrObjList;//Ĭ�ϱ��������б�
	//QList<ViewObj *> * mBeforHaveStrObjList;//�������ͣ�����ʾ�����б�
	//QList<ViewObj *> * mAfterHaveStrObjList;//�������ͣ�����ʾ�����б�
	//QList<ViewObj *> * mBeforNotHaveStrObjList;//�������ͣ��� ��ʾ�����б�
	//QList<ViewObj *> * mAfterNotHaveStrObjList;//�������ͣ��� ��ʾ�����б�
	QList<ChoiceItemTextWidget*>* mBeforOptionTempleteList;
	QList<ChoiceItemTextWidget*>* mAfterOptionTempleteList;
	QList<int>  mBeforChooseList;    //ѡ���б�
	QList<int>  mAfterChooseList;    //ѡ���б�
	bool isUseBefor;
};
#endif