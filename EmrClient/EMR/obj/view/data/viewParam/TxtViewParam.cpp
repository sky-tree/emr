#include "TxtViewParam.h"

#include "ViewObj.h"
#include "RowViewParam.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

TxtViewParam::TxtViewParam(ViewObj * viewOwner) : BaseViewParam(viewOwner, ConstantsID::ClassId::TXT_VIEW_PARAM)
, mAscent(0)
{
	mX = 0;
	mY = 0;
}
TxtViewParam::TxtViewParam(ViewObj * viewOwner, TxtViewParam * source) : BaseViewParam(viewOwner, ConstantsID::ClassId::TXT_VIEW_PARAM)
, mAscent(source->mAscent)
{
	mX = 0;
	mY = 0;
}

TxtViewParam::~TxtViewParam()
{
}


//void TxtViewParam::refresh()
//{
//	refreshRowParam();
//}
//void TxtViewParam::refreshRowParam()
//{
//	mRowParam = (NormalRowParam *)mViewOwner->getViewParent()->getViewParam();
//}

int TxtViewParam::getAscent()
{
	return mAscent;
}
void TxtViewParam::setAscent(int value)
{
	mAscent = value;
}
int TxtViewParam::getDescent()
{
	return mDescent;
}
void TxtViewParam::setDescent(int value)
{
	mDescent = value;
}