#include "ViewObj.h"




#include "WriterManager.h"
#include "WriterLogicMgr.h"

#include "ParagraphManager.h"


#include "ObjKeyManager.h"
#include "ObjMouseManager.h"
#include "ObjDrawManager.h"
#include "TxtWinDrawManager.h"
#include "BaseMouse.h"

#include "CrossParentMgr.h"
#include "ExcelCrossParentMgr.h"

#include "BaseParagraph.h"

#include "BaseViewParam.h"
#include "RelativeChildrenParam.h"

#include "BaseLayout.h"

#include "OperateMgr.h"
#include "BaseSystem.h"

#include "ConstantsID.h"
#include "BaseConstants.h"
#include "ExcelTemplete.h"
#include "StrContentData.h"
ViewObj::ViewObj(WriterManager * writerMgr, int objID) : BaseContainer(writerMgr, ConstantsID::ClassId::VIEW_OBJ, objID)
, mViewParam(NULL)
, mDrawMgr(NULL)
, mWindow(NULL)
, mWinData(NULL)
, mKeyMgr(NULL)
, mMouseMgr(NULL)
, mCrossMgr(NULL)
{
	mViewIndex = -1;
	mParagraphIndex = -1;
	mRelativeParam = NULL;
	mKeyMgr = new ObjKeyManager(this);
	mMouseMgr = new ObjMouseManager(this);

	if (!isTxtWindow())
	{
		mDrawMgr = new ObjDrawManager(this);
	}
	else
		mDrawMgr = new TxtWinDrawManager(this);
}
ViewObj::ViewObj(WriterManager * writerMgr, int classId, int objID, bool isNeedNewInputManager) : BaseContainer(writerMgr, classId, objID)
, mViewParam(NULL)
, mDrawMgr(NULL)
, mWindow(NULL)
, mWinData(NULL)
, mKeyMgr(NULL)
, mMouseMgr(NULL)
, mCrossMgr(NULL)
{
	mRelativeParam = NULL;
	if (isNeedNewInputManager)
	{
		mKeyMgr = new ObjKeyManager(this);
		mMouseMgr = new ObjMouseManager(this);
	}
	if (!isTxtWindow())
	{
		mDrawMgr = new ObjDrawManager(this);
	}
	else
		mDrawMgr = new TxtWinDrawManager(this);
}

ViewObj::ViewObj(WriterManager * writerMgr, ViewObj * source) : BaseContainer(writerMgr, source)
, mKeyMgr(NULL)
, mMouseMgr(NULL)
, mCrossMgr(NULL)
{
	mWindow = NULL;
	mWinData = NULL;
	mRelativeParam = NULL;

	mViewParam = BaseSystem::getInstance()->getViewParamCopy(this, source->mViewParam);

	if (source->mContentData)
	{
		BaseContentData * data = BaseSystem::getInstance()->getContentDataCopy(this, source->mContentData);
		initContentData(data);
	}

	mDrawMgr = BaseSystem::getInstance()->getDrawMgrCopy(this, source->mDrawMgr);
	if (source->mChildrenMgr)
	{
	}
	if (source->mLogicManager)
	{
	}
	if (source->mKeyMgr)
	{
		mKeyMgr = BaseSystem::getInstance()->getKeyMgrCopy(this, source->mKeyMgr);
	}
	if (source->mMouseMgr)
	{
		mMouseMgr = BaseSystem::getInstance()->getMouseMgrCopy(this, source->mMouseMgr);
	}
}
ViewObj::~ViewObj()
{
	mWindow = NULL;
	//if (isObj(ConstantsID::ContainerId::TXT_WIN_VIEW) || isObj(ConstantsID::ContainerId::FLAG))
	//{
	//	mWriterMgr->getLogicMgr()->removeFromRunningList(this);
	//}
}
void ViewObj::initViewParam(BaseViewParam * param)
{
	mViewParam = param;
}
void ViewObj::initDrawMgr(ObjDrawManager * drawMgr)
{
	mDrawMgr = drawMgr;
}
void ViewObj::initCrossParentMgr(CrossParentMgr * mgr)
{
	mCrossMgr = mgr;
}
ObjKeyManager * ViewObj::getKeyMgr()
{
	return mKeyMgr;
}
ObjMouseManager * ViewObj::getMouseMgr()
{
	return mMouseMgr;
}
ObjDrawManager * ViewObj::getDrawMgr()
{
	return mDrawMgr;
}
CrossParentMgr * ViewObj::getCrossParentMgr()
{
	return mCrossMgr;
}
ExcelCrossParentMgr * ViewObj::getExcelCrossParentMgr()
{
	return (ExcelCrossParentMgr *)mCrossMgr;
}
ViewObj * ViewObj::getViewParent()
{
	if (mCrossMgr)
	{
		ViewObj * activeParent = mCrossMgr->getActiveParent();
		if (activeParent)
			return activeParent;
	}
	return mViewParent;
}
ViewObj * ViewObj::getParentTxtLayer(bool circle)
{

	if (isRow())
	{
		return mViewParent;
	}
	else
	{
		ViewObj * row = findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, true);
		return mViewParent->getViewParent();
	}
}
BaseViewParam * ViewObj::getViewParam()
{
	return mViewParam;
}
void ViewObj::setViewIndex(int idx)
{
	mViewIndex = idx;
}
void ViewObj::refreshChildrenViewIndex(int idx)
{
	QList<BaseContainer *> * children = getChildren();
	int size = children->size();
	ViewObj * child = NULL;
	if (idx < 0)
	{
		idx = 0;
	}
	for (int i = idx; i < size; i++)
	{
		child = (ViewObj *)children->at(i);
		child->setViewIndex(i);
	}
}
void ViewObj::setRelativeParam(RelativeChildrenParam * source)
{
	mRelativeParam = source;
}
RelativeChildrenParam * ViewObj::getRelativeParam()
{
	return mRelativeParam;
}
BaseLayout * ViewObj::getLayout()
{
	return (BaseLayout *)mChildrenMgr;
}
ViewObj * ViewObj::getParentPage()
{
	ViewObj * page = NULL;
	page= findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	if (!page)
	{
		page = findViewFromParent(ConstantsID::ContainerId::EXCEL_PAGE, true);
	}
	return page;
}
ViewObj * ViewObj::getParentPagePanel()
{
	return findViewFromParent(ConstantsID::ContainerId::PAGE_PANEL, true);
}
ViewObj * ViewObj::getWindow()
{
	if (!mWindow)
	{
		ViewObj * root = mWriterMgr->getRootView();
		// root为空，代表正在初始化过程中
		if (!root)
		{
			return NULL;
		}
		mWindow = (ViewObj *)root->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, false);
		mWinData = (TxtWinData *)mWindow->getContentData();
	}
	return mWindow;
}
TxtWinData * ViewObj::getWinData()
{
	if (isTxtWindow())
	{
		mWindow = this;
		mWinData = (TxtWinData *)mWindow->getContentData();
		return mWinData;
	}
	if (!mWindow)
	{
		mWindow = (ViewObj *)mWriterMgr->getRootView()->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, false);
		mWinData = (TxtWinData *)mWindow->getContentData();
	}
	return mWinData;
}
int ViewObj::getViewIndex()
{
	if (!mViewParent)
	{
		return -1;
	}
	return mViewParent->getChildren()->indexOf(this);
}
int ViewObj::getParagraphIndex()
{
	return mParagraphIndex;
}
void ViewObj::setParagraphIndex(int idx)
{
	mParagraphIndex = idx;
}

ViewObj * ViewObj::getPreViewFromViewParent()
{
	int idx = getViewIndex();
	if (idx < 0)
	{
		return NULL;
	}
	QList<BaseContainer *> * children = NULL;
	if (idx > 0)
	{
		children = mViewParent->getChildren();
		return (ViewObj *)children->at(idx - 1);
	}
	BaseContainer * txtLayer = getParentTxtLayer(true);
	if (!isRow())
	{
		idx = mViewParent->getViewIndex();
	}
	if (idx > 0)
	{
		children = txtLayer->getChildren();
		BaseContainer * preParent = children->at(idx - 1);
		children = preParent->getChildren();
		int size = children->size();
		return (ViewObj *)children->at(size - 1);
	}
	// 如果页内容层
	else if (txtLayer->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
	{
		ViewObj * page = mViewParent->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
		int pageIdx = page->getViewIndex();
		ViewObj * pagePanel = page->getViewParent();
		children = pagePanel->getChildren();
		if (isExcelRow())
		{
			ExcelCrossParentMgr * crossMgr = getExcelCrossParentMgr();
			if (crossMgr && crossMgr->haveCrossParent())
			{
				txtLayer = crossMgr->getFirstCrossParent();
				page = txtLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
				pageIdx = page->getViewIndex();
				idx = txtLayer->getChildren()->indexOf(this);
				// 不在跨页首页的首行
				if (idx > 0)
				{
					return (ViewObj *)txtLayer->getChild(idx - 1);
				}
				else
				{
					if (pageIdx > 0)
					{
						page = (ViewObj *)children->at(pageIdx - 1);
						BaseContainer * txtLayer = page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
							->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
						children = txtLayer->getChildren();
						return (ViewObj *)children->at(children->size() - 1);
					}
					//else
					//{
					//	children = txtLayer->getChildren();
					//	idx = children->indexOf(this);
					//	if (idx > 0)
					//	{
					//		return (ViewObj *)children->at(idx - 1);
					//	}
					//}
				}
				return NULL;
			}
		}
		if (pageIdx > 0)
		{
			page = (ViewObj *)children->at(pageIdx - 1);
			BaseContainer * txtLayer = page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			if (isRow())
				return (ViewObj *)txtLayer->getLastChild(false);
			else
			{
				BaseContainer * lastRow = txtLayer->getLastChild(false);
				QList<BaseContainer *> * tempChildren = lastRow->getChildren();
				if (tempChildren)
					return (ViewObj *)lastRow->getLastChild(false);
			}
		}
		return NULL;
	}
	return NULL;
}
ViewObj * ViewObj::getNextViewFromViewParent()
{
	int idx = getViewIndex();
	if (idx < 0)
	{
		return NULL;
	}
	QList<BaseContainer *> * children = mViewParent->getChildren();
	int size = children->size();
	// 如果不为本行最后一个
	if (idx < size - 1)
	{
		return (ViewObj *)children->at(idx + 1);
	}
	if (!isRow())
		idx = mViewParent->getViewIndex();
	BaseContainer * txtLayer = getParentTxtLayer(true);
	children = txtLayer->getChildren();
	size = children->size();
	// 如果不为本页最后一行
	if (idx < size - 1)
	{
		BaseContainer * nextParent = children->at(idx + 1);
		children = nextParent->getChildren();
		return (ViewObj *)children->at(0);
	}
	// 如果页内容层
	else if (txtLayer->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
	{
		ViewObj * page = mViewParent->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
		int pageIdx = page->getViewIndex();
		ViewObj * pagePanel = page->getViewParent();
		children = pagePanel->getChildren();

		if (isExcelRow())
		{
			ExcelCrossParentMgr * crossMgr = getExcelCrossParentMgr();
			if (crossMgr && crossMgr->haveCrossParent())
			{
				txtLayer = crossMgr->getLastCrossParent();
				page = txtLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
				pageIdx = page->getViewIndex();
				idx = txtLayer->getChildren()->indexOf(this);
				// 如果最后跨页的最后一行
				if (idx < txtLayer->getChildren()->size() - 1)
				{
					return (ViewObj *)txtLayer->getChild(idx + 1);
				}
				else
				{
					// 不为最后一页
					if (pageIdx < children->size() - 1)
					{
						page = (ViewObj *)children->at(pageIdx + 1);
						BaseContainer * txtLayer = page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
							->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
						children = txtLayer->getChildren();
						idx = children->indexOf(this);
						if (idx < children->size() - 1)
							return (ViewObj *)children->at(idx + 1);
					}
					else
					{
						children = txtLayer->getChildren();
						idx = children->indexOf(this);
						if (idx < children->size() - 1)
						{
							return (ViewObj *)children->at(idx + 1);
						}
					}
				}
				return NULL;
			}
		}
		if (pageIdx < children->size() - 1)
		{
			page = (ViewObj *)children->at(pageIdx + 1);
			BaseContainer * txtLayer = page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			if (!isRow())
				return (ViewObj *)txtLayer->getChild(0)->getChild(0);
			else
				return (ViewObj *)txtLayer->getChild(0);
		}
		return NULL;
	}
	return NULL;
}

ViewObj * ViewObj::getPreViewFromParagraphParent()
{
	int idx = getParagraphIndex();
	ViewObj * dest = NULL;
	QList<BaseContainer *> * children = NULL;
	if (idx > 0)
	{
		children = mParagraphParent->getChildren();
		dest = (ViewObj *)children->at(idx - 1);
		return dest;
	}
	int paragraphIdx = mParagraphParent->getIndexFromParagraphMgr();
	if (paragraphIdx > 0)
	{
		ParagraphManager * mgr = mParagraphParent->getParagraphMgr();
		QList<BaseParagraph *> * paragraphList = mgr->getParagraphList();
		BaseParagraph * preParagraph = paragraphList->at(paragraphIdx - 1);
		children = preParagraph->getChildren();
		int size = children->size();
		dest = (ViewObj *)children->at(size - 1);
	}

	return dest;
}
ViewObj * ViewObj::getNextViewFromParagraphParent()
{
	int idx = getParagraphIndex();
	if (idx < 0)
	{
		return NULL;
	}
	ViewObj * dest = NULL;
	QList<BaseContainer *> * children = mParagraphParent->getChildren();
	int size = children->size();
	if (idx < size - 1)
	{
		dest = (ViewObj *)children->at(idx + 1);
		return dest;
	}
	int paragraphIdx = mParagraphParent->getIndexFromParagraphMgr();
	ParagraphManager * mgr = mParagraphParent->getParagraphMgr();
	QList<BaseParagraph *> * paragraphList = mgr->getParagraphList();
	size = paragraphList->size();
	if (paragraphIdx < size - 1)
	{
		BaseParagraph * tParagraph = paragraphList->at(paragraphIdx + 1);
		children = tParagraph->getChildren();
		dest = (ViewObj *)children->at(0);
	}
	return dest;
}
bool ViewObj::isInDest(int objID)
{
	ViewObj * parent = mViewParent->getViewParent();
	while (parent)
	{
		if (parent->isObj(objID))
		{
			return true;
		}
		parent = parent->getViewParent();
	}
	return false;
}
void ViewObj::setFocus(bool isRight, bool isFreshStrData, char pressType)
{
	if (isRight)
	{
		mWriterMgr->getOperateMgr()->setPress(pressType, this, BaseConstants::Direction::RIGHT, isFreshStrData);
	}
	else
	{
		mWriterMgr->getOperateMgr()->setPress(pressType, this, BaseConstants::Direction::LEFT, isFreshStrData);
	}
}
void ViewObj::setFocus1(char direction, bool isFreshStrData, char pressType)
{
	mWriterMgr->getOperateMgr()->setPress(pressType, this, direction, isFreshStrData);
}
void ViewObj::setFocusByX(int pressX, char pressType)
{
	int halfWidth = mViewParam->getTotalW() / 2;
	if (pressX > halfWidth)
	{
		setFocus(true, pressType);
	}
	else
	{
		setFocus(false, pressType);
	}
}
void ViewObj::setViewType(char viewType)
{
	/*if (isObj(ConstantsID::ContainerId::FLAG))
	{
	int a = 0;
	}*/
	mViewParam->setViewType(viewType);
}
void ViewObj::releaseFromWindow()
{
	if (isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
	{
		ExcelTemplete* excelTemplate = (ExcelTemplete*)getTempParent();
		excelTemplate->setIsPressOnExcel(false);
	}
	else
		mMouseMgr->getMouse()->releaseFocusFromWindow();
}
bool ViewObj::setDrawTxtParam(StrContentData* focusData)
{
	if (isTxt())
	{
		StrContentData * data = NULL;
		data = (StrContentData *)getContentData();
		char preFlowType;
		bool preBool;
		QString preFamily;
		int PrePointSize;
		bool isNeedFresh = false;
		switch (focusData->mOprateType)
		{
		case SetTxtType::FlowType:
			// 浮动类型：上下标
			 preFlowType = data->getFlowType();
			break;
		case SetTxtType::FontItalic:
			preBool = data->getItalic();
			break;
		case SetTxtType::Family:
			preFamily = data->getFamily();
			break;
		case SetTxtType::FontSize:
			PrePointSize = data->getPointSize();
			//mFont->setPointSize(data->getPointSize()); //setPointSize(data->mfontSize);
			break;
		case SetTxtType::Bold:
			preBool = data->getBold();
			break;
		}
		isNeedFresh = data->refreshTxtParamOnSetType(focusData);
		if (isNeedFresh)
		{
			data->refreshTxtWH();
		}
		if (mWriterMgr)
		{
			if (mWriterMgr->getOperateMgr()->canExcelCellContainObj(this, this))
			{
				return true;
			}
			else
			{
				switch (focusData->mOprateType)
				{
				case SetTxtType::FlowType:
					// 浮动类型：上下标
					 data->setFlowType(preFlowType);
					break;
				case SetTxtType::FontItalic:
					 data->setItalic(preBool);
					break;
				case SetTxtType::Family:
					 data->setFamily(preFamily);
					break;
				case SetTxtType::FontSize:
					data->setPointSize(PrePointSize);
					//mFont->setPointSize(data->getPointSize()); //setPointSize(data->mfontSize);
					break;
				case SetTxtType::Bold:
				    data->setBold(preBool);
					break;
				}
				return false;
			}
		}
		return true;
	}
	return true;
}