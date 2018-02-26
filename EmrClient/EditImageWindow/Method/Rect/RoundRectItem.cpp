#include "RoundRectItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QtWidgets/qstyleoption.h>
#include <math.h>
#include <QPen>


RoundRectItem::RoundRectItem():
	mType(PictureShape::Shape::RoundRect)
{
	setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	this->setBrush(QBrush(QColor(Qt::red)));
}


RoundRectItem::~RoundRectItem()
{
}


int RoundRectItem::getType()
{
	return mType;
}

void RoundRectItem::init(QPointF offsetPoint)
{
	this->setPolygon(mPolygon);
}


void RoundRectItem::startDraw(QGraphicsSceneMouseEvent * event)
{
	mStartPoint = event->scenePos().toPoint();
}

void RoundRectItem::drawing(QGraphicsSceneMouseEvent * event)
{
	QPointF newPoint = event->scenePos();
	QPainterPath path;
	QRectF rect;
	rect.setCoords(mStartPoint.x(), mStartPoint.y(), newPoint.x(), newPoint.y());
	path.addRoundRect(rect, 15, 15);   //�������������ķ�Χ0-99��ֵԽ��Խ԰
	QPolygon polygon = path.toFillPolygon().toPolygon();//������·���ϵ����еĵ�
	this->setPolygon(polygon);
	mPolygon = polygon;
}

void RoundRectItem::getData(StrItemData& strData)
{
	strData.strPolygon = mPolygon;
}

void RoundRectItem::setData(StrItemData strData)
{
	mPolygon = strData.strPolygon;
}
