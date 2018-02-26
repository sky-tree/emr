#ifndef _ACTION_MGR_H_
#define _ACTION_MGR_H_



#include <qDebug>

#include "BaseElement.h"

class OperateMgr;
class BaseAction;
class StepActionManger;

#include <QList>

// 操作痕迹管理器 
class ActionMgr : public BaseElement
{
public:
	ActionMgr(OperateMgr * mgr);
	~ActionMgr();

public:

	int getCurIndex();

	void createNewAction();
	void delAction();
	void clearAllAction();
	void deleteAllAction();
	void addAction(BaseAction * action);
	StepActionManger * getCurStepMgr();
	QList<BaseAction *> * getCurActionList();
	BaseAction * getCurLastAction();
	void addToLocalAction(BaseAction * action);
	void addToLocalActionAt(BaseAction * action, int executeOrder = 0);
	// 后退
	bool undo(bool needDel = false);
	//bool unDo(int idx);
	// 前进
	bool redo();
	//bool reDo(int idx);

public:

private:
	OperateMgr * mOperateMgr;
	// 当前动作索引
	int mCurActionIdx;


	QList<StepActionManger *> * mTotalActionList;
};

#endif // _ACTION_MGR_H_
