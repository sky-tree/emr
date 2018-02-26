#include "LayoutReplaceManager.h"




#include "LayoutReplaceData.h"
#include "BaseLayout.h"
#include "ViewObj.h"


#include "BaseConstants.h"
#include "GlobalTemplate.h"
#include "ConstantsNumber.h"


#include "ConstantsID.h"
#include "BaseSystem.h"
#include "GlobalTemplate.h"

LayoutReplaceManager::LayoutReplaceManager()
{
	//mReplaceLayoutList = new QVector();
	mReplaceLayoutList = new QList<LayoutReplaceData *>();
	LayoutReplaceData * data = NULL;
	//int * num = BaseSystem::getInstance()->getLayoutReplaceMaxType();
	int * num = new int[ConstantsNumber::LAYOUT_REPLACE_MAX_TYPE_NUM]
	{
		ConstantsID::ContainerId::TXT_WIN_VIEW, ConstantsID::ContainerId::PAGE_PANEL, ConstantsID::ContainerId::SCROLL_PANEL
			, ConstantsID::ContainerId::NORMAL_PAGE
			, ConstantsID::ContainerId::PAGE_TITLE, ConstantsID::ContainerId::PAGE_BOTTOM, ConstantsID::ContainerId::PAGE_CONTENT
			, ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER
			, ConstantsID::ContainerId::CONTENT_TXT_LAYER
			, ConstantsID::ContainerId::EXCEL_TXT_LAYER, ConstantsID::ContainerId::NORMAL_ROW
	};
	for (int i = 0; i < ConstantsNumber::LAYOUT_REPLACE_MAX_TYPE_NUM; i++)
	{
		data = new LayoutReplaceData(num[i]);
		mReplaceLayoutList->push_back(data);
	}

	delete [] num;
}

LayoutReplaceManager::~LayoutReplaceManager()
{
	if (mReplaceLayoutList)
	{
		deleteVector(mReplaceLayoutList);
		mReplaceLayoutList = NULL;
	}
}
void LayoutReplaceManager::pushLayout(BaseLayout * layout)
{
	layout->openReplace();
	int size = mReplaceLayoutList->size();
	LayoutReplaceData * data = NULL;
	int id = layout->getViewOwner()->getObjID();
	for (int i = 0; i < size; i++)
	{
		data = mReplaceLayoutList->at(i);
		if (data->isDestLayout(id))
		{
			data->pushLayout(layout);
			break;
		}
	}
}
//void LayoutReplaceManager::clearLayout()
//{
//	int size = mReplaceLayoutList->size();
//	for (int i = 0; i < size; i ++)
//	{
//		mReplaceLayoutList->at(i)->clearLayout();
//	}
//	mReplaceLayoutList->clear();
//}
void LayoutReplaceManager::replaceLayout()
{
	int size = mReplaceLayoutList->size();
	LayoutReplaceData * data = NULL;
	for (int i = size - 1; i >= 0; i--)
	{
		data = mReplaceLayoutList->at(i);
		if (i == 1)
		{
			int a = 0;
		}
		data->replaceLayout();
		if (i==8)
		{
			int a = 0;
		}
	}
}