#include "TempContainerMgr.h"



#include "ViewObj.h"

#include "ParagraphManager.h"

#include "ParagraphData.h"
#include "TempParagraphMgrData.h"

#include "OperateMgr.h"
#include "SystemParams.h"

#include "BaseSystem.h"

#include "ConstantsID.h"
#include "GlobalTemplate.h"

TempContainerMgr::TempContainerMgr(BaseContainer * owner, ParagraphManager * paragraphMgr, int classID)
: BaseChildrenManager(owner, classID)
{
	mParagraphMgr = paragraphMgr;
}
TempContainerMgr::TempContainerMgr(BaseContainer * owner, ParagraphManager * paragraphMgr, TempContainerMgr* otherTempContainerMgr) : BaseChildrenManager(owner, otherTempContainerMgr)
{
	mParagraphMgr = paragraphMgr;
}
TempContainerMgr::~TempContainerMgr()
{
	mParagraphMgr = NULL;
}
ParagraphManager * TempContainerMgr::getParagraphMgr()
{
	return mParagraphMgr;
}
bool TempContainerMgr::judgeCanOperate()
{
	return true;
}
bool TempContainerMgr::addChild(BaseContainer * child, int idx, bool refreshIdx)
{
	return addChild(child, idx, refreshIdx, NULL, -1);
}
bool TempContainerMgr::addChild(BaseContainer * child, int idx, bool refreshIdx, QList<ParagraphData *> * useParagraphDataList, int useIndex)
{
	if (!judgeCanOperate())
	{
		return false;
	}
	// 加到temp
	BaseChildrenManager::addChild(child, idx, refreshIdx);

	mParagraphMgr->addChild(child, refreshIdx, useParagraphDataList, useIndex, true);
	return true;
}
void TempContainerMgr::addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx)
{
	addChildren(children, start, num, idx, refreshIdx, NULL, -1);
}
void TempContainerMgr::addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx
	, QList<ParagraphData *> * useParagraphDataList, int useIndex)
{
	if (!judgeCanOperate() || num == 0)
	{
		return;
	}
	//if (child->isPageNumber())
	//{
	//	int a = 0;
	//}
	if (num < 0)
	{
		num = children->size();
	}
	// 加到temp
	BaseChildrenManager::addChildren(children, start, num, idx, refreshIdx);

	mParagraphMgr->addChildren(children, start, num, refreshIdx, useParagraphDataList, useIndex);

}
void TempContainerMgr::addAllChildrenToParagraphAndView(bool needAddToParagraph)
{
	int size = mChildren->size();
	BaseContainer * obj = NULL;
	SystemParams * sys = mOwner->getOperateMgr()->getSystemParams();
	QList<BaseContainer *> * tempChildren = mChildren;
	//// 如果为页眉tempContainer
	//if (mOwner->isObj(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER))
	//{
	//	for (int i = 0; i < size; i ++)
	//	{
	//		obj = mChildren->at(i);
	//		//if (obj->isPageNumber())
	//		//{
	//		//	if (!sys->isTitleNumView())
	//		//	{
	//		//		tempChildren = new QList<BaseContainer *>();
	//		//		for (int j = 0; j < size; j ++)
	//		//		{
	//		//			if (j != i)
	//		//				tempChildren->push_back(mChildren->at(j));
	//		//		}
	//		//		break;
	//		//	}
	//		//}
	//	}
	//}
	//// 如果为页脚tempContainer
	//else if (mOwner->isObj(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER))
	//{
	//	for (int i = 0; i < size; i++)
	//	{
	//		obj = mChildren->at(i);
	//		//if (obj->isPageNumber())
	//		//{
	//		//	if (!sys->isBottomNumView())
	//		//	{
	//		//		tempChildren = new QList<BaseContainer *>();
	//		//		for (int j = 0; j < size; j++)
	//		//		{
	//		//			if (j != i)
	//		//				tempChildren->push_back(mChildren->at(j));
	//		//		}
	//		//		break;
	//		//	}
	//		//}
	//	}
	//}
	mParagraphMgr->addChildren(tempChildren, 0, -1, true, NULL, -1, true, needAddToParagraph);
}
void TempContainerMgr::addRecordChildrenToParagraphAndView(ViewObj * lastView)
{
	mParagraphMgr->addRecordChildren(mChildren, lastView);
}
void TempContainerMgr::delRecordChildrenToParagraphAndView()
{
	mParagraphMgr->delRecordChildren(mChildren);
}
void TempContainerMgr::setParagraphManager(ParagraphManager * paragraphMgr)
{
	mParagraphMgr = paragraphMgr;
}

int TempContainerMgr::delChild(BaseContainer * child, bool refreshIdx)
{
	if (!judgeCanOperate())
	{
		return 0;
	}
	//// 如果为模板容器第0个对象，则不能
	//if (child->getTempIndex() == 0)
	//{
	//	return -1;
	//}
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

	mParagraphMgr->delChild(child, refreshIdx, true);
	int returnType = BaseChildrenManager::delChild(child, refreshIdx);

	return returnType;
}
int TempContainerMgr::delChildren(int start, int num, bool refreshIdx)
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

	mParagraphMgr->delChildren(mChildren, start, num, refreshIdx);

	int returnType = BaseChildrenManager::delChildren(start, num, refreshIdx);

	return returnType;
}