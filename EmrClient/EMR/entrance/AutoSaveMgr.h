#ifndef _AUTO_SAVE_MGR_H_
#define _AUTO_SAVE_MGR_H_
#include "QObject.h"


#include <QString>

class WriterManager;
class OperateMgr;

// 自动存档管理器
class AutoSaveMgr
{
public:
	AutoSaveMgr(OperateMgr * mgr);
	~AutoSaveMgr();

public:
	void changeStatus(char status, char clearTimeType);

	void myUpdate();

public:

private:

private:
	OperateMgr * mOperateMgr;
	// 状态
	char mStatus;
	// 全局时间
	int mWholeTime;


};


#endif
