#include "DrawFactory.h"



#include "UIConstants.h"
#include "DrawRect.h"
#include "DrawImage.h"
#include "DrawStr.h"
//#include "DrawLine.h"
//#include "DrawDirectLine.h"
//#include "DrawCircle.h"
#include "DrawRowTop.h"

//#include "BaseContainerData.h"
//#include "DrawRectParam.h"
//#include "DrawImageParam.h"
//#include "DrawTxtParam.h"
//#include "FocusTxtData.h"

//#include "DrawFreeLineParam.h"
//#include "DrawFreeLine.h"

//#include "DrawBaseImgButtonParam.h"
#include "ObjDrawManager.h"
#include "BaseViewParam.h"

#include "ViewObj.h"
//#include "DrawManager.h"
//#include "NormalRowParam.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

DrawFactory::DrawFactory()
{
}

DrawFactory::~DrawFactory()
{
}

DrawImageParam * DrawFactory::createDrawImageParam(ViewObj * viewOwner, NormalRowParam * rowParam, QString imgName)
{
	/*DrawImageParam * rectParam = new DrawImageParam(viewOwner, rowParam, imgName);

	if (!rectParam->getImage())
	{
		delete rectParam;
		rectParam = NULL;
		return NULL;
	}
	return rectParam;*/
	return NULL;
}
DrawImageParam * DrawFactory::createDrawImageParam(ViewObj * viewOwner, QString imgName)
{
	/*DrawImageParam * rectParam = new DrawImageParam(viewOwner, NULL, imgName);
	return rectParam;*/
	return NULL;
}
DrawImage * DrawFactory::createImage(ViewObj * viewOwner, DrawImageParam * imgParam)
{
	/*DrawManager * drawMgr = new DrawManager(viewOwner);
	DrawImage * img = new DrawImage(viewOwner);

	img->initParam(imgParam);
	drawMgr->initDraw(img);
	viewOwner->initDrawManager(drawMgr);

	return img;*/
	return NULL;
}
DrawRectParam * DrawFactory::createDrawRectParam(ViewObj * viewOwner, char fillRoundType, int roundLineColor, int fillColor, short roundRectThick)
{
	/*DrawRectParam * rectParam = new DrawRectParam(viewOwner);
	rectParam->setFillRectType(fillRoundType);
	rectParam->setColor(roundLineColor, fillColor);
	rectParam->setRoundLineThick(roundRectThick);
	return rectParam;*/
	return NULL;
}
DrawRect * DrawFactory::createRect(ViewObj * viewOwner, DrawRectParam * rectParam)
{
	ObjDrawManager * drawMgr = viewOwner->getDrawMgr();
	BaseViewParam * viewParam = new BaseViewParam(viewOwner);
	viewParam->init(0, 0, 0, 0);
	DrawRect * rect = new DrawRect(viewOwner, UIConstants::FillRectType::ONLY_ROUND_RECT, UIConstants::MyColor::YELLOW, BaseConstants::NONE, 1);
	drawMgr->initDraws(rect, NULL);
	

	return rect;
}
DrawRow * DrawFactory::createDrawRow(ViewObj * viewOwner, DrawRectParam * rectParam)
{
	/*DrawManager * drawMgr = new DrawManager(viewOwner);
	DrawRow * drawRow = new DrawRow(viewOwner);

	drawRow->initParam(rectParam);
	drawMgr->initDraw(drawRow);
	viewOwner->initDrawManager(drawMgr);

	return drawRow;*/
	return NULL;
}
DrawTxtParam * DrawFactory::createDrawTxtParam(ViewObj * viewOwner, QString str, DrawTxtParam * focusData)
{
	/*DrawTxtParam * txtParam = new DrawTxtParam(viewOwner, str, focusData);
	return txtParam;*/
	return NULL;
}
DrawTxtParam * DrawFactory::createDrawTxtParam(WriterManager * writ, ViewObj * viewOwner, QString str, DrawTxtParam * focusData)
{
	/*DrawTxtParam * txtParam = new DrawTxtParam(writ,viewOwner, str, focusData);
	return txtParam;*/
	return NULL;
}
// 创建文字绘制
DrawStr * DrawFactory::createDrawTxt(ViewObj * viewOwner, DrawTxtParam * contentData)
{
	//DrawManager * drawMgr = new DrawManager(viewOwner);
	////DrawTxtParam * txtParam = createDrawTxtParam(viewOwner, str, focusData);
	//DrawStr * draw = new DrawStr(viewOwner);
	//draw->initParam(contentData);
	//drawMgr->initDraw(draw);
	//viewOwner->initDrawManager(drawMgr);
	//return draw;
	return NULL;
}
DrawLine *DrawFactory::createDrawExcelLine(ViewObj * viewOwner, QColor *lineColor, int lineWidth, Qt::PenStyle penStyle)
{
	/*DrawLine * drawLine = new DrawLine(viewOwner);
	DrawLineParam * drawLineParam = DrawFactory::createDrawExcelLineParam(viewOwner, lineColor, lineWidth, penStyle);
	drawLine->SetLineParam(drawLineParam);
	DrawManager * drawMgr = new DrawManager(viewOwner);
	drawMgr->initDraw(drawLine);
	viewOwner->initDrawManager(drawMgr);
	return drawLine;*/
	return NULL;
}
DrawLineParam * DrawFactory::createDrawExcelLineParam(ViewObj * viewOwner, QColor *lineColor, int lineWidth, Qt::PenStyle penStyle){
	/*DrawLineParam * lineParam = new DrawLineParam(viewOwner);
	lineParam->setLineColor(lineColor);
	lineParam->setLineStyle(penStyle);
	lineParam->setLineWidth(lineWidth);
	return lineParam;*/
	return NULL;
}

DrawFreeLineParam * DrawFactory::createFreeLineDrawParam(ViewObj * viewOwner, int thick, int color)
{
	/*DrawFreeLineParam * drawParam = new DrawFreeLineParam(viewOwner, thick, color);
	return drawParam;*/
	return NULL;
}
//DrawFreeLine * DrawFactory::createFreeLine(ViewObj * viewOwner, DrawFreeLineParam * drawParam)
//{
//	DrawFreeLine * editImgLine = new DrawFreeLine(viewOwner);
//	//drawParam->addToPointList(x, y);
//	editImgLine->initParam(drawParam);
//	DrawManager * drawMgr = new DrawManager(viewOwner);
//	drawMgr->initDraw(editImgLine);
//	viewOwner->initDrawManager(drawMgr);
//	return editImgLine;
//}
BaseDraw * DrawFactory::createDraw(ViewObj * viewOwner)
{
	/*BaseDraw * draw = NULL;
	int objId = viewOwner->getObjId();
	switch (objId)
	{
	case ConstantsID::ContainerId::EDIT_IMG_FREE_LINE:
		draw = new DrawFreeLine(viewOwner);
		break;
	case ConstantsID::ContainerId::EDIT_IMG_DIRECT_LINE_NO_SIGN:
	case ConstantsID::ContainerId::EDIT_IMG_DIRECT_LINE_SIGN:
		draw = new DrawDirectLine(viewOwner);
		break;
	case ConstantsID::ContainerId::EDIT_IMG_CIRCLE:
		draw = new DrawCircle(viewOwner);
		break;
	}
	draw->initParam(drawParam);
	DrawManager * drawMgr = new DrawManager(viewOwner);
	drawMgr->initDraw(draw);
	viewOwner->initDrawManager(drawMgr);
	drawParam->initOwner(viewOwner);
	return draw;*/
	return NULL;
}
DrawBaseImgButtonParam * DrawFactory::createDrawBaseImgButtonParam(ViewObj * viewOwner, QList<QString> * imgNameList)
{
	/*DrawBaseImgButtonParam * dest = new DrawBaseImgButtonParam(viewOwner, imgNameList);
	dest->init();
	return dest;*/
	return NULL;
}
DrawBaseImgButton * DrawFactory::createDrawBaseImgButton(ViewObj * viewOwner, DrawBaseImgButtonParam * drawParam)
{
	/*DrawBaseImgButton * imgBtn = new DrawBaseImgButton(viewOwner);
	imgBtn->initParam(drawParam);
	DrawManager * drawMgr = new DrawManager(viewOwner);
	drawMgr->initDraw(imgBtn);
	viewOwner->initDrawManager(drawMgr);
	return imgBtn;*/
	return NULL;
}