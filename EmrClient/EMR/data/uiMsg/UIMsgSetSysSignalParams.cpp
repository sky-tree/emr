#include "UIMsgSetSysSignalParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "SystemParams.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

UIMsgSetSysSignalParams::UIMsgSetSysSignalParams(WriterManager * writerMgr, int type)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_SET_SYS_SIGNAL_PARAMS)
, mType(type)
{
}

UIMsgSetSysSignalParams::~UIMsgSetSysSignalParams()
{
}
void UIMsgSetSysSignalParams::doAction()
{
	switch (mType)
	{
	case BaseConstants::SetSystemParamType::ADD_CURRENT_TIME:
		mOperateMgr->addCurrentTime();
		break;
	case BaseConstants::SetSystemParamType::DO_PRINT:
		mOperateMgr->doPrint();
		break;
	case BaseConstants::SetSystemParamType::UNDO:
		mOperateMgr->doUndo();
		break;
	case BaseConstants::SetSystemParamType::REDO:
		mOperateMgr->doRedo();
		break;
	case BaseConstants::SetSystemParamType::DO_COPY:
		mOperateMgr->doCopy();
		break;
	case BaseConstants::SetSystemParamType::DO_CUT:
		mOperateMgr->doCut();
		break;
	case BaseConstants::SetSystemParamType::DO_PAST:
		mOperateMgr->doPast();
		break; 
	case BaseConstants::SetSystemParamType::DO_SAVE:
		//mOperateMgr->doSave();
		break;
	case BaseConstants::SetSystemParamType::SELECT_ALL:
		mOperateMgr->selectAll();
		break;
	}
}