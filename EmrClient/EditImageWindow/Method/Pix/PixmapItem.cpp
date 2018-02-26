#include "PixmapItem.h"


PixmapItem::PixmapItem(const QPixmap &pixmap, QGraphicsItem *parent):
	QGraphicsPixmapItem(pixmap),
	mType(PictureShape::Shape::Pixmap)
{
}


PixmapItem::~PixmapItem()
{
}

void PixmapItem::init()
{

}


void PixmapItem::startDraw(QGraphicsSceneMouseEvent * event)
{
	return;
}

void PixmapItem::drawing(QGraphicsSceneMouseEvent * event)
{
	return;
}

int PixmapItem::getType()
{
	return mType;
}

void PixmapItem::getData()
{

}

void PixmapItem::setData()
{

}
