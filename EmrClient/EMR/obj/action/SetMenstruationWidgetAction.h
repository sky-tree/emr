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
	QList<ViewObj *> * mBeforTopObjList;//默认显示文字列表
	QList<ViewObj *> * mBeforDownObjList;//默认显示文字列表
	QList<ViewObj *> * mAfterTopObjList;//默认显示文字列表
	QList<ViewObj *> * mAfterDownObjList;//默认显示文字列表
	bool isUseBefor;
};

#endif