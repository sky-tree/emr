#include "BaseChildrenManager.h"

#include <QDebug>



#include "WriterManager.h"

#include "ViewObj.h"


#include "ConstantsID.h"

#include "BaseConstants.h"
#include "GlobalTemplate.h"
#include "BaseSystem.h"

BaseChildrenManager::BaseChildrenManager(BaseContainer * owner) : BaseContainerParam(owner, ConstantsID::ClassId::BASE_CHILDREN_MANAGER)
, mChildren(NULL)
{
	mOwner->initChildrenMgr(this);
}
BaseChildrenManager::BaseChildrenManager(BaseContainer * owner, int classID) : BaseContainerParam(owner, classID)
, mChildren(NULL)
{
	mOwner->initChildrenMgr(this);
}
BaseChildrenManager::BaseChildrenManager(BaseContainer * owner, BaseChildrenManager* otherChildrenMgr)
: BaseContainerParam(owner, otherChildrenMgr, otherChildrenMgr->getClassId())
, mChildren(NULL)
{
	mOwner->initChildrenMgr(this);
	mChildren = new QList<BaseContainer *>();
	BaseSystem::getInstance()->CopyListToList(otherChildrenMgr->mChildren, owner->getWriterMgr(), mChildren);
}
BaseChildrenManager::~BaseChildrenManager()
{
	recycleChildren();
}
void BaseChildrenManager::recycleChildren()
{
	if (mChildren)
	{
		deleteVector(mChildren);
		mChildren = NULL;
	}
}
char BaseChildrenManager::haveChildren()
{
	if (!mChildren)
	{
		return -1;
	}
	if (mChildren->size() < 1)
	{
		return BaseConstants::NONE;
	}
	return 1;
}
void BaseChildrenManager::setChildWriteManger(WriterManager * writeManger)
{
	if (mChildren)
	{
		int size = mChildren->size();
		BaseContainer * tempObj = NULL;
		for (int i = 0; i < size;i++)
		{
			tempObj = mChildren->at(i);
			tempObj->setWriteManager(writeManger);
		}
	}
}
void BaseChildrenManager::createChildren()
{
	if (mChildren)
	{
		delete mChildren;
	}
	mChildren = new QList<BaseContainer *>();
	//mViewChildren = NULL;
}
BaseContainer * BaseChildrenManager::getChild(int idx)
{
	return mChildren->at(idx);
}
BaseContainer * BaseChildrenManager::getFirstChild()
{
	return mChildren->at(0);
}
BaseContainer * BaseChildrenManager::getLastChild()
{
	if (mChildren->size() < 1)
	{
		return NULL;
	}
	int size = mChildren->size();
	return mChildren->at(size - 1);
}
QList<BaseContainer *> * BaseChildrenManager::getChildren()
{
	return mChildren;
}
int BaseChildrenManager::getChildrenSize()
{
	if (mChildren)
	{
		return mChildren->size();
	}
	return 0;
}
//QList<ViewObj *> * BaseChildrenManager::getViewChildren()
//{
//	return mViewChildren;
//}
void BaseChildrenManager::pushChild(BaseContainer * child, bool refreshIdx)
{
	directAddChild(child, -1, refreshIdx);
}
bool BaseChildrenManager::addChild(BaseContainer * child, int idx, bool refreshIdx)
{
	directAddChild(child, idx, refreshIdx);
	return true;
}
void BaseChildrenManager::addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx)
{
	directAddChildren(children, start, num, idx, refreshIdx);
}
void BaseChildrenManager::directAddChild(BaseContainer * child, int idx, bool refreshIdx)
{
	if (!mChildren)
	{
		createChildren();
	}
	int size = mChildren->size();
	if (idx < 0)
	{
		idx = size;
	}
	if (mOwner->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER)
		&& mOwner->getParentViewById(ConstantsID::ContainerId::NORMAL_PAGE)->getViewIndex() == 1)
	{
		int a = 0;
	}
//#ifdef DEBUG_MODE
	if (mChildren->indexOf(child) >= 0)
	{
		qDebug() << "==================================================error: BaseChildrenManager::directAddChild()"
			<< " allready have obj!   ownerID:" << mOwner->getObjID()
			<< " childID: " << child->getObjID() << " =============================";
	}
//#endif
	//int preIdx = -1;
	//if (judgeRepeat)
	//{
	//	preIdx = swapChild(child, idx);
	//}
	if (idx == 0)
	{
		mChildren->push_front(child);
	}
	else if (idx == size)
	{
		mChildren->push_back(child);
	}
	else
	{
		mChildren->insert(idx, child);
	}
	child->haveAddToParent(mOwner);
	child->setParent(mOwner);

	if (refreshIdx)
		mOwner->refreshChildrenIndex(idx);
}
void BaseChildrenManager::directAddChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx)
{
	if (!qListHaveChildren(children))
	{
		return;
	}
	if (!mChildren)
	{
		createChildren();
	}
	int size = mChildren->size();
	if (idx < 0)
	{
		idx = size;
	}
	if (num == -1)
	{
		num = children->size() - start;
	}
	int end = start + num;

//#ifdef DEBUG_MODE
	QList<int> * tempList = BaseSystem::getInstance()->getTempIntList();
	swapChildren(children, start, num, idx, tempList);
	for (int i = start; i < start + num; i ++)
	{
		children->at(i)->haveAddToParent(mOwner);
	}
	//如果全部能添加
	if (tempList->size() >= 1)
		qDebug() << "==================================================error: BaseChildrenManager::directAddChildren()"
		<< " allready have obj!   ownerID:" << mOwner->getObjID() << " same children num: "
		<< tempList->size() << " =============================";
//#endif
	{
		int dis = 0;
		int preSize = children->size();
		int maxCount = end - start;
		int count = 0;
		BaseContainer * obj = NULL;
		if (idx == 0)
		{
			for (int i = start; i < end; i++)
			{
				preSize = children->size();
				obj = children->at(i);
				mChildren->insert(count, children->at(i));
				obj->setParent(mOwner);
				count++;
				dis = children->size() - preSize;
				if (dis != 0)
				{
					i += dis;
				}
				if (count >= maxCount)
				{
					break;
				}
			}
		}
		else if (idx == size)
		{
			for (int i = start; i < end; i++)
			{
				obj = children->at(i);
				preSize = children->size();
				mChildren->push_back(obj);
				obj->setParent(mOwner);
				count++;
				dis = children->size() - preSize;
				if (dis != 0)
				{
					i += dis;
				}
				if (count >= maxCount)
				{
					break;
				}
			}
		}
		else
		{
			for (int i = start; i < end; i++)
			{
				obj = children->at(i);
				preSize = children->size();
				mChildren->insert(idx + (i - start), obj);
				obj->setParent(mOwner);
				count++;
				dis = children->size() - preSize;
				if (dis != 0)
				{
					i += dis;
				}
				if (count >= maxCount)
				{
					break;
				}
			}
		}
	}

	if (refreshIdx)
		mOwner->refreshChildrenIndex(idx);
}

int BaseChildrenManager::delChild(int idx, bool refreshIdx)
{
	return directDelChild(idx, refreshIdx);
}
int BaseChildrenManager::delChild(BaseContainer * child, bool refreshIdx)
{
	return directDelChild(child, refreshIdx);
}
int BaseChildrenManager::delChildren(int start, int num, bool refreshIdx)
{
	return directDelChildren(start, num, refreshIdx);
}
int BaseChildrenManager::directDelChild(int idx, bool refreshIdx)
{
	BaseContainer * child = mChildren->at(idx);
	mChildren->removeAt(idx);

	child->haveDeleteFromParent(mOwner);
	child->clearParent(mOwner->getClassId());
	if (refreshIdx)
		mOwner->refreshChildrenIndex(idx);
	if (mChildren->size() < 1)
		return -1;
	return 1;
}
int BaseChildrenManager::directDelChild(BaseContainer * child, bool refreshIdx)
{
	mChildren->removeOne(child);

	child->haveDeleteFromParent(mOwner);
	child->clearParent(mOwner->getClassId());
	if (refreshIdx)
	{
		//int idx = mChildren->indexOf(child);
		mOwner->refreshChildrenIndex(0);
	}
	if (mChildren->size() < 1)
		return -1;
	return 1;
}
int BaseChildrenManager::directDelChildren(int start, int num, bool refreshIdx)
{
	if (!mChildren)
	{
		return -1;
	}
	int end = 0;
	if (num == -1)
		num = mChildren->size() - start;
	if (num < 1)
	{
		return 0;
	}
	BaseContainer * child = NULL;
	for (int i = 0; i < num; i ++)
	{
		if (num == 27)
		{
			int a = 0;
		}
		child = mChildren->at(start);
		child->haveDeleteFromParent(mOwner);
		mChildren->removeAt(start);
	}
	if (refreshIdx)
	{
		mOwner->refreshChildrenIndex(start);
	}
	if (mChildren->size() < 1)
		return 0;
	return 1;
}
void BaseChildrenManager::swapChild(BaseContainer * source, BaseContainer * dest)
{
	int sourceIdx = mChildren->indexOf(source);
	int destIdx = mChildren->indexOf(dest);
	mChildren->swap(sourceIdx, destIdx);
}
int BaseChildrenManager::swapChild(BaseContainer *child, int idx)
{
	int preIdx = mChildren->indexOf(child);
	if (preIdx >= 0 && idx < mChildren->size() && preIdx != idx)
	{
		mChildren->swap(preIdx, idx);
	}
	return preIdx;
}
bool BaseChildrenManager::swapChildren(QList<BaseContainer *> * children, int start, int num, int idx, QList<int> * tempList)
{
	tempList->clear();
	int destEnd = start + num;
	int preIdx = -1;
	int destIdx = -1;
	for (int i = start; i < destEnd; i++)
	{
		BaseContainer * obj = children->at(i);
		preIdx = mChildren->indexOf(obj);
		destIdx = idx + i - start;
		if (preIdx >= 0 && preIdx != destIdx)
		{
			mChildren->swap(preIdx, destIdx);
			tempList->push_back(i);
		}
	}
	if (tempList->size() > 0)
		return true;
	else
		return false;
}