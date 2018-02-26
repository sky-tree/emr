#ifndef _UI_MSG_SET_TXT_BOOL_PARAMS_H_
#define _UI_MSG_SET_TXT_BOOL_PARAMS_H_


#include "BaseUIMsgData.h"

// UI 文字bool属性设置事件（需要事件类型，以及bool参数）
class UIMsgSetTxtBoolParams : public BaseUIMsgData
{
public:
	UIMsgSetTxtBoolParams(WriterManager * writerMgr, char type, bool value);
	~UIMsgSetTxtBoolParams();

public:
	void doAction();

protected:
	char mType;
	bool mValue;
};

#endif	// _UI_MSG_SET_TXT_BOOL_PARAMS_H_