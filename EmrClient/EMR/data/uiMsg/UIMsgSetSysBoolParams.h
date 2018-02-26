#ifndef _UI_MSG_SET_SYS_BOOL_PARAMS_H_
#define _UI_MSG_SET_SYS_BOOL_PARAMS_H_


#include "BaseUIMsgData.h"

// UI 系统bool属性设置事件（需要事件类型，以及bool参数）
class UIMsgSetSysBoolParams : public BaseUIMsgData
{
public:
	UIMsgSetSysBoolParams(WriterManager * writerMgr, char type, bool value);
	~UIMsgSetSysBoolParams();

public:
	void doAction();

protected:
	char mType;
	bool mValue;
};

#endif	// _UI_MSG_SET_SYS_BOOL_PARAMS_H_