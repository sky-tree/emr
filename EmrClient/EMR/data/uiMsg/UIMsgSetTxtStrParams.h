#ifndef _UI_MSG_SET_TXT_STR_PARAMS_H_
#define _UI_MSG_SET_TXT_STR_PARAMS_H_


#include <QString>

#include "BaseUIMsgData.h"

// UI 文字str属性设置事件（需要事件类型，以及QString参数）
class UIMsgSetTxtStrParams : public BaseUIMsgData
{
public:
	UIMsgSetTxtStrParams(WriterManager * writerMgr, char type, QString str);
	~UIMsgSetTxtStrParams();

public:
	void doAction();

protected:
	char mType;
	QString mString;
};

#endif	// _UI_MSG_SET_TXT_STR_PARAMS_H_