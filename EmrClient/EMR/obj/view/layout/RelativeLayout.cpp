#include "RelativeLayout.h"




#include "ViewObj.h"

#include "LayoutViewParam.h"


#include "RelativeChildrenParam.h"
#include "BaseSystem.h"


#include "ConstantsID.h"
#include "GlobalTemplate.h"

RelativeLayout::RelativeLayout(ViewObj * owner) : BaseLayout(owner, ConstantsID::ClassId::RELATIVE_LAYOUT)
, mRelativeParams(NULL)
{
	mRelativeParams = new QList<RelativeChildrenParam *>();
}

RelativeLayout::~RelativeLayout()
{
	if (mRelativeParams)
	{
		deleteVector(mRelativeParams);
		mRelativeParams = NULL;
	}
}
void RelativeLayout::replaceChildren()
{
	if (!canReplace())
	{
		return;
	}
	else
		BaseLayout::replaceChildren();
	int paramSize = mRelativeParams->size();
	if (mOwner->isObj(ConstantsID::ContainerId::NORMAL_PAGE))
	{
		int a = 0;
	}
	for (int i = 0; i < paramSize; i++)
	{
		RelativeChildrenParam * param = mRelativeParams->at(i);
		param->replace(true);
	}
	int size = mChildren->size();
	ViewObj * obj = NULL;
	BaseViewParam * objParam = NULL;
	LayoutViewParam * selfParam = (LayoutViewParam *)mViewOwner->getViewParam();
	float insideL = selfParam->getInsideLeft();
	float insideT = selfParam->getInsideTop();
	bool isPage = mViewOwner->isObj(ConstantsID::ContainerId::NORMAL_PAGE);

	for (int i = 0; i < size; i++)
	{
		obj = (ViewObj *)mChildren->at(i);
		if (obj->isObj(ConstantsID::ContainerId::PAGE_CONTENT))
		{
			int a = 0;
		}
		objParam = obj->getViewParam();
		if (objParam->getX() < insideL)
		{
			objParam->setX(insideL);
		}
		else if (isPage)
		{
			objParam->setX(insideL);
		}
		if (!obj->isObj(ConstantsID::ContainerId::PAGE_PANEL) && objParam->getY() < insideT)
		{
			objParam->setY(insideT);
		}
	}
}
void RelativeLayout::addChild(ViewObj * child, int idx, bool refreshIdx, char replaceType, RelativeChildrenParam * relativeParams)
{
	BaseLayout::addChild(child, idx, refreshIdx, replaceType);
	mRelativeParams->push_back(relativeParams);
}