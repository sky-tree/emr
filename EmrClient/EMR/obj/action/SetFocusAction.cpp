#include "SetFocusAction.h"
#include "ConstantsID.h"
#include "iostream"
#include "ViewObj.h"
#include "StepActionManger.h"
#include "OperateMgr.h"
SetFocusAction::SetFocusAction(StepActionManger * stepActonManger) : BaseAction(stepActonManger, ConstantsID::ClassId::SET_FOCUS_ACTION)
{

}


SetFocusAction::~SetFocusAction()
{
	if (mPreFocus)
	{
		mPreFocus = NULL;
	}
	if (mAfterFocus)
	{
		mAfterFocus = NULL;
	}
}
void SetFocusAction::initAction(ViewObj * preFocus, char preDir, ViewObj * afterFocus, char afterDir)
{
	mPreFocus = preFocus;
	mPreDir = preDir;
	mAfterFocus = afterFocus;
	mAfterDir = afterDir;
}
void SetFocusAction::unDo()
{
	mStepMgr->getOperateMgr()->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE,mPreFocus, mPreDir);
}
void SetFocusAction::reDo()
{
	mStepMgr->getOperateMgr()->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mAfterFocus, mAfterDir);
}