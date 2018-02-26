#include "LogicFactory.h"



#include "ViewObj.h"

#include "BaseLogic.h"
#include "FlagFlashLogic.h"
#include "JudgePagePanelScrollLogic.h"

#include "ConstantsID.h"



LogicFactory::LogicFactory()
{
}

LogicFactory::~LogicFactory()
{
}

BaseLogic * LogicFactory::createLogic(ViewObj * viewOwner)
{
	BaseLogic * logic = NULL;
	int objId = viewOwner->getObjID();
	switch (objId)
	{
	case ConstantsID::ContainerId::FLAG:
		logic = new FlagFlashLogic(viewOwner);
		break;
	case ConstantsID::ContainerId::TXT_WIN_VIEW:
		logic = new JudgePagePanelScrollLogic(viewOwner);
		break;
	}
	
	return logic;
}