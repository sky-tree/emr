#ifndef _INPUTFILTER_H_
#define _INPUTFILTER_H_
#include "BaseElement.h"
#include "qstring.h"
class TextWidget;
class InputFilter :public BaseElement
{
public:
	InputFilter(TextWidget *ownerTemplete, int classID);
	virtual ~InputFilter();
	void setFilter(QString fliter, bool IsInclude);
	bool isInFilter(QString str);
	bool isInMaxSize(int size);
	virtual QString filt(QString inputStr,int currentSize,int insertIndex);
	int getMaxSize();
protected:
	TextWidget* mOwnerTemplete;
	QString mFilterStr;
	bool mIsIncludeFilter;
	int mStrMaxSize; //默认-1时，表示不限制数量
public:
	static const int INFINITE = -1; //不限制数量
};
#endif