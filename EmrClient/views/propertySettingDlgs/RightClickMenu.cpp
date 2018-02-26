#include "RightClickMenu.h"
#include "commonWidgets/TabWidgetCollection.h"
#include"MyAppEntrance.h"
#include"UIInputManager.h"
#include "UIConstants.h"
#include "BaseConstants.h"
#include "TempleteTypeEnumeration.h"
#include "inspection.h"
#include"dataModel\OperateData\DataBaseControl.h"
#include "MessageBox.h"
RightClickMenu::RightClickMenu(QWidget *parent) :QMenu(parent), m_menu(NULL), m_listActions(NULL)
, m_Isattribute(false), m_map(NULL),m_bIsCrusRcd(false), m_bIsEdit(false)
{
	Init();
	m_tidyView = new QAction("整洁显示", this);
	connect(m_tidyView, SIGNAL(triggered(bool)), this, SLOT(tidyView(bool)));
	m_tidyView->setCheckable(true);
}

#define DELETEPOINT(m_Point)\
if (m_Point != NULL){ delete m_Point; m_Point = NULL; }
RightClickMenu::~RightClickMenu()
{
	m_listActions->clear();
	DELETEPOINT(m_menu)DELETEPOINT(m_listActions)DELETEPOINT(m_insertFile)DELETEPOINT(m_fixedText)
	DELETEPOINT(m_keyWord)DELETEPOINT(m_DocumentLine)
	DELETEPOINT(m_headerAndFooter)DELETEPOINT(m_medicalFormula)DELETEPOINT(m_specialFormula)
	DELETEPOINT(m_radio)DELETEPOINT(m_multiSelect)DELETEPOINT(m_IsSelect)
	DELETEPOINT(m_writeTips)DELETEPOINT(m_image)DELETEPOINT(m_attribute)
	DELETEPOINT(m_Line)DELETEPOINT(m_pageBreak)DELETEPOINT(m_timerAndDate)DELETEPOINT(m_deleteElement)
}
void RightClickMenu::IsShowTidy()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return ;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	if (appEntrance!=NULL)
	{
		UIInputManager * mgr = appEntrance->getUIInputMgr();
		bool isTidy = mgr->isShowTidy();
		m_tidyView->setChecked(isTidy);
	}
}
#define CREATIONACTION(objectName,text,slot)\
	objectName = new QAction(tr(text), this); \
	connect(objectName, SIGNAL(triggered(bool)), this, SLOT(slot())); \
	m_menu->addAction(objectName);\
	m_listActions->push_back(objectName);
void RightClickMenu::Init()
{
	 //m_insertFile = new QAction("插入文件",this);
	    m_menu = new QMenu;
		m_listActions = new QList<QAction *>();
		CREATIONACTION(m_insertFile, "插入文件", insertFile)
			CREATIONACTION(m_fixedText, "固定文本", fixedText)
			CREATIONACTION(m_keyWord, "关键词", keyWord)
			CREATIONACTION(m_DocumentLine, "文档提纲", DocumentLine)
			CREATIONACTION(m_headerAndFooter, "页眉页脚", headerAndFooter)
			CREATIONACTION(m_medicalFormula, "医学公式", medicalFormula)
			CREATIONACTION(m_specialFormula, "特殊符号", specialFormula)
			CREATIONACTION(m_radio, "单选", radio)
			CREATIONACTION(m_multiSelect, "多选", multiSelect)
			CREATIONACTION(m_IsSelect, "有无选择", IsSelect)
			CREATIONACTION(m_writeTips, "录入提示", writeTips)
			CREATIONACTION(m_image, "图像", image)
			CREATIONACTION(m_Line, "横线", Line)
			CREATIONACTION(m_pageBreak, "分页符", pageBreak)
			CREATIONACTION(m_pageNumber, "页号", pageNumber)
			CREATIONACTION(m_timerAndDate, "时间日期", timerAndDate)
			CREATIONACTION(m_table, "表格", InsertTable)
			CREATIONACTION(m_deleteElement, "删除元素", deleteElement)

			CREATIONACTION(m_attribute, "属性", attribute)

			//病历书写的右键功能
			CREATIONACTION(m_saveEdit, "保存本次修改", saveEdit)
			CREATIONACTION(m_giveUpEdit, "放弃本次修改", giveUpEdit)
			CREATIONACTION(m_copy, "复制", copy)
			CREATIONACTION(m_paste, "粘贴", paste)
			CREATIONACTION(m_checkResult, "导入检查结果", checkResult)
			CREATIONACTION(m_currentTime, "插入系统时间", setCurrentTime)
			CREATIONACTION(m_modify, "修改", modifyTab)
		/*	m_modify = new QAction(tr("编辑"), this);
		   connect(m_modify, SIGNAL(triggered(bool)), this, SLOT(modifyTab()));
			m_menu->addAction(m_modify); 
			m_listActions->push_back(m_modify);*/


}
void RightClickMenu::setCurrentTime()
{
	/*MainViewContainer *container = TabWidgetCollection::getInstance()->getContainer();
	QTreeWidgetItem *item = NULL;
	QString id = TabWidgetCollection::getInstance()->getCurrentWidgetId();
	item = TabWidgetCollection::getInstance()->getTreeItem(id);*/
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	mgr->addCurrentTime();
}
void RightClickMenu::InsertTable()
{

}
void RightClickMenu::addActions(bool isControlClick)
{
	if (m_menu == NULL)
	{
		return;
	}
	m_menu->clear();

	//if (m_listActions != NULL&&isControlClick==false)
	//{
	//	/*m_menu->addActions(*m_listActions);
	//	m_menu->removeAction(m_deleteElement);
	//	m_menu->removeAction(m_attribute);*/
	//	
	//}
	IsShowTidy();
	if (isControlClick == true)
	{   
		m_listActions->clear();
		m_menu->addAction(m_deleteElement);
		m_menu->addAction(m_attribute);
	}
	else
	{
		addMedicalActions();
	}
}
//新增病历右键的权限
void RightClickMenu::addMedicalActions()
{
	m_menu->clear();
	m_menu->addAction(m_modify);
//    m_modify->setEnabled(isModify());
	m_menu->addAction(m_saveEdit);
	m_menu->addAction(m_giveUpEdit);
	m_menu->addAction(m_copy);
	m_menu->addAction(m_paste);
	m_menu->addAction(m_checkResult);
	m_menu->addAction(m_tidyView);
	m_menu->addAction(m_currentTime);
	if (isModify() == false)
	{
		m_modify->setEnabled(false);
		m_saveEdit->setEnabled(false);
		m_giveUpEdit->setEnabled(false);
		m_paste->setEnabled(false);
		m_checkResult->setEnabled(false);
		m_currentTime->setEnabled(false);
	}
}
void RightClickMenu::showMenu(bool isControlClick, QMap<QString, QString> *map, bool isMedical)
{
	m_isMedical = isMedical;
	if (isMedical==true)
	{
		addMedicalActions();
	}
	else
	{
		m_map = map;
		addActions(isControlClick);
	}
	m_menu->exec(QCursor::pos());
}
void RightClickMenu::insertFile()
{

}
void RightClickMenu::fixedText()
{

}
void RightClickMenu::keyWord()
{

}
void RightClickMenu::DocumentLine()
{

}
void RightClickMenu::headerAndFooter()
{

}
void RightClickMenu::medicalFormula()
{

}
void RightClickMenu::specialFormula()
{

}
void RightClickMenu::radio()
{

}
void RightClickMenu::multiSelect()
{

}
void RightClickMenu::IsSelect()
{

}
void RightClickMenu::writeTips()
{

}
void RightClickMenu::image()
{

}
void RightClickMenu::Line()
{

}
void RightClickMenu::pageBreak()
{

}
void RightClickMenu::pageNumber()
{

}
void RightClickMenu::timerAndDate()
{

}
void RightClickMenu::deleteElement()
{
	if (m_map != NULL)
	{
		if (m_map->contains("isDelete"))
		{
			//m_map->remove("isDelete");
			(*m_map)["isDelete"] = "true";
		}
		else
		{
			m_map->insert("isDelete", "true");
		}
	}
}
void RightClickMenu::attribute()
{
	attributeClick dlg;// = new attributeClick();
	if (!m_map->isEmpty())
	{
		if (m_map->contains("isDelete"))
		{
			(*m_map)["isDelete"] = "false";
		}
		else
		{
			m_map->insert("isDelete", "false");
		}
		m_map->insert(TempleteTypeEnumeration::PROPERTY_IS_CHANG, TempleteTypeEnumeration::STR_TRUE);
		dlg.show(*m_map);
	}
}

void RightClickMenu::saveEdit()
{
	QString id = TabWidgetCollection::getInstance()->getCurrentWidgetId();
	QTreeWidgetItem *item = NULL;
	item = TabWidgetCollection::getInstance()->getTreeItem(id);
	MainViewContainer *container = TabWidgetCollection::getInstance()->getContainer();

	QString stringName = item->text(0);
	TabWidgetCollection::getInstance()->saveTabName(stringName);
	QString SaveName = stringName.replace("*", "");
	item->setText(0, SaveName);
	QVariant var = item->data(0, Qt::UserRole);
	QStringList list = var.toStringList();
	QString isModify = list[2];
	QString    isModifySave = list[2];//用于数据库 0:insert,1:modify
	if (isModify != "2")//表明有改动的数据,改动的数据
	{
		isModifySave = "2";
		var.clear();
		QStringList list1;
		if (list.length() == 4)
		{
			list1 << list[0] << list[1] << isModifySave << list[3];
			var.setValue(list1);
		}
		else
		{
			list.removeAt(2);
			list << isModifySave;
			var.setValue(list);
		}

		item->setData(0, Qt::UserRole, var);
		
		if (container->isTemplate())
		{
			container->saveMedicalData(item, isModify, true);
		}
		else
		{
			container->saveTemplateData(item, isModify, true);
		}
		//if (isTemplate == AuthorityConstants::FileType::TEMPLETE)//表明是模板
		//{
		//	container->saveTemplateData(item, isModify);
		//}
		//else
		//{
		//	container->saveMedicalData(item, isModify);
		//}
	}
}

void RightClickMenu::giveUpEdit()
{
	MainViewContainer *container = TabWidgetCollection::getInstance()->getContainer();
	QTreeWidgetItem *item = NULL;
	QString id = TabWidgetCollection::getInstance()->getCurrentWidgetId();
	item = TabWidgetCollection::getInstance()->getTreeItem(id);
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	//appEntrance->init();
	//名字更新回来

	QVariant var = item->data(0, Qt::UserRole);
	QStringList list = var.toStringList();
	if (list[2] == "0")
	{
		MessageBox::showAboutMessageBox(QStringLiteral("提示"), QStringLiteral("新增的病历或模板，未保存时，此功能无效！"));
		return;
	}
	QString stringName = item->text(0);
	TabWidgetCollection::getInstance()->saveTabName(stringName);
	QString SaveName = stringName.replace("*", "");
	item->setText(0, SaveName);
	QStringList list1;
	QString isModify = "2";
	if (list.length() == 4)//将节点的状态回置回来
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
	item->setData(0, Qt::UserRole, var);
	container->showSelectMainMenuItem(item, 0);
	int index = TabWidgetCollection::getInstance()->getCurrentIndex();
	TabWidgetCollection::getInstance()->removeSubTab(index);
	container->showSelectMainMenuItem(item, 0);
}

void RightClickMenu::copy()
{
	
	MyAppEntrance *appEntrance = getInstance();
	if (appEntrance != NULL)
	{
		UIInputManager * mgr = appEntrance->getUIInputMgr();
		mgr->doCopy(appEntrance);
	}
}

void RightClickMenu::paste()
{
	MyAppEntrance *appEntrance = getInstance();
	if (appEntrance != NULL)
	{
		UIInputManager * mgr = appEntrance->getUIInputMgr();
		mgr->doPast(appEntrance);
	}
}
//导入检查结果
void RightClickMenu::checkResult()
{
	MainViewContainer *container = TabWidgetCollection::getInstance()->getContainer();
	//mainMenutempleteTreeWidget
//	bool istemplate = container->isTemplate();
	if (!container->isTemplate())
	{
		MessageBox::showAboutMessageBox(QStringLiteral("提示"), QStringLiteral("此功能在模板下无效"));
		return;
	}
	QString str = Inspection::getData();
	if (str.length()>0)
	{
		QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
		if (widget == NULL)return;
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		UIInputManager * mgr = appEntrance->getUIInputMgr();
		mgr->addSpecialTxt(str);
	}

}

void RightClickMenu::tidyView(bool isTidy)
{
	QString id = TabWidgetCollection::getInstance()->getCurrentWidgetId();
	QTreeWidgetItem *item = NULL;
	item = TabWidgetCollection::getInstance()->getTreeItem(id);
	QString stringName = item->text(0);
	/*根据名字获取上一次的编辑状态*/
	if (isTidy)
	{
		if (stringName.indexOf("*") > 0)
		{
			m_bIsEdit = true;
		}
		else
		{
			m_bIsEdit = false;
		}
	}
	MyAppEntrance *appEntrance = getInstance();
	if (appEntrance != NULL)
	{
		appEntrance->showTidy(isTidy);
	}

	MainViewContainer *container = TabWidgetCollection::getInstance()->getContainer();
	if (isTidy == true)
	{
		container->setMenuStatus(false);
		ToolBarClass * toolbar = appEntrance->getToolbar();
		if (toolbar)
			toolbar->setSomeActionsEnable(false);
		QString SaveName = stringName.replace("*", "");
		item->setText(0, SaveName);
		TabWidgetCollection::getInstance()->modifyMedicalName(SaveName);
	}
	else if (m_bIsEdit == true)
	{
		container->setMenuStatus(true);
		ToolBarClass * toolbar = appEntrance->getToolbar();
		if (toolbar)
			toolbar->setSomeActionsEnable(true);
		QString SaveName = stringName + "*";
		item->setText(0, SaveName);
		TabWidgetCollection::getInstance()->modifyMedicalName(SaveName);
	}
}

MyAppEntrance* RightClickMenu::getInstance()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return NULL;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	return appEntrance;
}

void RightClickMenu::modifyTab()
{
	if (m_bIsCrusRcd == true)
	{
		QString No_;
 		QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
 		if (widget == NULL)return;
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		UIInputManager* mgr = appEntrance->getUIInputMgr();
		mgr->openMedicalRecordEditAuthorityByFocus(No_);

		MainViewContainer *container = TabWidgetCollection::getInstance()->getContainer();
		container->setTreeCurrentItem(No_);
		container->setSaveDataFromRightMenu(true);
		container->modifyTreeNodeAndTab();
	}
	else
	{
		MainViewContainer *container = TabWidgetCollection::getInstance()->getContainer();
		container->setSaveDataFromRightMenu(true);
		container->modifyTreeNodeAndTab();
	}
}

bool RightClickMenu::isModify()
{
	m_bIsCrusRcd = false;
	MainViewContainer *container = TabWidgetCollection::getInstance()->getContainer();
	QTreeWidgetItem *item = NULL;
	QTreeWidget *mainMenuTreeWidget = container->getMainMenuTreeWidget();
	QTreeWidget *mainMenutempleteTreeWidget = container->getMainmenuTemplateWidget();
	QString id = TabWidgetCollection::getInstance()->getCurrentWidgetId();
	item = TabWidgetCollection::getInstance()->getCurrentWidgetTreeItem(id);


	if (mainMenuTreeWidget->isVisible())
	{
		QString strPar = item->parent()->text(0).mid(1, 2);
		if (strPar == progressNote)
		{
			m_bIsCrusRcd = true;
		}
	}

	QVariant var = item->data(0, Qt::UserRole);
	QStringList list = var.toStringList();
	QString   isModify = list[2];
	if (isModify == "0"||isModify=="1")
	return true;

	if (list.size() == 4)
	{
		id = list[0];
	}


	if (mainMenuTreeWidget->isVisible())
	{
		mainMenuTreeWidget->setCurrentItem(item);
		if(container->isAuthority(item, false)) return true;
		if (container->m_bIsAuthor == false)
		{
			return false;
		}
		return container->getPermissionsFormPid(id);
	}
	else
	{
		return container->getPermissionsFormPid(id);
	}
}

QTreeWidgetItem* RightClickMenu::getChildItem(QTreeWidgetItem *pItem, QString id)
{
	if (NULL == pItem||id.length()==0)return NULL;
	for (int i = 0; i < pItem->childCount(); i++)
	{
		QTreeWidgetItem *item = pItem->child(i);
		QVariant var = item->data(0, Qt::UserRole);
		QStringList list = var.toStringList();
		if (list[0] == id)
		{
			return item;
		}
	}

	return NULL;
}




