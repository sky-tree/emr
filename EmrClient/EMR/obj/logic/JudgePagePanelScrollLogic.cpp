#include "JudgePagePanelScrollLogic.h"

#include "OperateMgr.h"

#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseViewParam.h"
#include "LayoutViewParam.h"


#include "ConstantsID.h"
#include "UIConstants.h"
#include "BaseConstants.h"

JudgePagePanelScrollLogic::JudgePagePanelScrollLogic(BaseContainer * viewOwner) : BaseLogic(viewOwner, ConstantsID::ClassId::JUDGE_PAGE_PANEL_SCROLL_LOGIC)
{
	mPagePanel = NULL;
	mPagePanelParam = NULL;
}

JudgePagePanelScrollLogic::~JudgePagePanelScrollLogic()
{
	mPagePanel = NULL;
	mPagePanelParam = NULL;
}
int JudgePagePanelScrollLogic::myUpdate()
{
	int returnType = BaseConstants::NONE;
	if (isBlock())
	{
		return returnType;
	}
	BaseLogic::addTime();

	if (!mPagePanel)
	{
		mPagePanel = mOwner->getOperateMgr()->getPagePanel();
		mPagePanelParam = mPagePanel->getViewParam();
	}
	int status = mPagePanel->getStatus();
	if (status == BaseConstants::ObjStatus::FREE)
	{
		return returnType;
	}
	int y = mPagePanelParam->getY();
	int h = mPagePanelParam->getTotalH();
	int bottom = y + h;
	ViewObj * window = (ViewObj *)mOwner;
	LayoutViewParam * winParam = (LayoutViewParam *)window->getViewParam();
	int winBottom = winParam->getInsideTop() + winParam->getInsideH();
	switch (status)
	{
	case BaseConstants::ObjStatus::PAGE_PANEL_MOVE_UP:
		if (bottom > winBottom)
		{
			mPagePanelParam->addY(-UIConstants::DEFAULT_PAGE_PANEL_DRAG_DIS_Y);
			if (mPagePanelParam->getY() + h < winBottom)
				mPagePanelParam->setY(winBottom - h);
		}
		break;
	case BaseConstants::ObjStatus::PAGE_PANEL_MOVE_DOWN:
		if (y < 0)
		{
			mPagePanelParam->addY(UIConstants::DEFAULT_PAGE_PANEL_DRAG_DIS_Y);
			if (mPagePanelParam->getY() > 0)
			{
				mPagePanelParam->setY(0);
			}
		}
		break;
	}
	return returnType;
}