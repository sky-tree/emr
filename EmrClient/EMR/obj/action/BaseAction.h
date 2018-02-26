#ifndef _BASE_ACTION_H_
#define _BASE_ACTION_H_


#include "BaseElement.h"

class OperateMgr;
class StepActionManger;
class FlagParam;
// 显示相关属性
class BaseAction : public BaseElement
{
public:
	BaseAction(StepActionManger * mgr, int classId);
	void initPressStartAndEndParam();
	void setPressStartAndEndParam(FlagParam* startPress, FlagParam* endPress);
	virtual ~BaseAction();

public:
	virtual void unDo() = 0;
	virtual void reDo() = 0;

public:



public:

protected:
protected:
	OperateMgr * mOperateMgr;
	StepActionManger * mStepMgr;
	FlagParam * mSavedPressStart;
	FlagParam * mSavedPressEnd;
};

#endif
// _BASE_ACTION_H_
