#ifndef _DEL_OBJ_LIST_ACTION_H_
#define _DEL_OBJ_LIST_ACTION_H_


#include <QList>

#include "BaseAction.h"

class OperateMgr;

class ViewObj;
class BaseContainer;
class BaseParagraph;
class ParagraphData;

// 显示相关属性
class DelObjListAction : public BaseAction
{
public:
	DelObjListAction(StepActionManger * mgr);

	virtual ~DelObjListAction();

public:

	void doAction(BaseParagraph * nextParagraph, ViewObj * preEffectLogicPage, QList<BaseContainer *> * objList, ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir
		, ViewObj * afterFocus, char dir, bool isUpsidedown);

	void unDo();
	void reDo();

public:



public:

protected:
	QList<BaseContainer *> * mDelList;
	// 缓存的段落数据，用于粘贴redo时,保证复制段落属性一致
	QList<ParagraphData *> * mBufParagraphData;

	// 操作前、后逻辑页，主要用于表格跨页处理
	int mPreEffectLogicPageIndex;
	ViewObj * mAfterEffectLogicPage;

	// 添加时按下及松开光标
	ViewObj * mStartFocus;
	char mStartDir;

	ViewObj * mEndFocus;
	char mEndDir;

	ViewObj * mAfterFocus;
	char mAfterDir;

	bool mIsUpsidedown;
};

#endif
// _DEL_OBJ_LIST_ACTION_H_
