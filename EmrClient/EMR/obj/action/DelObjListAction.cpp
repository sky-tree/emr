#include "DelObjListAction.h"

#include "TempContainerMgr.h"

#include "StepActionManger.h"

#include "OperateMgr.h"

#include "BaseParagraph.h"
#include "ParagraphData.h"

#include "FlagParam.h"



#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseTemplate.h"

#include "BaseTools.h"
#include "BaseSystem.h"
#include "ConstantsID.h"

DelObjListAction::DelObjListAction(StepActionManger * mgr) : BaseAction(mgr, ConstantsID::ClassId::DEL_OBJ_LIST_ACTOIN)
{
	mPreEffectLogicPageIndex = -1;
	mAfterEffectLogicPage = NULL;
	mStartFocus = NULL;
	mEndFocus = NULL;
	mDelList = new QList<BaseContainer *>();
	mBufParagraphData = new QList<ParagraphData *>();
}

DelObjListAction::~DelObjListAction()
{
	mOperateMgr = NULL;
}
void DelObjListAction::doAction(BaseParagraph * nextParagraph, ViewObj * preEffectLogicPage, QList<BaseContainer *> * objList, ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir
	, ViewObj * afterFocus, char dir, bool isUpsidedown)
{
	mStartFocus = startFocus;
	mStartDir = startDir;
	mEndFocus = endFocus;
	mEndDir = endDir;
	for (int i = 0; i < objList->size(); i ++)
	{
		mDelList->push_back(objList->at(i));
	}

	mAfterFocus = afterFocus;
	mAfterDir = dir;
	mIsUpsidedown = isUpsidedown;

	mPreEffectLogicPageIndex = preEffectLogicPage->getViewIndex();
	mAfterEffectLogicPage = mOperateMgr->getEffectLogicPage();

	BaseContainer * obj = NULL;
	QList<BaseContainer *> * viewList = BaseSystem::getInstance()->getClearTempContainerList();
	BaseTools::exchangeListToViewList(objList, 0, objList->size(), viewList);
	BaseParagraph * paragraph = NULL;
	ParagraphData * data = NULL;
	int size = viewList->size();
	for (int i = 0; i < size; i++)
	{
		obj = viewList->at(i);
		paragraph = obj->getParagraphParent();
		data = (ParagraphData *)paragraph->getContentData();
		BaseTools::addToListWithoutSame(mBufParagraphData, data);
	}

	// 如果最后一个为回车，需要将下一段的属性加进去
	if (nextParagraph)
	{
		data = (ParagraphData *)nextParagraph->getContentData();
		BaseTools::addToListWithoutSame(mBufParagraphData, data);
	}
	//size = mDelList->size();
	//if (size > 0)
	//{
	//	obj = mDelList->at(size - 1);
	//	if (obj->isEnter())
	//	{
	//		paragraph = paragraph->getNextParagraph();
	//		data = (ParagraphData *)paragraph->getContentData();
	//		BaseTools::addToListWithoutSame(mBufParagraphData, data);
	//	}
	//}
}
void DelObjListAction::unDo()
{
	mOperateMgr->clearChooseList(true);
	// 先将光标设置到最初删除后位置
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mAfterFocus, mAfterDir);


	BaseContainer* tempParent = mAfterFocus->getTempParent();
	int tempIdx = -1;
	BaseContainer* tempBaseContainer = NULL;
	mAfterFocus->findNormalTempContainerFromParent(tempBaseContainer, tempIdx);
	if (tempBaseContainer == tempParent)
	{
		if (mAfterDir == BaseConstants::Direction::RIGHT)
		{
			tempIdx++;
		}
	}
	else
	{
		tempParent = tempBaseContainer->getChild(tempIdx);
		ViewObj* firstChild = tempParent->getFirstChildView();
		if (firstChild == mAfterFocus&&mAfterDir == BaseConstants::Direction::LEFT)
		{

		}
		else
		{
			tempIdx++;
		}
	}

	int firstNodeIdx = -1;
	int size = mDelList->size();
	BaseContainer * obj = NULL;
	for (int i = 0; i < size; i++)
	{
		obj = mDelList->at(i);
		if (obj->isNode())
		{
			firstNodeIdx = i;
			break;
		}
	}

	TempContainerMgr * tempMgr = (TempContainerMgr *)tempBaseContainer->getChildrenMgr();


	// 如果增删对象中没有节点
	if (firstNodeIdx < 0)
	{
		if (mBufParagraphData->size() > 1)
			tempMgr->addChildren(mDelList, 0, size, tempIdx, true, mBufParagraphData, 1);
		else
			tempMgr->addChildren(mDelList, 0, size, tempIdx, true, NULL, -1);
	}
	else
	{
		// 注意，前面的回车时不能删的，所以不需要添加
		if (firstNodeIdx > 1)
			tempMgr->addChildren(mDelList, 0, firstNodeIdx - 1, tempIdx, true, NULL, -1);
		tempMgr->addChildren(mDelList, firstNodeIdx, -1, tempMgr->getChildrenSize(), true, NULL, -1);
	}

	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS, mStartFocus, mStartDir);
	mOperateMgr->setPress(BaseConstants::MousePressType::RELEASE, mEndFocus, mEndDir);
	mOperateMgr->setUpsidedown(mIsUpsidedown);
}
void DelObjListAction::reDo()
{
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS, mStartFocus, mStartDir);
	mOperateMgr->setPress(BaseConstants::MousePressType::RELEASE, mEndFocus, mEndDir);
	//if (mStartFocus != mEndFocus || mStartDir != mEndDir)
	{
		mOperateMgr->refreshChooseList(true, false, true);
	}
	mOperateMgr->pressBackspace(0);
	//mOperateMgr->setEffectLogicPage(mAfterEffectLogicPage);
}