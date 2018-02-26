#include "UIMsgSetPageParams.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "SystemParams.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

UIMsgSetPageParams::UIMsgSetPageParams(WriterManager * writerMgr, bool isHorizontal, short paperType, int pageInsideTop, int pageInsideBottom, int pageInsideLeft, int pageInsideRight
	, int titleMaxH, int bottomMaxH)
	: BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_SET_PAGE_PARAMS)
	, mIsHorizontal(isHorizontal)
	, mPaperType(paperType)
	, mPageInsideTop(pageInsideTop)
	, mPageInsideBottom(pageInsideBottom)
	, mPageInsideLeft(pageInsideLeft)
	, mPageInsideRight(pageInsideRight)
	, mTitleMaxH(titleMaxH)
	, mBottomMaxH(bottomMaxH)
{
}

UIMsgSetPageParams::~UIMsgSetPageParams()
{
}
void UIMsgSetPageParams::doAction()
{
	mOperateMgr->setPageParams(true, mIsHorizontal, mPaperType, mPageInsideTop, mPageInsideBottom, mPageInsideLeft, mPageInsideRight, mTitleMaxH, mBottomMaxH);
}