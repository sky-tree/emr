#include "TextItem.h"

#include <QFont>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

#include "BaseMath.h"

RichTextItem::RichTextItem():
	QGraphicsTextItem(QString::fromLocal8Bit("")),
	mType(PictureShape::Shape::Txt), mbIsEnter(false), mbIsChoosed(false)
{
	QFont font = this->font();
	font.setPixelSize(25);  // 像素大小
//	font.setItalic(true);  // 斜体
//	font.setUnderline(true);  // 下划线
	this->setFont(font);
	this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	this->setTextInteractionFlags(Qt::TextEditorInteraction);  // 可编辑
	this->setTextWidth(120);
}

RichTextItem::~RichTextItem()
{

}
void RichTextItem::startDraw(QGraphicsSceneMouseEvent * event)
{
	this->moveBy(event->scenePos().x(), event->scenePos().y());
	mPoint = event->scenePos();
}
void RichTextItem::drawing(QGraphicsSceneMouseEvent * event)
{

}

int RichTextItem::getType()
{
	return mType;
}

void RichTextItem::getData(StrItemData& strData)
{
	strData.strPoint = mPoint;
	strData.strQstr = this->toPlainText();
}

void RichTextItem::setData(StrItemData strData)
{
	mPoint = strData.strPoint;
	this->setPlainText(strData.strQstr);
}
void RichTextItem::init(QPointF offsetPoint)
{
	this->moveBy(offsetPoint.x(), offsetPoint.y());
	this->setTextWidth(-1);
}

bool RichTextItem::isEmptyItem()
{
	bool bEmpty = this->toPlainText().size();
	return !bEmpty;
}

bool RichTextItem::isChoosedItem()
{
	return mbIsChoosed;
}


void RichTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	isSelectedItem(event->scenePos());
	QGraphicsTextItem::mousePressEvent(event);
}

void RichTextItem::keyPressEvent(QKeyEvent *event)
{
	if (mbIsEnter == false)
	{
		this->setTextWidth(-1);
		mbIsEnter = true;
	}
	QGraphicsTextItem::keyPressEvent(event);
}


void RichTextItem::inputMethodEvent(QInputMethodEvent *event)
{
	if (mbIsEnter == false)
	{
		this->setTextWidth(-1);
		mbIsEnter = true;
	}
	QGraphicsTextItem::inputMethodEvent(event);
}


void RichTextItem::isSelectedItem(QPointF currentPoint)
{
	mbIsChoosed = false;
	QRectF qsize = this->boundingRect();
	QPointF qPoint = this->scenePos();
	int x = this->scenePos().x();
	int y = this->scenePos().y();
	int w = x + this->boundingRect().width();
	int h = y + this->boundingRect().height();

	if (BaseMath::isPointerInsizeRect(currentPoint.x(), currentPoint.y(), x, y, w, h) == true 
		&&
		BaseMath::isPointerInsizeRect(currentPoint.x(), currentPoint.y(), x + 5, y + 5, w - 5, h - 5) == false)
	{
		mbIsChoosed = true;
	}
}
