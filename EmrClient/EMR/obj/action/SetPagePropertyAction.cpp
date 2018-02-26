#include "SetPagePropertyAction.h"



#include "OperateMgr.h"
#include "SystemParams.h"


#include "ConstantsID.h"
#include "WriterManager.h"

SetPagePropertyAction::SetPagePropertyAction(StepActionManger * stepActonManger) : BaseAction(stepActonManger, ConstantsID::ClassId::SET_PAGE_PROPERTY)
{
}


SetPagePropertyAction::~SetPagePropertyAction()
{
}
void SetPagePropertyAction::doAction(bool isHorizontal, short paperType, int pageInsideT, int pageInsideB, int pageInsideL, int pageInsideR
	, int titleMaxH, int bottomMaxH)
{
	SystemParams * sysParams = mOperateMgr->getSystemParams();
	mPreIsHorizontal = sysParams->isPaperHorizontal();
	mPrePaperType = sysParams->getPaperType();
	mPrePageInsideTop = sysParams->getPageInsideTop();
	mPrePageInsideBottom = sysParams->getPageInsideBottom();
	mPrePageInsideLeft = sysParams->getPageInsideLeft();
	mPrePageInsideRight = sysParams->getPageInsideRight();
	mPreTitleMaxH = sysParams->getTitleMaxH();
	mPreBottomMaxH = sysParams->getBottomMaxH();

	mIsHorizontal = isHorizontal;
	mPaperType = paperType;
	mPageInsideTop = pageInsideT;
	mPageInsideBottom = pageInsideB;
	mPageInsideLeft = pageInsideL;
	mPageInsideRight = pageInsideR;
	mTitleMaxH = titleMaxH;
	mBottomMaxH = bottomMaxH;
}
void SetPagePropertyAction::unDo()
{
	mOperateMgr->setPageParams(false, mPreIsHorizontal, mPrePaperType, mPrePageInsideTop, mPrePageInsideBottom, mPrePageInsideLeft, mPrePageInsideRight
		, mPreTitleMaxH, mPreBottomMaxH);
}

void SetPagePropertyAction::reDo()
{
	mOperateMgr->setPageParams(false, mIsHorizontal, mPaperType, mPageInsideTop, mPageInsideBottom, mPageInsideLeft, mPageInsideRight, mTitleMaxH, mBottomMaxH);
}