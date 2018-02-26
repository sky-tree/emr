#include "SetSysBoolParamAction.h"

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

SetSysBoolParamAction::SetSysBoolParamAction(StepActionManger * mgr) : BaseAction(mgr, ConstantsID::ClassId::SET_SYS_BOOL_PARAM_ACTOIN)
{
}

SetSysBoolParamAction::~SetSysBoolParamAction()
{
}
void SetSysBoolParamAction::doAction(char type, bool value)
{
	mActionType = type;
	mDestValue = value;
	mPreValue = getPreValue();
}
void SetSysBoolParamAction::unDo()
{
	setParamByType(mActionType, mPreValue);
}
void SetSysBoolParamAction::reDo()
{
	setParamByType(mActionType, mDestValue);
}
bool SetSysBoolParamAction::getPreValue()
{
	bool preValue = 0;
	SystemParams * params = mOperateMgr->getSystemParams();
	switch (mActionType)
	{
	case BaseConstants::SetSystemParamType::SET_TITLE_VIEW_TYPE:
		preValue = params->isTitleView();
		break;
	case BaseConstants::SetSystemParamType::SET_BOTTOM_VIEW_TYPE:
		preValue = params->isBottomView();
		break;
	case BaseConstants::SetSystemParamType::SET_PARAGRAPH_NUM_VIEW_TYPE:
		preValue = params->isParagraphNumView();
		break;
	}
	return preValue;
}
void SetSysBoolParamAction::setParamByType(char type, bool value)
{
	SystemParams * params = mOperateMgr->getSystemParams();
	switch (type)
	{
	case BaseConstants::SetSystemParamType::SET_TITLE_VIEW_TYPE:
		params->setTitleView(value);
		break;
	case BaseConstants::SetSystemParamType::SET_BOTTOM_VIEW_TYPE:
		params->setBottomView(value);
		break;
	case BaseConstants::SetSystemParamType::SET_PARAGRAPH_NUM_VIEW_TYPE:
		params->setParagraphNumberView(value);
		break;
	}
}