#include "emrversion.h"
#include <QGridLayout>

#pragma execution_character_set("utf-8")

EMRVersion::EMRVersion(QWidget *parent)
	: QDialog(parent)
{
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	setWindowFlags(flags);
	setWindowIcon(QIcon(":/res/Res/MenuIcon/case.png"));
	ui.setupUi(this);
	setFixedSize(400, 137);
	initUI();
	initConnect();
	this->setWindowTitle(tr("关于"));
}

EMRVersion::~EMRVersion()
{

}

void EMRVersion::initUI()
{
	m_LbDateHead = new QLabel(this);
	m_LbDateHead->setText(tr("发布日期："));
	m_LbDate = new QLabel(this);
	QString DateTime_;
	DateTime_ = __DATE__;
	m_LbDate->setText(DateTime_);
	
	m_LbTimeHead = new QLabel(this);
	m_LbTimeHead->setText(tr("发布时间："));
	m_LbTime = new QLabel(this);
	DateTime_ = __TIME__;
	m_LbTime->setText(DateTime_);

	m_LbVersionHead = new QLabel(this);
	m_LbVersionHead->setText(tr("版      本："));
	m_LbVersion = new QLabel(this);
	QString Version_;
	Version_ = EMR_VERSION;
#ifdef QT_DEBUG
	Version_ += "Debug";
#else
	Version_ += "Rc";
#endif
	m_LbVersion->setText(Version_);

	m_BtnOk = new QPushButton(this);
	m_BtnOk->setMinimumSize(90, 31);
	m_BtnOk->setText(tr("确定"));

	QGridLayout * pGridLayout_ = new QGridLayout(this);
	for (int i = 0; i < 10; i++)
	{
		pGridLayout_->setColumnStretch(i, 1);
		if(i < 3)
			pGridLayout_->setRowStretch(i, 1);
	}
	pGridLayout_->addWidget(m_LbDateHead, 0, 2);
	pGridLayout_->addWidget(m_LbDate, 0, 3);
	pGridLayout_->addWidget(m_LbTimeHead, 1, 2);
	pGridLayout_->addWidget(m_LbTime, 1, 3);
	pGridLayout_->addWidget(m_LbVersionHead, 2, 2);
	pGridLayout_->addWidget(m_LbVersion, 2, 3);
	pGridLayout_->addWidget(m_BtnOk, 3, 7, 1, 3);
	setLayout(pGridLayout_);
	QString btnStyle = "QPushButton{border-width:1px;\
border-style:solid;\
border-radius: 5px;\
border-color:rgb(127,192,224);\
background-color:rgb(219,239,246);\
color:rgb(0,0,0); }\
QPushButton:hover{background-color:rgb(185,227,247); }\
QPushButton:pressed{ background-color:rgb(157, 218, 246); }";
	this->setStyleSheet(btnStyle);
}

void EMRVersion::initConnect()
{
	connect(m_BtnOk, SIGNAL(clicked(bool)), this, SLOT(hide()));
}