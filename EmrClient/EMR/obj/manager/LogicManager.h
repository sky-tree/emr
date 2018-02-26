#ifndef LOGIC_MANAGER_H_
#define LOGIC_MANAGER_H_


#include <QList>


class BaseLogic;
#include "BaseContainerParam.h"


class LogicManager : public BaseContainerParam
{
public:
	LogicManager(BaseContainer * owner);
	LogicManager(BaseContainer * owner, int classID);
	~LogicManager();

public:
	void addLogic(BaseLogic * logic);
	void doRefreshLogic();
	BaseLogic * getLogicByClassID(int id);
	virtual int myUpdate();

public:

	QList<BaseLogic *> * mRunningLogicList;
};

#endif
