#ifndef _UI_MSG_SET_PARAGRAPH_INDENT_PARAMS_H_
#define _UI_MSG_SET_PARAGRAPH_INDENT_PARAMS_H_


#include "BaseUIMsgData.h"

// UI 段落缩进属性设置事件（缩进参数:inL,inR,indentType,indentValue）
class UIMsgSetParagraphIndentParams : public BaseUIMsgData
{
public:
	UIMsgSetParagraphIndentParams(WriterManager * writerMgr, int insideL, int insideR, char indentType, int indentValue);
	~UIMsgSetParagraphIndentParams();

public:
	void doAction();

protected:
	int mInsideLeft;
	int mInsideRight;
	char mIndentType;
	int mIndentValue;
};

#endif	// _UI_MSG_SET_PARAGRAPH_INDENT_PARAMS_H_