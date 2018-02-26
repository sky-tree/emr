#ifndef _BASETEMPLETEELEMENT_H_
#define _BASETEMPLETEELEMENT_H_
#include "BaseElement.h"
#include "BaseTemplate.h"
#include "qstring.h"
class BaseTempleteKeyboard :
	public BaseElement
{
public:
	BaseTempleteKeyboard(BaseTemplate* templeteOwner, int classid);
	virtual~BaseTempleteKeyboard();
	virtual int keyPress(QString str, BaseElement * obj)=0;
	virtual int keyRelease(QString str, BaseElement * obj)=0;
	void setTempleteOwner(BaseTemplate * templeteOwner);
	BaseTemplate* getTempleteOwner();
protected:
	BaseTemplate* mTempleteOwner;
};
#endif

