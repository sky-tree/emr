#include "BasePointData.h"


BasePointData::BasePointData(int x, int y)
{
	mX = x;
	mY = y;
}
BasePointData::BasePointData(BasePointData * source)
{
	mX = source->mX;
	mY = source->mY;
}

BasePointData::~BasePointData()
{
}