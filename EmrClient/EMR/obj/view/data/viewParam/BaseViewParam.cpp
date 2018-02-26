#include "BaseViewParam.h"




#include "WriterManager.h"

#include "ViewObj.h"
#include "RowViewParam.h"


#include "RelativeChildrenParam.h"

#include "BaseLayout.h"

#include "BaseScaleParam.h"
#include "LayoutViewParam.h"
#include "LayoutReplaceManager.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

BaseViewParam::BaseViewParam(ViewObj * owner) : BaseContainerParam(owner, ConstantsID::ClassId::BASE_VIEW_PARAM)
{
	initParams(owner, NULL);
}
BaseViewParam::BaseViewParam(ViewObj * owner, int classID) : BaseContainerParam(owner, classID)
{
	initParams(owner, NULL);
}
BaseViewParam::BaseViewParam(ViewObj * owner, BaseViewParam * source) : BaseContainerParam(owner, source->mClassID)
{
	initParams(owner, source);
}

BaseViewParam::~BaseViewParam()
{
}
void BaseViewParam::initParams(ViewObj * view, BaseViewParam * source)
{
	mViewOwner = view;
	mScaleParam = NULL;
	mClip = NULL;
	mDefaultW = BaseConstants::NONE;
	mDefaultH = BaseConstants::NONE;
	if (source)
	{
		mDefaultW = source->mDefaultW;
		mDefaultH = source->mDefaultH;
		mSelfW = source->mSelfW;
		mSelfH = source->mSelfH;
		if (source->mScaleParam)
		{
			mScaleParam = new BaseScaleParam(mViewOwner, source->mScaleParam);
		}
		if (source->mClip)
		{
			mClip = new int[4];
			for (int i = 0; i < 4; i++)
			{
				mClip[i] = source->mClip[i];
			}
		}
	}
	else
	{
		mSelfW = -1;
		mSelfH = -1;
	}
	mViewOwner->initViewParam(this);
	setViewType(BaseConstants::ViewType::NORMAL);
}
void BaseViewParam::init(float x, float y, int w, int h)
{
	mX = x;
	mY = y;
	mSelfW = w;
	mSelfH = h;
}
void BaseViewParam::initDefaultWH(short w, short h)
{
	mDefaultW = w;
	mDefaultH = h;
}
void BaseViewParam::initClip(int * clip)
{
	mClip = clip;
}
bool BaseViewParam::isView()
{
	return mViewType >= BaseConstants::ViewType::NORMAL;
}
char BaseViewParam::getViewType()
{
	return mViewType;
}
void BaseViewParam::setViewType(char type)
{
	mViewType = type;
}
int * BaseViewParam::getClip()
{
	return mClip;
}
void BaseViewParam::setX(float value)
{
	mX = value;
	//if (mViewOwner->getViewParent()->isRow() && mViewOwner->getChildren()->size() == 10)
	//{
	//	int a = 0;
	//}
	if (mViewOwner->isTxt() &&mViewOwner->getViewIndex() != 0 && value == 0)
	{
		int a = 0;
	}
}
void BaseViewParam::setY(float value)
{
	mY = value;
	if (mViewOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
	{
		int a = 0;
	}
}
void BaseViewParam::addY(float value)
{
	mY += value;
	if (mViewOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
	{
		int a = 0;
	}
}
bool BaseViewParam::setSelfW(int value, bool needAddToReplace)
{
	if (mSelfW != value)
	{
		BaseLayout * layout = mViewOwner->getLayout();
		if (layout)
		{
			if (!needAddToReplace)
			{
				if (layout->isRelativeLayout())
					mViewOwner->getWriterMgr()->getLayoutReplaceMgr()->pushLayout(layout);
			}
			else
				mViewOwner->getWriterMgr()->getLayoutReplaceMgr()->pushLayout(layout);
		}
		mSelfW = value;
		return true;
	}
	return false;
}
bool BaseViewParam::setSelfH(int value, bool needAddToReplace)
{
	if (mViewOwner->isObj(ConstantsID::ContainerId::ROOT_VIEW))
	{
		int a = 0;
	}
	if (mSelfH != value)
	{
		BaseLayout * layout = mViewOwner->getLayout();
		if (layout)
		{
			WriterManager * writerMgr = mViewOwner->getWriterMgr();
			LayoutReplaceManager * layoutReplaceMgr = writerMgr->getLayoutReplaceMgr();
			if (!needAddToReplace)
			{
				if (layout && layout->isRelativeLayout())
				{
					layoutReplaceMgr->pushLayout(layout);
				}
				else if (mViewOwner->getRelativeParam() != NULL)
				{
					layoutReplaceMgr->pushLayout(mViewOwner->getViewParent()->getLayout());
				}
			}
			else
			{
				layoutReplaceMgr->pushLayout(layout);
			}
		}
		mSelfH = value;
		return true;
	}
	return false;
}
bool BaseViewParam::setSelfW(int value, char childrenOrParentType, bool needAddToReplace)
{
	bool haveChanged = setSelfW(value, needAddToReplace);
	if (childrenOrParentType == BaseConstants::NONE || mViewOwner->haveChildren() < 1)
	{
		return haveChanged;
	}
	QList<BaseContainer *> * children = NULL;
	ViewObj * obj = NULL;
	switch (childrenOrParentType)
	{
	case BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_CHILDREN:
	case BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_PARENT_AND_CHILDREN:
		children = mViewOwner->getChildren();
		for (int i = 0; i < children->size(); i++)
		{
			obj = (ViewObj *)children->at(i);
			obj->getViewParam()->refreshWByParent(childrenOrParentType, needAddToReplace);
		}
		if (childrenOrParentType == BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_CHILDREN)
			break;
	case BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_PARENT:
		obj = mViewOwner->getViewParent();
		obj->getViewParam()->refreshWByChildren(true);
		break;
	}
	return haveChanged;
}
bool BaseViewParam::setSelfH(int value, char childrenOrParentType, bool needAddToReplace)
{
	if (mViewOwner->isObj(ConstantsID::ContainerId::ROOT_VIEW))
	{
		int a = 0;
	}
	bool haveChanged = setSelfH(value, needAddToReplace);
	if (childrenOrParentType == BaseConstants::NONE || mViewOwner->haveChildren() < 1)
	{
		return haveChanged;
	}
	QList<BaseContainer *> * children = NULL;
	ViewObj * obj = NULL;
	BaseViewParam * param = NULL;
	switch (childrenOrParentType)
	{

	case BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_CHILDREN:
	case BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_PARENT_AND_CHILDREN:
		children = mViewOwner->getChildren();
		for (int i = 0; i < children->size(); i++)
		{
			obj = (ViewObj *)children->at(i);
			param = obj->getViewParam();
			param->refreshHByParent(childrenOrParentType, needAddToReplace);
		}
		if (childrenOrParentType == BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_CHILDREN)
			break;
	case BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_PARENT:
		obj = mViewOwner->getViewParent();
		param = obj->getViewParam();
		param->refreshHByChildren(true);
		break;
	}
	return haveChanged;
}

float BaseViewParam::getX()
{
	return mX;
}
float BaseViewParam::getY()
{
	return mY;
}
float BaseViewParam::getAbsX(ViewObj * relativeObj)
{
	float dest = mX;
	ViewObj * temp = mViewOwner;
	while (temp != relativeObj)
	{
		temp = temp->getViewParent();
		if (!temp)
		{
			break;
		}
		dest += temp->getViewParam()->getX();
	}
	return dest;
}
float BaseViewParam::getAbsY(ViewObj * relativeObj)
{
	float dest = mY;
	ViewObj * temp = mViewOwner;
	while (temp != relativeObj)
	{
		temp = temp->getViewParent();
		if (!temp)
		{
			break;
		}
		dest += temp->getViewParam()->getY();
	}
	return dest;
}
int BaseViewParam::getSelfW()
{
	return mSelfW;
}
int BaseViewParam::getSelfH()
{
	return mSelfH;
}
float BaseViewParam::getTotalW()
{
	return mSelfW;
}
float BaseViewParam::getTotalH()
{
	return mSelfH;
}
short BaseViewParam::getDefaultW()
{
	return mDefaultW;
}
short BaseViewParam::getDefaultH()
{
	return mDefaultH;
}
void BaseViewParam::refreshWByChildren(bool circle)
{
	if (mDefaultW == BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE)
	{
		if (mViewOwner->haveChildren() < 1)
		{
			return;
		}
		QList<BaseContainer *> * children = mViewOwner->getChildren();
		int size = children->size();
		ViewObj * obj = NULL;
		BaseViewParam * param = NULL;
		float maxW = -1;
		float w = 0;
		for (int i = 0; i < size; i++)
		{
			obj = (ViewObj *)children->at(i);
			param = obj->getViewParam();
			w = param->getTotalW();
			if (w > maxW)
			{
				maxW = w;
			}
		}
		if (mSelfW != maxW)
		{
			setSelfW(maxW);
			if (circle)
			{
				mViewOwner->getViewParent()->getViewParam()->refreshWByChildren(circle);
			}
		}
	}
}
void BaseViewParam::refreshHByChildren(bool circle)
{
}
void BaseViewParam::refreshWByParent(char childrenOrParentType, bool needAddToReplace)
{
	ViewObj * parent = mViewOwner->getViewParent();
	float dest = 0;
	if (mDefaultW == BaseConstants::DefaultWHType::FILL_PARENT_INSIDE)
	{
		LayoutViewParam * param = (LayoutViewParam *)parent->getViewParam();
		if (parent->isRow())
		{
			dest = ((RowViewParam *)param)->getEffectW(mViewOwner);
		}
		else
			dest = param->getInsideW();
	}
	else if (mDefaultW == BaseConstants::DefaultWHType::FILL_PARENT_TOTAL)
	{
		BaseViewParam * param = parent->getViewParam();
		dest = param->getTotalW();
	}
	else if (mDefaultH == BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE)
	{
		mOwner->getWriterMgr()->getLayoutReplaceMgr()->pushLayout(mViewOwner->getLayout());
	}
	else
	{
		return;
	}
	//if (needAddToReplace)
	//{
	//	mOwner->getWriterMgr()->getLayoutReplaceMgr()->pushLayout(mViewOwner->getLayout());
	//}
	setSelfW(dest, childrenOrParentType, needAddToReplace);
}
void BaseViewParam::refreshHByParent(char childrenOrParentType, bool needAddToReplace)
{
	ViewObj * parent = mViewOwner->getViewParent();
	LayoutViewParam * param = (LayoutViewParam *)parent->getViewParam();
	float dest = 0;
	if (mDefaultH == BaseConstants::DefaultWHType::FILL_PARENT_INSIDE)
	{
		LayoutViewParam * param = (LayoutViewParam *)parent->getViewParam();
		dest = param->getInsideH();
	}
	else if (mDefaultH == BaseConstants::DefaultWHType::FILL_PARENT_TOTAL)
	{
		BaseViewParam * param = parent->getViewParam();
		dest = param->getTotalH();
	}
	else if (mDefaultH == BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE)
	{
		ViewObj * parent = mViewOwner->getViewParent();
		RelativeChildrenParam * relativeParam = mViewOwner->getRelativeParam();
		//if (relativeParam)
		relativeParam->replace(needAddToReplace);
		//mOwner->getWriterMgr()->getLayoutReplaceMgr()->pushLayout(mViewOwner->getLayout());
		return;
	}
	else
		return;
	//if (needAddToReplace)
	//{
	//	mOwner->getWriterMgr()->getLayoutReplaceMgr()->pushLayout(mViewOwner->getLayout());
	//}
	setSelfH(dest, childrenOrParentType, needAddToReplace);
}

void BaseViewParam::initScaleParam(BaseScaleParam * scaleParam)
{
	mScaleParam = scaleParam;
}

BaseScaleParam * BaseViewParam::getBaseScaleParam()
{
	return mScaleParam;
}