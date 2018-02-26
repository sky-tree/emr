#ifndef _SET_TXT_MULTIPLE_NUM_PARAM_ACTION_H_
#define _SET_TXT_MULTIPLE_NUM_PARAM_ACTION_H_


#include <QList>

#include "BaseAction.h"

class OperateMgr;

class ViewObj;
class BaseContainer;
class StrContentData;

// 显示相关属性
class SetTxtMultipleNumParamAction : public BaseAction
{
public:
	SetTxtMultipleNumParamAction(StepActionManger * mgr);

	virtual ~SetTxtMultipleNumParamAction();

public:

	void doAction(QList<BaseContainer *> * list, char type, int valueNum, int * value);

	void unDo();
	void reDo();

private:
	int * getPreValue(StrContentData * source);
	void setParamByType(StrContentData * source, int * value);



public:

protected:
	QList<BaseContainer *> * mObjList;
	//QList<StrContentData *> * mDataList;
	QList<QList<int *> *> * mPreValueList;
	// 文字设置类型
	char mActionType;
	// 目标值
	int * mDestValue;

	int mValueNum;
};

#endif
// _SET_TXT_MULTIPLE_NUM_PARAM_ACTION_H_
