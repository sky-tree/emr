#include "BaseScaleParam.h"

#include "qdebug.h"

#include "OperateMgr.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "ViewObj.h"
#include "RowLayout.h"
#include "BaseViewParam.h"
#include "ObjMouseManager.h"
#include "RowViewParam.h"
#include "UIConstants.h"
#include "BaseTools.h"
#include "BaseEditParam.h"

BaseScaleParam::BaseScaleParam(ViewObj * owner, int classID):BaseElement(ConstantsID::ClassId::IMG_VIEW_PARAM)
,mImgContentData(NULL)
{
	mViewOwner = owner;

	mPressDirection = BaseConstants::Direction::MIDDLE;
	init();
	Updata();
}
BaseScaleParam::BaseScaleParam(ViewObj * owner, BaseScaleParam * source) :BaseElement(ConstantsID::ClassId::IMG_VIEW_PARAM)
,mImgContentData(NULL)
{
	mViewOwner = owner;

	mPressDirection = BaseConstants::Direction::MIDDLE;

	mTopY = source->mTopY;
	mBottomY = source->mBottomY;
	mLeftX = source->mLeftX;
	mRightX = source->mRightX;

	mLastW = source->mLastW;
	mLastH = source->mLastH;

	mImgContentData = source->mImgContentData;
}


BaseScaleParam::~BaseScaleParam()
{
}

void BaseScaleParam::init()
{
	mImgContentData = (ImgConTentData*)mViewOwner->getContentData();

	// 缩放起始对象x、y
	mStartObjX = mViewOwner->getViewParam()->getX();
	mStartObjY = mViewOwner->getViewParam()->getY();

	// 缩放起始对象宽高
	mStartObjW = mViewOwner->getViewParam()->getSelfW();
	mStartObjH = mViewOwner->getViewParam()->getSelfH();

	// 拖动中对象的宽高
	mDestW = -1;
	mDestH = -1;

	//拖动中对象的x、y
	mDestX = -1;
	mDestY = -1;

}


void BaseScaleParam::judgeStart(int x, int y)
{
	if (x < mRightX && y < mTopY)
	{
		mPressDirection = BaseConstants::Direction::LEFT_TOP;
	}
	else if (x < mRightX && y > mBottomY)
	{
		mPressDirection = BaseConstants::Direction::LEFT_BOTTOM;
	}
	else if (x > mRightX && y < mTopY)
	{
		mPressDirection = BaseConstants::Direction::RIGHT_TOP;
	}
	else if (x > mRightX && y > mBottomY)
	{
		mPressDirection = BaseConstants::Direction::RIGHT_BOTTOM;
	}
	else
	{
		mPressDirection = BaseConstants::Direction::MIDDLE;
		return;
	}
	init();
	BaseViewParam * viewParam = mViewOwner->getViewParam();
	mLastW = viewParam->getTotalW();
	mLastH = viewParam->getTotalH();
	ViewObj * viewObj = mViewOwner->findViewFromParent(ConstantsID::ContainerId::TXT_WIN_VIEW, true);
	ObjMouseManager * mouseMgr = viewObj->getMouseMgr();
	mouseMgr->setFocus(mViewOwner);
	mStartX = x;
	mStartY = y;
}

void BaseScaleParam::Updata()
{
	BaseViewParam * viewParam = mViewOwner->getViewParam();
	mTopY = viewParam->getTotalH() * 0.2;
	mBottomY = viewParam->getTotalH() * 0.8;
	mLeftX = viewParam->getTotalW() * 0.2;
	mRightX = viewParam->getTotalW() * 0.8;

}

void BaseScaleParam::doScale(int x, int y)
{
	int dx = 0, dy = 0;
	switch (mPressDirection)
	{
	case BaseConstants::Direction::RIGHT_TOP:
		dx = x - mStartX;
		dy = mStartY - y;
		mDestW = mStartObjW + dx;
		mDestH = mStartObjH + dy;
		mDestX = mStartObjX;
		mDestY = mStartObjY - dy;
		break;
	case BaseConstants::Direction::RIGHT_BOTTOM:
		dx = x - mStartX;
		dy = y - mStartY;
		mDestW = mStartObjW + dx;
		mDestH = mStartObjH + dy;
		mDestX = mStartObjX;
		mDestY = mStartObjY;
		break;
	case BaseConstants::Direction::LEFT_TOP:
		dx = mStartX - x;
		dy = mStartY - y;
		mDestW = mStartObjW + dx;
		mDestH = mStartObjH + dy;
		mDestX = mStartObjX - dx;
		mDestY = mStartObjY - dy;
		break;
	case BaseConstants::Direction::LEFT_BOTTOM:
		dx = mStartX - x;
		dy = y - mStartY; 
		mDestW = mStartObjW + dx;
		mDestH = mStartObjH + dy;
		mDestX = mStartObjX - dx;
		mDestY = mStartObjY;
		break;
	default:
		break;
	}
	if (mDestW > UIConstants::MAX_ROW_INSIDE_W)
	{
		mDestW = UIConstants::MAX_ROW_INSIDE_W;
	}
	else if (mDestW < UIConstants::MIN_ROW_INSIDE_W)
	{
		mDestW = UIConstants::MIN_ROW_INSIDE_W;
	}
	if (mDestH > UIConstants::MAX_ROW_INSIDE_H)
	{
		mDestH = UIConstants::MAX_ROW_INSIDE_H;
	}
	else if (mDestH < UIConstants::MIN_ROW_INSIDE_H)
	{
		mDestH = UIConstants::MIN_ROW_INSIDE_H;
	}
}

void BaseScaleParam::judgeStop(int x, int y)
{
	if (mPressDirection == BaseConstants::Direction::MIDDLE)
		return;
	if (mDestH == -1 && mDestW == -1)
		return;
	ViewObj * row = mViewOwner->getViewParent();
	BaseViewParam * viewParam = mViewOwner->getViewParam();
	viewParam->setSelfH(mDestH);
	viewParam->setSelfW(mDestW);
	BaseEditParam* baseEditParam = mImgContentData->getBaseEditParam();
	baseEditParam->setScaleH(mDestH);
	baseEditParam->setScaleW(mDestW);
	Updata();
	ViewObj * viewObj = mViewOwner->findViewFromParent(ConstantsID::ContainerId::TXT_WIN_VIEW, true);
	ObjMouseManager * mouseMgr = viewObj->getMouseMgr();
	mouseMgr->setFocus(NULL);


	//判断是否需要撤销
	ViewObj * layer = (ViewObj*)viewObj->findChildrenByID(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER, true);

	OperateMgr* operateMgr = viewObj->getOperateMgr();
	replaceRow();
	ViewObj * txtLayer = row->getViewParent();
	BaseViewParam * layerParam = txtLayer->getViewParam();
	layerParam->refreshHByChildren(true);

	BaseTools::judgeTitleOrBottomOutArea(layer);
	operateMgr->addImgScaleAction(viewParam, mStartObjW, mStartObjH);
	if (operateMgr->getNeedUndo())
	{
		operateMgr->doUndo();
	}
}

int BaseScaleParam::getStartPointType()
{
	return mPressDirection;
}

int BaseScaleParam::getDestW()
{
	return mLastW;
}
int BaseScaleParam::getDestH()
{
	return mLastH;
}

void BaseScaleParam::getParam(int& x, int& y, int& w, int& h)
{
	if (mDestX == -1 && mDestY == -1 && mDestW == -1 && mDestH == -1)
	{
		x = mStartObjX;
		y = mStartObjY;
		w = mStartObjW;
		h = mStartObjH;
	}
	else
	{
		x = mDestX;
		y = mDestY;
		w = mDestW;
		h = mDestH;
	}
}

void BaseScaleParam::replaceRow()
{
	ViewObj * row = mViewOwner->getViewParent();
	RowLayout * rowLayout = (RowLayout *)row->getLayout();
	if(mDestW > mLastW && mDestW != -1)//放大
	{
		rowLayout->openReplace();
		rowLayout->judgeOverFlowCircle();
	}
	else if(mDestW < mLastW && mDestW != -1)//缩小
	{
		rowLayout->openReplace();
		rowLayout->judgeLessThanCircle(true);
	}
	// 宽度不变的情况下
	else
	{
		// 如果高度变化，则根据内容，遍历刷新父对象高度
		if (mDestH != mLastH && mDestH != -1)
		{
			mViewOwner->getViewParent()->getViewParam()->refreshHByChildren(true);
		}
	}
}