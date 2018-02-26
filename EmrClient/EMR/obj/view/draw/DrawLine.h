#ifndef _DRAWLINE_H_
#define _DRAWLINE_H_
#include "BaseDraw.h"
class DrawLine :
	public BaseDraw
{
public:
	DrawLine(ViewObj * viewOwner);
	~DrawLine();
	void setLineParam(int lineColor,int lineWidth,char type);
	bool draw(QPainter &pPainter, int x, int y, int w, int h);
private:
	// 线框色
	QColor * mLineColor;
	//线样式
	//Qt::PenStyle mPenStyle;
	char mPenStyle;
	//线宽度
	int mLineWidth;
};
#endif

