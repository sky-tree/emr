#ifndef _STEP_ACTION_MANGER_H_
#define _STEP_ACTION_MANGER_H_

#include "qlist.h"

#include "BaseElement.h"

class OperateMgr;
class BaseAction;

class StepActionManger : BaseElement
{
public:
	StepActionManger(OperateMgr * mgr);
	~StepActionManger();

public:
	OperateMgr * getOperateMgr();
	
	bool haveUsed();
	void setHaveUsed(bool haveUsed);

	void unDo();
	void reDo();

	void addAction(BaseAction * action, int order = 0);
	void clearLocalAction(QList<BaseAction *> * localAction);
	//void addAction(BaseAction * action, int order = 0);
	//BaseAction * getLocaltionLastAction();
	QList<QList<BaseAction *> *>*  getActionList();
	BaseAction * getLastAction();
	QList<BaseAction *>  * getLocaltionAction();
	QList<QList<BaseAction *> *>* mActionList;
	OperateMgr * mOperateMgr;

	bool mHaveUsed;
};
#endif