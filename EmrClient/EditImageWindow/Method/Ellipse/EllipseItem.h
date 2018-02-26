#ifndef _ELLIPSE_ITEM_H_
#define  _ELLIPSE_ITEM_H_

#pragma once
#include "qgraphicsitem.h"
#include "EditImageWindow/PictureShape.h"

class EllipseItem :
	public QGraphicsEllipseItem, public PictureShape
{
public:
	EllipseItem();
	~EllipseItem();

public:
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

#endif