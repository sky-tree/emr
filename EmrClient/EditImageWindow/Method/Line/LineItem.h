#ifndef _LINE_H_
#define _LINE_H_
#include <QGraphicsLineItem>
#include "EditImageWindow/PictureShape.h"

#pragma once
class LineItem: public QGraphicsLineItem, public PictureShape
{
public:
	LineItem();
	~LineItem();
	virtual void startDraw(QGraphicsSceneMouseEvent * event);
	virtual void drawing(QGraphicsSceneMouseEvent * event);
	virtual int getType();
public:
	void getData(StrItemData& strData);
	void setData(StrItemData strData);
	void init(QPointF offsetPoint);
private:
	QLineF mLine;
	int mType;
};

#endif