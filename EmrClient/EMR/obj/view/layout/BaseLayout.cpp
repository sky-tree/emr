#include "BaseLayout.h"




#include "WriterManager.h"
#include "SystemParams.h"
#include "OperateMgr.h"
#include "LayoutReplaceManager.h"
#include "CrossParentMgr.h"

#include "ViewObj.h"

#include "LayoutViewParam.h"
#include "RowLayout.h"
#include "StrContentData.h"

#include "BaseSystem.h"

#include "BaseConstants.h"
#include "ConstantsID.h"
#include "GlobalTemplate.h"

BaseLayout::BaseLayout(ViewObj * owner, int classID) : BaseChildrenManager(owner, classID)
{
	mViewOwner = owner;
	mNeedReplaceChildren = true;
	//mOwner->initChildrenMgr(this);
}
BaseLayout::BaseLayout(ViewObj * owner) : BaseChildrenManager(owner, ConstantsID::ClassId::BASE_LAYOUT)
{
	mViewOwner = owner;
	mNeedReplaceChildren = true;
	//mOwner->initChildrenMgr(this);
}
BaseLayout::~BaseLayout()
{
	mChildren = NULL;
}
ViewObj * BaseLayout::getViewOwner()
{
	return mViewOwner;
}
bool BaseLayout::isRelativeLayout()
{
	return mClassID == ConstantsID::ClassId::RELATIVE_LAYOUT;
}
BaseLayout * BaseLayout::getNextLayout(bool createWhenLast)
{
	ViewObj * parent = mViewOwner->getViewParent();
	QList<BaseContainer *> * children = parent->getChildren();
	int size = children->size();
	int idx = children->indexOf(mViewOwner);
	// 如果为最后一项
	if (idx == size - 1)
	{
		if (createWhenLast)
		{
			return createNextLayout();
		}
		else
			return NULL;
	}
	ViewObj * dest = (ViewObj *)children->at(idx + 1);
	return dest->getLayout();
}
BaseLayout * BaseLayout::createNextLayout()
{
	return NULL;
}
void BaseLayout::openReplace()
{
	if (!mNeedReplaceChildren)
	{
		mNeedReplaceChildren = true;
		//LayoutViewParam * param = (LayoutViewParam *)mViewOwner->getViewParam();
	}
}
void BaseLayout::closeReplace()
{
	mNeedReplaceChildren = false;
}
void BaseLayout::replaceChildren()
{
	mNeedReplaceChildren = false;
}
bool BaseLayout::canReplace()
{
	if (!mNeedReplaceChildren || haveChildren() < 1)
	{
		return false;
	}
	mNeedReplaceChildren = false;
	return true;
}
void BaseLayout::replaceByType(char replaceType)
{
	if (replaceType == BaseConstants::NONE)
	{
		return;
	}
	mNeedReplaceChildren = true;
	switch (replaceType)
	{
	case BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST:
		mViewOwner->getWriterMgr()->getLayoutReplaceMgr()->pushLayout(this);
		break;
	case BaseConstants::ReplaceType::REPLACE_NOW:
		replaceChildren();
		break;
	}
}
void BaseLayout::addChild(BaseContainer * child, int idx, bool refreshIdx, char replaceType)
{
	if (haveChildren() < 0)
	{
		createChildren();
	}
	int size = mChildren->size();
	int prePageNum = -1;
	// 如果为也面板删除页,需要处理页眉页脚的夸父对象
	if (mViewOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
	{
		judgeAddPage(child);
		prePageNum = mViewOwner->getChildren()->size();
	}
	BaseChildrenManager::addChild(child, idx, refreshIdx);
	if (prePageNum > 0)
		judgeRefreshPageNum(prePageNum);
	openReplace();
	replaceByType(replaceType);
}
void BaseLayout::addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx, char needReplace)
{
	if (haveChildren() < 0)
	{
		createChildren();
	}
	int size = mChildren->size();
	if (num < 0)
	{
		num = children->size() - start;
	}
	int prePageNum = -1;
	// 如果为也面板删除页,需要处理页眉页脚的夸父对象
	if (mViewOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
	{
		int end = start + num;
		for (int i = start; i < end; i++)
		{
			ViewObj * page = (ViewObj *)mChildren->at(i);
			judgeAddPage(page);
		}
		prePageNum = mViewOwner->getChildren()->size();
	}
	BaseChildrenManager::addChildren(children, start, num, idx, refreshIdx);
	if (prePageNum > 0)
		judgeRefreshPageNum(prePageNum);
	openReplace();
	replaceByType(needReplace);
}
int BaseLayout::delChild(BaseContainer * child, bool refreshIdx, char replaceType)
{
	int prePageNum = -1;
	// 如果为也面板删除页,需要处理页眉页脚的夸父对象
	if (mViewOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
	{
		judgeRemovePage(child);
		prePageNum = mViewOwner->getChildren()->size();
	}
	BaseChildrenManager::delChild(child, refreshIdx);
	if (prePageNum>0)
	judgeRefreshPageNum(prePageNum);
	openReplace();
	replaceByType(replaceType);
	return 1;
}
int BaseLayout::delChildren(int start, int num, bool refreshIdx, char replaceType)
{
	if (num < 0)
	{
		num = mChildren->size() - start;
	}
	int prePageNum = -1;
	// 如果为也面板删除页,需要处理页眉页脚的夸父对象
	if (mViewOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
	{
		int end = start + num;
		for (int i = start; i < end; i++)
		{
			ViewObj * page = (ViewObj *)mChildren->at(i);
			judgeRemovePage(page);
		}
		prePageNum = mViewOwner->getChildren()->size();
	}
	int returnType = BaseChildrenManager::delChildren(start, num, refreshIdx);
	if (prePageNum > 0)
		judgeRefreshPageNum(prePageNum);
	if (returnType != 0 && replaceType != BaseConstants::NONE)
	{
		openReplace();
		replaceByType(replaceType);
	}
	return returnType;
}
void BaseLayout::judgeRefreshPageNum(int preNum)
{
	int size = mChildren->size();
	bool needRefreshPageNum = false;
	bool isPlus = false;
	if (size > preNum)
	{
		isPlus = true;
		if (preNum <= 9)
		{
			if (size > 9)
			{
				needRefreshPageNum = true;
			}
		}
		else if (preNum <= 99)
		{
			if (size > 99)
			{
				needRefreshPageNum = true;
			}
		}
		else if (preNum <= 999)
		{
			if (size > 999)
			{
				needRefreshPageNum = true;
			}
		}
		else if (preNum <= 9999)
		{
			if (size > 9999)
			{
				needRefreshPageNum = true;
			}
		}
		else if (preNum <= 99999)
		{
			if (size > 99999)
			{
				needRefreshPageNum = true;
			}
		}
	}
	else if (size < preNum)
	{
		if (preNum > 9)
		{
			if (size <= 9)
			{
				needRefreshPageNum = true;
			}
		}
		else if (preNum > 99)
		{
			if (size <= 99)
			{
				needRefreshPageNum = true;
			}
		}
		else if (preNum > 999)
		{
			if (size <= 999)
			{
				needRefreshPageNum = true;
			}
		}
		else if (preNum > 9999)
		{
			if (size <= 9999)
			{
				needRefreshPageNum = true;
			}
		}
		else if (preNum > 99999)
		{
			if (size <= 99999)
			{
				needRefreshPageNum = true;
			}
		}
	}
	if (needRefreshPageNum)
	{
		SystemParams * sysParams = mViewOwner->getOperateMgr()->getSystemParams();
		ViewObj * pageNum = NULL;

		if (sysParams->getTitleNumViewType() != BaseConstants::NONE)
		{
			pageNum = sysParams->getTitlePageNumber();
			//if (!pageNum)
			//	pageNum = sysParams->createTitleNumber(mViewOwner->getWriterMgr());
			replaceNumber(isPlus, pageNum);
		}
		if (sysParams->getBottomNumViewType() != BaseConstants::NONE)
		{
			pageNum = sysParams->getBottomPageNumber();
			//if (!pageNum)
			//	pageNum = sysParams->createBottomNumber(mViewOwner->getWriterMgr());
			replaceNumber(isPlus, pageNum);
		}
	}
}
void BaseLayout::replaceNumber(bool isPlus, ViewObj * pageNum)
{
	StrContentData * strData = NULL;
	ViewObj * row = NULL;
	RowLayout * rowLayout = NULL;
	strData = (StrContentData *)pageNum->getContentData();
	strData->refreshTxtWH();
	row = pageNum->getViewParent();
	rowLayout = (RowLayout *)row->getLayout();
	rowLayout->openReplace();
	if (isPlus)
	{
		rowLayout->judgeOverFlowCircle();
	}
	else
	{
		rowLayout->judgeLessThanCircle(true);
	}
}
void BaseLayout::judgeRemovePage(BaseContainer * page)
{
	ViewObj * part = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_TITLE, false);
	CrossParentMgr * mgr = part->getCrossParentMgr();
	if (part->getViewParent() == page)
	{
		ViewObj * pagePanel = page->getViewParent();
		int pageIdx = ((ViewObj *)page)->getViewIndex();
		if (pageIdx > 0)
		{
			part->setParent(pagePanel->getChild(pageIdx - 1));
		}
		else
		{
			part->setParent(pagePanel->getChild(pageIdx + 1));
		}
	}
	mgr->removeParent((ViewObj *)page);
	part = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_BOTTOM, false);
	mgr = part->getCrossParentMgr();
	mgr->removeParent((ViewObj *)page);
}
void BaseLayout::judgeAddPage(BaseContainer * page)
{
	ViewObj * part = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_TITLE, false);
	CrossParentMgr * mgr = part->getCrossParentMgr();
	mgr->addParent((ViewObj *)page);
	part = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_BOTTOM, false);
	mgr = part->getCrossParentMgr();
	mgr->addParent((ViewObj *)page);
}