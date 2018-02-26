#ifndef _WRITER_LOGIC_MGR_H_
#define _WRITER_LOGIC_MGR_H_
#include "QObject.h"



class BaseContainer;
class BaseLogic;

// 自动存档管理器
class WriterLogicMgr
{
public:
	WriterLogicMgr();
	~WriterLogicMgr();

public:

	void addToRunningList(BaseLogic * logic);
	void removeFromRunningList(BaseLogic * logic);
	void clearRunningList();

	void myUpdate();

public:


private:
	//OperateMgr * mOperateMgr;
	QList<BaseLogic *> * mRunningList;

};


#endif
