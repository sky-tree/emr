#ifndef _UI_MSG_SET_PARAGRAPH_FLOAT_PARAMS_H_
#define _UI_MSG_SET_PARAGRAPH_FLOAT_PARAMS_H_


#include "BaseUIMsgData.h"

// UI 段落缩进属性设置事件（缩进参数:inL,inR,indentType,indentValue）
class UIMsgSetParagraphFloatParams : public BaseUIMsgData
{
public:
	UIMsgSetParagraphFloatParams(WriterManager * writerMgr, char type, float value);
	~UIMsgSetParagraphFloatParams();

public:
	void doAction();

protected:
	char mType;
	float mDestValue;
};

#endif	// _UI_MSG_SET_PARAGRAPH_FLOAT_PARAMS_H_