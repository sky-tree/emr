#include "treewidget.h"
#include"treeitemmimedata.h"
 
treeWidget::treeWidget(QWidget *parent) :QTreeWidget(parent)
{
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	this->setAcceptDrops(false);
	this->setDragEnabled(true);
	this->setStyleSheet("QTreeWidget::item:selected{ background-color: rgb(150, 0, 0)}");
}
void treeWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_startDragPoint = event->pos();
	}
	QTreeWidget::mousePressEvent(event);
}
void treeWidget::mouseMoveEvent(QMouseEvent *event)
{
	/*if (!(event->buttons() & Qt::LeftButton))
		return;*/
	if (event->buttons()&Qt::RightButton)
	{
		return;
	}
	if (event->buttons() &Qt::LeftButton)
	{
		QTreeWidgetItem *item = currentItem();
		if (item == NULL)return;
		if (item->text(0) == "患者基本信息" || item->text(0) == "基本元素" || item->text(0) == "自定义")
			return;
		int dragDistance = (event->pos() - m_startDragPoint).manhattanLength();
		if (dragDistance>QApplication::startDragDistance())
		{
			performDrag();
		}
	}
	QTreeWidget::mouseMoveEvent(event);
}
void treeWidget::mouseReleaseEvent(QMouseEvent *event)
{
	QTreeWidget::mouseReleaseEvent(event);
}
//void treeWidget::dragMoveEvent(QDragMoveEvent *event)
//{
//	//QWidget *source = qobject_cast<treeWidget *>(event->source());
//	//if (source /*&& source!=this*/)
//	//{
//	//	const TreeItemMimeData *pMimeData = (const TreeItemMimeData *)(event->mimeData());
//	//	const QTreeWidgetItem *item = pMimeData->DragItemData();
//	//	QTreeWidgetItem *currentItem = this->itemAt(event->pos());
//	//	if (currentItem == item)
//	//	{
//	//if (userModuleRoot->parent() == NULL&&userModuleRoot->childCount() == 0)
//	//		event->ignore();
//	//	}
//	//	else
//	//	{
//	//		setCurrentItem(currentItem);
//	//		event->setDropAction(Qt::MoveAction);
//	//		event->accept();
//	//	}
//
//	//}
//}
void treeWidget::performDrag()
{
	QTreeWidgetItem *item = currentItem();

	if (item)
	{
		TreeItemMimeData *mimeData = new TreeItemMimeData;
		mimeData->SetDragData("ItemMimeData", item);
		if (item->parent() == NULL || item->childCount() != 0)
		{

			mimeData->SetDragData("ItemMimeData", NULL);
		}
		QDrag *drag = new QDrag(this);
		drag->setMimeData(mimeData);
        //drag->setPixmap(QPixmap(":C:/Users/Administrator/Desktop/QQͼƬ20151102100357.jpg"));
		drag->exec(Qt::MoveAction);
		//delete item;
	}
}

