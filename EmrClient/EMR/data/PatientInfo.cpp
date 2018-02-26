#include "PatientInfo.h"

#include "ViewObj.h"

#include "BaseIOTools.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

PatientInfo::PatientInfo()
{
}
PatientInfo::PatientInfo(QString userID, QString userName)
{
	mUserID = userID;
	mUserName = userName;
}


PatientInfo::~PatientInfo()
{
}

void PatientInfo::doSave(BaseIOTools * ioTools)
{
	ioTools->writeString(mUserID);
	ioTools->writeString(mUserName);
}
void PatientInfo::doLoad(BaseIOTools * ioTools)
{
	mUserID = ioTools->readString();
	mUserName = ioTools->readString();
}

void PatientInfo::initInfo(QString userID, QString userName)
{
	mUserID = userID;
	mUserName = userName;
}
QString PatientInfo::getUserID()
{
	return mUserID;
}
QString PatientInfo::getUserName()
{
	return mUserName;
}