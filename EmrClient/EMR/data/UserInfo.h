#ifndef _USER_INFO_H_
#define _USER_INFO_H_



#include <QString>

class OperateMgr;
class AuthorityData;
class BaseIOTools;

class UserInfo
{
public:
	UserInfo(OperateMgr * mgr);
	UserInfo(OperateMgr * mgr, QString userID, QString userName, AuthorityData * data);
	~UserInfo();

	void doSave(BaseIOTools * ioTools);
	void doLoad(BaseIOTools * ioTools);
public:
	//void initInfo(QString userID, QString userName);

	QString getUserID();
	QString getUserName();


private:
	OperateMgr * mOperateMgr;
	QString mUserID;
	QString mUserName;
	AuthorityData * mAuthorityData;
};

#endif
