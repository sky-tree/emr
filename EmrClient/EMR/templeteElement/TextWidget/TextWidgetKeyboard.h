#ifndef _TEXTWIDGETKEYBOARD_H_
#define _TEXTWIDGETKEYBOARD_H_
#include "BaseTempleteKeyboard.h"
class TextWidget;
class ViewObj;
class TextWidgetKeyboard :public BaseTempleteKeyboard
{
public:
	TextWidgetKeyboard(BaseTemplate* templeteOwner);
	~TextWidgetKeyboard();
	virtual int keyPress(QString str, BaseElement * obj);
	virtual int keyRelease(QString str, BaseElement * obj);
	static bool isSystemKey(const QString & keyStr);
	static bool isDeleteOrBackspaceKey(const QString & keyStr);
	static int  mooveFocus(const QString& str,ViewObj * focusObj,char dir);
private:
	TextWidget * mTextWidget;
	
};
#endif
