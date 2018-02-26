#include "RelativeChildrenParam.h"




#include "ViewObj.h"
#include "BaseViewParam.h"
#include "LayoutViewParam.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

RelativeChildrenParam::RelativeChildrenParam(ViewObj * source, char minRelativeDir, ViewObj * minReferObj)
{
	mSourceObj = source;
	mSourceObj->setRelativeParam(this);
	mMinRelativeDir = minRelativeDir;
	mMinReferObj = minReferObj;
	mMaxRelativeDir = BaseConstants::NONE;
	mMaxReferObj = NULL;
	mMinValue = NULL;
}
RelativeChildrenParam::RelativeChildrenParam(ViewObj * source, char minRelativeDir, ViewObj * minReferObj, char maxRelativeDir, ViewObj * maxReferObj, int * minValue)
{
	mSourceObj = source;
	mSourceObj->setRelativeParam(this);
	mMinRelativeDir = minRelativeDir;
	mMinReferObj = minReferObj;
	mMaxRelativeDir = maxRelativeDir;
	mMaxReferObj = maxReferObj;
	mMinValue = minValue;
}

RelativeChildrenParam::~RelativeChildrenParam()
{
	mSourceObj = NULL;
	mMaxReferObj = NULL;
	mMaxReferObj = NULL;
	if (mMinValue)
	{
		delete[] mMinValue;
		mMinValue = NULL;
	}
}

float RelativeChildrenParam::getAreaValue(float source)
{
	if (mMinValue)
	{
		if (source < mMinValue[0])
		{
			return mMinValue[0];
		}
	}
	return source;
}
void RelativeChildrenParam::replace(bool needAddToReplace)
{
	if (!mMaxReferObj)
	{
		replaceSingle();
	}
	else
		replaceBetween(needAddToReplace);
}
void RelativeChildrenParam::replaceSingle()
{
	BaseViewParam * sourceParam = mSourceObj->getViewParam();
	BaseViewParam * minReferParam = mMinReferObj->getViewParam();
	float dest = 0;
	switch (mMinRelativeDir)
	{
	case BaseConstants::RelativeDirectionType::LEFT:
		dest = minReferParam->getX() - sourceParam->getTotalW();
		dest = getAreaValue(dest);
		sourceParam->setX(dest);
		break;
	case BaseConstants::RelativeDirectionType::RIGHT:
		dest = minReferParam->getX() + minReferParam->getTotalW();
		dest = getAreaValue(dest);
		sourceParam->setX(dest);
		break;
	case BaseConstants::RelativeDirectionType::MIDDLE_X:
		dest = ((LayoutViewParam *)minReferParam)->getInsideLeft() + (((LayoutViewParam *)minReferParam)->getInsideW() - sourceParam->getTotalW()) / 2;
		dest = getAreaValue(dest);
		sourceParam->setX(dest);
		break;
	case BaseConstants::RelativeDirectionType::MIDDLE_Y:
		dest = ((LayoutViewParam *)minReferParam)->getInsideTop() + (((LayoutViewParam *)minReferParam)->getInsideH() - sourceParam->getTotalH()) / 2;
		dest = getAreaValue(dest);
		sourceParam->setY(dest);
		break;
	case BaseConstants::RelativeDirectionType::TOP:
		dest = minReferParam->getY() - sourceParam->getTotalH();
		dest = getAreaValue(dest);
		sourceParam->setY(dest);
		break;
	case BaseConstants::RelativeDirectionType::BOTTOM:
		dest = minReferParam->getY() + minReferParam->getTotalH();
		dest = getAreaValue(dest);
		sourceParam->setY(dest);
		break;
	case BaseConstants::RelativeDirectionType::INSIDE_LEFT:
		dest = ((LayoutViewParam *)minReferParam)->getInsideLeft();
		dest = getAreaValue(dest);
		sourceParam->setX(dest);
		break;
	case BaseConstants::RelativeDirectionType::INSIDE_RIGHT:
		dest = minReferParam->getTotalW() - sourceParam->getTotalW() - ((LayoutViewParam *)minReferParam)->getInsideRight();
		dest = getAreaValue(dest);
		sourceParam->setX(dest);
		break;
	case BaseConstants::RelativeDirectionType::INSIDE_TOP:
		dest = ((LayoutViewParam *)minReferParam)->getInsideTop();
		dest = getAreaValue(dest);
		sourceParam->setY(dest);
		break;
	case BaseConstants::RelativeDirectionType::INSIDE_BOTTOM:
		dest = minReferParam->getTotalH() - sourceParam->getTotalH() - ((LayoutViewParam *)minReferParam)->getInsideBottom();
		dest = getAreaValue(dest);
		sourceParam->setY(dest);
		break;
	}
}
void RelativeChildrenParam::replaceBetween(bool needAddToReplace)
{
	BaseViewParam * sourceParam = mSourceObj->getViewParam();
	BaseViewParam * minReferParam = mMinReferObj->getViewParam();
	BaseViewParam * maxReferParam = mMaxReferObj->getViewParam();
	float min = 0;
	float max = 0;
	char needFill = 0;
	switch (mMinRelativeDir)
	{
	case BaseConstants::RelativeDirectionType::LEFT:
		min = minReferParam->getX();
		if (sourceParam->getDefaultW() == BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE)
		{
			needFill = 1;
		}
		else
			min += sourceParam->getTotalW();
		break;
	case BaseConstants::RelativeDirectionType::RIGHT:
		min = minReferParam->getX() + minReferParam->getTotalW();
		if (sourceParam->getDefaultW() == BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE)
		{
			needFill = 1;
		}
		else
			min += sourceParam->getTotalW();
		break;
	case BaseConstants::RelativeDirectionType::MIDDLE_X:
		min = ((LayoutViewParam *)minReferParam)->getInsideLeft() + ((LayoutViewParam *)minReferParam)->getInsideW() / 2;
		if (sourceParam->getDefaultW() == BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE)
		{
			needFill = 1;
		}
		else
			min += sourceParam->getTotalW();
		break;
	case BaseConstants::RelativeDirectionType::MIDDLE_Y:
		min = ((LayoutViewParam *)minReferParam)->getInsideTop() + ((LayoutViewParam *)minReferParam)->getInsideH() / 2;
		if (sourceParam->getDefaultH() == BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE)
		{
			needFill = 2;
		}
		else
			min += sourceParam->getTotalH();
		break;
	case BaseConstants::RelativeDirectionType::TOP:
		min = minReferParam->getY();
		if (sourceParam->getDefaultH() == BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE)
		{
			needFill = 2;
		}
		else
			min += sourceParam->getTotalH();
		break;
	case BaseConstants::RelativeDirectionType::BOTTOM:
		min = minReferParam->getY() + minReferParam->getTotalH();
		if (sourceParam->getDefaultH() == BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE)
		{
			needFill = 2;
		}
		else
			min += sourceParam->getTotalH();
		break;
	case BaseConstants::RelativeDirectionType::INSIDE_LEFT:
		min = ((LayoutViewParam *)minReferParam)->getInsideLeft();
		if (sourceParam->getDefaultW() == BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE)
		{
			needFill = 1;
		}
		else
			min += sourceParam->getTotalW();
		break;
	case BaseConstants::RelativeDirectionType::INSIDE_RIGHT:
		min = minReferParam->getTotalW() - ((LayoutViewParam *)minReferParam)->getInsideRight();
		if (sourceParam->getDefaultW() == BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE)
		{
			needFill = 1;
		}
		else
			min += sourceParam->getTotalW();
		break;
	case BaseConstants::RelativeDirectionType::INSIDE_TOP:
		min = ((LayoutViewParam *)minReferParam)->getInsideTop();
		if (sourceParam->getDefaultH() == BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE)
		{
			needFill = 2;
		}
		else
			min += sourceParam->getTotalH();
		break;
	case BaseConstants::RelativeDirectionType::INSIDE_BOTTOM:
		min = minReferParam->getTotalH() - ((LayoutViewParam *)minReferParam)->getInsideBottom();
		if (sourceParam->getDefaultH() == BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE)
		{
			needFill = 2;
		}
		else
			min += sourceParam->getTotalH();
		break;
	}
	//min = getAreaValue(min);
	switch (mMaxRelativeDir)
	{
	case BaseConstants::RelativeDirectionType::LEFT:
		max = maxReferParam->getX();
		break;
	case BaseConstants::RelativeDirectionType::RIGHT:
		max = maxReferParam->getX() + maxReferParam->getTotalW();
		break;
	case BaseConstants::RelativeDirectionType::MIDDLE_X:
		max = ((LayoutViewParam *)maxReferParam)->getInsideLeft() + ((LayoutViewParam *)maxReferParam)->getInsideW() / 2;
		break;
	case BaseConstants::RelativeDirectionType::MIDDLE_Y:
		max = ((LayoutViewParam *)maxReferParam)->getInsideTop() + ((LayoutViewParam *)maxReferParam)->getInsideH() / 2;
		break;
	case BaseConstants::RelativeDirectionType::TOP:
		max = maxReferParam->getY();
		break;
	case BaseConstants::RelativeDirectionType::BOTTOM:
		max = maxReferParam->getY() + maxReferParam->getTotalH();
		break;
	case BaseConstants::RelativeDirectionType::INSIDE_LEFT:
		max = ((LayoutViewParam *)maxReferParam)->getInsideLeft();
		break;
	case BaseConstants::RelativeDirectionType::INSIDE_RIGHT:
		max = maxReferParam->getTotalW() - ((LayoutViewParam *)maxReferParam)->getInsideRight();
		break;
	case BaseConstants::RelativeDirectionType::INSIDE_TOP:
		max = ((LayoutViewParam *)maxReferParam)->getInsideTop();
		break;
	case BaseConstants::RelativeDirectionType::INSIDE_BOTTOM:
		max = maxReferParam->getTotalH() - ((LayoutViewParam *)maxReferParam)->getInsideBottom();
		break;
	}
	float dest = min;
	switch (needFill)
	{
	case 0:
		dest = (max - min) / 2;
		dest = getAreaValue(dest);
		break;
	case 1:
		dest = getAreaValue(dest);
		sourceParam->setSelfW(max - dest, BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_CHILDREN, needAddToReplace);
		break;
	case 2:
		dest = getAreaValue(dest);
		sourceParam->setSelfH(max - dest, BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_CHILDREN, needAddToReplace);
		break;
	}

	switch (mMaxRelativeDir)
	{
	case BaseConstants::RelativeDirectionType::LEFT:
	case BaseConstants::RelativeDirectionType::RIGHT:
	case BaseConstants::RelativeDirectionType::MIDDLE_X:
	case BaseConstants::RelativeDirectionType::INSIDE_LEFT:
	case BaseConstants::RelativeDirectionType::INSIDE_RIGHT:
		sourceParam->setX(dest);
		break;
	case BaseConstants::RelativeDirectionType::MIDDLE_Y:
	case BaseConstants::RelativeDirectionType::TOP:
	case BaseConstants::RelativeDirectionType::BOTTOM:
	case BaseConstants::RelativeDirectionType::INSIDE_TOP:
	case BaseConstants::RelativeDirectionType::INSIDE_BOTTOM:
		sourceParam->setY(dest);
		break;
	}
}
float RelativeChildrenParam::getX()
{
	BaseViewParam * sourceParam = mSourceObj->getViewParam();
	float dest = 0;
	return dest;
}
float RelativeChildrenParam::getY()
{
	float dest = 0;
	return dest;
}