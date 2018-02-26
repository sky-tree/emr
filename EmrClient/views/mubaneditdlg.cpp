#include "mubaneditdlg.h"
#include <QPushButton>
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QStandardItem>
#include <QApplication>
#include <QTextEdit>
#include <QLabel>
#include <QStringListModel>
#include <QListView>
#include <QLineEdit>
#include <QHeaderView>
#include <QStringList>
#include <QRadioButton>
#include"qwidget.h"
#include "globalvariable.h"
#include"qcheckbox.h"
#include"qmessagebox.h"
#include "MubanEditManagement.h"
#include"dataModel\OperateData\DataBaseControl.h"
#include <qaction.h>
#include <QMenu>
#include "MubanPropertyModifyDlg.h"
#include <QMessageBox>
#include<QKeyEvent>
#include "MessageBox.h"
#define MAX_DLG_SIZE 963, 513

MuBanEditDlg::MuBanEditDlg(QWidget *parent) : QDialog(parent)
, addBtn(NULL), modifyBtn(NULL), deleteBtn(NULL), quitBtn(NULL), searchBtn(NULL), elementEditBtn(NULL)
, lineEdit(NULL), nameLabel(NULL), listLabel(NULL), mubanLabel(NULL), m_DataBaseControl(NULL), model(NULL)
, listView(NULL), tableView(NULL), templateModel(NULL), changeAction(NULL)
{
	isAllTempAdd = false;
	radioContainerWidget = NULL;
	radioHBoxLayout = NULL;
	allBtn = NULL;
	publicBtn = NULL;
	departmentBtn = NULL;
	privateBtn = NULL;

	isEdit = false;
	isEditRight = false;
	isAddTemplate = false;
	//if (m_DbManager == NULL)
	//{
	//	m_DbManager = new DBManager();
	//	m_DbManager->connectOracle();
	//}
	if (m_DataBaseControl == NULL)
	{
		m_DataBaseControl = new DataBaseControl(NULL);
	}
	this->setWindowTitle(tr("模板编辑管理"));
	this->setModal(true);
	this->resize(MAX_DLG_SIZE);
	this->setMinimumSize(MAX_DLG_SIZE);
	this->setMaximumSize(MAX_DLG_SIZE);
	this->createWidgets();
	this->loadListData();
	this->setFocus();
	connect(listView, SIGNAL(clicked(QModelIndex)), this, SLOT(Clicked(QModelIndex)));
	connect(tableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(rightClick(QPoint)));
	connect(tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tabWidgetDoubleClick(int, int)));
	tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	changeAction = new QAction(tr("修改(&C)"), this);
	//changeAction->setShortcuts(QKeySequence::New);
	changeAction->setStatusTip(tr("修改当前选中行属性值"));
	connect(changeAction, SIGNAL(triggered()), this, SLOT(onItemRightClicked()));

	lineEdit->installEventFilter(this);
	setStartUpRight();
	//setItemEdit(m_DataBaseControl->m_docutorData->isSupperUser);

	//connect(ascendSortAction, SIGNAL(triggered(bool)), this, SLOT(sort_ascend()));
	//connect(descendSortAction, SIGNAL(triggered(bool)), this, SLOT(sort_descend()));
	//connect(filterAction, SIGNAL(triggered(bool)), this, SLOT(filter_table()));
	//connect(unfilterAction, SIGNAL(triggered(bool)), this, SLOT(unfilter_table()));

	//this->reloadTableData();
	//this->setWindowIcon(QIcon(exeImagePath));
	//this->setAttribute(Qt::WA_DeleteOnClose, true);//设置关闭时自动析构	
	//operatorTemplateRight oTR = (*(DataBaseControl::m_operatorTemplate))[strMapType];
	if (DataBaseControl::m_operatorTemplate->count() > 0)
	{
		QMapIterator<QString, operatorTemplateRight> mapTemplate(*(DataBaseControl::m_operatorTemplate));
		while (mapTemplate.hasNext())
		{
			operatorTemplateRight oTR = (*(DataBaseControl::m_operatorTemplate))[mapTemplate.next().key()];
			if (oTR.addTemplate)
			{
				isAddTemplate = true;
			}
		}
		//isAddTemplate = true;
		//addBtn->setEnabled(true);
	}
	else
	{
		isAddTemplate = false;
		//addBtn->setEnabled(false);
	}
	if (m_DataBaseControl->m_docutorData->isSupperUser)
	{
		isAddTemplate = true;
		//addBtn->setEnabled(true);
	}
	this->setWindowIcon(QIcon(":/res/Res/MenuIcon/case.png"));
	
}
void MuBanEditDlg::tabWidgetDoubleClick(int row, int column)
{
	QString path = tableWidget->item(row, 5)->text();
	this->map.clear();
	this->map.insert("operateType", "Read");
	this->map.insert("path", path);
	emit QDialog::accept();
	this->close();
}
void MuBanEditDlg::closeEvent(QCloseEvent *e)
{

}
QString MuBanEditDlg::mappingType(QString type)
{
	QString mappingType = "";
	if (type == "1")
	{

		mappingType = "3";
	}
	else if (type == "2")
	{ 
		mappingType = "2"; 
	}
	else if (type == "3")
	{ 
		mappingType = "1"; 
	}
	return mappingType;
}
//////////////////////////////////////////////////////////////////////////
//type:1:修改，2:删除
//////////////////////////////////////////////////////////////////////////
bool MuBanEditDlg::getEditRight(int pType)
{
	QString createId = "";
	QString id = m_DataBaseControl->m_docutorData->user_id;
	bool  isSuperUser = m_DataBaseControl->m_docutorData->isSupperUser;
	QString classifyCode = "";

	int row = tableWidget->currentRow();
	if (row != -1)
	{
		createId = tableWidget->item(row, 7)->text();
		classifyCode = tableWidget->item(row, 0)->text();
	}
	if (/*id == createId ||*/ isSuperUser)
	{
		return true;
	}
	else
	{
		//if (classifyCode != ""&&DataBaseControl::m_operatorMedical->contains(classifyCode))
		//{
		QString dataId = tableWidget->item(row, 6)->text();
		QString sql = "select TYPE from medicalrecords_template_list where ID='" + dataId + "'";
		QString type = m_DataBaseControl->getId(sql);
		QString strMapType = mappingType(type);
		if (DataBaseControl::m_operatorTemplate->contains(strMapType))
		{
			operatorTemplateRight oTR = (*(DataBaseControl::m_operatorTemplate))[strMapType];
			bool isEditRight = false;
			switch (pType)
			{
			case 1:
				isEditRight = oTR.modifyTemplate;
				break;
				//删除
			case 2:
				isEditRight = oTR.deleteTemplate;
				break;
			default:
				break;
			}
			return isEditRight;
		}

	}
	return false;
}
void MuBanEditDlg::onPushBtnClicked()
{
	QObject * sender = this->sender();
	if (sender == addBtn)
	{
		if (isAddTemplate == false)
		{
			MessageBox::showErrorMessageBox("提示", "你无权新增模板!");
			return;
		}
		AddMuBanDlg dlg;// = new AddMuBanDlg;
		InitAddTemplate(&dlg);
		//dlg->show();
		if (dlg.exec() == QDialog::Accepted)
		{
			this->map.clear();
			this->map = dlg.getData();
			emit QDialog::accept();
			//dlg->close();
		}
	}
	if (sender == deleteBtn)
	{
		int row = tableWidget->currentRow();
		if (row == -1)
		{
			MessageBox::showErrorMessageBox("提示", "请先选择模板!");
			return;
		}
		QString createId = tableWidget->item(row, 7)->text();
		QString id = m_DataBaseControl->m_docutorData->user_id;
		bool  isSuperUser = m_DataBaseControl->m_docutorData->isSupperUser;
		bool isRightEdite = getEditRight(2);
		if (/*createId != id&&*/!isSuperUser&&!isRightEdite)
		{
			MessageBox::showErrorMessageBox("警告", "你无权删除当前模板!");
			return;
		}
		if (MessageBox::showQueryMessageBox("删除", "确定删除模板删除后将不可恢复!") == QMessageBox::Yes)
		{
			QRadioButton *rb = getCheckedRadioButton();
			//书写器预留
			QString EmrId = tableWidget->item(row, 5)->text();
			m_DataBaseControl->deleteMedicalData(EmrId, true);
			tableWidget->removeRow(row);
			tableWidget->setCurrentCell(-1, -1);
			Clicked(listView->currentIndex());
			if (rb != NULL)
			{
				rb->setChecked(true);
			}
		}
	}
	//修改模板
	if (sender == modifyBtn)
	{

		int row = tableWidget->currentRow();
		if (row == -1)
		{
			MessageBox::showErrorMessageBox("提示", "请先选择需要修改的模板!");
			return;
		}
		QString createId = tableWidget->item(row, 7)->text();
		QString id = m_DataBaseControl->m_docutorData->user_id;
		bool  isSuperUser = m_DataBaseControl->m_docutorData->isSupperUser;
		bool isRightEdite = getEditRight(1);
		if (/*createId != id&&*/!isSuperUser&&!isRightEdite)
		{
			MessageBox::showErrorMessageBox("警告", "你无权修改当前模板!");
			return;
		}
		QString path = tableWidget->item(row, 5)->text();
		this->map.clear();
		this->map.insert("operateType", "Modify");
		this->map.insert("path", path);
		emit QDialog::accept();
		this->close();
	}

	if (sender == elementEditBtn)
	{
		/*MubanEditManagement * memDlg = new MubanEditManagement();
		memDlg->show();
		this->close();*/
	}
	if (sender == startBtn)
	{
		modifyData();
		Clicked(listView->currentIndex());
	}

	if (sender == searchBtn)
	{
		//根据条件 搜索对应的数据

		if (lineEdit->text() != "")
		{
			QString sql;
			if (isAllTempAdd)
			{
				sql = "select * from MEDICALRECORDS_TEMPLATE_LIST where NAME like '%" + lineEdit->text() + "%'";
			}
			else
			{
				sql = "select * from MEDICALRECORDS_TEMPLATE_LIST where NAME like '%" + lineEdit->text() + "%'" + " AND(DEPARTMENT_CODE = '0000' OR DEPARTMENT_CODE = '" + DataBaseControl::m_docutorData->dept_code + "')";
			}
			if (model != NULL)
			{
				delete model;
			}
			model = NULL;
			model = &m_DataBaseControl->getQueryData(sql);
			allBtn->setChecked(true);
			reloadTableData();
		}
		else
		{
			QModelIndex index = listView->currentIndex();
			Clicked(index);
			//QMessageBox::information(NULL, tr("提示"), tr("请输入模板名称"), tr("确定"));
		}
	}
	if (sender == quitBtn)
	{
		emit QDialog::reject();
		this->close();
	}
}
void MuBanEditDlg::InitAddTemplate(AddMuBanDlg* dlg)
{
	QMap<QString, QString> var;
	QVariant name = listView->currentIndex().data();
	var.insert("businessName", name.toString());
	dlg->init(var);
}
#define CREATE_PUSH_BTN(objName, text, rect) \
	objName = new QPushButton(this); \
	objName->setText(tr(text)); \
	objName->setGeometry(rect); \
	objName->setStyleSheet(QStringLiteral("QPushButton{border-width:1px;border-color:rgb(127,192,224);background-color:rgb(219,239,246);color:rgb(0,0,0);}QPushButton:hover{background-color:rgb(185,227,247);}QPushButton:pressed{background-color:rgb(157, 218, 246);}")); \
	connect(objName, SIGNAL(clicked()), this, SLOT(onPushBtnClicked()));

#define CREATE_LABEL(labelName,text,rect)\
	labelName = new QLabel(this); \
	labelName->setText(tr(text)); \
	labelName->setGeometry(rect); \
	labelName->setFont(labelFont); \
	labelName->setStyleSheet(QStringLiteral("")); \
	labelName->setAlignment(Qt::AlignLeft);
void MuBanEditDlg::createWidgets()
{
	CREATE_PUSH_BTN(addBtn, "新增模板", QRect(20, 20, 76, 31))
		CREATE_PUSH_BTN(modifyBtn, "修改模板", QRect(140, 20, 76, 31))
		CREATE_PUSH_BTN(deleteBtn, "删除模板", QRect(260, 20, 76, 31))
		CREATE_PUSH_BTN(elementEditBtn, "元素编辑", QRect(380, 20, 76, 31))
		elementEditBtn->setVisible(false);
	elementEditBtn->setEnabled(false);
	CREATE_PUSH_BTN(quitBtn, "退出(Q)", QRect(380, 20, 76, 31))
		CREATE_PUSH_BTN(searchBtn, "查询", QRect(754, 20, 81, 31))
		addBtn->installEventFilter(this);
	modifyBtn->installEventFilter(this);//修改模板
	deleteBtn->installEventFilter(this);//删除模板
	searchBtn->setDefault(true);
	this->setStyleSheet(kPushBtnStyle);

	listLabel = new QLabel("电子病历目录", this);

	listLabel->setGeometry(QRect(20, 70, 183, 29));
	listLabel->setStyleSheet(QStringLiteral("background-color: rgb(137, 203, 235);color:white;font:bold 14px"));

	//mubanLabel = new QLabel("模板信息", this);
	//   mubanLabel->setGeometry(QRect(220, 71, 721, 29));
	//   mubanLabel->setFont(labelFont);
	//   mubanLabel->setStyleSheet(QStringLiteral("background-color: rgb(137,203,235);color:white"));

	nameLabel = new QLabel("模板名称:", this);
	nameLabel->setGeometry(QRect(504, 20, 90, 31));
	nameLabel->setStyleSheet("border-width:1px;border-style:solid;border-color:rgb(127,192,224);background-color:rgb(219,239,246);color:rgb(0,0,0);");
	nameLabel->setAlignment(Qt::AlignCenter);
	//860, 20, 81, 31

	listView = new QListView(this);
	listView->setGeometry(QRect(20, 99, 183, 391));

	lineEdit = new QLineEdit(this);
	lineEdit->setGeometry(QRect(594, 20, 151, 31));

	//tableView = new QTableView(this);
	//tableView->setGeometry(QRect(220, 150, 721, 341));
	tableWidget = new QTableWidget(this);
	tableWidget->setGeometry(QRect(220, 99, 721, 392));
	tableWidget->setColumnCount(8);
	this->setItemEdit(m_DataBaseControl->m_docutorData->isSupperUser);
	tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QStringList headers;
//	headers << "分类代码" << "模板代码" << "模板名称" << "使用科室" << "时间质控" << "书写质控" << "创建人" << "是否启用";
	headers << "分类代码" << "模板名称" << "使用科室" << "创建人" << "是否启用";
	tableWidget->setHorizontalHeaderLabels(headers);
	tableWidget->horizontalHeader()->setDefaultSectionSize(88);
	tableWidget->horizontalHeader()->setEnabled(true);
	//QHeaderView::ResizeToContents
	//tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	//tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableWidget->horizontalHeader()->setHighlightSections(false);
	//tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	tableWidget->horizontalHeader()->setStretchLastSection(true);
	//*************************************************************
	//废弃 by caokewen 2016/8/19
	//CREATE_LABEL(typeCodeLabel, "类别代码:", QRect(220, 80, 121, 29))
	//	typeCodeEdit = new QLineEdit(this);
	//typeCodeEdit->setGeometry(QRect(285, 71, 120, 30));

	//CREATE_LABEL(typeNameLabel, "类别名称:", QRect(435, 80, 121, 29))
	//	typeNameEdit = new QLineEdit(this);
	//typeNameEdit->setGeometry(QRect(500, 71, 120, 30));

	//CREATE_LABEL(dapartmentLabel, "使用科室:", QRect(650, 80, 121, 29))
	//	dapartmentEdit = new QLineEdit(this);
	//dapartmentEdit->setGeometry(QRect(715, 71, 120, 30));

	//CREATE_LABEL(blockNameLabel, "模块名称:", QRect(220, 115, 121, 29))
	//	blockNameEdit = new QLineEdit(this);
	//blockNameEdit->setGeometry(QRect(285, 106, 120, 30));

	//CREATE_LABEL(writeBlockLabel, "书写质控:", QRect(435, 115, 121, 29))
	//	writeBlockEdit = new QLineEdit(this);
	//writeBlockEdit->setGeometry(QRect(500, 106, 120, 30));

	//CREATE_LABEL(timeBlockLabel, "时间质控:", QRect(650, 115, 121, 29))
	//	timeBlockEdit = new QLineEdit(this);
	//timeBlockEdit->setGeometry(715, 106, 120, 30);

	startBtn = new QPushButton(this);
	startBtn->setText("启用");
	startBtn->setGeometry(850, 20, 76, 31);
	startBtn->setStyleSheet("QPushButton{border-width:1px;border-color:rgb(127,192,224);background-color:rgb(219,239,246);color:rgb(0,0,0);}QPushButton:hover{background-color:rgb(185,227,247);}QPushButton:pressed{background-color:rgb(157, 218, 246);}");
	connect(startBtn, SIGNAL(clicked()), this, SLOT(onPushBtnClicked()));
	startBtn->installEventFilter(this);

	radioContainerWidget = new QWidget(this);
	radioContainerWidget->setGeometry(QRect(220, 61, 721, 40));
	radioHBoxLayout = new QHBoxLayout();
	allBtn = new QRadioButton("所有模板");
	publicBtn = new QRadioButton("全院模板");
	departmentBtn = new QRadioButton("科室模板");
	privateBtn = new QRadioButton("个人模板");
	allBtn->setMinimumSize(90, 30);
	allBtn->setMaximumSize(90, 30);
	publicBtn->setMinimumSize(90, 30);
	publicBtn->setMaximumSize(90, 30);
	departmentBtn->setMinimumSize(90, 30);
	departmentBtn->setMaximumSize(90, 30);
	privateBtn->setMinimumSize(90, 30);
	privateBtn->setMaximumSize(90, 30);
	radioContainerWidget->setLayout(radioHBoxLayout);
	radioHBoxLayout->addStretch(1);
	radioHBoxLayout->addWidget(allBtn);
	radioHBoxLayout->addWidget(publicBtn);
	radioHBoxLayout->addWidget(departmentBtn);
	radioHBoxLayout->addWidget(privateBtn);
	radioHBoxLayout->addStretch(1);
	allBtn->setChecked(true);
	connect(allBtn, SIGNAL(clicked()), this, SLOT(onRadioBtnToggled()));
	connect(publicBtn, SIGNAL(clicked()), this, SLOT(onRadioBtnToggled()));
	connect(departmentBtn, SIGNAL(clicked()), this, SLOT(onRadioBtnToggled()));
	connect(privateBtn, SIGNAL(clicked()), this, SLOT(onRadioBtnToggled()));
}

void MuBanEditDlg::loadListData()
{
	if (m_DataBaseControl == NULL)
	{
		return;
	}
	//templateModel->setTable("MEDICALRECORDS_TEMPLATE_TYPE");
	if (templateModel)
	{
		delete templateModel;
		templateModel = NULL;
	}
	templateModel = m_DataBaseControl->getTableModel("1", "MEDICALRECORDS_TEMPLATE_TYPE");
	templateModel->setFilter("1=1 order by CLASSIFYCODE");
	templateModel->select();
	int rows = templateModel->rowCount();
	QStringList titleList;
	for (int i = 0; i < templateModel->rowCount(); i++)
	{
		QSqlRecord record = templateModel->record(i);
		QString name = record.value("NAME").toString();
		QString classifyCode = record.value("CLASSIFYCODE").toString();
		QString nameClass = "(" + classifyCode + ")" + name;
		titleList.append(nameClass);
		if (sqlMap.contains(name) == false)
		{
			sqlMap.insert(name, classifyCode);
		}
	}
	/*QStringListModel **/ strModel = new QStringListModel(titleList);
	listView->setModel(strModel);
	listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QModelIndex index = strModel->index(0);
	listView->setCurrentIndex(index);
	Clicked(index);
	//delete templateModel;
	//templateModel = NULL;
	//listView->setStyleSheet(kListViewStyle);
}
void MuBanEditDlg::reloadTableData()
{
	if (model == NULL)
	{
		return;
	}
	int rows = model->rowCount();
	//tableWidget->setRowCount(rows);
	tableWidget->clear();
	tableWidget->setRowCount(0);
	QVariant var;
	for (int i = 0; i < model->rowCount(); i++)
	{
		QSqlRecord record = model->record(i);
		QString temType = record.value("TYPE").toString();
		if (allBtn->isChecked() || (publicBtn->isChecked() && temType == "1") || (departmentBtn->isChecked() && temType == "2") || (privateBtn->isChecked() && temType == "3"))
		{
			QString code = record.value("CODE").toString();
			QString classify = record.value("CLASSIFYCODE").toString();
			QString name = record.value("NAME").toString();
			QString department = record.value("DEPARTMENT_NAME").toString();
			QString timeControl = "";
			QString writeControl = "";
			QString author = record.value("CREATE_NAME").toString();
			QString isEnable = record.value("ISENABLE").toString();
			QString data = record.value("DATA").toString();
			QString ID = record.value("ID").toString();
			QString createId = record.value("CREATE_ID").toString();

			int location = tableWidget->rowCount();
			tableWidget->insertRow(location);
			tableWidget->setItem(location, 0, new QTableWidgetItem(classify));
/*			tableWidget->setItem(location, 1, new QTableWidgetItem(code));*/
			tableWidget->setItem(location, 1, new QTableWidgetItem(name));
			QTableWidgetItem *tipItem = tableWidget->item(location, 1);
			tipItem->setToolTip(tipItem->text());
 			tableWidget->setItem(location, 2, new QTableWidgetItem(department));
// 			tableWidget->setItem(location, 4, new QTableWidgetItem(timeControl));
// 			tableWidget->setItem(location, 5, new QTableWidgetItem(writeControl));
			tableWidget->setItem(location, 3, new QTableWidgetItem(author));
#pragma region 启用控件



			QWidget *widget;
			QHBoxLayout *hLayout;
			QCheckBox *ckb;

			ckb = new QCheckBox();

			hLayout = new QHBoxLayout();
			widget = new QWidget(tableWidget);
			hLayout->addWidget(ckb);
			hLayout->setMargin(0);
			hLayout->setAlignment(ckb, Qt::AlignCenter);
			widget->setLayout(hLayout);

			if (isEnable == "Y")
			{
				ckb->setCheckState(Qt::Checked);
			}
			else
			{
				ckb->setCheckState(Qt::Unchecked);
			}
#pragma endregion
			tableWidget->setCellWidget(location, 4, widget);
			tableWidget->setItem(location, 5, new QTableWidgetItem(data));
			tableWidget->setItem(location, 6, new QTableWidgetItem(ID));
			tableWidget->setItem(location, 7, new QTableWidgetItem(createId));
		}
	}

	QStringList headers;
//	headers << "分类代码" << "模板代码" << "模板名称" << "使用科室" << "时间质控" << "书写质控" << "创建人" << "是否启用";
	headers << "分类代码" << "模板名称" << "使用科室" << "创建人" << "是否启用";
	tableWidget->setHorizontalHeaderLabels(headers);

	tableWidget->setColumnHidden(5, true);
	tableWidget->setColumnHidden(6, true);
	tableWidget->setColumnHidden(7, true);
}

MuBanEditDlg::~MuBanEditDlg()
{
	if (m_DataBaseControl)
	{
		delete m_DataBaseControl;
		m_DataBaseControl = NULL;
	}
	destory();
}
void MuBanEditDlg::setStartUpRight()
{
	QList<QString> list = DataBaseControl::m_operatorTemplate->keys();
	QListIterator<QString> listIterator(list);
	bool isStartUp = false;
	QString str = "";
	while (listIterator.hasNext())
	{
		str = listIterator.next();
		isStartUp = (*(DataBaseControl::m_operatorTemplate))[str].startUpTemplate;
		if (isStartUp)
		{
			isEditRight = true;
			isEdit = true;
			break;
		}
	}
	if (m_DataBaseControl->m_docutorData->isSupperUser)
	{
		isEdit = true;
		isStartUp = true;
		isEditRight = true;
	}
	startBtn->setEnabled(isStartUp);
	startBtn->setVisible(isStartUp);
	setItemEdit(isEdit);
}
void MuBanEditDlg::Clicked(const QModelIndex &index)
{
	int curRow = index.row();
	QString name = templateModel->record(curRow).value("NAME").toString();
	if (sqlMap.contains(name))
	{
		QString pid = sqlMap[name];
		QMap<QString,operatorTemplateRight>* map = m_DataBaseControl->m_operatorTemplate;
		QString sql;
		if ( map->contains("3")&& (map->value("3").addTemplate) == true )
		{
			sql = "select * from MEDICALRECORDS_TEMPLATE_LIST where CLASSIFYCODE='" + pid + "'";
			isAllTempAdd = true;
		}
		else
		{
			sql = "select * from MEDICALRECORDS_TEMPLATE_LIST where CLASSIFYCODE='" + pid + "'"
				+ " AND(DEPARTMENT_CODE = '0000' OR DEPARTMENT_CODE = '" + DataBaseControl::m_docutorData->dept_code + "')";
			isAllTempAdd = false;
		}	
		if (model)
		{
			delete model;
			model = NULL;
		}
		model = NULL;
		model = &m_DataBaseControl->getQueryData(sql);
		//allBtn->setChecked(true);
		reloadTableData();
		setItemEdit(isEdit);
		
	}

}
/*外部接口，设置列是否可编辑*/
void MuBanEditDlg::setItemEdit(bool isFlags)
{
	this->isEdit = isFlags;
	if (isFlags == false && isEditRight == false)
	{
		tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
		for (int i = 0; i < tableWidget->rowCount(); i++)
		{
			QWidget *widget = tableWidget->cellWidget(i, 4);
			QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
			ckb->setEnabled(false);
		}
	}
	else
	{
		for (int i = 0; i < tableWidget->rowCount(); i++)
		{
			//setItemState(tableWidget->item(i, 3), i, 3);
			//setItemState(tableWidget->item(i, 4), i, 4);
			//setItemState(tableWidget->item(i, 5), i, 5);
			//setItemState(tableWidget->item(i, 6), i, 6);
			QWidget *widget = tableWidget->cellWidget(i, 4);
			QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
			ckb->setEnabled(true);
		}
	}
}

void MuBanEditDlg::setItemState(QTableWidgetItem *item, int row, int column)
{
	//if (item == NULL)return;
	//QTableWidgetItem *item = tableWidget->item(row, column);
	if (item)
	{
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	}
	else
	{
		item = new QTableWidgetItem;
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(row, column, item);
	}
}

bool MuBanEditDlg::modifyData()
{
	bool isModify = false;
	bool isSaveData = false;
	int flags = 0;//判断是否点击了保存数据，messagbox定位只弹出一次
	if (model == NULL)return isModify;
	QList<QStringList> list;
	for (int i = 0; i < model->rowCount(); i++)
	{
		QSqlRecord record = model->record(i);
		QString ID = record.value("ID").toString();
		QString isEnable = record.value("ISENABLE").toString();
		QStringList strList;
		strList << ID << isEnable;
		list.append(strList);
	}
	int rows = list.length();
	for (int j = 0; j < tableWidget->rowCount(); j++)
	{
		QString tableID = tableWidget->item(j, 6)->text();
		int testCount = model->rowCount();
		for (int i = 0; i < list.length(); i++)
		{
			QStringList strList = list.at(i);
			QString ID = strList[0];
			QString isEnable = strList[1];
			if (tableID == ID)
			{
				QWidget *widget = tableWidget->cellWidget(j, 4);
				QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
				QString isFalg = "N";
				if (ckb->isChecked())
				{
					isFalg = "Y";
				}
				//如果没改动则退出tableWiget当前行的判断
				if (isEnable == isFalg)
				{
					break;
				}
				else
				{
					isModify = true;
					if (flags == 0)
					{
						isSaveData = isSave();
						flags = 1;
					}
					if (isSaveData == true)
					{
						QString sql = "update MEDICALRECORDS_TEMPLATE_LIST set ISENABLE='" + isFalg + "'where ID='" + tableID + "'";
						//m_DataBaseControl->getQueryData(sql);
						m_DataBaseControl->getId(sql);
						//isSaveData = false;
					}
				}
				break;
			}
		}
	}
	return isModify;
}

bool MuBanEditDlg::isSave()
{
	if (MessageBox::showQueryMessageBox("修改数据", "数据有改动，是否保存") == QMessageBox::Yes)
	{
		return true;
	}
	return false;
}

void MuBanEditDlg::rightClick(const QPoint& ps)
{
// 	QMenu *popMenu = new QMenu(this);//定义一个右键弹出菜单
// 	popMenu->addAction(changeAction);
// 	changeAction->setProperty("pos", ps);
// 	popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
}

void MuBanEditDlg::onItemRightClicked()
{
	QStringList strList;
	int row = tableWidget->rowAt(changeAction->property("pos").toPoint().y());
	QString createId = tableWidget->item(row, 6)->text();
	QString id = m_DataBaseControl->m_docutorData->user_id;
	bool  isSuperUser = m_DataBaseControl->m_docutorData->isSupperUser;
	bool isRightEdite = getEditRight(1);
	if (/*createId != id*/!isSuperUser&&!isRightEdite)
	{
		MessageBox::showErrorMessageBox("警告", "你无权修改当前模板!");
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		strList << tableWidget->item(row, i)->text();
	}

	QStringList temList = strList;
	MubanPropertyModifyDlg::changeData(strList);

	if (temList[0] != strList[0])
	{
		QString tableID = tableWidget->item(tableWidget->rowAt(changeAction->property("pos").toPoint().y()), 6)->text();
		QString sql = "update MEDICALRECORDS_TEMPLATE_LIST set CLASSIFYCODE='" + strList[0] + "' ,code='',NAME='" + strList[1] /*+ "',DEPARTMENT_NAME='" + strList[3] */ + "' where ID='" + tableID + "'";
		//m_DataBaseControl->getQueryData(sql);
		m_DataBaseControl->getId(sql);
		loadListData();
	}
	else
	{
		bool isChanged = false;
		for (int i = 1; i < strList.size(); i++)
		{
			if (temList[i] != strList[i])
			{
				isChanged = true;
				break;
			}
		}

		if (isChanged)
		{
			QString tableID = tableWidget->item(tableWidget->rowAt(changeAction->property("pos").toPoint().y()), 6)->text();
			QString sql = "update MEDICALRECORDS_TEMPLATE_LIST set code='',NAME='" + strList[1] + /*"',DEPARTMENT_NAME='" + strList[3] + "',DEPARTMENT_CODE='" + strList[4]*/ + "' where ID='" + tableID + "'";
			//m_DataBaseControl->getQueryData(sql);
			m_DataBaseControl->getId(sql);
			for (int i = 1; i < strList.size() - 1; i++)
			{
				tableWidget->item(row, i)->setText(strList[i]);
				QTableWidgetItem *tipItem = tableWidget->item(row, 1);
				tipItem->setToolTip(tipItem->text());
			}
		}
	}
	if (model != NULL)
	{
		delete model;
	}
	model = NULL;
	int medicalRow = listView->currentIndex().row();
	QString name = templateModel->record(medicalRow).value("NAME").toString();
	if (sqlMap.contains(name))
	{
		QString pid = sqlMap[name];
		QString sql = "select * from MEDICALRECORDS_TEMPLATE_LIST where CLASSIFYCODE='" + pid + "'" + " AND(DEPARTMENT_CODE = '0000' OR DEPARTMENT_CODE = '" + DataBaseControl::m_docutorData->dept_code + "')";
		//QString sql = "select * from MEDICALRECORDS_TEMPLATE_LIST where CLASSIFYCODE='" + pid + "'";
		if (model != NULL)
		{
			delete model;
		}
		model = NULL;
		model = &m_DataBaseControl->getQueryData(sql);
		//reloadTableData();
		//setItemEdit(isEdit);
	}

	Clicked(listView->currentIndex());
}

void MuBanEditDlg::onLineEditClicked()
{
	lineEdit->selectAll();
}

bool MuBanEditDlg::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == lineEdit)
	{
		if (event->type() == QEvent::MouseButtonRelease)
		{
			lineEdit->selectAll();
		}
	}
	else
	if (obj == addBtn || obj == modifyBtn || obj == deleteBtn || obj == startBtn)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (event->type() == QEvent::KeyPress || event->type() == QEvent::InputMethod)
		{
			int test = keyEvent->key();
			if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
			{
				if (lineEdit->text() != "")
				{
					QString	sql = "select * from MEDICALRECORDS_TEMPLATE_LIST where NAME like '%" + lineEdit->text() + "%'" + " AND(DEPARTMENT_CODE = '0000' OR DEPARTMENT_CODE = '" + DataBaseControl::m_docutorData->dept_code + "')";
					if (model != NULL)
					{
						delete model;
					}
					model = NULL;
					model = &m_DataBaseControl->getQueryData(sql);
					reloadTableData();
				}
				else
				{
					QModelIndex index = listView->currentIndex();
					Clicked(index);
				}
				return true;
			}
		}
	}
	return QDialog::eventFilter(obj, event);
}

void MuBanEditDlg::onRadioBtnToggled()
{
	reloadTableData();
}

QRadioButton* MuBanEditDlg::getCheckedRadioButton()
{
	//allBtn;
	//publicBtn;
	//departmentBtn;
	if (allBtn != NULL&&allBtn->isChecked()) return allBtn;
	if (publicBtn != NULL&&publicBtn->isChecked()) return publicBtn;
	if (departmentBtn != NULL&&departmentBtn->isChecked()) return departmentBtn;
	return NULL;
}

void MuBanEditDlg::destory()
{
	for (int i = 0; i < tableWidget->rowCount(); i++)
	{
		QWidget *widget = tableWidget->cellWidget(i, 4);
		QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
		if (ckb)
		{
			delete ckb;
		}
		ckb = NULL;
	}
	if (strModel)
	{
		delete strModel;
		strModel = NULL;
	}
	if (templateModel)
	{
		delete templateModel;
		templateModel = NULL;
	}
	if (model)
	{
		delete model;
		model = NULL;
	}
}
