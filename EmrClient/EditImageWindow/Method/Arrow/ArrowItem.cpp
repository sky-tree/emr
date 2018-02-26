#include "ArrowItem.h"

#include <QPainterPath>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QtCore/qmath.h>
#include <QDebug>
#include <float.h>
#include <QPen>
#include <math.h>
#include "BaseMath.h"

#define Pi 3.14159265358979323846

ArrowItem::ArrowItem(): mType(PictureShape::Shape::Arrow)
{
	this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	this->setBrush(QBrush(QColor(Qt::red)));
}

ArrowItem::~ArrowItem()
{

}

void ArrowItem::startDraw(QGraphicsSceneMouseEvent * event)
{
	mStartPoint = event->scenePos();
}

void ArrowItem::drawing(QGraphicsSceneMouseEvent * event)
{
	qreal lt = 20;
	QPointF qPos = event->scenePos();
	if (BaseMath::getDistanceFromTwoPoints(qPos.x(), qPos.y(), mStartPoint.x(), mStartPoint.y()) > 20)
	{
		lt = 20;//计算长度，分段处理
	}

	QLineF line(mStartPoint, event->scenePos());
	qreal iAngle =  line.angle() + 90;
	if (iAngle > 360)
	{
		iAngle -= 360;//计算拉伸的角度
	}

	qreal xt = lt * cos((90 - iAngle) * Pi / 180);
	qreal yt = lt * sin((90 - iAngle) * Pi / 180);
	xt = event->scenePos().x() - xt;
	yt = event->scenePos().y() - yt;	//计算三角形底部中点坐标

	qreal dx = lt * cos(iAngle * Pi / 180);
	qreal dy = lt * sin(iAngle * Pi / 180);//计算三角形底边长度


	QPolygonF Polygon;//分段处理箭头绘制
	if (BaseMath::getDistanceFromTwoPoints(qPos.x(), qPos.y(), mStartPoint.x(), mStartPoint.y()) < 20)
	{
		Polygon <<  QPointF(xt - 0.7 * dx, yt + 0.7 * dy)
			<< qPos
			<< QPointF(xt + 0.7 * dx, yt - 0.7 * dy) ;
	}
	else
	{
		Polygon << mStartPoint
			<< QPointF(xt - 0.3 * dx, yt + 0.3 * dy) << QPointF(xt - 0.7 * dx, yt + 0.7 * dy)
			<< qPos
			<< QPointF(xt + 0.7 * dx, yt - 0.7 * dy) << QPointF(xt + 0.3 * dx, yt - 0.3 * dy);
	}
	
	this->setPolygon(Polygon);
	mPolygon = Polygon;
}

int ArrowItem::getType()
{
	return mType;
}

void ArrowItem::getData(StrItemData& strData)
{
	strData.strPolygon = mPolygon;
}

void ArrowItem::setData(StrItemData strData)
{
	mPolygon = strData.strPolygon;
}

void ArrowItem::init(QPointF offsetPoint)
{
	this->setPolygon(mPolygon);
}