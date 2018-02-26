#ifndef _SETMENSTRUATIONWIDGETACTION_H_
#define _SETMENSTRUATIONWIDGETACTION_H_
#include "SetWidgetElementPropertyAction.h"
#include "qlist.h"
class ViewObj;
class MenstruationTemplete;
class SetMenstruationWidgetAction :
	public SetWidgetElementPropertyAction
{
public:
	SetMenstruationWidgetAction(StepActionManger * stepActonManger);
	~SetMenstruationWidgetAction();
	void unDo();
	void reDo();
	void initBeforObjList(MenstruationTemplete* setTemplete,QList<ViewObj *> * beforTopList, QList<ViewObj *> * beforDownStrList);
	void initAfterObjList(QList<ViewObj *> * afterTopList, QList<ViewObj *> * afterDownStrList);
	MenstruationTemplete * mSetTemplete;
	QList<ViewObj *> * mBeforTopObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mBeforDownObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mAfterTopObjList;//Ĭ����ʾ�����б�
	QList<ViewObj *> * mAfterDownObjList;//Ĭ����ʾ�����б�
	bool isUseBefor;
};

#endif