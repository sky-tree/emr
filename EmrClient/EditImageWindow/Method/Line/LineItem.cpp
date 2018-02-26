#include "LineItem.h"
#include <QGraphicsSceneMouseEvent>
#include <qstyleoption.h>
#include <QPen>
LineItem::LineItem() :
mType(PictureShape::Shape::Line)
{
	this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}


LineItem::~LineItem()
{
}

void LineItem::init(QPointF offsetPoint)
{
	setLine(mLine);
	this->moveBy(offsetPoint.x(), offsetPoint.y());
}

void LineItem::startDraw(QGraphicsSceneMouseEvent *event)
{
	this->setLine(QLineF(event->scenePos(), event->scenePos()));
}

void LineItem::drawing(QGraphicsSceneMouseEvent *event)
{
	QLineF newLine(line().p1(), event->scenePos());
	setLine(newLine);
	mLine = newLine;
}

int LineItem::getType()
{
	return mType;
}

void LineItem::getData(StrItemData& strData)
{
	strData.strLine = mLine;
}

void LineItem::setData(StrItemData strData)
{
	mLine = strData.strLine;
}
