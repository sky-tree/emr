#include "BaseMath.h"
#include <QDebug>


#include "ViewObj.h"
//#include "BaseObj.h"
#include "BaseViewParam.h"
//#include <QtCore / qmath>

#include "UIConstants.h"
#include "BaseConstants.h"

const float BaseMath::PI = 3.1415926f;

BaseMath::BaseMath()
{
}

BaseMath::~BaseMath()
{
}



void BaseMath::refreshPageWHByPixel(bool isHorizontal, short paperType, int& w, int& h)
{
	switch (paperType)
	{
	case UIConstants::PaperTypeAndWH::A4:
		w = UIConstants::PaperTypeAndWH::A4_W_PIX;
		h = UIConstants::PaperTypeAndWH::A4_W_PIX * UIConstants::PaperTypeAndWH::A4_H / UIConstants::PaperTypeAndWH::A4_W;
		break;
	case UIConstants::PaperTypeAndWH::B5:
		w = UIConstants::PaperTypeAndWH::A4_W_PIX * UIConstants::PaperTypeAndWH::B5_W / UIConstants::PaperTypeAndWH::A4_W;
		h = UIConstants::PaperTypeAndWH::A4_W_PIX * UIConstants::PaperTypeAndWH::B5_H / UIConstants::PaperTypeAndWH::A4_W;
		break;
	}
	if (isHorizontal)
	{
		int temp = w;
		w = h;
		h = temp;
	}
}
void BaseMath::refreshPageWHByCentimeter(short paperType, int& w, int& h)
{
	switch (paperType)
	{
	case UIConstants::PaperTypeAndWH::A4:
		w = UIConstants::PaperTypeAndWH::A4_W;
		h = UIConstants::PaperTypeAndWH::A4_H;
		break;
	case UIConstants::PaperTypeAndWH::B5:
		w = UIConstants::PaperTypeAndWH::B5_W;
		h = UIConstants::PaperTypeAndWH::B5_W * UIConstants::PaperTypeAndWH::B5_H / UIConstants::PaperTypeAndWH::B5_W;
		break;
	}
}
int BaseMath::changeFresequencyToMillisecond(int fresequency)
{
	if (fresequency <= 0)
		qDebug() << "error: BaseMath::changeFresequencyToMillisecond fresequency <= 0 !";
	return 1000 / fresequency;
}
int BaseMath::changeMillisecondToFresequency(int millisecond)
{
	return millisecond / BaseConstants::DEFAULT_FREQUENCY;
}

int BaseMath::getAbs(int value)
{
	return value >= 0 ? value : -value;
}
bool BaseMath::haveColor(int color)
{
	return (color & 0xff000000);
}
bool BaseMath::haveColor(const QColor &color)
{
	return color.alpha() > 0;
}
QColor * BaseMath::createColor(int color)
{
	int r = (color & 0xff000000) >> 24;
	QColor * qColor = new QColor((color & 0x00ff0000) >> 16, (color & 0x0000ff00) >> 8, color & 0x000000ff, r);
	return qColor;
}
int BaseMath::exchangeQColorToColor(QColor color)
{
	int alpha = color.alpha();
	int red = color.red();
	int green = color.green();
	int blue = color.blue();

	int dest = (alpha << 24) | (red << 16) | (green << 8) | blue;
	return dest;
}
//bool BaseMath::isPointerInsizeRect(int x, int y, BaseObj * obj)
//{
//	int rx = obj->mViewData->getX();
//	int ry = obj->mViewData->getY();
//	int rw = obj->mViewData->getTotalWidth();
//	int rh = obj->mViewData->getTotalHeight();
//	return isPointerInsizeRect(x, y, rx, ry, rw, rh);
//}
bool BaseMath::isPointerInsizeRect(int x, int y, int rx, int ry, int rw, int rh)
{
	if (x < rx || x > rx + rw || y < ry || y > ry + rh)
		return false;
	return true;
}
//bool BaseMath::isInsideTotalArea(int x, int y, ViewObj * viewObj)
//{
//	BaseViewParam  * viewData = viewObj->getViewParam();
//	int rx = viewData->getX();
//	int ry = viewData->getY();
//	int rw = viewData->getTotalW();
//	int rh = viewData->getTotalH();
//	if (BaseMath::isPointerInsizeRect(x, y, rx, ry, rw, rh))
//	{
//		return true;
//	}
//	return false;
//}
bool BaseMath::isValueInArea(int x, int rx, int rw)
{
	if (x < rx || x > rx + rw)
		return false;
	return true;
}

char BaseMath::pointInsideDragObjArea(float percent, int x, int y, int rx, int ry, int rw, int rh)
{
	// 拖动按钮宽高
	int w = rw * percent; //BaseConstants::MouseDragScaleAbout::AREA_PERCENT;
	int h = rh * percent; //BaseConstants::MouseDragScaleAbout::AREA_PERCENT;
	char pointType = BaseConstants::NONE;
	// 上
	if (BaseMath::isPointerInsizeRect(x, y, rx, ry, rw, h))
	{
		pointType = BaseConstants::Direction::UP;
	}
	// 下
	else if (BaseMath::isPointerInsizeRect(x, y, rx, ry + rh - h, rw, h))
	{
		pointType = BaseConstants::Direction::DOWN;
	}
	// 左
	if (BaseMath::isPointerInsizeRect(x, y, rx, ry, w, rh))
	{
		pointType = BaseConstants::Direction::LEFT;
	}
	// 右
	else if (BaseMath::isPointerInsizeRect(x, y, rx + rw - w, ry, w, rh))
	{
		pointType = BaseConstants::Direction::RIGHT;
	}
	return pointType;
}
char BaseMath::pointInsideCornerType(float percent, int x, int y, int rx, int ry, int rw, int rh)
{
	// 拖动按钮宽高
	int w = rw * percent;
	int h = rh * percent;
	char pointType = BaseConstants::NONE;
	// 左上
	if (BaseMath::isPointerInsizeRect(x, y, rx, ry, w, h))
	{
		pointType = BaseConstants::Direction::LEFT_TOP;
	}
	// 左下
	else if (BaseMath::isPointerInsizeRect(x, y, rx, ry + rh - h, w, h))
	{
		pointType = BaseConstants::Direction::LEFT_BOTTOM;
	}
	// 右上
	if (BaseMath::isPointerInsizeRect(x, y, rx + rw - w, ry, w, h))
	{
		pointType = BaseConstants::Direction::RIGHT_TOP;
	}
	// 右下
	else if (BaseMath::isPointerInsizeRect(x, y, rx + rw - w, ry + rh - h, w, h))
	{
		pointType = BaseConstants::Direction::RIGHT_BOTTOM;
	}
	return pointType;
}
bool BaseMath::isRectCrossWH(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	return isRectCross(x1, y1, x1 + w1, y1 + h1, x2, y2, x2 + w2, y2 + h2);
}
bool BaseMath::isRectCross(int left1, int top1, int right1, int bottom1, int left2, int top2, int right2, int bottom2)
{
	int tMaxLeft = 0;
	int tMaxTop = 0;
	int tMinRight = 0;
	int tMinBottom = 0;
	tMaxLeft = left1 >= left2 ? left1 : left2;
	tMaxTop = top1 >= top2 ? top1 : top2;
	tMinRight = right1 <= right2 ? right1 : right2;
	tMinBottom = bottom1 <= bottom2 ? bottom1 : bottom2;
	if (tMaxLeft > tMinRight || tMaxTop > tMinBottom)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool BaseMath::isRectCrossExceptSameSide(int left1, int top1, int right1, int bottom1, int left2, int top2, int right2, int bottom2)
{
	int tMaxLeft = 0;
	int tMaxTop = 0;
	int tMinRight = 0;
	int tMinBottom = 0;
	tMaxLeft = left1 >= left2 ? left1 : left2;
	tMaxTop = top1 >= top2 ? top1 : top2;
	tMinRight = right1 <= right2 ? right1 : right2;
	tMinBottom = bottom1 <= bottom2 ? bottom1 : bottom2;
	if (tMaxLeft >= tMinRight || tMaxTop >= tMinBottom)
	{
		return false;
	}
	else
	{
		return true;
	}
}
//void BaseMath::judgePagePanleOutArea(ViewObj * window, BaseViewParam * panelViewParam)
//{
//	if (panelViewParam->getY() > UIConstants::WindowParams::WINDOW_PAGE_PANEL_Y)
//	{
//		panelViewParam->setY(UIConstants::WindowParams::WINDOW_PAGE_PANEL_Y);
//	}
//	else
//	{
//		AbsLayoutViewParam * winViewParam = (AbsLayoutViewParam *)window->getViewParam();
//		int winInsideBottomY = winViewParam->getInsideTop() + winViewParam->getInsideH();
//		int panelH = panelViewParam->getTotalH();
//		int destY = winInsideBottomY;
//		if (panelViewParam->getY() + panelH < destY
//			//- UIConstants::WindowParams::WINDOW_PAGE_PANEL_BOTTOM
//			)
//		{
//			float y = destY - panelH;
//			if (y > UIConstants::WindowParams::WINDOW_PAGE_PANEL_Y)
//			{
//				y = UIConstants::WindowParams::WINDOW_PAGE_PANEL_Y;
//			}
//			panelViewParam->setY(y);
//		}
//	}
//}
int BaseMath::square(int value)
{
	return value * value;
}
int BaseMath::getAngleEffectValue(int angle)
{
	if (angle < 0)
	{
		angle = angle % 360 + 360;
	}
	else if (angle >= 360)
	{
		angle %= 360;
	}
	return angle;
}
float BaseMath::changeAngleToRadian(float angle)
{
	return angle * PI / 180;
}
float BaseMath::getDistanceFromTwoPoints(int x0, int y0, int x1, int y1)
{
	int disX = x1 - x0;
	int disY = y1 - y0;
	float value = sqrt(square(disX) + square(disY));
	return value;
}
float BaseMath::getAngleFromTwoPoints(int x0, int y0, int x1, int y1)
{
	int disX = x1 - x0;
	int disY = y1 - y0;
	float longLine = getDistanceFromTwoPoints(x0, y0, x1, y1);
	float cos = disX / longLine;
	float rad = acos(cos);
	float angle = 180 / (M_PI / rad);
	if (disY < 0)
		angle = 360 - angle;
	return angle;
}
bool BaseMath::isInsideTotalArea(int x, int y, ViewObj * viewObj)
{
	BaseViewParam  * viewData = viewObj->getViewParam();
	int rx = viewData->getX();
	int ry = viewData->getY();
	int rw = viewData->getTotalW();
	int rh = viewData->getTotalH();
	if (BaseMath::isPointerInsizeRect(x, y, rx, ry, rw, rh))
	{
		return true;
	}
	return false;
}