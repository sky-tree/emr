#include "NodeMgr.h"



#include "ViewObj.h"

#include "ParagraphManager.h"

#include "ParagraphData.h"
#include "TempParagraphMgrData.h"
#include "NodeParams.h"
#include "NodeChildrenMgr.h"

#include "OperateMgr.h"
#include "SystemParams.h"

#include "BaseSystem.h"

#include "ConstantsID.h"
#include "GlobalTemplate.h"

NodeMgr::NodeMgr(BaseContainer * owner, ParagraphManager * paragraphMgr)
: TempContainerMgr(owner, paragraphMgr, ConstantsID::ClassId::NODE_MGR)
{
	mParagraphMgr = paragraphMgr;
	mNodeList = new QList<BaseContainer *>();
}
NodeMgr::NodeMgr(BaseContainer * owner, ParagraphManager * paragraphMgr, NodeMgr * otherNodeMgr)
: TempContainerMgr(owner, paragraphMgr, otherNodeMgr)
{
	mNodeList = new QList<BaseContainer *>();
	int size = otherNodeMgr->mNodeList->size();
	BaseContainer * node = NULL;
	for (int i = 0; i < size; i++)
	{
		node = otherNodeMgr->mNodeList->at(i);
		mNodeList->push_back(node);
	}
}
NodeMgr::~NodeMgr()
{
	mParagraphMgr = NULL;
}
bool NodeMgr::judgeCanOperate()
{
	return true;
}
void NodeMgr::clearNodeList(bool circle)
{
	int size = mNodeList->size();
	BaseContainer * node = NULL;
	NodeChildrenMgr * nodeChildrenMgr = NULL;
	for (int i = 0; i < size; i++)
	{
		node = mNodeList->at(i);
		nodeChildrenMgr = (NodeChildrenMgr *)node->getChildrenMgr();
		nodeChildrenMgr->clearNodeList(circle);
	}
	mNodeList->clear();
}
//void NodeMgr::addChild(BaseContainer * child, int idx, bool refreshIdx)
//{
//	addChild(child, idx, refreshIdx, NULL, -1);
//}
bool NodeMgr::addChild(BaseContainer * child, int idx, bool refreshIdx, QList<ParagraphData *> * useParagraphDataList, int useIndex)
{
	if (!judgeCanOperate())
	{
		return false;
	}
	if (mOwner->isObj(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER))
	{
		int a = 0;
	}


	// 如果不为节点，则正常添加
	if (!child->isNode())
	{
		return TempContainerMgr::addChild(child, idx, refreshIdx, useParagraphDataList, useIndex);
	}
	// 如果为节点
	else
	{
		NodeParams * nodeParams = (NodeParams *)child->getContentData()->getCodeParams();
		int level = nodeParams->getLevel();
		if (level != 1)
		{
			return false;
		}
		int size = mNodeList->size();
		int tempIdx = -1;
		BaseContainer * node = NULL;
		int firstNodeIdx = -1;
		if (idx < 0)
		{
			idx = mChildren->size();
		}
		if (size > 0)
		{
			for (int i = 0; i < size; i++)
			{
				node = mNodeList->at(i);
				tempIdx = node->getTempIndex();
				if (tempIdx > idx)
				{
					idx = tempIdx;
					mNodeList->insert(i, child);
					firstNodeIdx = i;
					break;
				}
				else if (tempIdx == idx)
				{
					idx = tempIdx + 1;
					mNodeList->insert(i + 1, child);
					firstNodeIdx = i + 1;
					break;
				}
			}
			if (firstNodeIdx == -1)
			{
				mNodeList->push_back(child);
			}
		}
		else
		{
			//idx = mChildren->size();
			mNodeList->push_back(child);
		}
		if (firstNodeIdx == -1)
		{
			idx = mChildren->size();
		}
		OperateMgr * operateMgr = mOwner->getOperateMgr();
		ViewObj * focus = NULL;
		BaseContainer * destObj = NULL;
		if (idx > 0)
		{
			destObj = mChildren->at(idx - 1);
			//if (!nextObj->isNode())
			{
				focus = destObj->getLastView();
			}
			//else
			{

			}
		}
		else
		{
			destObj = mChildren->at(0);
			focus = destObj->getFirstView();
		}
		operateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::LEFT);
		// 加到temp
		BaseChildrenManager::addChild(child, idx, refreshIdx);

		mParagraphMgr->addNode(child, refreshIdx, useParagraphDataList, useIndex, true);
	}
	return true;
}
void NodeMgr::addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx
	, QList<ParagraphData *> * useParagraphDataList, int useIndex)
{
	if (!judgeCanOperate() || num == 0)
	{
		return;
	}
	if (mOwner->isObj(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER))
	{
		int a = 0;
	}
	if (num < 0)
	{
		num = children->size() - start;
	}
	if (idx < 0)
	{
		idx = mChildren->size();
	}
	int end = start + num;
	int curFirstNodeIdx = -1;
	int tempIdx = -1;
	BaseContainer * child = NULL;
	int firstAddNodeIdx = -1;
	int size = children->size();
	for (int i = start; i < end; i++)
	{
		child = children->at(i);
		if (child->isNode())
		{
			firstAddNodeIdx = i;
			break;
		}
	}
	// 如果待添加列表中有节点，才需要重新计算插入位置
	if (firstAddNodeIdx >= 0)
	{
		size = mNodeList->size();
		for (int i = 0; i < size; i++)
		{
			child = mNodeList->at(i);
			tempIdx = child->getTempIndex();
			if (tempIdx >= idx)
			{
				idx = tempIdx;
				curFirstNodeIdx = i;
				break;
			}
		}
	}

	// 如果添加列表中没有节点,则正常添加
	if (firstAddNodeIdx < 0)
	{
		return TempContainerMgr::addChildren(children, start, num, idx, true, useParagraphDataList, useIndex);
	}
	// 如果添加列表中有节点
	else
	{
		if (curFirstNodeIdx == -1)
			idx = mChildren->size();

		int addNum = firstAddNodeIdx - start;
		// 先将节点前对象正常添加
		TempContainerMgr::addChildren(children, start, addNum, idx, true, useParagraphDataList, useIndex);
		idx += addNum;
		// 添加后续节点
		int nodeSize = mNodeList->size();
		int tempIdx = -1;
		end = start + num;
		if (nodeSize > 0)
		{
			child = children->at(start + addNum);
			for (int i = 0; i < nodeSize; i++)
			{
				child = mNodeList->at(i);
				tempIdx = child->getTempIndex();
				if (tempIdx > idx)
				{
					idx = tempIdx;
					//mNodeList->insert(i, child);
					firstAddNodeIdx = i;
					break;
				}
				else if (tempIdx == idx)
				{
					idx = tempIdx + 1;
					firstAddNodeIdx = i + 1;
					break;
				}
			}
			//if (firstAddNodeIdx == -1)
			//{
			//	mNodeList->push_back(child);
			//}
		}
		//else
		//	mNodeList->push_back(child);

		if (firstAddNodeIdx == -1)
		{
			idx = mChildren->size();
		}
		OperateMgr * operateMgr = mOwner->getOperateMgr();
		ViewObj * focus = NULL;
		BaseContainer * destObj = NULL;
		start = start + addNum;
		for (int i = start; i < end; i++)
		{
			child = children->at(i);
			if (idx > 0)
			{
				destObj = mChildren->at(idx - 1);
				focus = destObj->getLastView();
			}
			else
			{
				destObj = mChildren->at(0);
				focus = destObj->getFirstView();
			}
			operateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::LEFT);
			// 加到temp
			BaseChildrenManager::addChild(child, idx, refreshIdx);
			mParagraphMgr->addNode(child, refreshIdx, useParagraphDataList, useIndex, true);
			mNodeList->insert(curFirstNodeIdx + (i - start), child);
			idx++;
		}
	}
}

void NodeMgr::addAllChildrenToParagraphAndView()
{
	int size = mChildren->size();
	BaseContainer * obj = NULL;
	SystemParams * sys = mOwner->getOperateMgr()->getSystemParams();
	QList<BaseContainer *> * tempChildren = mChildren;
	//// 如果为页眉tempContainer
	//if (mOwner->isObj(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER))
	//{
	//	for (int i = 0; i < size; i++)
	//	{
	//		obj = mChildren->at(i);
	//	}
	//}
	//// 如果为页脚tempContainer
	//else if (mOwner->isObj(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER))
	//{
	//	for (int i = 0; i < size; i++)
	//	{
	//		obj = mChildren->at(i);
	//	}
	//}
	mParagraphMgr->addChildren(tempChildren, true, NULL, -1);
}
void NodeMgr::addRecordChildrenToParagraphAndView(ViewObj * lastView)
{
	mParagraphMgr->addRecordChildren(mChildren, lastView);
}
void NodeMgr::delRecordChildrenToParagraphAndView()
{
	mParagraphMgr->delRecordChildren(mChildren);
}

int NodeMgr::delChild(BaseContainer * child, bool refreshIdx)
{
	if (!judgeCanOperate())
	{
		return 0;
	}
	OperateMgr * mgr = mOwner->getOperateMgr();
	SystemParams * sys = mgr->getSystemParams();
	if (mOwner->isObj(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER))
	{
		sys->refreshTitleNumNext(child);
	}
	else if (mOwner->isObj(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER))
	{
		sys->refreshTitleNumNext(child);
	}

	if (!child->isNode())
	{
		return TempContainerMgr::delChild(child, refreshIdx);
	}
	// 如果删节点
	else
	{
		int size = mChildren->size();
		// 如果当前节点为容器唯一项,则不能删除
		if (size == 1)
		{
			return 0;
		}
		mNodeList->removeOne(child);
		return TempContainerMgr::delChild(child, refreshIdx);
	}
}
int NodeMgr::delChildren(int start, int num, bool refreshIdx)
{
	OperateMgr * mgr = mOwner->getOperateMgr();
	SystemParams * sys = mgr->getSystemParams();
	if (mOwner->isObj(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER))
	{
		sys->refreshTitleNumNext(mChildren, start, num);
	}
	else if (mOwner->isObj(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER))
	{
		sys->refreshBottomNumNext(mChildren, start, num);
	}

	int size = mChildren->size();
	if (num < 0)
	{
		num = size - start;
	}
	BaseContainer * child = NULL;
	int firstNodeIdx = -1;
	int end = start + num;
	for (int i = start; i < end; i++)
	{
		child = mChildren->at(i);
		if (child->isNode())
		{
			firstNodeIdx = i;
			break;
		}
	}
	// 如果为删除所有项，则不处理
	if (size == num && firstNodeIdx < 1)
	{
		return 0;
	}

	// 如果列表中没有节点，则正常删除
	if (firstNodeIdx == -1)
	{
		int lastDelIndex = start + num - 1;
		BaseContainer * lastDelObj = mChildren->at(lastDelIndex);
		// node前的普通回车不能删
		if (lastDelObj->isEnter() && lastDelIndex < mChildren->size() && mChildren->at(lastDelIndex + 1)->isNode())
		{
			return -1;
		}
		//for (int i = start; i < start + num; i++)
		//{
		//	child = mChildren->at(i);
		//	mNodeList->removeOne(child);
		//}
		//mNodeList->removeOne(child);
		return TempContainerMgr::delChildren(start, num, refreshIdx);
	}
	else
	{
		OperateMgr * operateMgr = mOwner->getOperateMgr();
		// 如果删除起始项不为节点，则不能删第一个节点前面的回车
		if (firstNodeIdx > 0)
		{
			ViewObj * destFocus = NULL;
			int normalObjNum = firstNodeIdx - start;
			int delNum = 0;
			if (normalObjNum > 1)
			{
				delNum = normalObjNum - 1;
				TempContainerMgr::delChildren(start, delNum, refreshIdx);
				ViewObj * leaveEnter = (ViewObj *)mChildren->at(start)->getLastView();
				operateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, leaveEnter, BaseConstants::Direction::LEFT);
			}
			else if (normalObjNum == 1)
			{
				if (start > 0)
				{
					if (normalObjNum < 1)
						destFocus = (ViewObj *)mChildren->at(start - 1)->getLastView();
					else
						destFocus = (ViewObj *)mChildren->at(start)->getLastView();
				}
				else
					destFocus = (ViewObj *)mChildren->at(normalObjNum - 1)->getFirstView();
					//destFocus = (ViewObj *)mChildren->at(start + num + 1)->getFirstView();
				operateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, destFocus, BaseConstants::Direction::LEFT);
			}
			else
			{
				if (start > 0)
					destFocus = (ViewObj *)mChildren->at(start - 1)->getLastView();
				else
					destFocus = (ViewObj *)mChildren->at(start + num)->getFirstView();
				operateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, destFocus, BaseConstants::Direction::LEFT);
			}
			firstNodeIdx -= delNum;
			delNum = num - normalObjNum;
			BaseContainer * node = NULL;
			for (int i = firstNodeIdx; i < firstNodeIdx + delNum; i++)
			{
				node = mChildren->at(i);
				mNodeList->removeOne(node);
			}
			//if (normalObjNum == 1 && start > 0)
			{
				TempContainerMgr::delChildren(firstNodeIdx, delNum, refreshIdx);
			}
			//else
			//	TempContainerMgr::delChildren(start, num, refreshIdx);
			return BaseConstants::ReturnType::HAVE_NOT_DEL_ENTER;
		}
		// 如果删除内容全是节点，则正常删
		else
		{
			return TempContainerMgr::delChildren(start, num, refreshIdx);
		}
	}
}