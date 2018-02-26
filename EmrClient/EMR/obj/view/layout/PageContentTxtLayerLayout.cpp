#include "PageContentTxtLayerLayout.h"



#include "ViewObj.h"

#include "ExcelTemplete.h"

#include "RowViewParam.h"
#include "VerticalViewParam.h"

#include "LayoutReplaceManager.h"
#include "ExcelCrossParentMgr.h"

#include "WriterManager.h"
#include "ViewFactory.h"

#include "ConstantsID.h"
#include "BaseConstants.h"
#include "ExcelLayout.h"
#include "GlobalTemplate.h"

PageContentTxtLayerLayout::PageContentTxtLayerLayout(ViewObj * viewOwner) : VerticalLayout(viewOwner
	, ConstantsID::ClassId::PAGE_CONTENT_TXT_LAYER_LAYOUT)
{
	mRestH = 0;
	mPagePanel = NULL;
}
PageContentTxtLayerLayout::~PageContentTxtLayerLayout()
{
}
void PageContentTxtLayerLayout::addChild(BaseContainer * source, int idx, bool refreshIdx, char replaceType)
{
	BaseLayout::addChild(source, idx, refreshIdx, replaceType);
	// 行被添加到文字层上时，根据文字层宽度来刷新自身宽度
	ViewObj * row = (ViewObj *)source;
	row->getViewParam()->refreshWByParent(false);
}
void PageContentTxtLayerLayout::addChildren(QList<BaseContainer *> * rowList, int start, int num, int idx, bool refreshIdx, char replaceType)
{
	BaseLayout::addChildren(rowList, start, num, idx, refreshIdx, replaceType);
}
int PageContentTxtLayerLayout::delChild(BaseContainer * child, bool refreshIdx, char replaceType)
{
	if (child->isExcelRow())
	{
		ExcelCrossParentMgr * mgr = ((ViewObj *)child)->getExcelCrossParentMgr();
		if (mgr && mgr->haveCrossParent())
		{
			mgr->clearParents(false);
		}
	}
	mChildren->removeOne(child);

	child->clearParent(mOwner->getClassId());
	if (refreshIdx)
	{
		//int idx = mChildren->indexOf(child);
		mOwner->refreshChildrenIndex(0);
	}
	if (mChildren->size() < 1)
		return -1;

	//int objID = mViewOwner->getObjID();
	//if (objID == ConstantsID::ContainerId::EXCEL_TXT_LAYER)
	//{
	//	mViewOwner->getViewParam()->refreshHByChildren(true);
	//}
	openReplace();
	replaceByType(replaceType);
	return 1;
}
int PageContentTxtLayerLayout::delChildren(int start, int num, bool refreshIdx, char replaceType)
{
	if (num < 0)
	{
		num = mChildren->size() - start;
	}
	if (!mChildren)
	{
		return 0;
	}
	int end = 0;
	if (num == -1)
		num = mChildren->size() - start;
	if (num < 1)
	{
		return 0;
	}
	for (int i = 0; i < num; i++)
	{
		ViewObj * child = (ViewObj *)mChildren->at(start);
		if (child->isExcelRow())
		{
			ExcelCrossParentMgr * mgr = ((ViewObj *)child)->getExcelCrossParentMgr();
			if (mgr && mgr->haveCrossParent())
			{
				mgr->clearParents(false);
			}
		}
		mChildren->removeAt(start);
	}
	if (refreshIdx)
	{
		mOwner->refreshChildrenIndex(start);
	}
	if (replaceType != 0)
	{
		openReplace();
		replaceByType(replaceType);
	}
	if (mChildren->size() < 1)
		return -1;
	return 0;
}
void PageContentTxtLayerLayout::delChildWithOutClearCrossParents(BaseContainer * child, bool refreshIdx, char replaceType)
{
	BaseLayout::delChild(child, refreshIdx, replaceType);
}
void PageContentTxtLayerLayout::delChildrenWithOutClearCrossParents(int start, int num, bool refreshIdx, char replaceType)
{
	BaseLayout::delChildren(start, num, refreshIdx, replaceType);
}
void PageContentTxtLayerLayout::replaceChildren()
{
	if (!canReplace())
	{
		return;
	}
	else
		BaseLayout::replaceChildren();
	//if (pageList)
	{
		BaseContainer * parentPage = mViewOwner->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
		if (parentPage)
		{
			BaseContainer * pagePanel = parentPage->getViewParent();
			if (!pagePanel || !pagePanel->haveChild(parentPage))
			{
				return;
			}
		}
	}
	int objID = mViewOwner->getObjID();
	switch (objID)
	{
	case ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER:
		replaceTitleBottomLayer();
		break;
	case ConstantsID::ContainerId::CONTENT_TXT_LAYER:
		replaceContentLayer();
		break;
	case ConstantsID::ContainerId::EXCEL_TXT_LAYER:
		replaceExcelLayer();
		break;
	}
}

PageContentTxtLayerLayout * PageContentTxtLayerLayout::getPreLayout()
{
	ViewObj * page = mViewOwner->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	if (!mPagePanel)
		mPagePanel = page->getViewParent();
	QList<BaseContainer *> * pageList = mPagePanel->getChildren();
	int pageIdx = page->getViewIndex();
	if (pageIdx <= 0)
	{
		return NULL;
	}

	ViewObj * prePage = (ViewObj *)pageList->at(pageIdx - 1)->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
		->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
	PageContentTxtLayerLayout * layout = (PageContentTxtLayerLayout *)prePage->getLayout();
	return layout;
}
BaseLayout * PageContentTxtLayerLayout::createNextLayout()
{
	if (!mPagePanel)
		mPagePanel = mViewOwner->getViewParent();
	ViewObj * parentPage = mViewOwner->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	ViewObj * page = ViewFactory::createNormalPage(mViewOwner->getWriterMgr(), (ViewObj *)parentPage->findChildrenByID(ConstantsID::ContainerId::PAGE_TITLE, false)
		, (ViewObj *)parentPage->findChildrenByID(ConstantsID::ContainerId::PAGE_BOTTOM, false));
	int pageIdx = parentPage->getViewIndex();
	mPagePanel->getLayout()->addChild(page, pageIdx + 1, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
	ViewObj * layer = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
		->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
	BaseLayout * layout = layer->getLayout();
	return layout;
}
BaseLayout * PageContentTxtLayerLayout::getNextLayout(bool createWhenLast)
{
	ViewObj * page = mViewOwner->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	if (!mPagePanel)
		mPagePanel = mViewOwner->getViewParent();
	QList<BaseContainer *> * pageList = mPagePanel->getChildren();
	int size = pageList->size();
	int idx = page->getViewIndex();
	// 如果为最后一页
	if (idx == size - 1)
	{
		if (createWhenLast)
		{
			return createNextLayout();
		}
		else
			return NULL;
	}
	ViewObj * dest = (ViewObj *)pageList->at(idx + 1)->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
		->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
	return dest->getLayout();
}
int PageContentTxtLayerLayout::judgeCanHaveNum()
{
	int canHaveNum = 0;


	return canHaveNum;
}
int PageContentTxtLayerLayout::judgeNextPageIndent(int freeH)
{
	BaseContainer * lastRow = getLastChild();
	if (lastRow->getChild(0)->isObj(ConstantsID::ContainerId::END_PAGE_LINE))
	{
		return 0;
	}
	ViewObj * page = mViewOwner->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	if (!mPagePanel)
		mPagePanel = mViewOwner->getViewParent();
	QList<BaseContainer *> * pageList = mPagePanel->getChildren();
	int size = pageList->size();
	int idx = page->getViewIndex();
	if (idx == size - 1)
	{
		return 0;
	}
	ViewObj * nextPage = (ViewObj *)pageList->at(idx + 1);
	ViewObj * nextLayer = (ViewObj *)nextPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
		->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
	ViewObj * row = NULL;
	RowViewParam * rowParam = NULL;
	QList<BaseContainer *> * nextRowList = nextLayer->getChildren();
	size = nextRowList->size();
	int spendH = 0;
	int indentNum = -1;
	for (int i = 0; i < size; i++)
	{
		row = (ViewObj *)nextRowList->at(i);
		rowParam = (RowViewParam *)row->getViewParam();
		spendH += rowParam->getDistanceWithPreRow();
		spendH += rowParam->getTotalH();
		if (spendH > freeH)
		{
			if (row->isExcelRow())
			{
				indentNum = i + 1;
			}
			else
			{
				indentNum = i;
			}
			break;
		}
		else if (row->isHorizonOrEndPageRow() == 2)
		{
			if (size == 1 || i == size - 1)
			{
				indentNum = -1;
			}
			else
				indentNum = i + 1;
			break;
		}
	}
	if (indentNum == 0)
	{
		return 0;
	}
	// 下一页全部缩进完
	else if (indentNum < 0)
	{
		indentNum = size;
		mPagePanel->getLayout()->delChild(nextPage, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		ViewObj * lastRow = (ViewObj *)mChildren->at(mChildren->size() - 1);
		if (nextRowList->size() < 1 || nextRowList->at(0) != lastRow)
			addChildren(nextRowList, 0, indentNum, -1, true, BaseConstants::NONE);
		else
			addChildren(nextRowList, 1, indentNum - 1, -1, true, BaseConstants::NONE);
		//addChildren(nextRowList, 0, indentNum, -1, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
	}
	// 下一页仅缩进部分行
	else
	{
		ViewObj * lastRow = (ViewObj *)mChildren->at(mChildren->size() - 1);
		if (nextRowList->size() < 1 || nextRowList->at(0) != lastRow)
		{
			addChildren(nextRowList, 0, indentNum, -1, true, BaseConstants::NONE);
		}
		else
		{
			addChildren(nextRowList, 1, indentNum - 1, -1, true, BaseConstants::NONE);
		}
		//addChildren(nextRowList, 0, indentNum, -1, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		nextLayer->getLayout()->replaceByType(BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
	}
	return indentNum;
}
void PageContentTxtLayerLayout::replaceTitleBottomLayer()
{
	ViewObj * row = NULL;
	RowViewParam * rowParam = NULL;
	float minY = mViewParam->getInsideTop();
	//float disH = mViewParam->getChildrenDis();
	float curY = minY;
	float insideL = mViewParam->getInsideLeft();

	int maxH = 0;
	int size = mChildren->size();
	int rowPreDis = 0;
	ViewObj * page = mViewOwner->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	int pageIdx = page->getViewIndex();
	for (int i = 0; i < size; i++)
	{
		row = (ViewObj *)mChildren->at(i);
		rowParam = (RowViewParam *)row->getViewParam();
		if (rowParam->getX() < insideL)
		{
			rowParam->setX(insideL);
		}


		rowPreDis = rowParam->getDistanceWithPreRow();
		// 除了第0页、第0行，其他页均要加与上一行的间距
		if (i > 0 || pageIdx > 0)
		{
			curY += rowPreDis;
		}
		rowParam->setY(curY);
		curY += rowParam->getTotalH() + rowParam->getDistanceWithNextRow();
	}
	ViewObj * last = (ViewObj *)mChildren->at(size - 1);
	rowParam = (RowViewParam *)last->getViewParam();
	int destH = rowParam->getY() + rowParam->getTotalH();
	if (mViewParam->getPreInsideH() != destH)
	{
		mViewParam->setPreInsideH(destH);
		mViewParam->setInsideH(destH, BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_PARENT);

		ViewObj * page = mViewOwner->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
		if (!mPagePanel)
			mPagePanel = page->getViewParent();
		QList<BaseContainer *> * pageList = mPagePanel->getChildren();
		LayoutReplaceManager * tempMgr = mViewOwner->getWriterMgr()->getLayoutReplaceMgr();
		size = pageList->size();
		page = NULL;
		BaseLayout * pageLayout = NULL;
		ViewObj * pageContent = NULL;
		ViewObj * txtLayer = NULL;
		BaseLayout * layerLayout = NULL;
		for (int i = 0; i < size; i++)
		{
			page = (ViewObj *)pageList->at(i);
			pageLayout = page->getLayout();
			pageContent = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false);
			txtLayer = (ViewObj *)pageContent->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			layerLayout = txtLayer->getLayout();
			tempMgr->pushLayout(pageLayout);
			tempMgr->pushLayout(layerLayout);
		}
		//mViewParam->setSelfH(destH, BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_PARENT);
	}
}
void PageContentTxtLayerLayout::replaceContentLayer()
{
	ViewObj * pagePanel = mViewOwner->getParentPagePanel();
	ViewObj * page = mViewOwner->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	int pageIdx = page->getViewIndex();
	int size = mChildren->size();
	int insideTop = mViewParam->getInsideTop();
	int preRestH = 0;
	PageContentTxtLayerLayout * preLayout = getPreLayout();
	if (preLayout)
	{
		//preRestH = preLayout->mRestH;
	}
	else
	{
		if (mViewOwner->getViewParent()->getViewParent() == NULL)
		{
			return;
		}
	}
	int curY = insideTop - preRestH;
	int curBottom = 0;
	int maxY = insideTop + mViewParam->getInsideH();

	ViewObj * row = NULL;
	RowViewParam * rowParam = NULL;
	int rowH = 0;

	int canHaveRow = -1;
	int rowPreDis = 0;
	int * clip = mViewOwner->getViewParam()->getClip();
	//clip[3] = 0;
	// -1、代表当前剩余范围内，无可截取内容
	int excelCanLeaveMaxH = -1;
	int excelSpendH = 0;
	int preSpendH = 0;
	//int tempExcel = 0;
	ExcelCrossParentMgr * crossMgr = NULL;
	//if (page->getViewIndex() == 5)
	//{
	//	int a = 0;
	//}
	for (int i = 0; i < size; i++)
	{
		if (i == size - 2)
		{
			int a = 0;
		}
		row = (ViewObj *)mChildren->at(i);
		if (row->haveChildren() < 1)
		{
			return;
		}
		crossMgr = row->getExcelCrossParentMgr();
		bool isExcelRow = row->isExcelRow();
		// 如果当前子对象没有表格,判断此行之前是否为跨页行，如果是，清除跨页相关属性
		if (!isExcelRow)
		{
			if (crossMgr && crossMgr->haveCrossParent())
			{
				ViewObj * tempParent = NULL;
				BaseContainer * tempPage = NULL;
				QList<ViewObj *> * crossParents = crossMgr->getCrossParents();
				int tempSize = crossParents->size();
				// 删除中间页
				for (int i = 1; i < tempSize - 1; i++)
				{
					tempParent = crossParents->at(i);
					tempPage = tempParent->getParentPage();
					pagePanel->getLayout()->delChild(tempPage, true, 0);
				}
				// 从尾页中删除行
				if (tempSize > 1)
				{
					tempParent = crossParents->at(tempSize - 1);
					//if (tempParent != mViewOwner)
					{
						tempParent->getLayout()->delChild(row, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
					}
					//else
					{

					}
				}
				tempParent = crossParents->at(0);
				//tempParent = crossParents->at(0);
				row->setParent(tempParent);
				// 清空跨页父对象
				crossMgr->clearParents(false);
				// 如果当前层不为首页层
				if (tempParent != mViewOwner)
				{
					BaseLayout * layout = tempParent->getLayout();
					layout->openReplace();
					layout->replaceChildren();
					return;
				}
			}
		}

		rowParam = (RowViewParam *)row->getViewParam();
		rowPreDis = rowParam->getDistanceWithPreRow();
		// 除了第0页、第0行，其他页均要加与上一行的间距
		if (i > 0 || pageIdx > 0)
		{
			curY += rowPreDis;
		}
		int totalRowH = rowParam->getTotalH();
		rowH = totalRowH;
		if (isExcelRow)
		{
			crossMgr = row->getExcelCrossParentMgr();
			if (crossMgr && crossMgr->haveCrossParent())
			{
				rowH = rowH - crossMgr->getTotalPreSpendH(mViewOwner);
			}
		}
		curBottom = curY + rowH;
		// 如果超出本页范围
		if (curBottom > maxY)
		{
			// 如果不为表格行
			if (!isExcelRow)
			{
				mRestH = maxY - curY - rowPreDis;
				canHaveRow = i;

				// 判断本页后续行是否有跨页行，如果有，则清空跨页管理对象，并将行从对应页中移除
				if (i < size - 1)
				{
					for (int j = i + 1; j < size; j++)
					{
						ViewObj * tempRow = (ViewObj *)mChildren->at(j);
						ExcelCrossParentMgr * tempCrossMgr = tempRow->getExcelCrossParentMgr();
						if (tempCrossMgr && tempCrossMgr->haveCrossParent())
						{

							tempCrossMgr->clearParentsWithoutSource(mViewOwner);
						}
					}
				}
				break;
			}
			// 如果为表格行
			else
			{
				canHaveRow = i;
				mRestH = 0;
				excelSpendH = maxY - curY;

				BaseContainer * excel = row->getChild(0);
				ExcelTemplete * excelTemp = (ExcelTemplete *)excel->getTempParent();
				//excelCanLeaveMaxH = excelTemp->getWholeRowCrossPageH();
				//if (excelCanLeaveMaxH > -1)
				//{
				//	int tempDis = excelSpendH - excelCanLeaveMaxH;
				//	if (tempDis > 0)
				//	{
				//		excelSpendH = excelCanLeaveMaxH;
				//		clip[3] = tempDis;
				//	}
				//}
				// 如果未跨过页
				if (!crossMgr || !crossMgr->haveCrossParent())
				{
					if (!crossMgr)
					{
						crossMgr = new ExcelCrossParentMgr(row);
						row->initCrossParentMgr(crossMgr);
					}
					excelCanLeaveMaxH = excelTemp->getWholeRowCrossPageH(excelSpendH);
					if (excelCanLeaveMaxH > 0)
					{
						excelSpendH = excelCanLeaveMaxH;
					}
					else
						excelCanLeaveMaxH = -1;
					crossMgr->setFirstParam(mViewOwner, curY, excelSpendH);
				}
				else
				{
					// 如果已经跨页
					if (crossMgr->haveCrossParent())
					{
						// 如果为跨页首页
						if (isExcelRowFirstPage(page, i, crossMgr))
						{
							crossMgr->clearParentsWithoutSource(mViewOwner);
							excelCanLeaveMaxH = excelTemp->getWholeRowCrossPageH(excelSpendH);
							if (excelCanLeaveMaxH > 0)
							{
								excelSpendH = excelCanLeaveMaxH;
							}
							else
								excelCanLeaveMaxH = -1;
							crossMgr->setFirstParam(mViewOwner, curY, excelSpendH);
						}
						else
						{
							preSpendH = crossMgr->getTotalPreSpendH(mViewOwner);
							excelCanLeaveMaxH = excelTemp->getWholeRowCrossPageH(excelSpendH + preSpendH);
							if (excelCanLeaveMaxH > preSpendH)
							{
								excelSpendH = excelCanLeaveMaxH - preSpendH;
								excelCanLeaveMaxH = excelSpendH;
							}
							else
								excelCanLeaveMaxH = -1;
							crossMgr->pushParam(mViewOwner, excelSpendH);
							crossMgr->clearParentsAfterSource(mViewOwner);
						}
					}
					// 如果未跨页
					else
					{
						crossMgr->setFirstParam(mViewOwner, curY, excelSpendH);
					}
				}

				if (excelCanLeaveMaxH > -1)
				{
					int tempDis = maxY - curY - excelCanLeaveMaxH;
					if (tempDis > 0)
					{
						excelSpendH = excelCanLeaveMaxH;
						clip[3] = tempDis;
					}
				}
				// 判断本页后续行是否有跨页行，如果有，则清空跨页管理对象，并将行从对应页中移除
				if (i < size - 1)
				{
					for (int j = i + 1; j < size; j++)
					{
						ViewObj * tempRow = (ViewObj *)mChildren->at(j);
						ExcelCrossParentMgr * tempCrossMgr = tempRow->getExcelCrossParentMgr();
						if (tempCrossMgr && tempCrossMgr->haveCrossParent())
						{

							tempCrossMgr->clearParentsWithoutSource(mViewOwner);
						}
					}
				}
			}
			rowParam->setY(curY);
			curY = maxY;
			break;
		}
		// 如果未超出范围
		else
		{
			// 如果为表格行
			if (row->isExcelRow())
			{
				// 如果已处于跨页状态
				if (crossMgr && crossMgr->haveCrossParent())
				{
					// 如果为跨页首页,则清空跨页属性
					if (isExcelRowFirstPage(page, i, crossMgr))
					{
						crossMgr->clearParentsWithoutSource(mViewOwner);
					}
					// 如果不为跨页首页,则表示正在跨页
					else
					{
						if (excelCanLeaveMaxH > -1)
						{
							rowH = excelCanLeaveMaxH;
						}
						crossMgr->pushParam(mViewOwner, rowH);
						rowParam->setY(curY);
						curY += rowH + rowParam->getDistanceWithNextRow();
						continue;
					}
				}
			}
			// 如果为分页符
			else if (row->isHorizonOrEndPageRow() == 2)
			{
				canHaveRow = i + 1;
				mRestH = 0;
				// 判断本页后续行是否有跨页行，如果有，则清空跨页管理对象，并将行从对应页中移除
				if (i < size - 1)
				{
					for (int j = i + 1; j < size; j++)
					{
						ViewObj * tempRow = (ViewObj *)mChildren->at(j);
						ExcelCrossParentMgr * tempCrossMgr = tempRow->getExcelCrossParentMgr();
						if (tempCrossMgr && tempCrossMgr->haveCrossParent())
						{
							tempCrossMgr->clearParentsWithoutSource(mViewOwner);
						}
					}
				}
				rowParam->setY(curY);
				curY += rowH + rowParam->getDistanceWithNextRow();
				break;
			}
		}
		rowParam->setY(curY);

		curY += rowH + rowParam->getDistanceWithNextRow();
	}
	// 如果容纳不完
	if (canHaveRow != -1)
	{
		//if (page->getViewIndex() == 1 && canHaveRow == 18)
		//{
		//	int a = 0;
		//}
		BaseLayout * nextLayout = getNextLayout(true);
		nextLayout->addChildren(mChildren, canHaveRow, -1, 0, true, BaseConstants::ReplaceType::REPLACE_NOW);
	}
	// 如果未达到最大值,则判断下一页缩进
	else if (curY < maxY)
	{
		int indentNum = judgeNextPageIndent(maxY - curY);
		if (indentNum > 0)
		{
			canHaveRow = size;
			//size = mChildren->size();
			int end = canHaveRow + indentNum;
			for (int i = canHaveRow; i < end; i++)
			{
				row = (ViewObj *)mChildren->at(i);
				rowParam = (RowViewParam *)row->getViewParam();
				rowPreDis = rowParam->getDistanceWithPreRow();
				// 除了第0页、第0行，其他页均要加与上一行的间距
				if (i > 0 || pageIdx > 0)
				{
					curY += rowPreDis;
				}
				rowH = rowParam->getTotalH();
				curBottom = curY + rowH;
				//// 如果本行超出范围
				//if (curBottom > maxY)
				//{
				//	mRestH = maxY - rowH - rowPreDis;
				//	canHaveRow = i;
				//	break;
				//}
				rowParam->setY(curY);

				curY += rowH + rowParam->getDistanceWithNextRow();
			}
			if (curY > maxY)
			{
				openReplace();
				crossMgr = row->getExcelCrossParentMgr();
				if (crossMgr && crossMgr->haveCrossParent())
				{
					crossMgr->clearParents(true);
					row->setParent(mViewOwner);
				}
				replaceChildren();
				return;
			}
		}
		mRestH = maxY - curY;
	}
}
void PageContentTxtLayerLayout::replaceExcelLayer()
{
	ViewObj * row = NULL;
	RowViewParam * rowParam = NULL;

	ViewObj * excelPage = mViewOwner->findViewFromParent(ConstantsID::ContainerId::EXCEL_PAGE, true);
	int verticalAlignType = 0;
	int minH = 0;
	ExcelLayout::getCellBaseInformation(minH, verticalAlignType, excelPage);

	int rowPreDis = 0;
	int size = mChildren->size();
	int insideTop = mViewParam->getInsideTop();
	int insideH = mViewParam->getInsideH();
	int insideB = mViewParam->getInsideBottom();
	float curY = 0;
	int rowH = 0;
	int curBottom = 0;
	float childrenEffectH = mViewParam->getChildrenRowH();
	int firstY = insideTop;
	switch (verticalAlignType)
	{
	case BaseConstants::Direction::MIDDLE:
		firstY += (insideH - childrenEffectH) / 2;
		break;
	case BaseConstants::Direction::DOWN:
		firstY = insideH - childrenEffectH - insideB;
		break;
	}
	//curY = insideTop;
	for (int i = 0; i < size; i++)
	{
		row = (ViewObj *)mChildren->at(i);
		rowParam = (RowViewParam *)row->getViewParam();
		// 除了第0行，其他均要加与上一行的间距
		if (i > 0)
		{
			rowPreDis = rowParam->getDistanceWithPreRow();
			curY += rowPreDis;
		}
		rowH = rowParam->getTotalH();
		curBottom = curY + rowH;
		rowParam->setY(firstY + curY);
		//rowParam->setY(curY);

		curY += rowH + rowParam->getDistanceWithNextRow();
	}

	// 如果本行超出表格txt层高度,则刷新表格高度
	if (curBottom != insideTop + insideH)
	{
		if ((minH <= 0 || (childrenEffectH >= minH || insideH > minH)))
		{
			// 向上循环刷新高度
			mViewParam->refreshHByChildren(true);

			ViewObj * excelPage = mViewOwner->findViewFromParent(ConstantsID::ContainerId::EXCEL_PAGE, true);
			ExcelTemplete * excelTemp = (ExcelTemplete *)excelPage->getTempParent();
			excelTemp->requestLayout(true);
		}
		else if (childrenEffectH < minH)
		{
			mViewParam->setSelfH(minH - insideTop - insideB);
			mViewOwner->getViewParent()->getViewParam()->refreshHByChildren(true);
			ViewObj * excelPage = mViewOwner->findViewFromParent(ConstantsID::ContainerId::EXCEL_PAGE, true);
			ExcelTemplete * excelTemp = (ExcelTemplete *)excelPage->getTempParent();
			excelTemp->requestLayout(true);
		}
	}
	else if (minH <= 0 || childrenEffectH < minH)
	{
		mViewParam->setSelfH(curBottom - insideTop - insideB);
		mViewOwner->getViewParent()->getViewParam()->refreshHByChildren(true);
		ViewObj * excelPage = mViewOwner->findViewFromParent(ConstantsID::ContainerId::EXCEL_PAGE, true);
		ExcelTemplete * excelTemp = (ExcelTemplete *)excelPage->getTempParent();
		excelTemp->requestLayout(true);
	}
}

bool PageContentTxtLayerLayout::isExcelRowFirstPage(ViewObj * page, int rowIdx, ExcelCrossParentMgr * crossMgr)
{
	if (rowIdx > 0)
	{
		return true;
	}
	else
	{
		int pageIdx = page->getViewIndex();
		if (pageIdx == 0)
		{
			return true;
		}
		if (crossMgr->getFirstCrossParent() == mViewOwner)
			return true;
	}
	return false;
}