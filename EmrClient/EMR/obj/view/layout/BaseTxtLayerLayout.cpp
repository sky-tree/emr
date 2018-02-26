#include "BaseTxtLayerLayout.h"



#include "ViewObj.h"

#include "BaseViewParam.h"
#include "VerticalViewParam.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

#include "GlobalTemplate.h"

BaseTxtLayerLayout::BaseTxtLayerLayout(ViewObj * viewOwner, int classID) : VerticalLayout(viewOwner, classID)
{
}
BaseTxtLayerLayout::~BaseTxtLayerLayout()
{
}
void BaseTxtLayerLayout::replaceChildren()
{
	if (!canReplace())
	{
		return;
	}
	else
		BaseLayout::replaceChildren();
	int size = mChildren->size();
	ViewObj * obj = NULL;
	BaseViewParam * param = NULL;
	float minY = mViewParam->getInsideTop();
	float disH = mViewParam->getChildrenDis();
	float curY = minY;
	float insideL = mViewParam->getInsideLeft();
	for (int i = 0; i < size; i ++)
	{
		obj = (ViewObj *)mChildren->at(i);
		param = obj->getViewParam();
		if (param->getX() < insideL)
		{
			param->setX(insideL);
		}
		param->setY(curY);
		curY += param->getTotalH() + disH;
	}
}