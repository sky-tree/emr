#include "BaseMouse.h"




#include "BaseContainer.h"
#include "ViewObj.h"

#include "ConstantsID.h"

BaseMouse::BaseMouse(ViewObj * owner, int classID) : BaseContainerParam(owner, classID)
{
	mViewOwner = owner;
}


BaseMouse::~BaseMouse()
{
}
void BaseMouse::releaseFocusFromWindow()
{

}
