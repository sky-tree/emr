#include "adminlogin.h"
#include <QGridLayout>
#include "MessageBox.h"

#pragma execution_character_set("utf-8")

AdminLogin::AdminLogin(QWidget *parent)
	: QDialog(parent)
{
	m_nAdminAuthority = 0;
	ui.setupUi(this);
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	setWindowFlags(flags);
	setWindowIcon(QIcon(":/res/Res/MenuIcon/case.png"));
	ui.setupUi(this);
	setFixedSize(400, 137);
	initUI();
	initConnect();
	this->setWindowTitle(tr("管理员登录"));
}

AdminLogin::~AdminLogin()
{

}


void AdminLogin::initUI()
{
	m_pLabUsername = new QLabel(this);
	m_pLabUsername->setText("用户名");

	m_pLabUserPwd = new QLabel(this);
	m_pLabUserPwd->setText("密码");

	m_pUserName = new QLineEdit(this);
	m_pUserPwd = new QLineEdit(this);
	m_pUserPwd->setEchoMode(QLineEdit::Password);

	m_pLogin = new QPushButton("登录", this);
	m_pLogin->setMinimumSize(90, 31);
	m_pCanle = new QPushButton("取消", this);
	m_pCanle->setMinimumSize(90, 31);

	QGridLayout * pGridLayout_ = new QGridLayout(this);
	for (int i = 0; i < 6; i++)
	{
		pGridLayout_->setColumnStretch(i, 1);
		if (i < 3)
			pGridLayout_->setRowStretch(i, 1);
	}
	pGridLayout_->addWidget(m_pLabUsername, 0, 1, 1, 2);
	pGridLayout_->addWidget(m_pUserName, 0, 3, 1, 2);
	pGridLayout_->addWidget(m_pLabUserPwd, 1, 1, 1, 2);
	pGridLayout_->addWidget(m_pUserPwd, 1, 3, 1, 2);
	pGridLayout_->addWidget(m_pLogin, 2, 1, 1, 2);
	pGridLayout_->addWidget(m_pCanle, 2, 3, 1, 2);

	setLayout(pGridLayout_);
	QString btnStyle = "QPushButton{border-width:1px;\
border-style:solid;\
border-radius: 5px;\
border-color:rgb(127,192,224);\
background-color:rgb(219,239,246);\
color:rgb(0,0,0); }\
QPushButton:hover{background-color:rgb(185,227,247); }\
QPushButton:pressed{ background-color:rgb(157, 218, 246); }";
	QString lableStyle = "QLabel{border-width:1px;\
border-style:solid;\
border-radius: 1px;\
border-color:rgb(127,192,224);\
background-color:rgb(219,239,246);color:rgb(0,0,0)};";

	this->setStyleSheet(btnStyle + lableStyle);
}

void AdminLogin::initConnect()
{
	connect(m_pLogin, SIGNAL(clicked(bool)), this, SLOT(adminLogin()));
	connect(m_pCanle, SIGNAL(clicked(bool)), this, SLOT(hide()));
}

void AdminLogin::adminLogin()
{
	QString userName_, userPwd_;
	userName_ = m_pUserName->text();
	userPwd_ = m_pUserPwd->text();
	if (userName_.trimmed() == "admin" && userPwd_.trimmed() == "128181")
	{
		MessageBox::showAboutMessageBox("提示", "登录成功，成功获取管理员权限!");
		m_nAdminAuthority = 1;
		hide();
	}
	else
	{
		MessageBox::showErrorMessageBox("提示", "用户名或密码错误，请重试!");
		m_nAdminAuthority = 0;
	}
}

int AdminLogin::exec()
{
	QDialog::exec();
	return m_nAdminAuthority;
}
