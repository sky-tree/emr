#ifndef BASEKEY_H_
#define BASEKEY_H_
#include "BaseElement.h"
class BaseKey : public BaseElement
{
public:
	BaseKey();
	~BaseKey();
	virtual	int keyPress(QString str){ return 0; };
	virtual	int keyRelease(QString str){ return 0; };
};
#endif