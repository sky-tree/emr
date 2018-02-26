#include "DrawButton.h"
#include "ConstantsID.h"

#include "ViewObj.h"
#include "BaseViewParam.h"
#include "ImgContentData.h"
#include "ConstantsNumber.h"
#include "BaseConstants.h"


DrawButton::DrawButton(ViewObj * viewOwner) : BaseDraw(viewOwner, ConstantsID::ClassId::DRAW_IMAGE)
{
	mIndex = BaseConstants::MousePressType::RELEASE - 1;

}


DrawButton::~DrawButton()
{
	release();
}

void DrawButton::init(QString resRelease, QString resPress)
{
	QImage * releaseImage = new QImage(resRelease);
	QImage * PressImage = new QImage(resPress);
	mImageResVector.push_back(releaseImage);
	mImageResVector.push_back(PressImage);
	mImageResVector.replace(0, PressImage);
	mImageResVector.replace(1, releaseImage);
}

void DrawButton::release()
{
	if (mImageResVector.size() != 0)
	{
		for (auto it = mImageResVector.begin(); it != mImageResVector.end(); it++)
		{
			delete *it;
		}
		mImageResVector.clear();
	}
}

void DrawButton::setIndex(int index)
{
	mIndex = index - 1;
}

bool DrawButton::draw(QPainter &pPainter, int x, int y, int w, int h)
{
	BaseViewParam * viewParam = mViewOwner->getViewParam();

	if (w == ConstantsNumber::MIN_CHAR)
	{
		w = viewParam->getTotalW();
	}
	if (h == ConstantsNumber::MIN_CHAR)
	{
		h = viewParam->getTotalH();
	}

	//QRect *rect = new QRect(x, y, w, h);
	QRect rect(x, y, w, h);
	//// Õý³£Í¼Æ¬
	if (mIndex < mImageResVector.size())
	{
		pPainter.drawImage(rect, *mImageResVector[mIndex]);
	}
	else
	{
		return false;
	}
	
	return true;
}
