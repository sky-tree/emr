#include <QDebug>
#include <QWidget>
#include <QDateTime>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringList>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QTreeWidget>
#include <QFile>
#include <qlabel.h>
#include <QFileDialog>
#include <QString>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QDataStream>
#include <QTextCodec>
#include <QList>
#include <QMap>
#include <QColorDialog>

#include "./views/MainViewContainer.h"
#include "commonWidgets/TabWidgetCollection.h"
#include "dataModel/metaData.h"
#include "views/pharmacyapp.h"
#include "views/propertySettingDlgs/ParagraphSettingPopup.h"
#include "views/propertySettingDlgs/tablepropertydlg.h"
#include "views/propertySettingDlgs/CellSplitDlg.h"
#include "views/continueprintdlg.h"
#include "views/propertySettingDlgs/pagesettingdlg.h"
#include "dataModel/OperateData/DataBaseControl.h"
#include "EMR/output/HtmlDocument.h"

#include "globalvariable.h"
#include "BaseSystem.h"
#include "MyAppEntrance.h"
#include "WriterManager.h"
#include "OperateMgr.h"
#include "UIInputManager.h"
#include "Cursor.h"
#include "TempleteTypeEnumeration.h"
#include "AuthorityConstants.h"
#include "BaseIOTools.h"
#include "BaseConstants.h"
#include "toolbarclass.h"
#include "renamepopup.h"
#include "MessageBox.h"
#include "StrConstants.h"
#include "emrversion.h"
#include "UIConstants.h"
#include "adminLogin.h"

#pragma execution_character_set("utf-8")

#define CREATEACTION(objName, name, tip) \
	objName = new QAction(tr(name), this); \
	objName->setStatusTip(tr(tip));

MainViewContainer* MainViewContainer::m_pInstance = NULL;

MainViewContainer::MainViewContainer(QWidget *parent) :
QMainWindow(parent), m_pDbManager(NULL), m_pDataBaseControl(NULL), m_bIsAllTempAdd(false),
m_bHideOrDisplayTopPageNumber(false), m_bHideOrDisplayBottomPageNumber(false), m_bIsAdminUser(false)
{
	initHospitalTitle();
	m_pInstance = this;
	m_pDeleteTimer = new QTimer(this);
	connect(m_pDeleteTimer, SIGNAL(timeout()), this, SLOT(deleteAppEntrance()));
	m_pListAppEntrance = new QList<MyAppEntrance *>();
	m_pTableSettingMenu = NULL;
	m_bIsClose = false;
	m_bIsClickCanccel = false;
	m_pAddTemplateId = "";
	m_strSpecialId = "";
	m_strFlag = "";
	m_strIsPhysician = "false";
	if (m_pDataBaseControl == NULL)
	{
		m_pDataBaseControl = new DataBaseControl(NULL);
		QStringList listHis = QApplication::instance()->arguments();
		if (listHis.size() >= 2)
		{
			QString str = listHis[1];

			QStringList list1 = str.split("§");
			m_pDataBaseControl->initData(list1, m_wirteAth, m_stlMdcAth);
			m_strFlag = list1[4];
			m_strIsPhysician = list1[5];
		}
		else //测试使用
		{
			//住院号，医生ID，医生科室，超级用户，模板/病历，主管医生
//			QString str = "0120171200001§9999§0054§false§1§true";//test1 admin
//			QString str = "0120171200003§1387§0054§false§1§true";//test1 许雅芳
//			QString str = "0120171200100§4306§0054§false§1§true";//睢县
			QString str = "0120171200100§9999§0054§false§1§true";//睢县
//			QString str = "0120171200003§0748§0054§false§1§true";//test1 徐佳丽
//			QString str = "0120180100127§4010§0010§false§1§true";//潮阳 

			QStringList list1 = str.split("§");
			m_pDataBaseControl->initData(list1, m_wirteAth, m_stlMdcAth);
			m_strFlag = list1[4];
			m_strIsPhysician = list1[5];
		}
	}
	m_bIsSupperUser = m_pDataBaseControl->m_docutorData->isSupperUser;
	m_strTreeStyle = "QHeaderView::section{background-color:rgb(137,203,235);\
	font:16px;color:white;padding-left:6px;border-style:solid;min-height:10ex; }\
	QTreeWidget{font:14px;}QTreeWidget{background-color:rgb(242,249,255)}";
	createAllTreeNode();
	createActions();
	createMenus();
	createAllLayout();
	createConnectSignalSlot();
	setStyle();
	setMenuEnable(false);
	setMenuStatus(false);
	this->setWindowIcon(QIcon(":/res/Res/MenuIcon/case.png"));
	TabWidgetCollection::getInstance()->setViewContainer(this);
	//His过来的数据
	setIsTemplateVisable(m_strFlag);
	if (m_strFlag == "0")
	{
		this->hide();
		templateSet();
	}
	this->setWindowFlags(this->windowFlags()& ~Qt::WindowMinimizeButtonHint);
	this->showMaximized();
	mSaveDataIsFromRightMenu = false;
}

void MainViewContainer::initHospitalTitle()
{
	QString exePath = QCoreApplication::applicationDirPath();
	QFile file(exePath + "/hospitalConfig.txt");
	if (!file.open((QFile::ReadOnly | QIODevice::Text)))
	{
		qDebug("read hospitalTitle.txt error");
		return;
	}

	QTextCodec *code = QTextCodec::codecForName("GBK");
	QTextStream rstream(&file);
	rstream.setCodec(code);
	QVector<QString> QVstr_;
	while (rstream.atEnd() == 0)
	{
		QString linestr;
		linestr = rstream.readLine();
		QVstr_.append(linestr);
	}
	file.close();
	QString hospitalName;
	for (int i = 0; i < QVstr_.size(); i++)
	{
		int index_ = QVstr_[i].indexOf("hospitalTitle=");
		if (!(index_ < 0))
		{
			QString str_ = "hospitalTitle=";
			hospitalName = QVstr_[i].mid(str_.size());
		}
	}
	StrConstants::DEFAULT_HOSPITAL_TITLE = hospitalName;
}


QTreeWidget* MainViewContainer::getMainMenuTreeWidget()
{
	return m_pMainMenuTreeWidget;
}

QTreeWidget* MainViewContainer::getMainmenuTemplateWidget()
{
	return m_pMainMenutempleteTreeWidget;
}
DataBaseControl* MainViewContainer::getDataBaseControl()
{
	return m_pDataBaseControl;
}

void MainViewContainer::setStyle()
{
	QString menuStyle = "QMenu {"
		"background-color: white;"
		"padding: 1px;"  //缩小菜单项四个方向的padding
		"min-height:8ex;"
		"font:14px;"
		" }"
		"QMenu::item{"
		"background-color: transparent;"
		" }"
		" QMenu::item:selected{"
		"background-color: rgb(234, 243, 253);"
		"color: black;"
		" }";
	QString menubarStyle = "QMenuBar {"
		"background-color:rgb(137, 203, 235);"
		" width: 55px;"
		"font:14px;"
		"}"
		"QMenuBar::item {"
		"background: transparent;"
		"border-width: 2px;"
		"border-style: solid;"
		"}"

		"QMenuBar::item:selected {" 
		"background-color:rgb(185, 227, 247);"
		"color:black;"
		"border-color: rgb(185, 227, 247);"
		"}"
		"QMenuBar::item:pressed {"
		"background-color:rgb(157, 218, 246);"
		"border-color: rgb(157, 218, 246);"
		" }";
	this->setStyleSheet(menuStyle + menubarStyle);
}

MainViewContainer::~MainViewContainer()
{
	if (m_pDeleteTimer)
	{
		delete m_pDeleteTimer;
		m_pDeleteTimer = NULL;
	}
	if (m_pListAppEntrance)
	{
		if (m_pListAppEntrance->size() > 0)
		{
			MyAppEntrance  * tempAction;
			for (int i = 0; i < m_pListAppEntrance->size(); i++)
			{
				tempAction = m_pListAppEntrance->at(i);
				if (!tempAction)
				{
					continue;
				}
				delete tempAction;
			}
			m_pListAppEntrance->clear();
		}
		delete m_pListAppEntrance;
		m_pListAppEntrance = NULL;
	}
	if (m_pDataBaseControl)
	{
		delete m_pDataBaseControl;
		m_pDataBaseControl = NULL;
	}


	if (DataBaseControl::m_dbManager)
	{
		delete DataBaseControl::m_dbManager;
		DataBaseControl::m_dbManager = NULL;
	}
	if (DataBaseControl::m_docutorData)

	{
		delete DataBaseControl::m_docutorData;
		DataBaseControl::m_docutorData = NULL;
	}
	if (DataBaseControl::m_operatorMedical)
	{
		delete DataBaseControl::m_operatorMedical;
		DataBaseControl::m_operatorMedical = NULL;
	}
	if (DataBaseControl::m_patientData)
	{
		delete DataBaseControl::m_patientData;
		DataBaseControl::m_patientData = NULL;
	}
	if (DataBaseControl::m_operatorTemplate)
	{
		delete DataBaseControl::m_operatorTemplate;
		DataBaseControl::m_operatorTemplate = NULL;
	}
	TabWidgetCollection::getInstance()->destory();

	destory();
}

void MainViewContainer::createAllLayout()
{
	m_pRightVGroupBox = NULL;
	m_pRightLayout = NULL;
	//加载顶部布局效果界面
	createTopHorGroupBox();
	createTopHorMsgGroupBox();
	createLeftVerGroupBox();
	//添加TabWidgets空间

	createRightVerticalGroupBox(TabWidgetCollection::getInstance());
	createMainHorGroupBox();

	/****QMainWindow 加载layout 要特殊处理**********/
	QWidget *widget = new QWidget();
	this->setCentralWidget(widget);
	m_pAllLayout = new QVBoxLayout();
	m_pAllLayout->setMargin(0);
	m_pAllLayout->setSpacing(0);
	/*********************************************/
	/*添加部件*/
	//m_pAllLayout->addWidget(m_pTopHGroupBox);
	m_pAllLayout->addWidget(m_pTopHMsgGroupBox);
	m_pAllLayout->addWidget(m_pMainHGroupBox);
	//添加所有布局
	centralWidget()->setLayout(m_pAllLayout);
	//this->setWindowTitle(tr("电子病历"));
}

/*创建树节点*/
#define createTreeNode(stringName,treeNode) \
	userModuleName.clear(); \
	userModuleName.append(stringName); \
	userModuleRoot = NULL; \
	userModuleRoot = new QTreeWidgetItem(treeNode, userModuleName);
void MainViewContainer::setTemplateData(QTreeWidgetItem *Item)
{
	if (Item == NULL)return;
	QStringList userModuleName;

	QTreeWidgetItem *userModuleRoot = NULL;
	if (Item->text(0) == "基本元素")
	{
		createTreeNode("录入提示", Item)
		createTreeNode("单选", Item)
		createTreeNode("多项", Item)
		createTreeNode("有无选择", Item)
		createTreeNode("单选（勾选型）", Item)
		createTreeNode("多选（勾选型）", Item)
		createTreeNode("格式化录入", Item)
		createTreeNode("固定文本", Item)
		createTreeNode("公式", Item)
		createTreeNode("表格", Item)
	}
	if (Item->text(0) == "患者基本信息")
	{

		m_pModel = NULL;
		QString sql = "select * from macro_element";
		m_pModel = &m_pDataBaseControl->getQueryData(sql);
		int rows = m_pModel->rowCount();
		for (int i = 0; i < m_pModel->rowCount(); i++)
		{
			QSqlRecord record = m_pModel->record(i);
			QString name = record.value("MACRO_NAME").toString();
			QString id = record.value("MACRO_ID").toString();
			createTreeNode(name, Item);
			QVariant var;
			var.setValue(id);
			userModuleRoot->setData(0, Qt::UserRole, var);
		}
	}
	//by caokewen 2017/5/10 delete memory
	delete m_pModel;
	m_pModel = NULL;
}
void MainViewContainer::createAllTreeNode()
{
	//创建模板时的标签
	QStringList userModuleName;
	QTreeWidgetItem *userModuleRoot = NULL;
	m_pMainMenuRightTreeWidget = new treeWidget();
	m_pMainMenuRightTreeWidget->installEventFilter(this);
	m_pMainMenuRightTreeWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_pMainMenuRightTreeWidget->setColumnCount(1);
	m_pMainMenuRightTreeWidget->setMaximumWidth(300);
	m_pMainMenuRightTreeWidget->setHeaderHidden(false);
	m_pMainMenuRightTreeWidget->setHeaderLabel("工具箱");
	createTreeNode("患者基本信息", m_pMainMenuRightTreeWidget)
	setTemplateData(userModuleRoot);

	createTreeNode("基本元素", m_pMainMenuRightTreeWidget)
		//添加基本元素
	setTemplateData(userModuleRoot);
	//createTreeNode("自定义", m_pMainMenuRightTreeWidget)
	m_pMainMenuRightTreeWidget->setEnabled(false);
	m_pMainMenuRightTreeWidget->setVisible(false);
	m_pMainMenuRightTreeWidget->setStyleSheet(m_strTreeStyle);

	//创建病历
	m_pMainMenuTreeWidget = new QTreeWidget();
	m_pMainMenuTreeWidget->installEventFilter(this);
	m_pMainMenuTreeWidget->setColumnCount(1);
	m_pMainMenuTreeWidget->setHeaderHidden(false);
	m_pMainMenuTreeWidget->setHeaderLabel("病历列表");
	m_pMainMenuTreeWidget->setStyleSheet(m_strTreeStyle);
	//创建病历的结点
	m_pMainMenuBottomTreeWidget = new QTreeWidget(this);
	m_pMainMenuBottomTreeWidget->setColumnCount(1);
	m_pMainMenuBottomTreeWidget->setHeaderHidden(true);
	createTreeNode("主诉", m_pMainMenuBottomTreeWidget)
	createTreeNode("现病史", m_pMainMenuBottomTreeWidget)
	createTreeNode("既往史", m_pMainMenuBottomTreeWidget)
	createTreeNode("个人史", m_pMainMenuBottomTreeWidget)
	createTreeNode("家族史", m_pMainMenuBottomTreeWidget)
	createTreeNode("体格检查", m_pMainMenuBottomTreeWidget)
	m_pMainMenuBottomTreeWidget->setVisible(false);
	m_pMainMenuBottomTreeWidget->setEnabled(false);
	m_pMainMenutempleteTreeWidget = new QTreeWidget();
	m_pMainMenutempleteTreeWidget->installEventFilter(this);
	m_pMainMenutempleteTreeWidget->setColumnCount(1);
	m_pMainMenutempleteTreeWidget->setHeaderHidden(false);
	m_pMainMenutempleteTreeWidget->setHeaderLabel("模板列表");
	m_pMainMenutempleteTreeWidget->setVisible(false);
	m_pMainMenutempleteTreeWidget->setEnabled(false);
	m_pMainMenutempleteTreeWidget->setStyleSheet(m_strTreeStyle);
}


void MainViewContainer::loadTemplateData(QTreeWidget *widget, QString tableName, QMap<QString, QString>&map)
{
	widget->clear();
	map.clear();
	m_pModel = NULL;
	QStringList userModuleName;
	QTreeWidgetItem *userModuleRoot = NULL;
	QString sql = "select * from " + tableName + " order by CLASSIFYCODE";
	QSqlTableModel* model1 = NULL;
	m_pModel = &m_pDataBaseControl->getQueryData(sql);
	if (tableName == "MEDICALRECORDS_TEMPLATE_TYPE")
	{
		model1 = m_pDataBaseControl->getTableModel("", "MEDICALRECORDS_TEMPLATE_LIST");
	}
	else
	{
		model1 = m_pDataBaseControl->getTableModel("", "MEDICALRECORDS_LIST");
	}
	int rows = m_pModel->rowCount();
	for (int i = 0; i < m_pModel->rowCount(); i++)
	{
		QSqlRecord record = m_pModel->record(i);
		QString name = record.value("NAME").toString();
		QString classifyCode = record.value("CLASSIFYCODE").toString();
		QString nameClass = "(" + classifyCode + ")" + name;
		if (map.contains(name) == false)
		{
			map.insert(nameClass, classifyCode);
		}
		createTreeNode(nameClass, widget)
			//测试使用,加载模板的数据
#pragma region //模板列表的加载 
			if (tableName == "MEDICALRECORDS_TEMPLATE_TYPE")
			{
			model1->select();
			QMap<QString, operatorTemplateRight>* map = m_pDataBaseControl->m_operatorTemplate;
			QString sql1;
			if ( map->contains("3") && (map->value("3").addTemplate == true) )
			{
				sql1 = sql1 = " CLASSIFYCODE='" + classifyCode + "'";
				m_bIsAllTempAdd = true;
			}
			else
			{
				sql1 = "CLASSIFYCODE='" + classifyCode + "' and ( department_code='" + DataBaseControl::m_docutorData->dept_code + "' or department_code='0000')";
			}
			model1->setFilter(sql1);
			int nodeCount = model1->rowCount();
			int serialNumber = 0;//节点的序号
			for (int j = 0; j < model1->rowCount(); j++)
			{

				QSqlRecord sqlRecord = model1->record(j);
				QString   nodeName = sqlRecord.value("NAME").toString();
				QString   dataId = sqlRecord.value("ID").toString();
				QString   nodeCode = sqlRecord.value("CLASSIFYCODE").toString();
				QString   createName = "创建人:" + sqlRecord.value("CREATE_NAME").toString();
				if (nodeCode == classifyCode)
				{
					QStringList nodeModuleName;
					nodeModuleName.clear();
					nodeModuleName.append(nodeName);
					QTreeWidgetItem *childUserModuleRoot = NULL;
					childUserModuleRoot = new QTreeWidgetItem(userModuleRoot, nodeModuleName);
					childUserModuleRoot->setToolTip(0, createName);
					QVariant var;
					QStringList list;
					serialNumber += 1;
					QString isModify = "2";
					list << dataId << QString::number(serialNumber) << "2";
					var.setValue(list);
					childUserModuleRoot->setData(0, Qt::UserRole, var);
				}
			}
			}
#pragma endregion
#pragma region 病历列表的加载
			else if (tableName == "MEDICALRECORDS_TYPE")
			{
				model1->select();
				
				QString  sql1 = "CLASSIFYCODE='" + classifyCode + "'AND inp_no='" + m_pDataBaseControl->m_patientData->inp_NO + "' order by cast( SERIAL_NUMBER as int )";
//				QString  sql1 = "CLASSIFYCODE='" + classifyCode + "'AND PATIENT_ID='" + m_pDataBaseControl->m_patientData->id + "' order by SERIAL_NUMBER";
				model1->setFilter(sql1);
				int nodeCount = model1->rowCount();
				QString serialNumber = "0";//节点的序号
				for (int j = 0; j < model1->rowCount(); j++)
				{
					QSqlRecord sqlRecord = model1->record(j);
					QString     nodeName = sqlRecord.value("NAME").toString();
					QString      id = sqlRecord.value("ID").toString();
					QString     dataId = sqlRecord.value("DATA").toString();
					QString   serialNumber = sqlRecord.value("SERIAL_NUMBER").toString();
					QString   nodeCode = sqlRecord.value("CLASSIFYCODE").toString();
					QString   creatNameAndTime = "创建人:" + sqlRecord.value("FOUNDER_NAME").toString() + " 创建时间:" + sqlRecord.value("CREATE_TIME").toString().replace("T", " ");
					if (nodeCode == classifyCode)
					{
						QVariant var;
						QStringList list;
						QString isModify = "2";//0：新增，1，修改，2：未改动，从数据库出来的数据
						list << dataId << serialNumber << isModify;
						if (classifyCode == progressNote)
						{
							list.clear();
							list << id << serialNumber << isModify << dataId;
						}
						var.setValue(list);
						QStringList nodeModuleName;
						nodeModuleName.clear();
						nodeModuleName.append(nodeName);
						QTreeWidgetItem *childUserModuleRoot = NULL;
						childUserModuleRoot = new QTreeWidgetItem(userModuleRoot, nodeModuleName);
						childUserModuleRoot->setToolTip(0, creatNameAndTime);
						if (list.size() == 4)
						{
							dataId = list[0];
						}
						auto it = m_stlMdcAth.find(dataId);
						if (it != m_stlMdcAth.end())
						{
							MdcAuthority mdcAth_ = *it;
							bool ok_;
							int nAuthor = mdcAth_.mdcAuthor.toInt(&ok_, 10);
							if (!ok_)
							{
								nAuthor = -1;
							}
							modifyTreeItem(childUserModuleRoot, nAuthor);
						}
						childUserModuleRoot->setData(0, Qt::UserRole, var);
					}
				}
			}
#pragma endregion
	}
	m_pModel->clear();
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = NULL;
	}
	if (model1)
	{
		delete model1;
		model1 = NULL;
	}

}
// void MainViewContainer::loadNodeData()
// {
// }
/*主界面的右边主容器刷新*/
void MainViewContainer::refreshRightContainer(QGroupBox *groupBox)
{
	createRightVerticalGroupBox(groupBox);
	m_pMainLayout->addWidget(m_pRightVGroupBox);
}

void MainViewContainer::createMainHorGroupBox()
{
	QRect screetRect = QApplication::desktop()->screenGeometry();
	int h = screetRect.height();
	int w = screetRect.width();
	if (w <= 1440)
	{
		int rsetW = (w - 1020) / 2;
		m_pLeftVGroupBox->setMaximumWidth(rsetW);
		m_pMainMenuRightTreeWidget->setMaximumWidth(rsetW);

	}

	m_pMainHGroupBox = new QGroupBox();
	m_pMainHGroupBox->setObjectName("m_pMainHGroupBox");
	this->setMinimumSize(w, h);
	m_pMainLayout = new QHBoxLayout();
	m_pMainLayout->setMargin(0);

	m_pSplitter = new QSplitter(Qt::Horizontal, this);
	m_pSplitter1 = new QSplitter(Qt::Horizontal, this);
	m_pSplitter->addWidget(m_pLeftVGroupBox);
	m_pRightVGroupBox->setLayout(m_pRightLayout);
	m_pSplitter->addWidget(m_pRightVGroupBox);

	m_pSplitter1->addWidget(m_pSplitter);
	m_pSplitter1->addWidget(m_pMainMenuRightTreeWidget);
	m_pMainLayout->addWidget(m_pSplitter1);
	m_pMainHGroupBox->setLayout(m_pMainLayout);
}


void MainViewContainer::createTopHorGroupBox()
{

	m_pTopHGroupBox = new QGroupBox();
	m_pTopHGroupBox->setMaximumHeight(50);

	QString topTitleStr = tr("电子病历相关信息");
	m_pTopTitleLabel = new QLabel(topTitleStr);
	QFont titleFont;
	titleFont.setPointSize(20);
	m_pTopTitleLabel->setFont(titleFont);
	m_pTopTitleLabel->setFixedWidth(m_pTopTitleLabel->fontMetrics().width(topTitleStr));
	m_pTopTitleLabel->setAttribute(Qt::WA_TranslucentBackground, true);
}


void MainViewContainer::createTopHorMsgGroupBox()
{
	m_pTopHMsgGroupBox = new QGroupBox();
	m_pTopHMsgGroupBox->setMaximumHeight(30);
	m_pTopMsgLayout = new QHBoxLayout();


	m_pTopMsgTitle = new QLabel();
	/********设置Title字体大小********/
	QFont titleFont;

	QPalette fontColor;
	fontColor.setColor(QPalette::WindowText, Qt::black);
	m_pTopMsgTitle->setPalette(fontColor);
	m_pTopMsgTitle->setStyleSheet("font:14px");
	//设置m_pTopMsgTitlePic背景色透明
	m_pTopMsgTitle->setAttribute(Qt::WA_TranslucentBackground, true);
	/*************************************************/

	/********设置m_pTopMsgTitle的图片ICON********/
	m_pTopMsgTitlePic = new QLabel();
	m_pPic = new QPixmap(":/Res/images/m_pTopMsgTitlePic.png");
	m_pTopMsgTitlePic->setFixedSize(m_pPic->width(), m_pPic->height());

	m_pTopMsgTitlePic->setAttribute(Qt::WA_TranslucentBackground, true);
	/*************************************************/

	/********加载各个Widget部件********/
	m_pTopMsgLayout->addWidget(m_pTopMsgTitlePic);
	m_pTopMsgLayout->addWidget(m_pTopMsgTitle);
	m_pTopMsgLayout->setMargin(0);
	m_pTopHMsgGroupBox->setLayout(m_pTopMsgLayout);
	/*************************************************/

	/********设置顶部容器的背景渐变色样式********/
	m_pTopHMsgGroupBox->setStyleSheet("background-color:rgb(190,190,190)");
	m_pTopHMsgGroupBox->setAutoFillBackground(true);
	/*************************************************/
}

void MainViewContainer::createLeftVerGroupBox()
{
	m_pLeftVGroupBox = new QGroupBox();
	m_pLeftVGroupBox->setStyleSheet("QGroupBox{border:none;}");
	m_pLeftVGroupBox->setMaximumWidth(300);
	m_pLeftVGroupBox->setObjectName("leftVgpBox");
	QString mainStyle = "QWidget#leftVgpBox{background-color: rgb(255, 255, 255)}";

	m_pLeftLayout = new QVBoxLayout();
	m_pLeftLayout->setMargin(0);
	m_pLeftLayout->addWidget(m_pMainMenuTreeWidget);
	m_pLeftLayout->addWidget(m_pMainMenutempleteTreeWidget);
	m_pLeftVGroupBox->setLayout(m_pLeftLayout);
}
//读取XML文件
void MainViewContainer::ReadXml(QString pPath)
{
	QFile file(pPath);
	if (file.open(QIODevice::ReadOnly))
	{
		QDomDocument dom("WCM");
		if (dom.setContent(&file))
		{
			m_pMainMenuTreeWidget->clear();
			QDomElement docElem = dom.documentElement();
			listDom(docElem, NULL);
		}
	}
	file.close();
}
//循环递归添加树节点
void MainViewContainer::listDom(QDomElement& docElem, QTreeWidgetItem * pItem)
{
	QDomNode node = docElem.firstChild();
	if (node.toElement().isNull())
	{
		pItem->setText(1, docElem.text());
	}
	while (!node.isNull())
	{
		QDomElement element = node.toElement();
		if (!element.isNull())
		{
			QTreeWidgetItem *item;
			if (pItem)
			{
				item = new QTreeWidgetItem(pItem);
			}
			else
			{
				item = new QTreeWidgetItem(m_pMainMenuTreeWidget);
			}
			item->setText(0, element.tagName());
			listDom(element, item);
			if (pItem)
			{
				pItem->addChild(item);
			}
			else
			{
				m_pMainMenuTreeWidget->addTopLevelItem(item);
			}
		}

		node = node.nextSibling();
	}
	return;
}
void MainViewContainer::createRightVerticalGroupBox(QGroupBox *groupBox)
{
	m_pRightVGroupBox = groupBox;
}


QGroupBox* MainViewContainer::getUserCheckView()
{
	return NULL;
}

//创建控件相关的信号和槽
void MainViewContainer::createConnectSignalSlot()
{
	//病历节点的信号设置
	connect(m_pMainMenuTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this,
		SLOT(showSelectMainMenuItem(QTreeWidgetItem*, int)));
	m_pMainMenuTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_pMainMenuTreeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(rightClick(QPoint)));
	//模板的设置
	connect(m_pMainMenutempleteTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this,
		SLOT(showSelectMainMenuItem(QTreeWidgetItem*, int)));
	m_pMainMenutempleteTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_pMainMenutempleteTreeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(rightClickTemplate(QPoint)));
}
//点击书写助手
// void MainViewContainer::writeHelpClick()
// {
// 	QMenu *popMenu = new QMenu(this);//定义一个右键弹出菜单
// 	popMenu->addAction(icd_10Code);
// 	popMenu->addAction(drugDataBase);
// 	popMenu->addAction(diagnosisTreatment);
// 	popMenu->addAction(commonDictionary);
// 	popMenu->exec(QCursor::pos());
// }
void MainViewContainer::setMedicalPopMenu(QTreeWidgetItem* pCurItem, QMenu *pMenu)
{
	if (pCurItem == NULL)return;
	QString strPar = pCurItem->text(0).mid(1, 2);
	if (DataBaseControl::m_operatorMedical->contains(strPar))		//通过CLASSIFYCODE判断权限
	{
		operatorMedicalRight oMR = (*(DataBaseControl::m_operatorMedical))[strPar];
		if (oMR.addTemplate)
		{
			pMenu->setEnabled(true);
		}
		else
		{
			pMenu->setEnabled(false);
		}
	}
	else
	{
		pMenu->setEnabled(false);
	}
	if (strPar != "03" && strPar != "04")
	{
		if (DataBaseControl::m_docutorData->user_ser_code & 0x0001)
		{
			pMenu->setEnabled(true);
		}
		else
		{
			pMenu->setEnabled(false);
		}
	}
	else if (strPar == "04")
	{
		if (DataBaseControl::m_docutorData->user_ser_code & 0x0010)
		{
			pMenu->setEnabled(true);
		}
		else
		{
			pMenu->setEnabled(false);
		}
	}
//	pMenu->setEnabled(true);
}
void MainViewContainer::setTemplatePopMenu(QTreeWidgetItem* pCurItem, QMenu* pMenu)
{
	if (pCurItem == NULL)//表明点击的是跟节点
	{
		if (DataBaseControl::m_operatorTemplate->count() == 0)
		{
			pMenu->setEnabled(false);
		}
	}
	else
	{
		QVariant var = pCurItem->data(0, Qt::UserRole);
		QStringList list = var.toStringList();
		QString   templateId = list[0];
		QString sql1 = "select DEPARTMENT_CODE from medicalrecords_template_list where ID='" + templateId + "'";
		QString departmentCode = m_pDataBaseControl->getId(sql1);
		bool flag;
		if (m_bIsAllTempAdd)
		{
			flag = true;
		}
		else
		{
			flag = (departmentCode == m_pDataBaseControl->m_docutorData->dept_code);
		}
		if (flag || departmentCode == "0000")
		{
			QString sql = "select TYPE from medicalrecords_template_list where ID='" + templateId + "'";
			//借用取ID的函数取模板的类型,由于数据库的数据不对应，所以需要调整（公共模板和个人模板）
			QString type = m_pDataBaseControl->getId(sql);
			QString mappingType;
			if (type == "1")
			{

				mappingType = "3";
			}
			else if (type == "2"){ mappingType = "2"; }
			else if (type == "3"){ mappingType = "1"; }
			if (DataBaseControl::m_operatorTemplate->contains(mappingType))
			{
				operatorTemplateRight oTR = (*(DataBaseControl::m_operatorTemplate))[mappingType];
				m_pModifyEmr->setEnabled(oTR.modifyTemplate);
				m_pDeleteEmr->setEnabled(oTR.deleteTemplate);
				if (oTR.modifyTemplate || oTR.deleteTemplate)
				{
					pMenu->setEnabled(true);
				}
				if (oTR.modifyTemplate)
				{
					m_pSaveEmr->setEnabled(true);
				}
				else
				{
					m_pSaveEmr->setEnabled(false);
				}
			}
			else
			{
				pMenu->setEnabled(false);
			}

		}
		else
		{
			pMenu->setEnabled(false);
		}
	}

}
//右键点击树节点
void MainViewContainer::rightClick(const QPoint& pos)
{
	QTreeWidgetItem* curItem = m_pMainMenuTreeWidget->itemAt(pos);  //获取当前被点击的节点
	if (curItem == NULL)return;           //这种情况是右键的位置不在treeItem的范围内，即在空白位置右击
	QVariant var = curItem->data(0, Qt::UserRole);
	QMenu *popMenu = new QMenu(this);//定义一个右键弹出菜单
	if (curItem->parent() == NULL)
	{
		//点击根节点的时候
		popMenu->addAction(m_pAddEmr);//往菜单内添加QAction   该action在前面用设计器定义了
		//判断是否是主治医生，是否是在病程记录下
		//只有主治医生才能新增其他的病历，其他医生只能新增病程记录下的病历
		QString strPar = curItem->text(0).mid(1, 2);
		//病程记录，主治医生，超级用户

		if (m_bIsSupperUser)
		{
			popMenu->setEnabled(true);
		}
		else
		{
			setMedicalPopMenu(curItem, popMenu);
		}
	}
	else
	{
		//点击病历节点的时候,根据权限判定是否能编辑此节点
		if (curItem == NULL || curItem->parent() == NULL)return;
		popMenu->setEnabled(true);
		popMenu->addAction(m_pModifyEmr);
		popMenu->addAction(m_pSaveEmr);
		popMenu->addAction(m_pDeleteEmr);
		popMenu->addAction(m_pRename);
		m_pModifyEmr->setEnabled(true);
		m_pSaveEmr->setEnabled(true);
		m_pDeleteEmr->setEnabled(true);
		m_pRename->setEnabled(true);
		if (m_bIsAdminUser)
		{
			popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
			return;//管理权限，修改，删，保存，重命名
		}

		isAuthority(curItem, false);

		QVariant var = curItem->data(0, Qt::UserRole);//item  病历ID,排序ID，状态，病程记录的特殊ID
		QStringList list = var.toStringList();
		QString strId = list[0];
#ifndef _CHAO_YANG_
		setPopAuthority(curItem, popMenu);
#else
		auto it = m_stlMdcAth.find(strId);
		MdcAuthority mdcAth;
		if (it == m_stlMdcAth.end())
		{
			return;
		}
		else
		{
			mdcAth = *it;
		}
		if (mdcAth.mdcAuthor.size() == 0)
		{
			setPopAuthority(curItem, popMenu);
		}
		else
		{
			setPopAuthority(popMenu, mdcAth);
		}
#endif

	}
	popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
}
//只有本人编辑的病历才能进行编辑
bool MainViewContainer::isAuthority(QTreeWidgetItem *pItem, bool isTemplate)
{
	if (pItem == NULL || pItem->parent() == NULL)return false;
	QVariant var = pItem->data(0, Qt::UserRole);//item  病历ID,排序ID，状态，病程记录的特殊ID
	QStringList list = var.toStringList();
	QString id = m_pDataBaseControl->m_docutorData->user_id;
	QString strId = list[0];
	QString state = list[2];

	QString sql = "select FOUNDER_ID from medicalrecords_list where ID='" + strId + "'";//创建人ID
	QString sql1 = "select CREATE_ID from medicalrecords_template_list where ID='" + strId + "'";//模板创建人ID
	if (!isTemplate)
	{
		strId = m_pDataBaseControl->getId(sql);
	}
	else
	{
		strId = m_pDataBaseControl->getId(sql1);
	}
	if (strId.trimmed() == id.trimmed())
	{
		m_bIsAuthor = true;
	}
	else
	{
		m_bIsAuthor = false;
	}
	//state:0 新增的，1，表示修改状态，2已经保存的状态
	if (/*strId.trimmed() == id.trimmed() ||*/ state.trimmed() != "2")//非保存的或本人的给予权限
	{
		return true;
	}
	return false;
}

bool MainViewContainer::getPermissionsFormPid(QString pid)
{
	bool bRet = false;
	auto it = m_stlMdcAth.find(pid);
	MdcAuthority mdcAth;
	if (it == m_stlMdcAth.end())
	{
		return bRet;
	}
	else
	{
		mdcAth = *it;
	}
	bool ok_;
	int nMdcAth_ = mdcAth.mdcAuthor.toInt(&ok_, 10);
	switch (nMdcAth_)
	{
	case 0:
		if (m_bIsAuthor)
		{
			bRet = true;
		}
		break;
	case 1:
		if (m_pDataBaseControl->m_docutorData->user_id == m_wirteAth.RhtDctr)
		{
			bRet = true;
		}
		break;
	case 2:
	case 3:
		if (m_pDataBaseControl->m_docutorData->user_id == m_wirteAth.ChfDctr)
		{
			bRet = true;
		}
		break;
	default:
		break;
	}
	return bRet;
}


void MainViewContainer::setPopAuthority(QMenu *popMenu, MdcAuthority mdcAth)
{
	if (mdcAth.mdcAuthor.trimmed() == "0")
	{
		if (m_bIsAuthor == true)
		{
			popMenu->setEnabled(true);
		}
		else
		{
			popMenu->setEnabled(false);
		}
	}
	else if (mdcAth.mdcAuthor.trimmed() == "1")
	{
		if (m_pDataBaseControl->m_docutorData->user_id == m_wirteAth.RhtDctr)
		{
			popMenu->setEnabled(true);
		}
		else
		{
			popMenu->setEnabled(false);
		}
		m_pDeleteEmr->setEnabled(false);
	}
	else if (mdcAth.mdcAuthor.trimmed() == "2")
	{
		if (m_pDataBaseControl->m_docutorData->user_id == m_wirteAth.ChfDctr)
		{
			popMenu->setEnabled(true);
		}
		else
		{
			popMenu->setEnabled(false);
		}
		m_pDeleteEmr->setEnabled(false);
	}
	else if (mdcAth.mdcAuthor.trimmed() == "3")
	{
		if (m_pDataBaseControl->m_docutorData->user_id == m_wirteAth.ChfDctr)
		{
			popMenu->setEnabled(true);
		}
		else
		{
			popMenu->setEnabled(true);
		}
		m_pDeleteEmr->setEnabled(false);
	}
}


void MainViewContainer::setPopAuthority(QTreeWidgetItem* curItem, QMenu *popMenu)
{
	bool state = isAuthority(curItem, false);
	if (m_bIsSupperUser) { state = true; }
	QString strPar = curItem->parent()->text(0).mid(1, 2);
	popMenu->setEnabled(state);
	popMenu->addAction(m_pModifyEmr);
	popMenu->addAction(m_pSaveEmr);
	popMenu->addAction(m_pDeleteEmr);
	popMenu->addAction(m_pRename);
	if (state == true)
	{
		m_pModifyEmr->setEnabled(true);
		m_pSaveEmr->setEnabled(true);
		m_pDeleteEmr->setEnabled(true);
		m_pRename->setEnabled(true);
	}
	else
	{
		m_pRename->setEnabled(false);
	}
	//非本人状态下的操作权限
	if (state == false && strPar.trimmed() == "03"
		&& DataBaseControl::m_operatorMedical->contains(strPar))
	{
		operatorMedicalRight oMR = (*(DataBaseControl::m_operatorMedical))[strPar];
		m_pModifyEmr->setEnabled(oMR.modifyTemplate);
		m_pDeleteEmr->setEnabled(oMR.deleteTemplate);
		if (oMR.modifyTemplate == true)
		{
			m_pSaveEmr->setEnabled(true);
		}
		else
		{
			m_pSaveEmr->setEnabled(false);
		}
		if (oMR.addTemplate || oMR.deleteTemplate || oMR.modifyTemplate)
		{
			popMenu->setEnabled(true);
		}
	}
	else if (strPar.trimmed() != "03")
	{
		if (m_bIsAuthor == true)
		{
			popMenu->setEnabled(true);
			m_pModifyEmr->setEnabled(true);
			m_pSaveEmr->setEnabled(true);
			m_pDeleteEmr->setEnabled(true);
			m_pRename->setEnabled(true);
		}
		else
		{
			popMenu->setEnabled(false);
			m_pModifyEmr->setEnabled(true);
			m_pSaveEmr->setEnabled(true);
			m_pDeleteEmr->setEnabled(true);
			m_pRename->setEnabled(true);
		}
	}
}


void MainViewContainer::rightClickTemplate(const QPoint& pos)
{
	QTreeWidgetItem* curItem = m_pMainMenutempleteTreeWidget->itemAt(pos);  //获取当前被点击的节点
	if (curItem == NULL)return;           //这种情况是右键的位置不在treeItem的范围内，即在空白位置右击
	QVariant var = curItem->data(0, Qt::UserRole);
	QMenu *popMenu = new QMenu(this);//定义一个右键弹出菜单
	if (curItem->parent() == NULL)
	{
		popMenu->addAction(m_pAddTemplate);//往菜单内添加QAction   该action在前面用设计器定义了
		setTemplatePopMenu(NULL, popMenu);
	}
	else
	{
		bool state = isAuthority(curItem, true);
		if (m_bIsSupperUser)
		{
			state = true;
		}

		popMenu->setEnabled(state);
		popMenu->addAction(m_pModifyTemplate);
		popMenu->addAction(m_pSaveTemplate);
		popMenu->addAction(m_pDeleteTemplate);
		popMenu->addAction(m_pRename);
		//表明非本人和超级用户
		if (!state)
		{
			setTemplatePopMenu(curItem, popMenu);
			m_pRename->setEnabled(false);
		}
		else
		{
			m_pAddTemplate->setEnabled(true);
			m_pModifyTemplate->setEnabled(true);
			m_pSaveTemplate->setEnabled(true);
			m_pDeleteTemplate->setEnabled(true);
			m_pRename->setEnabled(true);
		}

	}
	popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
}

/*菜单命令选择器*/
void MainViewContainer::showSelectMainMenuItem(QTreeWidgetItem *item, int column)
{
	QTreeWidgetItem *parent = item->parent();
	if (parent == NULL)return;

	//item->setFlags(item->flags()&~(Qt::ItemIsEditable));
	QString strPar = parent->text(0).mid(1, 2);
	/*获取id*/
	QVariant var = item->data(0, Qt::UserRole);//itemData: classifycode 序号 状态（新增0，修改1，保存2（跟数据库无关）），（病程记录还有specialid	）
	QStringList varToString = var.toStringList();
	if (varToString.count() == 0)return;
	QString str = varToString[0];//ID
	setPrompts(str);
	if (strPar == progressNote&&m_pMainMenuTreeWidget->isVisible() == true)//病历状态下的病程记录
	{
		str = varToString[3];
		m_strSpecialId = varToString[3];
	}
	int childIndex = parent->indexOfChild(item);
	const QString menuItems = item->text(column);

	//判断当前点击的结点是在模板下还是在病历下面
	if (m_pMainMenuTreeWidget->isVisible() == true)
	{
		LoadViews(menuItems, false, item, str);//item->text,不是模板,item,specialID
		QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
		if (widget == NULL)return;
		QObject * pAction = this->sender();
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		UIInputManager* mgr = appEntrance->getUIInputMgr();
		if (strPar == progressNote)
		{
			mgr->closeAllLevelOneTempleteWriteAutority(true);
 			if (varToString[2] != "2")
 			{
				appEntrance->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT);
				appEntrance->getUIInputMgr()->closeAllLevelOneTempleteWriteAutorityExcept(true, varToString[0]);
				setMenuStatus(true);
				appEntrance->getToolbar()->setSomeActionsEnable(true);
 			}
			else
			{
				setMenuStatus(false);
				appEntrance->getToolbar()->setSomeActionsEnable(false);
			}
			//appEntrance->getUIInputMgr()->closeAllLevelOneTempleteWriteAutorityExcept(varToString[3]);
		}
		else
		{
 			if (varToString[2] == "2")
 			{
				appEntrance->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
				setMenuStatus(false);
				appEntrance->getToolbar()->setSomeActionsEnable(false);
 			}
			else
			{
				setMenuStatus(true);
				appEntrance->getToolbar()->setSomeActionsEnable(true);
			}
		}
	}
	else
	{
		LoadViews(menuItems, true, item, str);
		QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
		if (widget == NULL)return;
		//QObject * pAction = this->sender();
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		UIInputManager* mgr = appEntrance->getUIInputMgr();
		if (varToString[2] == "2")
		{
			mgr->setFileAuthority(true, AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
			setMenuStatus(false);
			appEntrance->getToolbar()->setSomeActionsEnable(false);
		}
		else
		{
			setMenuStatus(true);
			appEntrance->getToolbar()->setSomeActionsEnable(true);
		}
	}
}
/*动态加载书写器,pViews为模板的全路径*/
bool testBarray = false;
void MainViewContainer::LoadViews(QString pViews, bool isTemplate, QTreeWidgetItem *pItem, QString str)
{
	PatientData *data = m_pDataBaseControl->m_patientData;//病人的所有数据
	MyAppEntrance *myApp = NULL;
	QStringList list = pItem->data(0, Qt::UserRole).toStringList();//item携带的数据
	if (isTemplate == true)//模板
	{
		if (TabWidgetCollection::getInstance()->isExist(str) == false)//不存在此页面
		{
			MyAppEntrance * test = new MyAppEntrance(pViews, AuthorityConstants::FileType::TEMPLETE, AuthorityConstants::Authority::WRITE_ALL);
			//MyAppEntrance * test = new MyAppEntrance(AuthorityConstants::FileType::TEMPLETE, 0, AuthorityConstants::Authority::WRITE_ALL, pViews);
			myApp = test;
//			QByteArray ba = m_pDataBaseControl->getMedicalData(str, true);//数据来源可疑
			bool bIscompress_ = false;
			QByteArray ba = m_pDataBaseControl->getMedicalData(str, true, bIscompress_);//数据来源可疑
			if (bIscompress_)
			{
				test->initInCompressedData(&ba, NULL);
			}
			else
			{
				test->init(&ba, NULL);
			}
			test->getUIInputMgr()->setTitleCanWrite(true);
			test->getUIInputMgr()->setBottomCanWrite(true);
			test->setFocus();
			this->setFocusPolicy(Qt::NoFocus);
			Cursor::initCursor(test);
			QWidget * test1 = static_cast<QWidget*>(test);
			TabWidgetCollection::getInstance()->addTabWidget(test1, pViews, isTemplate, str, pItem);
		}
		else
		{
			TabWidgetCollection::getInstance()->addTabWidget(NULL, pViews, isTemplate, str, pItem);
			QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
			TabWidgetCollection::getInstance()->modifyMedicalName(pViews);
			if (widget == NULL)return;
			MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
			myApp = appEntrance;
			appEntrance->getUIInputMgr()->setTitleCanWrite(true);
			appEntrance->getUIInputMgr()->setBottomCanWrite(true);
		}
		if (myApp != NULL)
		{
			if (list[2] != "2")//非保存
			{
				myApp->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::WRITE_ALL);
				setMenuStatus(true);
				myApp->getToolbar()->setSomeActionsEnable(true);
				myApp->getUIInputMgr()->setTitleCanWrite(true);
				myApp->getUIInputMgr()->setBottomCanWrite(true);
			}
			else
			{
				myApp->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
				setMenuStatus(false);
				myApp->getToolbar()->setSomeActionsEnable(false);
				myApp->getUIInputMgr()->setTitleCanWrite(true);
				myApp->getUIInputMgr()->setBottomCanWrite(true);
			}
			
		}

	}
	else//病历
	{
		//先判断是否是病程记录
		QString strPar = pItem->parent()->text(0).mid(1, 2);
		QString medicalId = "0";
		if (strPar == progressNote)//表明是病程记录（CLASSIFYCODE判断）
		{
			str = m_strSpecialId;
			medicalId = list[0];
		}
		if (TabWidgetCollection::getInstance()->isExist(str) == false)
		{
			MyAppEntrance * test = NULL;
			test = new MyAppEntrance(pViews, AuthorityConstants::FileType::MEDICAL_RECORDS, AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
			int length = 0;
			if (m_pAddTemplateId == "")//如果为空，表明不是新增而是双击进入病历的编辑
			{
				//QByteArray ba = m_pDataBaseControl->getMedicalData(str, false);
				bool bIscompress_ = false;
				QByteArray ba = m_pDataBaseControl->getMedicalData(str, false, bIscompress_);
				
				length = ba.size();
				if (ba.size() > 0)
				{
					//test->setMacroData(data);s
					//test->saveByteArrayBeforOpen(ba);
					if (bIscompress_)
					{
						test->insertRecordInCompressed(&ba, medicalId, data, true);
					}
					else
					{
						test->insertRecord(&ba, medicalId, data, true);
					}
					/*if (bIscompress_)
					{
						test->insertRecordInCompressed(&ba, medicalId, NULL, true);
					}
					else
					{
						test->insertRecord(&ba, medicalId, NULL, true);
					}*/
					if (strPar == progressNote)
					{
						test->getUIInputMgr()->closeAllLevelOneTempleteWriteAutority(true);
						//dingweide jiekou he daima 
						test->getUIInputMgr()->gotoMedicalRecord(true, list[0]);
					}
				}
				else
				{
					QMessageBox::information(NULL, "文件大小有误", pViews + " 文件大小= " + QString::number(ba.size()), QMessageBox::Yes);
					return;
				}
			}
			else
			{
				//新增的病历
			//	QByteArray ba = m_pDataBaseControl->getMedicalData(m_pAddTemplateId, true);
				bool bIscompress_ = false;
				QByteArray ba = m_pDataBaseControl->getMedicalData(m_pAddTemplateId, true, bIscompress_);
				length = ba.size();

				if (list[0] != str&&strPar == progressNote)
				{
					//test->setMacroData(data);

				//	QByteArray baDatabase = m_pDataBaseControl->getMedicalData(str, false);
					bool bIscompressTmp_ = false;
					QByteArray baDatabase = m_pDataBaseControl->getMedicalData(str, false, bIscompressTmp_);
					if (bIscompressTmp_)
					{
						test->initInCompressedData(&baDatabase, data);
					}
					else
					{
						test->init(&baDatabase, data);
					}
					test->getUIInputMgr()->closeAllLevelOneTempleteWriteAutority(true);
					if (bIscompress_)
					{
						test->getUIInputMgr()->loadOtherMedicalRecordInCompressedData(true, list[0], &ba);//可以直接定位
					}
					else
					{
						test->getUIInputMgr()->loadOtherMedicalRecord(true, list[0], &ba);//可以直接定位
					}
					test->getUIInputMgr()->closeAllLevelOneTempleteWriteAutorityExcept(true, list[0]);
					test->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT);
					setMenuStatus(true);
					test->getToolbar()->setSomeActionsEnable(true);
				}
				else
				{
					if (bIscompress_)
					{
						test->insertRecordInCompressed(&ba, m_strSpecialId, data, true);
					}
					else
					{
						test->insertRecord(&ba, m_strSpecialId, data, true);
					}
					test->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT);
				}
				m_pAddTemplateId = "";
			}
			test->setFocus();
			Cursor::initCursor(test);
			this->setFocusPolicy(Qt::NoFocus);
			QWidget * test1 = static_cast<QWidget*>(test);
			QStringList listValue = pItem->data(0, Qt::UserRole).toStringList();
			int testLength = listValue.length();
			TabWidgetCollection::getInstance()->addTabWidget(test1, pViews, isTemplate, str, pItem);
		}
		else
		{
			TabWidgetCollection::getInstance()->addTabWidget(NULL, pViews, isTemplate, str, pItem);
			QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
			TabWidgetCollection::getInstance()->modifyMedicalName(pViews);
			if (widget == NULL)return;
			MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
			//书写器预留，病程记录
			int length = 0;
			QByteArray ba;
			if (m_pAddTemplateId == "")//如果为空，表明不是新增而是双击进入病历的编辑
			{
				if (strPar == progressNote)
				{
					appEntrance->getUIInputMgr()->gotoMedicalRecord(true, list[0]);
				}
			}
			else
			{
				bool bIsCompress_ = false;
				//ba = m_pDataBaseControl->getMedicalData(m_pAddTemplateId, true);
				ba = m_pDataBaseControl->getMedicalData(m_pAddTemplateId, true, bIsCompress_);
				length = ba.size();
				if (ba.size() > 0)
				{
					m_pAddTemplateId = "";
					if (bIsCompress_)
					{
						appEntrance->getUIInputMgr()->loadOtherMedicalRecordInCompressedData(true, list[0], &ba);//可以直接定位
					}
					else
					{
						appEntrance->getUIInputMgr()->loadOtherMedicalRecord(true, list[0], &ba);//可以直接定位
					}
				
					//appEntrance->setMacroData(data);
					appEntrance->getUIInputMgr()->closeAllLevelOneTempleteWriteAutorityExcept(true, list[0]);
					appEntrance->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT);
					setMenuStatus(true);
					appEntrance->getToolbar()->setSomeActionsEnable(true);
				}
			}

		}

	}
}
/*创建菜单栏*/
void MainViewContainer::createMenus()
{
	m_pFileMenu = menuBar()->addMenu(tr("文件  "));

	//m_pFileMenu->addAction(m_pNewAct);
	m_pFileMenu->addAction(m_pOpenAct);
	m_pFileMenu->addSeparator();//分割线

	m_pFileMenu->addAction(m_pSaveAct);
	m_pFileMenu->addAction(m_pExportAct);
	m_pFileExportMenu = m_pFileMenu->addMenu("导出");
	m_pFileExportMenu->addAction(m_pTxtAct);
	m_pFileExportMenu->addAction(m_pHtmlAct);
	m_pFileMenu->addSeparator();
	m_pFileMenu->setEnabled(true);
// 	m_pFileSubmit = m_pFileMenu->addMenu("提交病历");
// 	m_pFileSubmit->addAction(m_pSubmitCurrent);
// 	m_pFileSubmit->addAction(m_pSubmitMultipl);
	//Edit Menu
	m_pEditMenu = menuBar()->addMenu(tr("编辑  "));
	m_pEditMenu->addAction(m_pUndoAct);
	m_pEditMenu->addAction(m_pRedoAct);
	m_pEditMenu->addAction(m_pCopyAct);
	m_pEditMenu->addAction(m_pPasteAct);
	m_pEditMenu->addAction(m_pCutAct);


	m_pFormatMenu = menuBar()->addMenu(tr("格式"));
	m_pFontMenu = m_pFormatMenu->addMenu("字体");

	m_pFontMenu->addAction(m_pBoldAct);
	m_pFontMenu->addAction(m_pItalicAct);
	m_pFontMenu->addAction(m_pUnderlineAct);
	m_pFontMenu->addSeparator();
	m_pFontMenu->addAction(m_pSuperscriptAct);
	m_pFontMenu->addAction(m_pSubscriptAct);
	m_pFontMenu->addSeparator();
	m_pFontMenu->addAction(m_pLeftAlignAct);
	m_pFontMenu->addAction(m_pCenterAct);
	m_pFontMenu->addAction(m_pRightAlignAct);

	m_pParagraphMenu = menuBar()->addMenu(tr("段落"));


	m_pTextSpaceMenu = m_pParagraphMenu->addMenu("文本行距");
	m_pTextSpaceMenu->addAction(m_pTextSpacintAct1);
	m_pTextSpaceMenu->addAction(m_pTextSpacintAct2);
	m_pTextSpaceMenu->addAction(m_pTextSpacintAct3);
	m_pParagraphMenu->addAction(m_pParagraphAttrubuteAct);

	menuBar()->addSeparator();

	m_pPageMenu = menuBar()->addMenu(tr("页面"));
//	m_pLayoutAct->setCheckable(true);
	m_pLineNumAct->setCheckable(true);
//	m_pPageMenu->addAction(m_pLayoutAct);
	m_pPageMenu->addAction(m_pLineNumAct);
	m_pPageMenu->addAction(m_pHideOrDisplayHeaderAct);
	m_pHideOrDisplayHeaderAct->setCheckable(true);
	m_pPageMenu->addAction(m_pHideOrDisplayFooterAct);
	m_pHideOrDisplayFooterAct->setCheckable(true);
	m_pPageMenu->addAction(m_pHideOrDisplayTopPageNumber);
	m_pPageMenu->addAction(m_pHideOrDisplayBottomPageNumber);
	m_pPageMenu->addAction(m_pGridLineAct);
	m_pGridLineAct->setCheckable(true);
	m_pPageMenu->addAction(m_bPageAct);
	m_bHideOrDisplayTopPageNumber = true;
	m_bHideOrDisplayBottomPageNumber = true;
	m_pHideOrDisplayTopPageNumber->setCheckable(m_bHideOrDisplayTopPageNumber);
	m_pHideOrDisplayBottomPageNumber->setCheckable(m_bHideOrDisplayBottomPageNumber);
	m_pPageMenu->setMinimumWidth(40);
	menuBar()->addSeparator();

	m_pInsertMenu = menuBar()->addMenu(tr("插入"));

	m_pInsertMenu->addAction(m_pInsertImageAct);
	m_pInsertMenu->addAction(m_pInsertSpecialSymbolAct);
 	m_pInsertMenu->addAction(m_pPageBreaksAct);
 	m_pInsertMenu->addAction(m_pLineAct);
	
	menuBar()->addSeparator();

	m_pPrintMenu = menuBar()->addMenu(tr("打印"));


	m_pPrintMenu->addAction(m_pChoosePrint);//打印预览
	m_pPrintMenu->addAction(m_pGeneralPrint);
	m_pPrintMenu->addAction(m_pContinuousPrint);
	menuBar()->addSeparator();

	m_pTemplateMenu = menuBar()->addMenu(tr("模板"));
	m_pTemplateMenu->addAction(m_pTemplateAct);
	menuBar()->addSeparator();

	m_pTableMenu = menuBar()->addMenu(tr("表格"));
	m_pTableMenu->addAction(m_pInsertTableAct);
	m_pInsertcellTable = m_pTableMenu->addMenu(tr("插入"));
	m_pInsertcellTable->addAction(m_pInsertLeftAct);
	m_pInsertcellTable->addAction(m_pInsertRightAct);
	m_pInsertcellTable->addAction(m_pInsertAboveAct);
	m_pInsertcellTable->addAction(m_pInsertBlowAct);

	m_pHelpMenu = menuBar()->addMenu(tr("帮助"));
	m_pHelpMenu->addAction(m_pAboutAct);
	m_pHelpMenu->addAction(m_pAdmin);
	



	m_pTableMenu->addAction(m_pMergeCellsAct);
	m_pCelltableMenu = m_pTableMenu->addMenu("拆分单元格");
	m_pCelltableMenu->addAction(m_pSplitCellsAct);
	m_pCelltableMenu->addAction(m_pSplitTableAct);
	m_pCelltableMenu->addAction(m_pMultiplySplitCellAct);

	m_pDeletecellTable = m_pTableMenu->addMenu(tr("删除"));
	//m_pDeletecellTable->addAction(m_pDeleteCellAct);
	m_pDeletecellTable->addAction(m_pDeleteRowAct);
	m_pDeletecellTable->addAction(m_pDeleteColumnAct);

	m_pTableSettingMenu = m_pTableMenu->addMenu(tr("属性设置"));
	m_pTableSettingMenu->addAction(m_pCellSettingAct);
	m_pTableSettingMenu->addAction(m_pTableSettingAct);


}
void MainViewContainer::setMenuEnable(bool flag)
{
	m_pFontMenu->setEnabled(flag);
	m_pEditMenu->setEnabled(flag);
	m_pFormatMenu->setEnabled(flag);
	m_pParagraphMenu->setEnabled(flag);
	m_pPageMenu->setEnabled(flag);
	m_pTableMenu->setEnabled(flag);
	m_pInsertMenu->setEnabled(flag);
	m_pPrintMenu->setEnabled(flag);
}
/*设置超级用户*/
void MainViewContainer::setSuperUser(bool flag)
{
	m_bIsSupperUser = flag;
}
/*设置进入的是病历编辑还是模板编辑*/
void MainViewContainer::setIsTemplateVisable(QString isVisable)
{
	if (isVisable == "0")
	{
		this->setWindowTitle("病历模板");
		m_pMainMenuTreeWidget->setEnabled(false);
		m_pMainMenuTreeWidget->setVisible(false);
		QString strTitle = m_pTopMsgTitle->text();
		//strTitle += "操作员姓名:" + m_pDataBaseControl->m_docutorData->user_name;
		m_pTopMsgTitle->setText(strTitle);
		m_pMainMenutempleteTreeWidget->setEnabled(true);
		m_pMainMenutempleteTreeWidget->setVisible(true);
		m_pMainMenuRightTreeWidget->setEnabled(true);
		m_pMainMenuRightTreeWidget->setVisible(true);
	}
	else
	{
		this->setWindowTitle("电子病历");
		m_pMainMenuTreeWidget->setEnabled(true);
		m_pMainMenuTreeWidget->setVisible(true);
		QString strTitle = m_pTopMsgTitle->text();
		strTitle = "病人姓名:" + m_pDataBaseControl->m_patientData->name 
			+ "\t\t性别:" + m_pDataBaseControl->m_patientData->sex
			+ "\t年龄:" + m_pDataBaseControl->m_patientData->age 
			+ "\t主诊断:" + m_pDataBaseControl->m_patientData->current_diag_name;
		m_pTopMsgTitle->setText(strTitle);



		m_pMainMenutempleteTreeWidget->setEnabled(false);
		m_pMainMenutempleteTreeWidget->setVisible(false);
		m_pMainMenuRightTreeWidget->setEnabled(false);
		m_pMainMenuRightTreeWidget->setVisible(false);
	}

	//加载数据
	if (m_pMainMenutempleteTreeWidget->isVisible() == true)
	{
		loadTemplateData(m_pMainMenutempleteTreeWidget, "MEDICALRECORDS_TEMPLATE_TYPE", m_mapSqlTemplate);
	}
	else
	{
		loadTemplateData(m_pMainMenuTreeWidget, "MEDICALRECORDS_TYPE", m_mapSql);
	}
}
/*创建操作*/
void MainViewContainer::createActions()
{
//	m_pNewAct = new QAction(tr("新建(&N)"), this);

	m_pOpenAct = new QAction(tr("打开(&O)"), this);
	connect(m_pOpenAct, SIGNAL(triggered(bool)), this, SLOT(fileOpen()));

	m_pSaveAct = new QAction(tr("保存(&S)"), this);
	m_pSaveAct->setIcon(QIcon(":/res/Res/MenuIcon/formRegionSave.png"));
	connect(m_pSaveAct, SIGNAL(triggered(bool)), this, SLOT(fileSave()));
	m_pSaveAct->setEnabled(false);
	m_pSaveAct->setVisible(false);

	m_pExportAct = new QAction(tr("另存为"), this);
	connect(m_pExportAct, SIGNAL(triggered(bool)), this, SLOT(fileSaveAs()));


	m_pPrintAct = new QAction(tr("打印(&P)"), this);

	m_pPrintPreviewAct = new QAction(tr("打印预览"), this);

	m_pExitAct = new QAction(tr("退出(&X)"), this);
	m_pExitAct->setStatusTip(tr("退出应用程序 "));

	m_pUndoAct = new QAction(tr("撤销"), this);
	connect(m_pUndoAct, SIGNAL(triggered(bool)), this, SLOT(doCopyPasteCutUndoRedo()));
	m_pRedoAct = new QAction(tr("恢复"), this);
	connect(m_pRedoAct, SIGNAL(triggered(bool)), this, SLOT(doCopyPasteCutUndoRedo()));

	m_pCutAct = new QAction(tr("剪切"), this);
	connect(m_pCutAct, SIGNAL(triggered(bool)), this, SLOT(doCopyPasteCutUndoRedo()));

	m_pCopyAct = new QAction(tr("复制"), this);
	connect(m_pCopyAct, SIGNAL(triggered(bool)), this, SLOT(doCopyPasteCutUndoRedo()));

	m_pPasteAct = new QAction(tr("粘贴"), this);
	connect(m_pPasteAct, SIGNAL(triggered(bool)), this, SLOT(doCopyPasteCutUndoRedo()));

	m_pBoldAct = new QAction(tr("加粗(&B)"), this);
	QFont bold;
	bold.setBold(true);
	m_pBoldAct->setFont(bold);
	connect(m_pBoldAct, SIGNAL(triggered(bool)), this, SLOT(textBold()));

	m_pItalicAct = new QAction(tr("倾斜(&I)"), this);
	QFont italic;
	italic.setItalic(true);
	m_pItalicAct->setFont(italic);
	connect(m_pItalicAct, SIGNAL(triggered(bool)), this, SLOT(textItalic()));

	m_pUnderlineAct = new QAction(tr("下划线(&U)"), this);
	QFont underline;
	underline.setUnderline(true);
	m_pUnderlineAct->setFont(underline);
	connect(m_pUnderlineAct, SIGNAL(triggered(bool)), this, SLOT(textUnderline()));


	//paragraph
	QActionGroup *grp = new QActionGroup(this);
	if (QApplication::isLeftToRight())
	{
		m_pLeftAlignAct = new QAction(tr("左对齐(&L)"), grp);
		m_pCenterAct = new QAction(tr("居中(&E)"), grp);
		m_pRightAlignAct = new QAction(tr("右对齐(&R)"), grp);

	}
	else
	{
		m_pRightAlignAct = new QAction(tr("右对齐(&R)"), grp);
		m_pCenterAct = new QAction(tr("居中(&E)"), grp);
		m_pLeftAlignAct = new QAction(tr("左对齐(&L)"), grp);
	}
	m_pJustifyAct = new QAction(tr("两端对齐(&J)"), grp);
	m_pCenterAct->setToolTip(tr("居中"));
	m_pCenterAct->setStatusTip(tr("Font is centerAligned"));

	connect(m_pLeftAlignAct, SIGNAL(triggered(bool)), this, SLOT(leftAlignActTriggered()));
	connect(m_pRightAlignAct, SIGNAL(triggered(bool)), this, SLOT(rightAlignActTriggered()));
	connect(m_pCenterAct, SIGNAL(triggered(bool)), this, SLOT(centerAlignActTriggered()));

	//m_pJustifyAct->setCheckable(true);
	m_pJustifyAct->setToolTip(tr("两端对齐"));
	m_pJustifyAct->setStatusTip(tr("Font is justifyAlign and Increase text space according to need"));
	QPixmap pix(16, 16);
	pix.fill(Qt::red);
	m_pColorAct = new QAction(pix, tr("颜色(&C)..."), this);
	m_pColorAct->setToolTip(tr("color"));
	m_pColorAct->setStatusTip(tr("set Text Color"));
	connect(m_pColorAct, SIGNAL(triggered(bool)), this, SLOT(textColor()));

// 	CREATEACTION(m_pFindAct, "查找", "查找内容")
// 		CREATEACTION(m_pReplaceAct, "替换", "替换内容")
// 		CREATEACTION(m_pSelectAllAct, "全选", "全选页面内容")
// 		CREATEACTION(m_pDeleteAct, "删除", "删除所选内容")

// 	CREATEACTION(fontAct, "字体设置", "设置所选内容字体")
// 	CREATEACTION(fontSizeAct, "字号设置", "设置所选内容字号")
	CREATEACTION(m_pSuperscriptAct, "字体上标", "添加上标")
		connect(m_pSuperscriptAct, SIGNAL(triggered(bool)), this, SLOT(superscrip()));
	CREATEACTION(m_pSubscriptAct, "字体下标", "添加下标")
		connect(m_pSubscriptAct, SIGNAL(triggered(bool)), this, SLOT(subscript()));
// 
// 	CREATEACTION(m_pSubIndentationAct, "减少字符间缩进量", "减少字符间缩进量")
// 	CREATEACTION(m_pAddIndentationAct, "增加字符间缩进量", "减少字符间缩进量")
	CREATEACTION(m_pTextSpacintAct1, "1倍", "设置文本行距1倍")
		connect(m_pTextSpacintAct1, SIGNAL(triggered(bool)), this, SLOT(setTextSpace()));
	CREATEACTION(m_pTextSpacintAct2, "1.5倍", "设置文本行距1.5倍")
		connect(m_pTextSpacintAct2, SIGNAL(triggered(bool)), this, SLOT(setTextSpace()));
	CREATEACTION(m_pTextSpacintAct3, "2倍", "设置文本行距2倍")
		connect(m_pTextSpacintAct3, SIGNAL(triggered(bool)), this, SLOT(setTextSpace()));
// 	CREATEACTION(m_pLayoutAct, "整洁显示", "整洁显示")
// 		connect(m_pLayoutAct, SIGNAL(triggered(bool)), this, SLOT(dapper(bool)));
	CREATEACTION(m_bPageAct, "页面设置", "页面设置" )
		connect(m_bPageAct, SIGNAL(triggered(bool)), this, SLOT(onPageActTriggered()));
	CREATEACTION(m_pLineNumAct, "显示/隐藏段落号", "设置显示/隐藏段落号" )
		connect(m_pLineNumAct, SIGNAL(triggered(bool)), this, SLOT(displayLineNumber(bool)));
	CREATEACTION(m_pHideOrDisplayHeaderAct, "显示/隐藏页眉", "显示或者隐藏页眉")
		connect(m_pHideOrDisplayHeaderAct, SIGNAL(triggered(bool)), this, SLOT(displayPageHeader(bool)));
	CREATEACTION(m_pHideOrDisplayFooterAct, "显示/隐藏页脚", "显示或者隐藏页脚")
		connect(m_pHideOrDisplayFooterAct, SIGNAL(triggered(bool)), this, SLOT(displayPageFooter(bool)));
	CREATEACTION(m_pHideOrDisplayTopPageNumber, "显示/隐藏顶部页码", "显示或者顶部隐藏页码")
		connect(m_pHideOrDisplayTopPageNumber, SIGNAL(triggered(bool)), this, SLOT(displayTitlePage(bool)));
	CREATEACTION(m_pHideOrDisplayBottomPageNumber, "显示/隐藏底部页码", "显示或者底部隐藏页码")
		connect(m_pHideOrDisplayBottomPageNumber, SIGNAL(triggered(bool)), this, SLOT(displayBottomPage(bool)));
	CREATEACTION(m_pMergeCellsAct, "合并单元格", "合并单元格")
		connect(m_pMergeCellsAct, SIGNAL(triggered(bool)), this, SLOT(mergeCells()));
	CREATEACTION(m_pSplitCellsAct, "横向拆分", "横向拆分")
		connect(m_pSplitCellsAct, SIGNAL(triggered(bool)), this, SLOT(horSplitCells()));
	CREATEACTION(m_pSplitTableAct, "竖向拆分", "竖向拆分")
		connect(m_pSplitTableAct, SIGNAL(triggered(bool)), this, SLOT(verSplitCells()));

	CREATEACTION(m_pMultiplySplitCellAct, "多行/列拆分", "多行/列拆分")
		connect(m_pMultiplySplitCellAct, SIGNAL(triggered(bool)), this, SLOT(multiplySplitCellTable()));
	CREATEACTION(m_pInsertLeftAct, "从左边插入列", "从左边插入表格")
		connect(m_pInsertLeftAct, SIGNAL(triggered(bool)), this, SLOT(insertLeftTable()));
	CREATEACTION(m_pInsertRightAct, "从右边插入列", "从右边插入表格")
		connect(m_pInsertRightAct, SIGNAL(triggered(bool)), this, SLOT(insertRightTable()));
	CREATEACTION(m_pInsertAboveAct, "从上边插入行", "从上边插入表格")
		connect(m_pInsertAboveAct, SIGNAL(triggered(bool)), this, SLOT(insertAboveTable()));
	CREATEACTION(m_pInsertBlowAct, "从下边插入行", "从下边插入表格")
		connect(m_pInsertBlowAct, SIGNAL(triggered(bool)), this, SLOT(insertBlowTable()));
	CREATEACTION(m_pCellSettingAct, "单元格属性", "设置单元格属性")
		connect(m_pCellSettingAct, SIGNAL(triggered(bool)), this, SLOT(cellSettingTable()));
	CREATEACTION(m_pTableSettingAct, "表格属性", "设置表格属性")
		connect(m_pTableSettingAct, SIGNAL(triggered(bool)), this, SLOT(tableSettingTable()));
	CREATEACTION(m_pDeleteCellAct, "删除单元格", "删除选中单元格")
		connect(m_pDeleteCellAct, SIGNAL(triggered(bool)), this, SLOT(deleteCellTable()));
	CREATEACTION(m_pDeleteRowAct, "删除整行", "删除整行")
		connect(m_pDeleteRowAct, SIGNAL(triggered(bool)), this, SLOT(deleteRowTable()));
	CREATEACTION(m_pDeleteColumnAct, "删除整列", "删除整列")
		connect(m_pDeleteColumnAct, SIGNAL(triggered(bool)), this, SLOT(deleteColumnTable()));

	CREATEACTION(m_pInsertTableAct, "插入表格", "插入表格")
		connect(m_pInsertTableAct, SIGNAL(triggered(bool)), this, SLOT(insertTable()));
	CREATEACTION(m_pInsertImageAct, "图片", "插入图片")
		connect(m_pInsertImageAct, SIGNAL(triggered(bool)), this, SLOT(insertImage()));

	CREATEACTION(m_pInsertSpecialSymbolAct, "特殊符号", "插入图片")
		connect(m_pInsertSpecialSymbolAct, SIGNAL(triggered(bool)), this, SLOT(insertSpecialSymbol()));

	CREATEACTION(m_pPageBreaksAct, "分页符", "分页符")
		connect(m_pPageBreaksAct, SIGNAL(triggered(bool)), this, SLOT(insertPageBreaks()));

	CREATEACTION(m_pLineAct, "横线", "横线")
		connect(m_pLineAct, SIGNAL(triggered(bool)), this, SLOT(insertLine()));

	CREATEACTION(m_pGridLineAct, "显示/隐藏网格线", "显示/隐藏网格线")
		connect(m_pGridLineAct, SIGNAL(triggered(bool)), this, SLOT(insertGridLine(bool)));

	
// 	CREATEACTION(insertHeaderAct, "页眉", "插入页眉")
// 	CREATEACTION(insertFooterAct, "页脚", "插入页脚")
// 	CREATEACTION(insertPageNumAct, "页码", "插入页码")
// 	CREATEACTION(insertFormulaAct, "公式", "插入公式")
// 	CREATEACTION(m_pInsertSymbolAct, "符号", "插入符号")
// 
// 	CREATEACTION(m_pSelectPrintDeviceAct, "选择打印机", "选择打印机")
// 	CREATEACTION(m_pPrintsAct, "打印份数", "打印份数")
// 	CREATEACTION(m_pSelectLineNumAct, "是否选择行号", "是否选择行号")
// 	CREATEACTION(m_pFinishAct, "完成病历", "完成病历")

		//书写助手
// 	CREATEACTION(writeHelp, "书写助手", "书写助手"/*, triggered(bool), writeHelpClick()*/)
// 	connect(writeHelp, SIGNAL(triggered(bool)), this, SLOT(writeHelpClick()));
// 	CREATEACTION(icd_10Code, "icd-10国际编码", "icd-10国际编码")
// 	CREATEACTION(drugDataBase, "药品资料库", "药品资料库")
// 	CREATEACTION(diagnosisTreatment, "常见疾病诊疗规范", "常见疾病诊疗规范")
// 	CREATEACTION(commonDictionary, "常用字典", "常用字典")

		//打印
	CREATEACTION(m_pGeneralPrint, "普通打印", "普通打印")
		connect(m_pGeneralPrint, SIGNAL(triggered(bool)), this, SLOT(generalPrintData()));
	CREATEACTION(m_pContinuousPrint, "续打", "续打")
		connect(m_pContinuousPrint, SIGNAL(triggered(bool)), this, SLOT(continuePrintClicked()));


	CREATEACTION(m_pChoosePrint, "打印预览", "打印预览")
		connect(m_pChoosePrint, SIGNAL(triggered(bool)), this, SLOT(printView()));
	//树节点右键功能
	CREATEACTION(m_pAddEmr, "新增病历", "新增病历")
		connect(m_pAddEmr, SIGNAL(triggered(bool)), this, SLOT(selectmubandlg()));
	CREATEACTION(m_pModifyEmr, "修改病历", "修改病历")
		connect(m_pModifyEmr, SIGNAL(triggered(bool)), this, SLOT(modifyTreeNodeAndTab()));
	CREATEACTION(m_pDeleteEmr, "删除病历", "删除病历")
		connect(m_pDeleteEmr, SIGNAL(triggered(bool)), this, SLOT(deleteTreeNodeAndTab()));

	CREATEACTION(m_pSaveEmr, "保存病历", "保存病历")
		connect(m_pSaveEmr, SIGNAL(triggered(bool)), this, SLOT(saveTreeNodeAnadTab()));

	CREATEACTION(m_pRename, "重命名", "重命名当前节点"/*, triggered(bool), renameTriggered()*/)
		connect(m_pRename, SIGNAL(triggered()), this, SLOT(renameTriggered()));
	//模板设置
	CREATEACTION(m_pTemplateAct, "模板设置", "模板设置")
		connect(m_pTemplateAct, SIGNAL(triggered(bool)), this, SLOT(templateSet()));

	CREATEACTION(m_pAddTemplate, "新增模板", "新增模板")
		connect(m_pAddTemplate, SIGNAL(triggered(bool)), this, SLOT(addTemplateDlg()));
	CREATEACTION(m_pModifyTemplate, "修改模板", "修改模板")
		connect(m_pModifyTemplate, SIGNAL(triggered(bool)), this, SLOT(modifyTreeNodeAndTab()));
	CREATEACTION(m_pDeleteTemplate, "删除模板", "删除模板")
		connect(m_pDeleteTemplate, SIGNAL(triggered(bool)), this, SLOT(deleteTreeNodeAndTab()));

	CREATEACTION(m_pSaveTemplate, "保存模板", "保存模板")
		connect(m_pSaveTemplate, SIGNAL(triggered(bool)), this, SLOT(saveTreeNodeAnadTab()));

	CREATEACTION(m_pAboutAct, "关于", "关于")
		connect(m_pAboutAct, SIGNAL(triggered(bool)), this, SLOT(aboutVersion()));
	CREATEACTION(m_pAdmin, "管理员登录", "管理员登录")
		connect(m_pAdmin, SIGNAL(triggered(bool)), this, SLOT(adminLogin()));
	


	CREATEACTION(m_pParagraphAttrubuteAct, "段落属性", "段落属性")
		connect(m_pParagraphAttrubuteAct, SIGNAL(triggered(bool)), this, SLOT(paragraphSetting()));

	CREATEACTION(m_pTxtAct, "导出纯文本(TXT)格式", "导出纯文本(TXT)格式")
		connect(m_pTxtAct, SIGNAL(triggered(bool)), this, SLOT(exportTxt()));

	CREATEACTION(m_pHtmlAct, "导出HTML格式", "导出HTML格式")
		connect(m_pHtmlAct, SIGNAL(triggered(bool)), this, SLOT(exportHtml()));

	CREATEACTION(m_pSubmitCurrent, "提交当前病历", "提交当前病历")
		connect(m_pSubmitCurrent, SIGNAL(triggered(bool)), this, SLOT(submitCurrent()));

	CREATEACTION(m_pSubmitMultipl, "提交多份病历", "提交多份病历")
		connect(m_pSubmitMultipl, SIGNAL(triggered(bool)), this, SLOT(submitMultipl()));

}
//打印预览
void MainViewContainer::printView()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	mgr->printPreView();
}
//插入特殊符号
void MainViewContainer::insertSpecialSymbol()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::INPUT_PATTICULAR_SIGN);
}

//插入分页符
void MainViewContainer::insertPageBreaks()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->getUIInputMgr()->addEndPageLine();
}

//横线
void MainViewContainer::insertLine()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->getUIInputMgr()->addHorizontalLine();
}

//网格线
void MainViewContainer::insertGridLine(bool stats)
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager* mgr_ = appEntrance->getUIInputMgr();
	if (stats)
	{
		mgr_->setRowBottomLineType(UIConstants::LineType::SINGLE_DOT_LINE);
	}
	else
	{
		mgr_->setRowBottomLineType(BaseConstants::NONE);
	}
	//m_pGridLineAct->setCheckable(stats);
}

//多行列的拆分
void MainViewContainer::multiplySplitCellTable()
{
	CellSplitDlg dlg;
	dlg.show();
	int row = 1;
	int column = 1;
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	if (dlg.exec() == QDialog::Accepted)
	{
		row = dlg.row;
		column = dlg.column;
		appEntrance->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_DEVIDE_H_V + QString::number(row) + ":" + QString::number(column));
	}
}
//表格属性设置
void MainViewContainer::tableSettingTable()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_PROPERTY + QString::number(false));
}
//单元格属性设置
void MainViewContainer::cellSettingTable()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_PROPERTY + QString::number(true));
}
//左边插入列
void MainViewContainer::insertLeftTable()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);

	appEntrance->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_ADD_ROW_OR_COL + QString::number(3));
}
//右边插入列
void MainViewContainer::insertRightTable()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_ADD_ROW_OR_COL + QString::number(4));
}
//上边插入行
void MainViewContainer::insertAboveTable()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_ADD_ROW_OR_COL + QString::number(1));
}
//下边插入行
void MainViewContainer::insertBlowTable()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_ADD_ROW_OR_COL + QString::number(2));
}
void MainViewContainer::deleteCellTable()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	//appEntrance->sendKeyEvent(TempleteTypeEnumeration::EXCEL_SIGN + TempleteTypeEnumeration::EXCEL_ADD_ROW_OR_COL + QString::number(direction));
}
//删除整行
void MainViewContainer::deleteRowTable()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_DEL_ROW_OR_COL + QString::number(true));
}
//删除整列
void MainViewContainer::deleteColumnTable()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->sendKeyEvent(TempleteTypeEnumeration::SISTEM_SIGN + TempleteTypeEnumeration::EXCEL_DEL_ROW_OR_COL + QString::number(false));
}
//水平拆分
void MainViewContainer::horSplitCells()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->sendKeyEvent(TempleteTypeEnumeration::EXCEL_DEVIDE_H);

}
//竖向拆分
void MainViewContainer::verSplitCells()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->sendKeyEvent(TempleteTypeEnumeration::EXCEL_DEVIDE_V);
}
//合并单元格
void MainViewContainer::mergeCells()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->sendKeyEvent(TempleteTypeEnumeration::EXCEL_MERGER);
}
//插入图片
void MainViewContainer::insertImage()
{
	QFileInfo fi;
	QString fileName, filePath;

	QStringList list = QFileDialog::getOpenFileNames(NULL, "打开图片", ".", "*.jpg *.png *.bmp *.jpeg");
	if (list.count() > 0)
	{
		fi = QFileInfo(list.at(0));
		fileName = fi.fileName();
		filePath = fi.absoluteFilePath();
		//书写器预留
		QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
		if (widget == NULL)return;
		QObject * pAction = this->sender();
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		UIInputManager * mgr = appEntrance->getUIInputMgr();
		mgr->addImg(filePath);
	}
}
void MainViewContainer::insertTable()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->addExcel();
}
//显示/隐藏底部页码
void MainViewContainer::displayBottomPage(bool state)
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	if (state == false)
	{
		mgr->setBottomNumViewType(BaseConstants::NONE);
	}
	else
	{
		m_pHideOrDisplayFooterAct->setChecked(true);
		displayPageFooter(true);
		mgr->setBottomNumViewType(BaseConstants::SystemParams::ONLY_VIEW_PAGE_NUMBER);
	}
}
//显示/隐藏顶部页码
void MainViewContainer::displayTitlePage(bool state)
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	if (state == false)
	{
		mgr->setTitleNumViewType(BaseConstants::NONE);
	}
	else
	{
		m_pHideOrDisplayHeaderAct->setChecked(true);
		displayPageHeader(true);
//		mgr->setTitleNumViewType(BaseConstants::SystemParams::VIEW_PAGE_NUMBER_AND_ALL_PAGE_NUM);
		mgr->setTitleNumViewType(BaseConstants::SystemParams::ONLY_VIEW_PAGE_NUMBER);
	}
}
//显示页脚
void MainViewContainer::displayPageFooter(bool state)
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	if (state == false)
	{
		mgr->setBottomView(false);
		//appEntrance->clearPageBottomNum();
		m_pHideOrDisplayBottomPageNumber->setChecked(false);
	}
	else
	{
		mgr->setBottomView(true);
		//appEntrance->setBottomViewType(BaseConstants::ViewType::NORMAL);
		m_pHideOrDisplayBottomPageNumber->setChecked(m_bHideOrDisplayBottomPageNumber);
	}
}
//显示页眉
void MainViewContainer::displayPageHeader(bool state)
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	if (state == false)
	{
		mgr->setTitleView(false);
		//appEntrance->getUIInputMgr()->setTitleView(BaseConstants::NONE);
		//appEntrance->clearPageTitleNum();//页码则也必须清除
		m_pHideOrDisplayTopPageNumber->setChecked(false);
	}
	else
	{
		mgr->setTitleView(true);
		//appEntrance->setTitleViewType(BaseConstants::ViewType::NORMAL);
		m_pHideOrDisplayTopPageNumber->setChecked(m_bHideOrDisplayTopPageNumber);
	}
}

//显示行号
void MainViewContainer::displayLineNumber(bool state)
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	mgr->setParagraphNumView(state);
}
//整洁显示
void MainViewContainer::dapper(bool state)
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->showTidy(state);
	setMenuStatus(!state);
	appEntrance->getToolbar()->setSomeActionsEnable(true);
//*****************************************去掉名字"*"***********************************************//
	if (state == true )
	{
		QString id = TabWidgetCollection::getInstance()->getCurrentWidgetId();
		QTreeWidgetItem* item = TabWidgetCollection::getInstance()->getTreeItem(id);
		QString stringName = item->text(0);
		TabWidgetCollection::getInstance()->saveTabName(stringName);
		QString SaveName = stringName.replace("*", "");
		item->setText(0, SaveName);
	}

}
void MainViewContainer::generalPrintData()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	mgr->doPrint();
}
void MainViewContainer::setTextSpace()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	if (pAction == m_pTextSpacintAct1)//1倍
	{
		mgr->setRowDisTimes(1);
		//appEntrance->setParagraphRowDisH(1);
	}
	else if (pAction == m_pTextSpacintAct2)//1.5倍
	{
		mgr->setRowDisTimes(1.5);
		//appEntrance->setParagraphRowDisH(2);
	}
	else if (pAction == m_pTextSpacintAct3)//2倍
	{
		mgr->setRowDisTimes(2);
		//appEntrance->setParagraphRowDisH(3);
	}

}
void  MainViewContainer::paragraphSetting()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	//mgr->setParagraphIndentParams(0, 0, 0, 0);
	ParagraphSettingPopup paragraphAttr;
	ParagraphData * data = mgr->getFocusParagraphData();
	paragraphAttr.setParagraphData(data);
	if (paragraphAttr.exec() == QDialog::Accepted)
	{
		mgr->setParagraphIndentParams(paragraphAttr.disL, paragraphAttr.disR, paragraphAttr.type, paragraphAttr.indentLeft);
		//appEntrance->setParagraphIndentAbout();
	}
}
//文件保存
void MainViewContainer::fileSave()
{
	m_pDbManager->connectOracle();
	TabWidgetCollection::getInstance()->saveData(false);
}

QString MainViewContainer::FileDialogPath(QString name)
{

	QFileDialog *fileDialog = new QFileDialog(this);//创建一个QFileDialog对象，构造函数中的参数可以有所添加。  
	fileDialog->setWindowTitle(name);//设置文件保存对话框的标题  
	fileDialog->setAcceptMode(QFileDialog::AcceptSave);//设置文件对话框为保存模式  
	fileDialog->setFileMode(QFileDialog::AnyFile);//设置文件对话框弹出的时候显示任何文件，不论是文件夹还是文件  
	fileDialog->setViewMode(QFileDialog::Detail);
	fileDialog->setDirectory(".");
	if (fileDialog->exec() == QDialog::Accepted)
	{
		QString path = fileDialog->selectedFiles()[0];
		return path;
	}
	return "";
}
//文件另存为
void MainViewContainer::fileSaveAs()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QString itemId = TabWidgetCollection::getInstance()->getCurrentWidgetId();
	QTreeWidgetItem *item = TabWidgetCollection::getInstance()->getTreeItem(itemId);
	if (item == NULL)return;
	QString str = "./";
	QTreeWidgetItem *parItem = item->parent();
	QString strPar = parItem->text(0);
	QString strType = strPar.mid(1, 2);
	if (strType == progressNote&&m_pMainMenuTreeWidget->isVisible() == true)
	{
		MessageBox::showErrorMessageBox("提示","病程记录不允许另存为模板!");
		return;
	}
	str = str + item->text(0).replace("*", "") + ".dat";
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	QFileDialog* fDialog = new QFileDialog();
	QString path = fDialog->getSaveFileName(this, QString("另存为"), str, QString("*.dat"));
	if (path.isNull())
	{
		return;
	}
	QByteArray array = appEntrance->getUIInputMgr()->getByteArray(false,false);
	if (!path.contains(".dat"))
	{
		path = path + ".dat";
	}

	QFile file(path);
	if (!file.open(QIODevice::WriteOnly)) {
		//std::cerr << "Cannot open file for writing: "
		//	<< qPrintable(file.errorString()) << std::endl;

		return;
	}
	delete fDialog;
	QDataStream out(&file);
	out << array;
	file.close();

}
void MainViewContainer::exportTxt()
{
	if (m_pMainMenutempleteTreeWidget->isVisible())
	{
		MessageBox::showErrorMessageBox("提示", "模板不允许导出成TXT格式!");
		return;
	}
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	QString str = "./";
	str = str + TabWidgetCollection::getInstance()->getTabTxt().replace("*", "") + ".txt";
	QString path = QFileDialog::getSaveFileName(NULL, "导出纯文本", str, "*.txt");
	if (path.length() == 0)return;
	if (!path.contains(".txt"))
	{
		path = path + ".txt";
	}
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return;
	}
	QTextStream out(&file);
	QString data;
	appEntrance->getStr(data);
	//书写器预留
	out << data;
	file.close();

}
void MainViewContainer::exportHtml()
{
	if (m_pMainMenutempleteTreeWidget->isVisible())
	{
		MessageBox::showErrorMessageBox("提示","模板不允许导出成HTML格式!");
		return;
	}
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	QString str = "./";
	str = str + TabWidgetCollection::getInstance()->getTabTxt().replace("*", "") + ".html";
	QString path = QFileDialog::getSaveFileName(NULL, "导出html", str, "*.html");
	if (path.length() == 0)return;
	if (!path.contains(".html"))
	{
		path = path + ".html";
	}
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return;
	}

	QTextCodec *tc = QTextCodec::codecForName("utf-8");
	QTextCodec::setCodecForLocale(tc);
	QTextStream out(&file);

	HTMLDocument * doc = NULL;
	doc = HTMLDocument::create(appEntrance);
	QString data = tc->toUnicode(doc->data);
	out << data;
	file.close();
}
void MainViewContainer::fileOpen()
{

	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QString itemId = TabWidgetCollection::getInstance()->getCurrentWidgetId();
	QTreeWidgetItem *item = TabWidgetCollection::getInstance()->getTreeItem(itemId);
	if (item == NULL)return;
	QString str = "./";
	QTreeWidgetItem *parItem = item->parent();
	QString strPar = parItem->text(0);
	QString strType = strPar.mid(1, 2);
	if (strType == progressNote&&m_pMainMenuTreeWidget->isVisible() == true)//病历下的病程记录
	{
		MessageBox::showErrorMessageBox("提示", "病程记录不允许打开其他文件!");
		return;
	}
	QVariant var = item->data(0, Qt::UserRole);
	QStringList varList = var.toStringList();
	QString    isModify = varList[2];
	if (isModify == "2")
	{
		MessageBox::showErrorMessageBox("提示", "当文件处于新增或修改状态时才能用其他文件替换!");
		return;
	}


	QFileInfo fi;
	QString fileName, filePath;

	QStringList list = QFileDialog::getOpenFileNames(NULL, "打开文件", "", "*.dat");
	if (list.count() == 0)
	{
		return;
	}
	for (int i = 0; i < list.count(); i++)
	{
		fi = QFileInfo(list.at(i));
		fileName = fi.fileName();
		filePath = fi.absoluteFilePath();
	}
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	appEntrance->getUIInputMgr()->clearAllData();
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}
	QDataStream in(&file);
	QByteArray array;
	in >> array;
	//if (m_pMainMenuTreeWidget->isVisible())
	//{
	//	appEntrance->setMacroData(m_pDataBaseControl->m_patientData);
	//}
	appEntrance->init(&array,NULL);
	setMenuPageEnable(appEntrance);

}
/*创建工具栏*/
void MainViewContainer::createToolBars()
{
	m_pSaveToolBar = addToolBar("保存");
	m_pSaveToolBar->addAction(m_pSaveAct);
	m_pSaveToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);


	m_pFileToolBar = addToolBar("文件");
	m_pFileToolBar->addAction(m_pRedoAct);
	m_pFileToolBar->addAction(m_pCutAct);
	m_pFileToolBar->addAction(m_pCopyAct);
	m_pFileToolBar->addAction(m_pPasteAct);


	m_pFontToolBar = addToolBar("字体");
	m_pFontToolBar->addAction(m_pBoldAct);
	m_pFontToolBar->addAction(m_pItalicAct);
	m_pFontToolBar->addAction(m_pUnderlineAct);
	m_pFontToolBar->addAction(m_pSuperscriptAct);
	m_pFontToolBar->addAction(m_pSubscriptAct);
	m_pFontToolBar->addSeparator();

	this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}
void MainViewContainer::textColor()
{
	QColor col = QColorDialog::getColor(Qt::red, this);
	if (!col.isValid())return;
}

void MainViewContainer::textSize(const QString &p)
{
	qreal pointSize = p.toFloat();
	if (p.toFloat() > 0)
	{
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);
	}
}

//获取节点所属分类
QTreeWidgetItem* MainViewContainer::getSpecialItem(QTreeWidget *widget, QString str)
{
	QList<QTreeWidgetItem *> items = widget->findItems(str, Qt::MatchExactly);
	QTreeWidgetItem *item = NULL;
	for (int i = 0; i < items.count(); i++)
	{
		if (items[i]->parent() == NULL)
		{
			item = items[i];
			break;
		}
	}
	return item;
}
/*新增模板*/
void MainViewContainer::addTemplateDlg()
{
	m_pPreItem = m_pMainMenutempleteTreeWidget->currentItem();
	QTreeWidgetItem * deleteItem = TabWidgetCollection::getInstance()->saveData(false);
	if (deleteItem != NULL)
	{
		//TabWidgetCollection::getInstance()->setIsPopMessageBox(false);
		QTreeWidgetItem *parItem = deleteItem->parent();
		m_pMainMenutempleteTreeWidget->setCurrentItem(deleteItem);
		deleteTreeNodeAndTab(true);
		m_pMainMenutempleteTreeWidget->setCurrentItem(m_pPreItem);
	}
	if (m_bIsClickCanccel)
	{
		m_bIsClickCanccel = false;
		return;
	}
	AddMuBanDlg * dlg = new AddMuBanDlg;
	//根据界面设置弹出框所属业务的类型
	QMap<QString, QString> map;
	QString name = m_pMainMenutempleteTreeWidget->currentItem()->text(0);
	map.insert("businessName", name);
	dlg->init(map);
	dlg->show();
	if (dlg->exec() == QDialog::Accepted)
	{
		QMap<QString, QString> map = dlg->getData();
		map.clear();
		map = dlg->getData();
		m_mapAddTemplate.clear();
		m_mapAddTemplate = dlg->getData();
		QString businessName = map["businessName"];
		QTreeWidgetItem *item = NULL;
		item = getSpecialItem(m_pMainMenutempleteTreeWidget, businessName);// 
		QString  templateName = map["templateName"];
		if (item != NULL)
		{
			createTreeNodeTips(m_pMainMenutempleteTreeWidget, item, templateName);
		}
	}
}
/*模板设置*/
void MainViewContainer::templateSet()
{
	//先移除病历页面，提示是否保存
	QTreeWidgetItem * deleteItem = TabWidgetCollection::getInstance()->saveData(false);
	if (deleteItem != NULL)
	{
		//TabWidgetCollection::getInstance()->setIsPopMessageBox(false);
		QTreeWidgetItem *parItem = deleteItem->parent();
		if (m_pMainMenutempleteTreeWidget->isVisible() == true)
		{
			m_pMainMenutempleteTreeWidget->setCurrentItem(deleteItem);
		}
		else
		{
			m_pMainMenuTreeWidget->setCurrentItem(deleteItem);
		}
		deleteTreeNodeAndTab(true);

		if (m_pMainMenutempleteTreeWidget->isVisible() == true)
		{
			m_pMainMenutempleteTreeWidget->setCurrentItem(parItem);
		}
		else
		{
			m_pMainMenuTreeWidget->setCurrentItem(parItem);
		}
	}
	TabWidgetCollection::getInstance()->removeAllTab();
	MuBanEditDlg dlg;
	dlg.show();
	if (dlg.exec() == QDialog::Accepted)
	{
		m_pMainMenuBottomTreeWidget->setEnabled(false);
		m_pMainMenuBottomTreeWidget->setVisible(false);
		m_pMainMenuTreeWidget->setEnabled(false);
		m_pMainMenuTreeWidget->setVisible(false);

		m_pMainMenutempleteTreeWidget->setEnabled(true);
		m_pMainMenutempleteTreeWidget->setVisible(true);
		m_pMainMenuRightTreeWidget->setEnabled(true);
		m_pMainMenuRightTreeWidget->setVisible(true);
		this->show();
		//测试语句

		m_mapAddTemplate.clear();
		loadTemplateData(m_pMainMenutempleteTreeWidget, "MEDICALRECORDS_TEMPLATE_TYPE", m_mapSqlTemplate);

		m_mapAddTemplate = dlg.getData();
		if (m_mapAddTemplate.contains("operateType") && m_mapAddTemplate["operateType"] == "Add")
		{

			QString  templateName = m_mapAddTemplate["templateName"];
			QString  businessName = m_mapAddTemplate["businessName"];
			QTreeWidgetItem *item = getSpecialItem(m_pMainMenutempleteTreeWidget, businessName);
			if (item != NULL)
			{
				createTreeNodeTips(m_pMainMenutempleteTreeWidget, item, templateName);		
			}
		}
		else//表明是修改模板
		{
			if (m_mapAddTemplate.contains("path"))
			{
				//关联的ID数据
				QString path = m_mapAddTemplate["path"];
				QTreeWidgetItem *item = getItemToId(m_pMainMenutempleteTreeWidget, path);
				if (item != NULL)
				{
					QTreeWidgetItem *parentItem = item->parent();
					parentItem->setExpanded(true);
					m_pMainMenutempleteTreeWidget->setCurrentItem(item);
					LoadViews(item->text(0), true, item, path);
					if (m_mapAddTemplate["operateType"] == "Modify")
					{
						modifyNode(m_pMainMenutempleteTreeWidget);
						dlg.hide();
					}
				}
			}
		}
		m_bIsClose = false;
	}
	else
	{
		m_bIsClose = true;
		this->close();
	}
}
//根据id获取到对应的模板
QTreeWidgetItem* MainViewContainer::getItemToId(QTreeWidget *widget, QString id)
{
	if (widget == NULL)
		return NULL;
	QTreeWidgetItemIterator it(widget);
	while (*it)
	{
		QVariant var = (*it)->data(0, Qt::UserRole);
		if ((*it)->parent() == NULL)
		{
			++it;
			continue;
		}
		QStringList strList = var.toStringList();
		int strCount = strList.count();
		QString tid = strList[0];
		if (tid == id)
		{
			return *it;
		}
		++it;
	}
	return NULL;
}
void MainViewContainer::createTreeNodeTips(QTreeWidget *widget, QTreeWidgetItem *item, QString str)
{
	if (widget != NULL&&item != NULL)
	{
		addTreeNodeAndTab(item, str, true);
		widget->expandItem(item);
	}
}
bool MainViewContainer::isExist(QTreeWidgetItem *item, QString str)
{
	if (item->childCount() != 0)
	{
		QTreeWidgetItem *childItem = NULL;
		for (int i = 0; i < item->childCount(); i++)
		{
			childItem = item->child(i);
			if (childItem->text(0) == str)
			{
				return true;
			}
		}
	}
	return false;
}

/*新增病历弹出窗口*/
void MainViewContainer::selectmubandlg()
{
	m_pPreItem = m_pMainMenuTreeWidget->currentItem();
	QTreeWidgetItem * deleteItem = TabWidgetCollection::getInstance()->saveData(false);
	if (deleteItem != NULL)
	{
		//TabWidgetCollection::getInstance()->setIsPopMessageBox(false);
		QTreeWidgetItem *parItem = deleteItem->parent();
		m_pMainMenuTreeWidget->setCurrentItem(deleteItem);
		deleteTreeNodeAndTab(true);
		m_pMainMenuTreeWidget->setCurrentItem(m_pPreItem);
	}
	if (m_bIsClickCanccel)
	{
		m_bIsClickCanccel = false;
		return;
	}
	SelectMuBanDlg *selMuDlg = new SelectMuBanDlg(m_pPreItem->text(0).mid(1, 2));
	selMuDlg->show();
	if (selMuDlg->exec() == QDialog::Accepted)
	{
		m_Templatedata = selMuDlg->getData();//测试使用
		m_pAddTemplateId = m_Templatedata.data;
		//数据问题
		QTreeWidgetItem * item = m_pMainMenuTreeWidget->currentItem();
		addTreeNodeAndTab(item, m_Templatedata.name);
		m_pMainMenuTreeWidget->expandItem(item);
	}
}
/*新增病历*/
void MainViewContainer::addTreeNodeAndTab(QTreeWidgetItem* item, QString str, bool isTemplate)
{
	MdcAuthority mdcAth_;
	mdcAth_.mdcName = str;
	mdcAth_.mdcAuthor = "0";
	if (item != NULL&&item->parent() == NULL)
	{
		QStringList userModuleName;
		QString itemName = "";
		if (str.isEmpty() && str == "")
		{
			itemName = getItemName(item);
		}
		else
		{
			itemName = str;
		}
		//设置病历节点的序号以及病历的id
		itemName += "*";
		userModuleName.append(itemName);
		QString id = medicalRecordsId(isTemplate, item, str);//特殊处理病程记录书写,当存在的时候则不新添加书写器
		mdcAth_.mdcId = id;
		int serivalNumber = getItemSerialNumber(item);

		QTreeWidgetItem *item1 = new QTreeWidgetItem(item, userModuleName);
		QStringList strList;
		QString isModify = "0";//0:新增，1：修改，2：保存
		strList << id << QString::number(serivalNumber + 1) << isModify;
		QVariant var;
		var.setValue(strList);
		item1->setData(0, Qt::UserRole, var);
		if (m_pMainMenutempleteTreeWidget->isVisible() == true)
		{
			m_mapAllTemplate.insert(id, m_mapAddTemplate);
		}
		else
		{
			//病程记录的特殊处理
			QString parStr = item->text(0).mid(1, 2);
			if (parStr == progressNote)
			{
				id = m_strSpecialId;
				strList << m_strSpecialId;
				var.clear();
				var.setValue(strList);
				item1->setData(0, Qt::UserRole, var);
			}
			if (m_mapAllMedical.contains(id) == false)
			{
				m_mapAllMedical.insert(id, m_Templatedata);
			}
		}
		//item1->setSelected(true);
		LoadViews(itemName, isTemplate, item1, id);
	}
	m_stlMdcAth.insert(mdcAth_.mdcId, mdcAth_);
	setPrompts(mdcAth_.mdcId);
}
//id处理，病程记录特殊处理 (MODIFY2016/9/23)
QString MainViewContainer::medicalRecordsId(bool isMedical, QTreeWidgetItem *item, QString str)
{
	QString id = getId();
	if (isMedical == false && item != NULL)//表示处于病历的编写
	{
		if (m_mapSql.contains(item->text(0)))
		{
			if (m_mapSql[item->text(0)] == progressNote)//病程记录特殊处理
			{
				int count = item->childCount();
				if (count == 0)
				{
					m_strSpecialId = id;
				}
				else
				{
					QTreeWidgetItem *childItem = item->child(0);
					QVariant var = childItem->data(0, Qt::UserRole);
					QStringList   list = var.toStringList();
					m_strSpecialId = list[3];
				}
			}
		}
	}
	return id;
}

//获取病历的名字
QString MainViewContainer::getItemName(QTreeWidgetItem *item)
{
	int number = 1;
	QString itemName = item->text(0);//获取节点的名字
	if (item->childCount() == 0)
	{
		itemName = itemName + QString::number(number);
	}
	else
	{
		int length = itemName.length();
		for (int i = 0; i < item->childCount(); i++)
		{
			QString childString = item->child(i)->text(0);
			QString numberString = childString.mid(length); //获取名称后的序号
			if (numberString.toInt() >= number)
			{
				number = numberString.toInt();
			}
		}
		number = number + 1;
		itemName = itemName + QString::number(number); //字符串拼接
	}
	return itemName;
}
/*修改病历*/
void MainViewContainer::modifyTreeNodeAndTab()
{
	QTreeWidget *widget = NULL;
	if (m_pMainMenuTreeWidget->isVisible() == true)
	{
		widget = m_pMainMenuTreeWidget;
	}
	else
	{
		widget = m_pMainMenutempleteTreeWidget;
	}
	m_pPreItem = widget->currentItem();
	//新增的模版或者病历点击了编辑
	if (m_pPreItem == NULL || m_pPreItem->parent() == NULL)
	{
		return;
	}
	//修改时，关闭所有页面(此方法处理的目的：时间不够,暂用此方法处理)
	QStringList strlist = m_pPreItem->data(0, Qt::UserRole).toStringList();


	QTreeWidgetItem *parItem = m_pPreItem->parent();
	QString strPar = parItem->text(0);
	QString strCourseDisease = strPar.mid(1, 2);
	if (strlist[2] == "2")
	{
		QTreeWidgetItem * deleteItem = TabWidgetCollection::getInstance()->saveData(mSaveDataIsFromRightMenu);
		mSaveDataIsFromRightMenu = false;
		if (deleteItem != NULL)
		{
			//	TabWidgetCollection::getInstance()->setIsPopMessageBox(false);
			QTreeWidgetItem *parItem = deleteItem->parent();
			if (m_pMainMenutempleteTreeWidget->isVisible() == true)
			{
				m_pMainMenutempleteTreeWidget->setCurrentItem(deleteItem);
			}
			else
			{
				m_pMainMenuTreeWidget->setCurrentItem(deleteItem);
			}
			deleteTreeNodeAndTab(true);

			if (m_pMainMenutempleteTreeWidget->isVisible() == true)
			{
				m_pMainMenutempleteTreeWidget->setCurrentItem(m_pPreItem);
			}
			else
			{
				m_pMainMenuTreeWidget->setCurrentItem(m_pPreItem);
			}
		}
	}
	else
	{
		TabWidgetCollection::getInstance()->modifyAllName();
	}
	if (m_bIsClickCanccel)
	{
		m_bIsClickCanccel = false;
		return;
	}
	else
	{
		modifyNode(widget);
	}
}
/*删除病历*/
void MainViewContainer::deleteTreeNodeAndTab(bool isModifyForDelete)
{
	if (isModifyForDelete == true)
	{
		if (m_pMainMenuTreeWidget->isVisible() == true)
		{
			deleteNode(m_pMainMenuTreeWidget);
		}
		else
		{
			deleteNode(m_pMainMenutempleteTreeWidget);
		}
	}
	else
	{		
		if (MessageBox::showQueryMessageBox("删除","删除后将不可恢复，确定删除？") == QMessageBox::Yes)
		{
			if (m_pMainMenuTreeWidget->isVisible() == true)
			{
				deleteNode(m_pMainMenuTreeWidget);
				TabWidgetCollection::getInstance()->modifyAllName();
			}
			else
			{
				deleteNode(m_pMainMenutempleteTreeWidget);
			}
		}
	}
}
void MainViewContainer::saveNode(QTreeWidget *pWidget)
{
	QTreeWidgetItem *item = pWidget->currentItem();
	if (item != NULL&&item->parent() != NULL)
	{
		QString stringName = item->text(0);
		TabWidgetCollection::getInstance()->saveTabName(stringName);
		QString SaveName = stringName.replace("*", "");
		item->setText(0, SaveName);

		//通过状态表明哪些需要保存
		QVariant var = item->data(0, Qt::UserRole);
		QStringList list = var.toStringList();
		QString    isModify = list[2];
		QString    isModifySave = list[2];//用于数据库 0:insert,1:modify
		if (isModify != "2")//表明有改动的数据,改动的数据
		{
			isModify = "2";

			QStringList list1;
			if (list.length() == 4)//病程记录的特点
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
			item->setData(0, Qt::UserRole, var);
			if (m_pMainMenuTreeWidget->isVisible() == true)
			{
				saveMedicalData(item, isModifySave);
			}
			else
			{
				saveTemplateData(item, isModifySave);
			}
		}
	}
}
void MainViewContainer::saveMedicalData(QTreeWidgetItem* pItem, QString pIsModify, bool isFromRightMenu)
{
	if (pItem != NULL&&pItem->parent() != NULL)
	{

		QTreeWidgetItem *parItem = pItem->parent();
		QString strPar = parItem->text(0);
		QString strCourseDisease = strPar.mid(1, 2);
		QString FOUND_ID = m_pDataBaseControl->m_docutorData->user_id;
		QString FOUND_NAME = m_pDataBaseControl->m_docutorData->user_name;
		QString   strClassID = "";//分类代码
		QString   SERIAL_NUMBER = "";//病历序号
		QString   DEPARTMENT_NAME = m_pDataBaseControl->m_docutorData->dept_name;//科室名称
		QString   DEPARTMENT_CODE = m_pDataBaseControl->m_docutorData->dept_code;//科室代码
		QString   PATIENT_ID = m_pDataBaseControl->m_patientData->id;//病人ID
		QString   PATIENT_NAME = m_pDataBaseControl->m_patientData->name;//病人姓名
		QString   PATIENT_SEX = m_pDataBaseControl->m_patientData->sex;//病人性别
		QString   PATIENT_INP_NO = m_pDataBaseControl->m_patientData->inp_NO;//病人住院号
		if (PATIENT_SEX == "男")
		{
			PATIENT_SEX = "1";//1：男
		}
		else
		{
			PATIENT_SEX = "2";//2：女
		}
		QString   NAME = "";//病历名称
		QString   DATA = "";//病历数据关联的ID;//特别注意，此处不应该是模板ID;
		QString  FLAGS = "0";//病历编辑状态
		QString  CREATE_TIME = "";//创建时间

		QString PID = "";//存放EMR数据所关联的ID
		QDateTime  time;
		FLAGS = "'" + FLAGS + "')";//最后的一个数据
		QMap<QString, QString> map;
		if (m_mapSql.contains(strPar))
		{
			strClassID = m_mapSql[strPar];
			strClassID = "'" + strClassID + "',";
		}
		QVariant var = pItem->data(0, Qt::UserRole);
		QStringList list = var.toStringList();
		DATA = list[0];
		PID = list[0];
		SERIAL_NUMBER = list[1];
		SERIAL_NUMBER = "'" + SERIAL_NUMBER + "',";
		//病程记录特殊处理
		if (strCourseDisease == progressNote)
		{
			DATA = m_strSpecialId;
			PID = m_strSpecialId;
		}
		if (m_mapAllMedical.contains(DATA))
		{
			MRTemplate mrData = m_mapAllMedical[DATA];
			if (m_bIsSupperUser)
			{
				DEPARTMENT_CODE = mrData.departmentCode;

				DEPARTMENT_NAME = mrData.departmentName;
			}
			//	NAME = mrData.name;
			NAME = pItem->text(0).replace("*", "");
			NAME = "'" + NAME + "',";
			DEPARTMENT_NAME = "'" + DEPARTMENT_NAME + "',";
			DEPARTMENT_CODE = "'" + DEPARTMENT_CODE + "',";
			FOUND_ID = "'" + FOUND_ID + "',";
			FOUND_NAME = "'" + FOUND_NAME + "',";
			PATIENT_ID = "'" + PATIENT_ID + "',";
			PATIENT_NAME = "'" + PATIENT_NAME + "',";
			PATIENT_SEX = "'" + PATIENT_SEX + "',";
			CREATE_TIME = "SYSDATE,";
			DATA = list[0];
			DATA = "'" + DATA + "',";
			PATIENT_INP_NO = "'" + PATIENT_INP_NO + "',";
		}
		if (pIsModify == "0")
		{
			QString sql = "insert into medicalrecords_list(ID,CLASSIFYCODE,SERIAL_NUMBER,DEPARTMENT_NAME,DEPARTMENT_ID,NAME,DATA,CREATE_TIME,FOUNDER_ID,FOUNDER_NAME,PATIENT_ID,PATIENT_NAME,PATIENT_SEX,INP_NO,FLAGS)values";
			QString pSql = "(" + DATA + strClassID + SERIAL_NUMBER + DEPARTMENT_NAME + DEPARTMENT_CODE + NAME + DATA + CREATE_TIME + FOUND_ID + FOUND_NAME + PATIENT_ID + PATIENT_NAME + PATIENT_SEX + PATIENT_INP_NO + FLAGS;
			if (strCourseDisease == progressNote)
			{
				NAME = "'" + pItem->text(0).replace("*", "") + "',";
				QString strId = "'" + m_strSpecialId + "',";
				pSql = "(" + DATA + strClassID + SERIAL_NUMBER + DEPARTMENT_NAME + DEPARTMENT_CODE + NAME + strId + CREATE_TIME + FOUND_ID + FOUND_NAME + PATIENT_ID + PATIENT_NAME + PATIENT_SEX + PATIENT_INP_NO + FLAGS;
			}
			QString strSQL = sql + pSql;
			m_pDataBaseControl->insertData(strSQL);
		}
		//MEDICALRECORDS_DATA
		QString strSQLMeRecords = "SELECT SEQ_MEDICALRECORDS_DATA.NEXTVAL SEQ FROM DUAL";
		QString ID = "";
		ID = m_pDataBaseControl->getId(strSQLMeRecords);
		if (strCourseDisease == progressNote)//第四个用于病程记录的多对一,确定唯一的PID
		{
			if (list.size() < 4)
			{
				list << m_strSpecialId;
			}
			pItem->setData(0, Qt::UserRole, list);
		}
		if (strCourseDisease == progressNote)
		{
			showSelectMainMenuItem(pItem, 0);
		}
		QWidget * widget = TabWidgetCollection::getInstance()->getUnsavedWidget();
//		QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
		if (widget == NULL)
		{
			widget = TabWidgetCollection::getInstance()->getCurrentWidget();
		}
		if(widget == NULL)return;
		QObject * pAction = this->sender();
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		appEntrance->showTidy(false);
		
		if (pIsModify == "0")
		{
//			QByteArray ba = appEntrance->getUIInputMgr()->getByteArray(true, isFromRightMenu);
			QByteArray ba = appEntrance->getUIInputMgr()->getByteArrayAndCompress(true, isFromRightMenu);
			//int childCount = parItem->childCount();
			if (strCourseDisease == progressNote)
			{
				if (parItem->childCount() > 1) //当节点>1时，表明数据库有病程记录的数据，只需要进行修改就可以了
				{
					m_pDataBaseControl->updateMedicalData(ba, m_strSpecialId, false);
				}
				else
				{
					m_pDataBaseControl->insertMedicalData(ba, ID, m_strSpecialId, false);
				}
				appEntrance->getUIInputMgr()->closeAllLevelOneTempleteWriteAutority(true);
				appEntrance->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
			}
			else
			{
				m_pDataBaseControl->insertMedicalData(ba, ID, PID, false);
				if (strCourseDisease != progressNote)
				{
					appEntrance->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
				}
			}
			QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
			QString strTime = time.toString("yyyy-MM-ddThh:mm:ss");
			QString createNameAndTime = "创建人:" + m_pDataBaseControl->m_docutorData->user_name + " 创建时间:" + strTime;
			pItem->setToolTip(0, createNameAndTime);
			auto it = m_stlMdcAth.find(PID);
			if (it != m_stlMdcAth.end())
			{
				MdcAuthority mdcAth_ = *it;
				bool ok_;
				int nAuthor = mdcAth_.mdcAuthor.toInt(&ok_, 10);
				if (!ok_)
				{
					nAuthor = -1;
				}
				modifyTreeItem(pItem, nAuthor);
			}
		}
		else if (pIsModify == "1")
		{
			//QByteArray ba = appEntrance->getUIInputMgr()->getByteArray(true, isFromRightMenu);
			QByteArray ba = appEntrance->getUIInputMgr()->getByteArrayAndCompress(true, isFromRightMenu);
			m_pDataBaseControl->updateMedicalData(ba, PID, false);
			QString sql = "update medicalrecords_list set MODIFYER_NAME='" + m_pDataBaseControl->m_docutorData->user_name + "', MODIFYER_ID='" + m_pDataBaseControl->m_docutorData->user_id + "', MODIFY_TIME=SYSDATE where ID='" + PID + "'";
			m_pDataBaseControl->insertData(sql);
			if (strCourseDisease != progressNote)
			{
				appEntrance->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
			}
			else
			{
				appEntrance->getUIInputMgr()->closeAllLevelOneTempleteWriteAutority(true);
			}
		}
		//权限回置为2
		QStringList Itemlist = pItem->data(0, Qt::UserRole).toStringList();
		QStringList newlist;
		QString  save = "2";
		if (Itemlist.length() == 4)//病程记录的特点
		{
			newlist << Itemlist[0] << Itemlist[1] << save << Itemlist[3];
			var.clear();
			var.setValue(newlist);
		}
		else
		{
			Itemlist.removeAt(2);
			Itemlist << save;
			var.clear();
			var.setValue(Itemlist);
		}
		pItem->setData(0, Qt::UserRole, var);
		appEntrance->getToolbar()->setSomeActionsEnable(false);
	}
}
void MainViewContainer::saveTemplateData(QTreeWidgetItem* pItem, QString pIsModify, bool isFromRightMenu)
{
	if (pItem != NULL&&pItem->parent() != NULL)
	{
		QTreeWidgetItem *parItem = pItem->parent();
		QString   strPar = parItem->text(0);
		QString   id = "";
		QString   strClassID = "";//分类代码
		QString   templateId = "";//模板的ID,关联数据的ID
		QString   NAME = "";//模板名称
		QString   CODE = "";//模板代码
		QString   DEPARTMENT_NAME = "";//科室名称
		QString   DEPARTMENT_CODE = "";//科室代码
		QString   REMARKS = "";//备注
		QString   CREATE_ID = m_pDataBaseControl->m_docutorData->user_id;
		QString   CREATE_NAME = m_pDataBaseControl->m_docutorData->user_name;
		QString   TYPE = "";//模板类型
		QString   ISENABLE = "'N')";//是否启用
		QString PID = "";//模板真实数据的PID
		QMap<QString, QString> map;
		if (m_mapSqlTemplate.contains(strPar))
		{
			strClassID = m_mapSqlTemplate[strPar];
		}
		else
		{
			return;
		}
		//根据Id获取信息
		QVariant var = pItem->data(0, Qt::UserRole);
		QStringList list = var.toStringList();
		templateId = list[0];
		id = templateId;
		PID = templateId;
		id = "'" + id + "',";
		if (m_mapAllTemplate.contains(templateId))
		{
			map = m_mapAllTemplate[templateId];
			//NAME = "'" + map["templateName"] + "',";
			NAME = "'" + pItem->text(0).replace("*", "") + "',";
			QString NameCode = map["departmentName"];

			QStringList str = NameCode.split(')');

			CODE = str[0].mid(1, str[0].length() - 1);
			CODE = "'" + CODE + "',";
			//DEPARTMENT_NAME = NameCode.mid(4);
			DEPARTMENT_NAME = str[1];
			DEPARTMENT_NAME = "'" + DEPARTMENT_NAME + "',";
			CREATE_ID = "'" + CREATE_ID + "',";
			CREATE_NAME = "'" + CREATE_NAME + "',";
			QString businessCode = map["businessName"];
			strClassID = businessCode.mid(1, 2);
			strClassID = "'" + strClassID + "',";

			REMARKS = map["content"];
			REMARKS = "'" + REMARKS + "',";
			TYPE = map["templateType"];
			TYPE = "'" + TYPE + "',";
		}
		templateId = "'" + templateId + "',";
		if (pIsModify == "0")
		{
			QString sql = "insert into MEDICALRECORDS_TEMPLATE_LIST(ID,CLASSIFYCODE,TYPE,NAME,DEPARTMENT_NAME,DEPARTMENT_CODE,DATA,REMARKS,CREATE_ID,CREATE_NAME,ISENABLE)VALUES(";
			QString strSql = id + strClassID + TYPE + NAME + DEPARTMENT_NAME + CODE + templateId + REMARKS + CREATE_ID + CREATE_NAME + ISENABLE;
			QString templateSql = sql + strSql;
			m_pDataBaseControl->insertData(templateSql);
		}
		//保存真实数据
		QString strSQLMeRecords = "SELECT SEQ_MEDIC_TEMPLATE_DATA.NEXTVAL SEQ FROM DUAL";
		QString ID = "";
		m_pModel = NULL;
		ID = m_pDataBaseControl->getId(strSQLMeRecords);
		showSelectMainMenuItem(pItem, 0);
		QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
		if (widget == NULL)return;
		QObject * pAction = this->sender();
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		appEntrance->showTidy(false);
		UIInputManager * mgr = appEntrance->getUIInputMgr();
		if (pIsModify == "0")
		{
			QByteArray ba;
			//mgr->doSave(ba);
			mgr->doSaveAndCompress(ba);
			m_pDataBaseControl->insertMedicalData(ba, ID, PID, true);
			QString createNameAndTime = "创建人:" + m_pDataBaseControl->m_docutorData->user_name;
			pItem->setToolTip(0, createNameAndTime);

		}
		else
			if (pIsModify == "1")
			{
			//	QByteArray ba = mgr->getByteArray(true, isFromRightMenu);
				QByteArray ba = mgr->getByteArrayAndCompress(true, isFromRightMenu);
				m_pDataBaseControl->updateMedicalData(ba, PID, true);
			}
			mgr->setFileAuthority(true, AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
		appEntrance->getToolbar()->setSomeActionsEnable(false);

		QStringList Itemlist = pItem->data(0, Qt::UserRole).toStringList();
		QString  save = "2";
		Itemlist.removeAt(2);
		Itemlist << save;
		var.clear();
		var.setValue(Itemlist);
		pItem->setData(0, Qt::UserRole, var);
	}
}
void MainViewContainer::saveTreeNodeAnadTab()
{
	if (m_pMainMenuTreeWidget->isVisible() == true)
	{
		saveNode(m_pMainMenuTreeWidget);
	}
	else
	{
		saveNode(m_pMainMenutempleteTreeWidget);
	}
	setMenuStatus(false);
}
void MainViewContainer::modifyNode(QTreeWidget *widget)
{
	QTreeWidgetItem *item = widget->currentItem();
	if (item != NULL&&item->parent() != NULL)
	{
		showSelectMainMenuItem(item, 0);
		QString stringName = item->text(0);
		QString ModifyName = stringName.replace("*", "");
		ModifyName = ModifyName + "*";
		item->setText(0, ModifyName);
		TabWidgetCollection::getInstance()->modifyTabName(stringName);
		QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
		if (widget == NULL)return;
		QObject * pAction = this->sender();
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		//修改其状态
		QVariant var = item->data(0, Qt::UserRole);
		QStringList list = var.toStringList();
		QString   isModify = list[2];
		dapper(false);//去掉整洁显示
//		m_pLayoutAct->setChecked(false);
		if (isModify != "0")
		{
			QStringList list1;
			if (list.length() == 4)//病程记录的特点
			{
				isModify = "1";
				list1 << list[0] << list[1] << isModify << list[3];
				var.clear();
				var.setValue(list1);
				appEntrance->getUIInputMgr()->closeAllLevelOneTempleteWriteAutorityExcept(true, list[0]);
				appEntrance->getUIInputMgr()->gotoMedicalRecord(true, list[0]);
				appEntrance->getToolbar()->setSomeActionsEnable(true);
				appEntrance->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT);
				setMenuStatus(true);
			}
			else
			{
				var.clear();
				list.removeAt(2);
				isModify = "1";
				list << isModify;
				var.setValue(list);
				if (m_pMainMenuTreeWidget->isVisible() == true)
				{
					appEntrance->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT);
					setMenuStatus(true);
					appEntrance->getToolbar()->setSomeActionsEnable(true);
				}
				else
				{
					appEntrance->getUIInputMgr()->setFileAuthority(true, AuthorityConstants::Authority::WRITE_ALL);
					setMenuStatus(true);
					appEntrance->getToolbar()->setSomeActionsEnable(true);
				}
			}
			item->setData(0, Qt::UserRole, var);
		}
		setMenuPageEnable(appEntrance);
	}
}
void MainViewContainer::deleteNode(QTreeWidget *widget)
{
	QTreeWidgetItem * item = widget->currentItem();
	QTreeWidgetItem * parItem = item->parent();
	if (item != NULL&&item->parent() != NULL)
	{
		//根据ID删除数据库的记录
		QString strPar = item->parent()->text(0).mid(1, 2);
		int childCount = 0;

		QVariant var = item->data(0, Qt::UserRole);
		QStringList list = var.toStringList();
		QString id = list[0];
		if (strPar == progressNote&&m_pMainMenuTreeWidget->isVisible() == true)
		{
			childCount = item->parent()->childCount();
			id = list[3];
		}
		int index = TabWidgetCollection::getInstance()->getIndex(id);
		TabWidgetCollection::getInstance()->setIsPopMessageBox(false);
		if (strPar != progressNote)

		{

			bool isDelete = TabWidgetCollection::getInstance()->removeSubTab(index);
			if (isDelete)
			{
				delete item;
				item = NULL;
			}
		}
		if (m_pMainMenuTreeWidget->isVisible() == true)
		{
			if (strPar == progressNote)
			{

				childCount = parItem->childCount();
				QTreeWidgetItem *compareItem = parItem->child(childCount - 1);
				QVariant varPre = compareItem->data(0, Qt::UserRole);
				QStringList listPre = varPre.toStringList();
				QString idPre = listPre[0];
				if (childCount > 1 && list[0] != idPre)
				{
					MessageBox::showErrorMessageBox("提示","病程记录只能从最后一个节点开始删除");
					return;
				}

				id = list[0];
				m_pDataBaseControl->deleteMedicalData(id, false, true);
				//病程记录删除特定节点的相关数据

				QWidget *widgetEntrace = NULL; //TabWidgetCollection::getInstance()->getWidget(list[3]);
				MyAppEntrance *appEntrance = NULL;// static_cast<MyAppEntrance*>(widgetEntrace);
				if (childCount > 1)
				{
					showSelectMainMenuItem(item, 0);
					widgetEntrace = TabWidgetCollection::getInstance()->getWidget(list[3]);
					appEntrance = static_cast<MyAppEntrance*>(widgetEntrace);
				}
				if (childCount > 1)
				{
					appEntrance->getUIInputMgr()->deleteLevelOneTempleteByCodeId(true, id);
					//QByteArray ba = appEntrance->getUIInputMgr()->getByteArray(true, false);
					QByteArray ba = appEntrance->getUIInputMgr()->getByteArrayAndCompress(true, false);
					m_pDataBaseControl->updateMedicalData(ba, list[3], false);
					childCount = parItem->childCount();
					if (childCount > 1)
					{
						QTreeWidgetItem *childItem = parItem->child(childCount - 2);
						TabWidgetCollection::getInstance()->modifTabItem(childItem, list[3]);
						QVariant preVar = childItem->data(0, Qt::UserRole);
						QStringList preList = preVar.toStringList();
						QString preId = preList[0];
						appEntrance->getUIInputMgr()->gotoMedicalRecord(true, preId);
					}
				}
				if (childCount == 1)
				{
					id = list[3];
					TabWidgetCollection::getInstance()->removeSubTab(index);
					m_pDataBaseControl->deleteMedicalData(id, false, true, true);
				}
				delete item;
				item = NULL;

			}
			else
			{
				m_pDataBaseControl->deleteMedicalData(id, false);
			}
		}
		else
		{
			m_pDataBaseControl->deleteMedicalData(id, true);
			TabWidgetCollection::getInstance()->removeSubTab(index);
			delete item;
			item = NULL;
		}

	}
}

QString MainViewContainer::getId()
{
	QString str = "";
	QString strSQLTemplete = "SELECT SEQ_MEDIC_TEMPLATE_LIST.NEXTVAL SEQ FROM DUAL";
	QString strSQLMeRecords = "SELECT SEQ_MEDICALRECORDS_LIST.NEXTVAL SEQ FROM DUAL";
	m_pModel = NULL;
	if (m_pMainMenutempleteTreeWidget->isVisible() == true)
	{
		str = m_pDataBaseControl->getId(strSQLTemplete);
	}
	else
	{
		str = m_pDataBaseControl->getId(strSQLMeRecords);
	}
	return str;
}

int MainViewContainer::getItemSerialNumber(QTreeWidgetItem *item)
{
	int i = 0;
	if (item == NULL)return 0;
	int count = item->childCount();
	if (count == 0)return 0;
	QTreeWidgetItem *pItem = item->child(count - 1);
	QVariant var = pItem->data(0, Qt::UserRole);
	QStringList list = var.toStringList();
	i = list[1].toInt();
	return i;

}

bool MainViewContainer::eventFilter(QObject *watched, QEvent *event)
{
	bool isDownSendPressKey = false;
	bool isDownSendReleaseKey = false;
	QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
	if (watched == m_pMainMenuTreeWidget || watched == m_pMainMenutempleteTreeWidget || watched == m_pMainMenuRightTreeWidget || watched == TabWidgetCollection::getInstance())
	{
		if (event->type() == QEvent::FocusIn)
		{
			int index = TabWidgetCollection::getInstance()->getCurrentIndex();
			if (index != 0)
			{
				QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
				if (widget != NULL)
				{
					MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
					appEntrance->setFocus();
				}
			}
			return true;
			//return true;
		}
	}
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_Tab)
		{
			int index = TabWidgetCollection::getInstance()->getCurrentIndex();
			if (index != 0)
			{
				QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
				if (widget != NULL)
				{
					MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
					appEntrance->keyPressEvent(keyEvent);
					return true;
				}
			}
		}
		if ((keyEvent->modifiers()&Qt::ControlModifier) && keyEvent->key() == Qt::Key_C)
		{

			isDownSendPressKey = true;

		}
		if ((keyEvent->modifiers()&Qt::ControlModifier) && keyEvent->key() == Qt::Key_V)
		{
			isDownSendPressKey = true;

		}
	}
	if (event->type() == QEvent::KeyRelease)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_Tab)
		{
			int index = TabWidgetCollection::getInstance()->getCurrentIndex();
			if (index != 0)
			{
				QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
				if (widget != NULL)
				{
					//MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
					//appEntrance->keyReleaseEvent(keyEvent);
					return true;
				}
			}

		}
		if ((keyEvent->modifiers()&Qt::ControlModifier) && keyEvent->key() == Qt::Key_C)
		{
			isDownSendReleaseKey = true;
		}
		if ((keyEvent->modifiers()&Qt::ControlModifier) && keyEvent->key() == Qt::Key_V)
		{
			isDownSendReleaseKey = true;
		}
	}

	int index = TabWidgetCollection::getInstance()->getCurrentIndex();
	if (index != 0)
	{
		QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
		if (widget != NULL)
		{
			MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
			if (isDownSendPressKey == true)
			{
				appEntrance->keyPressEvent(keyEvent);
			}
			else
				if (isDownSendReleaseKey)
				{
				//appEntrance->keyReleaseEvent(keyEvent);
				}
			return true;
		}
	}
	return QWidget::eventFilter(watched, event);
}

void MainViewContainer::continuePrintClicked()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	QObject * pAction = this->sender();
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	UIInputManager * mgr = appEntrance->getUIInputMgr();
	int startValue;
	int endValue;
	bool isViewParagraphNumber = mgr->getParagraphNumView();
	mgr->setParagraphNumView(true);
	if (ContinuePrintDlg::getData(startValue, endValue))
	{
		if (!isViewParagraphNumber)
			mgr->setParagraphNumView(isViewParagraphNumber);
		mgr->sendContinuingPrintSign(startValue, endValue);
	}
	if (!isViewParagraphNumber)
		mgr->setParagraphNumView(isViewParagraphNumber);
}
bool MainViewContainer::closeWindow()
{
	QTreeWidgetItem * deleteItem = TabWidgetCollection::getInstance()->saveData(false);
	if (m_bIsClickCanccel)
	{
		m_bIsClickCanccel = false;
		return true;
	}
	return false;
}

void MainViewContainer::closeEvent(QCloseEvent *event)
{
	if (closeWindow())
	{
		event->ignore();
	}
}

bool MainViewContainer::isTemplate() const
{
	return m_pMainMenuTreeWidget->isVisible();
}

void MainViewContainer::resizeEvent(QResizeEvent *resizeEvent)
{
	//QSize size = resizeEvent->size();
	////m_size = size;
	//int a = 1;
}

//获取未保存的节点
QTreeWidgetItem* MainViewContainer::GetMedicalUnSaveNode(QTreeWidgetItem *pItem)
{
	if (pItem == NULL)return NULL;
	QTreeWidgetItem *item = pItem->parent();
	QTreeWidgetItem *child_item = NULL;
	for (int i = 0; i < item->childCount(); i++)
	{
		child_item = item->child(i);
		if (child_item->text(0).contains("*"))
		{
			showSelectMainMenuItem(child_item, 0);//切换到未保存的页
			return child_item;
		}
	}
	return NULL;
}

void MainViewContainer::onPageActTriggered()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	PageSettingDlg dlg(widget);
	if (dlg.exec() == QDialog::Accepted)
	{
	}
	if (widget != NULL)
	{
		MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
		UIInputManager * mgr = appEntrance->getUIInputMgr();
		//mgr->setPageProperty();
		//appEntrance->setPageProperty();
	}
}

void MainViewContainer::doCopyPasteCutUndoRedo()
{
	QAction  *action = qobject_cast<QAction *>(QObject::sender());
	if (action == m_pUndoAct)
	{
		ToolBarClass::undoData();
	}
	if (action == m_pRedoAct)
	{
		ToolBarClass::redoData();
	}
	if (action == m_pCopyAct)
	{
		ToolBarClass::copyData();
	}
	if (action == m_pPasteAct)
	{
		ToolBarClass::pasteData();
	}
	if (action == m_pCutAct)
	{
		ToolBarClass::cutData();
	}

}

void MainViewContainer::textBold()
{

	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	if (appEntrance != NULL)
	{
		appEntrance->getToolbar()->setActionIsChecked(1, !appEntrance->getToolbar()->boldAct->isChecked(), "");
		appEntrance->getToolbar()->textBold();
	}
}

void MainViewContainer::leftAlignActTriggered()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	if (appEntrance != NULL)
	{
		appEntrance->getToolbar()->setAlignment(1);
	}
}

void MainViewContainer::rightAlignActTriggered()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	if (appEntrance != NULL)
	{
		appEntrance->getToolbar()->setAlignment(3);
	}
}

void MainViewContainer::centerAlignActTriggered()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	if (appEntrance != NULL)
	{
		appEntrance->getToolbar()->setAlignment(2);
	}
}

void MainViewContainer::textItalic()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	if (appEntrance != NULL)
	{
		appEntrance->getToolbar()->setActionIsChecked(2, !appEntrance->getToolbar()->italicAct->isChecked(), "");
		appEntrance->getToolbar()->textItalic();
	}
}

void MainViewContainer::textUnderline()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	if (appEntrance != NULL)
	{
		appEntrance->getToolbar()->setActionIsChecked(4, !appEntrance->getToolbar()->underlineAct->isChecked(), "");
		appEntrance->getToolbar()->textUnderline();
	}
}

void MainViewContainer::superscrip()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	if (appEntrance != NULL)
	{
		appEntrance->getToolbar()->setActionIsChecked(6, !appEntrance->getToolbar()->superscriptAct->isChecked(), "");
		appEntrance->getToolbar()->superscrip();
	}
}

void MainViewContainer::subscript()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL)return;
	MyAppEntrance *appEntrance = static_cast<MyAppEntrance*>(widget);
	if (appEntrance != NULL)
	{
		appEntrance->getToolbar()->setActionIsChecked(7, !appEntrance->getToolbar()->subscriptAct->isChecked(), "");
		appEntrance->getToolbar()->subscript();
	}
}

MainViewContainer* MainViewContainer::getInstance()
{
	return m_pInstance;
}

void MainViewContainer::deleteEMR(MyAppEntrance *app)
{
	//黄洪 2.16.5.28
	if (m_pListAppEntrance)
	{
		int indexof = m_pListAppEntrance->indexOf(app);
		if (indexof < 0)
		{
			m_pListAppEntrance->push_back(app);
		}
		else{
			qDebug("MainViewContainer::deleteEMR");
		}

	}
	if (!m_pDeleteTimer->isActive())
	{
		m_pDeleteTimer->start(500);
	}
}
/*
*黄洪 2.16.5.28
*/
void MainViewContainer::deleteAppEntrance()
{
	if (m_pListAppEntrance)
	{
		if (m_pListAppEntrance->size() > 0)
		{
			MyAppEntrance  * tempAction;
			for (int i = 0; i < m_pListAppEntrance->size(); i++)
			{
				//QList<BaseAction *> * actionList = mTotalActionList->at(i);
				//clearLocalAction(actionList);
				tempAction = m_pListAppEntrance->at(i);
				if (!tempAction)
				{
					continue;
				}
				delete tempAction;
			}
			//deleteVector(mTotalActionList);
			m_pListAppEntrance->clear();
		}
	}
	if (m_pDeleteTimer->isActive())
	{
		m_pDeleteTimer->stop();
	}
}
void MainViewContainer::renameTriggered()
{
	QTreeWidgetItem * item = NULL;
	QTreeWidget *widget = NULL;
	QString preText = "";
	QString modifyText = "";
	QString sql = "";
	QString id = "";
	QString sqlid = "";
	bool  isVisableMainMenuTree = false;
	if (m_pMainMenuTreeWidget->isVisible())
	{
		item = m_pMainMenuTreeWidget->currentItem();
		isVisableMainMenuTree = true;
	}
	else
	{
		item = m_pMainMenutempleteTreeWidget->currentItem();
	}
	preText = item->text(0).replace("*", "");
	QVariant var = item->data(0, Qt::UserRole);
	QStringList list = var.toStringList();
	id = list[0];
	sqlid = list[0];
	QString   isModify = list[2];
	if (list.length() == 4)
	{
		id = list[3];
	}
	//get new NEW name;

	modifyText = RenamePopup::getData(preText);
	//////////////////////////////////////////////////////////////////////////

	if (preText != modifyText)
	{
		if (isModify != "2")
		{
			modifyText += "*";
			item->setText(0, modifyText);
		}
		if (isModify != "0")
		{
			QTreeWidgetItem* tabItem = TabWidgetCollection::getInstance()->getCurrentWidgetTreeItem(id);
			int index = -1;
			if (tabItem != NULL)
			{
				QVariant Tabvar = tabItem->data(0, Qt::UserRole);
				QStringList tabList = Tabvar.toStringList();

				if (tabList[0] == list[0])
				{
					index = TabWidgetCollection::getInstance()->getIndex(id);
				}

			}
			if (index != -1 && isModify == "1")
			{
				//item->setText(0, modifyText);

				TabWidgetCollection::getInstance()->modifyTabName(index, modifyText);
			}
			if (isModify == "1")
			{
				item->setText(0, modifyText);
			}

			modifyText = modifyText.replace("*", "");
			if (index != -1 && isModify == "2")
			{
				TabWidgetCollection::getInstance()->modifyTabName(index, modifyText);
			}
			if (isModify == "2")
			{
				item->setText(0, modifyText);
			}

			if (isVisableMainMenuTree)
			{
				sql = "update MEDICALRECORDS_LIST set NAME='" + modifyText + "' where ID='" + sqlid + "'";
			}
			else
			{
				sql = "update MEDICALRECORDS_TEMPLATE_LIST set NAME='" + modifyText + "' where ID='" + sqlid + "'";
			}
			m_pDataBaseControl->insertData(sql);
		}
		else
		{
			int index = TabWidgetCollection::getInstance()->getIndex(id);
			if (index != -1)
			{
				TabWidgetCollection::getInstance()->modifyTabName(index, modifyText);
			}
		}
	}
}

void MainViewContainer::setMenuPageEnable(MyAppEntrance *app)
{
	if (NULL == app)return;
	UIInputManager * mgr = app->getUIInputMgr();
	m_pHideOrDisplayHeaderAct->setChecked(mgr->getTitleView());
	m_pHideOrDisplayFooterAct->setChecked(mgr->getBottomView());
	m_pLineNumAct->setChecked(mgr->getParagraphNumView());
	m_bHideOrDisplayTopPageNumber = mgr->getTitleNumViewType();
	m_bHideOrDisplayBottomPageNumber = mgr->getBottomNumViewType();
	m_pGridLineAct->setChecked(mgr->getRowBottomLineType());
	m_pHideOrDisplayTopPageNumber->setChecked(m_bHideOrDisplayTopPageNumber);
	m_pHideOrDisplayBottomPageNumber->setChecked(m_bHideOrDisplayBottomPageNumber);
//	m_pLayoutAct->setChecked(app->isOnShowTidy());
}
void MainViewContainer::setMenuStatus(bool isEnable)
{
	m_pEditMenu->setEnabled(isEnable);
	m_pParagraphMenu->setEnabled(isEnable);
	m_pPageMenu->setEnabled(isEnable);
	m_pInsertMenu->setEnabled(isEnable);
	m_pTableMenu->setEnabled(isEnable);
	m_pFormatMenu->setEnabled(isEnable);
}
void MainViewContainer::destory()
{
	m_pMainMenuTreeWidget->clear();
	m_pMainMenutempleteTreeWidget->clear();
	m_pMainMenuBottomTreeWidget->clear();
	m_pMainMenuRightTreeWidget->clear();
	if (m_pTopHGroupBox)
	{
		delete m_pTopHGroupBox;
		m_pTopHGroupBox = NULL;
	}
	if (m_pTopHMsgGroupBox)
	{
		delete m_pTopHMsgGroupBox;
		m_pTopHMsgGroupBox = NULL;
	}
	if (m_pTopTitleLabel)
	{
		delete m_pTopTitleLabel;
		m_pTopTitleLabel = NULL;
	}
	if (m_pPic)
	{
		delete m_pPic;
		m_pPic = NULL;
	}
	if (m_pLeftLayout)
	{
		delete m_pLeftLayout;
		m_pLeftLayout = NULL;
	}
	if (DBManager::oracleLib)
	{
		delete DBManager::oracleLib;
		DBManager::oracleLib = NULL;
	}
}


void MainViewContainer::modifyMenuStatus(int type, bool isChecked)
{
	switch (type)
	{
	case PageSetType::PageOfHeader:
		m_bHideOrDisplayHeaderAct = isChecked;
		m_pHideOrDisplayHeaderAct->setChecked(isChecked);
		break;
	case PageSetType::PageOfFooter:
		m_bHideOrDisplayFooterAct = isChecked;
		m_pHideOrDisplayFooterAct->setChecked(isChecked);
		break;
	case PageSetType::TopPageNumber:
		m_bHideOrDisplayTopPageNumber = isChecked;
		m_pHideOrDisplayTopPageNumber->setChecked(isChecked);
		break;
	case PageSetType::BottomPageNumber:
		m_bHideOrDisplayBottomPageNumber = isChecked;
		m_pHideOrDisplayBottomPageNumber->setChecked(isChecked);
		break;
	case PageSetType::LineType:
		m_pGridLineAct->setChecked(isChecked);
		break;
	default:
		break;
	}
}


void MainViewContainer::showReport()
{

}
void MainViewContainer::setSaveDataFromRightMenu(bool isFromRightMenu)
{
	mSaveDataIsFromRightMenu = isFromRightMenu;
}


void MainViewContainer::modifyTreeNodeAndTab(QString strItem)
{
	bool bIsFindItem = false;
	QTreeWidget *widget = NULL;
	if (m_pMainMenuTreeWidget->isVisible() == true)
	{
		widget = m_pMainMenuTreeWidget;
	}
	else
	{
		widget = m_pMainMenutempleteTreeWidget;
	}

	QList<QTreeWidgetItem*> itemList_ = widget->findItems("(02)病程记录", Qt::MatchFlag::MatchExactly);
	if (itemList_.size() <= 0)
	{
		return;
	}
	for (int i = 0; i < itemList_[0]->childCount(); i++)
	{
		QTreeWidgetItem* item_ = itemList_[0]->child(i);
		QStringList strlist = item_->data(0, Qt::UserRole).toStringList();
		if(strlist[0] == strItem)
		{
			m_pPreItem = item_;
			bIsFindItem = true;
			break;
		}
	}
	if (bIsFindItem == false)
	{
		return;
	}

	//新增的模版或者病历点击了编辑
	if (m_pPreItem == NULL || m_pPreItem->parent() == NULL)
	{
		return;
	}
	//修改时，关闭所有页面(此方法处理的目的：时间不够,暂用此方法处理)
	QStringList strlist = m_pPreItem->data(0, Qt::UserRole).toStringList();


	QTreeWidgetItem *parItem = m_pPreItem->parent();
	QString strPar = parItem->text(0);
	QString strCourseDisease = strPar.mid(1, 2);
	if (strlist[2] == "2")
	{
		QTreeWidgetItem * deleteItem = TabWidgetCollection::getInstance()->saveData(mSaveDataIsFromRightMenu);
		mSaveDataIsFromRightMenu = false;
		if (deleteItem != NULL)
		{
			//	TabWidgetCollection::getInstance()->setIsPopMessageBox(false);
			QTreeWidgetItem *parItem = deleteItem->parent();
			if (m_pMainMenutempleteTreeWidget->isVisible() == true)
			{
				m_pMainMenutempleteTreeWidget->setCurrentItem(deleteItem);
			}
			else
			{
				m_pMainMenuTreeWidget->setCurrentItem(deleteItem);
			}
			deleteTreeNodeAndTab(true);

			if (m_pMainMenutempleteTreeWidget->isVisible() == true)
			{
				m_pMainMenutempleteTreeWidget->setCurrentItem(m_pPreItem);
			}
			else
			{
				m_pMainMenuTreeWidget->setCurrentItem(m_pPreItem);
			}
		}
	}
	if (m_bIsClickCanccel)
	{
		m_bIsClickCanccel = false;
		return;
	}
	else
	{
		QString stringName = m_pPreItem->text(0);
		QString ModifyName = stringName.replace("*", "");
		ModifyName = ModifyName + "*";
		m_pPreItem->setText(0, ModifyName);
		TabWidgetCollection::getInstance()->modifyTabName(stringName);
		//修改其状态
		QVariant var = m_pPreItem->data(0, Qt::UserRole);
		QStringList list = var.toStringList();
		QString   isModify = list[2];
		dapper(false);//去掉整洁显示
//		m_pLayoutAct->setChecked(false);
		if (isModify != "0")
		{
			QStringList list1;
			if (list.length() == 4)//病程记录的特点
			{
				isModify = "1";
				list1 << list[0] << list[1] << isModify << list[3];
				var.clear();
				var.setValue(list1);
				setMenuStatus(true);
			}
		}
		else
		{
			var.clear();
			list.removeAt(2);
			isModify = "1";
			list << isModify;
			var.setValue(list);
			if (m_pMainMenuTreeWidget->isVisible() == true)
			{
				setMenuStatus(true);
			}
			else
			{
				setMenuStatus(true);
			}
		}
		m_pPreItem->setData(0, Qt::UserRole, var);
	//	modifyNode(widget);
	}
}


void MainViewContainer::setTreeCurrentItem(QString strItem)
{
	bool bIsFindItem = false;
	QTreeWidget *widget = NULL;
	if (m_pMainMenuTreeWidget->isVisible() == true)
	{
		widget = m_pMainMenuTreeWidget;
	}
	else
	{
		widget = m_pMainMenutempleteTreeWidget;
	}

	QList<QTreeWidgetItem*> itemList_ = widget->findItems("(02)病程记录", Qt::MatchFlag::MatchExactly);
	if (itemList_.size() <= 0)
	{
		return;
	}
	QTreeWidgetItem* item_;
	for (int i = 0; i < itemList_[0]->childCount(); i++)
	{
		item_ = itemList_[0]->child(i);
		QStringList strlist = item_->data(0, Qt::UserRole).toStringList();
		if (strlist[0] == strItem)
		{
			m_pPreItem = item_;
			bIsFindItem = true;
			break;
		}
	}
	if (bIsFindItem == false)
	{
		return;
	}


	widget->setCurrentItem(item_);
	//新增的模版或者病历点击了编辑
}

void MainViewContainer::aboutVersion()
{
	EMRVersion emrVersion_;
	emrVersion_.exec();
}


void MainViewContainer::adminLogin()
{
	saveTreeNodeAnadTab();
	AdminLogin adminLogin_;
	m_bIsAdminUser = adminLogin_.exec();
}


void MainViewContainer::submitCurrent()
{
	int a = 0;
}

void MainViewContainer::submitMultipl()
{
	int a = 0;
}

void MainViewContainer::submitMdc(QString id)
{
	if (m_bIsAdminUser)
	{
		MessageBox::showAboutMessageBox("提示", "当前为管理员权限，无法进行该操作，请退出登录后重新提交！");
		return;
	}
	QString sql_;
	QString mdcAuthor_;
	auto it = m_stlMdcAth.find(id);
	if (it == m_stlMdcAth.end())
	{
		char msg[50];
		sprintf(msg, "can't find pid: %s .", id);
		qDebug(msg);
		return;
	}
	MdcAuthority & mdcAth_ = *it;
	if (mdcAth_.mdcAuthor.size() == 0 || mdcAth_.mdcAuthor.trimmed() == "0")
	{
		mdcAuthor_ = "1";//主治
	}
	else if (mdcAth_.mdcAuthor.trimmed() == "1")
	{
		mdcAuthor_ = "2";//主任
	}
	else if (mdcAth_.mdcAuthor.trimmed() == "3")
	{
		mdcAuthor_ = "3";//
	}
	else
	{
		return;
	}
	sql_ = "update MEDICALRECORDS_LIST t set t.isoptions = '" + mdcAuthor_ + "' where t.id = '" + id + "'";
	if (m_pDataBaseControl->insertData(sql_))
	{
 		MessageBox::showAboutMessageBox("提示", "提交病历成功");
		mdcAth_.mdcAuthor = mdcAuthor_;
		bool ok_;
		int nAuthor = mdcAuthor_.toInt(&ok_, 10);
		if (!ok_)
		{
			nAuthor = -1;
		}
		modifyTree(id, nAuthor);
	}
}


void MainViewContainer::modifyTree(QString id, int grade)
{
	QTreeWidget *widget = NULL;
	if (m_pMainMenuTreeWidget->isVisible() == true)
	{
		widget = m_pMainMenuTreeWidget;
	}
	else
	{
		widget = m_pMainMenutempleteTreeWidget;
	}
	QTreeWidgetItem * qTreeItem_ = NULL;
	QTreeWidgetItemIterator it(widget);
	while (*it)
	{
		//do something like
		QString text_ = (*it)->text(0);
		QStringList strlist = (*it)->data(0, Qt::UserRole).toStringList();
		if (strlist.size() != 0)
		{
			if (strlist[0] == id)
			{
				qTreeItem_ = (*it);
				break;
			}
		}
		++it;
	}
	if (qTreeItem_ == NULL)
	{
		qDebug("this point(qTreeItem) is NULL");
		return;
	}
	modifyTreeItem(qTreeItem_, grade);
	
}


void MainViewContainer::modifyTreeItem(QTreeWidgetItem * qTreeItem, int grade)
{
#ifdef _CHAO_YANG_
	QBrush brush_;

	QString strText_ = qTreeItem->toolTip(0);
	int index_ = strText_.lastIndexOf("已提交");
	if (index_ != -1)
	{
		strText_ = strText_.mid(0, index_);
	}
	switch (grade)
	{
	case 1:
		strText_ += "\r\n已提交上级主治医生:";
		strText_ += m_wirteAth.RhtDctrName;
		brush_ = QBrush(QColor("#EEAEEE"));
		break;
	case 2:
	case 3:
		strText_ += "\r\n已提交上级主任医生:";
		strText_ += m_wirteAth.ChfDctrName;
		brush_ = QBrush(QColor("#EE2C2C"));
		break;
	default:
		break;
	}
	if (grade > 0)
	{
		qTreeItem->setToolTip(0, strText_);
		qTreeItem->setBackground(0, brush_);
	}
#endif
}

void MainViewContainer::setPrompts(QString id)
{
#ifdef _CHAO_YANG_

	auto it = m_stlMdcAth.find(id);
	QString strTitle;
	if (it != m_stlMdcAth.end())
	{
		MdcAuthority mdcAth_ = *it;
		QString strAth_;
		bool ok_;
		int nMdcAth_ = mdcAth_.mdcAuthor.toInt(&ok_, 10);
		switch (nMdcAth_)
		{
		case 0:
			strAth_ = "医生:" + m_pDataBaseControl->m_docutorData->user_name;
			break;
		case 1:
			strAth_ = "主治医生:" + m_wirteAth.RhtDctrName;
			break;
		case 2:
		case 3:
			strAth_ = "主任医生:" + m_wirteAth.ChfDctrName;
			break;
		default:
			strAth_ = "主任医生:" + m_wirteAth.ChfDctrName;
			break;
		}
		strTitle = "病人姓名:" + m_pDataBaseControl->m_patientData->name + "\t\t性别:"
			+ m_pDataBaseControl->m_patientData->sex
			+ "\t年龄:" + m_pDataBaseControl->m_patientData->age
			+ "\t主诊断:" + m_pDataBaseControl->m_patientData->current_diag_name
			+ "\t\t当前病历权限提交给" + strAth_;
	}
	else
	{
		strTitle = "病人姓名:" + m_pDataBaseControl->m_patientData->name + "\t\t性别:"
			+ m_pDataBaseControl->m_patientData->sex
			+ "\t年龄:" + m_pDataBaseControl->m_patientData->age
			+ "\t主诊断:" + m_pDataBaseControl->m_patientData->current_diag_name;
	}
	m_pTopMsgTitle->setText(strTitle);
#endif
}