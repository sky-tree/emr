#include "AddObjListAction.h"

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

AddObjListAction::AddObjListAction(StepActionManger * mgr) : BaseAction(mgr, ConstantsID::ClassId::ADD_OBJ_LIST_ACTOIN)
{
	mPreEffectLogicPage = NULL;
	mAfterEffectLogicPage = NULL;
	mAddList = NULL;
	mBufParagraphData = new QList<ParagraphData *>();
}

AddObjListAction::~AddObjListAction()
{
	mOperateMgr = NULL;
	mPreEffectLogicPage = NULL;
}
void AddObjListAction::doAction(QList<BaseContainer *> * objList, ViewObj * beforeFocus, char beforeDir)
{
	mBeforeFocus = beforeFocus;
	mBeforeDir = beforeDir;
	//for (int i = 0; i < objList->size(); i ++)
	//{
	//	mAddList->push_back(objList->at(i));
	//}
	// 添加对象可以直接赋引用，因为添加的都是临时局部new出来的对象
	mAddList = objList;
	mAfterEffectLogicPage = mOperateMgr->getEffectLogicPage();


	BaseContainer * obj = NULL;
	QList<BaseContainer *> * viewList = BaseSystem::getInstance()->getClearTempContainerList();
	BaseTools::exchangeListToViewList(objList, 0, objList->size(), viewList);
	BaseParagraph * paragraph = NULL;
	ParagraphData * data = NULL;
	int size = viewList->size();
	for (int i = 0; i < size; i ++)
	{
		obj = viewList->at(i);
		paragraph = obj->getParagraphParent();
		data = (ParagraphData *)paragraph->getContentData();
		BaseTools::addToListWithoutSame(mBufParagraphData, data);
		//// 如果最后一个为回车，需要将下一段的属性加进去
		//if (i == size - 1 && obj->isEnter())
		//{
		//	paragraph = paragraph->getNextParagraph();
		//	data = (ParagraphData *)paragraph->getContentData();
		//	BaseTools::addToListWithoutSame(mBufParagraphData, data);
		//}
	}

	// 如果最后一个为回车，需要将下一段的属性加进去
	size = mAddList->size();
	if (size > 0)
	{
		obj = mAddList->at(size - 1);
		if (obj->isEnter())
		{
			paragraph = paragraph->getNextParagraph();
			data = (ParagraphData *)paragraph->getContentData();
			BaseTools::addToListWithoutSame(mBufParagraphData, data);
		}
	}
}
void AddObjListAction::unDo()
{
	BaseContainer * first = mAddList->at(0);
	BaseContainer * last = mAddList->at(mAddList->size() - 1);
	ViewObj * firstView = first->getFirstView();
	ViewObj * lastView = last->getLastView();
	char dir = BaseConstants::Direction::RIGHT;
	// 设置光标,并根据光标刷新选中项
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS, firstView, BaseConstants::Direction::LEFT);
	if (lastView->isEnter())
	{
		// 如果尾项不为节点
		if (!last->isNode())
		{
			lastView = lastView->getNextViewFromParagraphParent();
			dir = BaseConstants::Direction::LEFT;
		}
	}
	mOperateMgr->setPress(BaseConstants::MousePressType::RELEASE, lastView, dir);
	mOperateMgr->refreshChooseList(true, false, true);
	// 按下backspace删除选中项
	mOperateMgr->pressBackspace(0,false);
}
void AddObjListAction::reDo()
{
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mBeforeFocus, mBeforeDir);
	FlagParam * pressEnd = mOperateMgr->getPressEnd();
	int size = mAddList->size();
	char dir = 0;
	ViewObj * focus = NULL;
	mOperateMgr->getFocusAndDirection(focus, dir);
	BaseContainer* tempParent = NULL;
	BaseContainer* focusTempParent = NULL;
	ViewObj * addRow = NULL;

	BaseContainer* tempContainer = NULL;
	int tempIdx = -1;
	focus->findNormalTempContainerFromParent(tempContainer, tempIdx); /*应为focus是最低层的元素。*/
	tempParent = focus->getTempParent();
	if (tempParent == tempContainer)/*focus是普通文字*/
	{
		if (dir == BaseConstants::Direction::RIGHT)
		{
			tempIdx++;
		}
		focusTempParent = tempParent;
		addRow = focus->getViewParent();
	}
	else if (!tempParent->isNode())
	{
		tempParent = tempContainer->getChildrenMgr()->getChildren()->at(tempIdx);
		BaseContainer* firstView = tempParent->getFirstChildView();
		focusTempParent = tempParent;
		if (firstView == focus&& dir == BaseConstants::Direction::LEFT)
		{
			addRow = firstView->getViewParent();
		}
		else
		{
			tempIdx++;
			focus = (ViewObj*)focusTempParent->getLastChildView();
			dir = BaseConstants::Direction::RIGHT;
			focus->setFocus(true);
			addRow = focus->getViewParent();
		}
		tempParent = tempContainer;
	}
	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	if (mBufParagraphData->size() > 1)
		tempMgr->addChildren(mAddList, 0, size, tempIdx, true, mBufParagraphData, 1);
	else
		tempMgr->addChildren(mAddList, 0, size, tempIdx, true, NULL, -1);

	//mOperateMgr->setEffectLogicPage(mAfterEffectLogicPage);
	if (mBeforeDir == BaseConstants::Direction::RIGHT)
	{
		BaseContainer * lastAddObj = mAddList->at(size - 1);
		mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE
			, lastAddObj->getLastChildView(true), BaseConstants::Direction::RIGHT);
	}
}