#include "toolbarclass.h"
#include"qaction.h"
#include"qtextformat.h"
#include"qmenu.h"
#include"qcolor.h"
#include"qfontcombobox.h"
#include"qcombobox.h"
#include"qcolordialog.h"
#include"qtoolbar.h"
#include"qmainwindow.h"
#include"MyAppEntrance.h"
#include"UIInputManager.h"
#include "StrContentData.h"
#include "UIConstants.h"
#include "BaseConstants.h"
#include "BaseMath.h"
#include "TempleteTypeEnumeration.h"
#include "commonWidgets/TabWidgetCollection.h"
#include"dataModel\OperateData\DataBaseControl.h"
#include "AuthorityConstants.h"
#include "MessageBox.h"
#include <QIcon>
#include <QPixmap>
#include "QWidget"
#include <QWidgetAction>
#include <QPainter>
#include <QObjectUserData>
#include <QVariant>


//ToolBarClass * ToolBarClass::instance = NULL;

ToolBarClass::ToolBarClass(QWidget *parent, int flags)
: QToolBar(parent), pObject(NULL), writeHelpToolBar(NULL), writeHelp(NULL), exitToolBar(NULL), model(NULL), m_DbManager(NULL), m_DataBaseControl(NULL)
{
	initPointer();
	isTemplate = flags;
	createActions();
	//m_DbManager = new DBManager();
	//m_DbManager->connectOracle();
	m_DataBaseControl = new DataBaseControl(NULL);
	this->setMovable(false);

	//instance = this;
}
#define DELETEPOINT(objName) \
if (objName != NULL){ delete objName; objName = NULL; }
ToolBarClass::~ToolBarClass()
{
	/*DELETEPOINT(copyAct)DELETEPOINT(pasteAct)DELETEPOINT(cutAct)DELETEPOINT(undoAct)DELETEPOINT(fileToolBar)
		DELETEPOINT(saveAct)DELETEPOINT(saveToolBar)DELETEPOINT(giveUpModifyAct)DELETEPOINT(exitAct)
		DELETEPOINT(boldAct)DELETEPOINT(italicAct)DELETEPOINT(underlineAct)DELETEPOINT(superscriptAct)DELETEPOINT(subscriptAct)DELETEPOINT(fontToolBar)
		DELETEPOINT(leftAlignAct)DELETEPOINT(centerAct)DELETEPOINT(rightAlignAct)DELETEPOINT(justifyAct)DELETEPOINT(alignToolBar)
		DELETEPOINT(colorAct)DELETEPOINT(comboFont)DELETEPOINT(comboSize)DELETEPOINT(comboToolBar)DELETEPOINT(exitToolBar)
		DELETEPOINT(writeHelp)DELETEPOINT(writeHelpToolBar)DELETEPOINT(m_DataBaseControl)DELETEPOINT(m_DbManager)
		DELETEPOINT(popMenu)DELETEPOINT(tablePopMenu)DELETEPOINT(splitPopMenu)*/
}

void ToolBarClass::initPointer()
{
	copyAct = NULL;
	pasteAct = NULL;
	cutAct = NULL;
	undoAct = NULL;
	fileToolBar = NULL;
	saveAct = NULL;
	saveToolBar = NULL;
	giveUpModifyAct = NULL;
	exitAct = NULL;
	boldAct = NULL;
	italicAct = NULL;
	underlineAct = NULL;
	superscriptAct = NULL;
	subscriptAct = NULL;
	fontToolBar = NULL;
	leftAlignAct = NULL;
	centerAct = NULL;
	rightAlignAct = NULL;
	justifyAct = NULL;
	alignToolBar = NULL;
	colorAct = NULL;
	comboFont = NULL;
	comboSize = NULL;
	comboToolBar = NULL;
	exitToolBar = NULL;
	writeHelp = NULL;
	writeHelpToolBar = NULL;
	m_DataBaseControl = NULL;
	m_DbManager = NULL;
	popMenu = NULL;
	tablePopMenu = NULL;
	splitPopMenu = NULL;
	submitToolBar = NULL;
	submitCur = NULL;

	m_pUpDownAlignToolBar = NULL;
	m_pUpAlignAct = NULL;
	m_pCenterAlignAct = NULL;
	m_pDownAlignAct = NULL;

}
void ToolBarClass::setIsEnable(bool pIsEnable)
{
	this->setEnabled(false);
	copyAct->setEnabled(true);

	//屏蔽掉字体的设置
	comboFont->setEnabled(true);
	comboSize->setEnabled(true);
	comboFont->setVisible(true);
	comboSize->setVisible(true);

}
void ToolBarClass::InitObjcet(MyAppEntrance *object)
{
	setIsEnable(false);
	pObject = object;
	mCanResetApp = true;
	createToolBars();
	createConnectSlots();
	setToolBarStyle();
	pObject->setContextMenuPolicy(Qt::NoContextMenu);
	this->setFocusProxy(pObject);
}
void ToolBarClass::setToolBarStyle()
{
	QString toolbarStyle = "QToolBar {"
		"background: rgb(255,255,255);"
		"spacing: 3px;" /* spacing between items in the tool bar */
		"border-width: 0px;"
		"min-height:8ex;"
		"}"
		"QToolBar::handle {"
		"color: rgb(255, 0, 0);"
		"}"
		"QToolBox::tab {"
		"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
		"stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,"
		"stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);"
		"border-radius: 5px;"
		" color: darkgray;"
		"}"
		"QToolBox::tab:selected { " /*italicize selected tabs*/
		"font: italic;"
		"color: white;"
		"}";
	QString toolBtnStyle = "QToolButton {" /* all types of tool button */
		"background: transparent;"
		"border-width: 1px;"
		"border-style: solid;"
		//"width: 44px;"
		"height: 16px;"
		"}"
		"QToolButton[popupMode=\"1\"] { "/* only for MenuButtonPopup */
		"padding-right: 20px;" /* make way for the popup button */
		"}"
		"QToolButton:checked{"
		"background-color:rgb(253,244,191)"
		"}"
		"QToolButton:pressed {"
		"background-color:rgb(253, 244, 191);"
		"color:black;"
		"border-color: rgb(253, 244, 191);"
		"}"
		"QToolButton:hover{"
		"background-color:rgb(253, 244, 191);"
		"color:black;"
		"border-color: rgb(253, 244, 191);}"
		"QToolButton::menu-button {"
		"border: 0px;"
		"width: 16px;"
		"}"
		"QToolButton#textColor {"
		"QToolButton#textColor::menu-button {"
		"border-left: 1px solid;"
		"}"
		"QToolButton#backgroundColor {"
		"}"
		"QToolButton#backgroundColor::menu-button {"
		"border-left: 1px solid;"
		"}"	
		"QToolButton::menu-arrow:open {"
		" top: 1px; left: 1px;" /* shift it a bit */
		"}";
	pObject->setStyleSheet(toolbarStyle + toolBtnStyle);
}
#define CREATEACTION(objName, name, tip) \
	objName = new QAction(tr(name), this); \
	objName->setStatusTip(tr(tip));
void ToolBarClass::createActions()
{
	//段落文字的操作
	CREATEACTION(copyAct, "复制", "复制")
		CREATEACTION(pasteAct, "粘贴", "粘贴")
		CREATEACTION(cutAct, "剪切", "剪切")
		CREATEACTION(undoAct, "撤销", "撤销")
		CREATEACTION(redoAct, "恢复", "恢复")
		copyAct->setIcon(QIcon(":/res/Res/MenuIcon/copy.png"));
	pasteAct->setIcon(QIcon(":/res/Res/MenuIcon/paste.png"));
	cutAct->setIcon(QIcon(":/res/Res/MenuIcon/scissors.png"));
	undoAct->setIcon(QIcon(":/res/Res/MenuIcon/undo.png"));
	redoAct->setIcon(QIcon(":/res/Res/MenuIcon/do.png"));

		//字体样式设置
		CREATEACTION(boldAct, "加粗", "加粗")
		CREATEACTION(italicAct, "倾斜", "倾斜")
		CREATEACTION(underlineAct, "下划线", "下划线")
		CREATEACTION(superscriptAct, "字体上标", "字体上标")
		CREATEACTION(subscriptAct, "字体下标", "字体下标")

		boldAct->setIcon(QIcon(":/res/Res/MenuIcon/bold.png"));
		italicAct->setIcon(QIcon(":/res/Res/MenuIcon/italic.png"));
		underlineAct->setIcon(QIcon(":/res/Res/MenuIcon/underline.png"));
		superscriptAct->setIcon(QIcon(":/res/Res/MenuIcon/up.png"));
		subscriptAct->setIcon(QIcon(":/res/Res/MenuIcon/down.png"));

		//对齐方式
		//QToolBar *alignToolBar;
		CREATEACTION(leftAlignAct, "左对齐", "左对齐")
		CREATEACTION(centerAct, "居中", "居中")
		CREATEACTION(rightAlignAct, "右对齐", "右对齐")
		CREATEACTION(justifyAct, "两端对齐", "两端对齐")

		leftAlignAct->setIcon(QIcon(":/res/Res/MenuIcon/AlignLeft.png"));
		centerAct->setIcon(QIcon(":/res/Res/MenuIcon/AlignCenter.png"));
		rightAlignAct->setIcon(QIcon(":/res/Res/MenuIcon/AlignRight.png"));

		CREATEACTION(m_pUpAlignAct, "文字顶端对齐", "文字顶端对齐");
		CREATEACTION(m_pCenterAlignAct, "文字居中对齐", "文字居中对齐");
		CREATEACTION(m_pDownAlignAct, "文字底端对齐", "文字底端对齐");

		m_pUpAlignAct->setIcon(QIcon(":/res/Res/MenuIcon/AlignTop.png"));
		m_pCenterAlignAct->setIcon(QIcon(":/res/Res/MenuIcon/AlignMiddle.png"));
		m_pDownAlignAct->setIcon(QIcon(":/res/Res/MenuIcon/AlignBottom.png"));

		//数字签名
		CREATEACTION(finishAct, "数字签名", "数字签名")

		//字体设置 
		CREATEACTION(colorAct, "字体颜色", "字体颜色")

		setFontCombox();
	//书写助手弹出框
	CREATEACTION(icd_10Code, "icd_10国际编码", "icd_10国际编码")
		CREATEACTION(drugDataBase, "药品资料库", "药品资料库")
		CREATEACTION(diagnosisTreatment, "常见疾病诊疗规范", "常见疾病诊疗规范")
		CREATEACTION(commonDictionary, "常用字典", "常用字典")
		//书写助手
		CREATEACTION(writeHelp, "书写助手", "书写助手")
		popMenu = new QMenu;
	popMenu->addAction(icd_10Code);
	popMenu->addAction(drugDataBase);
	popMenu->addAction(diagnosisTreatment);
	popMenu->addAction(commonDictionary);

	//表格操作
	CREATEACTION(mergeCell, "合并单元格", "合并单元格")
		CREATEACTION(landscapeCell, "横向拆分", "横向拆分")
		CREATEACTION(portraitCell, "竖向拆分", "竖向拆分")
		CREATEACTION(tableOptions, "表格", "表格")
		CREATEACTION(splitCell, "拆分单元格", "拆分单元格")

		tablePopMenu = new QMenu;
	tablePopMenu->addAction(mergeCell);
	//tablePopMenu->addAction(splitCell);

	splitPopMenu = new QMenu("拆分单元格");
	splitPopMenu->addAction(landscapeCell);
	splitPopMenu->addAction(portraitCell);
	tablePopMenu->addMenu(splitPopMenu);

#ifdef  _CHAO_YANG_
	CREATEACTION(submitCur, "病历提交", "病历提交")
#endif //  _CHAO_YANG_

	CREATEACTION(saveAct, "保存", "保存")
	CREATEACTION(giveUpModifyAct, "放弃修改", "放弃修改")
	CREATEACTION(exitAct, "退出", "退出")
	CREATEACTION(printAct, "打印", "打印")

	saveAct->setIcon(QIcon(":/res/Res/MenuIcon/formRegionSave.png"));
	giveUpModifyAct->setIcon(QIcon(":/res/Res/MenuIcon/ClearGrid.png"));
	//exitAct->setIcon(QIcon(":/MenuIcon/Res/MenuIcon/AlignLeft.png"));
	//exitAct->setIcon(QIcon(":/Res/themes/generic/images/innerdockdocpaneright.png"));
	exitAct->setIcon(QIcon(":/res/Res/MenuIcon/pinclose.png"));	
	//this->widgetForAction(saveAct)->resize(QSize(22, 22));
	//QPixmap * pixm
}
void ToolBarClass::createToolBars()
{

	saveToolBar = pObject->addToolBar("保存");
	
	va_listToolBars(saveToolBar, 2, saveAct, giveUpModifyAct);
	saveToolBar->addSeparator();
	saveToolBar->setMovable(false);

	fileToolBar = pObject->addToolBar("文件");
	va_listToolBars(fileToolBar, 5, copyAct, pasteAct, cutAct, undoAct,redoAct);
	fileToolBar->addSeparator();
	fileToolBar->setMovable(false);

	grp = new QActionGroup(this);

	fontToolBar = pObject->addToolBar("字体");
	va_listToolBars(fontToolBar, 5, boldAct, italicAct, underlineAct, superscriptAct, subscriptAct);
	boldAct->setCheckable(true);
	italicAct->setCheckable(true);
	underlineAct->setCheckable(true);

	QFont bold;
	bold.setBold(true);
	boldAct->setFont(bold);

	QFont italic;
	italic.setItalic(true);
	//italic.setBold(true);   
	italicAct->setFont(italic);


	QFont underline;
	underline.setUnderline(true);
	underlineAct->setFont(underline);

	//grp->addAction(superscriptAct);
	//grp->addAction(subscriptAct);
	superscriptAct->setCheckable(true);
	subscriptAct->setCheckable(true);

	fontToolBar->addSeparator();
	fontToolBar->setMovable(false);

	grpAlign = new QActionGroup(this);

	alignToolBar = pObject->addToolBar("对齐");
	va_listToolBars(alignToolBar, 3, leftAlignAct, centerAct, rightAlignAct);
	leftAlignAct->setCheckable(true);
	centerAct->setCheckable(true);
	rightAlignAct->setCheckable(true);
	justifyAct->setCheckable(true);
	grpAlign->addAction(leftAlignAct);
	grpAlign->addAction(centerAct);
	grpAlign->addAction(rightAlignAct);
	//grpAlign->addAction(justifyAct);

	alignToolBar->addSeparator();
	alignToolBar->setMovable(false);

	m_pgroupUpDownAlign = new QActionGroup(this);
	m_pUpDownAlignToolBar = pObject->addToolBar("对齐方式");
	va_listToolBars(m_pUpDownAlignToolBar, 3, m_pUpAlignAct, m_pCenterAlignAct, m_pDownAlignAct);
	m_pUpAlignAct->setCheckable(true);
	m_pCenterAlignAct->setCheckable(true);
	m_pDownAlignAct->setCheckable(true);
	m_pgroupUpDownAlign->addAction(m_pUpAlignAct);
	m_pgroupUpDownAlign->addAction(m_pCenterAlignAct);
	m_pgroupUpDownAlign->addAction(m_pDownAlignAct);
	m_pUpDownAlignToolBar->addSeparator();
	m_pUpDownAlignToolBar->setMovable(false);

	//屏蔽掉字体大小，颜色设置
	//if (isTemplate == AuthorityConstants::FileType::TEMPLETE)
	//{
//	pObject->addToolBarBreak(Qt::TopToolBarArea);
	comboToolBar = pObject->addToolBar("字体颜色及大小");
	comboToolBar->addWidget(comboFont);
	comboToolBar->addWidget(comboSize);
//	va_listToolBars(comboToolBar, 1, colorAct); //屏蔽掉颜色

/*****************************************zl字体颜色，背景颜色**********************************************/
	mTbTextColor = new QToolButton();
	mTbTextColor->setAutoRaise(false);
	mTbTextColor->setObjectName("textColor");
	mTbTextColor->setToolTip("字体颜色");
	mTbTextColor->setIcon(QIcon(":/res/Res/MenuIcon/textcolor/textcolor_black.png"));
	mTbTextColor->setProperty("textColor", QColor(0,0,0));
	connect(mTbTextColor, SIGNAL(clicked()), this, SLOT(setTextColor()));
	createColorTb(mTbTextColor);
	comboToolBar->addWidget(mTbTextColor);

	mTbTextBackgroundColor = new QToolButton();
	mTbTextBackgroundColor->setObjectName("backgroundColor");
	mTbTextBackgroundColor->setIcon(QIcon(":/res/Res/MenuIcon/backgroundcolor/backgroundcolor_black.png"));
	mTbTextBackgroundColor->setProperty("backgroundColor", QColor(0,0,0));
	connect(mTbTextBackgroundColor, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
	createColorTb(mTbTextBackgroundColor);
	comboToolBar->addWidget(mTbTextBackgroundColor);
/***********************************************************************************************************/

	comboToolBar->addSeparator();
	comboToolBar->setMovable(false);

#ifdef  _CHAO_YANG_

	submitToolBar = pObject->addToolBar("提交");
	va_listToolBars(submitToolBar, 1, submitCur);
	submitToolBar->addSeparator();
	submitToolBar->setMovable(false);
#endif

	exitToolBar = pObject->addToolBar("退出");
	va_listToolBars(exitToolBar, 1, exitAct);
	exitToolBar->setMovable(false);

	//}
	//屏蔽掉表格
	/*tableToolBar = pObject->addToolBar("表格操作");
	va_listToolBars(tableToolBar, 1, tableOptions);
	tableToolBar->addSeparator();*/

	//屏蔽掉打印
	/*printToolBar = pObject->addToolBar("打印");
	va_listToolBars(printToolBar, 1, printAct);
	printToolBar->addSeparator();
	printToolBar->setMovable(false);*/





	//writeHelpToolBar = pObject->addToolBar("书写助手");
	//va_listToolBars(writeHelpToolBar, 1, writeHelp);


	//finishToolBar = pObject->addToolBar("数字签名");
	//va_listToolBars(finishToolBar, 1, finishAct);

}
//字体上标
void ToolBarClass::superscrip()
{
	UIInputManager * mgr = pObject->getUIInputMgr();
	if (superscriptAct->isChecked() == true)
	{
		mgr->setFlowType(BaseConstants::Direction::UP);
		subscriptAct->setChecked(false);//上下标的选中为互斥事件   尹露
	}
	else
	{
		mgr->setFlowType(BaseConstants::NONE);
	}

}
//字体下标
void ToolBarClass::subscript()
{
	UIInputManager * mgr = pObject->getUIInputMgr();
	if (subscriptAct->isChecked() == true)
	{
		mgr->setFlowType(BaseConstants::Direction::DOWN);
		superscriptAct->setChecked(false);//上下标的选中为互斥事件   尹露
	}
	else
	{
		mgr->setFlowType(BaseConstants::NONE);
	}

}
void ToolBarClass::createConnectSlots()
{
	connect(saveAct, SIGNAL(triggered(bool)), this, SLOT(saveFile()));
	//connect(grp, SIGNAL(triggered(QAction*)), this, SLOT(textStyle(QAction*)));
	connect(grpAlign, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));
	connect(m_pgroupUpDownAlign, SIGNAL(triggered(QAction*)), this, SLOT(textUpDownAlign(QAction*)));
	connect(colorAct, SIGNAL(triggered(bool)), this, SLOT(textColor()));
	connect(writeHelp, SIGNAL(triggered(bool)), this, SLOT(writeHelpClick()));
	connect(finishAct, SIGNAL(triggered(bool)), this, SLOT(finishFile()));

	connect(icd_10Code, SIGNAL(triggered(bool)), this, SLOT(icd_10CodeFunc()));
	connect(drugDataBase, SIGNAL(triggered(bool)), this, SLOT(drugDataBaseFunc()));
	connect(diagnosisTreatment, SIGNAL(triggered(bool)), this, SLOT(diagnosisTreatmentFunc()));
	connect(commonDictionary, SIGNAL(triggered(bool)), this, SLOT(commonDictionaryFunc()));

	connect(boldAct, SIGNAL(triggered()), this, SLOT(textBold()));
	connect(italicAct, SIGNAL(triggered()), this, SLOT(textItalic()));
	connect(underlineAct, SIGNAL(triggered()), this, SLOT(textUnderline()));
	connect(superscriptAct, SIGNAL(triggered(bool)), this, SLOT(superscrip()));
	connect(subscriptAct, SIGNAL(triggered(bool)), this, SLOT(subscript()));

	connect(comboFont, SIGNAL(activated(int)), this, SLOT(changeFont(int )));

	connect(comboSize, SIGNAL(activated(int)), this, SLOT(changeSize(int)));

	connect(tableOptions, SIGNAL(triggered(bool)), this, SLOT(tableOptionsClick()));


	//connect(splitCell, SIGNAL(hovered()), this, SLOT(splitCellClick()));

	connect(landscapeCell, SIGNAL(triggered(bool)), this, SLOT(landscapeCellClick()));
	connect(portraitCell, SIGNAL(triggered(bool)), this, SLOT(portraitCellClick()));
	connect(mergeCell, SIGNAL(triggered(bool)), this, SLOT(mergeCellCellClick()));

	connect(giveUpModifyAct, SIGNAL(triggered(bool)), this, SLOT(giveUpModifyClick()));

#ifdef  _CHAO_YANG_
	connect(submitCur, SIGNAL(triggered(bool)), this, SLOT(submitMdc()));
#endif

	connect(exitAct, SIGNAL(triggered(bool)), this, SLOT(exitClick()));
	connect(printAct, SIGNAL(triggered(bool)), this, SLOT(printeClick()));

	connect(copyAct, SIGNAL(triggered(bool)), this, SLOT(copyData()));
	connect(cutAct, SIGNAL(triggered(bool)), this, SLOT(cutData()));
	connect(pasteAct, SIGNAL(triggered(bool)), this, SLOT(pasteData()));
	connect(undoAct, SIGNAL(triggered(bool)), this, SLOT(undoData()));
	connect(redoAct, SIGNAL(triggered(bool)), this, SLOT(redoData()));
}
void ToolBarClass::copyData()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	mgr->doCopy(appEntrance);
}
void ToolBarClass::cutData()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	mgr->doCut(appEntrance);
}
void ToolBarClass::pasteData()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	mgr->doPast(appEntrance);
}
void ToolBarClass::undoData()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	mgr->doUndo();
}
void ToolBarClass::redoData()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	mgr->doRedo();
}
void ToolBarClass::giveUpModifyClick()
{
	MainViewContainer *container = TabWidgetCollection::getInstance()->getContainer();
	container->m_pHideOrDisplayHeaderAct->setChecked(false);
	container->m_pHideOrDisplayHeaderAct->setChecked(false);
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
void ToolBarClass::exitClick()
{
	TabWidgetCollection::getInstance()->setUnSaveTabtoCurrent();
	QString id = TabWidgetCollection::getInstance()->getCurrentWidgetId();
	QTreeWidgetItem *item = NULL;
	item = TabWidgetCollection::getInstance()->getTreeItem(id);
	MainViewContainer *container = TabWidgetCollection::getInstance()->getContainer();

	QVariant var = item->data(0, Qt::UserRole);
	QStringList list = var.toStringList();
	QString isModify = list[2];
	bool isTemplateState = false;
	if (isTemplate == AuthorityConstants::FileType::TEMPLETE)
	{
		isTemplateState = true;
	}
	if (isModify == "0")
	{
		int rb = MessageBox::showQueryMessageBox(QStringLiteral("保存"), QStringLiteral("是否保存当前页数据?"));
		if (rb == QMessageBox::Yes)
		{
			if (isTemplate == AuthorityConstants::FileType::TEMPLETE)//模板状态
			{
				container->saveTemplateData(item, isModify);
			}
			else
			{
				container->saveMedicalData(item, isModify);
			}
		}
		else if (rb==QMessageBox::Cancel)
		{
			return;
		}
	}
	if (isModify == "1")
	{
		QByteArray byteArray = m_DataBaseControl->getMedicalData(id, isTemplateState);
		UIInputManager* mgr = pObject->getUIInputMgr();
		QByteArray emrByteArray = mgr->getByteArray(true,false);
		if (byteArray != emrByteArray)
		{
			int rb = MessageBox::showQueryMessageBox(QStringLiteral("保存"), QStringLiteral("是否保存当前页数据?"));
			if (rb == QMessageBox::Yes)
			{
				if (list.length() == 4)//病程记录特殊处理
				{
					id = list[3];
				}
				m_DataBaseControl->updateMedicalData(emrByteArray, id, isTemplateState);
			}
			else if (rb==QMessageBox::Cancel)
			{
				return;
			}
		}
	}
	//isModify = "2";
	//list.removeAt(2);
	//list << isModify;
	QStringList list1;
	if (list.length() == 4)
	{
		list1 << list[0] << list[1] << "2" << list[3];
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
	int index = TabWidgetCollection::getInstance()->getCurrentIndex();
	TabWidgetCollection::getInstance()->removeSubTab(index);
	if (isTemplate == AuthorityConstants::FileType::MEDICAL_RECORDS)
	{
		TabWidgetCollection::getInstance()->getContainer()->close();
	}
	else
	{
		TabWidgetCollection::getInstance()->getContainer()->hide();
		TabWidgetCollection::getInstance()->getContainer()->templateSet();
	}

}
void ToolBarClass::changeSize(int index)
{
	int num = comboSize->itemText(index).toInt();

	UIInputManager * mgr = pObject->getUIInputMgr();
	mgr->setFontSize(num);
}
void ToolBarClass::changeFont(int index)
{
	QFont font = comboFont->currentFont();
	UIInputManager * mgr = pObject->getUIInputMgr();
	mgr->setFontFamily(font.family());
}
void ToolBarClass::textBold()
{
	/*QTextCharFormat fmt;
	fmt.setFontWeight(boldAct->isChecked() ? QFont::Bold : QFont::Normal);*/
	UIInputManager * mgr = pObject->getUIInputMgr();
	if (boldAct->isChecked())
	{
		mgr->setFontBold(true);
	}
	else
	{
		mgr->setFontBold(false);
	}

}
void ToolBarClass::textItalic()
{
	UIInputManager * mgr = pObject->getUIInputMgr();
	if (italicAct->isChecked())
	{
		mgr->setFontItalic(true);
	}
	else
	{
		mgr->setFontItalic(false);
	}
}
void ToolBarClass::textUnderline()
{
	UIInputManager * mgr = pObject->getUIInputMgr();
	if (underlineAct->isChecked())
	{
	
		mgr->setUnderlineParams(UIConstants::LineType::SINGLE_LINE, 1);
	}
	else
	{
		mgr->setUnderlineParams(BaseConstants::NONE, 1);
	}
}
void ToolBarClass::icd_10CodeFunc()
{
	//书写器预留
}
void ToolBarClass::drugDataBaseFunc()
{
	//书写器预留
}
void ToolBarClass::diagnosisTreatmentFunc()
{
	//书写器预留
}
void ToolBarClass::commonDictionaryFunc()
{
	//书写器预留
}
void ToolBarClass::va_listToolBars(QToolBar * toolBar, int argc, QAction* ...)
{
	va_list va;
	va_start(va, argc);
	for (int i = 0; i != argc; ++i)
	{
		toolBar->addAction(va_arg(va, QAction*));
	}
	va_end(va);
}
void ToolBarClass::setFontCombox()
{
	comboFont = new QFontComboBox(this);
	comboSize = new QComboBox(this);
	comboFont->setEditable(false);
	comboSize->setEditable(false);
	comboFont->installEventFilter(this);

	//comboFont->setFocusPolicy(Qt::NoFocus|);

	comboSize->installEventFilter(this);
	//comboSize->setFocusPolicy(Qt::NoFocus);
	//comboSize->setEditable(true);
	QFontDatabase db;
	foreach(int size, db.standardSizes())
	{
		if (size>50)
		{
			break;
		}
		comboSize->addItem(QString::number(size));
	}
}
void ToolBarClass::setTextSize(QString &str)
{
	qreal pointSize = str.toFloat();
	if (str.toFloat() > 0)
	{
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);
	}
}
void ToolBarClass::dividCellClick(int row, int col)
{
//	pObject->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_DEVIDE_H_V + QString::number(row) + ":" + QString::number(col));
}
void ToolBarClass::addCells(int direction)
{
//	pObject->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_ADD_ROW_OR_COL + QString::number(direction));
}
void ToolBarClass::deleteRowOrCol(bool isRow)
{
//	pObject->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_DEL_ROW_OR_COL + QString::number(isRow));
}
void ToolBarClass::setExcelProperty(bool isExelCellProperty)
{
//	pObject->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_PROPERTY + QString::number(isExelCellProperty));
}
//hebing 
void ToolBarClass::mergeCellCellClick()
{
//	pObject->sendKeyEvent(TempleteTypeEnumeration::EXCEL_MERGER);
}
//hengxiang
void ToolBarClass::landscapeCellClick()
{
//	pObject->sendKeyEvent(TempleteTypeEnumeration::EXCEL_DEVIDE_H);
}
//zhongxiang
void ToolBarClass::portraitCellClick()
{
//	pObject->sendKeyEvent(TempleteTypeEnumeration::EXCEL_DEVIDE_V);
}

void ToolBarClass::splitCellClick()
{
}
void ToolBarClass::inputParticularSign()
{
//	pObject->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::INPUT_PATTICULAR_SIGN);
}
void ToolBarClass::tableOptionsClick()
{
	tablePopMenu->exec(QCursor::pos());
}
void ToolBarClass::writeHelpClick()
{
	popMenu->exec(QCursor::pos());//设置弹出框的位置
}
void ToolBarClass::textColor()
{
	QColor col = QColorDialog::getColor(Qt::red, this);
	
	if (!col.isValid())
		return;
	int cloNum = BaseMath::exchangeQColorToColor(col);
	UIInputManager * mgr = pObject->getUIInputMgr();
	mgr->setFontColor(cloNum);
}
void ToolBarClass::printeClick()
{
//	pObject->print();
}
void ToolBarClass::textAlign(QAction *a)
{
	UIInputManager * mgr = pObject->getUIInputMgr();
	if (a == leftAlignAct)
	{
		//书写器预留
		mgr->setHorizonAlignType(BaseConstants::Direction::LEFT);
	}

	else if (a == centerAct)
	{
		//书写器预留
		mgr->setHorizonAlignType((BaseConstants::Direction::MIDDLE));
	}
	else if (a == rightAlignAct)
	{
		//书写器预留
		mgr->setHorizonAlignType((BaseConstants::Direction::RIGHT));
	}
	else if (a == justifyAct)
	{
		//书写器预留
	}
}

void ToolBarClass::textUpDownAlign(QAction* a)
{
	UIInputManager * mgr = pObject->getUIInputMgr();
	if (a == m_pUpAlignAct)
	{
		//书写器预留
	//	mgr->setHorizonAlignType(BaseConstants::Direction::LEFT);
		mgr->setExcelParagraphAlignVerticalType(BaseConstants::Direction::UP);

	}

	else if (a == m_pCenterAlignAct)
	{
		//书写器预留
	//	mgr->setHorizonAlignType((BaseConstants::Direction::MIDDLE));
		mgr->setExcelParagraphAlignVerticalType(BaseConstants::Direction::MIDDLE);
	}
	else if (a == m_pDownAlignAct)
	{
		//书写器预留
	//	mgr->setHorizonAlignType((BaseConstants::Direction::RIGHT));
		mgr->setExcelParagraphAlignVerticalType(BaseConstants::Direction::DOWN);
	}
	else if (a == justifyAct)
	{
		//书写器预留
	}
}

void ToolBarClass::textStyle(QAction *a)
{
	UIInputManager * mgr = pObject->getUIInputMgr();
	if (a == boldAct)
	{
		//书写器预留
	}

	else if (a == italicAct)
	{
		//书写器预留
	}
	else if (a == underlineAct)
	{
		//书写器预留
	}
	else if (a == superscriptAct)
	{
		mgr->setFlowType(BaseConstants::Direction::UP);
	}
	else if (a == subscriptAct)
	{
		mgr->setFlowType(BaseConstants::Direction::DOWN);
	}

}
//void ToolBarClass::modifyItemData(QTreeWidgetItem *pitem)
//{
//	showSelectMainMenuItem(item, 0);
//	QString stringName = item->text(0);
//	QString ModifyName = stringName.replace("*", "") + "*";
//	item->setText(0, ModifyName);
//	TabWidgetCollection::getInstance()->modifyTabName(stringName);
//	//修改其状态
//	QVariant var = item->data(0, Qt::UserRole);
//	QStringList list = var.toStringList();
//	QString   isModify = list[2];
//	if (isModify != "0")
//	{
//		var.clear();
//		list.removeAt(2);
//		isModify = "1";
//		list << isModify;
//		var.setValue(list);
//		item->setData(0, Qt::UserRole, var);
//	}
//}
void ToolBarClass::saveFile()
{
	QString id = TabWidgetCollection::getInstance()->getCurrentWidgetId();
	QTreeWidgetItem *item = NULL;
	item = TabWidgetCollection::getInstance()->getTreeItem(id);
	MainViewContainer *container = TabWidgetCollection::getInstance()->getContainer();
	QString stringName = item->text(0);
	QVariant var = item->data(0, Qt::UserRole);
	QStringList list = var.toStringList();
	QString isModify = list[2];
	QString    isModifySave = list[2];//用于数据库 0:insert,1:modify
	if (isModify != "2")//表明有改动的数据,改动的数据
	{
		isModifySave = "2";
		/*	list.removeAt(2);
			list << isModifySave;*/
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
		if (isTemplate == AuthorityConstants::FileType::TEMPLETE)//表明是模板
		{
			//TabWidgetCollection::getInstance()->saveTabName(stringName);
			container->saveTemplateData(item, isModify);		
		}
		else
		{
			//TabWidgetCollection::getInstance()->saveTabName(stringName);
			container->saveMedicalData(item, isModify);
		}
//		container->m_pLayoutAct->setChecked(false);
		//if (isModify == "0")//保存后将状态重置为修改，目的：保存后，编写在保存
		//{
		//	isModifySave = "1";
		//	list.removeAt(2);
		//	list << isModifySave;
		//	var.clear();
		//	var.setValue(list);
		//	item->setData(0, Qt::UserRole, var);
		//}
		container->setMenuStatus(false);
	}
	QString SaveName = stringName;
	SaveName = SaveName.replace("*", "");
	item->setText(0, SaveName);
	TabWidgetCollection::getInstance()->saveTabName(stringName);
}
void ToolBarClass::finishFile()
{
	//书写器预留
}
//type:1:加粗，2：倾斜，3,字体大小，4：下划线，5：字体,6，字体上标，7，字体下标,str:字体大小，可不填
void ToolBarClass::setActionIsChecked(int type, bool isCheck, QString str)
{
	switch (type)
	{
	case 1:
		boldAct->setChecked(isCheck); break;
	case 2:
		italicAct->setChecked(isCheck); break;
	case 3:
		comboSize->setCurrentText(str); break;
	case 4:
		underlineAct->setChecked(isCheck); break;
	case 5:
		comboFont->setCurrentText(str); break;
	case 6:
		superscriptAct->setChecked(isCheck); break;
	case 7:
		subscriptAct->setChecked(isCheck); break;
	default:
		break;
	}
}

bool ToolBarClass::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == comboFont || watched == comboSize)
	{
		if (event->type() == QEvent::FocusIn)
		{
			pObject->setFocus();
			//	return false;
		}

	}
	return QWidget::eventFilter(watched, event);
}

void ToolBarClass::setToolBarIsChecked(StrContentData * data, char alignType)
{
	if (data == NULL)
		return;

	mCanResetApp = false;
	// 粗体
	boldAct->setChecked(data->getBold());
	// 倾斜
	italicAct->setChecked(data->getItalic());
	// 下划线
	char underlineType = data->getUnderLineType();
	underlineAct->setChecked(underlineType);
	//字体上、下标

	//字体样式
	comboFont->setCurrentText(data->getFamily());
	//字体大小
	int pointSize = data->getPointSize();
	comboSize->setCurrentText(QString::number(pointSize));
	char flowType = data->getFlowType();
	switch (flowType)
	{
	case BaseConstants::Direction::UP:
		superscriptAct->setChecked(true);
		subscriptAct->setChecked(false);
		break;
	case BaseConstants::Direction::DOWN:
		superscriptAct->setChecked(false);
		subscriptAct->setChecked(true);
		break;
	case BaseConstants::NONE:
		superscriptAct->setChecked(false);
		subscriptAct->setChecked(false);
		break;
	}

	//对齐方式
	switch (alignType)
	{
	case BaseConstants::Direction::LEFT:
		leftAlignAct->setChecked(true);
		centerAct->setChecked(false);
		rightAlignAct->setChecked(false);
		break;
	case BaseConstants::Direction::MIDDLE:
		leftAlignAct->setChecked(false);
		centerAct->setChecked(true);
		rightAlignAct->setChecked(false);
		break;
	case BaseConstants::Direction::RIGHT:
		leftAlignAct->setChecked(false);
		centerAct->setChecked(false);
		rightAlignAct->setChecked(true);
		break;
	default: break;
	}

	mCanResetApp = true;

}

void ToolBarClass::setToolBarIsChecked(StrContentData * data, char alignType, char upDownAlignType)
{
	if(data != NULL) 
		setToolBarIsChecked(data, alignType);
	switch (upDownAlignType)
	{
	case BaseConstants::Direction::UP:
		m_pUpAlignAct->setChecked(true);
		m_pCenterAlignAct->setChecked(false);
		m_pDownAlignAct->setChecked(false);
		break;
	case BaseConstants::Direction::MIDDLE:
		m_pUpAlignAct->setChecked(false);
		m_pCenterAlignAct->setChecked(true);
		m_pDownAlignAct->setChecked(false);
		break;
	case BaseConstants::Direction::DOWN:
		m_pUpAlignAct->setChecked(false);
		m_pCenterAlignAct->setChecked(false);
		m_pDownAlignAct->setChecked(true);
		break;
	default: break;
	}
}


void ToolBarClass::setActionsEnabled(bool state)
{
	undoAct->setEnabled(state);//撤销
	cutAct->setEnabled(state);//剪切
	copyAct->setEnabled(state);//复制
	pasteAct->setEnabled(state);//粘贴
	boldAct->setEnabled(state); //加粗
	italicAct->setEnabled(state);//倾斜
	underlineAct->setEnabled(state);//下划线
	superscriptAct->setEnabled(state);//字体上标
	subscriptAct->setEnabled(state);//字体下标
	leftAlignAct->setEnabled(state); //左对齐
	centerAct->setEnabled(state);    //居中
	rightAlignAct->setEnabled(state); //右对齐
	justifyAct->setEnabled(state);//两端对齐
	finishAct->setEnabled(state);//数字签名
	//colorAct->setEnabled(state);//字体颜色
	//屏蔽掉颜色
	colorAct->setEnabled(true);
	colorAct->setVisible(true);
	comboFont->setEnabled(state);//字体样式
	comboSize->setEnabled(state);//字体大小
	writeHelp->setEnabled(state);

	icd_10Code->setEnabled(state);//icd_10
	drugDataBase->setEnabled(state);//药品资料库
	diagnosisTreatment->setEnabled(state);//常见疾病诊疗规范
	commonDictionary->setEnabled(state);//常用字典

	tableOptions->setEnabled(state);
	mergeCell->setEnabled(state);//合并单元格
	splitCell->setEnabled(state);//拆分单元格
	landscapeCell->setEnabled(state);//横向拆分单元格
	portraitCell->setEnabled(state);//竖向拆分单元格

	giveUpModifyAct->setEnabled(state);//放弃修改
	exitAct->setEnabled(state);//退出

	printAct->setEnabled(state);//打印
	//屏蔽掉打印 
	printAct->setEnabled(false);
	printAct->setVisible(false);
	saveAct->setEnabled(state);

	m_pUpAlignAct->setEnabled(state);//纵向对齐
	m_pCenterAlignAct->setEnabled(state);//纵向对齐
	m_pDownAlignAct->setEnabled(state);//纵向对齐
#ifdef  _CHAO_YANG_
	submitCur->setEnabled(state);//
#endif
}

//ToolBarClass * ToolBarClass::getInstance()
//{
//	return instance;
//}

void ToolBarClass::setAlignment(int type)
{
	UIInputManager * mgr = pObject->getUIInputMgr();
	if (type == 1)
	{
		//书写器预留
		leftAlignAct->setChecked(true);
		mgr->setHorizonAlignType(BaseConstants::Direction::LEFT);
	}

	else if (type == 2)
	{
		//书写器预留
		centerAct->setChecked(true);
		mgr->setHorizonAlignType(BaseConstants::Direction::MIDDLE);
	}
	else if (type == 3)
	{
		//书写器预留
		rightAlignAct->setChecked(true);
		mgr->setHorizonAlignType(BaseConstants::Direction::RIGHT);
	}
}
void ToolBarClass::setToolBarStatus(StrContentData& strContenData)
{
	//还原状态
	boldAct->setCheckable(false);
	italicAct->setChecked(false);
	underlineAct->setChecked(false);
	superscriptAct->setChecked(false);
	subscriptAct->setChecked(false);
	leftAlignAct->setChecked(false);
	rightAlignAct->setChecked(false);
	centerAct->setChecked(false);
	comboFont->setCurrentFont(QFont());
	comboSize->setCurrentIndex(0);

 	boldAct->setChecked(strContenData.getBold());
 	italicAct->setChecked(strContenData.getItalic());
	char underlineType = strContenData.getUnderLineType();
	if (underlineType == UIConstants::LineType::SINGLE_LINE)
		underlineAct->setChecked(true);
	else
		underlineAct->setChecked(false);
	// 上下标类型
	char flowType = strContenData.getFlowType();
	// 设置上标
	if (flowType == BaseConstants::Direction::UP)
		superscriptAct->setChecked(true);
	else
		superscriptAct->setChecked(false);
	// 设置下标
	if (flowType == BaseConstants::Direction::DOWN)
		subscriptAct->setChecked(true);
	else
		subscriptAct->setChecked(false);
	UIInputManager * mgr = pObject->getUIInputMgr();
	char alignType = mgr->getHorizonAlignType();
	if (alignType == BaseConstants::Direction::LEFT)
		leftAlignAct->setChecked(true);
	else
		leftAlignAct->setChecked(false);
	if (alignType == BaseConstants::Direction::MIDDLE)
		centerAct->setChecked(true);
	else
		centerAct->setChecked(false);
	if (alignType == BaseConstants::Direction::RIGHT)
		rightAlignAct->setChecked(true);
	else
		rightAlignAct->setChecked(false);

	//字体
 	comboFont->setCurrentFont(strContenData.getFamily());

	//字体大小
	int fontSize = strContenData.getPointSize();
	switch (fontSize)
	{
	case 6:
		comboSize->setCurrentIndex(0);
	case 7:
		comboSize->setCurrentIndex(1);
	case 8:
		comboSize->setCurrentIndex(2);
	case 10:
		comboSize->setCurrentIndex(3);
	case 11:
		comboSize->setCurrentIndex(4);
	case 12:
		comboSize->setCurrentIndex(5);
	case 14:
		comboSize->setCurrentIndex(6);
	case 16:
		comboSize->setCurrentIndex(7);
	case 18:
		comboSize->setCurrentIndex(8);
	case 20:
		comboSize->setCurrentIndex(9);
	case 22:
		comboSize->setCurrentIndex(11);
	case 24:
		comboSize->setCurrentIndex(12);
	case 26:
		comboSize->setCurrentIndex(13);
	case 28:
		comboSize->setCurrentIndex(14);
	case 36:
		comboSize->setCurrentIndex(15);
	case 48:
		comboSize->setCurrentIndex(16);
	default:
		break;
	}
}
void ToolBarClass::createColorTb(QToolButton* tb)
{
	const QColor colors[2][5] =
	{
		{ QColor(0, 0, 0,255), QColor(192,0,0,255), QColor(255,0,0,255), QColor(255,192,0,255), QColor(255,255,0,255), },
		{ QColor(146,208,80,255), QColor(0,176,240,255), QColor(0,122,192,255), QColor(194,194,194,255), QColor(255,255,255,255) }
	};

	tb->setPopupMode(QToolButton::MenuButtonPopup);

	QGridLayout *pGridLayout = new QGridLayout;
	pGridLayout->setAlignment(Qt::AlignCenter);
	pGridLayout->setContentsMargins(2, 2, 2, 2);

	int actionSeed = 0;//用于标志每一个action
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			QAction *action = new QAction(this);
			action->setData(colors[i][j]);
			action->setIcon(createColorIcon(colors[i][j]));
			if (tb->objectName() == "textColor")
			{
				connect(action, SIGNAL(triggered()), this, SLOT(setTextColor()));
			}
			else
			{
				connect(action, SIGNAL(triggered()), this, SLOT(setBackgroundColor()));
			}
			action->setProperty("actionSeed", actionSeed);
			actionSeed++;
			QToolButton *pBtnColor = new QToolButton();
			pBtnColor->setFixedSize(QSize(16, 16));
			pBtnColor->setAutoRaise(false);
			pBtnColor->setDefaultAction(action);
			pGridLayout->addWidget(pBtnColor, i, j);
		}
	}
	
	QMenu *colorMenu = new QMenu(this);
	colorMenu->setLayout(pGridLayout);
	tb->setMenu(colorMenu);
}
QIcon ToolBarClass::createColorIcon(QColor color)
{
	QPixmap pixmap(16, 16);
	QPainter painter(&pixmap);
	painter.setPen(Qt::NoPen);
	painter.fillRect(QRect(0, 0, 16, 16), color);
	return QIcon(pixmap);
}
void ToolBarClass::setTextColor()
{
	QAction* action = dynamic_cast<QAction*>(sender());
	QToolButton* tb = dynamic_cast<QToolButton*>(sender());
	QColor color;
	if (action)
	{
		color = action->data().value<QColor>();
		int actionSeed = action->property("actionSeed").toInt();
		setTbtextColorIcon(actionSeed);
		mTbTextColor->menu()->close();
		mTbTextColor->setProperty("textColor", QColor(color));
	}
	else if (tb)
	{
		QVariant var = mTbTextColor->property("textColor");
		color = mTbTextColor->property("textColor").value<QColor>();
	}
	if (color.isValid())
	{
		int cloNum = BaseMath::exchangeQColorToColor(color);
		UIInputManager * mgr = pObject->getUIInputMgr();
		mgr->setFontColor(cloNum);
	}
		
}
void ToolBarClass::setBackgroundColor()
{
	QAction* action = dynamic_cast<QAction*>(sender());
	QToolButton* tb = dynamic_cast<QToolButton*>(sender());

	QColor color;
	if (action)
	{
		color = action->data().value<QColor>();
		int actionSeed = action->property("actionSeed").toInt();
		setTbBackgroundColorIcon(actionSeed);
		mTbTextBackgroundColor->menu()->close();
		mTbTextBackgroundColor->setProperty("backgroundColor", QColor(color));
	}
	else if (tb)
	{
		QVariant var = mTbTextBackgroundColor->property("backgroundColor");
		color = mTbTextBackgroundColor->property("backgroundColor").value<QColor>();
	}
	if (color.isValid())
	{
		int cloNum = BaseMath::exchangeQColorToColor(color);
		UIInputManager * mgr = pObject->getUIInputMgr();
		mgr->setBackgroundColor(cloNum);
	}
	
}
void ToolBarClass::setTbtextColorIcon(int actionSeed)
{
	switch (actionSeed)
	{
	case 0:
		mTbTextColor->setIcon(QIcon(":/res/Res/MenuIcon/textcolor/textcolor_black.png"));
		break;
	case 1:
		mTbTextColor->setIcon(QIcon(":/res/Res/MenuIcon/textcolor/textcolor_deepred.png"));
		break;
	case 2:
		mTbTextColor->setIcon(QIcon(":/res/Res/MenuIcon/textcolor/textcolor_red.png"));
		break;
	case 3:
		mTbTextColor->setIcon(QIcon(":/res/Res/MenuIcon/textcolor/textcolor_deepyellow.png"));
		break;
	case 4:
		mTbTextColor->setIcon(QIcon(":/res/Res/MenuIcon/textcolor/textcolor_yellow.png"));
		break;
	case 5:
		mTbTextColor->setIcon(QIcon(":/res/Res/MenuIcon/textcolor/textcolor_green.png"));
		break;
	case 6:
		mTbTextColor->setIcon(QIcon(":/res/Res/MenuIcon/textcolor/textcolor_blue.png"));
		break;
	case 7:
		mTbTextColor->setIcon(QIcon(":/res/Res/MenuIcon/textcolor/textcolor_deepblue.png"));
		break;
	case 8:
		mTbTextColor->setIcon(QIcon(":/res/Res/MenuIcon/textcolor/textcolor_gray.png"));
		break;
	case 9:
		mTbTextColor->setIcon(QIcon(":/res/Res/MenuIcon/textcolor/textcolor_white.png"));
		break;
	default:
		mTbTextColor->setIcon(QIcon(":/res/Res/MenuIcon/textcolor/textcolor_black.png"));
		break;
	}
}
void ToolBarClass::setTbBackgroundColorIcon(int actionSeed)
{
	switch (actionSeed)
	{
	case 0:
		mTbTextBackgroundColor->setIcon(QIcon(":/res/Res/MenuIcon/backgroundcolor/backgroundcolor_black.png"));
		break;
	case 1:
		mTbTextBackgroundColor->setIcon(QIcon(":/res/Res/MenuIcon/backgroundcolor/backgroundcolor_deepred.png"));
		break;
	case 2:
		mTbTextBackgroundColor->setIcon(QIcon(":/res/Res/MenuIcon/backgroundcolor/backgroundcolor_red.png"));
		break;
	case 3:
		mTbTextBackgroundColor->setIcon(QIcon(":/res/Res/MenuIcon/backgroundcolor/backgroundcolor_deepyellow.png"));
		break;
	case 4:
		mTbTextBackgroundColor->setIcon(QIcon(":/res/Res/MenuIcon/backgroundcolor/backgroundcolor_yellow.png"));
		break;
	case 5:
		mTbTextBackgroundColor->setIcon(QIcon(":/res/Res/MenuIcon/backgroundcolor/backgroundcolor_green.png"));
		break;
	case 6:
		mTbTextBackgroundColor->setIcon(QIcon(":/res/Res/MenuIcon/backgroundcolor/backgroundcolor_blue.png"));
		break;
	case 7:
		mTbTextBackgroundColor->setIcon(QIcon(":/res/Res/MenuIcon/backgroundcolor/backgroundcolor_deepblue.png"));
		break;
	case 8:
		mTbTextBackgroundColor->setIcon(QIcon(":/res/Res/MenuIcon/backgroundcolor/backgroundcolor_gray.png"));
		break;
	case 9:
		mTbTextBackgroundColor->setIcon(QIcon(":/res/Res/MenuIcon/backgroundcolor/backgroundcolor_white.png"));
		break;
	default:
		mTbTextBackgroundColor->setIcon(QIcon(":/res/Res/MenuIcon/backgroundcolor/backgroundcolor_black.png"));
		break;
	}
}

void ToolBarClass::setSomeActionsEnable(bool isEnabled)
{
	fileToolBar->setEnabled(isEnabled);
	fontToolBar->setEnabled(isEnabled);
	alignToolBar->setEnabled(isEnabled);
	comboToolBar->setEnabled(isEnabled);
	saveToolBar->setEnabled(isEnabled);
	m_pUpDownAlignToolBar->setEnabled(isEnabled);
#ifdef  _CHAO_YANG_
	submitToolBar->setEnabled(isEnabled);
#endif
	exitAct->setEnabled(isEnabled);
}

void ToolBarClass::submitMdc()
{
	if (MessageBox::showQueryMessageBox("提交", "是否提交给上级医生，确认提交？") == QMessageBox::Yes)
	{
		saveFile();
		QString id = TabWidgetCollection::getInstance()->getCurrentWidgetId();
		QTreeWidgetItem *item = NULL;
		item = TabWidgetCollection::getInstance()->getTreeItem(id);
		QVariant var = item->data(0, Qt::UserRole);
		QStringList list = var.toStringList();
		if (list.size() == 4)
		{
			id = list[0];
		}
		MainViewContainer* mainView_ = MainViewContainer::getInstance();
		mainView_->submitMdc(id);
	}
}
