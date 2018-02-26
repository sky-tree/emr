#include "BaseTempleteKeyboard.h"


BaseTempleteKeyboard::BaseTempleteKeyboard(BaseTemplate* templeteOwner, int classid) :BaseElement(classid)
, mTempleteOwner(templeteOwner)
{
}
BaseTempleteKeyboard::~BaseTempleteKeyboard()
{
	if (mTempleteOwner){
		mTempleteOwner = NULL;
	}
}
void BaseTempleteKeyboard::setTempleteOwner(BaseTemplate * templeteOwner){
	mTempleteOwner = templeteOwner;
}
BaseTemplate* BaseTempleteKeyboard::getTempleteOwner(){
	return mTempleteOwner;
}