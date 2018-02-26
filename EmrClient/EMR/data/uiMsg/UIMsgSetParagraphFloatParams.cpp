#include "UIMsgSetParagraphFloatParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "ParagraphData.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

UIMsgSetParagraphFloatParams::UIMsgSetParagraphFloatParams(WriterManager * writerMgr, char type, float value)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_SET_PARAGRAPH_FLOAT_PARAMS)
, mType(type)
, mDestValue(value)
{
}

UIMsgSetParagraphFloatParams::~UIMsgSetParagraphFloatParams()
{
}
void UIMsgSetParagraphFloatParams::doAction()
{
	switch (mType)
	{
	case BaseConstants::UIInputType::SET_PARAGRAPH_ROW_DIS_TIMES:
		mOperateMgr->setRowDisTimes(mDestValue);
		break;
	}
}