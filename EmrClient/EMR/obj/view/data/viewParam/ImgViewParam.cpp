#include "ImgViewParam.h"

#include "ViewObj.h"

#include "RowLayout.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

#include "BaseMath.h"

ImgViewParam::ImgViewParam(ViewObj * viewOwner) : BaseViewParam(viewOwner, ConstantsID::ClassId::IMG_VIEW_PARAM)
, mSourceW(0)
, mSourceH(0)
{
}
ImgViewParam::ImgViewParam(ViewObj * viewOwner, ImgViewParam * source) : BaseViewParam(viewOwner, source)
, mSourceW(source->mSourceW)
, mSourceH(source->mSourceH)
{
}

ImgViewParam::~ImgViewParam()
{
}
void ImgViewParam::setSourceW(int w)
{
	mSourceW = w;
}
void ImgViewParam::setSourceH(int h)
{
	mSourceH = h;
}
float ImgViewParam::getSourceW()
{
	return mSourceW;
}
float ImgViewParam::getSourceH()
{
	return mSourceH;
}