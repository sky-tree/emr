#include "UIMsgSetTxtMultipleIntParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "ConstantsID.h"
#include "BaseConstants.h"
#include "GlobalTemplate.h"

UIMsgSetTxtMultipleIntParams::UIMsgSetTxtMultipleIntParams(WriterManager * writerMgr, char type, QList<int> * valueList)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_SET_TXT_MULTIPLE_INT_PARAMS)
, mType(type)
, mValueList(valueList)
{
}

UIMsgSetTxtMultipleIntParams::~UIMsgSetTxtMultipleIntParams()
{
	if (mValueList)
	{
		mValueList->clear();
		delete mValueList;
		mValueList = NULL;
	}
}
void UIMsgSetTxtMultipleIntParams::doAction()
{
	SystemParams * sysParams = mOperateMgr->getSystemParams();
	switch (mType)
	{
	case BaseConstants::TxtParamActionType::SET_UNDERLINE:
		mOperateMgr->setUnderlineParam(mValueList->at(0), mValueList->at(1));
		break;
	case BaseConstants::TxtParamActionType::SET_DEL_LINE:
		mOperateMgr->setDelLineParam(mValueList->at(0), mValueList->at(1));
		break;
	}
}