#include "TxtWinDrawManager.h"




#include "OperateMgr.h"
#include "FileData.h"

#include "CrossParentMgr.h"
#include "ExcelCrossParentMgr.h"

#include "BaseViewParam.h"
#include "RowViewParam.h"
#include "FlagViewParam.h"
#include "LayoutViewParam.h"
#include "VerticalViewParam.h"

#include "TxtWinData.h"

#include "WriterManager.h"
#include "OperateMgr.h"

#include "BaseDraw.h"
#include "ViewObj.h"
#include "PageContentTxtLayerLayout.h"

#include "BaseConstants.h"
#include "ConstantsID.h"
#include "UIConstants.h"
#include "ConstantsNumber.h"
#include "AuthorityConstants.h"

TxtWinDrawManager::TxtWinDrawManager(ViewObj * owner) : ObjDrawManager(owner, ConstantsID::ClassId::TXT_WINDOW_DRAW_MANAGER)
{
	mWinViewParam = NULL;
	mFlag = NULL;
}

TxtWinDrawManager::~TxtWinDrawManager()
{
}


void TxtWinDrawManager::drawFlag(QPainter &pPainter, int x, int y, bool needJudgeInsideWindow)
{
	//int authority = mOwner->getOperateMgr()->getFileData()->getFileAuthority();
	//if (mOwner->getOperateMgr()->getFileData()->isReadOnly())
	//{
	//	return;
	//}

	OperateMgr * operateMgr = mViewOwner->getWriterMgr()->getOperateMgr();
	if (!mWinViewParam)
	{
		mWinViewParam = mViewOwner->getViewParam();
		mFlag = operateMgr->getFlag();
	}
	if (mWinViewParam->getViewType() < BaseConstants::ViewType::NORMAL)
	{
		return;
	}

	x += mWinViewParam->getX();
	y += mWinViewParam->getY();

	FlagViewParam * flagParam = (FlagViewParam *)mFlag->getViewParam();
	ViewObj * focus = flagParam->getFocus();
	BaseViewParam * focusParam = focus->getViewParam();
	char dir = flagParam->getDirection();
	float objH = focusParam->getTotalH();
	ViewObj * row = focus->findParentRow(false);
	BaseViewParam * rowParam = row->getViewParam();
	float rowH = rowParam->getTotalH();
	ViewObj * layer = row->getViewParent();
	int * winClip = mWinViewParam->getClip();
	int winClipY = mWinViewParam->getX() + winClip[1];


	// 如果为页眉、页脚层
	if (layer->isObj(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER))
	{
		ViewObj * titleOrBottom = layer->getViewParent();
		BaseViewParam * titleOrBottomParam = titleOrBottom->getViewParam();
		// 先计算相对页坐标
		x += focusParam->getAbsX(titleOrBottom);
		y += focusParam->getAbsY(titleOrBottom);
		// 加上焦点页坐标
		ViewObj * page = mOwner->getOperateMgr()->getEffectLogicPage();
		//ViewObj * page = titleOrBottom->getCrossParentMgr()->getActiveParent();
		BaseViewParam * pageParam = page->getViewParam();
		x += pageParam->getAbsX(mViewOwner);
		y += pageParam->getAbsY(mViewOwner);
	}
	// 如果为页内容
	else if (layer->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
	{
		bool isCross = false;
		// 如果行里有表格
		if (row->isExcelRow())
		{
			ExcelCrossParentMgr * crossMgr = row->getExcelCrossParentMgr();
			if (crossMgr && crossMgr->haveCrossParent())
			{
				// x项未有跨页偏移，所以直接取focusParam
				x += focusParam->getAbsX(mViewOwner);
				if (dir == BaseConstants::Direction::RIGHT)
				{
					x += focusParam->getTotalW();
				}
				isCross = true;
				QList<ViewObj *> * crossParents = crossMgr->getCrossParents();
				int size = crossParents->size();
				int disY = 0;
				int spendH = 0;
				int tempY = 0;
				BaseViewParam * layerParam = NULL;
				BaseViewParam * rowParam = NULL;
				for (int i = 0; i < size; i++)
				{
					tempY = y;
					ViewObj * layer = crossParents->at(i);
					if (i == 0)
						disY = crossMgr->getDisY(layer);
					else
						disY = 0;
					spendH = crossMgr->getSpendH(layer);
					layerParam = layer->getViewParam();
					rowParam = row->getViewParam();
					tempY += layerParam->getAbsY(mViewOwner) + disY;

					flagParam->setSelfH(spendH);
					mFlag->getDrawMgr()->draw(pPainter, x, tempY, needJudgeInsideWindow);
				}
				return;
			}
		}
		if (!isCross)
		{
			x += focusParam->getAbsX(mViewOwner);
			y += focusParam->getAbsY(mViewOwner);
		}
	}
	// 如果为表格页内容
	else if (layer->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER))
	{
		bool isCross = false;
		// 如果为表格行
		ViewObj * parentRow = row->findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, true);
		RowViewParam * parentRowParam = (RowViewParam *)parentRow->getViewParam();
		if (parentRow->isExcelRow())
		{
			ExcelCrossParentMgr * crossMgr = parentRow->getExcelCrossParentMgr();
			if (crossMgr && crossMgr->haveCrossParent())
			{
				QList<ViewObj *> * crossParents = crossMgr->getCrossParents();
				int size = crossParents->size();

				x += focusParam->getAbsX(mViewOwner);
				if (dir == BaseConstants::Direction::RIGHT)
				{
					x += focusParam->getTotalW();
				}
				isCross = true;
				ViewObj * curDrawPage = operateMgr->getEffectLogicPage();
				ViewObj * layer = (ViewObj *)curDrawPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
					->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
				// 先算出相对于容纳表格的行的坐标
				int disY = focusParam->getAbsY(parentRow);
				int preDisY = disY;
				// 容纳表格行相对于层的y
				disY += parentRowParam->getCrossY(layer);
				// 本页无内容，需要取上一页
				//if (disY + rowH < 0)
				//{
				//	PageContentTxtLayerLayout * layout = (PageContentTxtLayerLayout *)layer->getLayout();
				//	PageContentTxtLayerLayout * preLayout = layout->getPreLayout();
				//	layer = preLayout->getViewOwner();
				//	disY = preDisY + parentRowParam->getCrossY(layer);
				//	operateMgr->setEffectLogicPage(layer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true));
				//}
				int spendH = 0;
				int tempY = 0;
				VerticalViewParam * layerParam = NULL;
				tempY = y;

				layerParam = (VerticalViewParam *)layer->getViewParam();
				tempY += layerParam->getAbsY(mViewOwner) + disY;

				flagParam->setSelfH(rowH);
				//mFlag->getDrawMgr()->draw(pPainter, x, tempY, needJudgeInsideWindow);

				for (int i = 0; i < size; i++)
				{
					tempY = y;
					ViewObj * layer = crossParents->at(i);
					disY = preDisY + parentRowParam->getCrossY(layer);
					//if (i == 0)
					//	disY = crossMgr->getDisY(layer);
					//else
					//	disY = 0;
					spendH = crossMgr->getSpendH(layer);
					layerParam = (VerticalViewParam *)layer->getViewParam();
					rowParam = row->getViewParam();
					int bottom = disY + rowH;
					int layerBottom = layerParam->getInsideTop() + layerParam->getInsideH();
					int selfH = rowH;
					if (disY < 0)
					{
						selfH += disY;
						disY = 0;
					}
					else if (bottom > layerBottom)
					{
						selfH -= bottom - layerBottom;
					}
					if (selfH < 0)
					{
						continue;
					}
					tempY = y + layerParam->getAbsY(mViewOwner) + disY;

					if (tempY < winClipY)
					{
						selfH -= winClipY - tempY;
						tempY = winClipY;
					}
					if (selfH < 0)
					{
						continue;
					}
					flagParam->setSelfH(selfH);
					mFlag->getDrawMgr()->draw(pPainter, x, tempY, needJudgeInsideWindow);
				}
				return;
			}
		}
		if (!isCross)
		{
			ViewObj * parentLayer = layer->findViewFromParent(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW, true)
				->getViewParent()->getViewParent();
			ViewObj * pagePart = parentLayer->getViewParent();
			BaseViewParam * partParam = pagePart->getViewParam();
			//int partY = partParam->getY();
			int focusY = focusParam->getAbsY(pagePart);

			x += focusParam->getAbsX(mViewOwner);
			if (parentLayer->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
			{
				y += focusParam->getAbsY(mViewOwner);
			}
			else
			{
				ViewObj * focusPage = operateMgr->getEffectLogicPage();
				BaseViewParam * pageParam = focusPage->getViewParam();
				int pageY = pageParam->getAbsY(mViewOwner);
				y += pageY + focusY;
			}
		}
	}
	if (dir == BaseConstants::Direction::RIGHT)
	{
		x += focusParam->getTotalW();
	}
	int dis = y - UIConstants::UI_TOOLBAR_H;
	// 判断最小越界
	if (dis < 0)
	{
		y -= dis;
		rowH += dis;
	}
	// 判断最大越界
	else
	{
		LayoutViewParam * viewParam = (LayoutViewParam *)mViewOwner->getViewParam();
		int winH = viewParam->getTotalH();
		int * clip = viewParam->getClip();
		int maxY = viewParam->getY() + winH - clip[3];
		if (y > maxY)
		{
			return;
		}
		if (y + rowH > maxY)
		{
			rowH = maxY - y;
		}
	}
	if (y < winClipY)
	{
		rowH -= winClipY - y;
		y = winClipY;
	}
	if (rowH < 0)
	{
		return;
	}
	if (rowH > 0)
	{
		//rowH = UIConstants::UI_TOOLBAR_H;
		flagParam->setSelfH(rowH);
		mFlag->getDrawMgr()->draw(pPainter, x, y, needJudgeInsideWindow);
	}
}
int TxtWinDrawManager::draw(QPainter &pPainter, int x, int y, bool needJudgeInsideWindow)
{
	int returnType = ObjDrawManager::draw(pPainter, x, y, needJudgeInsideWindow);

	drawFlag(pPainter, x, y, needJudgeInsideWindow);

	return returnType;
}
