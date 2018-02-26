#ifndef _ARROW_H_
#define _ARROW_H_

#include <QGraphicsPathItem>
#include "EditImageWindow/PictureShape.h"
#pragma once

class ArrowItem : public QGraphicsPolygonItem, public PictureShape
{
public:
	ArrowItem();
	~ArrowItem();

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