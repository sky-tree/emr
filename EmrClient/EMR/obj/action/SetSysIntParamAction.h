#ifndef _SET_SYS_INT_PARAM_ACTION_H_
#define _SET_SYS_INT_PARAM_ACTION_H_


#include <QList>

#include "BaseAction.h"

class OperateMgr;

class ViewObj;
class BaseContainer;
class StrContentData;

// 显示相关属性
class SetSysIntParamAction : public BaseAction
{
public:
	SetSysIntParamAction(StepActionManger * mgr);

	virtual ~SetSysIntParamAction();

public:

	void doAction(char type, int value);

	void unDo();
	void reDo();

private:
	int getPreValue();
	void setParamByType(char type, int value);



public:

protected:
	// 文字设置类型
	char mActionType;
	// 目标值
	int mDestValue;
	// 初始值
	int mPreValue;
};

#endif
// _SET_SYS_INT_PARAM_ACTION_H_
