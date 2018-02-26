#include "VerticalLayout.h"



#include "ViewObj.h"

#include "BaseViewParam.h"
#include "VerticalViewParam.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

#include "GlobalTemplate.h"

VerticalLayout::VerticalLayout(ViewObj * viewOwner) : BaseLayout(viewOwner, ConstantsID::ClassId::VERTICAL_LAYOUT)
{
	mViewParam = (VerticalViewParam *)viewOwner->getViewParam();
}
VerticalLayout::VerticalLayout(ViewObj * viewOwner, int classID) : BaseLayout(viewOwner, classID)
{
	mViewParam = (VerticalViewParam *)viewOwner->getViewParam();
}
VerticalLayout::~VerticalLayout()
{
}
void VerticalLayout::replaceChildren()
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
	mViewParam->refreshHByChildren(true);
}