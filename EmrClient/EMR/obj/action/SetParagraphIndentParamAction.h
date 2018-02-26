#ifndef _SET_PARAGRAPH_INDENT_PARAM_ACTION_H_
#define _SET_PARAGRAPH_INDENT_PARAM_ACTION_H_


#include <QList>

#include "BaseAction.h"

class OperateMgr;

class ViewObj;
class BaseContainer;
class ParagraphData;

// 显示相关属性
class SetParagraphIndentParamAction : public BaseAction
{
public:
	SetParagraphIndentParamAction(StepActionManger * mgr);

	virtual ~SetParagraphIndentParamAction();

public:

	void doAction(QList<BaseContainer *> * chooseList, int inL, int inR, char indentType, int indentValue, ViewObj * startFocus, char startDir
		, ViewObj * endFocus, char endDir, bool isUpsidedown);

	void unDo();
	void reDo();

public:



public:

protected:
	QList<BaseContainer *> * mChooseList;
	// 被设置的段落数据列表(缓存的数据指针)
	QList<ParagraphData *> * mParagraphDataList;
	// 设置前段落数据列表（设置前段落数据的拷贝）
	QList<ParagraphData *> * mPreParagraphDataListCopy;
	// 设置后段落属性
	int mDestInsideLeft;
	int mDestInsideRight;
	char mDestIndentType;
	int mDestIndentValue;

	// 执行前按下及松开光标
	ViewObj * mStartFocus;
	char mStartDir;

	ViewObj * mEndFocus;
	char mEndDir;

	bool mIsUpsidedown;
};

#endif
// _SET_PARAGRAPH_INDENT_PARAM_ACTION_H_
