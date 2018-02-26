#ifndef PICTUREPATH_H
#define PICTUREPATH_H

#include "EditImageWindow/PictureShape.h"
#include <QGraphicsPathItem>

#pragma once
class FreeLineItem : public QGraphicsPathItem, public PictureShape
{
public:
	FreeLineItem();
	~FreeLineItem();

	virtual void startDraw(QGraphicsSceneMouseEvent * event);
	virtual void drawing(QGraphicsSceneMouseEvent * event);
	virtual int  getType(); 
public:
	void getData(StrItemData& strData);
	void setData(StrItemData strData);
	void init(QPointF offsetPoint);
private:
	QPainterPath mPath;
	int mType;
};

#endif
