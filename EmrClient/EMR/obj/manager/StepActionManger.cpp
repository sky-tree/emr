#include "StepActionManger.h"

#include "OperateMgr.h"

#include "BaseAction.h"

#include "ConstantsID.h"
#include "GlobalTemplate.h"
StepActionManger::StepActionManger(OperateMgr * mgr) : BaseElement(ConstantsID::ClassId::STEP_ACTION_MANAGER)
, mOperateMgr(mgr)
{
	mHaveUsed = true;
	mActionList = new  QList<QList<BaseAction *> *>();
}


StepActionManger::~StepActionManger()
{
	if (mActionList)
	{
		delete mActionList;
		mActionList = NULL;
	}
}
OperateMgr * StepActionManger::getOperateMgr()
{
	return mOperateMgr;
}
bool StepActionManger::haveUsed()
{
	return mHaveUsed;
}
void StepActionManger::setHaveUsed(bool haveUsed)
{
	mHaveUsed = haveUsed;
}
void StepActionManger::unDo()
{
	QList < BaseAction *> * localAction = NULL;
	BaseAction * tempAction = NULL;
	for (int i = 0; i < mActionList->size(); i++)
	{
		localAction = mActionList->at(i);
		if (!localAction)
		{
			continue;
		}
		int start = localAction->size() - 1;
		for (int m = start; m >= 0; m--)
		{
			tempAction = localAction->at(m);
			if (!tempAction)
			{
				continue;
			}
			tempAction->unDo();
		}
	}
}
void StepActionManger::reDo()
{
	QList < BaseAction *> * localAction = NULL;
	BaseAction * tempAction = NULL;
	for (int i = 0; i < mActionList->size(); i++)
	{
		localAction = mActionList->at(i);
		if (!localAction)
		{
			continue;
		}
		for (int m = 0; m < localAction->size(); m++)
		{
			tempAction = localAction->at(m);
			if (!tempAction)
			{
				continue;
			}
			tempAction->reDo();
		}
	}
}
void StepActionManger::addAction(BaseAction * action, int order)
{
	QList < BaseAction *> * localAction = NULL;
	int size = mActionList->size();
	for (int i = size; i <= order; i++)
	{
		localAction = new QList< BaseAction *>();
		mActionList->push_back(localAction);
	}
	localAction = mActionList->at(order);
	localAction->push_back(action);
}
QList<QList<BaseAction *> *>* StepActionManger::getActionList()
{
	return  mActionList;
}
BaseAction * StepActionManger::getLastAction()
{
	if (mActionList->size() == 0)
	{
		return NULL;
	}
	return  mActionList->at(mActionList->size() - 1)->last();
}
QList<BaseAction *>  *StepActionManger::getLocaltionAction()
{
	return  mActionList->at(mActionList->size() - 1);
}
void StepActionManger::clearLocalAction(QList<BaseAction *> * localAction)
{
	if (!localAction)
		return;
	deleteVector(localAction);
	//delete localAction;
	localAction = NULL;
}