#ifndef _AUTHORITY_VIEW_LINE_AND_STR_H_
#define _AUTHORITY_VIEW_LINE_AND_STR_H_



#include <QString>

#include "BaseElement.h"

class BaseIOTools;

class AuthorityViewLineAndStr : public BaseElement
{
public:
	AuthorityViewLineAndStr();
	AuthorityViewLineAndStr(int lineNumber, char linePosition, int lineColor, int strColor);
	~AuthorityViewLineAndStr();


public:
	//void doSave(BaseIOTools * ioTools);
	//void doLoad(BaseIOTools * ioTools);

public:
	int getLineNum();
	char getLinePosition();
	int getLineColor();
	int getStrColor();
	 
private:
	// 线的数量
	int mLineNumber;
	// 线的位置:BaseConstants::Direction
	char mLinePosition;
	// 线的颜色
	int mLineColor;
	// 文字颜色:0、代表不设置文字颜色
	int mStrColor;
};

#endif
