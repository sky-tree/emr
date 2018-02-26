#ifndef _ADD_OBJ_LIST_ACTION_H_
#define _ADD_OBJ_LIST_ACTION_H_


#include <QList>

#include "BaseAction.h"

class OperateMgr;

class ViewObj;
class BaseContainer;
class ParagraphData;

// 显示相关属性
class AddObjListAction : public BaseAction
{
public:
	AddObjListAction(StepActionManger * mgr);

	virtual ~AddObjListAction();

public:

	void doAction(QList<BaseContainer *> * objList, ViewObj * beforeFocus, char beforeDir);

	void unDo();
	void reDo();

public:



public:

protected:
	QList<BaseContainer *> * mAddList;
	// 缓存的段落数据，用于粘贴redo时,保证复制段落属性一致
	QList<ParagraphData *> * mBufParagraphData;

	// 操作前、后逻辑页，主要用于表格跨页处理
	ViewObj * mPreEffectLogicPage;
	ViewObj * mAfterEffectLogicPage;
	// 添加时按下及松开光标
	ViewObj * mBeforeFocus;
	char mBeforeDir;

	//ViewObj * mEndFocus;
	//char mEndDir;
};

#endif
// _ADD_OBJ_LIST_ACTION_H_
