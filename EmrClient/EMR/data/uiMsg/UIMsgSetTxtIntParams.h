#ifndef _UI_MSG_SET_TXT_INT_PARAMS_H_
#define _UI_MSG_SET_TXT_INT_PARAMS_H_


#include "BaseUIMsgData.h"

// UI 文字int属性设置事件（需要事件类型，以及int参数,包括char、short、int）
class UIMsgSetTxtIntParams : public BaseUIMsgData
{
public:
	UIMsgSetTxtIntParams(WriterManager * writerMgr, char type, int value);
	~UIMsgSetTxtIntParams();

public:
	void doAction();

protected:
	char mType;
	int mValue;
};

#endif	// _UI_MSG_SET_TXT_INT_PARAMS_H_