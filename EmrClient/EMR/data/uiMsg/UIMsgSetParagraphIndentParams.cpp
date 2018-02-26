#include "UIMsgSetParagraphIndentParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "ParagraphData.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

UIMsgSetParagraphIndentParams::UIMsgSetParagraphIndentParams(WriterManager * writerMgr, int insideL, int insideR, char indentType, int indentValue)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_SET_PARAGRAPH_INDENT_PARAMS)
, mInsideLeft(insideL)
, mInsideRight(insideR)
, mIndentType(indentType)
, mIndentValue(indentValue)
{
}

UIMsgSetParagraphIndentParams::~UIMsgSetParagraphIndentParams()
{
}
void UIMsgSetParagraphIndentParams::doAction()
{
	mOperateMgr->setFocusParagraphIndentParam(mInsideLeft, mInsideRight, mIndentType, mIndentValue);
}