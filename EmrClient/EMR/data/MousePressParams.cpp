#include "MousePressParams.h"


MousePressParams::MousePressParams():
mX(0)
, mY(0)
{
	mData.clear();
}

MousePressParams::~MousePressParams()
{
}
int MousePressParams::getX()
{
	return mX;
}
int MousePressParams::getY()
{
	return mY;
}
int MousePressParams::getType()
{
	return mType;
}
void MousePressParams::refresh(int type, int x, int y)
{
	mType = type;
	mX = x;
	mY = y;
}
void MousePressParams::refresh(int type, int x, int y, QMap<QString, QString>  data)
{
	refresh(type, x, y);
	mData = data;
}
QMap<QString, QString>  MousePressParams::getData()
{
	return mData;
}