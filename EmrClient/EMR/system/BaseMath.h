#ifndef BASE_MATH_H_
#define BASE_MATH_H_

#include<QColor>
#include<QtMath>
class ViewObj;

// 数学相关函数
class BaseMath
{
public:
	static const float PI;

public:
	BaseMath();
	~BaseMath();
public:

	static void refreshPageWHByPixel(bool isHorizontal, short paperType, int& w, int& h);
	static void refreshPageWHByCentimeter(short paperType, int& w, int& h);
	static int changeFresequencyToMillisecond(int fresequency);
	static int changeMillisecondToFresequency(int millisecond);

	static int getAbs(int value);
	static bool haveColor(int color);
	static bool haveColor(const QColor &color);
	static QColor * createColor(int color);
	static int exchangeQColorToColor(QColor color);

	static bool isPointerInsizeRect(int x, int y, int rx, int ry, int rw, int rh);
	//static bool isInsideTotalArea(int x, int y, ViewObj * viewObj);
	static bool isValueInArea(int x, int rx, int rw);

	static char pointInsideDragObjArea(float percent, int x, int y, int rx, int ry, int rw, int rh);
	static char pointInsideCornerType(float percent, int x, int y, int rx, int ry, int rw, int rh);
	static bool isRectCrossWH(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
	static bool isRectCross(int left1, int top1, int right1, int bottom1, int left2, int top2, int right2, int bottom2);
	static bool isRectCrossExceptSameSide(int left1, int top1, int right1, int bottom1, int left2, int top2, int right2, int bottom2);
	//static void judgePagePanleOutArea(ViewObj * window, BaseViewParam * panelViewParam);

	static int getAngleEffectValue(int angle);
	static float changeAngleToRadian(float angle);
	static float getDistanceFromTwoPoints(int x0, int y0, int x1, int y1);
	static float getAngleFromTwoPoints(int x0, int y0, int x1, int y1);
	static int square(int value);
	static bool isInsideTotalArea(int x, int y, ViewObj * viewObj);
};

#endif // BASE_MATH_H_
