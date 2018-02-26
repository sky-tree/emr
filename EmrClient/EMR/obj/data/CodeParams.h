#ifndef _CODE_PARAMS_H_
#define _CODE_PARAMS_H_


#include <QString>


#include "BaseElement.h"


#include "ConstantsID.h"

class BaseIOTools;

class CodeParams : public BaseElement
{
public:
	CodeParams(int classID = ConstantsID::ClassId::BASE_CODE_DATA);
	CodeParams(QString id, QString name, int classID = ConstantsID::ClassId::BASE_CODE_DATA);
	~CodeParams();

public:
	void saveID(BaseIOTools *ioTools);
	virtual void saveSelfData(BaseIOTools *ioTools);
	virtual void loadSelfData(BaseIOTools *ioTools);

public:

	QString getID();
	QString getName();

	// �ж���дȨ��:0�������ܲ�����1����������
	virtual int judgeAuthority();

protected:

private:
	QString mID;
	QString mName;
};

#endif	//_CODE_PARAMS_H_
