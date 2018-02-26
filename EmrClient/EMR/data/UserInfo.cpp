#include "UserInfo.h"

#include "ViewObj.h"

#include "EMR\data\authority\AuthorityData.h"
#include "OperateMgr.h"
#include "SystemParams.h"

#include "BaseTools.h"
#include "BaseIOTools.h"

#include "ConstantsID.h"
#include "BaseConstants.h"
#include "StrConstants.h"

UserInfo::UserInfo(OperateMgr * mgr)
{
	mOperateMgr = mgr;
	mAuthorityData = NULL;
}
UserInfo::UserInfo(OperateMgr * mgr, QString userID, QString userName, AuthorityData * data)
{
	mOperateMgr = mgr;
	mUserID = userID;
	mUserName = userName;
	mAuthorityData = data;
}


UserInfo::~UserInfo()
{
	if (mAuthorityData)
	{
		delete mAuthorityData;
		mAuthorityData = NULL;
	}
	mOperateMgr = NULL;
}
void UserInfo::doSave(BaseIOTools * ioTools)
{
	ioTools->writeString(mUserID);
	ioTools->writeString(mUserName);

	if (mAuthorityData)
	{
		ioTools->writeByte(1);
		mAuthorityData->doSave(ioTools);
	}
	else
	{
		ioTools->writeByte(0);
	}
}
void UserInfo::doLoad(BaseIOTools * ioTools)
{
	mUserID = ioTools->readString();
	mUserName = ioTools->readString();

	int curVersionInt = mOperateMgr->getSystemParams()->getVersionInt();
	if (BaseTools::isOverOrEqualsVersion(curVersionInt, StrConstants::VERSION_0_0_0_5))
	{
		char haveData = ioTools->readByte();
		if (haveData == 0)
		{
			mAuthorityData = NULL;
		}
		else
		{
			mAuthorityData = new AuthorityData();
			mAuthorityData->doLoad(ioTools);
		}
	}
	else
	{
		mAuthorityData = NULL;
	}
}
//void UserInfo::initInfo(QString userID, QString userName)
//{
//	mUserID = userID;
//	mUserName = userName;
//}

QString UserInfo::getUserID()
{
	return mUserID;
}
QString UserInfo::getUserName()
{
	return mUserName;
}