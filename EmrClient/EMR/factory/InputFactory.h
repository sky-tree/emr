#ifndef _INPUT_FACTORY_H_
#define _INPUT_FACTORY_H_


class ViewObj;
class BaseWinKey;
class ImgMouse;
class BaseTxtLayerMouse;
class BaseTxtWindowMouse;
class TitleOrBottomTxtLayerMouse;
class BaseButtonMouse;

class DrawBaseImgButtonParam;

class EditImgPageMouse;
class EditImgWindowMouse;
class EditImgWindowTxtLayerMouse;
class InputFilter;
class NumberInputFilter;
class TextWidget;
class TextWidgetMouse;
class BaseTemplate;
class TextWidgetKeyboard;
class BracketWidgetKeyboard;

class InputFactory
{
public:
	InputFactory();
	~InputFactory();

public:

	static BaseButtonMouse * createButtonInput(ViewObj * viewOwner, DrawBaseImgButtonParam * drawParam);

	static ImgMouse * createImgMouse(ViewObj * viewOwner);
	static BaseTxtLayerMouse * createPageContentTxtLayerMouse(ViewObj * viewOwner);
	static TitleOrBottomTxtLayerMouse * createPageTitleOrBottomTxtLayerMouse(ViewObj * viewOwner);

	static EditImgWindowTxtLayerMouse * createEidtImgWindowTxtLayerMouse(ViewObj * viewOwner);

	static EditImgPageMouse * createEditImgPageMouse(ViewObj * viewOwner);
	static EditImgWindowMouse * createEditImgWindowMouse(ViewObj * viewOwner);

	static BaseTxtWindowMouse * createTxtWindowMouse(ViewObj * viewOwner);
	// 创建窗口按键
	static BaseWinKey * createBaseWinKey(ViewObj * viewParent);
	//过滤器
	static InputFilter * createInputFilter(TextWidget * ownerTemplete);
	//数字过滤器
	static NumberInputFilter * createNumberInputFilter(TextWidget * ownerTemplete);
	static TextWidgetMouse * createTextWidgetMouse(BaseTemplate* templeteOwner);
	static TextWidgetKeyboard * createTextWidgetKeyboard(BaseTemplate* templeteOwner);
	//前后带{}符号的输入文本
	static BracketWidgetKeyboard * createBrackWidgetKeyboard(BaseTemplate* templeteOwner);
};

#endif // _INPUT_FACTORY_H_
