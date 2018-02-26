#include "ParagraphManager.h"




#include <QDebug>


#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseParagraph.h"
#include "BaseTemplate.h"
#include "ExcelCrossParentMgr.h"

#include "BaseViewParam.h"
#include "LayoutViewParam.h"

#include "SystemParams.h"
#include "ParagraphManager.h"
#include "FlagParam.h"
#include "TxtWinData.h"
#include "ParagraphData.h"

#include "PageContentTxtLayerLayout.h"

#include "RowLayout.h"

#include "WriterManager.h"
#include "OperateMgr.h"

#include "BaseSystem.h"
#include "BaseTools.h"

#include "TempleteFactory.h"
#include "ViewFactory.h"

#include "ConstantsID.h"
#include "UIConstants.h"
#include "BaseConstants.h"
#include "GlobalTemplate.h"
#include "ExcelCellData.h"
ParagraphManager::ParagraphManager(BaseTemplate * owner) : BaseElement(ConstantsID::ClassId::PARAGRAPH_MGR)
{
	mIsExcelParagraphManger = false;
	mWriterMgr = owner->getWriterMgr();
	mTempParent = owner;
	mParagraphList = new QList<BaseParagraph *>();
}
ParagraphManager::ParagraphManager(BaseTemplate * owner, ParagraphManager* otherParagraphMgr) :BaseElement(otherParagraphMgr)
, mIsExcelParagraphManger(otherParagraphMgr->mIsExcelParagraphManger)
, mWriterMgr(owner->getWriterMgr())
{
	mTempParent = owner;
	mParagraphList = new QList<BaseParagraph *>();
	int paragraphSize = otherParagraphMgr->mParagraphList->size();
	BaseParagraph * temParagraph;
	BaseParagraph * temOtherParagraph;
	for (int i = 0; i < paragraphSize; i++)
	{
		temOtherParagraph = otherParagraphMgr->mParagraphList->at(i);

		temParagraph = TempleteFactory::createParagraph(mWriterMgr, this, (ParagraphData*)temOtherParagraph->getContentData());
		temParagraph->setIndexFromParagraphMgr(temOtherParagraph->getIndexFromParagraphMgr());
		mParagraphList->push_back(temParagraph);
	}
}
ParagraphManager::~ParagraphManager()
{
	if (mParagraphList)
	{
		deleteVector(mParagraphList);
		mParagraphList = NULL;
	}
}
BaseTemplate * ParagraphManager::getTempParent()
{
	return mTempParent;
}
QList<BaseParagraph *> * ParagraphManager::getParagraphList()
{
	return mParagraphList;
}
BaseParagraph * ParagraphManager::getParagrapAt(int index)
{
	if (mParagraphList)
	{
		int size = mParagraphList->size();
		if (index < size)
		{
			return mParagraphList->at(index);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}
ViewObj * ParagraphManager::getFirstLayer()
{
	if (mIsExcelParagraphManger)
	{
		int objID = mTempParent->getObjID();
		if (objID == ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER)
		{
			ExcelCellData * cellData = mTempParent->getExcelCellData();
			ViewObj * cellPage = cellData->getViewOwner();
			ViewObj * cellPageLayer = (ViewObj *)cellPage->findChildrenByID(ConstantsID::ContainerId::EXCEL_TXT_LAYER, false);
			return cellPageLayer;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		int objID = mTempParent->getObjID();
		ViewObj * txtLayer = NULL;
		WriterManager * mgr = mTempParent->getWriterMgr();
		ViewObj * window = mgr->getRootView()->getWindow();
		TxtWinData * winData = window->getWinData();
		switch (objID)
		{
			// 注意，paragraphMgr是通篇拉通，是跨模板容器的的，所以放在页眉、页脚、页内容的根容器里
		case ConstantsID::ContainerId::TITLE_TEMP:
			txtLayer = (ViewObj *)winData->getTitleView()->findChildrenByID(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER, false);
			break;
		case ConstantsID::ContainerId::BOTTOM_TEMP:
			txtLayer = (ViewObj *)winData->getBottomView()->findChildrenByID(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER, false);
			break;
		case ConstantsID::ContainerId::CONTENT_TEMP:
			txtLayer = (ViewObj *)window->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false)
				->findChildrenByID(ConstantsID::ContainerId::NORMAL_PAGE, false)->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			break;
		}
		return txtLayer;
	}

}
void ParagraphManager::setExcelParagrphManager()
{
	mIsExcelParagraphManger = true;
}
//void ParagraphManager::seperateList(QList<QList<BaseContainer *> *> * seperateAllViewChildren, QList<BaseContainer *> * source)
//{
//	int size = source->size();
//	ViewObj * obj = NULL;
//	int seperateIdx = 0;
//	QList<BaseContainer *> * curList = NULL;
//	//if (seperateAllViewChildren->size() > 0)
//	//{
//	//	curList = seperateAllViewChildren->at(seperateIdx);
//	//}
//	//else
//	seperateAllViewChildren->clear();
//	curList = new QList<BaseContainer *>();
//	seperateAllViewChildren->push_back(curList);
//	for (int i = 0; i < size; i++)
//	{
//		obj = (ViewObj *)source->at(i);
//		curList->push_back(obj);
//		if (obj->isEnter() && i < size - 1)
//		{
//			QList<BaseContainer *> * list = new QList<BaseContainer *>();
//			seperateAllViewChildren->push_back(list);
//			seperateIdx++;
//		}
//	}
//}
void ParagraphManager::refreshChildrenIndex(int start)
{
	int size = mParagraphList->size();
	BaseParagraph * paragraph = NULL;
	for (int i = start; i < size; i++)
	{
		paragraph = mParagraphList->at(i);
		paragraph->setIndexFromParagraphMgr(i);
	}
}
void ParagraphManager::addParagraph(BaseParagraph * paragraph, int idx, bool refreshIdx)
{
	int size = mParagraphList->size();
	if (idx == -1)
	{
		idx = size;
	}
	int preIdx = -1;
	if (idx == 0)
	{
		mParagraphList->push_front(paragraph);
	}
	else if (idx == size)
	{
		mParagraphList->push_back(paragraph);
	}
	else
	{
		mParagraphList->insert(idx, paragraph);
	}

	paragraph->initParagraphMgr(this);
	if (refreshIdx)
		refreshChildrenIndex(idx);
}
void ParagraphManager::delParagraph(BaseParagraph * paragraph, bool refreshIdx)
{
	int idx = mParagraphList->indexOf(paragraph);
	mParagraphList->removeOne(paragraph);
	if (refreshIdx)
	{
		refreshChildrenIndex(idx);
	}
}
void ParagraphManager::addNode(BaseContainer * child, bool refreshIdx, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow)
{
	QList<QList<BaseContainer *> *> * seperateAllViewChildren = mWriterMgr->getTempParagraphSeperateViewList();
	QList<BaseContainer *> * tempAllViewChildren = mWriterMgr->getClearTempParagraphViewList();
	BaseTools::exchangeObjToViewList(child, tempAllViewChildren);
	BaseTools::seperateListByEnter(seperateAllViewChildren, tempAllViewChildren);
	addExchangeNode(seperateAllViewChildren, tempAllViewChildren, refreshIdx, useParagraphDataList, useIndex, needReplaceRow);
}
void ParagraphManager::addChild(BaseContainer * child, bool refreshIdx, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow)
{
	QList<QList<BaseContainer *> *> * seperateAllViewChildren = mWriterMgr->getTempParagraphSeperateViewList();
	QList<BaseContainer *> * tempAllViewChildren = mWriterMgr->getClearTempParagraphViewList();
	BaseTools::exchangeObjToViewList(child, tempAllViewChildren);
	BaseTools::seperateListByEnter(seperateAllViewChildren, tempAllViewChildren);
	if (child->isEnter() && tempAllViewChildren->at(0) != child)
	{
		int a = 0;
	}
	addExchangeChildren(seperateAllViewChildren, tempAllViewChildren, refreshIdx, useParagraphDataList, useIndex, needReplaceRow, true);
}
void ParagraphManager::addChildren(QList<BaseContainer *> * children, bool refreshIdx
	, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow)
{
	addChildren(children, 0, children->size(), refreshIdx, useParagraphDataList, useIndex, needReplaceRow);
}
void ParagraphManager::addChildren(QList<BaseContainer *> * children, int start, int num, bool refreshIdx
	, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow)
{
	QList<QList<BaseContainer *> *> * seperateAllViewChildren = mWriterMgr->getTempParagraphSeperateViewList();
	QList<BaseContainer *> * tempAllViewChildren = mWriterMgr->getClearTempParagraphViewList();
	BaseTools::exchangeListToViewList(children, start, num, tempAllViewChildren);
	BaseTools::seperateListByEnter(seperateAllViewChildren, tempAllViewChildren);
	addExchangeChildren(seperateAllViewChildren, tempAllViewChildren, refreshIdx, useParagraphDataList, useIndex, needReplaceRow, true);
}
void ParagraphManager::addChildren(QList<BaseContainer *> * children, int start, int num, bool refreshIdx
	, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow, bool needAddToParagraph)
{
	QList<QList<BaseContainer *> *> * seperateAllViewChildren = mWriterMgr->getTempParagraphSeperateViewList();
	QList<BaseContainer *> * tempAllViewChildren = mWriterMgr->getClearTempParagraphViewList();
	BaseTools::exchangeListToViewList(children, start, num, tempAllViewChildren);
	BaseTools::seperateListByEnter(seperateAllViewChildren, tempAllViewChildren);
	addExchangeChildren(seperateAllViewChildren, tempAllViewChildren, refreshIdx, useParagraphDataList, useIndex, needReplaceRow, needAddToParagraph);
}
//void ParagraphManager::resetParagraphWhenNotSameAsEnter(int startParagraphIdx, int seperSize)
//{
//}
void ParagraphManager::createParagraphWhenNotCorrect(int startParagraphIdx)
{
	BaseParagraph * tLastParagraph = mParagraphList->at(startParagraphIdx);
	BaseParagraph * tP = new BaseParagraph(mWriterMgr, this, tLastParagraph);
	ParagraphData * data = new ParagraphData(tP, tLastParagraph->getParagraphData());
	tP->initContentData(data);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(tP);
	tP->setParent(getTempParent());
	mParagraphList->push_back(tP);
	refreshChildrenIndex(mParagraphList->size() - 1);
}
void ParagraphManager::addRecordChildren(QList<BaseContainer *> * children, ViewObj * lastView)
{
	QList<QList<BaseContainer *> *> * seperateAllViewChildren = mWriterMgr->getTempParagraphSeperateViewList();
	QList<BaseContainer *> * tempAllViewChildren = mWriterMgr->getClearTempParagraphViewList();
	BaseTools::exchangeListToViewList(children, 0, children->size(), tempAllViewChildren);
	BaseTools::seperateListByEnter(seperateAllViewChildren, tempAllViewChildren);
	addExchangeRecordChildren(seperateAllViewChildren, tempAllViewChildren, lastView);
}
void ParagraphManager::addExchangeRecordChildren(QList<QList<BaseContainer *> *> * seperateAllViewChildren
	, QList<BaseContainer *> * tempAllViewChildren, ViewObj * lastView)
{

	ViewObj * row = NULL;
	RowLayout * rowLayout = NULL;
	ParagraphData * tempData = NULL;
	OperateMgr * operateMgr = mWriterMgr->getOperateMgr();
	BaseContainer * pagePanel = operateMgr->getWindow()->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
	QList<BaseContainer *> * pageList = pagePanel->getChildren();
	BaseContainer * page = pageList->at(pageList->size() - 1);
	ViewObj * txtLayer = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
		->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
	int seperSize = seperateAllViewChildren->size();

	QList<BaseContainer *> * list = NULL;
	BaseParagraph * paragraph = NULL;
	int startParagraphIdx = 0;
	BaseParagraph * preParagraph = lastView->getParagraphParent();
	startParagraphIdx = mParagraphList->indexOf(preParagraph);
	if (startParagraphIdx < 0)
	{
		startParagraphIdx = 0;
	}
	else
		startParagraphIdx++;
	//if (!paragraphList)
	//{
	//	paragraphList = mParagraphList;
	//	startParagraphIdx = lastView->getParagraphParent()->getIndexFromParagraphMgr() + 1;
	//}


	int totalParagraphNum = mParagraphList->size();
	int lesParagraphNum = (startParagraphIdx + seperSize) - totalParagraphNum;
	while (lesParagraphNum > 0)
	{
		createParagraphWhenNotCorrect(startParagraphIdx - 1);
		lesParagraphNum--;
	}
	//resetParagraphWhenNotSameAsEnter(startParagraphIdx, seperSize);


	// 添加进paragraph
	for (int i = 0; i < seperSize; i++)
	{
		list = seperateAllViewChildren->at(i);
		paragraph = mParagraphList->at(startParagraphIdx + i);
		//paragraph = paragraphList->at(startParagraphIdx + i);
		//if (paragraphList != mParagraphList)
		//{
		//	addParagraph(paragraph, -1, true);
		//}
		BaseChildrenManager * paragraphChildrenMgr = paragraph->getChildrenMgr();
		paragraphChildrenMgr->addChildren(list, 0, -1, 0, true);
	}
	paragraph = mParagraphList->at(startParagraphIdx);
	//paragraph = mParagraphList->at(0);
	row = ViewFactory::createRow(mWriterMgr, paragraph);
	rowLayout = (RowLayout *)row->getLayout();

	PageContentTxtLayerLayout * layout = (PageContentTxtLayerLayout *)txtLayer->getChildrenMgr();
	layout->addChild(row, -1, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

	rowLayout->addChildren(tempAllViewChildren, 0, -1, 0, true, BaseConstants::ReplaceType::REPLACE_NOW);
	rowLayout->judgeOverFlowCircleUntilObj(tempAllViewChildren->at(tempAllViewChildren->size() - 1));
}
void ParagraphManager::addExchangeChildren(QList<QList<BaseContainer *> *> * seperateAllViewChildren
	, QList<BaseContainer *> * tempAllViewChildren, bool refreshIdx
	, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow, bool needAddToParagraph)
{
	ViewObj * row = NULL;
	RowLayout * rowLayout = NULL;
	ViewObj * txtLayer = NULL;
	ParagraphData * tempData = NULL;
	OperateMgr * operateMgr = mWriterMgr->getOperateMgr();
	ParagraphData * paragraphData = NULL;
	if (mIsExcelParagraphManger)
	{
		paragraphData = BaseSystem::getInstance()->getDefaultParagraphData();
	}
	else
	{
		paragraphData = operateMgr->getFocusParagraphData();
	}
	FlagParam * focusParam = operateMgr->getPressEnd();
	BaseParagraph * paragraph = NULL;
	BaseChildrenManager * paragraphChildrenMgr = NULL;
	QList<BaseContainer *> * list = NULL;
	ViewObj * focus = focusParam->getFocusObj();
	int seperSize = seperateAllViewChildren->size();

	// 如果当前有焦点
	if (focus)
	{

		char dir = focusParam->getDirection();

		int idxFromTemp = focus->getTempIndex();
		int idxFromView = focus->getViewIndex();
		int idxFromParagraph = focus->getParagraphIndex();
		paragraph = focus->getParagraphParent();
		int paragraphIdx = mParagraphList->indexOf(paragraph);
		QList<BaseContainer *> * preChildren = paragraph->getChildren();
		int preSize = preChildren->size();
		BaseParagraph * nextParagraph = NULL;
		if (dir == BaseConstants::Direction::RIGHT)
		{
			idxFromTemp++;
			idxFromParagraph++;
			idxFromView++;
		}
		paragraphChildrenMgr = paragraph->getChildrenMgr();
		if (needAddToParagraph)
		{
			// 添加进paragraph
			for (int i = 0; i < seperSize; i++)
			{
				list = seperateAllViewChildren->at(i);
				if (i == 0)
				{
					if (seperSize > 1)
					{
						paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, idxFromParagraph, true);
					}
					// 如果仅添加一段
					else
					{
						// 添加项里有回车
						if (list->at(list->size() - 1)->isEnter())
						{
							paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, idxFromParagraph, true);
							//if (!nextParagraph)
							if (useParagraphDataList != NULL)
							{
								tempData = BaseSystem::getInstance()->useBufferParagraphData(useParagraphDataList, useIndex);
								if (!tempData)
								{
									tempData = paragraphData;
								}
							}
							else
								tempData = paragraphData;
							nextParagraph = TempleteFactory::createParagraph(mWriterMgr, this, tempData);
							paragraphChildrenMgr = nextParagraph->getChildrenMgr();
							paragraphChildrenMgr->addChildren(preChildren, idxFromParagraph + list->size(), -1, 0, true);
							//if (!haveNext)
							addParagraph(nextParagraph, paragraphIdx + 1, refreshIdx);
						}
						// 如果没回车
						else
						{
							paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, idxFromParagraph, true);
						}
					}
				}
				// 中间段
				else if (i < seperSize - 1)
				{
					tempData = BaseSystem::getInstance()->useBufferParagraphData(useParagraphDataList, useIndex);
					if (!tempData)
					{
						tempData = operateMgr->getFocusParagraphData();
					}
					paragraph = TempleteFactory::createParagraph(mWriterMgr, this, tempData);
					paragraphChildrenMgr = paragraph->getChildrenMgr();
					paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, 0, true);
					addParagraph(paragraph, paragraphIdx + i, refreshIdx);
				}
				// 结尾段
				else
				{
					// 如果最后一个为回车
					if (list->at(list->size() - 1)->isEnter())
					{
						if (useParagraphDataList)
							tempData = BaseSystem::getInstance()->useBufferParagraphData(useParagraphDataList, useIndex);
						if (!tempData)
						{
							tempData = operateMgr->getFocusParagraphData();
						}
						paragraph = TempleteFactory::createParagraph(mWriterMgr, this, tempData);
						paragraphChildrenMgr = paragraph->getChildrenMgr();
						paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, 0, true);

						addParagraph(paragraph, paragraphIdx + i, refreshIdx);


						// 添加首段剪切剩余内容
						if (useParagraphDataList)
							tempData = BaseSystem::getInstance()->useBufferParagraphData(useParagraphDataList, useIndex);
						paragraph = TempleteFactory::createParagraph(mWriterMgr, this, tempData);
						paragraphChildrenMgr = paragraph->getChildrenMgr();
						paragraphChildrenMgr->addChildren(preChildren, idxFromParagraph + preChildren->size() - preSize, -1, 0, true);
						addParagraph(paragraph, paragraphIdx + i + 1, refreshIdx);
					}
					else
					{
						// 添加尾段内容
						if (useParagraphDataList)
							tempData = BaseSystem::getInstance()->useBufferParagraphData(useParagraphDataList, useIndex);
						paragraph = TempleteFactory::createParagraph(mWriterMgr, this, tempData);
						paragraphChildrenMgr = paragraph->getChildrenMgr();
						paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, 0, true);
						// 添加首段剪切剩余内容
						paragraphChildrenMgr->addChildren(preChildren, idxFromParagraph + preChildren->size() - preSize, -1, -1, true);
						addParagraph(paragraph, paragraphIdx + i, refreshIdx);
					}
				}
			}
		}
		if (!needReplaceRow)
		{
			return;
		}
		// 添加到显示部分的行中
		row = focus->getViewParent();
		rowLayout = (RowLayout *)row->getLayout();
		if (tempAllViewChildren->at(0)->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
		{
			if (row->getViewParent()->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER))
			{
				int a = 0;
			}
		}
		rowLayout->judgeAddChildren(tempAllViewChildren, 0, -1, idxFromView, BaseConstants::ReplaceType::REPLACE_NOW);
		ViewObj * layer = row->getViewParent();
		BaseTools::judgeTitleOrBottomOutArea(layer);
	}
	// 初始化时没有焦点
	else
	{
		bool allReadyLoad = false;
		if (mParagraphList->size() > 0)
			allReadyLoad = true;

		for (int i = 0; i < seperSize; i++)
		{
			list = seperateAllViewChildren->at(i);
			if (i == 48)
			{
				int a = 0;
			}
			// 如果为新建，无存档
			if (!allReadyLoad)
			{
				paragraph = TempleteFactory::createParagraph(mWriterMgr, this, paragraphData);
				if (mTempParent->isObj(ConstantsID::ContainerId::TITLE_TEMP)
					|| mTempParent->isObj(ConstantsID::ContainerId::BOTTOM_TEMP))
				{
					ParagraphData * tempData = (ParagraphData *)paragraph->getContentData();
					tempData->setHorizonAlignType(BaseConstants::Direction::MIDDLE, false);
				}
			}
			else
			{
				if (i >= mParagraphList->size())
				{
					createParagraphWhenNotCorrect(i - 1);
				}
				paragraph = mParagraphList->at(i);
			}

			paragraphChildrenMgr = paragraph->getChildrenMgr();
			if (needAddToParagraph)
			{
				paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, 0, true);
				if (!allReadyLoad)
				{
					if (i < seperSize - 1)
					{
						addParagraph(paragraph, i, false);
					}
					else
						addParagraph(paragraph, i, refreshIdx);
				}
			}


			if (!needReplaceRow)
			{
				return;
			}
			row = ViewFactory::createRow(mWriterMgr, paragraph);
			rowLayout = (RowLayout *)row->getLayout();

			txtLayer = getFirstLayer();
			PageContentTxtLayerLayout * layout = (PageContentTxtLayerLayout *)txtLayer->getChildrenMgr();
			layout->addChild(row, -1, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

			if (row->getViewParent()->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
			{
				int a = 0;
			}
			rowLayout->judgeAddChildren(list, 0, -1, 0, BaseConstants::ReplaceType::REPLACE_NOW);
			//rowLayout->addChildren(list, 0, -1, 0, true, BaseConstants::ReplaceType::REPLACE_NOW);
			//rowLayout->judgeOverFlowCircleUntilObj(list->at(list->size() - 1));
		}
	}
}
void ParagraphManager::addExchangeNode(QList<QList<BaseContainer *> *> * seperateAllViewChildren
	, QList<BaseContainer *> * tempAllViewChildren, bool refreshIdx
	, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow)
{
	ViewObj * row = NULL;
	RowLayout * rowLayout = NULL;
	ViewObj * txtLayer = NULL;
	ParagraphData * tempData = NULL;
	OperateMgr * operateMgr = mWriterMgr->getOperateMgr();
	ParagraphData * paragraphData = NULL;
	if (mIsExcelParagraphManger)
	{
		paragraphData = BaseSystem::getInstance()->getDefaultParagraphData();
	}
	else
	{
		paragraphData = operateMgr->getFocusParagraphData();
	}
	FlagParam * focusParam = operateMgr->getPressEnd();
	BaseParagraph * paragraph = NULL;
	BaseChildrenManager * paragraphChildrenMgr = NULL;
	QList<BaseContainer *> * list = NULL;
	ViewObj * focus = focusParam->getFocusObj();
	int seperSize = seperateAllViewChildren->size();

	// 如果当前有焦点
	if (focus)
	{

		char dir = focusParam->getDirection();

		int idxFromTemp = focus->getTempIndex();
		int idxFromView = focus->getViewIndex();
		int idxFromParagraph = focus->getParagraphIndex();
		paragraph = focus->getParagraphParent();
		int paragraphIdx = mParagraphList->indexOf(paragraph);
		QList<BaseContainer *> * preChildren = paragraph->getChildren();
		int preSize = preChildren->size();
		BaseParagraph * nextParagraph = NULL;
		if (dir == BaseConstants::Direction::RIGHT)
		{
			idxFromTemp++;
			idxFromParagraph++;
			idxFromView++;
		}
		paragraph = TempleteFactory::createParagraph(mWriterMgr, this, paragraphData);
		paragraphChildrenMgr = paragraph->getChildrenMgr();
		list = seperateAllViewChildren->at(0);
		paragraphChildrenMgr->addChildren(list, 0, -1, 0, true);
		addParagraph(paragraph, paragraphIdx + 1, refreshIdx);
		//paragraphIdx =
		////bool haveNext = false;
		////if (nextParagraph)
		//{
		//	//	haveNext = true;
		//}
		//// 添加进paragraph
		//for (int i = 1; i < seperSize; i++)
		//{
		//	list = seperateAllViewChildren->at(i);
		//	if (i == 0)
		//	{
		//		if (seperSize > 1)
		//		{
		//			paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, idxFromParagraph, true);
		//			// 先添加进去
		//			//paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, idxFromParagraph, true);
		//			//// 将原来光标后面内容剪切到新段
		//			////if (!nextParagraph)
		//			//nextParagraph = TempleteFactory::createParagraph(mWriterMgr, this, paragraphData);
		//			//paragraphChildrenMgr = nextParagraph->getChildrenMgr();
		//			//paragraphChildrenMgr->addChildren(preChildren, idxFromParagraph, -1, 0, true);
		//			////if (!haveNext)
		//			//	addParagraph(nextParagraph, paragraphIdx + 1, refreshIdx);
		//		}
		//		// 如果仅添加一段
		//		else
		//		{
		//			// 添加项里有回车
		//			if (list->at(list->size() - 1)->isEnter())
		//			{
		//				paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, idxFromParagraph, true);
		//				//if (!nextParagraph)
		//				nextParagraph = TempleteFactory::createParagraph(mWriterMgr, this, paragraphData);
		//				paragraphChildrenMgr = nextParagraph->getChildrenMgr();
		//				paragraphChildrenMgr->addChildren(preChildren, idxFromParagraph + list->size(), -1, 0, true);
		//				//if (!haveNext)
		//				addParagraph(nextParagraph, paragraphIdx + 1, refreshIdx);
		//			}
		//			// 如果没回车
		//			else
		//			{
		//				paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, idxFromParagraph, true);
		//			}
		//		}
		//	}
		//	// 中间段
		//	else if (i < seperSize - 1)
		//	{
		//		tempData = BaseSystem::getInstance()->useBufferParagraphData(useParagraphDataList, useIndex);
		//		if (!tempData)
		//		{
		//			tempData = operateMgr->getFocusParagraphData();
		//		}
		//		paragraph = TempleteFactory::createParagraph(mWriterMgr, this, tempData);
		//		paragraphChildrenMgr = paragraph->getChildrenMgr();
		//		paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, 0, true);
		//		addParagraph(paragraph, paragraphIdx + i, refreshIdx);
		//	}
		//	// 结尾段
		//	else
		//	{
		//		// 如果最后一个为回车
		//		if (list->at(list->size() - 1)->isEnter())
		//		{
		//			if (useParagraphDataList)
		//				tempData = BaseSystem::getInstance()->useBufferParagraphData(useParagraphDataList, useIndex);
		//			if (!tempData)
		//			{
		//				tempData = operateMgr->getFocusParagraphData();
		//			}
		//			paragraph = TempleteFactory::createParagraph(mWriterMgr, this, tempData);
		//			paragraphChildrenMgr = paragraph->getChildrenMgr();
		//			paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, 0, true);

		//			addParagraph(paragraph, paragraphIdx + i, refreshIdx);


		//			// 添加首段剪切剩余内容
		//			paragraph = TempleteFactory::createParagraph(mWriterMgr, this, paragraphData);
		//			paragraphChildrenMgr = paragraph->getChildrenMgr();
		//			paragraphChildrenMgr->addChildren(preChildren, idxFromParagraph + preChildren->size() - preSize, -1, 0, true);
		//			addParagraph(paragraph, paragraphIdx + i + 1, refreshIdx);
		//		}
		//		else
		//		{
		//			// 添加尾段内容
		//			paragraph = TempleteFactory::createParagraph(mWriterMgr, this, paragraphData);
		//			paragraphChildrenMgr = paragraph->getChildrenMgr();
		//			paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, 0, true);
		//			// 添加首段剪切剩余内容
		//			paragraphChildrenMgr->addChildren(preChildren, idxFromParagraph + preChildren->size() - preSize, -1, -1, true);
		//			addParagraph(paragraph, paragraphIdx + i, refreshIdx);
		//		}
		//	}
		//}
		if (!needReplaceRow)
		{
			return;
		}
		// 添加到显示部分的行中
		row = focus->getViewParent();
		txtLayer = row->getViewParent();
		int preRowIdx = row->getViewIndex();
		row = ViewFactory::createRow(mWriterMgr, paragraph);
		rowLayout = (RowLayout *)row->getLayout();
		PageContentTxtLayerLayout * layout = (PageContentTxtLayerLayout *)txtLayer->getChildrenMgr();
		layout->addChild(row, preRowIdx + 1, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

		rowLayout->judgeAddChildren(list, 0, -1, 0, BaseConstants::ReplaceType::REPLACE_NOW);

		//rowLayout->judgeAddChildren(tempAllViewChildren, 0, -1, idxFromView, BaseConstants::ReplaceType::REPLACE_NOW);
		ViewObj * layer = row->getViewParent();
		BaseTools::judgeTitleOrBottomOutArea(layer);
	}
	// 初始化时没有焦点
	else
	{
		bool allReadyLoad = false;
		if (mParagraphList->size() > 0)
			allReadyLoad = true;
		for (int i = 0; i < seperSize; i++)
		{
			list = seperateAllViewChildren->at(i);
			// 如果为新建，无存档
			if (!allReadyLoad)
			{
				paragraph = TempleteFactory::createParagraph(mWriterMgr, this, paragraphData);
				if (mTempParent->isObj(ConstantsID::ContainerId::TITLE_TEMP)
					|| mTempParent->isObj(ConstantsID::ContainerId::BOTTOM_TEMP))
				{
					ParagraphData * tempData = (ParagraphData *)paragraph->getContentData();
					tempData->setHorizonAlignType(BaseConstants::Direction::MIDDLE, false);
				}
			}
			else
				paragraph = mParagraphList->at(i);

			paragraphChildrenMgr = paragraph->getChildrenMgr();
			paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, 0, true);

			if (!allReadyLoad)
			{
				if (i < seperSize - 1)
				{
					addParagraph(paragraph, i, false);
				}
				else
					addParagraph(paragraph, i, refreshIdx);
			}

			if (!needReplaceRow)
			{
				return;
			}
			row = ViewFactory::createRow(mWriterMgr, paragraph);
			rowLayout = (RowLayout *)row->getLayout();

			txtLayer = getFirstLayer();
			PageContentTxtLayerLayout * layout = (PageContentTxtLayerLayout *)txtLayer->getChildrenMgr();
			layout->addChild(row, -1, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

			if (row->getViewParent()->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
			{
				int a = 0;
			}
			rowLayout->judgeAddChildren(list, 0, -1, 0, BaseConstants::ReplaceType::REPLACE_NOW);
			//rowLayout->addChildren(list, 0, -1, 0, true, BaseConstants::ReplaceType::REPLACE_NOW);
			//rowLayout->judgeOverFlowCircleUntilObj(list->at(list->size() - 1));
		}
	}
}


void ParagraphManager::delChild(BaseContainer * child, bool refreshIdx, bool needReplaceRow)
{
	QList<QList<BaseContainer *> *> * seperateAllViewChildren = mWriterMgr->getTempParagraphSeperateViewList();
	QList<BaseContainer *> * tempAllViewChildren = mWriterMgr->getClearTempParagraphViewList();
	BaseTools::exchangeObjToViewList(child, tempAllViewChildren);
	BaseTools::seperateListByEnter(seperateAllViewChildren, tempAllViewChildren);
	delExchangeChildren(seperateAllViewChildren, tempAllViewChildren, refreshIdx, needReplaceRow);
}
void ParagraphManager::delRecordChildren(QList<BaseContainer *> * children)
{
	QList<QList<BaseContainer *> *> * seperateAllViewChildren = mWriterMgr->getTempParagraphSeperateViewList();
	QList<BaseContainer *> * tempAllViewChildren = mWriterMgr->getClearTempParagraphViewList();
	int size = children->size();
	for (int i = 0; i < size; i++)
	{
		BaseTools::exchangeObjToViewList(children->at(i), tempAllViewChildren);
	}
	BaseTools::seperateListByEnter(seperateAllViewChildren, tempAllViewChildren);
	delExchangeRecordChildren(seperateAllViewChildren, tempAllViewChildren);
}
void ParagraphManager::delChildren(QList<BaseContainer *> * children, int start, int num, bool refreshIdx, bool needReplaceRow)
{
	QList<QList<BaseContainer *> *> * seperateAllViewChildren = mWriterMgr->getTempParagraphSeperateViewList();
	QList<BaseContainer *> * tempAllViewChildren = mWriterMgr->getClearTempParagraphViewList();
	int end = start + num;
	if (num < 0)
		end = children->size() - start;
	for (int i = start; i < end; i++)
	{
		BaseTools::exchangeObjToViewList(children->at(i), tempAllViewChildren);
	}
	if (tempAllViewChildren->size() == 0)
	{
		return;
	}
	BaseTools::seperateListByEnter(seperateAllViewChildren, tempAllViewChildren);
	delExchangeChildren(seperateAllViewChildren, tempAllViewChildren, refreshIdx, needReplaceRow);
}
void ParagraphManager::delExchangeChildren(QList<QList<BaseContainer *> *> * seperateAllViewChildren
	, QList<BaseContainer *> * tempAllViewChildren, bool refreshIdx, bool needReplaceRow)
{
	int temp = 0;
	ViewObj * first = (ViewObj *)tempAllViewChildren->at(0);
	//first->getViewIndex();
	BaseParagraph * firstParagraph = first->getParagraphParent();
	int firstIdxFromParagraph = first->getParagraphIndex();
	// 首项所属段落的段落idx
	int firstParagraphIdx = firstParagraph->getIndexFromParagraphMgr();
	int size = tempAllViewChildren->size();
	ViewObj * last = (ViewObj *)tempAllViewChildren->at(size - 1);
	//last->getViewIndex();
	BaseParagraph * lastParagraph = last->getParagraphParent();
	int lastIdxFromParagraph = last->getParagraphIndex();
	int lastParagraphIdx = lastParagraph->getIndexFromParagraphMgr();

	bool needRefreshParagraphRow = false;
	if (first->isEnter())
	{
		needRefreshParagraphRow = true;
	}
	BaseParagraph * paragraph = NULL;
	BaseChildrenManager * paragraphMgr = NULL;
	BaseParagraph * nextParagraph = NULL;
	// 如果要删除的对象列表在同一段内
	if (firstParagraphIdx == lastParagraphIdx)
	{
		paragraph = mParagraphList->at(firstParagraphIdx);
		paragraphMgr = paragraph->getChildrenMgr();
		paragraphMgr->delChildren(firstIdxFromParagraph, lastIdxFromParagraph - firstIdxFromParagraph + 1, true);
		// 如果最后一个为回车
		if (last->isEnter())
		{
			if (firstParagraphIdx < mParagraphList->size() - 1)
			{
				nextParagraph = mParagraphList->at(firstParagraphIdx + 1);
				paragraphMgr->addChildren(nextParagraph->getChildren(), 0, -1, -1, true);
				delParagraph(nextParagraph, true);
			}
			else
			{
				delParagraph(paragraph, true);
			}
		}
		else
		{

		}
	}
	else
	{
		// 删除首段内容
		paragraph = mParagraphList->at(firstParagraphIdx);
		paragraphMgr = paragraph->getChildrenMgr();
		paragraphMgr->delChildren(firstIdxFromParagraph, -1, true);

		// 删除中间段
		int tempStart = firstParagraphIdx + 1;
		for (int i = tempStart; i < lastParagraphIdx; i++)
		{
			paragraph = mParagraphList->at(tempStart);
			delParagraph(paragraph, false);
		}
		// 刷新索引
		refreshChildrenIndex(tempStart);
		// 重置尾段索引
		lastParagraphIdx = lastParagraph->getIndexFromParagraphMgr();
		// 删除尾段
		// 如果选中尾项为尾段回车,则将尾段删除,将下一段合并至首段
		if (last->isEnter())
		{
			if (lastParagraphIdx < mParagraphList->size() - 1)
			{
				// 获取尾段下一段
				nextParagraph = mParagraphList->at(lastParagraphIdx + 1);
			}

			// 删除尾段
			paragraph = mParagraphList->at(lastParagraphIdx);
			delParagraph(paragraph, true);

			if (lastParagraphIdx < mParagraphList->size())
			{
				// 将下一段内容剪切到首段
				paragraph = mParagraphList->at(firstParagraphIdx);
				paragraphMgr = paragraph->getChildrenMgr();

				paragraphMgr->addChildren(nextParagraph->getChildren(), 0, -1, -1, true);

				// 删除下一段
				delParagraph(nextParagraph, true);
			}
			// 如果尾段为节点，则删除整个首段
			else
			{
				paragraph = mParagraphList->at(firstParagraphIdx);
				delParagraph(paragraph, true);
			}
		}
		else
		{
			paragraph = mParagraphList->at(firstParagraphIdx);
			paragraphMgr = paragraph->getChildrenMgr();
			// 当前所选尾段
			lastParagraph = mParagraphList->at(lastParagraphIdx);
			paragraphMgr->addChildren(lastParagraph->getChildren(), lastIdxFromParagraph + 1, -1, -1, true);
			// 删除尾段
			delParagraph(lastParagraph, true);
		}
	}
	if (!needReplaceRow)
	{
		return;
	}
	ExcelCrossParentMgr * crossMgr = NULL;
	ViewObj * firstRow = first->getViewParent();
	RowLayout * firstRowLayout = (RowLayout *)firstRow->getLayout();
	ViewObj * firstTxtLayer = firstRow->getViewParent();
	// 如果删除首项为表格，判断是否跨页，如果跨页，则取所跨第一页
	if (first->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
	{
		crossMgr = firstRow->getExcelCrossParentMgr();
		if (crossMgr && crossMgr->haveCrossParent())
		{
			firstTxtLayer = crossMgr->getFirstCrossParent();
		}
	}
	PageContentTxtLayerLayout * firstTxtLayerLayout = (PageContentTxtLayerLayout *)firstTxtLayer->getLayout();
	ViewObj * firstPage = firstTxtLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	ViewObj * pagePanel = firstPage->getViewParent();
	ViewObj * firstPart = NULL;
	// 如果为页眉页脚层
	if (firstTxtLayer->isObj(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER))
	{
		firstPart = firstTxtLayer->getViewParent();
		if (!pagePanel)
		{
			firstPage = firstPart->getCrossParentMgr()->getLastCrossParent();
			QList<BaseContainer *> * tPageParts = firstPage->getChildren();
			for (int i = 0; i < tPageParts->size(); i++)
			{
				tPageParts->at(i)->setParent(firstPage);
			}
			pagePanel = firstPage->getViewParent();
		}
	}
	BaseLayout * pagePanelLayout = pagePanel->getLayout();
	QList<BaseContainer *> * pageList = pagePanel->getChildren();
	int firstPageIdx = firstPage->getViewIndex();
	int firstRowIdx = firstTxtLayer->getChildren()->indexOf(firstRow);
	int firstViewIdx = first->getViewIndex();
	if (firstViewIdx == -1)
	{
		int a = 0;
	}
	ViewObj * lastRow = last->getViewParent();
	ViewObj * lastTxtLayer = lastRow->getViewParent();
	PageContentTxtLayerLayout * lastTxtLayerLayout = (PageContentTxtLayerLayout *)lastTxtLayer->getLayout();
	RowLayout * lastRowLayout = (RowLayout *)lastRow->getLayout();

	int isLastCrossExcelRow = 0;
	// 如果删除尾项为表格，判断是否跨页，如果跨页，则取所跨第一页
	if (last->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
	{
		isLastCrossExcelRow = 1;
	}
	else if (last->isEnter())
	{
		int tempSize = tempAllViewChildren->size();
		if (size > 1 && tempAllViewChildren->at(size - 1)->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
		{
			isLastCrossExcelRow = 1;
		}
	}
	// 如果尾项跨页
	if (isLastCrossExcelRow == 1)
	{
		crossMgr = lastRow->getExcelCrossParentMgr();
		if (crossMgr && crossMgr->haveCrossParent())
		{
			lastTxtLayer = crossMgr->getLastCrossParent();
			isLastCrossExcelRow = 2;
		}
	}
	int lastRowIdx = lastTxtLayer->getChildren()->indexOf(lastRow);
	int lastViewIdx = last->getViewIndex();
	ViewObj * lastPage = lastTxtLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	int lastPageIdx = lastPage->getViewIndex();


	QList<BaseContainer *> * rowList = firstTxtLayer->getChildren();

	ViewObj * firstRowFirstChild = (ViewObj *)firstRow->getChild(0);
	int firstRowFirstChildWBefore = firstRowFirstChild->getViewParam()->getTotalW();
	int firstRowFirstChildWAfter = 0;
	//ViewObj * preRow = firstRow->getPreViewFromViewParent();
	RowLayout * preRowLayout = (RowLayout *)firstRowLayout->getPreLayout(false);
	//if (preRow)
	//	preRowLayout = (RowLayout *)preRow->getLayout();
	// 首尾同页
	if (firstPageIdx == lastPageIdx)
	{
		// 首尾同行
		if (firstRowIdx == lastRowIdx)
		{
			firstRowLayout->delChildren(firstViewIdx, lastViewIdx - firstViewIdx + 1, true, 0);
			// 删除后行为null
			if (firstRowLayout->haveChildren() < 1)
			{
				// 获取下一行
				BaseLayout * nextRowLayout = firstRowLayout->getNextLayout(false);
				if (nextRowLayout)
				{
					ViewObj * nextChild = (ViewObj *)nextRowLayout->getChild(0);
					firstRowFirstChildWAfter = nextChild->getViewParam()->getTotalW();
				}
				firstTxtLayerLayout->delChild(firstRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				// 删除行后txtLayer为null、则删除初始页
				if (firstTxtLayer->haveChildren() < 1)
				{
					pagePanelLayout->delChild(firstPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				}
				else
				{
					firstTxtLayerLayout->openReplace();
					firstTxtLayerLayout->replaceByType(BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				}
				if (preRowLayout)
				{
					// 如果首项宽度变窄,上一行进行补位判断
					if (firstRowFirstChildWBefore > firstRowFirstChildWAfter)
					{
						preRowLayout->judgeLessThanCircle(true);
					}
				}
			}
			// 删除后行不为null
			else
			{
				bool haveNextIndent = false;
				if (preRowLayout)
				{
					if (firstViewIdx == 0)
					{
						firstRowFirstChild = (ViewObj *)firstRowLayout->getChild(0);
						firstRowFirstChildWAfter = firstRowFirstChild->getViewParam()->getTotalW();
						// 如果首项宽度变窄,上一行进行补位判断
						if (firstRowFirstChildWBefore > firstRowFirstChildWAfter || firstRowFirstChild->isEnter())
						{
							haveNextIndent = preRowLayout->judgeLessThanCircle(true);
						}
					}
				}
				if (!haveNextIndent)
					firstRowLayout->judgeLessThanCircle(true);
			}
		}
		// 同页跨行
		else
		{
			// 删除中间行
			int delRowNum = lastRowIdx - firstRowIdx - 1;
			if (delRowNum > 0)
				firstTxtLayerLayout->delChildren(firstRowIdx + 1, delRowNum, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			bool delLastRow = false;
			// 最后待删除项为尾行回车
			if (last->isEnter())
			{
				delLastRow = true;
				firstTxtLayerLayout->delChild(lastRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			}
			// 尾行有剩余项，删除尾行选中项
			else
			{
				//lastRowLayout = (RowLayout *)lastRow->getLayout();
				int tempSize = lastRowLayout->getChildrenSize();
				if (lastViewIdx + 1 >= tempSize)
				{
					delLastRow = true;
					firstTxtLayerLayout->delChild(lastRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				}
				else
					lastRowLayout->delChildren(0, lastViewIdx + 1, true, 0);
			}

			// 如果首行不是从第0个开始删
			if (firstViewIdx > 0)
			{
				firstRowLayout->delChildren(firstViewIdx, -1, true, 0);

				bool haveNextIndent = firstRowLayout->judgeLessThanCircle(true);
				//if (!haveNextIndent)
				//{
				//	astRowLayout->replaceChildren();
				//}
			}
			// 删除首行
			else
			{
				firstTxtLayerLayout->delChild(firstRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				if (preRowLayout)
				{
					bool haveNextIndent = preRowLayout->judgeLessThanCircle(true);
					//if (!haveNextIndent)
					//{
					//	lastRowLayout->replaceChildren();
					//}
				}
				//else
				// 如果未删除尾行,则对尾行排版
				if (!delLastRow)
					lastRowLayout->replaceChildren();
				// 如果首尾行均删除，则删除本页
				else
				{
					if (firstTxtLayerLayout->haveChildren() < 1)
						pagePanelLayout->delChild(firstPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				}
			}
		}
	}
	// 跨页
	else
	{
		// 删首页光标以后的行
		firstTxtLayerLayout->delChildren(firstRowIdx + 1, -1, false, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

		// 删除中间页
		temp = pagePanelLayout->delChildren(firstPageIdx + 1, lastPageIdx - firstPageIdx - 1, true
			, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

		// 删除尾页光标以前行
		lastTxtLayerLayout->delChildren(0, lastRowIdx, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		// 删除尾页光标行内容
		temp = lastRowLayout->delChildren(0, lastViewIdx + 1, true, BaseConstants::ReplaceType::REPLACE_NOW);
		// 如果尾行删除后为null
		if (temp <= 0)
		{
			ExcelCrossParentMgr * tMgr = ((ViewObj *)lastRow)->getExcelCrossParentMgr();
			if (tMgr && tMgr->haveCrossParent())
			{
				tMgr->clearParents(false);
			}
			int lastPageTemp = 0;
			lastPageTemp = lastTxtLayerLayout->delChild(lastRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			if (lastPageTemp <= 0)
			{
				pagePanelLayout->delChild(lastPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			}
		}
		else if (isLastCrossExcelRow == 2)
		{
			// 如果将跨页表格行里的表格删除
			if (!lastRow->isExcelRow())
			{
				// 如果首尾页相同，则从尾页中删除
				if (firstRow == lastRow)
				{
					int lastPageTemp = 0;
					crossMgr = lastRow->getExcelCrossParentMgr();
					crossMgr->clearParents(false);
					lastPageTemp = lastTxtLayerLayout->delChild(lastRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
					firstRow->setParent(firstTxtLayer);
					firstTxtLayerLayout->replaceByType(BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
					if (lastPageTemp <= 0)
					{
						pagePanelLayout->delChild(lastPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
					}
				}
			}
			else
			{
				crossMgr = lastRow->getExcelCrossParentMgr();
				crossMgr->clearParents(false);
			}
		}

		// 如果首尾行不为同一行（代表不是跨页行），删除首页光标行内容
		if (firstRowLayout != lastRowLayout)
		{
			temp = firstRowLayout->delChildren(firstViewIdx, -1, true, 0);
		}
		// 如果首行删除后为null
		if (temp <= 0)
		{
			//preRowLayout = firstRowLayout->getPreLayout(false);
			firstTxtLayerLayout->delChild(firstRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			// 如果从第0行开始，则删除初始页
			if (firstRowIdx == 0)
				pagePanelLayout->delChild(firstPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			// 如果不为首行,则上一行进行缩进判断
			if (preRowLayout)
			{
				preRowLayout->judgeLessThanCircle(true);
			}
		}
		// 检测首行缩进
		else
			firstRowLayout->judgeLessThanCircle(true);
	}

	if (needRefreshParagraphRow)
	{
		ParagraphData * data = (ParagraphData *)firstParagraph->getContentData();
		//data->replaceRowList();
	}
}
void ParagraphManager::delExchangeRecordChildren(QList<QList<BaseContainer *> *> * seperateAllViewChildren, QList<BaseContainer *> * tempAllViewChildren)
{
	int temp = 0;
	ViewObj * first = (ViewObj *)tempAllViewChildren->at(0);
	BaseParagraph * firstParagraph = first->getParagraphParent();
	int firstIdxFromParagraph = first->getParagraphIndex();
	// 首项所属段落的段落idx
	int firstParagraphIdx = firstParagraph->getIndexFromParagraphMgr();
	int size = tempAllViewChildren->size();
	ViewObj * last = (ViewObj *)tempAllViewChildren->at(size - 1);
	BaseParagraph * lastParagraph = last->getParagraphParent();
	int lastIdxFromParagraph = last->getParagraphIndex();
	int lastParagraphIdx = lastParagraph->getIndexFromParagraphMgr();

	bool needRefreshParagraphRow = false;
	if (first->isEnter())
	{
		needRefreshParagraphRow = true;
	}
	BaseParagraph * paragraph = NULL;
	BaseChildrenManager * paragraphMgr = NULL;
	BaseParagraph * nextParagraph = NULL;

	// 从paragraphManager中删除段落
	for (int i = firstParagraphIdx; i < lastParagraphIdx + 1; i++)
	{
		paragraph = mParagraphList->at(firstParagraphIdx);
		delParagraph(paragraph, true);
	}

	ViewObj * firstRow = first->getViewParent();
	RowLayout * firstRowLayout = (RowLayout *)firstRow->getLayout();
	ViewObj * firstTxtLayer = firstRow->getViewParent();
	PageContentTxtLayerLayout * firstTxtLayerLayout = (PageContentTxtLayerLayout *)firstTxtLayer->getLayout();
	ViewObj * firstPage = firstTxtLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	ViewObj * pagePanel = firstPage->getViewParent();
	BaseLayout * pagePanelLayout = pagePanel->getLayout();
	QList<BaseContainer *> * pageList = pagePanel->getChildren();
	int firstPageIdx = firstPage->getViewIndex();
	int firstRowIdx = firstRow->getViewIndex();
	int firstViewIdx = first->getViewIndex();

	ViewObj * lastPage = last->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	int lastPageIdx = lastPage->getViewIndex();
	ViewObj * lastRow = last->getViewParent();
	ViewObj * lastTxtLayer = lastRow->getViewParent();
	PageContentTxtLayerLayout * lastTxtLayerLayout = (PageContentTxtLayerLayout *)lastTxtLayer->getLayout();
	RowLayout * lastRowLayout = (RowLayout *)lastRow->getLayout();
	int lastRowIdx = lastRow->getViewIndex();
	int lastViewIdx = last->getViewIndex();

	QList<BaseContainer *> * rowList = firstTxtLayer->getChildren();

	ViewObj * firstRowFirstChild = (ViewObj *)firstRow->getChild(0);
	int firstRowFirstChildWBefore = firstRowFirstChild->getViewParam()->getTotalW();
	int firstRowFirstChildWAfter = 0;
	RowLayout * preRowLayout = (RowLayout *)firstRowLayout->getPreLayout(false);
	// 首尾同页
	if (firstPageIdx == lastPageIdx)
	{
		// 首尾同行
		if (firstRowIdx == lastRowIdx)
		{
			firstRowLayout->delChildren(firstViewIdx, lastViewIdx - firstViewIdx + 1, true, 0);
			// 删除后行为null
			if (firstRowLayout->haveChildren() < 1)
			{
				// 获取下一行
				BaseLayout * nextRowLayout = firstRowLayout->getNextLayout(false);
				if (nextRowLayout)
				{
					ViewObj * nextChild = (ViewObj *)nextRowLayout->getChild(0);
					firstRowFirstChildWAfter = nextChild->getViewParam()->getTotalW();
				}
				firstTxtLayerLayout->delChild(firstRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				// 删除行后txtLayer为null、则删除初始页
				if (firstTxtLayer->haveChildren() < 1)
				{
					pagePanelLayout->delChild(firstPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				}
				else
				{
					firstTxtLayerLayout->openReplace();
					firstTxtLayerLayout->replaceByType(BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				}
				if (preRowLayout)
				{
					// 如果首项宽度变窄,上一行进行补位判断
					if (firstRowFirstChildWBefore > firstRowFirstChildWAfter)
					{
						preRowLayout->judgeLessThanCircle(true);
					}
				}
			}
			// 删除后行不为null
			else
			{
				bool haveNextIndent = false;
				if (preRowLayout)
				{
					if (firstViewIdx == 0)
					{
						firstRowFirstChild = (ViewObj *)firstRowLayout->getChild(0);
						firstRowFirstChildWAfter = firstRowFirstChild->getViewParam()->getTotalW();
						// 如果首项宽度变窄,上一行进行补位判断
						if (firstRowFirstChildWBefore > firstRowFirstChildWAfter)
						{
							haveNextIndent = preRowLayout->judgeLessThanCircle(true);
						}
					}
				}
				if (!haveNextIndent)
					firstRowLayout->judgeLessThanCircle(true);
			}
		}
		// 同页跨行
		else
		{
			// 删除中间行
			int delRowNum = lastRowIdx - firstRowIdx - 1;
			if (delRowNum > 0)
				firstTxtLayerLayout->delChildren(firstRowIdx + 1, delRowNum, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			bool delLastRow = false;
			// 最后待删除项为尾行回车
			if (last->isEnter())
			{
				delLastRow = true;
				firstTxtLayerLayout->delChild(lastRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			}
			// 尾行有剩余项，删除尾行选中项
			else
			{
				//lastRowLayout = (RowLayout *)lastRow->getLayout();
				lastRowLayout->delChildren(0, lastViewIdx + 1, true, 0);
			}

			// 如果首行不是从第0个开始删
			if (firstViewIdx > 0)
			{
				firstRowLayout->delChildren(firstViewIdx, -1, true, 0);

				bool haveNextIndent = firstRowLayout->judgeLessThanCircle(true);
				//if (!haveNextIndent)
				//{
				//	astRowLayout->replaceChildren();
				//}
			}
			// 删除首行
			else
			{
				firstTxtLayerLayout->delChild(firstRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				if (preRowLayout)
				{
					bool haveNextIndent = preRowLayout->judgeLessThanCircle(true);
					//if (!haveNextIndent)
					//{
					//	lastRowLayout->replaceChildren();
					//}
				}
				//else
				// 如果未删除尾行,则对尾行排版
				if (!delLastRow)
					lastRowLayout->replaceChildren();
				// 如果首尾行均删除，则删除本页
				else
				{
					if (firstTxtLayerLayout->haveChildren() < 1)
						pagePanelLayout->delChild(firstPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				}
			}
		}
	}
	// 跨页
	else
	{
		// 删首页光标以后的行
		firstTxtLayerLayout->delChildren(firstRowIdx + 1, -1, false, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

		// 删除中间页
		temp = pagePanelLayout->delChildren(firstPageIdx + 1, lastPageIdx - firstPageIdx - 1, true
			, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

		// 删除尾页光标以前行
		lastTxtLayerLayout->delChildren(0, lastRowIdx, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		// 删除尾页光标行内容
		temp = lastRowLayout->delChildren(0, lastViewIdx + 1, true, BaseConstants::ReplaceType::REPLACE_NOW);
		// 如果尾行删除后为null
		if (temp <= 0)
		{
			lastTxtLayerLayout->delChild(lastRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		}
		// 删除首页光标行内容
		temp = firstRowLayout->delChildren(firstViewIdx, -1, true, 0);
		// 如果首行删除后为null
		if (temp <= 0)
		{
			//preRowLayout = firstRowLayout->getPreLayout(false);
			firstTxtLayerLayout->delChild(firstRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			// 如果从第0行开始，则删除初始页
			if (firstRowIdx == 0)
				pagePanelLayout->delChild(firstPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			// 如果不为首行,则上一行进行缩进判断
			if (preRowLayout)
			{
				preRowLayout->judgeLessThanCircle(true);
			}
		}
		// 检测首行缩进
		else
			firstRowLayout->judgeLessThanCircle(true);
	}
}