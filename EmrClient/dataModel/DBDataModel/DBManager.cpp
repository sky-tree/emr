#include "DBManager.h"

#include <QMessagebox>
#include<QDebug>
#include <QSqlError>
#include <qsqlquery.h>

#include <iostream>
#include <string.h>
#include <QFile>
#include <QCoreApplication>
#include <QLibrary>
#include <QFile>
#include "MessageBox.h"




bool DBManager::bIsConnect = false;
bool DBManager::bIsConnectSuccess = true;
QLibrary* DBManager::oracleLib = NULL;
DBManager::DBManager()
{

}


DBManager::~DBManager()
{
	disconnectOracle();
}

void DBManager::setDoctorData(const DoctorData & doctorData)
{
	this->docData = doctorData;
}
bool DBManager::connectDataBase()
{
	bool isConectToOracel = true;
	if (isConectToOracel)
	{
		return connectOracle();
	}
	else
	{
		return connectMYSQL();
	}

}
bool DBManager::connectOracle()
{
	if (bIsConnect==true)
	{
		return bIsConnectSuccess;
	}
	if (!checkOracleDLL()) return false;
	//if (!checkDocData()) return false;

	if (db.isOpen()) return true;

	QString exePath = QCoreApplication::applicationDirPath();


	QString oracleRelesePath;
	QString oracleProjectPath;

	oracleRelesePath = exePath + "/Oracle/oci.dll";
	oracleProjectPath = exePath + "/../../../Oracle/oci.dll";

// 	if (Is64Bit_OS())
// 	{
// 		oracleRelesePath = exePath + "/Oracle/oci.dll";
// 		oracleProjectPath = exePath + "/../../../Oracle/oci.dll";
// 	}
// 	else
// 	{
// 		oracleRelesePath = exePath + "/Oracle_x86/oci.dll";
// 		oracleProjectPath = exePath + "/../../../Oracle_x86/oci.dll";
// 	}


	QFile fileDetected;
	bool isReleasePathExist = fileDetected.exists(oracleRelesePath);
	bool isProjectPathExist = fileDetected.exists(oracleProjectPath);
	if (!(isReleasePathExist || isProjectPathExist))
	{
		QString errInfo = tr("缺少Oracle客户端库文件 ") + "\n" + "oci.dll";
		MessageBox::showErrorMessageBox(tr("提示"), errInfo);
		return false;
	}

	/*QLibrary * */oracleLib = new QLibrary((isReleasePathExist)? oracleRelesePath : oracleProjectPath);
	oracleLib->load();
	if (!oracleLib->isLoaded())
	{
		QString error_ = oracleLib->errorString();
		error_ += "\r\n Oracle客户端文件加载失败！";
		MessageBox::showErrorMessageBox("提示", error_);
		return false;
	}

	QFile file(exePath + "/config.txt");
	QString hostName;
	int port;
	QString dbName;
	QString userName;
	QString password;
	if (file.open((QFile::ReadOnly | QIODevice::Text)))
	{
		hostName = file.readLine();
		QString tem = file.readLine();
		port = tem.toInt();
		dbName = file.readLine();
		userName = file.readLine();
		password = file.readLine();
	}

	file.close();
	
	db = QSqlDatabase::addDatabase("QOCI");
	db.setHostName(hostName);
	db.setPort(port);
	db.setDatabaseName(dbName);
	db.setUserName(userName.trimmed());
	db.setPassword(password.trimmed());

	//db = QSqlDatabase::addDatabase("QOCI");
	////db.setHostName("192.168.20.6");
	//db.setHostName("119.4.167.250");
	//db.setPort(1539);
	//db.setDatabaseName("ORCL");
	//db.setUserName("TEST1");
	//db.setPassword("TEST1");

	//db = QSqlDatabase::addDatabase("QOCI");
	//db.setHostName("192.168.20.6");
	//db.setPort(1521);
	//db.setDatabaseName("ORCL");
	//db.setUserName("TEST1");
	//db.setPassword("TEST1");
	
	//db.setHostName(docData.dbInfo.hostName);
	//db.setPort(docData.dbInfo.port);
	//db.setDatabaseName(docData.dbInfo.dbName);
	//db.setUserName(docData.name);
	//db.setPassword(docData.password);
	bool state = db.open();
	if (state)
	{
		//QMessageBox::information(NULL, tr("提示"), tr("Oracle数据库连接成功！"), tr("确定"));
		//QSqlQuery query;
		//query.exec("insert into MEDICALRECORDS_TEMPLATE_LIST(ID,CLASSIFYCODE,TYPE,NAME,CODE,PY_CODE,DEPARTMENT_NAME,DEPARTMENT_CODE,CREATE_NAME,CREATE_ID,DATA,REMARKS,ISENABLE) values('01','01', '1', '血液检测', '01', 'XYJC', '血液科', '01', '老医生', '01', '数据', '备注', 'Y')");
	}
	else
	{
		QString errInfo = tr("Oracle数据库连接失败！") + "\n" + QString(db.lastError().text());
		MessageBox::showErrorMessageBox("提示", errInfo);
	}
	bIsConnect = true;
	return state;
}
bool DBManager::connectMYSQL()
{
	if (bIsConnect == true)
	{
		return bIsConnectSuccess;
	}
	if (!checkMYSQLDLL()) return false;
	QString exePath = QCoreApplication::applicationDirPath();

	QString mysqlRelesePath = exePath + "/MYSQL/libmysql.dll";
	QFile fileDetected;
	bool isReleasePathExist = fileDetected.exists(mysqlRelesePath);
	if (!isReleasePathExist)
	{
		QString errInfo = tr("缺少MYSQL客户端库文件 ") + "\n" + "libmysql.dll";
		MessageBox::showErrorMessageBox(tr("提示"), errInfo);
		return false;
	}

	/*QLibrary * */oracleLib = new QLibrary(mysqlRelesePath);
	oracleLib->load();
	if (!oracleLib->isLoaded())
	{
		MessageBox::showErrorMessageBox("提示", "MYSQL客户端文件加载失败！");
		return false;
	}

	QFile file(exePath + "/configofmysql.txt");
	QString hostName;
	int port;
	QString dbName;
	QString userName;
	QString password;
	if (file.open((QFile::ReadOnly | QIODevice::Text)))
	{
		hostName = file.readLine();
		QString tem = file.readLine();
		port = tem.toInt();
		dbName = file.readLine();
		userName = file.readLine();
		password = file.readLine();
	}
	else
	{
		MessageBox::showErrorMessageBox("提示", "MYSQL客户端配置文件configofmysql.txt加载失败！");
		return false;
	}

	file.close();

	db = QSqlDatabase::addDatabase("MYSQL");
	db.setHostName(hostName);
	db.setPort(port);
	db.setDatabaseName(dbName);
	db.setUserName(userName.trimmed());
	db.setPassword(password.trimmed());
	bool state = db.open();
	if (state)
	{
		//QMessageBox::information(NULL, tr("提示"), tr("Oracle数据库连接成功！"), tr("确定"));
		//QSqlQuery query;
		//query.exec("insert into MEDICALRECORDS_TEMPLATE_LIST(ID,CLASSIFYCODE,TYPE,NAME,CODE,PY_CODE,DEPARTMENT_NAME,DEPARTMENT_CODE,CREATE_NAME,CREATE_ID,DATA,REMARKS,ISENABLE) values('01','01', '1', '血液检测', '01', 'XYJC', '血液科', '01', '老医生', '01', '数据', '备注', 'Y')");
	}
	else
	{
		QString errInfo = tr("MYSQL数据库连接失败！") + "\n" + QString(db.lastError().text());
		MessageBox::showErrorMessageBox("提示", errInfo);
	}
	bIsConnect = true;
	return state;
}
bool DBManager::checkOracleDLL()
{
	bool isAvailable = false;
	QStringList drivers = QSqlDatabase::drivers();

	foreach(QString driver, drivers)
	{
		if (driver == "QOCI")
		{
			isAvailable = true;
			break;
		}
	}

	if (!isAvailable)
	{
#ifdef QT_NO_DEBUG  
		QMessageBox::critical(NULL, "critical", tr("缺少库文件qsqloci.dll"), QMessageBox::Yes, QMessageBox::Yes);
#else  
		QMessageBox::critical(NULL, "critical", tr("缺少库文件qsqlocid.dll"), QMessageBox::Yes, QMessageBox::Yes);
#endif  
	}
	bIsConnectSuccess = isAvailable;
	return isAvailable;
}
bool DBManager::checkMYSQLDLL()
{
	bool isAvailable = false;
	QStringList drivers = QSqlDatabase::drivers();

	foreach(QString driver, drivers)
	{
		if (driver == "QMYSQL")
		{
			isAvailable = true;
			break;
		}
	}

	if (!isAvailable)
	{
#ifdef QT_NO_DEBUG  
		QMessageBox::critical(NULL, "critical", tr("缺少库文件qsqlmysql.dll"), QMessageBox::Yes, QMessageBox::Yes);
#else  
		QMessageBox::critical(NULL, "critical", tr("缺少库文件qsqlmysqld.dll"), QMessageBox::Yes, QMessageBox::Yes);
#endif  
	}
	bIsConnectSuccess = isAvailable;
	return isAvailable;
}
void DBManager::disconnectOracle()
{
	db.close();
}

bool DBManager::checkDocData()
{
	//using namespace std;
	//QString errInfo;
	//char info[256];
	//memset(info, 0, sizeof(info));
	//if (docData.name == "")
	//{
	//	sprintf(info, "%s%s", "用户名" ,"不能为空");
	//}
	QString prefix;
	bool state = true;
	if (docData.name == "")
	{
		prefix = tr("用户名");
		state = false;
	}
	else if (docData.password == "")
	{
		prefix = tr("密码");
		state = false;
	}
	else if (docData.dbInfo.dbName == "")
	{
		prefix = tr("数据库名字");
		state = false;
	}
	else if (docData.dbInfo.hostName == "")
	{
		prefix = tr("主机地址");
		state = false;
	}

	if (!state)
	{
		QString errInfo = prefix + tr("不能为空");
		MessageBox::showErrorMessageBox("提示", errInfo);
	}
	
	return true;
}

bool DBManager::Is64Bit_OS()
{
	BOOL bIsWow64 = FALSE;
	IsWow64Process(GetCurrentProcess(), &bIsWow64);
	return bIsWow64;
}