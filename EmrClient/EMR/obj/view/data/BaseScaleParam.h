#ifndef _BASE_SCALE_PARAM_H_
#define _BASE_SCALE_PARAM_H_


#pragma once
#include "BaseContainer.h"
#include "ImgConTentData.h"
#include "BaseElement.h"

class BaseScaleParam :
	public BaseElement
{
public:
	BaseScaleParam(ViewObj * owner, int classID);
	BaseScaleParam(ViewObj * owner, BaseScaleParam * source);
	~BaseScaleParam();
private:
	int mPressDirection;

	int mStartX;
	int mStartY;

	int mLastW;
	int mLastH;

	//控制鼠标能拖动的区域
	int mTopY;
	int mBottomY;
	int mLeftX;
	int mRightX;

	// 缩放起始对象x、y
	int mStartObjX;
	int mStartObjY;

	// 起始宽高
	int mStartObjW;
	int mStartObjH;

	// 拖动中宽高
	int  mDestW;
	int  mDestH;

	// 拖动中x、y
	int mDestX;
	int mDestY;

	ImgConTentData* mImgContentData;

	ViewObj * mViewOwner;
private:
	void init();
public:
	void judgeStart(int x, int y);
	void doScale(int x, int y);
	void judgeStop(int x, int y);
	int getStartPointType();

	int getDestW();
	int getDestH();
	void getParam(int& x, int& y, int& w, int& h);
	void replaceRow();
	void Updata();
};


#endif