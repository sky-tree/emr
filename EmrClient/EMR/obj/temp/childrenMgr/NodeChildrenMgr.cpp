#include "NodeChildrenMgr.h"



#include "ViewObj.h"
#include "BaseTemplate.h"

#include "ParagraphManager.h"


#include "ParagraphData.h"
#include "TempParagraphMgrData.h"
#include "NodeParams.h"

#include "FlagParam.h"
#include "OperateMgr.h"
#include "SystemParams.h"

#include "BaseSystem.h"

#include "ConstantsID.h"
#include "GlobalTemplate.h"

NodeChildrenMgr::NodeChildrenMgr(BaseContainer * owner, ParagraphManager * paragraphMgr, int classID)
: TempContainerMgr(owner, paragraphMgr, classID)
{
	mParagraphMgr = paragraphMgr;
	mNodeList = new QList<BaseContainer *>();
}
NodeChildrenMgr::NodeChildrenMgr(BaseContainer * owner, ParagraphManager * paragraphMgr, NodeChildrenMgr * otherNodeChildrenMgr)
: TempContainerMgr(owner, paragraphMgr, otherNodeChildrenMgr)
{
	mParagraphMgr = paragraphMgr;
	mNodeList = new QList<BaseContainer *>();
	int size = otherNodeChildrenMgr->mNodeList->size();
	BaseContainer * node = NULL;
	for (int i = 0; i < size; i++)
	{
		node = otherNodeChildrenMgr->mNodeList->at(i);
		mNodeList->push_back(node);
	}
}
NodeChildrenMgr::~NodeChildrenMgr()
{
	mParagraphMgr = NULL;
}
bool NodeChildrenMgr::judgeCanOperate()
{
	return true;
}
void NodeChildrenMgr::clearNodeList(bool circle)
{
	int size = mNodeList->size();
	BaseContainer * node = NULL;
	NodeChildrenMgr * nodeMgr = NULL;
	if (circle)
	{
		for (int i = 0; i < size; i++)
		{
			node = mNodeList->at(i);
			nodeMgr = (NodeChildrenMgr *)node->getChildrenMgr();
			nodeMgr->clearNodeList(circle);
		}
	}
	mNodeList->clear();
}
bool NodeChildrenMgr::addChild(BaseContainer * child, int idx, bool refreshIdx, QList<ParagraphData *> * useParagraphDataList, int useIndex)
{
	if (!judgeCanOperate())
	{
		return false;
	}
	// �����Ϊ�ڵ㣬���������
	if (!child->isNode())
	{
		return TempContainerMgr::addChild(child, idx, refreshIdx, useParagraphDataList, useIndex);
	}
	else
	{
		if (idx < 0)
		{
			idx = mChildren->size();
		}
		BaseContainer * tempParent = mOwner->getTempParent();
		FlagParam * pressEnd = mOwner->getOperateMgr()->getPressEnd();
		ViewObj *  focus = pressEnd->getFocusObj();
		BaseContainer * parentNode = focus->findTempFromParent(ConstantsID::ContainerId::NODE_CONTAINER, true);
		int parentLevel = 0;
		if (parentNode)
		{
			NodeParams * parentNodeParams = (NodeParams *)parentNode->getContentData()->getCodeParams();
			parentLevel = parentNodeParams->getLevel();
		}
		NodeParams * nodeParams = (NodeParams *)child->getContentData()->getCodeParams();
		int level = nodeParams->getLevel();
		if (level >= 1)
		{
			int disLevel = level - parentLevel;
			// ���뼶������1����������1���ڵ������3���ڵ㣬������
			if (disLevel > 1)
				return false;
			// ��ͬ�����߸��߼�������2�����ϲ�1��������1�����ϲ�1������嵽��ǰ�߼��ڵ�+1λ��,����������nodeList
			else if (disLevel < 1)
			{
				TempContainerMgr * parentMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
				idx = mOwner->getTempIndex();
				for (int i = 0; i < -disLevel; i++)
				{
					tempParent = tempParent->getTempParent();
					idx = tempParent->getTempIndex();
					parentMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
				}
				//idx++;
				parentMgr->addChild(child, idx, refreshIdx, useParagraphDataList, useIndex);
				return true;
			}
			// ���1�������������Ӷ��󣬲�����nodeList
			else
			{
				int size = mNodeList->size();
				int tempIdx = -1;
				BaseContainer * node = NULL;

				if (size > 0)
				{
					bool haveInsertNode = false;
					for (int i = 0; i < size; i++)
					{
						node = mNodeList->at(i);
						tempIdx = node->getTempIndex();
						if (tempIdx >= idx)
						{
							idx = tempIdx;
							haveInsertNode = true;
							mNodeList->insert(i, child);
							break;
						}
					}
					if (!haveInsertNode)
					{
						mNodeList->push_back(child);
					}
				}
				else
				{
					//idx = mChildren->size();
					mNodeList->push_back(child);
				}
				OperateMgr * operateMgr = mOwner->getOperateMgr();
				ViewObj * focus = NULL;
				BaseContainer * nextObj = NULL;
				if (idx < mChildren->size() - 1)
				{
					nextObj = mChildren->at(idx + 1);
					focus = nextObj->getFirstView();
				}
				else
				{
					nextObj = mChildren->at(idx - 1);
					focus = nextObj->getLastView();
				}
				operateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::LEFT);
				// �ӵ�temp
				BaseChildrenManager::addChild(child, idx, refreshIdx);

				mParagraphMgr->addNode(child, refreshIdx, useParagraphDataList, useIndex, true);
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}
//void NodeChildrenMgr::addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx)
//{
//	addChildren(children, start, num, idx, refreshIdx, NULL, -1);
//}
void NodeChildrenMgr::addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx
	, QList<ParagraphData *> * useParagraphDataList, int useIndex)
{
	if (!judgeCanOperate())
	{
		return;
	}
	if (num < 0)
	{
		num = children->size() - start;
	}
	BaseContainer * child = NULL;
	int end = start + num;
	int firstAddNodeIdx = -1;
	for (int i = 0; i < end; i ++)
	{
		child = children->at(i);
		if (child->isNode())
		{
			firstAddNodeIdx = i;
			break;
		}
	}
	// �������б���û�нڵ㣬���������
	if (firstAddNodeIdx < 0)
	{
		return TempContainerMgr::addChildren(children, start, num, idx, refreshIdx, useParagraphDataList, useIndex);
	}
	else
	{
		OperateMgr * operateMgr = mOwner->getOperateMgr();
		ViewObj * focus = NULL;
		int addNum = 0;
		// �������ͨ����
		if (firstAddNodeIdx > 0)
		{
			idx = mChildren->size() - 1;
			addNum = firstAddNodeIdx - start;
			focus = mChildren->at(idx)->getLastView();
			operateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::LEFT);
			TempContainerMgr::addChildren(children, start, addNum, idx, true);
		}
		end = start + num - addNum;
		idx = mChildren->size();
		NodeChildrenMgr * nodeMgr = this;
		for (int i = start + addNum; i < end; i ++)
		{
			child = children->at(i);
			nodeMgr->addChild(child, -1, true, useParagraphDataList, useIndex);
			nodeMgr = child->getNodeChildrenMgr();
			focus = child->getLastView();
			operateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::LEFT);
		}
	}
}
void NodeChildrenMgr::addAllChildrenToParagraphAndView()
{
	int size = mChildren->size();
	BaseContainer * obj = NULL;
	SystemParams * sys = mOwner->getOperateMgr()->getSystemParams();
	QList<BaseContainer *> * tempChildren = mChildren;
	// ���ΪҳütempContainer
	if (mOwner->isObj(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER))
	{
		for (int i = 0; i < size; i++)
		{
			obj = mChildren->at(i);
			//if (obj->isPageNumber())
			//{
			//	if (!sys->isTitleNumView())
			//	{
			//		tempChildren = new QList<BaseContainer *>();
			//		for (int j = 0; j < size; j ++)
			//		{
			//			if (j != i)
			//				tempChildren->push_back(mChildren->at(j));
			//		}
			//		break;
			//	}
			//}
		}
	}
	// ���Ϊҳ��tempContainer
	else if (mOwner->isObj(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER))
	{
		for (int i = 0; i < size; i++)
		{
			obj = mChildren->at(i);
			//if (obj->isPageNumber())
			//{
			//	if (!sys->isBottomNumView())
			//	{
			//		tempChildren = new QList<BaseContainer *>();
			//		for (int j = 0; j < size; j++)
			//		{
			//			if (j != i)
			//				tempChildren->push_back(mChildren->at(j));
			//		}
			//		break;
			//	}
			//}
		}
	}
	mParagraphMgr->addChildren(tempChildren, true, NULL, -1);
}
void NodeChildrenMgr::addRecordChildrenToParagraphAndView(ViewObj * lastView)
{
	mParagraphMgr->addRecordChildren(mChildren, lastView);
}
void NodeChildrenMgr::delRecordChildrenToParagraphAndView()
{
	mParagraphMgr->delRecordChildren(mChildren);
}

int NodeChildrenMgr::delChild(BaseContainer * child, bool refreshIdx)
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

	int size = mChildren->size();
	if (child->isNode())
	{
		mNodeList->removeOne(child);
	}
	//// ���ɾ����ǰΨһ���ɾ���ڵ㱾��
	//if (size == 1)
	//{
	//	BaseTemplate * tempParent = mOwner->getTempParent();
	//	BaseTemplate * grandTempParent = tempParent->getTempParent();
	//	return grandTempParent->delChild(tempParent, true);
	//}
	//else
	{
		return TempContainerMgr::delChild(child, refreshIdx);
	}
}
int NodeChildrenMgr::delChildren(int start, int num, bool refreshIdx)
{
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
	// ����б���û�нڵ㣬������ɾ��
	if (firstNodeIdx == -1)
	{
		int lastDelIndex = start + num - 1;
		BaseContainer * lastDelObj = mChildren->at(lastDelIndex);
		// nodeǰ����ͨ�س�����ɾ
		if (lastDelObj->isEnter() && lastDelIndex < mChildren->size()
			&& (lastDelIndex == mChildren->size() - 1 || mChildren->at(lastDelIndex + 1)->isNode()))
		{
			return TempContainerMgr::delChildren(start, num - 1, refreshIdx);
		}
		return TempContainerMgr::delChildren(start, num, refreshIdx);
	}

	// ����нڵ�

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


	OperateMgr * operateMgr = mOwner->getOperateMgr();
	// ���ɾ����ʼ�Ϊ�ڵ㣬����ɾ��һ���ڵ�ǰ��Ļس�
	if (firstNodeIdx > 0)
	{
		int normalObjNum = firstNodeIdx - start;
		int delNum = 0;
		if (normalObjNum > 1)
		{
			delNum = normalObjNum - 1;
			TempContainerMgr::delChildren(start, delNum, refreshIdx);
			ViewObj * leaveEnter = (ViewObj *)mChildren->at(start)->getLastView();
			operateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, leaveEnter, BaseConstants::Direction::LEFT);
		}
		else
		{
			ViewObj * destFocus = NULL;
			if (start > 0)
				destFocus = (ViewObj *)mChildren->at(start - 1)->getLastView();
			else
				destFocus = (ViewObj *)mChildren->at(start + num + 1)->getFirstView();
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
		TempContainerMgr::delChildren(firstNodeIdx, delNum, refreshIdx);
		return BaseConstants::ReturnType::HAVE_NOT_DEL_ENTER;
	}
	// ���ɾ������ȫ�ǽڵ㣬������ɾ
	else
	{
		return TempContainerMgr::delChildren(start, num, refreshIdx);
	}
}