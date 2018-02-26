#ifndef _NUMBERINPUTFILTER_H_
#define _NUMBERINPUTFILTER_H_
#include "InputFilter.h"
class NumberInputFilter:public InputFilter
{
public:
	NumberInputFilter(TextWidget *ownerTemplete, int classID);
	virtual	~NumberInputFilter();
	void initFilter(int maxLeght = INFINITE, int decimalPointLenght = INFINITE);
	virtual QString filt(QString inputStr, int currentSize,  int insertIndex);
	int getDecimalPointLenght();
	int mDecimalPointLenght;
	QString mDecimal;
};
#endif

