#ifndef _SET_TXT_NUM_PARAM_ACTION_H_
#define _SET_TXT_NUM_PARAM_ACTION_H_


#include <QList>

#include "BaseAction.h"

class OperateMgr;

class ViewObj;
class BaseContainer;
class StrContentData;

// 显示相关属性
class SetTxtNumParamAction : public BaseAction
{
public:
	SetTxtNumParamAction(StepActionManger * mgr);

	virtual ~SetTxtNumParamAction();

public:

	void doAction(QList<BaseContainer *> * list, char type, int value);

	void unDo();
	void reDo();

private:
	int getPreValue(StrContentData * source);
	void setParamByType(StrContentData * source, char type, int value, bool & needReplace);



public:

protected:
	QList<BaseContainer *> * mObjList;
	QList<QList<int> *> * mPreValueList;
	int mFocusPreValue;
	// 文字设置类型
	char mActionType;
	// 目标值
	int mDestValue;
};

#endif
// _SET_TXT_NUM_PARAM_ACTION_H_
