#include "ActionMgr.h"

#include "OperateMgr.h"


#include "ViewObj.h"

#include "BaseAction.h"

#include "StepActionManger.h"

#include "ConstantsID.h"
#include "BaseConstants.h"
#include "GlobalTemplate.h"

#include "BaseTools.h"

ActionMgr::ActionMgr(OperateMgr * mgr) : BaseElement(ConstantsID::ClassId::ACTION_MANAGER)
, mOperateMgr(mgr)
{
	mCurActionIdx = -1;
	mTotalActionList = new  QList<StepActionManger *>();
}

ActionMgr::~ActionMgr()
{
	mOperateMgr = NULL;
	if (mTotalActionList)
	{
		deleteVector(mTotalActionList);
		mTotalActionList = NULL;
	}
}
int ActionMgr::getCurIndex()
{
	return mCurActionIdx;
}
void ActionMgr::createNewAction()
{
	if (mCurActionIdx < 0)
		mCurActionIdx = 0;
	else
		mCurActionIdx++;
	int size = mTotalActionList->size();
	StepActionManger * stepMgr = NULL;
	// 将当前后续步骤清空
	if (mCurActionIdx < size)
	{
		for (int i = mCurActionIdx; i < size; i++)
		{
			stepMgr = mTotalActionList->at(mCurActionIdx);
			delete stepMgr;
			mTotalActionList->removeAt(mCurActionIdx);
		}
	}


	stepMgr = new StepActionManger(mOperateMgr);
	mTotalActionList->push_back(stepMgr);
}
void ActionMgr::delAction()
{
	StepActionManger  * stepMgr = mTotalActionList->at(mCurActionIdx);
	if (stepMgr)
	{
		delete stepMgr;
	 }

	mTotalActionList->removeAt(mCurActionIdx);
	mCurActionIdx--;
}
void ActionMgr::clearAllAction()
{
	mTotalActionList->clear();
	mCurActionIdx = -1;
}
void ActionMgr::deleteAllAction()
{
	int size = mTotalActionList->size();
	StepActionManger * stepMgr = NULL;
	int idx = mTotalActionList->size() - 1;
	for (int i = idx; i >= 0; i--)
	{
		stepMgr = mTotalActionList->at(idx);
		delete stepMgr;
		mTotalActionList->removeAt(idx);
		idx = mTotalActionList->size() - 1;
	}
	mTotalActionList->clear();
	mCurActionIdx = -1;
}
void ActionMgr::addAction(BaseAction * action)
{
	StepActionManger * stepMgr = mTotalActionList->at(mCurActionIdx);
	stepMgr->addAction(action);
}
StepActionManger * ActionMgr::getCurStepMgr()
{
	return mTotalActionList->at(mCurActionIdx);
}
QList<BaseAction *> * ActionMgr::getCurActionList()
{
	StepActionManger * mgr = mTotalActionList->at(mCurActionIdx);
	return	mgr->getLocaltionAction();
}
BaseAction * ActionMgr::getCurLastAction()
{
	 StepActionManger * mgr = mTotalActionList->at(mCurActionIdx);
	 return mgr->getLastAction();
}
void ActionMgr::addToLocalAction(BaseAction * action)
{
	//QList < BaseAction *> * localAction = NULL;
	StepActionManger * localAction = NULL;
	localAction = mTotalActionList->at(mCurActionIdx);
	localAction->addAction(action);
	//localAction->add(action);
}
void ActionMgr::addToLocalActionAt(BaseAction * action, int executeOrder)
{
	StepActionManger * localAction = NULL;
	localAction = mTotalActionList->at(mCurActionIdx);
	localAction->addAction(action, executeOrder);
}
bool ActionMgr::undo(bool needDel)
{
	if (mCurActionIdx < 0)
		return true;
	
	mTotalActionList->at(mCurActionIdx)->unDo();
	if (needDel)
	{
		StepActionManger * stepMgr = mTotalActionList->at(mCurActionIdx);
		delete stepMgr;
		mTotalActionList->removeAt(mCurActionIdx);
	}

	mCurActionIdx--;
	if (mCurActionIdx < 0)
		return true;
	return false;
}
//bool ActionMgr::unDo(int idx)
//{
//	if (mCurActionIdx <= idx)
//		return true;
//
//	mTotalActionList->at(mCurActionIdx)->unDo();
//	mCurActionIdx--;
//	if (mCurActionIdx <= idx)
//		return true;
//	return false;
//}
bool ActionMgr::redo()
{
	if (mCurActionIdx >= mTotalActionList->size() - 1)
		return true;
	mCurActionIdx++;
	mTotalActionList->at(mCurActionIdx)->reDo();
	if (mCurActionIdx >= mTotalActionList->size() - 1)
		return true;
	return false;
}
//bool ActionMgr::reDo(int idx)
//{
//	
//	if (mCurActionIdx >= idx)
//		return true;
//	mCurActionIdx++;
//	mTotalActionList->at(mCurActionIdx)->reDo();
//	if (mCurActionIdx >= idx)
//		return true;
//	return false;
//}