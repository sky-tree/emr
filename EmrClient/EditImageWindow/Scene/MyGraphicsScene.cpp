#include "MyGraphicsScene.h"

#include "EditImageWindow/Method/Line/LineItem.h"
#include "EditImageWindow/Method/FreeLine/FreeLineItem.h"
#include "EditImageWindow/Method/Arrow/ArrowItem.h"
#include "EditImageWindow/Method/Txt/TextItem.h"
#include "EditImageWindow/Method/Circular/CircularItem.h"
#include "EditImageWindow/Method/Ellipse/EllipseItem.h"
#include "EditImageWindow/Method/Rect/RoundRectItem.h"
#include "EditImageWindow/Method/Pix/PixmapItem.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>
#include <QTextDocument>

MyGraphicsScene::MyGraphicsScene(QString imagePath, PictureEditWindow * parent) :
	mShape(NULL), mShapeType(PictureShape::None),mItemToRemove(NULL),
	mbAddItem(false), mbIsLeftButtonDown(false), miAddTxtItem(0),
	mParent(parent), miNeedSetFocus(0)
{
	initImg(imagePath);
	QRectF qRect(0, 0, mSize.width(), mSize.height());
	this->setSceneRect(qRect);
	mbIsChanged = false;
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	if(event->button() == Qt::MouseButton::LeftButton)
	{
		mbIsLeftButtonDown = true;
		if(mItemToRemove)
			mItemToRemove->setSelected(false);
		mItemToRemove = NULL;
		foreach(QGraphicsItem *item, items(event->scenePos()))
		{
			PictureShape* p = dynamic_cast<PictureShape *>(item);
			switch (p->getType())
			{
				case PictureShape::Txt:
				{
					if (miNeedSetFocus++ == 0)
					{
						mParent->canleFocus();
						this->clearSelection();
						setFocusItem(item);
					}
					mItemToRemove = item;
					break;
				}
				case PictureShape::FreeLine:
				case PictureShape::Line:
				case PictureShape::Arrow:
				case PictureShape::Circular:
				case PictureShape::EllipseItem:
				case PictureShape::RoundRect:
				{
					mItemToRemove = item;
					break;
				}
				case PictureShape::Pixmap:
					break;
				default:
					break;
			}

		}
		if (!mItemToRemove)
		{
			if (mShapeType != PictureShape::None)
			{
				createShape(event);
				if (mShapeType != PictureShape::Txt)
				{
					miAddTxtItem = 0;
				}
			}
		}
	}
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	if (mbIsLeftButtonDown == true)
	{
		mbIsChanged = true;
		if (mItemToRemove == NULL)
		{
			if (mbAddItem)
			{
				addLastItem();
				mbAddItem = false;
			}
			if (mShape != NULL && mShapeType != PictureShape::None)
			{
				mShape->drawing(event);
			}
		}
	}
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseReleaseEvent(event);
	if (event->button() == Qt::MouseButton::LeftButton)
	{ 
		mbIsLeftButtonDown = false;
		if (mItemToRemove == NULL)
		{
			QGraphicsItem * item = dynamic_cast<QGraphicsItem *>(mShape);
			if (item != NULL && mShapeType != PictureShape::None)
			{
				item->setSelected(true);
				mShape = NULL;
			}
		}
	}
}

void MyGraphicsScene::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Backspace:
	case Qt::Key_Delete:
	{
		deleteItem(event);
		mbIsChanged = true;
		break;
	}
	default:
		QGraphicsScene::keyPressEvent(event);        // 其他按键处理交给父类  
		break;
	}
}



void MyGraphicsScene::setType(PictureShape::Shape type)
{
	mShapeType = type;
}

void MyGraphicsScene::setAttribute()
{

}


void MyGraphicsScene::createShape(QGraphicsSceneMouseEvent *event)
{
	switch (mShapeType) {
	case PictureShape::Line:
	{
		mShape = new LineItem();
		mShape->startDraw(event);
	}
	break;
	case PictureShape::FreeLine:
	{
		mShape = new FreeLineItem();
		mShape->startDraw(event);
	}
	break;
	case PictureShape::Arrow:
	{
		mShape = new ArrowItem();
		mShape->startDraw(event);
	}
	break;
	case PictureShape::Txt:
	{
		mShape = new RichTextItem();
		mShape->startDraw(event);
		if ((miAddTxtItem++ % 2) == 0)
		{
			addItem((RichTextItem*)mShape);
			mbIsChanged = true;
			mParent->canleFocus();
			QGraphicsItem * item = dynamic_cast<QGraphicsItem*>(mShape);
			setFocusItem(item);
			miNeedSetFocus = 1;
		}
	}
	break;
	case PictureShape::Circular:
	{
		mShape = new CircularItem();
		mShape->startDraw(event);
	}
	break;
	case PictureShape::EllipseItem:
	{
		mShape = new EllipseItem();
		mShape->startDraw(event);
	}
	break;
	case PictureShape::RoundRect:
	{
		mShape = new RoundRectItem();
		mShape->startDraw(event);
	}
	break;
	default:
		break;
	}
	mbAddItem = true;
}

void MyGraphicsScene::deleteItem(QKeyEvent *event)
{
	if (mItemToRemove)
	{
		PictureShape* p = dynamic_cast<PictureShape*>(mItemToRemove);
		switch (p->getType())
		{
		case PictureShape::Txt:
		{
			PictureShape* p = dynamic_cast<PictureShape*>(mItemToRemove);
			if (p->getType() == PictureShape::Txt)
			{
				RichTextItem * pTxt = dynamic_cast<RichTextItem*>(mItemToRemove);
				if (pTxt->isChoosedItem() == false)
				{
					QGraphicsScene::keyPressEvent(event);
					break;
				}
			}
		}
		case PictureShape::Arrow:
		case PictureShape::Circular:
		case PictureShape::FreeLine:
		case PictureShape::Line:
		case PictureShape::EllipseItem:
		case PictureShape::RoundRect:
		{
			removeItem(mItemToRemove);
			mItemToRemove = NULL;
		}
		break;
		case PictureShape::Pixmap:
			break;
		default:
			break;
		}
	}
}

void MyGraphicsScene::addLastItem()
{
	switch (mShapeType) {
	case PictureShape::Line:
	{
		this->addItem((LineItem *)mShape);
	}
	break;
	case PictureShape::FreeLine:
	{
		this->addItem((FreeLineItem *)mShape);
	}
	break;
	case PictureShape::Arrow:
	{
		this->addItem((ArrowItem *)mShape);
	}
	break;
	case PictureShape::Txt:
	{
	//	this->addItem((RichTextItem*)mShape);
	}
	break;
	case PictureShape::Circular:
	{
		this->addItem((CircularItem *)mShape);
	}
	break;
	case PictureShape::EllipseItem:
	{
		this->addItem((EllipseItem *)mShape);
	}
	break;
	case PictureShape::RoundRect:
	{
		this->addItem((RoundRectItem*)mShape);
	}
	break;
	default:
		break;
	}
}

int MyGraphicsScene::width()
{
	return mSize.width();
}
int MyGraphicsScene::height()
{
	return mSize.height();
}

void MyGraphicsScene::addLastItems(QList<StrItemData>& items)
{
	QList<StrItemData> it(items);
	for(int i = 0; i < it.size(); i++)
	{
		switch (it[i].strType)
		{
		case PictureShape::Line:
		{
			LineItem * p = new LineItem();
			p->setData(it[i]);
			p->init(it[i].strOffsetPoint);
			this->addItem(p);
		}
		break;
		case PictureShape::FreeLine:
		{
			FreeLineItem * p = new FreeLineItem();
			p->setData(it[i]);
			p->init(it[i].strOffsetPoint);
			this->addItem(p);
		}
		break;
		case PictureShape::Arrow:
		{
			ArrowItem * p = new ArrowItem();
			p->setData(it[i]);
			p->init(it[i].strOffsetPoint);
			this->addItem(p);
			p->setPos(it[i].strOffsetPoint);
		}
		break;
		case PictureShape::Txt:
		{
			RichTextItem * p = new RichTextItem();
			p->setData(it[i]);
			p->init(it[i].strOffsetPoint);
			this->addItem(p);
		}
		break;
		case PictureShape::Circular:
		{
			CircularItem * p = new CircularItem();
			p->setData(it[i]);
			p->init(it[i].strOffsetPoint);
			this->addItem(p);
		}
		break;
		case PictureShape::EllipseItem:
		{
			EllipseItem * p = new EllipseItem();
			p->setData(it[i]);
			p->init(it[i].strOffsetPoint);
			this->addItem(p);
		}
		break;
		case PictureShape::RoundRect:
		{
			RoundRectItem * p = new RoundRectItem();
			p->setData(it[i]);
			p->init(it[i].strOffsetPoint);
			this->addItem(p);
		}
		break;
		default:
			break;
		}
	}
}

QList<StrItemData>& MyGraphicsScene::saveData()
{
	mItems.clear();
	mbIsChanged = false;
	StrItemData strItemData;
	auto it = this->items();
	for (int i = 0; i < it.size(); i++)
	{
		strItemData.strType = PictureShape::None;
		PictureShape* p = dynamic_cast<PictureShape *>(it[i]);
		switch (p->getType())
		{
		case PictureShape::Txt:
		{
			RichTextItem* item = dynamic_cast<RichTextItem *>(it[i]);
			if (!item->isEmptyItem())
			{
				item->getData(strItemData);
				strItemData.strType = PictureShape::Txt;
			}
			else
			{
				removeItem(item);
			}
		}
			break;
		case PictureShape::FreeLine:
		{
			FreeLineItem* item = dynamic_cast<FreeLineItem *>(it[i]);
			item->getData(strItemData);
			strItemData.strType = PictureShape::FreeLine;
		}
			break;
		case PictureShape::Line:
		{
			LineItem* item = dynamic_cast<LineItem *>(it[i]);
			item->getData(strItemData);
			strItemData.strType = PictureShape::Line;
		}
			break;
		case PictureShape::Arrow:
		{
			ArrowItem* item = dynamic_cast<ArrowItem *>(it[i]);
			item->getData(strItemData);
			strItemData.strType = PictureShape::Arrow;
		}
			break;
		case PictureShape::Circular:
		{
			CircularItem* item = dynamic_cast<CircularItem *>(it[i]);
			item->getData(strItemData);
			strItemData.strType = PictureShape::Circular;
		}
			break;
		case PictureShape::EllipseItem:
		{
			EllipseItem* item = dynamic_cast<EllipseItem*>(it[i]);
			item->getData(strItemData);
			strItemData.strType = PictureShape::EllipseItem;
		}
			break;
		case PictureShape::RoundRect:
		{
			RoundRectItem* item = dynamic_cast<RoundRectItem*>(it[i]);
			item->getData(strItemData);
			strItemData.strType = PictureShape::RoundRect;
		}
			break;
		case PictureShape::Pixmap:
		{
			PixmapItem* item = dynamic_cast<PixmapItem *>(it[i]);
			item->getData();
			strItemData.strType = PictureShape::Pixmap;
		}
			break;
		default:
			break;
		}
		if (strItemData.strType != PictureShape::None)
		{
			strItemData.strOffsetPoint = it[i]->scenePos();
			mItems.push_back(strItemData);
		}
	}
	return mItems;
}


bool MyGraphicsScene::isChanged()
{
	return mbIsChanged;
}

void MyGraphicsScene::initImg(QString imagePath)
{
	QPixmap pic(imagePath);
	PixmapItem * Pixmap = new PixmapItem(pic);
	addItem(Pixmap);
	mSize = pic.size();
}

void MyGraphicsScene::cancelFoucs()
{
	//取消选中item
	if (mItemToRemove)
		mItemToRemove->setSelected(false);
	clearSelection();
}

void MyGraphicsScene::changedFoucs()
{
	QList<QGraphicsItem *> items = this->selectedItems();
	//mItemToRemove;
	if (items.size() != 0)
	{
	//	PictureShape* p = dynamic_cast<PictureShape*>(mItemToRemove);
		PictureShape* p = dynamic_cast<PictureShape*>(items[0]);
		if (p->getType() != PictureShape::Txt && p->getType() != PictureShape::Pixmap)
		{
			miNeedSetFocus = 0;
		}
	}
}