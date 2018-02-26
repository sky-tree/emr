#include "DrawLine.h"
#include "ConstantsID.h"
#include "BaseViewParam.h"
#include "ViewObj.h"
#include "BaseMath.h"
#include "UIConstants.h"
#include <QColor>
DrawLine::DrawLine(ViewObj * viewOwner) : BaseDraw(viewOwner,ConstantsID::ClassId::DRAW_LINE)
,mLineColor(NULL)
{
}


DrawLine::~DrawLine()
{
	if (mLineColor)
	{
		delete mLineColor;
		mLineColor = NULL;
	}
}
void DrawLine::setLineParam(int lineColor, int lineWidth, char type)
{
	if (mLineColor)
	{
		delete mLineColor;
	}
	mLineColor = BaseMath::createColor(lineColor);
	mLineWidth = lineWidth;
	mPenStyle = type;
	
}
bool DrawLine::draw(QPainter &pPainter, int x, int y, int w1, int h1)
{
	BaseViewParam * viewParam = mViewOwner->getViewParam();
	int w = viewParam->getTotalW();
	int h = viewParam->getTotalH();


	
	
		QPen pen;  // creates a default pen  
		if (mPenStyle == UIConstants::LineType::SINGLE_LINE)
		{
			pen.setStyle(Qt::SolidLine);
		}
		else if (mPenStyle == UIConstants::LineType::SINGLE_DOT_LINE)
		{
			pen.setStyle(Qt::DotLine);
		}
		else if (mPenStyle == UIConstants::LineType::DASH_DOT_LINE)
		{
			pen.setStyle(Qt::DashDotLine);
		}
		pen.setColor(*mLineColor);
		pen.setWidth(mLineWidth);
		//pen.setCapStyle(Qt::RoundCap);
		//pen.setJoinStyle(Qt::RoundJoin);
		pPainter.setPen(pen);
		pPainter.drawLine(x, y, x + w, y + h);

	
		pPainter.drawLine(x, y, x + w, y + h);
	
	//pPainter.set
	//pPainter.drawLine();
	return false;
}
