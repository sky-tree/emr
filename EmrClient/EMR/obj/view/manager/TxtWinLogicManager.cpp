#include "TxtWinLogicManager.h"



#include "ViewObj.h"
#include "OperateMgr.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "GlobalTemplate.h"

TxtWinLogicManager::TxtWinLogicManager(BaseContainer * owner) : LogicManager(owner, ConstantsID::ClassId::TXT_WIN_LOGIC_MANAGER)
{
}

TxtWinLogicManager::~TxtWinLogicManager()
{
}
int TxtWinLogicManager::myUpdate()
{
	ViewObj * flag = mOwner->getOperateMgr()->getFlag();
	int returnType = flag->myUpdate();
	if (returnType != BaseConstants::NONE)
	{
		return returnType;
	}
	
	return LogicManager::myUpdate();
}