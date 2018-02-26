#include "tools/HisConstCollection.h"
#include <QSettings>
#include <QString>

QString HisConstCollection::appPluginPath(QString pluginFileName)
{
	QString dirPath = QApplication::applicationDirPath();
	dirPath = dirPath.left(dirPath.lastIndexOf("/" + QApplication::applicationName()));
	dirPath = dirPath + "/" + pluginFileName;
	return dirPath;
}

void HisConstCollection::writeIpConfigFile(QString ipAndPort)
{
	QSettings *configIniWrite = new QSettings(appPluginPath("ipConfig.ini"), QSettings::IniFormat);
	configIniWrite->setValue("/ip/ipAndPort", ipAndPort);
	configIniWrite->setValue("ip/login/", QObject::tr("/ShineHIS/userAccount/login?"));
}

void HisConstCollection::setIpConfigFile(QString ipAndPort)
{
	QSettings *configIniWrite = new QSettings(appPluginPath("ipConfig.ini"), QSettings::IniFormat);
	configIniWrite->setValue("/ip/ipAndPort", ipAndPort);
}

void HisConstCollection::readIpConfigFile()
{
	QSettings *configIniRead = new QSettings(appPluginPath("ipConfig.ini"), QSettings::IniFormat);
	ipAndPort = configIniRead->value("/ip/ipAndPort").toString();
	LOGIN_ID = configIniRead->value("ip/login").toString();
	login = ipAndPort + LOGIN_ID;
}

void HisConstCollection::writeDataIniFile()
{
	//INI文件由节、键、值组成
	//Qt中使用QSettings类读写ini文件
	//QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省
	QSettings *configIniWrite = new QSettings(appPluginPath("config.ini"), QSettings::IniFormat);
	//向ini文件中写入内容,setValue函数的两个参数是键值对
	//向ini文件的第一个节写入内容,ip节下的第一个参数
	configIniWrite->setValue("/ip/localhost1", QObject::tr("http://192.168.20.7")); 
	configIniWrite->setValue("/ip/localhost2", QObject::tr("http://119.4.167.250"));
	configIniWrite->setValue("/port/ipPort", QObject::tr(":8080"));
	configIniWrite->setValue("/ip/jsonMark", QObject::tr("?json="));

	configIniWrite->setValue("ip/LOGIN_URL/", QObject::tr("/ShineHIS/userAccount/login"));
	delete configIniWrite;
}

void HisConstCollection::readDataIniFile()
{
	QSettings *configIniRead = new QSettings(appPluginPath("config.ini"), QSettings::IniFormat);
	ipAndPort = configIniRead->value("/ip/localhost1").toString();
	QString ip2Str = configIniRead->value("/ip/localhost2").toString();
	ipPort = configIniRead->value("/port/ipPort").toString();
	jsonMark = configIniRead->value("/ip/jsonMark").toString();

	login = ipAndPort + ipPort + configIniRead->value("ip/LOGIN_URL/").toString() + jsonMark;
	delete configIniRead;
}

