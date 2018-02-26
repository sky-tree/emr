#include "BaseEditParam.h"



BaseEditParam::BaseEditParam(ViewObj * owner, int classID) 
	:BaseElement(ConstantsID::ClassId::IMG_EDIT_PARAM)
{
	mScaleW = -1;
	mScaleH = -1;
	mViewOwner = owner;
}

BaseEditParam::BaseEditParam(ViewObj * owner, int classID, BaseEditParam * source)
	: BaseElement(ConstantsID::ClassId::IMG_EDIT_PARAM)
{
	mItemsData = source->mItemsData;
}


BaseEditParam::~BaseEditParam()
{
}


void BaseEditParam::setItem(QList<StrItemData> items)
{
	mItemsData = items;
}

QList<StrItemData>& BaseEditParam::getItem()
{
	return mItemsData;
}

void BaseEditParam::setScaleW(int w)
{
	mScaleW = w;
}

void BaseEditParam::setScaleH(int h)
{
	mScaleH = h;
}

int BaseEditParam::getScaleW()
{
	return mScaleW;
}

int BaseEditParam::getScaleH()
{
	return mScaleH;
}