#ifndef _UI_MSG_MEDICAL_RECORD_
#define  _UI_MSG_MEDICAL_RECORD_

#pragma once
#include "BaseUIMsgData.h"

class UIMsgMedicalRecord :
	public BaseUIMsgData
{
public:
	enum MedicalRecordType
	{
		LoadMedicalRecord,
		GotoMedicalRecord,
		//OpenMedicalRecordEditAuthority,
		OpenMedicalRecordEditAuthorityByFocus
	};
public:
	UIMsgMedicalRecord(WriterManager * writerMgr, QString id, QByteArray * source, MedicalRecordType type);
	UIMsgMedicalRecord(WriterManager * writerMgr, QString id, MedicalRecordType type);
	~UIMsgMedicalRecord();
public:
	void doAction();
	QString getID();
private:
	QString			mId;
	QByteArray  	mSource;
	MedicalRecordType mType;
};

#endif