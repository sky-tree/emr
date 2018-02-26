#ifndef _SET_SYS_BOOL_PARAM_ACTION_H_
#define _SET_SYS_BOOL_PARAM_ACTION_H_


#include <QList>

#include "BaseAction.h"

class OperateMgr;

class ViewObj;
class BaseContainer;
class StrContentData;

// 显示相关属性
class SetSysBoolParamAction : public BaseAction
{
public:
	SetSysBoolParamAction(StepActionManger * mgr);

	virtual ~SetSysBoolParamAction();

public:

	void doAction(char type, bool value);

	void unDo();
	void reDo();

private:
	bool getPreValue();
	void setParamByType(char type, bool value);



public:

protected:
	// 文字设置类型
	char mActionType;
	// 目标值
	bool mDestValue;
	// 初始值
	bool mPreValue;
};

#endif
// _SET_SYS_BOOL_PARAM_ACTION_H_
