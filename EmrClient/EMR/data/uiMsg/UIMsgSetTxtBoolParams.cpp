#include "UIMsgSetTxtBoolParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

UIMsgSetTxtBoolParams::UIMsgSetTxtBoolParams(WriterManager * writerMgr, char type, bool value)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_SET_TXT_BOOL_PARAMS)
, mType(type)
, mValue(value)
{
}

UIMsgSetTxtBoolParams::~UIMsgSetTxtBoolParams()
{
}
void UIMsgSetTxtBoolParams::doAction()
{
	switch (mType)
	{
	case BaseConstants::TxtParamActionType::SET_BOLD:
		mOperateMgr->setFontBold(mValue);
		break;
	case BaseConstants::TxtParamActionType::SET_ITALIC:
		mOperateMgr->setFontItalic(mValue);
		break;
	}
}