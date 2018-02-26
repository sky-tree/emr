#ifndef _PARAGRAPH_CODE_PARAMS_H_
#define _PARAGRAPH_CODE_PARAMS_H_


#include <QString>


#include "CodeParams.h"


#include "ConstantsID.h"

class BaseIOTools;

class ParagraphCodeParams : public CodeParams
{
public:
	ParagraphCodeParams(int classID = ConstantsID::ClassId::PARAGRAPH_CODE_DATA);
	ParagraphCodeParams(QString id, QString name, int authority, int level, int classID = ConstantsID::ClassId::PARAGRAPH_CODE_DATA);
	~ParagraphCodeParams();

public:
	void saveSelfData(BaseIOTools *ioTools);
	void loadSelfData(BaseIOTools *ioTools);

public:

	int getLevel();
	int getAuthority();
	int judgeAuthority();
	void setAuthority(int authority);

private:
	// 节点级别
	int mLevel;
	// 读写权限：0、不可写；1、正常操作(默认为1)
	int mWriteAuthority;
};

#endif	//_PARAGRAPH_CODE_PARAMS_H_
