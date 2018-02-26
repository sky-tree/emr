#include "BaseContainerParam.h"

#include "ImgContentData.h"
BaseContainerParam::BaseContainerParam(BaseContainer * owner, int classID) : BaseElement(classID)
, mOwner(owner)
{ 
}
BaseContainerParam::BaseContainerParam(BaseContainer * owner, BaseContainerParam* otherOne, int classID) : BaseElement(classID)
, mOwner(owner)
{

}
BaseContainerParam::~BaseContainerParam()
{
	mOwner = NULL;
}

BaseContainer * BaseContainerParam::getOwner()
{
	return mOwner;
}
void BaseContainerParam::initOwner(BaseContainer * owner)
{
	mOwner = owner;
}
