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

	//����������϶�������
	int mTopY;
	int mBottomY;
	int mLeftX;
	int mRightX;

	// ������ʼ����x��y
	int mStartObjX;
	int mStartObjY;

	// ��ʼ���
	int mStartObjW;
	int mStartObjH;

	// �϶��п��
	int  mDestW;
	int  mDestH;

	// �϶���x��y
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