#include "LogicManager.h"




#include "WriterManager.h"
#include "WriterLogicMgr.h"
#include "BaseLogic.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "GlobalTemplate.h"

LogicManager::LogicManager(BaseContainer * owner) : BaseContainerParam(owner, ConstantsID::ClassId::OBJ_LOGIC_MANAGER)
{
	mRunningLogicList = new QList<BaseLogic *>();
	owner->initLogicMgr(this);
}
LogicManager::LogicManager(BaseContainer * owner, int classID) : BaseContainerParam(owner, classID)
{
	mRunningLogicList = new QList<BaseLogic *>();
	owner->initLogicMgr(this);
}

LogicManager::~LogicManager()
{
	if (mRunningLogicList)
	{
		int size = mRunningLogicList->size();
		BaseLogic * logic = NULL;
		for (int i = 0; i < size; i ++)
		{
			logic = mRunningLogicList->at(i);
			mOwner->getWriterMgr()->getLogicMgr()->removeFromRunningList(logic);
			delete logic;
		}
		mRunningLogicList->clear();
		mRunningLogicList = NULL;
	}
}
void LogicManager::addLogic(BaseLogic * logic)
{
	if (mRunningLogicList->indexOf(logic) >= 0)
		return;
	mRunningLogicList->push_back(logic);
}
void LogicManager::doRefreshLogic()
{
	for (int i = 0; i < mRunningLogicList->size(); i++)
	{
		mRunningLogicList->at(i)->refreshLogic();
	}
}
BaseLogic * LogicManager::getLogicByClassID(int id)
{
	BaseLogic * logic = NULL;
	for (int i = 0; i < mRunningLogicList->size(); i++)
	{
		logic = mRunningLogicList->at(i);
		if (logic->getClassId() == id)
		{
			return logic;
		}
	}
	return NULL;
}
int LogicManager::myUpdate()
{
	int returnType = BaseConstants::NONE;

	if (mRunningLogicList)
	{
		int size = mRunningLogicList->size();
		for (int i = 0; i < size; i++)
		{
			BaseLogic * logic = mRunningLogicList->at(i);
			returnType = logic->myUpdate();
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
				return returnType;
		}
	}

	return returnType;
}