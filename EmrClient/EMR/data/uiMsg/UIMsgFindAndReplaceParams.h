#ifndef _UI_MSG_FIND_AND_REPLACE_PARAMS_H_
#define _UI_MSG_FIND_AND_REPLACE_PARAMS_H_


#include "BaseUIMsgData.h"

// UI 系统bool属性设置事件（需要事件类型，以及bool参数）
class UIMsgFindAndReplaceParams : public BaseUIMsgData
{
public:
	UIMsgFindAndReplaceParams(WriterManager * writerMgr, char type, QString findStr);
	UIMsgFindAndReplaceParams(WriterManager * writerMgr, char type, QString findStr, QString replaceStr);
	~UIMsgFindAndReplaceParams();

public:
	void doAction();

protected:
	char mType;
	QString mFindStr;
	QString mReplaceStr;
};

#endif	// _UI_MSG_FIND_AND_REPLACE_PARAMS_H_