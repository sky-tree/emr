#include "WriterLogicMgr.h"

#include <QDebug>



#include "MyAppEntrance.h"
#include "BaseLogic.h"

#include "BaseTools.h"
#include "BaseConstants.h"

WriterLogicMgr::WriterLogicMgr() :
mRunningList(NULL)
{
	//mRunningList = new QList<BaseContainer *>();
}


WriterLogicMgr::~WriterLogicMgr()
{
	if (mRunningList)
	{
		mRunningList->clear();
		delete mRunningList;
		mRunningList = NULL;
	}
}

void WriterLogicMgr::addToRunningList(BaseLogic * logic)
{
	if (!mRunningList)
	{
		mRunningList = new QList<BaseLogic *>();
	}
	if (mRunningList->indexOf(logic) >= 0)
	{
		return;
	}
	mRunningList->push_back(logic);
}
void WriterLogicMgr::removeFromRunningList(BaseLogic * logic)
{
	if (!mRunningList)
	{
		return;
	}
	mRunningList->removeOne(logic);
}
void WriterLogicMgr::clearRunningList()
{
	if (!mRunningList)
	{
		return;
	}
	mRunningList->clear();
}
void WriterLogicMgr::myUpdate()
{
	//BaseTools::println("WriterLogicMgr::myUpdate() 111");
	int size = mRunningList->size();
	BaseLogic * logic = NULL;
	for (int i = 0; i < size; i ++)
	{
		logic = mRunningList->at(i);
		logic->myUpdate();
	}
	//BaseTools::println("WriterLogicMgr::myUpdate() 222");
}