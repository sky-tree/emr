#ifndef HISCONSTCOLLECTION_H
#define HISCONSTCOLLECTION_H

#include <QString>

#include <QApplication>
 

#include "Template/SingletonTemplate.h"

// 代码一定要是: UTF-8(BOM)      
//qt版本高于等于qt5，VS版本高于或等于VS2010    
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif

/************************************************************************/
/* 新HIS系统系统常量集合                                                                     */
/************************************************************************/
class HisConstCollection : public Singleton<HisConstCollection>
{   
public:
	QString appPluginPath(QString pluginFileName);
	//IP地址
	QString ipAndPort;
	//端口号
	QString ipPort;
	QString jsonMark;
	//登陆命令
	QString login;
	//登陆唯一标识
	QString LOGIN_ID;
	/*******************HTTP联网方式*********************/
	QString CONNECT_GET;
	QString CONNECT_POST;
	QString CONNETC_PUT;
	QString CONNECT_DELETE;

	//读取ini系统配置文件
	void readDataIniFile();
	//创建ini系统配置文件
	void writeDataIniFile();
	/*读取ip配置文件*/
	void readIpConfigFile();
	/*写ip配置文件*/
	void writeIpConfigFile(QString ipAndPort);
	void setIpConfigFile(QString ipAndPort);
protected:
	static HisConstCollection *pInstance;
};

#endif // HISCONSTCOLLECTION_H
