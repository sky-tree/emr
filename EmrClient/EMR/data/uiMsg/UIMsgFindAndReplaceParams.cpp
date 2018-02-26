#include "UIMsgFindAndReplaceParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "SystemParams.h"

#include "ConstantsID.h"
#include "BaseConstants.h"


UIMsgFindAndReplaceParams::UIMsgFindAndReplaceParams(WriterManager * writerMgr, char type, QString findStr)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_FIND_AND_REPLACE_PARAMS)
, mType(type)
, mFindStr(findStr)
{
}
UIMsgFindAndReplaceParams::UIMsgFindAndReplaceParams(WriterManager * writerMgr, char type, QString findStr, QString replaceStr)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_FIND_AND_REPLACE_PARAMS)
, mType(type)
, mFindStr(findStr)
, mReplaceStr(replaceStr)
{
}

UIMsgFindAndReplaceParams::~UIMsgFindAndReplaceParams()
{
}
void UIMsgFindAndReplaceParams::doAction()
{
	switch (mType)
	{
	case BaseConstants::SetSystemParamType::FIND_PRE_STR:
	case BaseConstants::SetSystemParamType::FIND_NEXT_STR:
		mOperateMgr->findStr(mType, mFindStr);
		break;
	case BaseConstants::SetSystemParamType::REPLACE_PRE_STR:
	case BaseConstants::SetSystemParamType::REPLACE_NEXT_STR:
	case BaseConstants::SetSystemParamType::REPLACE_ALL_STR:
		mOperateMgr->replaceStr(mType, mFindStr, mReplaceStr, true);
		break;
	}
}