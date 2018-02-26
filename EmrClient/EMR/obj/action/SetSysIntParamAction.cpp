#include "SetSysIntParamAction.h"

#include "OperateMgr.h"
#include "SystemParams.h"

#include "TempContainerMgr.h"

#include "StepActionManger.h"

#include "StrContentData.h"

#include "FlagParam.h"



#include "BaseContainer.h"
#include "ViewObj.h"

#include "ContentDataFactory.h"

#include "BaseTools.h"
#include "BaseSystem.h"
#include "ConstantsID.h"

SetSysIntParamAction::SetSysIntParamAction(StepActionManger * mgr) : BaseAction(mgr, ConstantsID::ClassId::SET_SYS_INT_PARAM_ACTOIN)
{
}

SetSysIntParamAction::~SetSysIntParamAction()
{
}
void SetSysIntParamAction::doAction(char type, int value)
{
	mActionType = type;
	mDestValue = value;
	mPreValue = getPreValue();
}
void SetSysIntParamAction::unDo()
{
	setParamByType(mActionType, mPreValue);
}
void SetSysIntParamAction::reDo()
{
	setParamByType(mActionType, mDestValue);
}
int SetSysIntParamAction::getPreValue()
{
	int preValue = 0;
	SystemParams * params = mOperateMgr->getSystemParams();
	switch (mActionType)
	{
	case BaseConstants::SetSystemParamType::SET_TITLE_NUMBER_VIEW_TYPE:
		preValue = params->getTitleNumViewType();
		break;
	case BaseConstants::SetSystemParamType::SET_BOTTOM_NUMBER_VIEW_TYPE:
		preValue = params->getBottomNumViewType();
		break;
	case BaseConstants::SetSystemParamType::SET_ROW_BOTTOM_LINE_TYPE:
		preValue = params->getRowBottomLineType();
		break;
	}
	return preValue;
}
void SetSysIntParamAction::setParamByType(char type, int value)
{
	SystemParams * params = mOperateMgr->getSystemParams();
	switch (type)
	{
	case BaseConstants::SetSystemParamType::SET_TITLE_NUMBER_VIEW_TYPE:
		params->setTitleNumberViewType(value, false);
		break;
	case BaseConstants::SetSystemParamType::SET_BOTTOM_NUMBER_VIEW_TYPE:
		params->setBottomNumberViewType(value, false);
		break;
	case BaseConstants::SetSystemParamType::SET_ROW_BOTTOM_LINE_TYPE:
		//params->setRowBottomLineType(value);
		mOperateMgr->setRowBottomLineType(value, false, true);
		break;
	}
}