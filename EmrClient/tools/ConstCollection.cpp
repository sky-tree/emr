#include "tools/ConstCollection.h"
#include <QString>
#include <QApplication>
#include <QDebug>
 
//ConstCollection *ConstCollection::pInstance = NULL;
ConstCollection::ConstCollection()
{
	//ConstCollection::tempIP = "http://119.4.167.250:1124/OfficeAutomation";
	//ConstCollection::tempIP = "http://119.4.167.250:1122/DemoTest";
	//ConstCollection::tempIP = "http://localhost:8080/OfficeAutomation";
	ConstCollection::tempIP = "http://182.92.218.212:8088/ShineOA";
	ConstCollection::IS_SUCESS = "success";
	/*******************HTTP联网方式*********************/
	CONNECT_GET = "get";
	CONNECT_POST = "post";
	CONNETC_PUT = "put";
	CONNECT_DELETE = "deleteResource";
	//登录URL
	ConstCollection::LOGIN_URL = tempIP + "/user.do?method=isLogin";
	// ConstCollection::LOGIN_URL = tempIP + "/user.do?method=login";
}
QString ConstCollection::appPluginPath(QString pluginFileName)
{
	QString dirPath = QApplication::applicationDirPath();
	dirPath = dirPath.left(dirPath.lastIndexOf("/" + QApplication::applicationName()));
	dirPath = dirPath + "/" + pluginFileName;
	//QMessageBox::information(NULL, "Title", dirPath, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	return dirPath;
}




