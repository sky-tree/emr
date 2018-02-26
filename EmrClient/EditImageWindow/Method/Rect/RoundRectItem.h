#ifndef _ROUND_RECT_ITEM_H_
#define _ROUND_RECT_ITEM_H_

#pragma once

#include "qgraphicsitem.h"
#include "EditImageWindow/PictureShape.h"

class RoundRectItem :
	public QGraphicsPolygonItem, public PictureShape
{
public:
	RoundRectItem();
	~RoundRectItem();
public:
	virtual void startDraw(QGraphicsSceneMouseEvent * event);
	virtual void drawing(QGraphicsSceneMouseEvent * event);
	virtual int getType();
public:
	void getData(StrItemData& strData);
	void setData(StrItemData strData);
	void init(QPointF offsetPoint);
private:
	QPointF		mStartPoint;
	QPolygonF	mPolygon;
	int			mType;
};

#endif