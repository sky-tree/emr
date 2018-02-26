#include "DrawImage.h"

#include "qdebug.h"

#include "ViewObj.h"
#include "BaseViewParam.h"
#include "BaseScaleParam.h"
#include "RowViewParam.h"

#include "ImgContentData.h"

#include "OperateMgr.h"
#include "SystemParams.h"


#include "ConstantsNumber.h"

#include "ConstantsID.h"
#include "UIConstants.h"

DrawImage::DrawImage(ViewObj * viewOwner, ImgConTentData * drawParam) : BaseDraw(viewOwner, ConstantsID::ClassId::DRAW_IMAGE)
{
	mDrawParam = drawParam;
	mRect = new QRect();
}

DrawImage::~DrawImage()
{
	if (mRect)
	{
		delete mRect;
		mRect = NULL;
	}
}


bool DrawImage::draw(QPainter &pPainter, int x, int y, int w, int h)
{
	BaseViewParam * viewParam = mViewOwner->getViewParam();
	if (mViewOwner->isEnter())
	{
		SystemParams * sysParams = mViewOwner->getOperateMgr()->getSystemParams();
		if (sysParams->isTidy())
		{
			return true;
		}
	}
	if (w == ConstantsNumber::MIN_CHAR)
	{
		w = viewParam->getTotalW();
	}
	if (h == ConstantsNumber::MIN_CHAR)
	{
		h = viewParam->getTotalH();
	}
	ViewObj * row = mViewOwner->getViewParent();
	RowViewParam * rowParam = (RowViewParam *)row->getViewParam();
	int rowInsideTop = rowParam->getInsideTop();
	int rowInsideH = rowParam->getInsideH();
	y += rowInsideTop + rowInsideH - h;
	//QRect *rect = new QRect(x, y, w, h);
	mRect->setRect(x,y,w,h);
	//// ����ͼƬ
	QImage * snap = mDrawParam->getSnap();
	// ����н�ͼ����ֱ�ӻ��ƽ�ͼ
	if (snap != NULL)
	{
		pPainter.drawImage(*mRect, *snap);
	}
	else
	{
		pPainter.drawImage(*mRect, *mDrawParam->getImg());
	}
	BaseScaleParam * scaleParam = viewParam->getBaseScaleParam();
	// ������Ŀ��ͼƬ��͸��
	if (mViewOwner->getObjID() == ConstantsID::ContainerId::IMG && 
		mViewOwner->getStatus() == BaseConstants::ObjStatus::DRAG_SCALE
		&& scaleParam->getStartPointType() != BaseConstants::Direction::MIDDLE)
	{
		BaseScaleParam * scaleData = viewParam->getBaseScaleParam();
		int dx = 0, dy = 0, dw = 0, dh = 0;
		scaleData->getParam(dx, dy, dw, dh);
		dx += x - viewParam->getX();
		dy += y - viewParam->getY();
		mRect->setLeft(dx);
		mRect->setTop(dy);
		mRect->setWidth(dw);
		mRect->setHeight(dh);

		// ����ͼƬ
		int preAlpha = pPainter.opacity();
		pPainter.setOpacity(UIConstants::IMG_DRAG_ALPHA);
		//ͼƬ����
		pPainter.drawImage(*mRect, *mDrawParam->getImg());
		pPainter.setOpacity(preAlpha);
	}
	return true;
}
