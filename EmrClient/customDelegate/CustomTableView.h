/*********************************************************************************
*Copyright(C),Shine
*FileName:  customComboBoxDelegate
*Author:  liuyi
*Modifier:caokewen
*Version:  1.0
*Date:  //2016/2/24
*Description:  //此类为一个公共的TableView类，为其他模块添加TableView提供方便。
**********************************************************************************/
#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H
 
#include <QTableView>
#include <QItemDelegate>
#include <QAbstractItemDelegate>
#include <QList>
#include <qtablewidget.h>
/********qt版本高于等于qt5，VS版本高于或等于VS2010 ******/
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
/***************************自定义表格控件继承自Qt系统的QTableView控件***********************************/
class CustomTableView : public QTableView
{
	Q_OBJECT
public:
	explicit CustomTableView(QWidget *parent = 0);
	~CustomTableView();

	/*添加自定义Delegate 继承自QItemDelegate*/
	void addColumnItemDelegate(QItemDelegate *item);
	/*添加自定义Delegate 继承自QItemDelegate*/
	void addColumnAbstractItemDelegate(QAbstractItemDelegate *item);                                                  
	/*设置自定义Delegate List 继承自QItemDelegate*/
	void setColumnItemDelegateList(QList<QItemDelegate *> *itemList);
	/*设置自定义AbstractDelegate List 继承自QItemDelegate*/
	void setColumnAbstractItemDelegateList(QList<QAbstractItemDelegate *> *itemList);
	/*设置List中的某个自定义ItemDelegateItem*/
	void setColumnItemDelegate(int index, QItemDelegate *item);
	/*设置List中的某个自定义AbstractDelegateItem*/
	void setColumnAbstractItemDelegate(int index, QAbstractItemDelegate *item);
	/*获取私有自定义QItemDelegate list*/
	QList<QItemDelegate *> getItemDeleList()const;
	/*获取私有自定义QAbstractItemDelegate list*/
	QList<QAbstractItemDelegate *> getAbstractItemDeleList()const;

	/*设置自定义TableView表格样式*/
	void setCustomQTableSytle(QString qss);
	/*设置自定义TableView水平表头样式*/
	void setCustomHorizHeaderStyle(QString qss);
	/*设置自定义TableView垂直表头样式*/
	void setCustomVerticalHeaderStyle(QString qss);
	/*获取自定义TableView表格样式*/
	QString getCustomQTableSytle()const;
	/*获取自定义TableView水平表头样式*/
	QString getCustomHorizHeaderStyle()const;
	/*获取自定义TableView垂直表头样式*/
	QString getCustomVerticalHeaderStyle()const;
	/*表格嵌套表格*/
	void addCelTable(int pRow, int pColumn);
	/*绑定数据*/
	void bindingData(QAbstractItemModel &pModel);
private:
	/*私有自定义TableView表格样式变量*/
	QString customQTableStyle;
	/*私有自定义TableView水平表头样式变量*/
	QString customHorizHeaderStyle;
	/*私有自定义TableView垂直表头样式变量*/
	QString customVerticalHeaderStyle;
	/*私有自定义QItemDelegate list*/
	QList<QItemDelegate *> *itemColumnDeleList;
	/*私有自定义QAbstractItemDelegate list*/
	QList<QAbstractItemDelegate *> *abstractItemColumnDeleList;
	QTableView *m_tableView;
};

#endif // CUSTOMTABLEVIEW_H
