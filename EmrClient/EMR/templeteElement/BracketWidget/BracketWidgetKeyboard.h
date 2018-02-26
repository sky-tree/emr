#ifndef _BRACKETWIDGETKEYBOARD_H_
#define _BRACKETWIDGETKEYBOARD_H_
#include "BaseTempleteKeyboard.h"
class BracketWidget;
class BaseTemplate;
class BracketWidgetKeyboard :public BaseTempleteKeyboard
{
public:
	BracketWidgetKeyboard(BaseTemplate* templeteOwner);
	~BracketWidgetKeyboard();
	virtual int keyPress(QString str, BaseElement * obj);
	virtual int keyRelease(QString str, BaseElement * obj);
	void handleIfLeftSign();
private:
	BracketWidget * mTextWidget;
};
#endif
