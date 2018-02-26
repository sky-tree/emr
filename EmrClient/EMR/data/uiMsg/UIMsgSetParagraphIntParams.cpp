#include "UIMsgSetParagraphIntParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "ParagraphData.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

UIMsgSetParagraphIntParams::UIMsgSetParagraphIntParams(WriterManager * writerMgr, char type, int value)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_SET_PARAGRAPH_INT_PARAMS)
, mType(type)
, mDestValue(value)
{
}

UIMsgSetParagraphIntParams::~UIMsgSetParagraphIntParams()
{
}
void UIMsgSetParagraphIntParams::doAction()
{
	switch (mType)
	{
	case BaseConstants::UIInputType::SET_PARAGRAPH_HORIZON_ALIGN_TYPE:
		mOperateMgr->setHorizonAlignType(mDestValue);
		break;
	case BaseConstants::UIInputType::SET_EXCEL_VERTICAL_ALIGN_TYPE:
		mOperateMgr->setExcelParagraphAlignVerticalType(mDestValue);
		break;
	}
}