#include "UIMsgSetSysStrParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "SystemParams.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

UIMsgSetSysStrParams::UIMsgSetSysStrParams(WriterManager * writerMgr, char type, QString value)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_SET_SYS_STR_PARAMS)
, mType(type)
, mValue(value)
{
}

UIMsgSetSysStrParams::~UIMsgSetSysStrParams()
{
}
void UIMsgSetSysStrParams::doAction()
{
	switch (mType)
	{
	case BaseConstants::SetSystemParamType::ADD_SPECIAL_TXT:
		mOperateMgr->addStr(mValue, true);
		break;
	case BaseConstants::SetSystemParamType::COPY_TXT_FROM_OUTSIDE:
		mOperateMgr->createNewAction();
		mOperateMgr->addStr(mValue, false);
		break;
	case BaseConstants::SetSystemParamType::ADD_IMG:
		mOperateMgr->addImg(mValue);
		break;
	}
}