#ifndef _TEXT_ITEM_H_
#define _TEXT_ITEM_H_

#pragma once


#include <QGraphicsTextItem>


#include "qgraphicsitem.h"
#include "EditImageWindow/PictureShape.h"
#include "BaseEditParam.h"

class QTextDocument;
class QRubberBand;

using namespace std;

class RichTextItem : public QGraphicsTextItem, public PictureShape
{
public:
	RichTextItem();
	~RichTextItem();
public:
	virtual void startDraw(QGraphicsSceneMouseEvent * event);
	virtual void drawing(QGraphicsSceneMouseEvent * event);
	virtual int getType();
public:
	void getData(StrItemData& strData);
	void setData(StrItemData strData);
	void init(QPointF offsetPoint);
	bool isEmptyItem();
	bool isChoosedItem();
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void inputMethodEvent(QInputMethodEvent *event);
private:
	void isSelectedItem(QPointF currentPoint);

private:
	QPointF		mPoint;
	QString		mStr;
	int			mType;
	bool		mbIsEnter;
	bool		mbIsChoosed;
};


#endif // CCQRAPHICSTEXTITEM_H
