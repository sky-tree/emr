#include "ConstantsID.h"
#include "ViewObj.h"
#include "ObjMouseManager.h"
#include "MiddleButtonViewParam.h"



MiddleButtonViewParam::MiddleButtonViewParam(ViewObj * owner):BaseViewParam(owner, ConstantsID::ClassId::FLAG_VIEW_PARAM)
{
}


MiddleButtonViewParam::~MiddleButtonViewParam()
{
}

bool MiddleButtonViewParam::setSelfH(int value, char childrenOrParentType, bool needAddToReplace)
{
	bool haveChanged = BaseViewParam::setSelfH(value, childrenOrParentType, needAddToReplace);
	if (mOwner->getObjID() == ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE)
	{
		ViewObj * windowViewObj = mViewOwner->findViewFromParent(ConstantsID::ContainerId::TXT_WIN_VIEW, true);
		if (windowViewObj == NULL)
		{
			return haveChanged;
		}
		BaseViewParam * windowViewParam = windowViewObj->getViewParam();
		int pageH = windowViewParam->getTotalH();

		ViewObj * pagePanelViewObj = (ViewObj*)windowViewObj->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, true);
		if (pagePanelViewObj == NULL)
		{
			return haveChanged;
		}
		BaseViewParam * pagePanelViewParam = pagePanelViewObj->getViewParam();
		int y = pagePanelViewParam->getY();
		int totalH = pagePanelViewParam->getTotalH();

		ViewObj * middleButtonBarViewobj = (ViewObj*)mViewOwner->findChildrenByID(ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE_BAR, false);
		if (middleButtonBarViewobj == NULL)
		{
			return haveChanged;
		}
		ObjMouseManager * muoseMgr = middleButtonBarViewobj->getMouseMgr();
		muoseMgr->setMiddleButtonParam(y , pageH, totalH);
	}
	return haveChanged;
}
