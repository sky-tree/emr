#include "AutoSaveMgr.h"

#include <QDebug>


#include "WriterManager.h"
#include "OperateMgr.h"
#include "SystemParams.h"

#include "ViewObj.h"
#include "ObjKeyManager.h"
#include "ObjMouseManager.h"

#include "ParagraphData.h"
#include "StrContentData.h"

#include "MyAppEntrance.h"
#include "BaseConstants.h"

#include "BaseMath.h"

AutoSaveMgr::AutoSaveMgr(OperateMgr * mgr)
{
	mOperateMgr = mgr;
	changeStatus(BaseConstants::NONE, true);
}


AutoSaveMgr::~AutoSaveMgr()
{
	mOperateMgr = NULL;
}
void AutoSaveMgr::changeStatus(char status, char clearTimeType)
{
	char preStatus = mStatus;
	mStatus = status;
	if (clearTimeType == BaseConstants::AutoSaveClearTimeType::CLEAR_ABS)
		mWholeTime = 0;
	else if (clearTimeType == BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE && preStatus != mStatus)
	{
		mWholeTime = 0;
	}
}

void AutoSaveMgr::myUpdate()
{
	if (mStatus == BaseConstants::NONE)
		return;
	mWholeTime++;

	if (mWholeTime >= BaseConstants::AUTO_SAVE_DIS_TIME)
	{
		mWholeTime = 0;
		//mOperateMgr->autoSave();
	}
}