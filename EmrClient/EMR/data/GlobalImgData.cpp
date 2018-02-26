#include "GlobalImgData.h"



#include "BaseConstants.h"

GlobalImgData::GlobalImgData(QString imgName)
{
	mImgName = imgName;

	mImage = new QImage(imgName);

	mUseNum = 1;
}

GlobalImgData::~GlobalImgData()
{
	if (mImage)
	{
		delete mImage;
		mImage = NULL;
	}
}
bool GlobalImgData::isSameImg(QString name)
{
	if (mImgName == name)
		return true;
	return false;
}

bool GlobalImgData::delNum()
{
	mUseNum--;
	if (mUseNum < 1)
	{
		return true;
	}
	return false;
}
void GlobalImgData::addNum()
{
	mUseNum++;
}
QImage * GlobalImgData::getImg()
{
	return mImage;
}

QString GlobalImgData::getImgName()
{
	return mImgName;
}