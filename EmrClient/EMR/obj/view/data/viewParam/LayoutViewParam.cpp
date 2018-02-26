#include "LayoutViewParam.h"




#include <QDebug>

#include "ViewObj.h"
#include "WriterManager.h"
#include "LayoutReplaceManager.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

LayoutViewParam::LayoutViewParam(ViewObj * owner) : BaseViewParam(owner, ConstantsID::ClassId::LAYOUT_VIEW_PARAM)
{
	//mChildrenW = -1;
	mPreInsideW = -1;
	mPreInsideH = -1;
	mMinH = -1;
	initInside(0, 0, 0, 0);
}

LayoutViewParam::LayoutViewParam(ViewObj * owner, int classID) : BaseViewParam(owner, classID)
{
	//mChildrenW = -1;
	mPreInsideW = -1;
	mPreInsideH = -1;
	mMinH = -1;
	initInside(0, 0, 0, 0);
}

LayoutViewParam::~LayoutViewParam()
{
}
void LayoutViewParam::initInside(int inL, int inR, int inT, int inB)
{
	mInsideLeft = inL;
	mInsideRight = inR;
	mInsideTop = inT;
	mInsideBottom = inB;

	mPreInsideW = 0;
	mPreInsideH = 0;
}
void LayoutViewParam::initMinH(int h)
{
	mMinH = h;
}
int LayoutViewParam::getInsideLeft()
{
	return mInsideLeft;
}
void LayoutViewParam::setInsideLeft(int insideLeft)
{
	mInsideLeft = insideLeft;
}
int LayoutViewParam::getInsideRight()
{
	return mInsideRight;
}
void LayoutViewParam::setInsideRight(int insideright)
{
	mInsideRight = insideright;
}
void LayoutViewParam::setInsideTop(int value)
{
	mInsideTop = value;
}
int LayoutViewParam::getInsideTop()
{
	return mInsideTop;
}
void LayoutViewParam::setInsideBottom(int value)
{
	mInsideBottom = value;
}
int LayoutViewParam::getInsideBottom()
{
	return mInsideBottom;
}
int LayoutViewParam::getInsideW()
{
	return mSelfW - mInsideLeft - mInsideRight;
}
int LayoutViewParam::getInsideH()
{
	return mSelfH - mInsideTop - mInsideBottom;
}
void LayoutViewParam::refreshPreInsideW()
{
	mPreInsideW = getInsideW();
}
void LayoutViewParam::refreshPreInsideH()
{
	mPreInsideH = getInsideH();
}
int LayoutViewParam::getPreInsideW()
{
	return mPreInsideW;
}
int LayoutViewParam::getPreInsideH()
{
	return mPreInsideH;
}
void LayoutViewParam::setPreInsideH(int value)
{
	mPreInsideH = value;
}

void LayoutViewParam::refreshInsideW(char childrenOrParentType)
{
	mViewOwner->getWriterMgr()->getLayoutReplaceMgr()->pushLayout(mViewOwner->getLayout());
	QList<BaseContainer *> * children = NULL;
	ViewObj * obj = NULL;
	BaseViewParam * param = NULL;
	children = mViewOwner->getChildren();
	for (int i = 0; i < children->size(); i++)
	{
		obj = (ViewObj *)children->at(i);
		param = obj->getViewParam();
		param->refreshWByParent(childrenOrParentType, true);
	}
}
void LayoutViewParam::refreshInsideH(char childrenOrParentType)
{
	mViewOwner->getWriterMgr()->getLayoutReplaceMgr()->pushLayout(mViewOwner->getLayout());
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
			param->refreshHByParent(childrenOrParentType, true);
		}
		break;
	}
}
void LayoutViewParam::setInsideW(int value, char childrenOrParentType)
{
	setSelfW(value + mInsideLeft + mInsideRight, childrenOrParentType);
}
void LayoutViewParam::setInsideH(int value, char childrenOrParentType)
{
	setSelfH(value + mInsideTop + mInsideBottom, childrenOrParentType);
}
bool LayoutViewParam::judgeCanResetHByChildren(int maxChildrenH)
{
	if (mSelfW != maxChildrenH && mMinH >= 0 && maxChildrenH >= mMinH)
		return true;
	return false;
}
void LayoutViewParam::refreshWByChildren(bool circle)
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
			setInsideW(maxW, 0);
			if (circle)
			{
				ViewObj * viewParent = mViewOwner->getViewParent();
				if (viewParent)
					viewParent->getViewParam()->refreshWByChildren(circle);
			}
		}
	}
	else if (mDefaultW == BaseConstants::DefaultWHType::FILL_PARENT_INSIDE)
	{
		LayoutViewParam * parentViewParam = (LayoutViewParam *)mViewOwner->getViewParent()->getViewParam();
		setSelfW(parentViewParam->getInsideW());
	}
	else if (mDefaultW == BaseConstants::DefaultWHType::FILL_PARENT_TOTAL)
	{
		LayoutViewParam * parentViewParam = (LayoutViewParam *)mViewOwner->getViewParent()->getViewParam();
		setSelfW(parentViewParam->getTotalW());
	}
}
void LayoutViewParam::refreshHByChildren(bool circle)
{
	if (mDefaultH == BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE)
	{
		if (mViewOwner->haveChildren() < 1)
		{
			return;
		}
		QList<BaseContainer *> * children = mViewOwner->getChildren();
		int size = children->size();
		ViewObj * obj = NULL;
		BaseViewParam * param = NULL;
		float maxInsideH = -1;
		float h = 0;
		for (int i = 0; i < size; i++)
		{
			obj = (ViewObj *)children->at(i);
			param = obj->getViewParam();
			h = param->getTotalH();
			if (h > maxInsideH)
			{
				maxInsideH = h;
			}
		}
		if (maxInsideH < mMinH && mMinH >= 0)
		{
			maxInsideH = mMinH;
		}
		if (mSelfH != maxInsideH)
		{
			setInsideH(maxInsideH, 0);
			if (circle)
			{
				ViewObj * viewParent = mViewOwner->getViewParent();
				if (viewParent)
					viewParent->getViewParam()->refreshHByChildren(circle);
			}
		}
	}
	else if (mDefaultH == BaseConstants::DefaultWHType::FILL_PARENT_INSIDE)
	{
		LayoutViewParam * parentViewParam = (LayoutViewParam *)mViewOwner->getViewParent()->getViewParam();
		setSelfH(parentViewParam->getInsideH());
	}
	else if (mDefaultH == BaseConstants::DefaultWHType::FILL_PARENT_TOTAL)
	{
		LayoutViewParam * parentViewParam = (LayoutViewParam *)mViewOwner->getViewParent()->getViewParam();
		setSelfH(parentViewParam->getTotalH());
	}
}
float LayoutViewParam::getChildrenW()
{
	return 0;
}
float LayoutViewParam::getChildrenRowH()
{
	return 0;
}
//void LayoutViewParam::openChildrenW()
//{
//	mChildrenW = -1;
//}
//void LayoutViewParam::refreshChildrenW()
//{
//
//}