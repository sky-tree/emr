#ifndef _SET_TXT_STR_PARAM_ACTION_H_
#define _SET_TXT_STR_PARAM_ACTION_H_


#include <QList>

#include "BaseAction.h"

class OperateMgr;

class ViewObj;
class BaseContainer;
class BaseContentData;
class StrContentData;

// 显示相关属性
class SetTxtStrParamAction : public BaseAction
{
public:
	SetTxtStrParamAction(StepActionManger * mgr);

	virtual ~SetTxtStrParamAction();

public:

	void doAction(QList<BaseContainer *> * list, char type, QString value);

	void unDo();
	void reDo();

private:
	QString getPreValue(BaseContentData * source);
	void setParamByType(BaseContentData * source, char type, QString value);
	void setParamByTypeAndFreshLayOut(BaseContentData * source, char type, QString value,ViewObj* view);


public:

protected:
	QList<BaseContainer *> * mObjList;
	//QList<BaseContentData *> * mDataList;

	QList<QList<QString> *> * mPreValueList;
	// 文字设置类型
	char mActionType;
	// 目标值
	QString mDestValue;
};

#endif
// _SET_TXT_STR_PARAM_ACTION_H_
