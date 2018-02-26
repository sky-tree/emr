#ifndef _UI_MSG_SET_SYS_INT_PARAMS_H_
#define _UI_MSG_SET_SYS_INT_PARAMS_H_


#include "BaseUIMsgData.h"

// UI 系统bool属性设置事件（需要事件类型，以及bool参数）
class UIMsgSetSysIntParams : public BaseUIMsgData
{
public:
	UIMsgSetSysIntParams(WriterManager * writerMgr, char type, int value);
	~UIMsgSetSysIntParams();

public:
	void doAction();

protected:
	char mType;
	int mValue;
};

#endif	// _UI_MSG_SET_SYS_INT_PARAMS_H_