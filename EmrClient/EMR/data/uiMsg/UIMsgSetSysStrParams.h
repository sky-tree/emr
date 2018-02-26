#ifndef _UI_MSG_SET_SYS_STR_PARAMS_H_
#define _UI_MSG_SET_SYS_STR_PARAMS_H_


#include <QString>

#include "BaseUIMsgData.h"

// UI 系统str属性设置事件（需要事件类型，以及str参数）
class UIMsgSetSysStrParams : public BaseUIMsgData
{
public:
	UIMsgSetSysStrParams(WriterManager * writerMgr, char type, QString value);
	~UIMsgSetSysStrParams();

public:
	void doAction();

protected:
	char mType;
	QString mValue;
};

#endif	// _UI_MSG_SET_SYS_STR_PARAMS_H_