#include "UIMsgSetTxtIntParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "SystemParams.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

UIMsgSetTxtIntParams::UIMsgSetTxtIntParams(WriterManager * writerMgr, char type, int value)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_SET_TXT_INT_PARAMS)
, mType(type)
, mValue(value)
{
}

UIMsgSetTxtIntParams::~UIMsgSetTxtIntParams()
{
}
void UIMsgSetTxtIntParams::doAction()
{
	switch (mType)
	{
	case BaseConstants::TxtParamActionType::SET_FONT_COLOR:
		mOperateMgr->setFontColor(mValue);
		break;
	case BaseConstants::TxtParamActionType::SET_BACKGROUND__COLOR:
		mOperateMgr->setBackgroundColor(mValue);
		break;
	case BaseConstants::TxtParamActionType::SET_FONT_SIZE:
		mOperateMgr->setFontSize(mValue);
		break;
	case BaseConstants::TxtParamActionType::SET_FLOW_TYPE:
		mOperateMgr->setFlowType(mValue);
		break;
	}
}