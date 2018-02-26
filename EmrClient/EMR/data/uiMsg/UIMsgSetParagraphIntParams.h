#ifndef _UI_MSG_SET_PARAGRAPH_INT_PARAMS_H_
#define _UI_MSG_SET_PARAGRAPH_INT_PARAMS_H_


#include "BaseUIMsgData.h"

// UI 段落缩进属性设置事件（缩进参数:inL,inR,indentType,indentValue）
class UIMsgSetParagraphIntParams : public BaseUIMsgData
{
public:
	UIMsgSetParagraphIntParams(WriterManager * writerMgr, char type, int value);
	~UIMsgSetParagraphIntParams();

public:
	void doAction();

protected:
	char mType;
	int mDestValue;
};

#endif	// _UI_MSG_SET_PARAGRAPH_INT_PARAMS_H_