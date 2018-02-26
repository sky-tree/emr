#include "DrawHorizontalLine.h"

#include "OperateMgr.h"
#include "SystemParams.h"

#include "ViewObj.h"
#include "BaseViewParam.h"



#include "UIConstants.h"
#include "StrConstants.h"

#include "BaseTools.h"
#include "BaseSystem.h"

DrawHorizontalLine::DrawHorizontalLine(ViewObj * viewOwner, char type):BaseDraw(viewOwner, ConstantsID::ClassId::DRAW_HORIZONTAL_LINE)
{

	int objID = mViewOwner->getObjID();
	switch (objID)
	{
	case ConstantsID::ContainerId::HORIZONTAL_LINE:
		mStr = StrConstants::HORIZONTAL_LINE_STR;
		break;
	case ConstantsID::ContainerId::END_PAGE_LINE:
		mStr = StrConstants::END_PAGE_LINE_STR;
		break;
	}

	mDisStrX = -1;
	mStrW = -1;
	mStrAscent = -1;
	mPreHorizontalPage = mViewOwner->getOperateMgr()->getSystemParams()->isPaperHorizontal();
}
DrawHorizontalLine::DrawHorizontalLine(ViewObj * owner, DrawHorizontalLine * otherDraw) : BaseDraw(owner, otherDraw)
{
	mDisStrX = otherDraw->mDisStrX;
	mStrW = otherDraw->mStrW;
	mStrAscent = otherDraw->mStrAscent;
	mStr = otherDraw->mStr;
	mPreHorizontalPage = mViewOwner->getOperateMgr()->getSystemParams()->isPaperHorizontal();
	//if (!isHorizontalPage)
	//{
	//	mPreHorizontalPage = 0;
	//}
	//else
	//	mPreHorizontalPage = 1;
}


DrawHorizontalLine::~DrawHorizontalLine()
{
}

bool DrawHorizontalLine::draw(QPainter &pPainter, int x, int y, int w, int h)
{
	ViewObj * parent = (ViewObj*)mViewOwner->findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, false);
	BaseViewParam * viewParam = parent->getViewParam();
	int destW = viewParam->getSelfW();

	BaseSystem * baseSys = BaseSystem::getInstance();
	StrContentData * defaultTxtData = baseSys->getDefaultStrData();
	bool isHorizontalPage = mViewOwner->getOperateMgr()->getSystemParams()->isPaperHorizontal();
	if (mStrW == -1 || mPreHorizontalPage != isHorizontalPage)
	{
		mPreHorizontalPage = isHorizontalPage;
		baseSys->refreshPainterFont(pPainter, defaultTxtData);
		QFontMetrics  graphics(*baseSys->getFont());
		mStrW = graphics.width(mStr);
		mDisStrX = (destW - mStrW) / 2;
		mStrAscent = graphics.ascent();
	}
	else
		baseSys->refreshPainterFont(pPainter, defaultTxtData);

	int strX = x + mDisStrX;
	int bottomY = y + UIConstants::HORIZONTAL_LINE_OFFSET_HEIGHT;
	// ÎÄ×Ö
	pPainter.drawText(strX, y + mStrAscent, mStr);
	// ×ó²àÏß
	BaseTools::drawLines(pPainter, UIConstants::MyColor::BLACK_COLOR_OBJ, 1, UIConstants::LineType::SINGLE_LINE
		, x, bottomY, strX - 5, bottomY);
	// ÓÒ²àÏß
	BaseTools::drawLines(pPainter, UIConstants::MyColor::BLACK_COLOR_OBJ, 1, UIConstants::LineType::SINGLE_LINE
		, strX + mStrW + 5, bottomY, x + destW, bottomY);
	return true;
}

