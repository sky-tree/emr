#ifndef _PATIENT_INFO_H_
#define _PATIENT_INFO_H_


#include <QString>

class BaseIOTools;

class PatientInfo
{
public:
	PatientInfo();
	PatientInfo(QString userID, QString userName);
	~PatientInfo();



	void doSave(BaseIOTools * ioTools);
	void doLoad(BaseIOTools * ioTools);

	void initInfo(QString userID, QString userName);

public:
	QString getUserID();
	QString getUserName();
	 
protected:

protected:
	QString mUserID;
	QString mUserName;
};

#endif
