#include "ExcelCrossParentMgr.h"



#include "WriterManager.h"

#include "BaseKey.h"
#include "ViewObj.h"
#include "BaseViewParam.h"

#include "BaseLayout.h"
#include "PageContentTxtLayerLayout.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

ExcelCrossParentMgr::ExcelCrossParentMgr(ViewObj * view) : CrossParentMgr(view, ConstantsID::ClassId::EXCEL_CROSS_PARENT_MGR)
{
	mFirstPageDisY = 0;
	mSpendHList = new QList<int>();
}
ExcelCrossParentMgr::~ExcelCrossParentMgr()
{
}
void ExcelCrossParentMgr::setFirstParam(ViewObj * parent, int y, int spendH)
{
	int idx = mCrossParents->indexOf(parent);
	if (idx < 0)
	{
		if (mCrossParents->size() < 1)
		{
			mFirstPageDisY = y;
		}
		addParent(parent);
		//mDisYList->push_back(y);
		mSpendHList->push_back(spendH);
	}
	else
	{
		if (idx == 0)
		{
			mFirstPageDisY = y;
		}
		//mDisYList->removeAt(idx);
		//mDisYList->insert(idx, y);
		mSpendHList->removeAt(idx);
		mSpendHList->insert(idx, spendH);
	}
}
void ExcelCrossParentMgr::pushParam(ViewObj * parent, int spendH)
{
	int idx = mCrossParents->indexOf(parent);
	if (idx < 0)
	{
		addParent(parent);
		mSpendHList->push_back(spendH);
	}
	else
	{
		//mDisYList->removeAt(idx);
		//mDisYList->insert(idx, y);
		mSpendHList->removeAt(idx);
		mSpendHList->insert(idx, spendH);
	}
}
void ExcelCrossParentMgr::clearParents(bool needRemoveFromTxtLayer)
{
	if (needRemoveFromTxtLayer)
	{
		int size = mCrossParents->size();
		ViewObj * layer = NULL;
		PageContentTxtLayerLayout * layout = NULL;
		for (int i = 0; i < size; i++)
		{
			layer = mCrossParents->at(i);
			layout = (PageContentTxtLayerLayout *)layer->getLayout();
			layout->delChildWithOutClearCrossParents(mOwner, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		}
	}
	mCrossParents->clear();
	clearParams();
}
void ExcelCrossParentMgr::clearParentsWithoutSource(ViewObj * source)
{
	int size = mCrossParents->size();
	ViewObj * layer = NULL;
	PageContentTxtLayerLayout * layout = NULL;
	for (int i = 0; i < size; i++)
	{
		layer = mCrossParents->at(i);
		if (source == layer)
			continue;
		layout = (PageContentTxtLayerLayout *)layer->getLayout();
		layout->delChildWithOutClearCrossParents(mOwner, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
	}
	mOwner->setParent(source);
	mCrossParents->clear();
	clearParams();
}
void ExcelCrossParentMgr::clearParentsAfterSource(ViewObj * source)
{
	int idx = mCrossParents->indexOf(source);
	if (idx < 0)
	{
		return;
	}
	idx += 1;

	int size = mCrossParents->size();
	ViewObj * layer = NULL;
	PageContentTxtLayerLayout * layout = NULL;
	for (int i = idx; i < size; i++)
	{
		layer = mCrossParents->at(idx);
		layout = (PageContentTxtLayerLayout *)layer->getLayout();
		layout->delChildWithOutClearCrossParents(mOwner, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		mCrossParents->removeAt(idx);
	}
	clearParams(idx);
}
void ExcelCrossParentMgr::clearParams(int start)
{
	//mDisYList->clear();
	if (start > 0)
	{
		int size = mSpendHList->size();
		for (int i = start; i < size; i++)
		{
			mSpendHList->removeAt(start);
		}
	}
	else
		mSpendHList->clear();
}
int ExcelCrossParentMgr::getDisY(ViewObj * layer)
{
	int idx = mCrossParents->indexOf(layer);
	int size = mCrossParents->size();
	if (idx == 0)
	{
		return mFirstPageDisY;
	}
	return -getTotalPreSpendH(layer);
}
int ExcelCrossParentMgr::getSpendH(ViewObj * layer)
{
	int size = mCrossParents->size();
	int spendH = 0;
	int h = 0;
	for (int i = 0; i < size; i++)
	{
		h = mSpendHList->at(i);
		spendH += h;
		if (mCrossParents->at(i) == layer)
		{
			return h;
		}
	}
	return 0;
}
int ExcelCrossParentMgr::getTotalPreSpendH(ViewObj * layer)
{
	int idx = mCrossParents->indexOf(layer);
	int size = mCrossParents->size();
	if (idx == 0)
	{
		return 0;
	}
	else if (idx < 0)
	{
		idx = size;
	}
	int spendH = 0;
	for (int i = 0; i < idx; i++)
	{
		spendH += mSpendHList->at(i);
	}
	return spendH;
}