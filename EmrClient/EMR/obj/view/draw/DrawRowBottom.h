#ifndef _DRAW_ROW_BOTTOM_H_
#define _DRAW_ROW_BOTTOM_H_


#include <QColor>

#include "BaseDraw.h"

class ViewObj;


#include <QFont>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")      
#endif
class DrawRowBottom : public BaseDraw
{
public:
	DrawRowBottom(ViewObj * viewOwner);
	~DrawRowBottom();

public:
	// 绘制选中时上层填充


	bool draw(QPainter &pPainter, int x, int y, int w, int h);

private:

	void drawChildrenUnderline(QPainter &pPainter, RowViewParam * rowParam, int x, int y);

	void drawTxtBackgroundColor(QPainter &pPainter, RowViewParam * rowParam, int x, int y);
	void drawRound(QPainter &pPainter, RowViewParam * rowParam, int x, int y);

private:
	//DrawRect * mRect;
};

#endif // _DRAW_ROW_BOTTOM_H_
