#include "EllipseItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QtWidgets/qstyleoption.h>
#include <math.h>
#include <QPen>

EllipseItem::EllipseItem() :
	mType(PictureShape::Shape::EllipseItem)
{
	setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	this->setBrush(QBrush(QColor(Qt::red)));
}


EllipseItem::~EllipseItem()
{
}


int EllipseItem::getType()
{
	return mType;
}

void EllipseItem::init(QPointF offsetPoint)
{
	this->setRect(mRect);
	this->moveBy(offsetPoint.x(), offsetPoint.y());
}


void EllipseItem::startDraw(QGraphicsSceneMouseEvent * event)
{
	mStartPoint = event->scenePos().toPoint();
}

void EllipseItem::drawing(QGraphicsSceneMouseEvent * event)
{
	QPointF newPoint = event->scenePos();
	mRect.setCoords(mStartPoint.x(), mStartPoint.y(), newPoint.x(), newPoint.y());
	this->setRect(mRect);
}

void EllipseItem::getData(StrItemData& strData)
{
	strData.strRect = mRect;
}

void EllipseItem::setData(StrItemData strData)
{
	mRect = strData.strRect;
}
