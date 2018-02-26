#include "DrawRowTop.h"




#include "OperateMgr.h"
#include "SystemParams.h"

#include "ViewObj.h"
#include "BaseViewParam.h"
#include "RowViewParam.h"

#include "BaseParagraph.h"

#include "ExcelCrossParentMgr.h"


#include "ConstantsID.h"
#include "UIConstants.h"
#include "BaseConstants.h"
#include "ConstantsNumber.h"

#include "BaseMath.h"
#include "BaseTools.h"

DrawRowTop::DrawRowTop(ViewObj * viewOwner) : BaseDraw(viewOwner, ConstantsID::ClassId::DRAW_ROW_TOP)
{
}

DrawRowTop::~DrawRowTop()
{
}

void DrawRowTop::drawChildrenMask(QPainter &pPainter, int x, int y, QList<BaseContainer *> * children)
{
	ViewObj * obj = NULL;
	BaseViewParam * param = NULL;
	int size = children->size();
	int maskColor = 0;
	int preColor = 0;
	int start = -1;
	int end = -1;
	RowViewParam * rowParam = (RowViewParam *)mViewOwner->getViewParam();
	for (int i = 0; i < size; i++)
	{
		obj = (ViewObj *)children->at(i);
		maskColor = obj->getMaskColor();
		// 如果有填充色
		if (maskColor != 0)
		{
			preColor = maskColor;
			if (start < 0)
			{
				start = i;
			}
			if (i == size - 1)
			{
				BaseTools::drawChildrenBackground(pPainter, rowParam, x, y, children, start, size, maskColor);
				//drawChildrenMask(pPainter, x, y, children, start, size);
				break;
			}
		}
		else if (start >= 0)
		{
			end = i;
			BaseTools::drawChildrenBackground(pPainter, rowParam, x, y, children, start, end, preColor);
			//drawChildrenMask(pPainter, x, y, children, start, end);
			start = -1;
			end = -1;
		}
	}
}
void DrawRowTop::drawParagraphNum(QPainter &pPainter, int x, int y)
{
	bool isParagraphNumView = mOwner->getOperateMgr()->getSystemParams()->isParagraphNumView();
	if (!isParagraphNumView)
		return;
	ViewObj * layer = mViewOwner->getViewParent();
	// 如果为页眉、页脚层的行，则不显示行号
	if (layer->isObj(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER))
	{
		return;
	}
	// 如果为表格层的行，则不显示行号
	if (layer->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER))
	{
		return;
	}
	// 第一个子对象
	ViewObj * firstChild = (ViewObj *)mOwner->getChild(0);
	// 第一个子对象所属段落
	BaseParagraph * paragraph = firstChild->getParagraphParent();
	// 第一个子对象在段落中的索引
	int childParagraphIndex = firstChild->getParagraphIndex();
	// 如果子对象不为段落首个view，则不绘制段落号
	if (childParagraphIndex > 0)
	{
		return;
	}
	// 行所属的段落号
	int paragraphIdx = paragraph->getIndexFromParagraphMgr();

	QFont font = pPainter.font();
	font.setPointSize(UIConstants::RowParams::PARAGRAPH_NUMBER_FONT_SIZE);
	font.setWeight(UIConstants::RowParams::PARAGRAPH_NUMBER_FONT_WEIGHT);
	pPainter.setFont(font);
	pPainter.setPen(*UIConstants::MyColor::PARAGRAPH_NUM_COLOR_OBJ);

	RowViewParam * rowParam = (RowViewParam *)mViewOwner->getViewParam();

	QFontMetrics * graphics = new QFontMetrics(font);
	int strH = graphics->height();
	int strAscent = graphics->xHeight();
	int rowDescent = rowParam->getDescent();
	//int disH = mOwner->mViewData->getChildrenDisH();
	int insideTop = rowParam->getInsideTop();
	int insideH = rowParam->getInsideH();
	//if (rowDescent == 0)
	//{
	//	rowDescent = rowH - strAscent;
	//}

	int destY = y + insideTop + insideH - rowDescent;
	QString str = QString::number(paragraphIdx + 1);
	int strW = graphics->width(str);
	pPainter.drawText(x + UIConstants::RowParams::PARAGRAPH_NUMBER_FONT_DIS_X - strW, destY, str);

	delete graphics;
	graphics = NULL;
}
void DrawRowTop::drawExcelCrossPageLine(QPainter &pPainter, int x, int y)
{
	if (mViewOwner->isExcelRow())
	{
		ExcelCrossParentMgr * crossMgr = mViewOwner->getExcelCrossParentMgr();
		if (crossMgr && crossMgr->haveCrossParent())
		{
			LayoutViewParam * viewParam = (LayoutViewParam *)mViewOwner->getViewParam();
			int rowTop = viewParam->getY();
			int rowBottom = rowTop + viewParam->getTotalH();
			int insideL = viewParam->getInsideLeft();
			int w = viewParam->getInsideW();
			ViewObj * page = mViewOwner->getOperateMgr()->getEffectLoopPage();
			ViewObj * txtLayer = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			LayoutViewParam * pageLayoutViewParam = (LayoutViewParam *)txtLayer->getViewParam();
			int pageY = pageLayoutViewParam->getAbsY(NULL);
			int pageBottom = pageY + pageLayoutViewParam->getInsideTop() + pageLayoutViewParam->getInsideH();

			QPen pen;  // creates a default pen  
			pen.setStyle(Qt::SolidLine);
			//QColor color(UIConstants::MyColor::BLACK);
			pen.setColor(*UIConstants::MyColor::BLACK_COLOR_OBJ);
			pPainter.setPen(pen);
			if (y < pageY)
			{
				pPainter.drawLine(x + insideL, pageY + 0, x + insideL + w, pageY + 0);
			}
			if (y + rowBottom > pageBottom)
			{
				pPainter.drawLine(x + insideL, pageBottom - 0, x + insideL + w, pageBottom - 0);
			}
		}
	}
}
bool DrawRowTop::draw(QPainter &pPainter, int x, int y, int w, int h)
{
	QList<BaseContainer *> * children = mViewOwner->getChildren();
	// 绘制选中遮挡
	drawChildrenMask(pPainter, x, y, children);
	// 绘制段落号
	drawParagraphNum(pPainter, x, y);

	// 绘制表格跨页边框线
	//drawExcelCrossPageLine(pPainter, x, y);
	return false;
}