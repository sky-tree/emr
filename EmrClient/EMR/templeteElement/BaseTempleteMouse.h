#ifndef _BASEELEMENT_H_
#define _BASEELEMENT_H_
#include "BaseElement.h"
class BaseTemplate;
class BaseTempleteMouse :
	public BaseElement
{
public:
	BaseTempleteMouse(BaseTemplate* templeteOwner,int classid);
	virtual ~BaseTempleteMouse();
	virtual	int mousePress(int type, int x, int y, BaseElement * obj)=0;
	virtual	int mouseRelease(int type, int x, int y, BaseElement * obj)=0;
	virtual	int mouseMove(int type, int x, int y, BaseElement * obj)=0;
	virtual	int mouseDrag(int type, int x, int y, BaseElement * obj)=0;
	virtual	int mouseDoublePress(int type, int x, int y, BaseElement * obj)=0;
	virtual	int mouseScroll(int angle, BaseElement * obj)=0;
	void setTempleteOwner(BaseTemplate * templeteOwner);
	BaseTemplate* getTempleteOwner();
protected:
	BaseTemplate* mTempleteOwner;
};
#endif

