#include "CustomTableView.h"
#include <QDebug>
CustomTableView::CustomTableView(QWidget *parent)
: QTableView(parent)
{
	 /*************************Tableview自定义QSS样式设置************************************/
	if (customQTableStyle.size() > 0)
	{
	}
	else
	{
		this->setStyleSheet("QTableView,QTableWidget{selection-background-color:#44c767;background-color:white;border:1px solid;gridline-color:lightgray;");
	}
	/****************************设置CustomTableView中表格选中方式**************************/
    this->setEditTriggers(QAbstractItemView::SelectedClicked);
	this->setEditTriggers(QAbstractItemView::CurrentChanged);

	/*********创建自定义Delegate list列表，包括了QItemDelegate和QAbstractItemDelegate*******/
	QList<QItemDelegate *> *itemDeleList = new QList<QItemDelegate *>;
	QList<QAbstractItemDelegate *> *abstractDeleList = new QList<QAbstractItemDelegate *>;
	/***************************************************************************************/
}
CustomTableView::~CustomTableView()
{

}
void CustomTableView::setColumnItemDelegateList(QList<QItemDelegate *> *itemList)
{
	itemColumnDeleList = itemList;

	for (int i = 0; i < itemColumnDeleList->size(); i++)
	{
		this->setItemDelegateForColumn(i, itemColumnDeleList->at(i));
	}
}
void CustomTableView::setColumnAbstractItemDelegateList(QList<QAbstractItemDelegate *> *itemList)
{
	abstractItemColumnDeleList = itemList;
	for (int i = 0; i < abstractItemColumnDeleList->size(); i++)
	{
		this->setItemDelegateForColumn(i, abstractItemColumnDeleList->at(i));
	}
}
void CustomTableView::addColumnItemDelegate(QItemDelegate *item)
{
	itemColumnDeleList->append(item);
	this->setItemDelegateForColumn(itemColumnDeleList->size() - 1, item);
}
/*表格嵌套表格, */
void CustomTableView::addCelTable(int pRow,int pColumn)
{	
	 //this->setCellWidget(pRow,pColumn,)
    // QModelIndex test=this->currentIndex();
     
}
/*绑定数据源*/
void CustomTableView::bindingData(QAbstractItemModel &pModel)
{
	//QTableView *tableView = (QTableView *)this->parent();
	this->setModel(&pModel);
}
void CustomTableView::addColumnAbstractItemDelegate(QAbstractItemDelegate *item)
{   
	abstractItemColumnDeleList->append(item);
	this->setItemDelegateForColumn(abstractItemColumnDeleList->size() - 1, item);
}
void CustomTableView::setColumnItemDelegate(int index, QItemDelegate *item)
{
	if (itemColumnDeleList->size() > 0 && itemColumnDeleList->size() - 1 <= index && index > -1)
	{
		itemColumnDeleList->insert(index, item);
		this->setItemDelegateForColumn(index, item);
	}
}
void CustomTableView::setColumnAbstractItemDelegate(int index, QAbstractItemDelegate *item)
{
	if (abstractItemColumnDeleList->size() > 0 && abstractItemColumnDeleList->size() - 1 <= index && index > -1)
	{
		abstractItemColumnDeleList->insert(index, item);
		this->setItemDelegateForColumn(index, item);
	}
}

void CustomTableView::setCustomQTableSytle(QString qss)
{
	customQTableStyle = qss;
	this->setStyleSheet(customQTableStyle);
}

void CustomTableView::setCustomHorizHeaderStyle(QString qss)
{
	customHorizHeaderStyle = qss;
}

void CustomTableView::setCustomVerticalHeaderStyle(QString qss)
{
	customVerticalHeaderStyle = qss;
}

QString CustomTableView::getCustomQTableSytle()const
{
	return customQTableStyle;
}

QString CustomTableView::getCustomHorizHeaderStyle()const
{
	return customHorizHeaderStyle;
}

QString CustomTableView::getCustomVerticalHeaderStyle()const
{
	return customVerticalHeaderStyle;
}


