#ifndef _SET_PARAGRAPH_FLOAT_PARAM_ACTION_H_
#define _SET_PARAGRAPH_FLOAT_PARAM_ACTION_H_


#include <QList>

#include "BaseAction.h"

class OperateMgr;

class ViewObj;
class BaseContainer;
class ParagraphData;

// 显示相关属性
class SetParagraphFloatParamAction : public BaseAction
{
public:
	SetParagraphFloatParamAction(StepActionManger * mgr);

	virtual ~SetParagraphFloatParamAction();

public:

	void doAction(QList<BaseContainer *> * list, char type, float value, ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir, bool isUpsidedown);

	void unDo();
	void reDo();

private:
	float getValue(ParagraphData * source, char type);



public:

protected:
	QList<BaseContainer *> * mChooseList;
	// 被设置的段落数据列表(缓存的数据指针)
	//QList<ParagraphData *> * mParagraphDataList;
	// 设置前段落数据列表（设置前段落数据的拷贝）
	QList<float > * mPreParagraphValueList;
	// 设置属性类型
	char mType;
	// 目标值
	int mDestValue;

	// 执行前按下及松开光标
	ViewObj * mStartFocus;
	char mStartDir;

	ViewObj * mEndFocus;
	char mEndDir;

	bool mIsUpsidedown;
};

#endif
// _SET_PARAGRAPH_FLOAT_PARAM_ACTION_H_
