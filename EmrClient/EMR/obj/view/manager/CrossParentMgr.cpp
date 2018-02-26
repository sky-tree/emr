#include "CrossParentMgr.h"



#include "WriterManager.h"

#include "BaseKey.h"
#include "ViewObj.h"
#include "qdebug.h"
#include "ConstantsID.h"
#include "BaseConstants.h"

CrossParentMgr::CrossParentMgr(ViewObj * view) : BaseContainerParam(view, ConstantsID::ClassId::CROSS_PARENT_MGR)
{
	mViewOwner = view;
	init();
}
CrossParentMgr::CrossParentMgr(ViewObj * view, int classId) : BaseContainerParam(view, classId)
{
	mViewOwner = view;
	init();
}
CrossParentMgr::~CrossParentMgr()
{
	if (mActiveParent)
	{
		mActiveParent = NULL;
	}
	//if (mCrossParents)
	//{
	//	mCrossParents->clear();
	//	delete mCrossParents;
	//	mCrossParents = NULL;
	//}
}
void CrossParentMgr::init()
{
	((ViewObj *)mOwner)->initCrossParentMgr(this);
	mActiveParent = NULL;
	mCrossParents = new QList<ViewObj *>();
}
void CrossParentMgr::setActiveParent(ViewObj * parent)
{
	mActiveParent = parent;
	//if (!mActiveParent->getViewParent()->haveChild(mActiveParent))
	//{
	//	refindActiveParent();
	//}
}
ViewObj * CrossParentMgr::getActiveParent()
{
	return mActiveParent;
}
bool CrossParentMgr::haveCrossParent()
{
	if (!mCrossParents || mCrossParents->size() < 1)
	{
		return false;
	}
	return true;
}
//QList<ViewObj *> * CrossParentMgr::getCrosssParents()
//{
//	return mCrossParents;
//}

void CrossParentMgr::addParent(ViewObj * page)
{
	int pageIdx = page->getViewIndex();
	ViewObj * tempPage = NULL;
	bool haveAdd = false;
	if (pageIdx >= 0)
	{
		for (int i = 0; i < mCrossParents->size(); i++)
		{
			tempPage = mCrossParents->at(i);
			if (tempPage->getViewIndex() > pageIdx)
			{
				mCrossParents->insert(i, page);
				haveAdd = true;
				break;
			}
		}
	}
	if (!haveAdd)
		mCrossParents->push_back(page);

}
void CrossParentMgr::removeParent(ViewObj * page)
{
	//if (mActiveParent == page)
	//{
	//	refindActiveParent();
	//}
	mCrossParents->removeOne(page);
}
ViewObj * CrossParentMgr::getFirstCrossParent()
{
	return mCrossParents->at(0);
}
ViewObj * CrossParentMgr::getLastCrossParent()
{
	int size = mCrossParents->size();
	return mCrossParents->at(size - 1);
}
QList<ViewObj *> * CrossParentMgr::getCrossParents()
{
	return mCrossParents;
}