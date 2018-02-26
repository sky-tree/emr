#ifndef BASE_CONTAINER_PARAM_H_
#define BASE_CONTAINER_PARAM_H_



#include "BaseElement.h"
#include "BaseContainer.h"


class BaseContainerParam : public BaseElement
{
public:
	BaseContainerParam(BaseContainer * owner, int classID);
	BaseContainerParam(BaseContainer * owner, BaseContainerParam* otherOne, int classID);
	virtual~BaseContainerParam();


public:
	BaseContainer * getOwner();
	void initOwner(BaseContainer * owner);

protected:
	BaseContainer * mOwner;
};

#endif
