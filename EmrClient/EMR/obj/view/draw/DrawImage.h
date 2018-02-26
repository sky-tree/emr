#ifndef _DRAWIMAGE_H_
#define _DRAWIMAGE_H_
#pragma once
#include "BaseDraw.h"

class ViewObj;
class ImgConTentData;
class ImgViewParam;

class DrawImage :
	public BaseDraw
{
public:
	DrawImage(ViewObj * viewOwner, ImgConTentData * drawParam);
	~DrawImage();
public:
	
	bool draw(QPainter &pPainter, int x, int y, int w, int h);

private:
	ImgConTentData * mDrawParam;
	QRect * mRect;
};

#endif
