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
	QList<ViewObj *> * mBeforDefaultObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mBeforStrObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mAfterDefaultObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mAfterStrObjList;//Ĭ�ϱ��������б�
	QList<ViewObj *> * mBeforHaveStrObjList;//�������ͣ�����ʾ�����б�
	QList<ViewObj *> * mAfterHaveStrObjList;//�������ͣ�����ʾ�����б�
	QList<ViewObj *> * mBeforNotHaveStrObjList;//�������ͣ��� ��ʾ�����б�
	QList<ViewObj *> * mAfterNotHaveStrObjList;//�������ͣ��� ��ʾ�����б�
	QList<TextWidget*>* mBeforOptionTempleteList;
	QList<TextWidget*>* mAfterOptionTempleteList;
	QList<int>  mBeforChooseList;    //ѡ���б�
	QList<int>  mAfterChooseList;    //ѡ���б�
	bool isUseBefor;
};
#endif