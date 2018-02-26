#ifndef TABWIDGETCOLLECTION_H
#define TABWIDGETCOLLECTION_H

#include <QGroupBox>
 
class QDialog;
class QTabWidget;
class QPushButton;
#include"qmessagebox.h"
#include "qmap.h"
#include"views/MainViewContainer.h"
#include "toolbarclass.h"
// 代码一定要是: UTF-8(BOM)      
//qt版本高于等于qt5，VS版本高于或等于VS2010    
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
//Q_DECLARE_METATYPE(QTreeWidgetItem*)
class TabWidgetCollection : public QGroupBox
{
	Q_OBJECT
public:
	explicit TabWidgetCollection(QWidget *parent = 0);
	~TabWidgetCollection();
	static TabWidgetCollection *getInstance();
	void destory()
	{
		if (tabWidget)
		{
			delete tabWidget;
			tabWidget = NULL;
		}
		if (map)
		{
			delete map;
			map = NULL;
		}
	}
signals:
	//  void tabCloseRequest(int index);
	public slots :
	bool removeSubTab(int index);
	void addTabWidgetSlot();
	QTreeWidgetItem* saveData(bool isFromRightMenu);
	void IsChangeed(int index);
public:
	void addTabWidget(QWidget *qWidget, QString str, bool index,QString id,QTreeWidgetItem *pItem=NULL);
	void modifyTabName(QString str);
	void modifyTabName(int index, QString str);
	QWidget* getWidget(QString id);
	//病程记录名称的更改
	void modifyMedicalName(QString str);
	void saveTabName(QString str);
	void saveTabData(int index);
	void modifTabItem(QTreeWidgetItem *pItem, QString id);
	QString getTabTxt() const;
	void setUnSaveTabtoCurrent();
	void setIsPopMessageBox(bool isFlags)
	{
		m_isPopMessagebox = isFlags;
	}
	void setViewContainer(MainViewContainer *pObject)
	{
		pViewContainer = pObject;
	}
	int  getTabCount()
	{
		return tabWidget->count();
	}
	QWidget* getCurrentWidget()
	{
       	return tabWidget->currentWidget();
	}
	//根据ID获取标签页
	QString getCurrentWidgetId()
	{
		QString id = "";
		if (tabWidget->count() <1)
		{
			return id;
		}
		int index_ = tabWidget->currentIndex();
		id = tabWidget->tabWhatsThis(index_);
		return id;
	}
	MainViewContainer* getContainer()
	{
		return pViewContainer;
	}
	int  getIndex(QString id);
	int  getCurrentIndex();
	bool removeAllTab();
	bool isExist(QString str);
	QTreeWidgetItem* getTreeItem(QString id);
	QTreeWidgetItem* getCurrentWidgetTreeItem(QString id);
	QWidget* getUnsavedWidget();
	void modifyAllName();

protected:
	QTabWidget *tabWidget;
	QPushButton *addButton;
	//NavigationView * navigationView;
	QMap<int, bool> *map;
	static TabWidgetCollection *pInstance;
	MainViewContainer *pViewContainer;
	bool m_isPopMessagebox;
	bool m_isClickCancel;
};

#endif // TABWIDGETCOLLECTION_H
