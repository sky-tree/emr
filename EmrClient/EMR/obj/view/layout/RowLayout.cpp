#include "RowLayout.h"

#include "WriterManager.h"

#include "ExcelCrossParentMgr.h"
#include "LayoutReplaceManager.h"

#include "ViewObj.h"
#include "BaseParagraph.h"

#include "ParagraphData.h"

#include "BaseViewParam.h"
#include "RowViewParam.h"
#include "VerticalViewParam.h"

#include "ViewFactory.h"

#include "WriterManager.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

#include "GlobalTemplate.h"

RowLayout::RowLayout(ViewObj * viewOwner) : BaseLayout(viewOwner, ConstantsID::ClassId::ROW_LAYOUT)
{
	mRowParam = (RowViewParam *)viewOwner->getViewParam();
}
RowLayout::~RowLayout()
{
}
BaseParagraph * RowLayout::getParagraph()
{
	return mChildren->at(0)->getParagraphParent();
}
BaseLayout * RowLayout::createPreLayout()
{
	ViewObj * preRow = ViewFactory::createRow(mViewOwner->getWriterMgr(), mViewOwner->getParagraphParent());
	BaseLayout * preLayout = preRow->getLayout();
	ViewObj * txtLayer = mViewOwner->getViewParent();
	BaseViewParam * newParam = preRow->getViewParam();
	int rowIdx = txtLayer->getChildren()->indexOf(mViewOwner);
	txtLayer->getChildrenMgr()->addChild(preRow, rowIdx, true);
	newParam->refreshWByParent(BaseConstants::NONE);
	return preLayout;
}
BaseLayout * RowLayout::createNextLayout()
{
	ViewObj * nextRow = ViewFactory::createRow(mViewOwner->getWriterMgr(), mViewOwner->getParagraphParent());
	BaseLayout * nextLayout = nextRow->getLayout();
	ViewObj * txtLayer = mViewOwner->getViewParent();
	BaseViewParam * newParam = nextRow->getViewParam();
	int rowIdx = txtLayer->getChildren()->indexOf(mViewOwner);
	BaseLayout * layout = (BaseLayout *)txtLayer->getChildrenMgr();
	layout->addChild(nextRow, rowIdx + 1, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
	newParam->refreshWByParent(BaseConstants::NONE);
	return nextLayout;
}
int RowLayout::judgeCanHaveNumAfterAdd(QList<BaseContainer *> * children, int start, int num, int idx)
{
	if (num == 0)
	{
		return -1;
	}
	else if (num == -1)
	{
		num = children->size();
	}
	ViewObj * obj = NULL;
	BaseViewParam * objParam = NULL;
	int end = start + num;
	ViewObj * firstChild = (ViewObj *)children->at(0);
	if (haveChildren() > 0)
	{
		firstChild = (ViewObj *)mChildren->at(0);
	}
	float objW = 0;
	float curW = 0;// mRowParam->getEffectL(firstChild);
	int maxW = mRowParam->getEffectW(firstChild);
	int maxNum = -1;
	if (haveChildren() > BaseConstants::NONE)
	{
		int size = mChildren->size();
		// 遍历idx前面子对象总宽度
		for (int i = 0; i < idx; i++)
		{
			obj = (ViewObj *)mChildren->at(i);
			if (obj->isEnter())
			{
				maxNum = i + 1;
				return maxNum;
			}
			else if (obj->isHorizontalOrEndPageLine())
			{
				if (i > 0)
				{
					return i;
				}
				return 1;
			}
			objParam = obj->getViewParam();
			// 如果为表格，则默认为行宽
			if (obj->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
			{
				objW = maxW;
			}
			else
				objW = objParam->getTotalW();
			curW += objW;
			if (curW > maxW)
			{
				maxNum = i;
				if (maxNum == 0)
				{
					maxNum = 1;
				}
				return maxNum;
			}
		}
		// 全部容纳
		maxNum = idx;
		// 遍历需添加子对象总宽度
		for (int i = start; i < end; i++)
		{
			obj = (ViewObj *)children->at(i);
			if (obj->isEnter())
			{
				maxNum += i - start + 1;
				return maxNum;
			}
			else if (obj->isHorizontalOrEndPageLine())
			{
				maxNum += i - start;
				if (maxNum == 0)
				{
					maxNum = 1;
				}
				return maxNum;
			}
			objParam = obj->getViewParam();
			if (obj->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
			{
				objW = maxW;
			}
			else
				objW = objParam->getTotalW();
			curW += objW;
			if (curW > maxW)
			{
				if (i == 0 && !children->at(start)->isEnter() && mChildren->at(0)->isEnter())
				{
					return 2;
				}
				maxNum += i - start;
				return maxNum;
			}
		}
		// 全部容纳
		if (maxNum == idx)
		{
			maxNum += num;
		}

		// 遍历idx后面子对象总宽度
		for (int i = idx; i < size; i++)
		{
			obj = (ViewObj *)mChildren->at(i);
			if (obj->isEnter())
			{
				maxNum += i - idx + 1;
				return maxNum;
			}
			else if (obj->isHorizontalOrEndPageLine())
			{
				maxNum += i - idx;
				if (maxNum == 0)
				{
					maxNum = 1;
				}
				return maxNum;
			}
			objParam = obj->getViewParam();
			if (obj->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
			{
				objW = maxW;
			}
			else
				objW = objParam->getTotalW();
			curW += objW;
			if (curW > maxW)
			{
				maxNum += i - idx;
				return maxNum;
			}
		}
		if (maxNum == idx + num)
		{
			maxNum = size + num;
		}
	}
	else
	{
		for (int i = start; i < end; i++)
		{
			obj = (ViewObj *)children->at(i);
			if (obj->isEnter())
			{
				maxNum = i + 1;
				return maxNum;
			}
			else if (obj->isHorizontalOrEndPageLine())
			{
				if (i > 0)
				{
					maxNum = i;
				}
				else
					maxNum = 1;
				return maxNum;
			}
			objParam = obj->getViewParam();
			if (obj->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
			{
				objW = maxW;
			}
			else
				objW = objParam->getTotalW();
			curW += objW;
			if (curW > maxW)
			{
				maxNum = i - start;
				if (maxNum == 0)
					maxNum = 1;
				break;
			}
		}
	}


	return maxNum;
}
void RowLayout::judgeAddChildren(QList<BaseContainer *> * children, int start, int num, int idx, char needReplace)
{
	int canHaveNum = judgeCanHaveNumAfterAdd(children, start, num, idx);
	if (haveChildren() < 0)
	{
		createChildren();
	}
	int size = mChildren->size();
	if (num == -1)
	{
		num = children->size() - start;
	}
	int end = start + num;
	// 如果当前行能全部容纳
	if (canHaveNum >= size + num)
	{
		addChildren(children, start, num, idx, true, needReplace);
	}
	// 如果本行容纳不完
	else
	{
		LayoutReplaceManager * replaceMgr = mViewOwner->getWriterMgr()->getLayoutReplaceMgr();
		RowLayout * tempLayout = NULL;
		ViewObj * obj = NULL;
		RowLayout * nextLayout = (RowLayout *)getNextLayout(true);
		ViewObj * nextRow = nextLayout->getViewOwner();
		RowViewParam * nextRowParam = (RowViewParam *)nextRow->getViewParam();
		//QList<ViewObj *> * tempList = mViewOwner->getWriterMgr()->getTempViewList();
		//tempList->clear();
		int tempStart = 0;
		// 如果能容纳项不足idx
		if (canHaveNum < idx)
		{
			// 将本行canHaveNum以前的对象添加到下一行
			nextLayout->addChildren(mChildren, canHaveNum, idx - canHaveNum, canHaveNum, false, 0);
			//for (int i = canHaveNum; i < idx; i++)
			//{
			//	obj = (ViewObj *)mChildren->at(canHaveNum);
			//	nextLayout->addChild(obj, i - canHaveNum, false, BaseConstants::NONE, false);
			//}
			// 将外来对象添加到下一行
			tempStart = idx - canHaveNum;
			nextLayout->addChildren(children, start, end - start, tempStart, false, 0);
			//for (int i = start; i < end; i++)
			//{
			//	obj = (ViewObj *)children->at(i);
			//	nextLayout->addChild(obj, tempStart, false, BaseConstants::NONE, false);
			//}
			tempStart += end - start;
			nextLayout->addChildren(mChildren, canHaveNum, mChildren->size() - idx, tempStart, false, 0);
			//for (int i = idx; i < mChildren->size(); i++)
			//{
			//	obj = (ViewObj *)mChildren->at(canHaveNum);
			//	nextLayout->addChild(obj, tempStart, false, BaseConstants::NONE, false);
			//}
			nextLayout->mViewOwner->refreshChildrenViewIndex(0);
			nextLayout->judgeOverFlowCircle();
		}
		// idx之前能全部容纳，但新添加的对象不能全部容纳
		else if (canHaveNum <= idx + num)
		{
			// 将需要添加的部分子对象加到本行
			int canAddNum = canHaveNum - idx;
			if (canAddNum > 0)
			{
				addChildren(children, start, canAddNum, idx, false, 0);
			}
			// 如果自身为当前层的最有一行
			else if (mViewOwner == mViewOwner->getViewParent()->getLastChild(false))
			{
				replaceMgr->pushLayout(mViewOwner->getViewParent()->getLayout());
			}
			// 将剩余待添加子对象加入下一行
			tempStart = start + canAddNum;

			int tempEnd = end;
			ViewObj * txtLayer = mViewOwner->getViewParent();;
			ViewObj * tempView = NULL;
			BaseViewParam * tempParam = NULL;
			RowViewParam * rowParam = (RowViewParam *)mViewOwner->getViewParam();
			int rowEffectW = rowParam->getEffectW();
			float spendW = 0;
			//float spendW = rowParam->getEffectL();
			ViewObj * preView = (ViewObj *)mChildren->at(0);
			int tempAddRowIdx = mViewOwner->getViewIndex() + 1;
			int afterNextAddIdx = BaseConstants::MIN_INT;
			for (int i = tempStart; i < tempEnd; i++)
			{
				tempView = (ViewObj *)children->at(i);
				tempParam = tempView->getViewParam();
				int dis = 1;
				if (tempView->isEnter())
				{
					spendW = rowEffectW;
				}
				else if (tempView->isHorizontalOrEndPageLine())
				{
					spendW = rowEffectW;
					if (i > tempStart)
						dis = 0;
				}
				else if (tempView->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
				{
					spendW = rowEffectW;
					if (i > tempStart)
					{
						dis = 0;
					}
					// 如果不为最后一项
					if (i < tempEnd - 1)
					{
						if (children->at(i + 1)->isEnter())
						{
							i = i + 1;
							dis = 1;
						}
					}
				}
				else
					spendW += tempParam->getTotalW();
				if (spendW > rowEffectW)
				{
					dis = 0;
					tempView = preView;
				}
				if (spendW >= rowEffectW)
				{
					if (i == tempEnd - 1)
					{
						spendW = 0;
						if (i - tempStart + dis < 1)
						{
							nextLayout->addChildren(children, tempStart, end - tempStart, 0, true, 0);
							afterNextAddIdx = end - tempStart;
						}
						else
						{
							if (dis == 1)
							{
								nextLayout->addChildren(children, tempStart, i - tempStart + dis, 0, true, 0);
								afterNextAddIdx = i - tempStart + dis;
							}
							else
							{
								ViewObj * tempRow = ViewFactory::createRow(mViewOwner->getWriterMgr(), tempView->getParagraphParent());
								txtLayer->getLayout()->addChild(tempRow, tempAddRowIdx, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
								RowLayout * tempLayout = (RowLayout *)tempRow->getLayout();
								tempLayout->addChildren(children, tempStart, i - tempStart + dis, 0, true, BaseConstants::ReplaceType::REPLACE_NOW);
								tempAddRowIdx++;
								nextLayout->addChildren(children, i, 1, 0, true, 0);
								afterNextAddIdx = 1;
							}
							tempStart = i + dis;
						}
						break;
					}
					else if (tempView->isEnter())
					{
						spendW = 0;
						ViewObj * tempRow = ViewFactory::createRow(mViewOwner->getWriterMgr(), tempView->getParagraphParent());
						txtLayer->getLayout()->addChild(tempRow, tempAddRowIdx, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
						RowLayout * tempLayout = (RowLayout *)tempRow->getLayout();
						tempLayout->addChildren(children, tempStart, i - tempStart + dis, 0, true, BaseConstants::ReplaceType::REPLACE_NOW);
						tempAddRowIdx++;
						tempStart = i + dis;
						i = tempStart - 1;
					}
					else if (spendW >= rowEffectW)
					{
						spendW = 0;
						ViewObj * tempRow = ViewFactory::createRow(mViewOwner->getWriterMgr(), tempView->getParagraphParent());
						txtLayer->getLayout()->addChild(tempRow, tempAddRowIdx, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
						RowLayout * tempLayout = (RowLayout *)tempRow->getLayout();
						if (i > tempStart)
						{
							tempLayout->addChildren(children, tempStart, i - tempStart + dis, 0, true, BaseConstants::ReplaceType::REPLACE_NOW);
							tempAddRowIdx++;
							tempStart = i + dis;
							i = tempStart - 1;
						}
						else
						{
							i++;
							tempLayout->addChildren(children, tempStart, i - tempStart + dis, 0, true, BaseConstants::ReplaceType::REPLACE_NOW);
							tempAddRowIdx++;
							tempStart = i + dis;
							i = tempStart - 1;
						}
					}
					else
						spendW = 0;
				}
				preView = tempView;
				if (i == tempEnd - 1)
				{
					nextLayout->addChildren(children, tempStart, end - tempStart, 0, false, 0);
					afterNextAddIdx = end - tempStart;
				}
			}
			//nextLayout->addChildren(children, tempStart, end - tempStart, 0, false, 0);
			// 将本行后续子对象加入下一行
			tempStart = idx + canAddNum;
			size = mChildren->size();
			if (afterNextAddIdx == BaseConstants::MIN_INT)
			{
				nextLayout->addChildren(mChildren, tempStart, size - tempStart, 0, false, 0);
				//nextLayout->addChildren(mChildren, tempStart, size - tempStart, end - start - canAddNum, false, 0);
			}
			else
				nextLayout->addChildren(mChildren, tempStart, size - tempStart, afterNextAddIdx, false, 0);
			if (canAddNum > 0)
			{
				openReplace();
				mViewOwner->refreshChildrenViewIndex(0);
				replaceChildren();
			}

			nextLayout->judgeOverFlowCircle();
		}
		// 新添加的对象能全部容纳,但本行idx之后的子对象不能全部容纳
		else
		{
			// 将新对象全部添加到本行
			addChildren(children, start, end - start, idx, false, 0);
			//for (int i = start; i < end; i++)
			//{
			//	obj = (ViewObj *)children->at(i);
			//	addChild(obj, idx + i - start, false, BaseConstants::NONE, false);
			//}

			// 将本行不能容纳对象添加到下一行
			tempStart = idx + num;
			int size = mChildren->size();
			nextLayout->addChildren(mChildren, canHaveNum, size - canHaveNum, 0, false, 0);
			//for (int i = canHaveNum; i < size; i++)
			//{
			//	obj = (ViewObj *)mChildren->at(canHaveNum);
			//	nextLayout->addChild(obj, i - canHaveNum, false, BaseConstants::NONE, false);
			//}
			openReplace();
			mViewOwner->refreshChildrenViewIndex(0);
			replaceChildren();

			nextLayout->judgeOverFlowCircle();
		}
		ViewObj * txtLayer = nextRow->getViewParent();
		BaseLayout * txtLayout = txtLayer->getLayout();
		replaceMgr->pushLayout(txtLayout);
	}
}
void RowLayout::addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx, char needReplace)
{
	if (haveChildren() < 0)
	{
		createChildren();
	}
	openReplace();
	if (num < 0)
	{
		num = children->size();
	}
	if (idx < 0)
	{
		idx = mChildren->size();
	}
	int end = start + num;
	ViewObj * obj = NULL;
	int curSize = children->size();
	int preSize = curSize;
	int spendNum = 0;
	int tempIdx = start;
	for (int i = start; i < end; i++)
	{
		obj = (ViewObj *)children->at(i);
		mChildren->insert(idx + tempIdx - start, obj);
		obj->setParent(mViewOwner);
		if (obj->isHorizontalOrEndPageLine())
		{
			obj->getViewParam()->refreshWByParent(BaseConstants::NONE, false);
		}
		spendNum++;
		curSize = children->size();
		int dis = curSize - preSize;
		tempIdx++;
		if (dis != 0)
		{
			preSize = curSize;
			i += dis;
			if (spendNum >= num)
			{
				break;
			}
		}
	}
	if (refreshIdx)
		mViewOwner->refreshChildrenViewIndex(idx);
	if (needReplace != 0)
	{
		replaceChildren();
	}
}
int RowLayout::delChildren(int start, int num, bool refreshIdx, bool needReplace)
{
	openReplace();
	if (num < 0)
	{
		num = mChildren->size() - start;
	}

	int returnType = BaseChildrenManager::delChildren(start, num, refreshIdx);
	if (returnType < 0)
		return returnType;
	if (needReplace)
	{
		replaceChildren();
	}
	return returnType;
}
BaseLayout * RowLayout::getPreLayout(bool createWhenLast)
{
	ViewObj * dest = NULL;
	ViewObj * txtLayer = mViewOwner->getViewParent();
	ExcelCrossParentMgr * crossMgr = mViewOwner->getExcelCrossParentMgr();
	if (crossMgr && crossMgr->haveCrossParent())
	{
		txtLayer = crossMgr->getFirstCrossParent();
	}
	QList<BaseContainer *> * rowList = txtLayer->getChildren();
	int size = rowList->size();
	int idx = rowList->indexOf(mViewOwner);
	//int idx = mViewOwner->getViewIndex();

	BaseContainer * pagePanel = NULL;
	QList<BaseContainer *> * pageList = NULL;
	ViewObj * page = NULL;
	int pageIdx = -1;
	// 如果为第0项
	if (idx == 0)
	{
		// 如果为则为页眉、页脚
		if (!txtLayer->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
		{
			if (createWhenLast)
			{
				return createPreLayout();
			}
			else
				return NULL;
		}
		// 如果为页内容
		page = txtLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
		pagePanel = page->getViewParent();
		pageList = pagePanel->getChildren();
		pageIdx = page->getViewIndex();
		// 如果为第0页
		if (pageIdx == 0)
		{
			if (createWhenLast)
			{
				return createNextLayout();
			}
			else
				return NULL;
		}
		// 获取上一页最后一行
		page = (ViewObj *)pageList->at(pageIdx - 1);
		txtLayer = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
			->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
		dest = (ViewObj *)txtLayer->getLastChild(false);
	}
	else
		dest = (ViewObj *)rowList->at(idx - 1);
	return dest->getLayout();
}
BaseLayout * RowLayout::getNextLayout(bool createWhenLast)
{
	ViewObj * dest = NULL;
	ViewObj * txtLayer = mViewOwner->getViewParent();
	ExcelCrossParentMgr * crossMgr = mViewOwner->getExcelCrossParentMgr();
	if (crossMgr && crossMgr->haveCrossParent())
	{
		txtLayer = crossMgr->getLastCrossParent();
	}
	QList<BaseContainer *> * rowList = txtLayer->getChildren();
	int size = rowList->size();
	int idx = mViewOwner->getViewIndex();
	// 如果为最后一项
	if (idx == size - 1)
	{
		// 如果为则为页眉、页脚
		if (!txtLayer->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
		{
			if (createWhenLast)
			{
				return createNextLayout();
			}
			else
				return NULL;
		}
		// 如果为页内容

		ViewObj * page = txtLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
		BaseContainer * pagePanel = page->getViewParent();
		QList<BaseContainer *> * pageList = pagePanel->getChildren();
		int pageIdx = page->getViewIndex();

		// 如果为最后一页
		if (pageIdx == pageList->size() - 1)
		{
			if (createWhenLast)
			{
				return createNextLayout();
			}
			else
				return NULL;
		}
		// 获取下一页第0行
		page = (ViewObj *)pageList->at(pageIdx + 1);
		txtLayer = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
			->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
		if (txtLayer->haveChildren() > 0)
		{
			dest = (ViewObj *)txtLayer->getChild(0);
		}
		else if (createWhenLast)
		{
			return createNextLayout();
		}
		else
			return NULL;
	}
	else
		dest = (ViewObj *)rowList->at(idx + 1);
	return dest->getLayout();
}
void RowLayout::replaceChildren()
{
	if (!canReplace())
	{
		return;
	}
	else
		BaseLayout::replaceChildren();

	BaseParagraph * paragraph = getParagraph();
	ParagraphData * paragraphData = (ParagraphData *)paragraph->getContentData();
	int size = mChildren->size();
	ViewObj * obj = NULL;
	BaseViewParam * param = NULL;

	RowViewParam * rowParam = (RowViewParam *)mViewOwner->getViewParam();

	int paragraphInsideL = paragraphData->getInsideLeft();
	int paragraphInsideR = paragraphData->getInsideRight();
	int minY = rowParam->getInsideTop();

	char horizonAlignType = paragraphData->getHorizonAlignType();
	//int paragraphTopDis = paragraphData->getParagraphDisTop();
	//int paragraphBottomDis = paragraphData->getParagraphDisBottom();
	//float rowDisH = paragraphData->getRowDisHValue();

	int curY = minY;
	int insideL = rowParam->getEffectL();
	int insideW = rowParam->getEffectW();
	float maxChildrenW = 0;
	float startL = insideL;

	int maxH = -1;
	int curH = 0;
	// 如果不靠左
	if (horizonAlignType != BaseConstants::Direction::LEFT)
	{
		for (int i = 0; i < size; i++)
		{
			obj = (ViewObj *)mChildren->at(i);
			if (obj->isEnter())
			{
				continue;
			}
			param = obj->getViewParam();
			maxChildrenW += param->getTotalW();
		}
		// 居中
		if (horizonAlignType == BaseConstants::Direction::MIDDLE)
			startL = (insideW - maxChildrenW) / 2;
		// 靠右
		else if (horizonAlignType == BaseConstants::Direction::RIGHT)
			startL = (insideW - maxChildrenW);
	}

	for (int i = 0; i < size; i++)
	{
		obj = (ViewObj *)mChildren->at(i);
		param = obj->getViewParam();
		if (startL < insideL)
		{
			startL = insideL;
		}
		param->setX(startL);
		param->setY(minY);
		startL += param->getTotalW();
	}
	rowParam->refreshHByChildren(true);
}
RowLayout * RowLayout::judgeOverflow()
{
	int size = mChildren->size();
	ViewObj * obj = NULL;
	BaseViewParam * param = NULL;
	int canHaveNum = size;
	float curW = 0;

	float childrenW = mRowParam->getChildrenW();
	int effectW = mRowParam->getEffectW();
	// 先判断enter
	for (int i = 0; i < size; i++)
	{
		obj = (ViewObj *)mChildren->at(i);
		if (obj->isEnter())
		{
			canHaveNum = i + 1;
			break;
		}
		else if (obj->isHorizontalOrEndPageLine())
		{
			if (i > 0)
				canHaveNum = i;
			else
				canHaveNum = i + 1;
			break;
		}
		param = obj->getViewParam();
		if (obj->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
		{
			curW += effectW;
		}
		else
			curW += param->getTotalW();
		if (curW >= effectW)
		{
			if (i < 1)
			{
				if (size > 1 && mChildren->at(i + 1)->isEnter())
				{
					canHaveNum = 2;
				}
				else
					canHaveNum =  1;
			}
			else if (curW == effectW)
				canHaveNum = i + 1;
			else
				canHaveNum = i;
			break;
		}
	}
	if (canHaveNum < size)
	{
		RowLayout * nextLayout = (RowLayout *)getNextLayout(true);
		if (nextLayout->getViewOwner()->getViewIndex()==25)
		{
			int a = 0;
		}
		int tempNum = size - canHaveNum;
		// 判断如果当前行为跨页表格，则将表格本身从所跨第一页以外的页里删除，清空其跨页属性（因为插入内容后，表格可能缩进到其他行，本行可能不再跨页）
		if (mViewOwner->isExcelRow())
		{
			ExcelCrossParentMgr * crossMgr = mViewOwner->getExcelCrossParentMgr();
			if (crossMgr && crossMgr->haveCrossParent())
			{
				crossMgr->clearParentsWithoutSource(crossMgr->getFirstCrossParent());
			}
		}
		nextLayout->addChildren(mChildren, canHaveNum, tempNum, 0, true, 0);
		return nextLayout;
	}
	return NULL;
}
void RowLayout::judgeOverFlowCircle()
{
	RowLayout * tempLayout = NULL;
	RowLayout * layout = this;
	RowViewParam * rowParam = mRowParam;
	while (layout)
	{
		tempLayout = layout->judgeOverflow();
		layout->replaceChildren();
		if (!tempLayout)
			break;
		layout = tempLayout;
		rowParam = (RowViewParam *)layout->getViewOwner()->getViewParam();
	}
}
void RowLayout::judgeOverFlowCircleUntilObj(BaseContainer * last)
{
	RowLayout * tempLayout = NULL;
	RowLayout * layout = this;
	RowViewParam * rowParam = mRowParam;
	ViewObj * lastViewRow = NULL;
	ViewObj * lastTxtLayer = NULL;
	ViewObj * lastPage = NULL;
	BaseLayout * lastLayout = NULL;
	ViewObj * curRow = NULL;
	ViewObj * curTxtLayer = NULL;
	ViewObj * curPage = NULL;
	int curRowIdx = -1;
	int lastRowIdx = -1;
	int curPageIdx = -1;
	int lastPageIdx = -1;
	bool isPageUpsidown = false;
	while (layout)
	{
		tempLayout = layout->judgeOverflow();
		curRow = layout->getViewOwner();
		curRowIdx = curRow->getViewIndex();
		curTxtLayer = curRow->getViewParent();
		if (curTxtLayer->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
		{
			curPage = curTxtLayer->getViewParent()->getViewParent();
			curPageIdx = curPage->getViewIndex();
		}
		layout->replaceChildren();
		if (!tempLayout)
		{
			lastViewRow = last->getViewParent();
			lastRowIdx = lastViewRow->getViewIndex();
			lastLayout = lastViewRow->getLayout();

			lastTxtLayer = lastViewRow->getViewParent();
			if (lastTxtLayer->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
			{
				lastPage = lastTxtLayer->getViewParent()->getViewParent();
				lastPageIdx = lastPage->getViewIndex();
			}
			else if (lastTxtLayer->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER))
			{

				//continue;
			}
			else
				break;
			if (lastPageIdx < curPageIdx)
			{
				break;
			}
			else if (curPageIdx == lastPageIdx && lastRowIdx < curRowIdx)
			{
				break;
			}
			if (lastLayout == layout)
				break;
			layout->replaceChildren();
			layout = (RowLayout *)layout->getNextLayout(false);
			continue;
		}
		layout = tempLayout;
		rowParam = (RowViewParam *)layout->getViewOwner()->getViewParam();
	}
}
RowLayout * RowLayout::judgeLessThan(bool & haveIndent, bool needJudgeEnter)
{
	int size = mChildren->size();
	ViewObj * obj = (ViewObj *)mChildren->at(size - 1);
	if (needJudgeEnter && obj->isEnter())
	{
		return NULL;
	}
	if (obj->isHorizontalOrEndPageLine())
	{
		return NULL;
	}
	if (obj->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
	{
		return NULL;
	}
	float childrenW = mRowParam->getChildrenW();
	int effectW = mRowParam->getEffectW();
	float freeW = effectW - childrenW;
	RowLayout * nextLayout = (RowLayout *)getNextLayout(false);
	if (!nextLayout)
	{
		if (freeW < 0)
		{
			judgeOverFlowCircle();
		}
		return NULL;
	}
	QList<BaseContainer *> * nextChildren = nextLayout->getChildren();
	if (!nextChildren)
	{
		nextLayout->createChildren();
		nextChildren = nextLayout->getChildren();
	}
	size = nextChildren->size();
	BaseViewParam * param = NULL;
	float spendW = 0;
	int indentNum = 0;
	for (int i = 0; i < size; i++)
	{
		obj = (ViewObj *)nextChildren->at(i);
		if (obj->isEnter())
		{
			indentNum++;
			break;
		}
		else if (obj->isHorizontalOrEndPageLine())
		{
			//if (i > 0)
				indentNum = i;
			//else
			//	indentNum = i + 1;
			break;
		}
		param = obj->getViewParam();
		if (obj->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
		{
			spendW = effectW;
		}
		else
			spendW = param->getTotalW();
		if (spendW < freeW)
		{
			indentNum++;
			freeW -= spendW;
		}
		else if (spendW == freeW)
		{
			indentNum++;
			break;
		}
		else
		{
			break;
		}
	}
	// 如果不需要缩进，直接返回NULL
	if (indentNum < 1)
	{
		return NULL;
	}
	haveIndent = true;
	// 如果下一行未缩进完
	if (indentNum < size)
	{
		ViewObj * nextRow = nextLayout->getViewOwner();
		if (nextRow->isExcelRow())
		{
			ExcelCrossParentMgr * crossMgr = nextRow->getExcelCrossParentMgr();
			if (crossMgr && crossMgr->haveCrossParent())
			{
				crossMgr->clearParentsWithoutSource(nextRow);
			}
		}
		addChildren(nextChildren, 0, indentNum, -1, true, BaseConstants::ReplaceType::REPLACE_NOW);
		nextLayout->openReplace();
	}
	// 如果下一行全部缩进
	else
	{
		nextLayout->closeReplace();
		ViewObj * nextRow = nextLayout->getViewOwner();
		if (nextRow->isExcelRow())
		{
			ExcelCrossParentMgr * crossMgr = nextRow->getExcelCrossParentMgr();
			if (crossMgr && crossMgr->haveCrossParent())
			{
				crossMgr->clearParentsWithoutSource(nextRow);
			}
		}
		ViewObj * txtLayer = nextRow->getViewParent();
		QList<BaseContainer *> * rowList = txtLayer->getChildren();
		int nextRowIdx = nextRow->getViewIndex();
		int size = rowList->size();

		//ExcelCrossParentMgr * crossMgr = NULL;
		ViewObj * pagePanel = txtLayer->findViewFromParent(ConstantsID::ContainerId::PAGE_PANEL, true);
		//if (nextRow->isExcelRow())
		//{
		//	crossMgr = nextRow->getExcelCrossParentMgr();
		//	if (crossMgr)
		//	{
		//		QList<ViewObj *> * crossParents = crossMgr->getCrossParents();
		//		ViewObj * tempLayer = crossParents->at(0);
		//		ViewObj * page = tempLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
		//		if (tempLayer->getChildren()->size() == 1)
		//		{
		//			pagePanel->getLayout()->delChild(page, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		//		}
		//		else
		//			tempLayer->getLayout()->delChild(nextRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);

		//		// 删除中间页
		//		for (int i = 1; i < crossParents->size() - 1; i ++)
		//		{
		//			tempLayer = crossParents->at(i);
		//			page = tempLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
		//			pagePanel->getLayout()->delChild(page, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		//		}
		//		// 尾页
		//		tempLayer = crossParents->at(i);
		//		page = tempLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
		//	}
		//}

		addChildren(nextLayout->getChildren(), 0, -1, -1, true, 0);


		// 如果仅剩1行
		if (size == 1)
		{
			ViewObj * page = txtLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
			// 删除对应页
			pagePanel->getLayout()->delChild(page, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		}
		// 大于1行
		else
		{
			txtLayer->getLayout()->delChild(nextRow, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
		}
		// 继续判断自身缩进
		return this;
	}

	return nextLayout;
}
bool RowLayout::judgeLessThanCircleUntilObj(BaseContainer * last)
{
	RowLayout * layout = this;
	RowLayout * tempLayout = NULL;
	bool haveNextIndent = false;
	ViewObj * lastViewRow = NULL;
	BaseLayout * lastLayout = NULL;
	ViewObj * lastTxtLayer = NULL;
	ViewObj * lastPage = NULL;

	ViewObj * curRow = NULL;
	ViewObj * curTxtLayer = NULL;
	ViewObj * curPage = NULL;
	int curRowIdx = -1;
	int lastRowIdx = -1;
	int curPageIdx = -1;
	int lastPageIdx = -1;
	bool isTxtLayer = false;

	if (mViewOwner->getViewParent()->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
		isTxtLayer = true;

	while (layout)
	{
		tempLayout = layout->judgeLessThan(haveNextIndent, true);

		curRow = layout->getViewOwner();
		curRowIdx = curRow->getViewIndex();
		curTxtLayer = curRow->getViewParent();
		curPage = curTxtLayer->getViewParent()->getViewParent();
		curPageIdx = curPage->getViewIndex();
		if (!tempLayout)
		{
			lastViewRow = last->getViewParent();
			lastRowIdx = lastViewRow->getViewIndex();
			lastLayout = lastViewRow->getLayout();

			lastTxtLayer = lastViewRow->getViewParent();
			lastPage = lastTxtLayer->getViewParent()->getViewParent();
			lastPageIdx = lastPage->getViewIndex();
			if (isTxtLayer)
			{
				if (lastPageIdx < curPageIdx)
				{
					break;
				}
			}
			if (lastPageIdx == curPageIdx && lastRowIdx < curRowIdx)
			{
				break;
			}
			if (lastLayout == layout)
				break;
			layout->replaceChildren();
			layout = (RowLayout *)layout->getNextLayout(false);
			continue;
		}
		if (layout != tempLayout)
		{
			layout->replaceChildren();
			layout = tempLayout;
		}
	}
	if (layout)
	{
		layout->replaceChildren();
	}
	return haveNextIndent;
}
bool RowLayout::judgeLessThanCircle(bool needJudgeEnter)
{
	RowLayout * layout = this;
	RowLayout * tempLayout = NULL;
	bool haveNextIndent = false;
	while (layout)
	{
		tempLayout = layout->judgeLessThan(haveNextIndent, needJudgeEnter);
		if (!tempLayout)
			break;
		if (layout != tempLayout)
		{
			layout->replaceChildren();
			layout = tempLayout;
		}
	}
	if (layout)
		layout->replaceChildren();
	return haveNextIndent;
}