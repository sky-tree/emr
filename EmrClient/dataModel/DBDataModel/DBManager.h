#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "Template\SingletonTemplate.h"
#include "..\BaseDataType.h"

#include <QObject>
#include<QSqlDatabase>
#include <windows.h>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
class QLibrary;
class DBManager : public QObject , public Singleton<DBManager>
{
	Q_OBJECT
public:
	DBManager();
	~DBManager();

	/*设置医生基本数据*/
	void setDoctorData(const DoctorData & doctorData);
	bool connectDataBase();
	bool connectOracle();
	bool connectMYSQL();
	void disconnectOracle();

	/*检查是否存在Oracle库文件*/
	bool checkOracleDLL();
	bool checkMYSQLDLL();
	/*检查医生基本信息*/
	bool checkDocData();
	bool Is64Bit_OS();
		
private:
	DoctorData docData;
	QSqlDatabase db;
public:
	static bool bIsConnect;//判断是否已经连接过
	static bool bIsConnectSuccess;//判断是否连接成功
	static QLibrary * oracleLib;
};

#endif //define DBMANAGER_H
