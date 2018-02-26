#ifndef _TXT_WIN_LOGIC_MANAGER_H_
#define _TXT_WIN_LOGIC_MANAGER_H_


#include <QList>


#include "LogicManager.h"


class TxtWinLogicManager : public LogicManager
{
public:
	TxtWinLogicManager(BaseContainer * owner);
	~TxtWinLogicManager();

public:
	int myUpdate();

public:
};

#endif //_TXT_WIN_LOGIC_MANAGER_H_
