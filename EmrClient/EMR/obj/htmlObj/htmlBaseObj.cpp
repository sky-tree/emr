#include "htmlBaseObj.h"

htmlBaseObj::htmlBaseObj(int classId) : BaseElement(classId)
,mX(0)
, mY(0)
, mType(0)
, mWidth(0)
, mHeight(0)
, mByteArray(NULL)
, mIsBold(false)
, mIsItalic(false)
{
	mStr.clear();
}

htmlBaseObj::~htmlBaseObj()
{
	if (mByteArray)
	{
		delete mByteArray;
		mByteArray = NULL;
	}
}
void htmlBaseObj::setType(char type)
{
	mType = type;
}
char htmlBaseObj::getType()
{
	return mType;
}
void htmlBaseObj::setStr(QString str)
{
	mStr = str;
}
QString htmlBaseObj::getStr()
{
	return mStr;
}
void htmlBaseObj::setPicturePath(QString path)
{
	mStr = path;
}
QString htmlBaseObj::getPicturePath()
{
	return mStr;
}
void htmlBaseObj::setWidth(int width)
{
	mWidth = width*ratio;
}
int htmlBaseObj::getWidth()
{
	return mWidth;
}
void htmlBaseObj::setHeight(int height)
{
	mHeight = height;
}
int htmlBaseObj::getHeight()
{
	return mHeight;
}
void htmlBaseObj::setStartPoint(int x, int y)
{
	mStartPoint.setX(x*ratio);
	mStartPoint.setY(y);
}
void htmlBaseObj::setEndPoint(int x, int y)
{
	mEndPoint.setX(x*ratio);
	mEndPoint.setY(y);
}
QPoint htmlBaseObj::getStartPoint()
{
	return mStartPoint;
}
QPoint htmlBaseObj::getEndPoint()
{
	return mEndPoint;
}
void htmlBaseObj::setFontSize(int size)
{
	mFontSize = size;
}
int htmlBaseObj::getFontSize()
{
	return mFontSize;
}
void htmlBaseObj::setFontColor(QColor color)
{
	mFontColor=color;
}
QColor htmlBaseObj::getFontColor()
{
	return mFontColor;
}
void htmlBaseObj::setX(int x)
{
	mX = x*ratio;
}
void htmlBaseObj::setY(int y)
{
	mY = y;
}
int htmlBaseObj::getX()
{
	return mX;
}
int htmlBaseObj::getY()
{
	return mY;
}
QByteArray * htmlBaseObj::getByteArray()
{
	if (!mByteArray)
	{
		mByteArray = new QByteArray();
	}
	return mByteArray;
}
void htmlBaseObj::setBold(bool isBold)
{
	mIsBold = isBold;
}
bool htmlBaseObj::getBold()
{
	return mIsBold;
}
void htmlBaseObj::setItalic(bool isItalic)
{
	mIsItalic = isItalic;
}
bool htmlBaseObj::getItalic()
{
	return mIsItalic;
}