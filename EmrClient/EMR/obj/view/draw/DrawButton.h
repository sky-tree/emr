#ifndef _DRAW_BUTTON_
#define _DRAW_BUTTON_

#pragma once
#include "BaseDraw.h"

class ViewObj;
class ImgConTentData;
class ImgViewParam;

class DrawButton :
	public BaseDraw
{
public:
	DrawButton(ViewObj * viewOwner);
	~DrawButton();
private:
	QVector<QImage*> mImageResVector;
	int				 mIndex;
public:
	void init(QString resRelease, QString resPress);
	void release();
	void setIndex(int index);
	bool draw(QPainter &pPainter, int x, int y, int w, int h);
};

#endif