#include "CircularItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QtWidgets/qstyleoption.h>
#include <math.h>
#include <QPen>

CircularItem::CircularItem():
	mType(PictureShape::Shape::Circular)
{
	setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	this->setBrush(QBrush(QColor(Qt::red)));
}


CircularItem::~CircularItem()
{
}

int CircularItem::getType()
{
	return mType;
}

void CircularItem::init(QPointF offsetPoint)
{
	this->setRect(mRect);
	this->moveBy(offsetPoint.x(), offsetPoint.y());
}


void CircularItem::startDraw(QGraphicsSceneMouseEvent * event)
{
	mStartPoint = event->scenePos().toPoint();
}

void CircularItem::drawing(QGraphicsSceneMouseEvent * event)
{
	QPointF newPoint = event->scenePos();
	int distance = sqrt(pow(newPoint.x() - mStartPoint.x(), 2)
		+ pow(newPoint.y() - mStartPoint.y(), 2));
	mRect.setCoords(distance, distance, -distance, -distance);
	mRect.moveCenter(mStartPoint.toPoint());
 	this->setRect(mRect);
}

void CircularItem::getData(StrItemData& strData)
{
	strData.strRect = mRect;
}

void CircularItem::setData(StrItemData strData)
{
	mRect = strData.strRect;
}
