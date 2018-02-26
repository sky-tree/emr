#include "FlagViewParam.h"




#include "ViewObj.h"

#include "TxtViewParam.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

FlagViewParam::FlagViewParam(ViewObj * owner) : BaseViewParam(owner, ConstantsID::ClassId::FLAG_VIEW_PARAM)
{
	mFocusObj = NULL;
	mDirection = BaseConstants::NONE;
}

FlagViewParam::~FlagViewParam()
{
}
void FlagViewParam::setFocus(ViewObj * focus, char dir)
{
	if (!focus)
	{
		int a = 0;
	}
	mFocusObj = focus;
	mDirection = dir;
}
void FlagViewParam::getFocusAndDirection(ViewObj * &focus, char &dir)
{
	focus = mFocusObj;
	dir = mDirection;
}
ViewObj * FlagViewParam::getFocus()
{
	return mFocusObj;
}
char FlagViewParam::getDirection()
{
	return mDirection;
}