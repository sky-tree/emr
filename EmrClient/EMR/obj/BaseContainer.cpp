#include "BaseContainer.h"




#include "WriterManager.h"

#include "FileData.h"

#include "ObjKeyManager.h"
#include "ObjMouseManager.h"
#include "LogicManager.h"


#include "BaseChildrenManager.h"

#include "NodeMgr.h"
#include "NodeChildrenMgr.h"

#include "BaseLayout.h"

#include "BaseLogic.h"

#include "ViewObj.h"

#include "CrossParentMgr.h"

#include "FlagViewParam.h"
#include "BaseTemplate.h"
#include "BaseParagraph.h"
#include "ParagraphData.h"

#include "OperateMgr.h"
#include "BaseSystem.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "StrContentData.h"
#include "CodeTempData.h"

BaseContainer::BaseContainer(WriterManager * writerMgr, int objID) : BaseElement(ConstantsID::ClassId::BASE_CONTAINER)
, mWriterMgr(writerMgr)
, mBlockParentType(BaseConstants::NONE)
, mObjID(objID)
, mContentData(NULL)
, mLogicManager(NULL)
, mChildrenMgr(NULL)
, mViewParent(NULL)
, mTempParent(NULL)
, mParagraphParent(NULL)
, mMaskColor(0)
, mTempIndex(-1)
, mCanChooseID(-1)
, mIsShowTidy(false)
{
	changeStatus(BaseConstants::ObjStatus::FREE, false);
}
BaseContainer::BaseContainer(WriterManager * writerMgr, int classID, int objID) : BaseElement(classID)
, mWriterMgr(writerMgr)
, mBlockParentType(BaseConstants::NONE)
, mObjID(objID)
, mContentData(NULL)
, mLogicManager(NULL)
, mChildrenMgr(NULL)
, mViewParent(NULL)
, mTempParent(NULL)
, mParagraphParent(NULL)
, mMaskColor(0)
, mTempIndex(-1)
, mCanChooseID(-1)
, mIsShowTidy(false)
{
	changeStatus(BaseConstants::ObjStatus::FREE, false);
}
BaseContainer::BaseContainer(WriterManager * writerMgr, BaseContainer * source) : BaseElement(source->getClassId())
, mWriterMgr(writerMgr)
, mBlockParentType(source->mBlockParentType)
, mObjID(source->mObjID)
, mContentData(NULL)
, mLogicManager(NULL)
, mChildrenMgr(NULL)
, mViewParent(NULL)
, mTempParent(NULL)
, mParagraphParent(NULL)
, mMaskColor(0)
, mTempIndex(-1)
{
	mIsShowTidy = source->mIsShowTidy;
	mCanChooseID = source->mCanChooseID;
	changeStatus(BaseConstants::ObjStatus::FREE, false);
}

BaseContainer::~BaseContainer()
{
	if (mChildrenMgr)
	{
		delete mChildrenMgr;
		mChildrenMgr = NULL;
	}
	if (mContentData)
	{
		delete mContentData;
		mContentData = NULL;
	}
	if (mLogicManager)
	{
		delete mLogicManager;
		mLogicManager = NULL;
	}

	mViewParent = NULL;
	mTempParent = NULL;
	mParagraphParent = NULL;
	mWriterMgr = NULL;
}

short BaseContainer::getStatus()
{
	return mStatus;
}
void BaseContainer::changeStatus(short status, bool circleChildren)
{
	if (isObj(ConstantsID::ContainerId::EXCEL_PAGE))
	{
		int a = 0;
	}
	mStatus = status;
	if (circleChildren && haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mChildrenMgr->getChildren();
		int size = children->size();
		for (int i = 0; i < size; i++)
		{
			children->at(i)->changeStatus(status, circleChildren);
		}
	}
}
bool BaseContainer::isTempleteParent(BaseContainer * obj, bool isCircle)
{
	BaseContainer * parent = obj->getTempParent();
	while (parent != this)
	{
		if (!isCircle)
		{
			return false;
		}
		parent = parent->getTempParent();
		if (!parent)
		{
			return false;
		}
	}
	return true;
}
ViewObj * BaseContainer::getFlagObj()
{
	if (mWriterMgr)
	{
		return mWriterMgr->getFlag();
	}
	else
	{
		return NULL;
	}
}
void BaseContainer::getFocusObjAndDir(ViewObj* & focusObj, char & dir)
{
	if (mWriterMgr)
	{
		mWriterMgr->getFocusObjAndDir(focusObj, dir);
	}
	else
	{
		return;
	}
}
void BaseContainer::savePressStartAndEnd()
{
	if (mWriterMgr)
	{
		mWriterMgr->savePressStartAndEnd();
	}
	else
	{
		return;
	}
}
void BaseContainer::backSetPressStartAndEnd(bool freshChooseList)
{
	if (mWriterMgr)
	{
		mWriterMgr->backSetPressStartAndEnd(freshChooseList);
	}
	else
	{
		return;
	}
}
StrContentData * BaseContainer::getFocusStrData()
{
	if (mWriterMgr)
	{
		return mWriterMgr->getFocusTxtData();
	}
	else
	{
		return NULL;
	}
}
void BaseContainer::haveDeleteFromParent(BaseContainer * parent)
{
}
void BaseContainer::haveAddToParent(BaseContainer * parent)
{
}
void BaseContainer::setParent(BaseContainer * parent)
{
	if (parent->isView())
	{
		if (mViewParent == parent)
		{
			return;
		}
		if (mViewParent)
		{
			bool needRemove = true;
			// 如果为页眉、页脚
			if (isTitleOrBottomView())
			{
				needRemove = false;
			}
			if (isExcelRow())
			{
				CrossParentMgr * mgr = ((ViewObj *)this)->getCrossParentMgr();
				if (mgr && mgr->haveCrossParent())
				{
					needRemove = false;
				}
			}
			if (needRemove)
			{
				bool haveRemoved = mViewParent->getChildren()->removeOne(this);
				if (haveRemoved)
					mViewParent->getLayout()->openReplace();
			}
		}
		//else
		//{

		//	if (isExcelRow() && mChildrenMgr->getChildrenSize() == 2)
		//	{
		//		int a = 0;
		//	}
		//}
		mViewParent = (ViewObj *)parent;
	}
	else if (parent->isParagraph())
	{
		if (mParagraphParent == parent)
		{
			return;
		}
		if (mParagraphParent)
		{
			mParagraphParent->getChildren()->removeOne(this);
		}
		mParagraphParent = (BaseParagraph *)parent;
	}
	else if (parent->isTemp())
	{
		if (mTempParent == parent)
		{
			return;
		}
		if (mTempParent)
		{
			mTempParent->getChildren()->removeOne(this);
		}
		mTempParent = (BaseTemplate *)parent;
	}
}
void BaseContainer::clearParent(int classID)
{
	if (classID == ConstantsID::ClassId::VIEW_OBJ)
	{
		if (mViewParent)
		{
			mViewParent->getLayout()->openReplace();

			//if (isExcelRow() && mChildrenMgr->getChildrenSize() == 2)
			//{
			//	int a = 0;
			//}
			mViewParent = NULL;
		}
		ViewObj * temp = (ViewObj *)this;
		BaseLayout * layout = temp->getLayout();
		if (layout)
		{
			layout->closeReplace();
		}
	}
	else if (classID == ConstantsID::ClassId::PARAGRAPH_OBJ)
	{
		mParagraphParent = NULL;
	}
	else if (classID <= ConstantsID::ClassId::BASE_TEMPLATE)
	{
		mTempParent = NULL;
	}
	else if (classID == 0)
	{
		if (isExcelRow() && mChildrenMgr->getChildrenSize() == 2)
		{
			int a = 0;
		}
		mViewParent = NULL;
		mParagraphParent = NULL;
		mTempParent = NULL;
	}
}
void BaseContainer::setTempleteParent(BaseContainer * parent)
{
	if (mTempParent == parent)
	{
		return;
	}
	if (mTempParent)
	{
		QList<BaseContainer *> *children = mTempParent->getChildren();
		if (children)
		{
			children->removeOne(this);
		}
		mTempParent = (BaseTemplate *)parent;
	}
	else
	{
		mTempParent = (BaseTemplate*)parent;
	}

}
bool BaseContainer::isView()
{
	return (mClassID == ConstantsID::ClassId::VIEW_OBJ) || (mClassID == ConstantsID::ClassId::TEMPLATE_VIEW_OBJ_CLASS);
}
bool BaseContainer::isTemp()
{
	if (mClassID <= ConstantsID::ClassId::BASE_TEMPLATE)
	{
		return true;
	}
	return false;
}
bool BaseContainer::isParagraph()
{
	if (mClassID == ConstantsID::ClassId::PARAGRAPH_OBJ)
	{
		return true;
	}
	return false;
}
BaseTemplate * BaseContainer::getTempParent()
{
	return mTempParent;
}
BaseParagraph * BaseContainer::getParagraphParent()
{
	return mParagraphParent;
}
ViewObj * BaseContainer::getViewParent()
{
	return mViewParent;
}
ViewObj * BaseContainer::getFirstView()
{
	if (isView())
	{
		return (ViewObj *)this;
	}
	QList<BaseContainer *> * children = getChildren();
	BaseContainer * child = children->at(0);
	if (child->isView())
	{
		return (ViewObj *)child;
	}
	else
	{
		return child->getFirstChildView();
	}
}
ViewObj * BaseContainer::getLastView()
{
	if (isView())
	{
		return (ViewObj *)this;
	}
	QList<BaseContainer *> * children = getChildren();
	int size = children->size();
	BaseContainer * child = children->at(size - 1);
	if (child->isView())
	{
		return (ViewObj *)child;
	}
	else
	{
		return child->getLastChildView();
	}
}
BaseLogic * BaseContainer::getLogicByClassID(int id)
{
	return mLogicManager->getLogicByClassID(id);
}
void BaseContainer::initContentData(BaseContentData * data)
{
	mContentData = data;
}
void BaseContainer::initChildrenMgr(BaseChildrenManager * childrenMgr)
{
	mChildrenMgr = childrenMgr;
}
void BaseContainer::initLogicMgr(LogicManager * logicMgr)
{
	mLogicManager = logicMgr;
}
bool BaseContainer::isObj(int destID)
{
	return mObjID == destID;
}
void BaseContainer::setBlockType(int block)
{
	mBlockParentType = block;
}
bool BaseContainer::isBlock(char bit)
{
	if ((mBlockParentType & bit) != 0)
		return true;
	return false;
}
WriterManager * BaseContainer::getWriterMgr()
{
	return mWriterMgr;
}
OperateMgr * BaseContainer::getOperateMgr()
{
	return mWriterMgr->getOperateMgr();
}
void BaseContainer::checkIsNeedUndo()
{
	mWriterMgr->checkIsNeedUndo();
}
BaseChildrenManager * BaseContainer::getChildrenMgr()
{
	return mChildrenMgr;
}
void BaseContainer::setChildWriteManger(WriterManager * writeManger)
{
	if (mChildrenMgr)
	{
		mChildrenMgr->setChildWriteManger(writeManger);
	}
}
NodeMgr * BaseContainer::getNodeMgr()
{
	return (NodeMgr *)mChildrenMgr;
}
NodeChildrenMgr * BaseContainer::getNodeChildrenMgr()
{
	return (NodeChildrenMgr *)mChildrenMgr;
}
int BaseContainer::getObjID()
{
	return mObjID;
}
//int BaseContainer::getIndexFromTempContainer()
//{
//	BaseContainer * tempParent = mTempParent;
//	int index = mTempIndex;
//	while (!tempParent->isTempContainer())
//	{
//		tempParent = tempParent->getTempParent();
//		index = tempParent->getTempIndex();
//	}
//	return index;
//}
bool BaseContainer::haveChild(BaseContainer * child)
{
	if (haveChildren() > 0)
	{
		return mChildrenMgr->getChildren()->indexOf(child) >= 0;
	}
	return false;
}
char BaseContainer::haveChildren()
{
	if (!mChildrenMgr)
	{
		return -2;
	}
	return mChildrenMgr->haveChildren();
}
QList<BaseContainer *> * BaseContainer::getChildren()
{
	if (!mChildrenMgr)
	{
		return NULL;
	}
	return mChildrenMgr->getChildren();
}
//void BaseContainer::exchangeToViewChildren(QList<ViewObj *> * source)
//{
//	if (isView())
//	{
//		source->push_back((ViewObj *)this);
//		return;
//	}
//	if (haveChildren() < 1)
//	{
//		return;
//	}
//	QList<BaseContainer *> * children = mChildrenMgr->getChildren();
//	int size = children->size();
//	for (int i = 0; i < size; i++)
//	{
//		children->at(i)->exchangeToViewChildren(source);
//	}
//}
BaseContentData * BaseContainer::getContentData()
{
	return mContentData;
}
StrContentData * BaseContainer::getStrContentData()
{
	return (StrContentData *)mContentData;
}
ParagraphData * BaseContainer::getParagraphData()
{
	return (ParagraphData *)mContentData;
}
BaseContainer * BaseContainer::getChild(int idx)
{
	return mChildrenMgr->getChildren()->at(idx);
}
BaseContainer * BaseContainer::getLastChild(bool isCircle)
{
	if (!mChildrenMgr)
	{
		return this;
	}
	if (!isCircle)
		return mChildrenMgr->getLastChild();
	else
		return mChildrenMgr->getLastChild()->getLastChild(true);
}
BaseContainer * BaseContainer::findChildrenByID(int objID, bool circle)
{
	if (isObj(objID))
	{
		return this;
	}
	if (haveChildren() < 1)
	{
		return NULL;
	}
	QList<BaseContainer *> * children = getChildren();
	int size = children->size();
	for (int i = 0; i < size; i++)
	{
		BaseContainer * child = children->at(i);
		if (child->isObj(objID))
		{
			return child;
		}
		else if (circle)
		{
			BaseContainer * tmp = child->findChildrenByID(objID, true);
			if (tmp)
			{
				return tmp;
			}
		}
	}
	return NULL;
}
/*查找最后一个元素容器*/
BaseContainer * BaseContainer::findLastContainer()
{
	BaseContainer * lastElement = findLastElementFromChildren(); //找最低层的元素对象
	/*if (mObjID < ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER)
	{
	BaseContainer* container = findContainerFromChildren(ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER);
	if (container)
	{

	}
	return container;
	}
	else if (mObjID == ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER)
	{
	return findContainerFromBrothers(ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER);
	}
	else
	{
	}*/
	return lastElement->findTempFromParent(ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER, true);

}
void BaseContainer::findNormalTempContainerFromParent(BaseContainer *&normalTempContainer, int &index)
{

	normalTempContainer = findTempFromParent(ConstantsID::ContainerId::NODE_CONTAINER, true, index);
	if (normalTempContainer)
	{
		return;
	}
	normalTempContainer = findTempFromParent(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER, true, index);
	if (normalTempContainer)
	{
		return;
	}
	normalTempContainer = findTempFromParent(ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER, true, index);
	if (normalTempContainer)
	{
		return;
	}
	normalTempContainer = findTempFromParent(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, true, index);
	if (normalTempContainer)
	{
		return;
	}
	normalTempContainer = findTempFromParent(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, true, index);
}
/*index = -1 时，表示寻找最后一个*/
BaseContainer * BaseContainer::findContainerFromChildren(int objId, int index)
{
	if (haveChildren() < 1)
	{
		return NULL;
	}
	QList<BaseContainer *> * children = getChildren();
	int size = children->size();
	if (size == 0)
	{
		return NULL;
	}
	if (index >= size)
	{
		return NULL;
	}
	if (index == -1)
	{
		index = size - 1;
	}
	BaseContainer * child = children->at(index);
	if (child->isObj(objId))
	{
		return child;
	}
	else
	{
		return NULL;
	}
}
BaseContainer *BaseContainer::findContainerFromBrothers(int objId, int index)
{
	return NULL;
}
ViewObj * BaseContainer::findViewFromParent(int objId, bool circle)
{
	ViewObj * parent = getViewParent();
	if (parent)
	{
		if (parent->isObj(objId))
		{
			return parent;
		}
		else if (circle)
		{
			return parent->findViewFromParent(objId, circle);
		}
	}
	return NULL;
}
BaseContainer * BaseContainer::findTempFromParent(int objId, bool circle)
{
	BaseContainer * parent = NULL;
	if (isObj(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER))
	{
		parent = ((BaseTemplate*)this)->getTemplateParentInExcelCell();
	}
	else
	{
		parent = getTempParent();
	}
	if (parent)
	{
		if (parent->isObj(objId))
		{
			return parent;
		}
		else if (circle)
		{
			return parent->findTempFromParent(objId, circle);
		}
	}
	return NULL;
}
BaseContainer * BaseContainer::findTempFromParent(int objId, bool circle, int &index)
{
	BaseContainer * parent = NULL;
	if (isObj(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER))
	{
		parent = ((BaseTemplate*)this)->getTemplateParentInExcelCell();
	}
	else
	{
		parent = getTempParent();
	}
	//BaseContainer * parent = getTempParent();
	if (parent)
	{
		if (parent->isObj(objId))
		{
			index = parent->getChildrenMgr()->getChildren()->indexOf(this);
			return parent;
		}
		else if (circle)
		{
			return parent->findTempFromParent(objId, circle, index);
		}
	}
	return NULL;
}
ViewObj * BaseContainer::findParentRow(bool circle)
{
	ViewObj * parent = getViewParent();
	if (parent)
	{
		if (parent->isRow())
		{
			return parent;
		}
		else if (circle)
		{
			return parent->findParentRow(circle);
		}
	}
	return NULL;
}
BaseContainer * BaseContainer::findLastElementFromChildren(bool stopWhenFindView)
{
	if (haveChildren() < 1)
	{
		return this;
	}
	if (stopWhenFindView && isView())
		return this;
	QList<BaseContainer *> * children = getChildren();
	int size = children->size();
	/*if (size == 0)
	{
	return this;
	}*/
	return children->at(size - 1)->findLastElementFromChildren(stopWhenFindView);
}
BaseContainer * BaseContainer::findFirstElementFromChildren(bool stopWhenFindView)
{
	if (haveChildren() < 1)
	{
		return this;
	}
	
	if (stopWhenFindView && isView())
		return this;
	QList<BaseContainer *> * children = getChildren();
	return children->at(0)->findFirstElementFromChildren(stopWhenFindView);
}
bool BaseContainer::isTxt()
{
	if (mObjID == ConstantsID::ContainerId::TXT || mObjID == ConstantsID::ContainerId::TITLE_PAGE_NUMBER
		|| mObjID == ConstantsID::ContainerId::BOTTOM_PAGE_NUMBER || mObjID == ConstantsID::ContainerId::TMPLATE_TXT || mObjID == ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW)
	{
		return true;
	}
	return false;
}
bool BaseContainer::isCommonTxt()
{
	if (mObjID == ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW)
	{
		return false;
	}
	else
	{
		return isTxt();
	}
}
bool BaseContainer::isEnterImgOrHLine()
{
	if (mObjID == ConstantsID::ContainerId::ENTER || mObjID == ConstantsID::ContainerId::IMG || mObjID == ConstantsID::ContainerId::HORIZONTAL_LINE
		|| mObjID == ConstantsID::ContainerId::END_PAGE_LINE)
	{
		return true;
	}
	return false;
}
bool BaseContainer::isPageNumber()
{
	if (isObj(ConstantsID::ContainerId::TITLE_PAGE_NUMBER) || isObj(ConstantsID::ContainerId::BOTTOM_PAGE_NUMBER))
	{
		return true;
	}
	return false;
}
bool BaseContainer::isTitleOrBottomView()
{
	if (isObj(ConstantsID::ContainerId::PAGE_TITLE) || isObj(ConstantsID::ContainerId::PAGE_BOTTOM))
	{
		return true;
	}
	return false;
}
bool BaseContainer::isTxtWindow()
{
	if (isObj(ConstantsID::ContainerId::TXT_WIN_VIEW))
	{
		return true;
	}
	return false;
}
bool BaseContainer::isTemplateElement()
{
	if (mClassID > ConstantsID::ClassId::TEMPLETE_ELEMENT_MIX&&mClassID < ConstantsID::ClassId::TEMPLETE_ELEMENT_MAX)
	{
		return true;
	}
	return false;
}
bool BaseContainer::isTempContainer()
{
	if (mObjID >= ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER
		&& mObjID <= ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER)
	{
		return true;
	}
	return false;
}
bool BaseContainer::isTempContainerExceptDest(int exceptID)
{
	if (mObjID == exceptID)
	{
		return false;
	}
	if (mObjID >= ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER
		&& mObjID <= ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER)
	{
		return true;
	}
	return false;
}
//bool BaseContainer::isTitleBottomOrContentContainer()
//{
//}
bool BaseContainer::isRow()
{
	if (isObj(ConstantsID::ContainerId::NORMAL_ROW))
	{
		return true;
	}
	return false;
}
bool BaseContainer::isExcelRow()
{
	if (isRow())
	{
		if (mChildrenMgr)
		{
			QList<BaseContainer *> * children = mChildrenMgr->getChildren();
			if (!children)
			{
				return false;
			}
			BaseContainer * obj = NULL;
			int size = children->size();
			for (int i = 0; i < size; i++)
			{
				obj = children->at(i);
				if (obj->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
				{
					return true;
				}
			}
		}
	}
	return false;
}
char BaseContainer::isHorizonOrEndPageRow()
{
	if (isRow())
	{
		if (mChildrenMgr)
		{
			QList<BaseContainer *> * children = mChildrenMgr->getChildren();
			if (!children || children->size() < 1)
			{
				return false;
			}
			BaseContainer * obj = children->at(0);
			return obj->isHorizontalOrEndPageLine();
		}
	}
	return 0;
}
bool BaseContainer::isEnter()
{
	if (isObj(ConstantsID::ContainerId::ENTER))
	{
		return true;
	}
	return false;
}
bool BaseContainer::isNode()
{
	if (isObj(ConstantsID::ContainerId::NODE_CONTAINER))
	{
		return true;
	}
	return false;
}
char BaseContainer::isHorizontalOrEndPageLine()
{
	if (isObj(ConstantsID::ContainerId::HORIZONTAL_LINE))
	{
		return 1;
	}
	if (isObj(ConstantsID::ContainerId::END_PAGE_LINE))
	{
		return 2;
	}
	return 0;
}
bool BaseContainer::haveMask()
{
	return mMaskColor != 0;
}
int BaseContainer::getMaskColor()
{
	return mMaskColor;
}
void BaseContainer::setMaskColor(int color)
{
	mMaskColor = color;
}
void BaseContainer::clearMaskColor()
{
	mMaskColor = 0;
}

QString BaseContainer::getCodeId()
{
	CodeTempData* data = (CodeTempData *)this->getContentData();
	return data->getCodeId();
}
void BaseContainer::setCodeId(QString id)
{
	//mCodeId = id;
	CodeTempData* data = (CodeTempData *)this->getContentData();
	data->setCodeId(id);
}

//void BaseContainer::getCodeIdFromTemp()
//{
//	CodeTempData* data = (CodeTempData *)this->getContentData();
//	mCodeId = data->getCodeId();
//}

int BaseContainer::getTempIndex()
{
	return mTempIndex;
}
void BaseContainer::setTempIndex(int start)
{
	mTempIndex = start;
}
void BaseContainer::refreshChildrenIndex(int start)
{
	QList<BaseContainer *> * children = getChildren();
	if (!children)
	{
		return;
	}
	int size = children->size();
	BaseContainer * child = NULL;
	if (isView())
	{
		((ViewObj *)this)->refreshChildrenViewIndex(start);
	}
	else if (isParagraph())
	{
		((BaseParagraph *)this)->refreshChildrenParagraphIndex(start);
	}
	else
	{
		((BaseTemplate *)this)->refreshChildrenTempIndex(start);
	}
}
int BaseContainer::myUpdate()
{
	int returnType = BaseConstants::NONE;
	if (isObj(ConstantsID::ContainerId::FLAG))
	{
		int a = 0;
	}
	if (haveChildren() > 0)
	{
		QList<BaseContainer *>	* children = mChildrenMgr->getChildren();
		int size = children->size();
		// 倒序执行
		for (int i = size - 1; i >= 0; i--)
		{
			BaseContainer * child = children->at(i);
			returnType = child->myUpdate();
			// 如果某个子对象逻辑执行后需要中断后续逻辑，则返回
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
				return returnType;
		}
	}
	bool needBlock = isBlock(BaseConstants::BlockType::ONLY_LOGIC);
	if (!mLogicManager)
	{
		if (needBlock)
			return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		return returnType;
	}
	// 执行自身逻辑
	returnType = mLogicManager->myUpdate();
	if (returnType > BaseConstants::NONE)
		return returnType;
	if (needBlock)
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;

	return returnType;
}
ViewObj * BaseContainer::getParentViewById(int id)
{
	BaseContainer * dest = this;

	while (dest->mObjID != id)
	{
		dest = dest->getViewParent();
		if (dest == NULL)
			return NULL;
	}
	return (ViewObj *)dest;
}
/*必须要有第一个子视图对象*/
ViewObj* BaseContainer::getFirstChildView(bool returnThisWhenNoChildren)
{
	if (haveChildren() <= 0)
	{
		if (returnThisWhenNoChildren)
		{
			if (isView())
				return (ViewObj *)this;
		}
		return NULL;
	}
	QList<BaseContainer *>	* children = mChildrenMgr->getChildren();
	BaseContainer * firstChild = children->at(0);
	if (firstChild->isView())
	{
		return	(ViewObj*)firstChild;
	}
	else
	{
		return firstChild->getFirstChildView();
	}
}
/*必须要有第一个子视图对象*/
ViewObj * BaseContainer::getLastChildView(bool returnThisWhenNoChildren)
{
	if (haveChildren() <= 0)
	{
		if (returnThisWhenNoChildren)
		{
			if (isView())
				return (ViewObj *)this;
		}
		return NULL;
	}
	QList<BaseContainer *>	* children = mChildrenMgr->getChildren();
	int size = children->size();
	BaseContainer * lastChild = children->at(size - 1);
	if (lastChild->isView())
	{
		return	(ViewObj*)lastChild;
	}
	else
	{
		ViewObj* lastView = lastChild->getLastChildView();
		if (!lastView)
		{
			lastView = lastChild->getPreView();
		}
		return lastView;
	}
}
ViewObj* BaseContainer::getNextView(int endParentobjId)
{
	BaseContainer * next = this;
	while (true)
	{
		int tempIdx = next->getTempIndex();
		if (tempIdx < 0)
		{//表格单元格模板是特例
			return NULL;
		}
		BaseContainer * templeteParent = next->getTempParent();
		if (!templeteParent)
		{
			return NULL;
		}
		// 如果当前为窗口
		else if (templeteParent->isObj(endParentobjId))
		{
			return NULL;
		}
		QList<BaseContainer *> * children = templeteParent->getChildren();
		if (!children)
			return NULL;
		if (tempIdx < children->size() - 1)
		{
			next = children->at(tempIdx + 1);
			if (next->isView())
			{
				return (ViewObj *)next;
			}
			else
			{
				if (next->haveChildren() < 1)
				{
					continue;
				}
				else
				{
					return next->getFirstChildView();

				}
			}
		}
		else
		{
			if (templeteParent)
			{
				next = templeteParent;
			}
			else
			{
				return NULL;
			}
		}
	}
	return NULL;

	/*int tempIdx = getTempleteIndexFromParent();
	BaseTemplete * templeteParent = getTempleteParent();
	QList<BaseContainer *> * children = templeteParent->getChildren();
	if (tempIdx < children->size() - 1)
	{
	next = children->at(tempIdx + 1);
	if (next->isViewObj())
	{
	return (ViewObj *)next;
	}
	else
	{
	return next->getFirstChildView();
	}
	}
	else
	{
	int tempParentId = templeteParent->getTempleteIndexFromParent();
	BaseContainer * tempParentParent = templeteParent->getTempleteParent();
	if (tempParentId < tempParentParent->getChildren()->size() - 1)
	{
	next = tempParentParent->getChild(tempParentId + 1);
	if (next->isViewObj())
	{
	return (ViewObj *)next;
	}
	else
	return (ViewObj *)next->getFirstChildView();
	}
	}

	return NULL;*/
}
ViewObj* BaseContainer::getPreView(int endParentobjId)
{
	//判断是否在表格，如果是，不要找到表格外去了
	BaseContainer * excelTemplete = this->findTempFromParent(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER, true);
	bool isInExcel = false;
	if (excelTemplete)
	{
		endParentobjId = ConstantsID::ContainerId::EXCEL_TEMPLETE;
		isInExcel = true;
	}
	bool isNextBreak = false;
	BaseContainer * preObj = this;
	while (true)
	{
		if (!preObj)
		{
			return NULL;
		}
		if (isNextBreak)
		{
			return NULL;
		}
		int objId = preObj->getObjID();
		if (objId == ConstantsID::ContainerId::PAGE_PANEL || objId == ConstantsID::ContainerId::TITLE_TEMP)
		{
			return NULL;
		}
		if (objId == endParentobjId)
		{
			isNextBreak = true;
			return NULL;
		}
		BaseContainer * templeteParent = preObj->getTempParent();
		if (!templeteParent)
		{
			return NULL;
		}
		int tempIdx = preObj->getTempIndex();
		QList<BaseContainer *> * children = templeteParent->getChildren();
		if (tempIdx > 0)
		{
			preObj = children->at(tempIdx - 1);
			if (preObj->isView())
			{
				return (ViewObj *)preObj;
			}
			else
			{
				QList<BaseContainer *> * children1 = preObj->getChildren();
				if (children1)
				{
					if (children1->size() == 0)
					{
						continue;

					}
					else
					{
						return preObj->getLastChildView();
					}
				}
				else
				{
					continue;
				}


			}
		}
		else
		{
			if (isInExcel&&templeteParent->isObj(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER))
			{
				return NULL;
			}
			else
			{
				preObj = templeteParent;
			}

		}
	};

	/*int tempParentId = templeteParent->getTempleteIndexFromParent();
	if (tempParentId > 0)
	{
	templeteParent = templeteParent->getTempleteParent();
	preObj = templeteParent->getChild(tempParentId - 1);
	if (preObj->isViewObj())
	{
	return (ViewObj *)preObj;
	}
	else
	return preObj->getLastChildView();
	}
	else
	{
	templeteParent = templeteParent->getTempleteParent();
	tempParentId = templeteParent->getTempleteIndexFromParent();
	if (tempParentId > 0)
	{
	templeteParent = templeteParent->getTempleteParent();
	preObj = templeteParent->getChild(tempParentId - 1);
	if (preObj->isViewObj())
	{
	return (ViewObj *)preObj;
	}
	else
	return preObj->getLastChildView();
	}
	}
	return NULL;*/
}
void BaseContainer::refreshViewTxtParamOnly(StrContentData * focusTxtData, ViewObj * obj)
{
	if (obj->isTxt())
	{
		StrContentData * txtParam = (StrContentData *)obj->getContentData();
		txtParam->refreshTxtParamBySourceParam(focusTxtData, true);
	}

}
BaseContainer * BaseContainer::getChooseObj()
{
	if (mCanChooseID < 0)
	{
		return this;
	}
	BaseContainer * dest = findTempFromParent(mCanChooseID, true);
	return dest;
}
int BaseContainer::getChooseObjId()
{
	return mCanChooseID;
}
void BaseContainer::setChooseObjId(int chooseObjId)
{
	mCanChooseID = chooseObjId;
}
//void BaseContainer::setPreFocus()
//{
//	mWriterMgr->setPreFocus();
//}
void BaseContainer::showTidy()
{
	if (mIsShowTidy)
	{
		mIsShowTidy = false;
	}
	else
	{
		mIsShowTidy = true;
	}
	QList<BaseContainer *> * children = getChildren();
	if (children)
	{
		int size = children->size();
		BaseContainer* child = NULL;
		for (int i = 0; i < size; i++)
		{
			child = children->at(i);
			child->showTidy();
		}
	}
}

bool BaseContainer::getIsShowTidy()
{
	return mIsShowTidy;
}
void BaseContainer::setWriteManager(WriterManager* writeManger, bool isCircle)
{
	mWriterMgr = writeManger;
	if (isCircle)
	{
		setChildWriteManger(writeManger);
	}
}
int BaseContainer::delChild(BaseContainer * obj, bool refreshIdx)
{
	int returnType = BaseConstants::NONE;
	mChildrenMgr->delChild(obj, refreshIdx);
	return returnType;
}
bool BaseContainer::canEditTempleteProperty()
{
	FileData * fileData = mWriterMgr->getFileData();
	return fileData->canEditTemplateProperty();
}
bool BaseContainer::canEditTemplete()
{
	if (templateParentCanWrite())
	{
		FileData * fileData = mWriterMgr->getFileData();
		return fileData->canEditTemplate();
	}
	else
	{
		return false;
	}

}
bool BaseContainer::templateParentCanWrite()
{
	BaseTemplate * tempParent = getTempParent();
	if (!tempParent->canWrite())
	{
		return false;
	}
	return true;
}
void BaseContainer::addExcelTempelateToOperate(ExcelTemplete* excelTemp)
{
	getOperateMgr()->addExcelTempelateToOperate(excelTemp);
}
bool BaseContainer::isShow(BaseContainer* container)
{
	ViewObj* view = NULL;
	if (container->isView())
	{
		view = (ViewObj*)container;
	}
	else
	{
		view = container->getFirstView();
		if (!view)
		{
			view = container->getLastView();
		}
	}
	if (view)
	{
		ViewObj* excelContainerView = view->findViewFromParent(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW, true);
		if (excelContainerView)
		{
			if (isShowOnNormalRow(excelContainerView))
			{
				if (isShowOnNormalRow(view))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (isShowOnNormalRow(view))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

}
bool BaseContainer::isShowOnNormalRow(ViewObj* view)
{
	ViewObj* normalRow = view->findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, true);
	if (normalRow)
	{
		int indexOfView = normalRow->getChildren()->indexOf(view);
		if (indexOfView >= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}