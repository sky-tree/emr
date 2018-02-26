#ifndef _MY_GRAPHICS_SCENE_H_
#define _MY_GRAPHICS_SCENE_H_

#pragma once
#include "qgraphicsscene.h"
#include "EditImageWindow/PictureShape.h"
//#include "PictureShape.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "BaseEditParam.h"
#include "EditImageWindow/PictureEditWindow.h"

class MyGraphicsScene : public QGraphicsScene
{
public:

	MyGraphicsScene(QString ImagePath, PictureEditWindow * parent);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

	void setType(PictureShape::Shape type = PictureShape::None);
	void setAttribute();
	int width();
	int height();
	void addLastItems(QList<StrItemData>& items);
	bool isChanged();
	void cancelFoucs();
	QList<StrItemData>& saveData();
	void changedFoucs();

private:
	void createShape(QGraphicsSceneMouseEvent *event);
	void deleteItem(QKeyEvent *event);
	void addLastItem();
	void initImg(QString imagePath);
private:
	PictureEditWindow*			mParent;
	PictureShape::Shape			mShapeType;
	PictureShape *				mShape;
	QGraphicsItem *				mItemToRemove;
	QSize						mSize;
	QList<StrItemData>			mItems;
	bool						mbAddItem;
	bool						mbIsChanged;
	bool						mbIsLeftButtonDown;
	int							miAddTxtItem;
	int							miNeedSetFocus;
};

#endif