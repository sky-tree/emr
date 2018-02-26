#ifndef _CIRCULAR_H_
#define _CIRCULAR_H_

#pragma once
#include <QGraphicsPathItem>
#include "EditImageWindow/PictureShape.h"

class CircularItem :public QGraphicsEllipseItem, public PictureShape
{
public:
	CircularItem();
	~CircularItem();

	virtual void startDraw(QGraphicsSceneMouseEvent * event);
	virtual void drawing(QGraphicsSceneMouseEvent * event);
	virtual int getType();
public:
	void getData(StrItemData& strData);
	void setData(StrItemData strData);
	void init(QPointF offsetPoint);
private:
	QPointF  mStartPoint;
	QRect	 mRect;
	int mType;
};

#endif // !_CIRCULAR_H_