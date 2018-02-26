#ifndef _TEXTWIDGETMOUSE_H_
#define _TEXTWIDGETMOUSE_H_
#include "BaseTempleteMouse.h"
class TextWidget;
class BaseTemplate;
class TextWidgetMouse :
	public BaseTempleteMouse
{
public:
	TextWidgetMouse(BaseTemplate* templeteOwner, int classid);
	~TextWidgetMouse();
	virtual	int mousePress(int type, int x, int y, BaseElement * obj);
	virtual	int mouseRelease(int type, int x, int y, BaseElement * obj);
	virtual	int mouseMove(int type, int x, int y, BaseElement * obj);
	virtual	int mouseDrag(int type, int x, int y, BaseElement * obj);
	virtual	int mouseDoublePress(int type, int x, int y, BaseElement * obj);
	virtual	int mouseScroll(int angle, BaseElement * obj);
	void fixTextHandleRightMouse();
	void timeTextHandeRightMouse();
	void numberFIlterHandleRightMouse();
	void inputTextHandleRightMouse();
	TextWidget * mTempleteOwner;
	bool mHadHandled;
};
#endif

