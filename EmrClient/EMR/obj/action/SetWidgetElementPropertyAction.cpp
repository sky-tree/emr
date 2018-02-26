#include "SetWidgetElementPropertyAction.h"
//#include "ConstantsID.h"
#include "iostream"
SetWidgetElementPropertyAction::SetWidgetElementPropertyAction(StepActionManger * stepActonManger, int classId) : BaseAction(stepActonManger, classId)
, mBeforFocus(NULL)
, mAfterFocus(NULL)
{
}


SetWidgetElementPropertyAction::~SetWidgetElementPropertyAction()
{
	if (mBeforFocus)
	{
		mBeforFocus = NULL;
	}
	if (mAfterFocus)
	{
		mAfterFocus = NULL;
	}
}
void SetWidgetElementPropertyAction::initFocus(ViewObj * beforFocus, ViewObj * aferFocus, char beforDir, char afterDir)
{
	mBeforFocus = beforFocus;
	mAfterFocus = aferFocus;
	mBeforDir = beforDir;
	mAfterDir = afterDir;
}