#ifndef _DRAW_HORIZONTAL_LINE_
#define _DRAW_HORIZONTAL_LINE_

#pragma once
#include "BaseDraw.h"
class DrawHorizontalLine :
	public BaseDraw
{
public:
	DrawHorizontalLine(ViewObj * viewOwner, char type);
	DrawHorizontalLine(ViewObj * owner, DrawHorizontalLine *otherDraw);

	~DrawHorizontalLine();

private:
	void initFont();


public:
	bool draw(QPainter &pPainter, int x, int y, int w, int h);

private:
	//StrContentData * mFont;
	bool mPreHorizontalPage;
	QString mStr;
	int mDisStrX;
	int mStrW;
	int mStrAscent;
};
#endif
