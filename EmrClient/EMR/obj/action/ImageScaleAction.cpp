
#include "ConstantsID.h"
#include "BaseAction.h"
#include "BaseScaleParam.h"
#include "ImageScaleAction.h"



ImageScaleAction::ImageScaleAction(StepActionManger * mgr):BaseAction(mgr, ConstantsID::ClassId::ZOOM_IMAGE_ACTION)
{
	mDestViewParam = NULL;
}


ImageScaleAction::~ImageScaleAction()
{
	mDestViewParam = NULL;
}


void ImageScaleAction::doAction(BaseViewParam * viewParam, int w, int h)
{
	mDestViewParam = viewParam;
	mPreW = w;
	mPreH = h;
	getPreValue();
}

void ImageScaleAction::getPreValue() 
{
	mDestW = mDestViewParam->getSelfW();
	mDestH = mDestViewParam->getSelfH();
}

void ImageScaleAction::unDo()
{
	setParamByType(mPreW, mPreH);
}

void ImageScaleAction::reDo()
{
	setParamByType(mDestW, mDestH);
}

void ImageScaleAction::setParamByType(int w, int h)
{
	if (mDestViewParam)
	{
		mDestViewParam->setSelfW(w);
		mDestViewParam->setSelfH(h);
		replaceRow();
	}
}

void ImageScaleAction::replaceRow()
{
	BaseScaleParam * baseScaleParam = mDestViewParam->getBaseScaleParam();
	baseScaleParam->replaceRow();
	baseScaleParam->Updata();
}