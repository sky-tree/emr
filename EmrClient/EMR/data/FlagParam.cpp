#include "FlagParam.h"

#include "ViewObj.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

FlagParam::FlagParam()
{
	mFocusObj = NULL;
}

FlagParam::~FlagParam()
{
	mFocusObj = NULL;
}

char FlagParam::getDirection()
{
	return mDirection;
}
bool FlagParam::isSame(FlagParam * source)
{
	return (mFocusObj == source->mFocusObj && mDirection == source->mDirection);
}
ViewObj * FlagParam::getFocusObj()
{
	return mFocusObj;
}
void FlagParam::setParam(ViewObj * focus, char dir)
{
	mFocusObj = focus;
	mDirection = dir;
}
void FlagParam::setParam(FlagParam * source)
{
	mFocusObj = source->mFocusObj;
	mDirection = source->mDirection;
}
//void FlagParam::setDirection(char dir)
//{
//	mDirection = dir;
//}