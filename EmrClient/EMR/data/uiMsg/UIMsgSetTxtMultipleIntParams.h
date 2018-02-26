#ifndef _UI_MSG_SET_TXT_MULTIPLE_INT_PARAMS_H_
#define _UI_MSG_SET_TXT_MULTIPLE_INT_PARAMS_H_


#include <QList>

#include "BaseUIMsgData.h"


// UI 文字int[]属性设置事件（需要事件类型，以及QList<int> *参数）
class UIMsgSetTxtMultipleIntParams : public BaseUIMsgData
{
public:
	UIMsgSetTxtMultipleIntParams(WriterManager * writerMgr, char type, QList<int> * valueList);
	~UIMsgSetTxtMultipleIntParams();

public:
	void doAction();

protected:
	char mType;
	QList<int> * mValueList;
};

#endif	// _UI_MSG_SET_TXT_MULTIPLE_INT_PARAMS_H_