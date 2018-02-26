#include "LayoutReplaceData.h"

#include "BaseLayout.h"
#include "ViewObj.h"

LayoutReplaceData::LayoutReplaceData(int layoutID)
{
	mCurReplaceIndex = 0;
	mLayoutID = layoutID;
	mLayoutList = new QList<BaseLayout *>();
}

LayoutReplaceData::~LayoutReplaceData()
{
	if (mLayoutList)
	{
		mLayoutList->clear();
		delete mLayoutList;
		mLayoutList = NULL;
	}
}
bool LayoutReplaceData::isDestLayout(int id)
{
	return mLayoutID == id;
}
void LayoutReplaceData::pushLayout(BaseLayout * layout)
{
	layout->openReplace();
	int size = mLayoutList->size();
	int idx = -1;
	for (int i = size - 1; i >= 0; i --)
	{
		if (mLayoutList->at(i) == layout)
		{
			idx = mLayoutList->indexOf(layout);
			break;
		}
	}
	//if (mLayoutID == 10000801)
	//{
	//	BaseContainer * page = layout->getViewOwner()->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	//	BaseContainer * pagePanel = page->getViewParent();
	//	if (pagePanel && !pagePanel->haveChild(page))
	//	{
	//		int a = 0;
	//	}
	//}
	// 如果列表中没有此layout，则直接添加到最后
	if (idx < 0)
	{
		mLayoutList->push_back(layout);
	}
	// 如果列表中有layout,同时在列表中索引小于当前正在刷新的索引，代表本轮循环已经刷新到他后面了，则需要将他重新加到后面
	else if (idx <= mCurReplaceIndex)
	{
		mLayoutList->push_back(layout);
	}
}
void LayoutReplaceData::clearLayout()
{
	mCurReplaceIndex = -1;
	mLayoutList->clear();
}
void LayoutReplaceData::replaceLayout()
{
	//int size = mLayoutList->size();
	//mCurReplaceIndex = 0;
	for (mCurReplaceIndex = 0; mCurReplaceIndex < mLayoutList->size(); mCurReplaceIndex++)
	{
		mLayoutList->at(mCurReplaceIndex)->replaceChildren();
	}
	clearLayout();
}