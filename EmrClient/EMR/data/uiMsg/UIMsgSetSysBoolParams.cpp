#include "UIMsgSetSysBoolParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "SystemParams.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

UIMsgSetSysBoolParams::UIMsgSetSysBoolParams(WriterManager * writerMgr, char type, bool value)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_SET_SYS_BOOL_PARAMS)
, mType(type)
, mValue(value)
{
}

UIMsgSetSysBoolParams::~UIMsgSetSysBoolParams()
{
}
void UIMsgSetSysBoolParams::doAction()
{
	switch (mType)
	{
	case BaseConstants::SetSystemParamType::SET_TITLE_VIEW_TYPE:
		mOperateMgr->setTitleView(mValue);
		break;
	case BaseConstants::SetSystemParamType::SET_BOTTOM_VIEW_TYPE:
		mOperateMgr->setBottomView(mValue);
		break;
	case BaseConstants::SetSystemParamType::SET_PARAGRAPH_NUM_VIEW_TYPE:
		mOperateMgr->setParagraphNumberView(mValue);
		break;
	case BaseConstants::SetSystemParamType::SET_SHOW_TIDY:
		mOperateMgr->showTidy(mValue);
		break;
	}
}