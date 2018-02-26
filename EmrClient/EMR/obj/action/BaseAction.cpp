#include "BaseAction.h"

#include "StepActionManger.h"

#include "OperateMgr.h"
#include "FlagParam.h"

BaseAction::BaseAction(StepActionManger * mgr, int classId) : BaseElement(classId)
, mStepMgr(mgr)
, mSavedPressStart(NULL)
, mSavedPressEnd(NULL)
{
	mOperateMgr = mStepMgr->getOperateMgr();
}
BaseAction::~BaseAction()
{
	mOperateMgr = NULL;
	if (mSavedPressStart)
	{
		delete mSavedPressStart;
		mSavedPressStart = NULL;
	}
	if (mSavedPressEnd)
	{
		delete mSavedPressEnd;
		mSavedPressEnd = NULL;
	}
}
void BaseAction::initPressStartAndEndParam()
{
	if (!mSavedPressStart)
	{
		mSavedPressStart = new FlagParam();
	}
	if (!mSavedPressEnd)
	{
		mSavedPressEnd = new FlagParam();
	}

}
void BaseAction::setPressStartAndEndParam(FlagParam* startPress, FlagParam* endPress)
{
	initPressStartAndEndParam();
	mSavedPressStart->setParam(startPress);
	mSavedPressEnd->setParam(endPress);
}