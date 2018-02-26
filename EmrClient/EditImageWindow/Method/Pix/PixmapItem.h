#ifndef _PIXMAP_H_
#define _PIXMAP_H_

#include <QPixmap>
#include <QGraphicsitem>
#include "EditImageWindow/PictureShape.h"

#pragma once
class QGraphicsItem;

class PixmapItem :
	 public QGraphicsPixmapItem, public PictureShape
{
public:
	PixmapItem(const QPixmap &pixmap, QGraphicsItem *parent = 0);
	~PixmapItem();

public:
	virtual void startDraw(QGraphicsSceneMouseEvent * event);
	virtual void drawing(QGraphicsSceneMouseEvent * event);
	virtual int getType();
public:
	void getData();
	void init();
	void setData();
private:
	int mType;
};

#endif