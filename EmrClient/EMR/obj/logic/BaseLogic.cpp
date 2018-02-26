#include "BaseLogic.h"

#include "ViewObj.h"

#include "BaseConstants.h"
#include "ConstantsNumber.h"

BaseLogic::BaseLogic(BaseContainer * owner, int classID) : BaseContainerParam(owner, classID)
, mTime(0)
, mIsBlock(false)
{
}

BaseLogic::~BaseLogic()
{
}
int BaseLogic::getTime()
{
	return mTime;
}
void BaseLogic::addTime()
{
	mTime++;
	if (mTime >= ConstantsNumber::MAX_INT)
		mTime = 0;
}
void BaseLogic::clearTime()
{
	mTime = 0;
}
void BaseLogic::setBlock(bool value)
{
	mIsBlock = value;
}
bool BaseLogic::isBlock()
{
	return mIsBlock;
}
void BaseLogic::refreshLogic()
{
}