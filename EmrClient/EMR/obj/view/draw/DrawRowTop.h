#ifndef _DRAW_ROW_TOP_H_
#define _DRAW_ROW_TOP_H_


#include <QColor>

#include "BaseDraw.h"

class ViewObj;


#include <QFont>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")      
#endif
class DrawRowTop : public BaseDraw
{
public:
	DrawRowTop(ViewObj * viewOwner);
	~DrawRowTop();

public:
	bool draw(QPainter &pPainter, int x, int y, int w, int h);

private:
	void drawParagraphNum(QPainter &pPainter, int x, int y);
	// 绘制选中时上层填充
	void drawChildrenMask(QPainter &pPainter, int x, int y, QList<BaseContainer *> * children);

	void drawExcelCrossPageLine(QPainter &pPainter, int x, int y);

private:

};

#endif // _DRAW_ROW_TOP_H_
