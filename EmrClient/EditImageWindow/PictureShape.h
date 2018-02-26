#ifndef _PICTURE_SHAPE_H_
#define _PICTURE_SHAPE_H_
#pragma once
#include <QDebug>
#include "BaseEditParam.h"

class QGraphicsSceneMouseEvent;

class PictureShape
{
public:
	enum Shape
	{
		None,
		Txt,
		FreeLine,
		Line,
		Arrow,
		Circular,
		EllipseItem,
		RoundRect,
		Pixmap,
	};
public:
	virtual void startDraw(QGraphicsSceneMouseEvent * event) = 0;
	virtual void drawing(QGraphicsSceneMouseEvent * event) = 0;
	virtual int getType() = 0;
};

#endif