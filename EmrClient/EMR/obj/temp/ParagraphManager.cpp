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
			// ע�⣬paragraphMgr��ͨƪ��ͨ���ǿ�ģ�������ĵģ����Է���ҳü��ҳ�š�ҳ���ݵĸ�������
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


	// ��ӽ�paragraph
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

	// �����ǰ�н���
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
			// ��ӽ�paragraph
			for (int i = 0; i < seperSize; i++)
			{
				list = seperateAllViewChildren->at(i);
				if (i == 0)
				{
					if (seperSize > 1)
					{
						paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, idxFromParagraph, true);
					}
					// ��������һ��
					else
					{
						// ��������лس�
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
						// ���û�س�
						else
						{
							paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, idxFromParagraph, true);
						}
					}
				}
				// �м��
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
				// ��β��
				else
				{
					// ������һ��Ϊ�س�
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


						// ����׶μ���ʣ������
						if (useParagraphDataList)
							tempData = BaseSystem::getInstance()->useBufferParagraphData(useParagraphDataList, useIndex);
						paragraph = TempleteFactory::createParagraph(mWriterMgr, this, tempData);
						paragraphChildrenMgr = paragraph->getChildrenMgr();
						paragraphChildrenMgr->addChildren(preChildren, idxFromParagraph + preChildren->size() - preSize, -1, 0, true);
						addParagraph(paragraph, paragraphIdx + i + 1, refreshIdx);
					}
					else
					{
						// ���β������
						if (useParagraphDataList)
							tempData = BaseSystem::getInstance()->useBufferParagraphData(useParagraphDataList, useIndex);
						paragraph = TempleteFactory::createParagraph(mWriterMgr, this, tempData);
						paragraphChildrenMgr = paragraph->getChildrenMgr();
						paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, 0, true);
						// ����׶μ���ʣ������
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
		// ��ӵ���ʾ���ֵ�����
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
	// ��ʼ��ʱû�н���
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
			// ���Ϊ�½����޴浵
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

	// �����ǰ�н���
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
		//// ��ӽ�paragraph
		//for (int i = 1; i < seperSize; i++)
		//{
		//	list = seperateAllViewChildren->at(i);
		//	if (i == 0)
		//	{
		//		if (seperSize > 1)
		//		{
		//			paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, idxFromParagraph, true);
		//			// ����ӽ�ȥ
		//			//paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, idxFromParagraph, true);
		//			//// ��ԭ�����������ݼ��е��¶�
		//			////if (!nextParagraph)
		//			//nextParagraph = TempleteFactory::createParagraph(mWriterMgr, this, paragraphData);
		//			//paragraphChildrenMgr = nextParagraph->getChildrenMgr();
		//			//paragraphChildrenMgr->addChildren(preChildren, idxFromParagraph, -1, 0, true);
		//			////if (!haveNext)
		//			//	addParagraph(nextParagraph, paragraphIdx + 1, refreshIdx);
		//		}
		//		// ��������һ��
		//		else
		//		{
		//			// ��������лس�
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
		//			// ���û�س�
		//			else
		//			{
		//				paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, idxFromParagraph, true);
		//			}
		//		}
		//	}
		//	// �м��
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
		//	// ��β��
		//	else
		//	{
		//		// ������һ��Ϊ�س�
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


		//			// ����׶μ���ʣ������
		//			paragraph = TempleteFactory::createParagraph(mWriterMgr, this, paragraphData);
		//			paragraphChildrenMgr = paragraph->getChildrenMgr();
		//			paragraphChildrenMgr->addChildren(preChildren, idxFromParagraph + preChildren->size() - preSize, -1, 0, true);
		//			addParagraph(paragraph, paragraphIdx + i + 1, refreshIdx);
		//		}
		//		else
		//		{
		//			// ���β������
		//			paragraph = TempleteFactory::createParagraph(mWriterMgr, this, paragraphData);
		//			paragraphChildrenMgr = paragraph->getChildrenMgr();
		//			paragraphChildrenMgr->addChildren((QList<BaseContainer *> *)list, 0, -1, 0, true);
		//			// ����׶μ���ʣ������
		//			paragraphChildrenMgr->addChildren(preChildren, idxFromParagraph + preChildren->size() - preSize, -1, -1, true);
		//			addParagraph(paragraph, paragraphIdx + i, refreshIdx);
		//		}
		//	}
		//}
		if (!needReplaceRow)
		{
			return;
		}
		// ��ӵ���ʾ���ֵ�����
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
	// ��ʼ��ʱû�н���
	else
	{
		bool allReadyLoad = false;
		if (mParagraphList->size() > 0)
			allReadyLoad = true;
		for (int i = 0; i < seperSize; i++)
		{
			list = seperateAllViewChildren->at(i);
			// ���Ϊ�½����޴浵
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
	// ������������Ķ���idx
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
	// ���Ҫɾ���Ķ����б���ͬһ����
	if (firstParagraphIdx == lastParagraphIdx)
	{
		paragraph = mParagraphList->at(firstParagraphIdx);
		paragraphMgr = paragraph->getChildrenMgr();
		paragraphMgr->delChildren(firstIdxFromParagraph, lastIdxFromParagraph - firstIdxFromParagraph + 1, true);
		// ������һ��Ϊ�س�
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
		// ɾ���׶�����
		paragraph = mParagraphList->at(firstParagraphIdx);
		paragraphMgr = paragraph->getChildrenMgr();
		paragraphMgr->delChildren(firstIdxFromParagraph, -1, true);

		// ɾ���м��
		int tempStart = firstParagraphIdx + 1;
		for (int i = tempStart; i < lastParagraphIdx; i++)
		{
			paragraph = mParagraphList->at(tempStart);
			delParagraph(paragraph, false);
		}
		// ˢ������
		refreshChildrenIndex(tempStart);
		// ����β������
		lastParagraphIdx = lastParagraph->getIndexFromParagraphMgr();
		// ɾ��β��
		// ���ѡ��β��Ϊβ�λس�,��β��ɾ��,����һ�κϲ����׶�
		if (last->isEnter())
		{
			if (lastParagraphIdx < mParagraphList->size() - 1)
			{
				// ��ȡβ����һ��
				nextParagraph = mParagraphList->at(lastParagraphIdx + 1);
			}

			// ɾ��β��
			paragraph = mParagraphList->at(lastParagraphIdx);
			delParagraph(paragraph, true);

			if (lastParagraphIdx < mParagraphList->size())
			{
				// ����һ�����ݼ��е��׶�
				paragraph = mParagraphList->at(firstParagraphIdx);
				paragraphMgr = paragraph->getChildrenMgr();

				paragraphMgr->addChildren(nextParagraph->getChildren(), 0, -1, -1, true);

				// ɾ����һ��
				delParagraph(nextParagraph, true);
			}
			// ���β��Ϊ�ڵ㣬��ɾ�������׶�
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
			// ��ǰ��ѡβ��
			lastParagraph = mParagraphList->at(lastParagraphIdx);
			paragraphMgr->addChildren(lastParagraph->getChildren(), lastIdxFromParagraph + 1, -1, -1, true);
			// ɾ��β��
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
	// ���ɾ������Ϊ����ж��Ƿ��ҳ�������ҳ����ȡ�����һҳ
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
	// ���Ϊҳüҳ�Ų�
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
	// ���ɾ��β��Ϊ����ж��Ƿ��ҳ�������ҳ����ȡ�����һҳ
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
	// ���β���ҳ
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
	// ��βͬҳ
	if (firstPageIdx == lastPageIdx)
	{
		// ��βͬ��
		if (firstRowIdx == lastRowIdx)
		{
			firstRowLayout->delChildren(firstViewIdx, lastViewIdx - firstViewIdx + 1, true, 0);
			// ɾ������Ϊnull
			if (firstRowLayout->haveChildren() < 1)
			{
				// ��ȡ��һ��
				BaseLayout * nextRowLayout = firstRowLayout->getNextLayout(false);
				if (nextRowLayout)
				{
					ViewObj * nextChild = (ViewObj *)nextRowLayout->getChild(0);
					firstRowFirstChildWAfter = nextChild->getViewParam()->getTotalW();
				}
				firstTxtLayerLayout->delChild(firstRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				// ɾ���к�txtLayerΪnull����ɾ����ʼҳ
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
					// ��������ȱ�խ,��һ�н��в�λ�ж�
					if (firstRowFirstChildWBefore > firstRowFirstChildWAfter)
					{
						preRowLayout->judgeLessThanCircle(true);
					}
				}
			}
			// ɾ�����в�Ϊnull
			else
			{
				bool haveNextIndent = false;
				if (preRowLayout)
				{
					if (firstViewIdx == 0)
					{
						firstRowFirstChild = (ViewObj *)firstRowLayout->getChild(0);
						firstRowFirstChildWAfter = firstRowFirstChild->getViewParam()->getTotalW();
						// ��������ȱ�խ,��һ�н��в�λ�ж�
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
		// ͬҳ����
		else
		{
			// ɾ���м���
			int delRowNum = lastRowIdx - firstRowIdx - 1;
			if (delRowNum > 0)
				firstTxtLayerLayout->delChildren(firstRowIdx + 1, delRowNum, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			bool delLastRow = false;
			// ����ɾ����Ϊβ�лس�
			if (last->isEnter())
			{
				delLastRow = true;
				firstTxtLayerLayout->delChild(lastRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			}
			// β����ʣ���ɾ��β��ѡ����
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

			// ������в��Ǵӵ�0����ʼɾ
			if (firstViewIdx > 0)
			{
				firstRowLayout->delChildren(firstViewIdx, -1, true, 0);

				bool haveNextIndent = firstRowLayout->judgeLessThanCircle(true);
				//if (!haveNextIndent)
				//{
				//	astRowLayout->replaceChildren();
				//}
			}
			// ɾ������
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
				// ���δɾ��β��,���β���Ű�
				if (!delLastRow)
					lastRowLayout->replaceChildren();
				// �����β�о�ɾ������ɾ����ҳ
				else
				{
					if (firstTxtLayerLayout->haveChildren() < 1)
						pagePanelLayout->delChild(firstPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				}
			}
		}
	}
	// ��ҳ
	else
	{
		// ɾ��ҳ����Ժ����
		firstTxtLayerLayout->delChildren(firstRowIdx + 1, -1, false, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

		// ɾ���м�ҳ
		temp = pagePanelLayout->delChildren(firstPageIdx + 1, lastPageIdx - firstPageIdx - 1, true
			, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

		// ɾ��βҳ�����ǰ��
		lastTxtLayerLayout->delChildren(0, lastRowIdx, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		// ɾ��βҳ���������
		temp = lastRowLayout->delChildren(0, lastViewIdx + 1, true, BaseConstants::ReplaceType::REPLACE_NOW);
		// ���β��ɾ����Ϊnull
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
			// �������ҳ�������ı��ɾ��
			if (!lastRow->isExcelRow())
			{
				// �����βҳ��ͬ�����βҳ��ɾ��
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

		// �����β�в�Ϊͬһ�У������ǿ�ҳ�У���ɾ����ҳ���������
		if (firstRowLayout != lastRowLayout)
		{
			temp = firstRowLayout->delChildren(firstViewIdx, -1, true, 0);
		}
		// �������ɾ����Ϊnull
		if (temp <= 0)
		{
			//preRowLayout = firstRowLayout->getPreLayout(false);
			firstTxtLayerLayout->delChild(firstRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			// ����ӵ�0�п�ʼ����ɾ����ʼҳ
			if (firstRowIdx == 0)
				pagePanelLayout->delChild(firstPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			// �����Ϊ����,����һ�н��������ж�
			if (preRowLayout)
			{
				preRowLayout->judgeLessThanCircle(true);
			}
		}
		// �����������
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
	// ������������Ķ���idx
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

	// ��paragraphManager��ɾ������
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
	// ��βͬҳ
	if (firstPageIdx == lastPageIdx)
	{
		// ��βͬ��
		if (firstRowIdx == lastRowIdx)
		{
			firstRowLayout->delChildren(firstViewIdx, lastViewIdx - firstViewIdx + 1, true, 0);
			// ɾ������Ϊnull
			if (firstRowLayout->haveChildren() < 1)
			{
				// ��ȡ��һ��
				BaseLayout * nextRowLayout = firstRowLayout->getNextLayout(false);
				if (nextRowLayout)
				{
					ViewObj * nextChild = (ViewObj *)nextRowLayout->getChild(0);
					firstRowFirstChildWAfter = nextChild->getViewParam()->getTotalW();
				}
				firstTxtLayerLayout->delChild(firstRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				// ɾ���к�txtLayerΪnull����ɾ����ʼҳ
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
					// ��������ȱ�խ,��һ�н��в�λ�ж�
					if (firstRowFirstChildWBefore > firstRowFirstChildWAfter)
					{
						preRowLayout->judgeLessThanCircle(true);
					}
				}
			}
			// ɾ�����в�Ϊnull
			else
			{
				bool haveNextIndent = false;
				if (preRowLayout)
				{
					if (firstViewIdx == 0)
					{
						firstRowFirstChild = (ViewObj *)firstRowLayout->getChild(0);
						firstRowFirstChildWAfter = firstRowFirstChild->getViewParam()->getTotalW();
						// ��������ȱ�խ,��һ�н��в�λ�ж�
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
		// ͬҳ����
		else
		{
			// ɾ���м���
			int delRowNum = lastRowIdx - firstRowIdx - 1;
			if (delRowNum > 0)
				firstTxtLayerLayout->delChildren(firstRowIdx + 1, delRowNum, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			bool delLastRow = false;
			// ����ɾ����Ϊβ�лس�
			if (last->isEnter())
			{
				delLastRow = true;
				firstTxtLayerLayout->delChild(lastRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			}
			// β����ʣ���ɾ��β��ѡ����
			else
			{
				//lastRowLayout = (RowLayout *)lastRow->getLayout();
				lastRowLayout->delChildren(0, lastViewIdx + 1, true, 0);
			}

			// ������в��Ǵӵ�0����ʼɾ
			if (firstViewIdx > 0)
			{
				firstRowLayout->delChildren(firstViewIdx, -1, true, 0);

				bool haveNextIndent = firstRowLayout->judgeLessThanCircle(true);
				//if (!haveNextIndent)
				//{
				//	astRowLayout->replaceChildren();
				//}
			}
			// ɾ������
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
				// ���δɾ��β��,���β���Ű�
				if (!delLastRow)
					lastRowLayout->replaceChildren();
				// �����β�о�ɾ������ɾ����ҳ
				else
				{
					if (firstTxtLayerLayout->haveChildren() < 1)
						pagePanelLayout->delChild(firstPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				}
			}
		}
	}
	// ��ҳ
	else
	{
		// ɾ��ҳ����Ժ����
		firstTxtLayerLayout->delChildren(firstRowIdx + 1, -1, false, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

		// ɾ���м�ҳ
		temp = pagePanelLayout->delChildren(firstPageIdx + 1, lastPageIdx - firstPageIdx - 1, true
			, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

		// ɾ��βҳ�����ǰ��
		lastTxtLayerLayout->delChildren(0, lastRowIdx, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		// ɾ��βҳ���������
		temp = lastRowLayout->delChildren(0, lastViewIdx + 1, true, BaseConstants::ReplaceType::REPLACE_NOW);
		// ���β��ɾ����Ϊnull
		if (temp <= 0)
		{
			lastTxtLayerLayout->delChild(lastRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		}
		// ɾ����ҳ���������
		temp = firstRowLayout->delChildren(firstViewIdx, -1, true, 0);
		// �������ɾ����Ϊnull
		if (temp <= 0)
		{
			//preRowLayout = firstRowLayout->getPreLayout(false);
			firstTxtLayerLayout->delChild(firstRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			// ����ӵ�0�п�ʼ����ɾ����ʼҳ
			if (firstRowIdx == 0)
				pagePanelLayout->delChild(firstPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			// �����Ϊ����,����һ�н��������ж�
			if (preRowLayout)
			{
				preRowLayout->judgeLessThanCircle(true);
			}
		}
		// �����������
		else
			firstRowLayout->judgeLessThanCircle(true);
	}
}