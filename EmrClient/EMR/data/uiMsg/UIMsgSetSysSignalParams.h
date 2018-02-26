#ifndef _UI_MSG_SET_SYS_SIGNAL_PARAMS_H_
#define _UI_MSG_SET_SYS_SIGNAL_PARAMS_H_


#include "BaseUIMsgData.h"

// UI 系统信号事件（需要事件类型，int）
class UIMsgSetSysSignalParams : public BaseUIMsgData
{
public:
	UIMsgSetSysSignalParams(WriterManager * writerMgr, int type);
	~UIMsgSetSysSignalParams();

public:
	void doAction();

protected:
	int mType;
};

#endif	// _UI_MSG_SET_SYS_SIGNAL_PARAMS_H_