#include "commonWidgets/TabWidgetCollection.h"
#include "views/NavigationView.h"
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QIcon>
#include <QDebug>
#include<qscrollarea.h>
#include<QTreeWidgetItem>
#include "MyAppEntrance.h"
#include "UIInputManager.h"
#include "SystemParams.h"
#include"BaseConstants.h"
#include "AuthorityConstants.h"
#include"Cursor.h"
#include "MessageBox.h"

/*Q_DECLARE_METATYPE(QTreeWidgetItem*)	*///2017.8.3zl
TabWidgetCollection *TabWidgetCollection::pInstance = NULL;
TabWidgetCollection::TabWidgetCollection(QWidget *parent) : QGroupBox(parent), pViewContainer(NULL), m_isPopMessagebox(true), m_isClickCancel(false)
{

	QString tabStyle = "QWidget{background-color:rgb(255,255,255);}QTabBar::close-button{image:url(:/res/Res/MenuIcon/pinclose.png);}QTabBar::close-button:hover{background-color:rgb(194,224,255);border:1px solid rgb(51,153,255);}QTabBar::Tab{background:rgb(227,236,241);min-height:30px;min-width:80px;border: 1px solid rgb(137,137,137);}QTabBar::tab:selected{background-color:rgb(190,190,190);}";

	tabWidget = new QTabWidget();
	//tabWidget->setStyleSheet(style1);
	tabWidget->setStyleSheet(tabStyle);
	map = new QMap<int, bool>();
	//设置tabWidget中每个Tab上的关闭按钮
	tabWidget->setTabsClosable(true);
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(tabWidget);
	layout->setMargin(0);
	layout->setSpacing(0);

	setLayout(layout);


	//创建TabWidgets关闭事件
	connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(removeSubTab(int)));
	//connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addTabWidgetSlot()));
	connect(tabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(IsChangeed(int)));

}
void TabWidgetCollection::IsChangeed(int index)
{
	//int currentIndex = tabWidget->currentIndex();
	/*if (index!=currentIndex&&isTemplate==0)*/
	{
		QString id_ = tabWidget->tabWhatsThis(index);
		if (pViewContainer)	pViewContainer->setPrompts(id_);
		tabWidget->setCurrentIndex(index);
		QWidget *widget = getCurrentWidget();
		if (widget == NULL)return;
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		UIInputManager * mgr = appEntrance->getUIInputMgr();
		//SystemParams * sysParams = mgr->getSystemParams();
		Cursor::initCursor(appEntrance);

		bool isTidy = mgr->isShowTidy();
//		pViewContainer->m_pLayoutAct->setChecked(isTidy);
		bool isTitleView = mgr->getTitleView();
		//char TitleView = sysParams->getTitleViewType();
		if (!isTitleView)
		{
			pViewContainer->m_pHideOrDisplayHeaderAct->setChecked(false);
			pViewContainer->m_pHideOrDisplayTopPageNumber->setChecked(false);
		}
		else
		{
			pViewContainer->m_pHideOrDisplayHeaderAct->setChecked(true);
			//pViewContainer->hideOrDisplayTitlePageAct->setChecked(true);
			bool state = mgr->getParagraphNumView();
			pViewContainer->m_pHideOrDisplayTopPageNumber->setChecked(state);
		}
		bool isBottomView = mgr->getBottomView();
		if (!isBottomView)
		{
			pViewContainer->m_pHideOrDisplayFooterAct->setChecked(false);
			pViewContainer->m_pHideOrDisplayBottomPageNumber->setChecked(false);
		}
		else
		{
			pViewContainer->m_pHideOrDisplayFooterAct->setChecked(true);
			char state = mgr->getBottomNumViewType();
			pViewContainer->m_pHideOrDisplayBottomPageNumber->setChecked(state);
		}
		pViewContainer->m_pLineNumAct->setChecked(mgr->getParagraphNumView());
	}
}
TabWidgetCollection *TabWidgetCollection::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new TabWidgetCollection();
	}
	return pInstance;
}

bool TabWidgetCollection::removeAllTab()
{
	//for (int i = 0; tabWidget->count()>0; i)
	//{
	//	/*QMessageBox::StandardButton rb = QMessageBox::question(NULL, "保存", "是否保存?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	//	if (rb == QMessageBox::Yes)
	//	{
	//

	//	}*/
	//	tabWidget->removeTab(i);
	//	int test = tabWidget->count();
	//	int a = 1;
	//}
	while (tabWidget->count() > 0)
	{
		QWidget *widget = tabWidget->widget(0);
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		pViewContainer->deleteEMR(appEntrance);
		tabWidget->removeTab(0);//黄洪 2.16.5.28
	}
	if (tabWidget->count() == 0)
	{
		if (pViewContainer != NULL)
		{
			pViewContainer->setMenuEnable(false);
		}
	}
	map->clear();
	return true;
}
QTreeWidgetItem* TabWidgetCollection::saveData(bool isFromRightMenu)
{
	pViewContainer->m_bIsClickCanccel = false;
	int testCount = tabWidget->count();
	for (int i = 0; i < tabWidget->count(); i++)
	{
		QString id = tabWidget->tabWhatsThis(i);
		QTreeWidgetItem *item = getTreeItem(id);
		if (item != NULL)
		{
			QVariant var = item->data(0, Qt::UserRole);
			QStringList list = var.toStringList();
			QString  isModify = list[2];
			QString  isModifySave = list[2];
			if (isModify != "2")
			{
// 				tabWidget->setCurrentIndex(i);
// 				QString text = item->text(0);
// 				QString tips = text + "未保存，是否保存?";
// 				QMessageBox::StandardButton rb = QMessageBox::question(NULL, "保存", tips, QMessageBox::Yes | QMessageBox::No |// QMessageBox::Cancel, QMessageBox::Yes);
				int query = MessageBox::showQueryMessageBox("保存", "当前数据未保存，是否保存?");
				if (query == QMessageBox::Yes)
				{
					QString ItemText = item->text(0);
					ItemText = ItemText.replace("*", "");
					item->setText(0, ItemText);
					bool isTemplate = map->value(i);
					isModify = "2";

					QStringList list1;
					if (list.length() == 4)
					{
						list1 << list[0] << list[1] << isModify << list[3];
						var.clear();
						var.setValue(list1);
					}
					else
					{
						list.removeAt(2);
						list << isModify;
						var.clear();
						var.setValue(list);
					}

					if (isTemplate == true)
					{
						pViewContainer->saveTemplateData(item, isModifySave, isFromRightMenu);
					}
					else
					{
						pViewContainer->saveMedicalData(item, isModifySave, isFromRightMenu);
					}
					QString tabText = tabWidget->tabText(i);
					tabText = tabText.replace("*", "");
					tabWidget->setTabText(i, tabText);
				}
				else if (query == QMessageBox::No)
				{
					if (isModify == "0")//如果是新增的，不保存的话，应当将对应的结点删除掉
					{
						return item;
					}
					else
						if (isModify == "1")
						{
						QString ItemText = item->text(0);
						ItemText = ItemText.replace("*", "");
						item->setText(0, ItemText);
						QString tabText = tabWidget->tabText(i);
						tabText = tabText.replace("*", "");
						tabWidget->setTabText(i, tabText);

						QWidget *widget = getCurrentWidget();
						if (widget == NULL)return NULL;
						MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
// 						QByteArray array;
// 						array.resize(0);
// 						appEntrance->init(&array);
						pViewContainer->showSelectMainMenuItem(item, 0);
//						QTreeWidgetItem *pItem = item->parent();

						//不保存，将当前页的状态从修改状态 更新为未有任何改动
						isModify = "2";
						QStringList list1;
						if (list.length() == 4)
						{
							list1 << list[0] << list[1] << isModify << list[3];
							var.clear();
							var.setValue(list1);
							//appEntrance->closeAllLevelOneTempleteWriteAutority();
						}
						else
						{
							list.removeAt(2);
							list << isModify;
							var.clear();
							var.setValue(list);
							//appEntrance->setFileAuthority(AuthorityConstants::Authority::ONLY_READ);
						}
						//pItem->setSelected(true);
						item->setData(0, Qt::UserRole, var);
						QWidget *widgetRemove = tabWidget->widget(i);
						MyAppEntrance *appEntranceRemove = static_cast<MyAppEntrance*>(widgetRemove);
						pViewContainer->deleteEMR(appEntranceRemove);
						tabWidget->removeTab(i); //黄洪 2.16.5.28
						}
				}
				else
				{
					pViewContainer->m_bIsClickCanccel = true;
				}
			}
		}
	}
	return NULL;
}
void TabWidgetCollection::saveTabData(int index)
{
	tabWidget->setCurrentIndex(index);
	QString id = tabWidget->tabWhatsThis(index);
	QTreeWidgetItem *item = getTreeItem(id);

	if (item != NULL)
	{
		QVariant var = item->data(0, Qt::UserRole);
		QStringList list = var.toStringList();
		QString  isModify = list[2];
		QString  isModifySave = list[2];
		if (isModify != "2")
		{
			QString text = item->text(0);
			int query = MessageBox::showQueryMessageBox("保存","当前数据未保存，是否保存?");
			if (query == QMessageBox::Yes)
			{
				QString ItemText = item->text(0);
				ItemText = ItemText.replace("*", "");
				item->setText(0, ItemText);
				QString tabText = tabWidget->tabText(index);
				tabText = tabText.replace("*", "");
				tabWidget->setTabText(index, tabText);
				bool isTemplate = map->value(index);
				isModify = "2";
				//保存，将当前页的状态从修改状态 更新为未有任何改动
				QStringList list1;
				if (list.length() == 4)
				{
					list1 << list[0] << list[1] << isModify << list[3];
					var.clear();
					var.setValue(list1);
				}
				else
				{
					list.removeAt(2);
					list << isModify;
					var.clear();
					var.setValue(list);
				}
				if (isTemplate == true)
				{
					pViewContainer->saveTemplateData(item, isModifySave);

				}
				else
				{
					pViewContainer->saveMedicalData(item, isModifySave);
				}
			}
			else if (query == QMessageBox::No)
			{

				if (isModify == "0")//如果是新增的，不保存的话，应当将对应的结点删除掉
				{
					delete item;
					item = NULL;
				}
				else
					if (isModify == "1")
					{
					QString ItemText = item->text(0);
					ItemText = ItemText.replace("*", "");
					item->setText(0, ItemText);
					//不保存，将当前页的状态从修改状态 更新为未有任何改动
					QStringList list1;
					if (list.length() == 4)
					{
						isModify = "2";//2017/4/28,病程记录保存时点否的情况
						list1 << list[0] << list[1] << isModify << list[3];
						var.clear();
						var.setValue(list1);
					}
					else
					{
						isModify = "2";
						list.removeAt(2);
						list << isModify;
						var.clear();
						var.setValue(list);
					}
					item->setData(0, Qt::UserRole, var);

					//pItem->setSelected(true);
					}
			}
			else
			{
				m_isClickCancel = true;
				return;
			}
		}
	}
}
bool TabWidgetCollection::removeSubTab(int index)
{
	if (index == -1)
	{
		m_isPopMessagebox = true;
		return true;
	}
	//关闭前判断是否需要保存
	if (m_isPopMessagebox == true)

	{
		saveTabData(index);
	}
	m_isPopMessagebox = true;
	if (m_isClickCancel == true)
	{
		m_isClickCancel = false;
		return false;
	}
	if (index >= 0)
	{
		QWidget *widget = tabWidget->widget(index);
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		//appEntrance->timerPause();
		pViewContainer->deleteEMR(appEntrance);
		tabWidget->removeTab(index);//黄洪 2.16.5.28
		//	int count = tabWidget->count();
		//tabWidget->removeTab(index);
	}
	if (tabWidget->count() < 1)
	{
		if (pViewContainer != NULL)
		{
			pViewContainer->setMenuEnable(false);
			pViewContainer->setMenuStatus(false);
		}
	}
	int index_ = getCurrentIndex();
	QString id_ = tabWidget->tabWhatsThis(index_);
	if (pViewContainer)	pViewContainer->setPrompts(id_);
	return true;
	//MainViewContainer::getInstance().setMenuEnable(false);
}
void TabWidgetCollection::saveTabName(QString str)
{
	for (int i = 0; i < tabWidget->count(); i++)
	{
		if (str == tabWidget->tabText(i) && str.contains("*"))
		{
			QString ModifyName = str.replace("*", "");
			tabWidget->setTabText(i, ModifyName);
			tabWidget->setCurrentIndex(i);
			break;
		}
	}
}
void TabWidgetCollection::modifyTabName(int index, QString str)
{
	tabWidget->setTabText(index, str);
}
void TabWidgetCollection::modifyTabName(QString str)
{
	for (int i = 0; i < tabWidget->count(); i++)
	{
		if (str == tabWidget->tabText(i))
		{

			QString ModifyName = str.replace("*", "");
			ModifyName = ModifyName + "*";
			tabWidget->setTabText(tabWidget->currentIndex(), ModifyName);
			//tabWidget->setCurrentIndex(i);
		}
	}
}
void TabWidgetCollection::modifyMedicalName(QString str)
{
	tabWidget->setTabText(tabWidget->currentIndex(), str);
}
void TabWidgetCollection::addTabWidgetSlot()
{
	QLabel *label2 = new QLabel("addTabWidgetSlot");
	tabWidget->addTab(label2, "addTabWidgetSlot");
	tabWidget->setCurrentIndex(tabWidget->count() - 1);
}
int TabWidgetCollection::getCurrentIndex()
{
	int index = 0;
	index = tabWidget->currentIndex();
	return index;
}
bool TabWidgetCollection::isExist(QString str)
{
	for (int i = 0; i < tabWidget->count(); i++)
	{
		QString id = tabWidget->tabWhatsThis(i);//ID，病程记录为specialID
		if (str == id)
		{
			tabWidget->setCurrentIndex(i);
			//tabWidget->setCurrentIndex(i);
			tabWidget->currentWidget()->setFocus();
			return true;
			break;
		}
	}
	return false;
}
int  TabWidgetCollection::getIndex(QString id)
{
	for (int i = 0; i < tabWidget->count(); i++)
	{
		QString strId = tabWidget->tabWhatsThis(i);
		if (strId == id)
		{
			return i;
		}
	}
	return -1;
}
QWidget* TabWidgetCollection::getWidget(QString id)
{
	int count = tabWidget->count();
	for (int i = 0; i < tabWidget->count(); i++)
	{
		QString strId = tabWidget->tabWhatsThis(i);
		if (strId == id)
		{
			QWidget* widget = NULL;
			widget = tabWidget->widget(i);
			return widget;
		}
	}
	return NULL;
}
void TabWidgetCollection::modifTabItem(QTreeWidgetItem *pItem, QString id)
{
	for (int i = 0; i < tabWidget->count(); i++)
	{
		QString m_id = tabWidget->tabWhatsThis(i);
		QString text = pItem->text(0);
		if (m_id == id)
		{
			QVariant varItem = pItem->data(0, Qt::UserRole);
			QStringList list = varItem.toStringList();
			if (list.length() == 4)
			{
				QByteArray byteId = m_id.toLatin1();
				QVariant var;
				var.setValue(pItem);
				tabWidget->setProperty(byteId, var);
				tabWidget->setTabText(i, text);
			}
		}
	}
}
void TabWidgetCollection::addTabWidget(QWidget *qWidget, QString str, bool index, QString id, QTreeWidgetItem *pItem)
{
	bool isAdd = true;

	for (int i = 0; i < tabWidget->count(); i++)
	{
		QString m_id = tabWidget->tabWhatsThis(i);
		if (m_id == id)
		{
			tabWidget->setCurrentIndex(i);
			tabWidget->currentWidget()->setFocus();
			isAdd = false;
			//病程记录特殊处理
			QVariant varItem = pItem->data(0, Qt::UserRole);
			QStringList list = varItem.toStringList();
			if (list.length() == 4)
			{
				QByteArray byteId = m_id.toLatin1();
				QVariant var;
				var.setValue(pItem);
				tabWidget->setProperty(byteId, var);
			}

			QWidget *widget = getCurrentWidget();
			if (widget == NULL)return;
			MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
			UIInputManager * mgr = appEntrance->getUIInputMgr();
			bool isTidy = mgr->isShowTidy();
			//pViewContainer->m_pLayoutAct->setChecked(isTidy);
			bool isTitleView = mgr->getTitleView();
			if (!isTitleView)
			{
				pViewContainer->m_pHideOrDisplayHeaderAct->setChecked(false);
				pViewContainer->m_pHideOrDisplayTopPageNumber->setChecked(false);
			}
			else
			{
				pViewContainer->m_pHideOrDisplayHeaderAct->setChecked(true);
				char a = mgr->getTitleNumViewType();
				pViewContainer->m_pHideOrDisplayTopPageNumber->setChecked(a);
			}
			bool isBottomView = mgr->getBottomView();
			if (!isBottomView)
			{
				pViewContainer->m_pHideOrDisplayFooterAct->setChecked(false);
				pViewContainer->m_pHideOrDisplayBottomPageNumber->setChecked(false);
			}
			else
			{
				pViewContainer->m_pHideOrDisplayFooterAct->setChecked(true);
				pViewContainer->m_pHideOrDisplayBottomPageNumber->setChecked(mgr->getBottomNumViewType());
			}
			pViewContainer->m_pLineNumAct->setChecked(mgr->getParagraphNumView());
			break;
		}
	}
	if (isAdd)
	{
		if (qWidget == NULL)return;
		tabWidget->addTab(qWidget, str);
		tabWidget->setCurrentIndex(tabWidget->count() - 1);
		tabWidget->setTabWhatsThis(tabWidget->currentIndex(), id);
		QByteArray byteId = id.toLatin1();
		QVariant var;
		var.setValue(pItem);
		tabWidget->setProperty(byteId, var);

		map->insert(tabWidget->count() - 1, index);
		tabWidget->currentWidget()->setFocus();
		QWidget *widget = getCurrentWidget();
		if (widget == NULL)return;
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		UIInputManager * mgr = appEntrance->getUIInputMgr();
		//QSize size1 = pViewContainer->size();
		//bool isTidy = mgr->isShowTidy();
		//pViewContainer->m_pLayoutAct->setChecked(isTidy);
		bool isTitleView = mgr->getTitleView();
		if (!isTitleView)
		{
			pViewContainer->m_pHideOrDisplayHeaderAct->setChecked(false);
			pViewContainer->m_pHideOrDisplayTopPageNumber->setChecked(false);
		}
		else
		{
			pViewContainer->m_pHideOrDisplayHeaderAct->setChecked(true);
			pViewContainer->m_pHideOrDisplayTopPageNumber->setChecked(mgr->getTitleNumViewType());
		}
		bool isBottomView = mgr->getBottomView();
		if (!isBottomView)
		{
			pViewContainer->m_pHideOrDisplayFooterAct->setChecked(false);
			pViewContainer->m_pHideOrDisplayBottomPageNumber->setChecked(false);
		}
		else
		{
			pViewContainer->m_pHideOrDisplayFooterAct->setChecked(true);
			pViewContainer->m_pHideOrDisplayBottomPageNumber->setChecked(mgr->getBottomNumViewType());
		}
		pViewContainer->m_pLineNumAct->setChecked(mgr->getParagraphNumView());
	}
	if (tabWidget->count() > 0)
	{
		if (pViewContainer != NULL)
		{
			pViewContainer->setMenuEnable(true);
		}
	}
}
QTreeWidgetItem* TabWidgetCollection::getCurrentWidgetTreeItem(QString id)
{
	if (id == "")return NULL;
	QTreeWidgetItem* item = NULL;
	QByteArray byteId = id.toLatin1();
	QVariant var1 = tabWidget->property(byteId);
	if (var1.canConvert<QTreeWidgetItem *>())
	{
		item = var1.value<QTreeWidgetItem *>();
	}
	return item;
}
//关闭所有Tab时，检查未保存的item
QTreeWidgetItem* TabWidgetCollection::getTreeItem(QString id)
{
	QTreeWidgetItem* item = NULL;
	QTreeWidgetItem* medicalItem = NULL;//病程记录

	if (id == "")return NULL;
	QByteArray byteId = id.toLatin1();
	QVariant var1 = tabWidget->property(byteId);//取得Tab对应Item携带的item
	if (var1.canConvert<QTreeWidgetItem *>())
	{
		item = var1.value<QTreeWidgetItem *>();
	}
	if (item != NULL)
	{
		QVariant var = item->data(0, Qt::UserRole);//item 的data classifycode SERIAL_NUMBER （specialid）
		QStringList list = var.toStringList();
		if (list.count() == 4)//病程记录下
		{
			medicalItem = pViewContainer->GetMedicalUnSaveNode(item);
			if (medicalItem != NULL)
			{
				return medicalItem;
			}
		}
	}
	return item;
}
TabWidgetCollection::~TabWidgetCollection()
{
	//delete tabWidget;
}

void TabWidgetCollection::setUnSaveTabtoCurrent()
{
	QString str = "";
	for (int i = 0; i < tabWidget->count(); i++)
	{
		str = tabWidget->tabText(i);
		if (str.contains("*"))
		{
			tabWidget->setCurrentIndex(i);
		}
	}
}

QString TabWidgetCollection::getTabTxt() const
{
	int index = tabWidget->currentIndex();
	return tabWidget->tabText(index);
}

QWidget* TabWidgetCollection::getUnsavedWidget()
{
	bool bFindUnSaveFile = false;
	int index_ = 0;
	QString tabName_;
	do 
	{
		tabName_ = tabWidget->tabText(index_);
		if (tabName_.indexOf("*") != -1)
		{
			bFindUnSaveFile = true;
			break;
		}
		index_++;
	} while (tabName_.size() > 1);

	if (bFindUnSaveFile == true)
	{
		return tabWidget->widget(index_);
	}
	else
	{
		return NULL;
	}
}

void TabWidgetCollection::modifyAllName()
{
	int testCount = tabWidget->count();
	for (int i = 0; i < tabWidget->count(); i++)
	{
		QString id = tabWidget->tabWhatsThis(i);
		QTreeWidgetItem *item = getTreeItem(id);
		if (item != NULL)
		{
			QString ItemText = item->text(0);
			if (0 < ItemText.indexOf("*"))
			{
				break;
			}
			ItemText = ItemText.replace("*", "");
			item->setText(0, ItemText);
			QString tabText = tabWidget->tabText(i);
			tabText = tabText.replace("*", "");
			tabWidget->setTabText(i, tabText);
		}
	}
}
