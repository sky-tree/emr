#include "UIMsgSetSysIntParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "SystemParams.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

UIMsgSetSysIntParams::UIMsgSetSysIntParams(WriterManager * writerMgr, char type, int value)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_SET_SYS_INT_PARAMS)
, mType(type)
, mValue(value)
{
}

UIMsgSetSysIntParams::~UIMsgSetSysIntParams()
{
}
void UIMsgSetSysIntParams::doAction()
{
	switch (mType)
	{
	case BaseConstants::SetSystemParamType::SET_FILE_AUTHORITY:
		mOperateMgr->setFileAuthority(mValue);
		break;
	case BaseConstants::SetSystemParamType::SET_TITLE_NUMBER_VIEW_TYPE:
		mOperateMgr->setTitleNumberViewType(mValue);
		break;
	case BaseConstants::SetSystemParamType::SET_BOTTOM_NUMBER_VIEW_TYPE:
		mOperateMgr->setBottomNumberViewType(mValue);
		break;
	}
}