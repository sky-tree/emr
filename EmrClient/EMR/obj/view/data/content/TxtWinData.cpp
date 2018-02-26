#include "TxtWinData.h"




#include "ViewObj.h"

#include "FlagParam.h"
#include "BaseViewParam.h"
#include "FlagViewParam.h"

#include "ViewFactory.h"
#include "ConstantsID.h"
#include "BaseConstants.h"

TxtWinData::TxtWinData(BaseContainer * owner) : BaseContentData(owner, ConstantsID::ClassId::TXT_WIN_DATA, NULL)
, mTitle(NULL)
, mBottom(NULL)
, mPagePanel(NULL)
, mPageList(NULL)
{
	init();
}

TxtWinData::~TxtWinData()
{
}
void TxtWinData::init()
{
	//mFlag->getViewParam()->setViewType(BaseConstants::ViewType::HIDE);
}
ViewObj * TxtWinData::getTitleView()
{
	if (!mTitle)
	{
		mTitle = (ViewObj *)getPagePanel()
			->findChildrenByID(ConstantsID::ContainerId::NORMAL_PAGE, false)
			->findChildrenByID(ConstantsID::ContainerId::PAGE_TITLE, false);
	}
	return mTitle;
}
ViewObj * TxtWinData::getBottomView()
{
	if (!mBottom)
	{
		mBottom = (ViewObj *)getPagePanel()->findChildrenByID(ConstantsID::ContainerId::NORMAL_PAGE, false)
			->findChildrenByID(ConstantsID::ContainerId::PAGE_BOTTOM, false);
	}
	return mBottom;
}
ViewObj * TxtWinData::getPagePanel()
{
	if (!mPagePanel)
	{
		mPagePanel = (ViewObj *)mOwner->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
	}
	return mPagePanel;
}
QList<BaseContainer *> * TxtWinData::getPageList()
{
	if (!mPageList)
	{
		if (!mPagePanel)
			getPagePanel();
		mPageList = mPagePanel->getChildren();
	}
	return mPageList;
}
int TxtWinData::getPageListNum()
{
	if (!mPageList)
		getPageList();
	return mPageList->size();
}