#ifndef CONSTCOLLECTION_H
#define CONSTCOLLECTION_H

#include <QString>
#include <QStringList>
#include"Template/SingletonTemplate.h"
// 代码一定要是: UTF-8(BOM)      
//qt版本高于等于qt5，VS版本高于或等于VS2010    
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif 
class ConstCollection :public Singleton<ConstCollection>
{
public:
	ConstCollection();
	//static ConstCollection *getInstance();
	QString tempIP;
	QString LOGIN_URL;

	/*******************HTTP联网方式*********************/
	QString CONNECT_GET;
	QString CONNECT_POST;
	QString CONNETC_PUT;
	QString CONNECT_DELETE;
	QString IS_SUCESS;
	QString BIGREGION_ID;

	QString GET_AREA_URL;
	QString appPluginPath(QString pluginFileName);

protected:
	//static ConstCollection *pInstance;

};
#endif // CONSTCOLLECTION_H
