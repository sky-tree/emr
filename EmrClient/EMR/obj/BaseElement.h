#ifndef BASE_ELEMENT_H_
#define BASE_ELEMENT_H_

#include <QString>

class BaseElement
{
public:
	BaseElement(int classId);
	BaseElement(BaseElement* otherElement);
	virtual~BaseElement();

public:
	const int mClassID;
	int getClassId();
};

#endif
