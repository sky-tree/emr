#include "UIMsgMedicalRecord.h"
#include "ConstantsID.h"
#include "OperateMgr.h"


UIMsgMedicalRecord::UIMsgMedicalRecord(WriterManager * writerMgr, QString id, QByteArray * source, MedicalRecordType type):
	BaseUIMsgData(writerMgr, ConstantsID::ClassId::SET_MEDICAL_RECORD),mId(id),mSource(*source), mType(type)
{
}

UIMsgMedicalRecord::UIMsgMedicalRecord(WriterManager * writerMgr, QString id, MedicalRecordType type):
BaseUIMsgData(writerMgr, ConstantsID::ClassId::SET_MEDICAL_RECORD), mId(id), mType(type)
{
}



UIMsgMedicalRecord::~UIMsgMedicalRecord()
{
}

QString UIMsgMedicalRecord::getID()
{
	return mId;
}
void UIMsgMedicalRecord::doAction()
{
	switch (mType)
	{
	case UIMsgMedicalRecord::LoadMedicalRecord:
		mOperateMgr->loadOtherMedicalRecord(mId, &mSource);
		break;
	case UIMsgMedicalRecord::GotoMedicalRecord:
		mOperateMgr->gotoMedicalRecord(mId);
		break;
	//case UIMsgMedicalRecord::OpenMedicalRecordEditAuthority:
	//	mOperateMgr->activeRecordFromRecordList(mId);
	//	break;
	case OpenMedicalRecordEditAuthorityByFocus:
		mOperateMgr->activeRecordFromRecordListByFocus(mId);
			break;
	default:
		break;
	}
}
