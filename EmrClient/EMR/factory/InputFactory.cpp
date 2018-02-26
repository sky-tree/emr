#include "InputFactory.h"



#include "ViewObj.h"
//#include "BaseWinKey.h"
//#include "ImgMouse.h"
//#include "BaseTxtLayerMouse.h"
//#include "BaseTxtWindowMouse.h"
//#include "TitleOrBottomTxtLayerMouse.h"

//#include "TxtWinScrollUpOrDownButtonMouse.h"
//#include "TxtWinScrollMiddleButtonMouse.h"
//#include "EditWindowToolbarButtonMouse.h"

//#include "EditImgPageMouse.h"
//#include "EditImgWindowMouse.h"
//#include "EditImgWindowTxtLayerMouse.h"
#include "InputFilter.h"
#include "ConstantsID.h"

#include "NumberInputFilter.h"
#include "TextWidgetMouse.h"
#include "TextWidgetKeyboard.h"
#include "BracketWidgetKeyboard.h"



InputFactory::InputFactory()
{
}

InputFactory::~InputFactory()
{
}

BaseButtonMouse * InputFactory::createButtonInput(ViewObj * viewOwner, DrawBaseImgButtonParam * drawParam)
{
	BaseButtonMouse * mouse = NULL;
	int objId = viewOwner->getObjID();
	switch (objId)
	{
		
	case ConstantsID::ContainerId::SCROLL_BUTTON_UP:
	case ConstantsID::ContainerId::SCROLL_BUTTON_DOWN:
		//mouse = new TxtWinScrollUpOrDownButtonMouse(viewOwner, drawParam);
		break;
	case ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE:
		//mouse = new TxtWinScrollMiddleButtonMouse(viewOwner, drawParam);
		break; 
	/*case ConstantsID::ContainerId::EDIT_IMG_WINDOW_TXT_BUTTON:
	case ConstantsID::ContainerId::EDIT_IMG_WINDOW_FREE_LINE_BUTTON:
	case ConstantsID::ContainerId::EDIT_IMG_WINDOW_NO_ARROW_LINE_BUTTON:
	case ConstantsID::ContainerId::EDIT_IMG_WINDOW_ARROW_LINE_BUTTON:
	case ConstantsID::ContainerId::EDIT_IMG_WINDOW_CIRCLE_BUTTON:
	case ConstantsID::ContainerId::EDIT_IMG_WINDOW_CLOSE_BUTTON:
		mouse = new EditWindowToolbarButtonMouse(viewOwner, drawParam);*/
		
		break;
	default:
		break;
	}
	
	//viewOwner->addMouse(mouse);
	return mouse;
}
ImgMouse * InputFactory::createImgMouse(ViewObj * viewOwner)
{
	//ImgMouse * imgMouse = new ImgMouse(viewOwner);
	//viewOwner->addMouse(imgMouse);
	//return imgMouse;
	return NULL;
}
BaseTxtLayerMouse * InputFactory::createPageContentTxtLayerMouse(ViewObj * viewOwner)
{
	/*BaseTxtLayerMouse * mouse = new BaseTxtLayerMouse(viewOwner);
	viewOwner->addMouse(mouse);
	return mouse;*/
	return NULL;
}
TitleOrBottomTxtLayerMouse * InputFactory::createPageTitleOrBottomTxtLayerMouse(ViewObj * viewOwner)
{
	/*TitleOrBottomTxtLayerMouse * mouse = new TitleOrBottomTxtLayerMouse(viewOwner);
	viewOwner->addMouse(mouse);
	return mouse;*/
	return NULL;
}
EditImgWindowTxtLayerMouse * InputFactory::createEidtImgWindowTxtLayerMouse(ViewObj * viewOwner)
{
	/*EditImgWindowTxtLayerMouse * mouse = new EditImgWindowTxtLayerMouse(viewOwner);
	viewOwner->addMouse(mouse);
	return mouse;*/
	return NULL;
}
EditImgPageMouse * InputFactory::createEditImgPageMouse(ViewObj * viewOwner)
{
	/*EditImgPageMouse * mouse = new EditImgPageMouse(viewOwner);
	viewOwner->addMouse(mouse);
	return mouse;*/
	return NULL;
}
EditImgWindowMouse * InputFactory::createEditImgWindowMouse(ViewObj * viewOwner)
{
	/*EditImgWindowMouse * mouse = new EditImgWindowMouse(viewOwner);
	viewOwner->addMouse(mouse);
	return mouse;*/
	return NULL;
}
BaseTxtWindowMouse * InputFactory::createTxtWindowMouse(ViewObj * viewOwner)
{
	/*BaseTxtWindowMouse * mouse = new BaseTxtWindowMouse(viewOwner);
	viewOwner->addMouse(mouse);
	return mouse;*/
	return NULL;
}
BaseWinKey * InputFactory::createBaseWinKey(ViewObj * viewParent)
{
	/*BaseWinKey * winKey = new BaseWinKey(viewParent);
	viewParent->initKeyboard(winKey);
	return winKey;*/
	return NULL;
}
InputFilter * InputFactory::createInputFilter(TextWidget * ownerTemplete)
{
	InputFilter * inputFilter = new InputFilter(ownerTemplete, ConstantsID::ClassId::INPUT_FILTER);
	return inputFilter;
}
NumberInputFilter * InputFactory::createNumberInputFilter(TextWidget * ownerTemplete){
	NumberInputFilter * inputFilter = new NumberInputFilter(ownerTemplete, ConstantsID::ClassId::NUMBER_INPUT_FILTER);
	return inputFilter;
}
TextWidgetMouse * InputFactory::createTextWidgetMouse(BaseTemplate* templeteOwner){
	TextWidgetMouse * textMouse = new TextWidgetMouse(templeteOwner, ConstantsID::ClassId::TEXT_WIDGET_MOUSE);
	return textMouse;
}
TextWidgetKeyboard * InputFactory::createTextWidgetKeyboard(BaseTemplate* templeteOwner)
{
	TextWidgetKeyboard * textMouse = new TextWidgetKeyboard(templeteOwner);
	return textMouse;
}
BracketWidgetKeyboard * InputFactory::createBrackWidgetKeyboard(BaseTemplate* templeteOwner)
{
	BracketWidgetKeyboard * textMouse = new BracketWidgetKeyboard(templeteOwner);
	return textMouse;
	return NULL;
}
