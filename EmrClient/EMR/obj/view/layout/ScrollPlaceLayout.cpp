#include "ConstantsID.h"
#include "ScrollPlaceLayout.h"
#include "ViewObj.h"
#include "ObjMouseManager.h"


ScrollPlaceLayout::ScrollPlaceLayout(ViewObj * owner):BaseLayout(owner, ConstantsID::ClassId::RELATIVE_LAYOUT)
{
}


ScrollPlaceLayout::~ScrollPlaceLayout()
{
}

void ScrollPlaceLayout::addChild(ViewObj * child, int idx, bool refreshIdx, char replaceType)
{
	BaseLayout::addChild(child, idx, refreshIdx, replaceType);
}

void ScrollPlaceLayout::replaceChildren()
{

	if (!canReplace())
	{
		return;
	}
	else
		BaseLayout::replaceChildren();
	return;
}

void ScrollPlaceLayout::refleshScollPanel(int y, int effectH, int totalH)
{
	ViewObj * viewObj = (ViewObj*)mViewOwner->findChildrenByID(ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE_BAR, true);
	ObjMouseManager * mouseMgr = viewObj->getMouseMgr();
	mouseMgr->setMiddleButtonParam(y, effectH, totalH);
}