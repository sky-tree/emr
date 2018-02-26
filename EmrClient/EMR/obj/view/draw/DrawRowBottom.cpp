#include "DrawRowBottom.h"

#include "ViewObj.h"
#include "BaseViewParam.h"
#include "RowViewParam.h"

#include "StrContentData.h"

#include "OperateMgr.h"
#include "SystemParams.h"

#include "ConstantsID.h"
#include "UIConstants.h"
#include "BaseConstants.h"
#include "ConstantsNumber.h"

#include "BaseMath.h"
#include "BaseTools.h"

DrawRowBottom::DrawRowBottom(ViewObj * viewOwner) : BaseDraw(viewOwner, ConstantsID::ClassId::DRAW_ROW_BOTTOM)
{
}

DrawRowBottom::~DrawRowBottom()
{
}
void DrawRowBottom::drawRound(QPainter &pPainter, RowViewParam * rowParam, int x, int y)
{
	OperateMgr * mgr = mViewOwner->getOperateMgr();
	SystemParams * sys = mgr->getSystemParams();
	char rowLineType = sys->getRowBottomLineType();
	if (rowLineType == BaseConstants::NONE)
	{
		return;
	}
	if (mViewOwner->isInDest(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
	{
		return;
	}

	if (!mViewOwner->isInDest(ConstantsID::ContainerId::PAGE_CONTENT))
	{
		return;
	}
	int w = rowParam->getTotalW();
	int h = rowParam->getTotalH();

	//BaseTools::setLineType(pPainter, UIConstants::MyColor::BLACK_COLOR_OBJ, 1, UIConstants::LineType::SINGLE_LINE);
	//pPainter.drawRect(x, y, w, h);


	//BaseTools::drawLines(pPainter, UIConstants::MyColor::BLACK_COLOR_OBJ, 1, UIConstants::LineType::SINGLE_DOT_LINE, x, y + h, x + w, y + h);
	BaseTools::drawLines(pPainter, UIConstants::MyColor::BLACK_COLOR_OBJ, 1, rowLineType, x, y + h, x + w, y + h);
}

void DrawRowBottom::drawTxtBackgroundColor(QPainter &pPainter, RowViewParam * rowParam, int x, int y)
{
	QList<BaseContainer *> * children = mViewOwner->getChildren();
	ViewObj * obj = NULL;
	int size = children->size();
	int maskColor = 0;
	int preColor = 0;
	int start = -1;
	int end = -1;
	StrContentData * txtData = NULL;
	for (int i = 0; i < size; i++)
	{
		obj = (ViewObj *)children->at(i);
		if (!obj->isTxt())
		{
			if (start >= 0)
			{
				end = i;
				BaseTools::drawChildrenBackground(pPainter, rowParam, x, y, children, start, end, preColor);
				start = -1;
				end = -1;
				preColor = 0;
			}
			continue;
		}
		txtData = (StrContentData *)obj->getContentData();
		maskColor = txtData->getBackgroundColor();
		// 如果有填充色
		if (maskColor != 0)
		{
			if (preColor != 0 && preColor != maskColor)
			{
				end = i;
				BaseTools::drawChildrenBackground(pPainter, rowParam, x, y, children, start, end, preColor);
				start = -1;
				end = -1;
				preColor = 0;
				i--;
				continue;
			}
			if (start < 0)
			{
				start = i;
				preColor = maskColor;
			}
			if (i == size - 1)
			{
				BaseTools::drawChildrenBackground(pPainter, rowParam, x, y, children, start, size, maskColor);
				break;
			}
		}
		else if (start >= 0)
		{
			end = i;
			BaseTools::drawChildrenBackground(pPainter, rowParam, x, y, children, start, end, preColor);
			start = -1;
			end = -1;
			preColor = 0;
		}
	}
}
void DrawRowBottom::drawChildrenUnderline(QPainter &pPainter, RowViewParam * rowParam, int x, int y)
{
	QList<BaseContainer *> * children = mViewOwner->getChildren();
	ViewObj * obj = NULL;
	int size = children->size();
	int start = -1;
	int end = -1;
	StrContentData * txtData = NULL;
	char type = -1;
	int thick = 0;
	char preType = 0;
	int preThick = 0;
	bool needRefreshPre = false;
	for (int i = 0; i < size; i++)
	{
		needRefreshPre = false;
		obj = (ViewObj *)children->at(i);
		if (!obj->isTxt())
		{
			if (start >= 0)
			{
				end = i;
				BaseTools::drawChildrenUnderline(pPainter, rowParam, x, y, children, start, end, preType, preThick);
				start = -1;
				end = -1;
				preType = 0;
				preThick = 0;
			}
			continue;
		}
		txtData = (StrContentData *)obj->getContentData();
		type = txtData->getUnderLineType();
		thick = txtData->getUnderLineThick();
		// 如果有填充色
		if (type != 0)
		{
			if (start < 0)
			{
				needRefreshPre = true;
				start = i;
			}
			if (preType != 0 && (preType != type || preThick != thick))
			{
				end = i;
				BaseTools::drawChildrenUnderline(pPainter, rowParam, x, y, children, start, end, preType, preThick);
				start = -1;
				end = -1;
				preType = type;
				preThick = thick;
				i--;
				continue;
			}
			if (start < 0 || needRefreshPre)
			{
				start = i;
				preType = type;
				preThick = thick;
			}
			if (i == size - 1)
			{
				BaseTools::drawChildrenUnderline(pPainter, rowParam, x, y, children, start, size, type, thick);
				break;
			}
		}
		else if (start >= 0)
		{
			end = i;
			BaseTools::drawChildrenUnderline(pPainter, rowParam, x, y, children, start, end, preType, preThick);
			start = -1;
			end = -1;
			preType = 0;
			preThick = 0;
		}
	}
}
bool DrawRowBottom::draw(QPainter &pPainter, int x, int y, int w, int h)
{
	RowViewParam * rowParam = (RowViewParam *)mViewOwner->getViewParam();
	// 画行背景色及底框
	drawRound(pPainter, rowParam, x, y);
	// 画文字背景填充色
	drawTxtBackgroundColor(pPainter, rowParam, x, y);

	
	// 文字下划线
	drawChildrenUnderline(pPainter, rowParam, x, y);

	return false;
}