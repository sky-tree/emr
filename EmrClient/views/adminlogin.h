#ifndef ADMINLOGIN_H
#define ADMINLOGIN_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "ui_adminlogin.h"

class AdminLogin : public QDialog
{
	Q_OBJECT

public:
	AdminLogin(QWidget *parent = 0);
	~AdminLogin();
	int exec();

private:
	Ui::AdminLogin ui;
private:
	void initUI();
	void initConnect();


private:
	QLabel *			m_pLabUsername;
	QLabel *			m_pLabUserPwd;

	QLineEdit *			m_pUserName;
	QLineEdit *			m_pUserPwd;

	QPushButton *		m_pLogin;
	QPushButton *		m_pCanle;

	int					m_nAdminAuthority;

	private slots:
	void adminLogin();
};

#endif // ADMINLOGIN_H
