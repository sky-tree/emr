#ifndef _IMAGE_SCALE_ACTION_H_
#define _IMAGE_SCALE_ACTION_H_
#pragma once
#include "BaseViewParam.h"
#include "BaseAction.h"

class ImageScaleAction :
	public BaseAction
{
public:
	ImageScaleAction(StepActionManger * mgr);
	virtual ~ImageScaleAction();

public:

	void doAction(BaseViewParam * viewParam, int w, int h);

	void unDo();
	void reDo();
private:
	void getPreValue();
	void setParamByType(int w, int h);
	void replaceRow();
protected:
	// Ŀ��ֵ���
	int mDestW;
	int mDestH;
	// ��ʼֵ���
	int mPreW;
	int mPreH;

	BaseViewParam * mDestViewParam;
};

#endif