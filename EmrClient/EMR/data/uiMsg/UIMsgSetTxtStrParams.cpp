#include "UIMsgSetTxtStrParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

UIMsgSetTxtStrParams::UIMsgSetTxtStrParams(WriterManager * writerMgr, char type, QString str)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_SET_TXT_STR_PARAMS)
, mType(type)
, mString(str)
{
}

UIMsgSetTxtStrParams::~UIMsgSetTxtStrParams()
{
}
void UIMsgSetTxtStrParams::doAction()
{
	SystemParams * sysParams = mOperateMgr->getSystemParams();
	switch (mType)
	{
	case BaseConstants::TxtParamActionType::SET_FAMILY:
		mOperateMgr->setFontFamily(mString);
		break;
	}
}