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

	// 判断书写权限:0、代表不能操作；1、正常操作
	virtual int judgeAuthority();

protected:

private:
	QString mID;
	QString mName;
};

#endif	//_CODE_PARAMS_H_
