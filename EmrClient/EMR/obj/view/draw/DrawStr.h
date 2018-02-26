#ifndef _DRAW_STR_H_
#define _DRAW_STR_H_


#include "BaseDraw.h"

class ViewObj;
class StrContentData;
class TxtViewParam;


// 字符串绘制
class DrawStr : public BaseDraw
{
public:
	DrawStr(ViewObj * viewOwner);
	~DrawStr();

public:

	bool drawStrList(QPainter &pPainter, int x, int y, QList<StrContentData *> * list);
	bool draw(QPainter &pPainter, int x, int y, int w, int h);

private:
	void drawStr(QPainter &pPainter, QString str, int x, int y);
	//QString judgeStrWithPageNumber(DrawTxtParam * source);
	//QString rebuildStr(QList<DrawTxtParam *> * list);
private:
	StrContentData * mTxtData;
	TxtViewParam * mTxtViewParam;
};

#endif // _DRAW_STR_H_
