#include "FreeLineItem.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>
#include <qstyleoption>
#include <QDebug>
#include <QPen>

FreeLineItem::FreeLineItem():
	mType(PictureShape::Shape::FreeLine)
{
	this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

FreeLineItem::~FreeLineItem()
{

}

void FreeLineItem::init(QPointF offsetPoint)
{
	setPath(mPath);
	this->moveBy(offsetPoint.x(), offsetPoint.y());
}

void FreeLineItem::startDraw(QGraphicsSceneMouseEvent *event)
{
	QPainterPath path;
	path.moveTo(event->scenePos());
	setPath(path);
}

void FreeLineItem::drawing(QGraphicsSceneMouseEvent *event)
{
	QPainterPath path;
	path.moveTo(this->path().currentPosition());
	path.addPath(this->path());
	path.lineTo(event->scenePos());
	setPath(path);
	mPath = path;
}
int FreeLineItem::getType()
{
	return mType;
}
void FreeLineItem::getData(StrItemData& strData)
{
	strData.strPath = mPath;
}

void FreeLineItem::setData(StrItemData strData)
{
	mPath = strData.strPath;
}
