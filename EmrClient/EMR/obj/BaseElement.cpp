#include "BaseElement.h"


BaseElement::BaseElement(int classId) : mClassID(classId)
{
}
BaseElement::BaseElement(BaseElement* otherElement)
: mClassID(otherElement->mClassID)
{
}
BaseElement::~BaseElement()
{
}

int BaseElement::getClassId()
{
	return mClassID;
}
