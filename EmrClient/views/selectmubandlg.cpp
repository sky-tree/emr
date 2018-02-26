#include "selectmubandlg.h"
#include <QRadioButton>
#include <QDebug>
#include <QPushButton>
#include <QFile>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItem>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QApplication>
#include"globalvariable.h"
#include"qheaderview.h"
#include"dataModel\OperateData\DataBaseControl.h"
#include "qvalidator.h"
#include <QApplication>
#include <QDesktopWidget>


#define CREATE_RADIO_BTN(objName, text) \
objName = new QRadioButton(this); \
objName->setMinimumSize(90, 30); \
objName->setMaximumSize(90, 30); \
objName->setText(text); \
connect(objName, SIGNAL(clicked()), this, SLOT(onRadioBtnToggled()));
 
#define MAX_DLG_SIZE 1016, 681
#define SCREEN_RATE 0.25

SelectMuBanDlg::SelectMuBanDlg(QString pType, QWidget * parent) : QDialog(parent)
  ,allBtn(NULL), publicBtn(NULL), departmentBtn(NULL), personalBtn(NULL)
  , codeLabel(NULL), inputEdit(NULL), searchBtn(NULL), m_DataBaseControl(NULL), model(NULL), type(pType), m_DbManager(NULL)
{
    this->setModal(true);
	QSize size = QSize(QApplication::desktop()->width() * SCREEN_RATE, QApplication::desktop()->height() * SCREEN_RATE);
    this->resize(QSize(978, 549));
	this->setWindowTitle("病历模板");
	//this->setWindowIcon(QIcon(exeImagePath));
	//this->setAttribute(Qt::WA_DeleteOnClose, true);//设置关闭时自动析构
    //创建选择按钮布局及空间
	//绑定数据库数据

    QVBoxLayout * vLayout = new QVBoxLayout();
    QHBoxLayout * hLayout = new QHBoxLayout();
    this->setLayout(vLayout);

	CREATE_RADIO_BTN(allBtn, tr("全部模板"))
		CREATE_RADIO_BTN(publicBtn, tr("全院模板"))
		CREATE_RADIO_BTN(departmentBtn, tr("科室模板"))
		CREATE_RADIO_BTN(personalBtn, tr("个人模板"))

    codeLabel = new QLabel(tr("模板名称"), this);
    inputEdit = new QLineEdit(this);
    inputEdit->setMaximumSize(90, 30);
    inputEdit->setMinimumSize(90, 30);

	//设置输入的正则表达式
	QValidator *borderValidator = new QIntValidator(0, 999999, this);
	//inputEdit->setValidator(borderValidator);

    searchBtn = new QPushButton(tr("查询"), this);
    searchBtn->setMaximumSize(90, 30);
    searchBtn->setMinimumSize(90, 20);
	connect(searchBtn, SIGNAL(clicked(bool)), this, SLOT(onSearchBtnClicked()));

    hLayout->addStretch(1);
    hLayout->addWidget(allBtn);
    hLayout->addWidget(publicBtn);
    hLayout->addWidget(departmentBtn);
    hLayout->addWidget(personalBtn);
    hLayout->addWidget(codeLabel);
    hLayout->addWidget(inputEdit);
    hLayout->addWidget(searchBtn);
    hLayout->addStretch(1);

    table = new QTableView();
    connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));

    //QStandardItemModel *model=new QStandardItemModel();
	//QStringList headerList;
	//headerList << "分类代码" << "模板代码" << "模板名称" << "拼音码" << "科室名称" << "创建者";
	//model->setHorizontalHeaderLabels(headerList);
	//if (m_DbManager == NULL)
	//{
	//	m_DbManager = new DBManager();
	//	m_DbManager->connectOracle();
	//}
	if (m_DataBaseControl == NULL)
	{
		m_DataBaseControl = new DataBaseControl(NULL);
	}

	//设置默认显示全部模板按钮选中
	allBtn->setChecked(true);
	onRadioBtnToggled();

    vLayout->addLayout(hLayout);
    vLayout->addWidget(table);
}

void SelectMuBanDlg::closeEvent(QCloseEvent *e)
{
}

void SelectMuBanDlg::onRadioBtnToggled()
{
	QString condition = "where ISENABLE='Y' ";

	if (allBtn->isChecked())
	{
		condition += "and (TYPE = '1' or (TYPE = '2' and DEPARTMENT_CODE = '%1') or (TYPE = '3' and CREATE_ID= '%2'))";
		condition = condition.arg(DataBaseControl::m_docutorData->dept_code, DataBaseControl::m_docutorData->user_id);
	}
	else if (publicBtn->isChecked())
	{
		condition += "and TYPE ='1'";
	}
	else if (departmentBtn->isChecked())
	{
		condition += "and (TYPE = '2' and DEPARTMENT_CODE = '%1')";
		condition = condition.arg(DataBaseControl::m_docutorData->dept_code);
	}
	else if (personalBtn->isChecked())
	{
		condition += "and (TYPE = '3' and CREATE_ID= '%1')";
		condition = condition.arg(DataBaseControl::m_docutorData->user_id);
	}
	condition += " and CLASSIFYCODE='" + type + "'";
	QString sqlString ="select CLASSIFYCODE,CODE,NAME,PY_CODE,DEPARTMENT_NAME,CREATE_NAME,DATA from MEDICALRECORDS_TEMPLATE_LIST " + condition;
	model = &m_DataBaseControl->getQueryData(sqlString);
	
	updateTable();
}

void SelectMuBanDlg::doubleClicked(const QModelIndex &index)
{
    int curRow = index.row();
	//QString path = model->record(curRow).value(6).toString();
	selectedData.name = model->record(curRow).value(2).toString();
	selectedData.data = model->record(curRow).value(6).toString();
	emit QDialog::accept();
}

SelectMuBanDlg::~SelectMuBanDlg()
{
	if (m_DataBaseControl)
	{
		delete m_DataBaseControl;
		m_DataBaseControl = NULL;
	}
	if (m_DbManager)
	{
		delete m_DbManager;
		m_DbManager = NULL;
	}
}

void SelectMuBanDlg::updateTable()
{
	int rows = model->rowCount();
	model->setHeaderData(0, Qt::Horizontal, tr("分类代码"));
	model->setHeaderData(1, Qt::Horizontal, tr("模板代码"));
	model->setHeaderData(2, Qt::Horizontal, tr("模板名称"));
	model->setHeaderData(3, Qt::Horizontal, tr("拼音码"));
	model->setHeaderData(4, Qt::Horizontal, tr("科室名称"));
	model->setHeaderData(5, Qt::Horizontal, tr("创建者"));
	table->setModel(model);
	//设置显示	位置的列隐藏
	table->setColumnHidden(6, true);
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void SelectMuBanDlg::onSearchBtnClicked()
{
	if (inputEdit->text() != "")
	{
		;
		QString condition = "where NAME like '%%1%' and ISENABLE='Y' and (TYPE = '1' or (TYPE = '2' and DEPARTMENT_CODE = '%2') or (TYPE = '3' and CREATE_ID= '%3'))";
		condition += " and CLASSIFYCODE='" + type + "'";
		condition = condition.arg(inputEdit->text(), DataBaseControl::m_docutorData->dept_code, DataBaseControl::m_docutorData->user_id);

		QString sqlString = QString("select CLASSIFYCODE,CODE,NAME,PY_CODE,DEPARTMENT_NAME,CREATE_NAME,DATA from MEDICALRECORDS_TEMPLATE_LIST %1").arg(condition);
		model = &m_DataBaseControl->getQueryData(sqlString);
		updateTable();
	}
	else
	{
		onRadioBtnToggled();
	}
	
}

