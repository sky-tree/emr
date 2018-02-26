#include "BaseTempleteMouse.h"


BaseTempleteMouse::BaseTempleteMouse(BaseTemplate* templeteOwner, int classid) :BaseElement(classid)
, mTempleteOwner(templeteOwner)
{

}


BaseTempleteMouse::~BaseTempleteMouse()
{
	if (mTempleteOwner)
	{
		mTempleteOwner = NULL;
	}
}


