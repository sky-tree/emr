#include "ViewFactory.h"


#include <iostream>

#include <QTime>



#include "WriterManager.h"
#include "WriterLogicMgr.h"
#include "OperateMgr.h"
#include "CrossParentMgr.h"
#include "SystemParams.h"

#include "RelativeChildrenParam.h"

#include "DrawRect.h"
#include "DrawImage.h"
#include "DrawStr.h"
#include "DrawRowTop.h"
#include "DrawRowBottom.h"
#include "DrawHorizontalLine.h"

#include "BaseContainer.h"
#include "ViewObj.h"
#include "TxtWinDrawManager.h"
#include "TxtWinLogicManager.h"

#include "CodeParams.h"

#include "RelativeChildrenParam.h"

#include "TxtWindowMouse.h"

#include "BaseChildrenManager.h"
#include "RelativeLayout.h"
#include "VerticalLayout.h"
#include "RowLayout.h"
#include "PageContentTxtLayerLayout.h"
#include "ScrollPlaceLayout.h"

#include "BaseViewParam.h"
#include "LayoutViewParam.h"
#include "VerticalViewParam.h"
#include "RowViewParam.h"
#include "ImgViewParam.h"
#include "FlagViewParam.h"
#include "TxtViewParam.h"
#include "MiddleButtonViewParam.h"
#include "BaseScaleParam.h"

#include "GlobalImgData.h"
#include "ImgConTentData.h"
#include "TxtWinData.h"
#include "StrContentData.h"

#include "ButtonInput.h"
#include "ImageMouseInput.h"
#include "ImgContentData.h"
#include "ObjMouseManager.h"
#include "ObjDrawManager.h"
#include "TempleteViewObj.h"
#include "SystemParams.h"

#include "LogicManager.h"
#include "BaseLogic.h"

#include "BaseSystem.h"

#include "ConstantsID.h"
#include "UIConstants.h"
#include "BaseConstants.h"
#include "StrConstants.h"
#include "ConstantsNumber.h"
#include "BaseMath.h"

#include "BaseTemplate.h"
#include "ExcelLayout.h"
#include "DrawLine.h"
#include "TempleteFactory.h"
#include "LogicFactory.h"
#include "ExcelCellPageMouse.h"
#include "ExcelTemplete.h"
ViewFactory::ViewFactory()
{
}

ViewFactory::~ViewFactory()
{
}

void ViewFactory::init()
{
}

ViewObj * ViewFactory::createRoot(WriterManager * writerMgr)
{
	ViewObj * root = new ViewObj(writerMgr, ConstantsID::ContainerId::ROOT_VIEW);
	BaseChildrenManager * mgr = new BaseChildrenManager(root);
	BaseViewParam * viewParam = new BaseViewParam(root);
	viewParam->init(0, 0, 0, 0);
	DrawRect * rect = new DrawRect(root, UIConstants::FillRectType::ONLY_FILL, UIConstants::MyColor::GREEN, BaseConstants::NONE, 1);
	ObjDrawManager * drawMgr = root->getDrawMgr();
	drawMgr->initDraws(rect, NULL);
	ViewObj * window = createTxtWindow(writerMgr);

	mgr->pushChild(window, true);
	return root;
}
ViewObj * ViewFactory::createTxtWindow(WriterManager * writerMgr)
{
	ViewObj * window = new ViewObj(writerMgr, ConstantsID::ContainerId::TXT_WIN_VIEW);
	TxtWinData * winData = new TxtWinData(window);
	window->initContentData(winData);
	ObjMouseManager * mouseMgr = window->getMouseMgr();
	TxtWindowMouse * mouse = new TxtWindowMouse(window);
	mouseMgr->setMouse(mouse);
	DrawRect * rect = new DrawRect(window, UIConstants::FillRectType::ONLY_FILL, UIConstants::MyColor::WINDOW_BACKGROUND_COLOR, BaseConstants::NONE, 1);
	ObjDrawManager * drawMgr = window->getDrawMgr();
	drawMgr->initDraws(rect, NULL);

	TxtWinLogicManager * winLogicMgr = new TxtWinLogicManager(window);
	BaseLogic * logic = LogicFactory::createLogic(window);
	winLogicMgr->addLogic(logic);

	writerMgr->getLogicMgr()->addToRunningList(logic);

	LayoutViewParam * viewParam = new LayoutViewParam(window);
	int * clip = new int[4];
	clip[0] = 0;
	clip[1] = UIConstants::UI_TOOLBAR_H;
	//clip[1] = 100;
	clip[2] = 0;
	clip[3] = 0;
	viewParam->initClip(clip);
	viewParam->init(0, 0, 0, 0);
	viewParam->initInside(0, 0, UIConstants::UI_TOOLBAR_H, 0);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_TOTAL, BaseConstants::DefaultWHType::FILL_PARENT_TOTAL);
	RelativeLayout * mgr = new RelativeLayout(window);
	ViewObj * pagePanel = createPagePanel(writerMgr);
	ViewObj * scrollPanel = createScrollPanel(writerMgr);
	RelativeChildrenParam * xParam = new RelativeChildrenParam(scrollPanel, BaseConstants::RelativeDirectionType::INSIDE_RIGHT, window);
	mgr->addChild(scrollPanel, -1, true, BaseConstants::NONE, xParam);

	int * minValue = new int[1];
	minValue[0] = 0;
	xParam = new RelativeChildrenParam(pagePanel, BaseConstants::RelativeDirectionType::INSIDE_LEFT, window,
		BaseConstants::RelativeDirectionType::LEFT, scrollPanel, minValue);
	mgr->addChild(pagePanel, -1, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST, xParam);

	mgr->swapChild(scrollPanel, pagePanel);
	return window;
}
ViewObj * ViewFactory::createPagePanel(WriterManager * writerMgr)
{
	ViewObj * dest = new ViewObj(writerMgr, ConstantsID::ContainerId::PAGE_PANEL);
	VerticalViewParam * viewParam = new VerticalViewParam(dest);
	viewParam->setChildrenDis(UIConstants::PageParams::PAGE_DIS_Y);
	//DrawRect * rect = new DrawRect(dest, UIConstants::FillRectType::ONLY_FILL, UIConstants::MyColor::BLACK, BaseConstants::NONE, 1);
	//ObjDrawManager * drawMgr = dest->getDrawMgr();
	//drawMgr->initDraws(rect, NULL);
	viewParam->init(0, 0, 0, 0);
	viewParam->initInside(UIConstants::WindowParams::WINDOW_PAGE_PANEL_INSIDE_LEFT_RIGHT
		, UIConstants::WindowParams::WINDOW_PAGE_PANEL_INSIDE_LEFT_RIGHT, UIConstants::WindowParams::WINDOW_PAGE_PANEL_INSIDE_TOP
		, UIConstants::WindowParams::WINDOW_PAGE_PANEL_INSIDE_BOTTOM);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);

	VerticalLayout * mgr = new VerticalLayout(dest);
	ViewObj * page = createNormalPage(writerMgr);
	mgr->addChild(page, -1, true, 0);
	mgr->replaceChildren();

	viewParam->refreshWByChildren(true);
	viewParam->refreshHByChildren(true);
	int a = 0;
	return dest;
}
ViewObj * ViewFactory::createScrollPanel(WriterManager * writerMgr)
{
	ViewObj * dest = new ViewObj(writerMgr, ConstantsID::ContainerId::SCROLL_PANEL);
	LayoutViewParam * viewParam = new LayoutViewParam(dest);
	DrawRect * rect = new DrawRect(dest, UIConstants::FillRectType::ONLY_FILL, UIConstants::MyColor::GRAY, BaseConstants::NONE, 1);
	ObjDrawManager * drawMgr = dest->getDrawMgr();
	drawMgr->initDraws(rect, NULL);
	viewParam->init(0, 0, 16, 580);
	viewParam->initDefaultWH(0, BaseConstants::DefaultWHType::FILL_PARENT_INSIDE);

	RelativeLayout * mgr = new RelativeLayout(dest);

	ViewObj * ButtonUP = createScrollButtonUp(writerMgr);
	RelativeChildrenParam * xParam = new RelativeChildrenParam(ButtonUP, BaseConstants::RelativeDirectionType::INSIDE_TOP, dest);
	mgr->addChild(ButtonUP, -1, true, BaseConstants::NONE, xParam);

	ViewObj * ButtonDown = createScrollButtonDown(writerMgr);
	xParam = new RelativeChildrenParam(ButtonDown, BaseConstants::RelativeDirectionType::INSIDE_BOTTOM, dest);
	mgr->addChild(ButtonDown, -1, true, BaseConstants::NONE, xParam);

	ViewObj * ButtonMiddle = createScrollButtonMiddle(writerMgr);
	xParam = new RelativeChildrenParam(ButtonMiddle, BaseConstants::RelativeDirectionType::BOTTOM, ButtonUP, BaseConstants::RelativeDirectionType::TOP, ButtonDown, NULL);
	mgr->addChild(ButtonMiddle, -1, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST, xParam);

	mgr->replaceChildren();
	return dest;
}
ViewObj * ViewFactory::createNormalPage(WriterManager * writerMgr)
{
	return createNormalPage(writerMgr, NULL, NULL);
}
ViewObj * ViewFactory::createNormalPage(WriterManager * writerMgr, ViewObj * title, ViewObj * bottom)
{
	ViewObj * dest = new ViewObj(writerMgr, ConstantsID::ContainerId::NORMAL_PAGE);
	LayoutViewParam * viewParam = new LayoutViewParam(dest);
	viewParam->init(0, 0, 0, 0);
	SystemParams * sysParams = writerMgr->getOperateMgr()->getSystemParams();
	int insideTop = sysParams->getPageInsideTop();
	int insideBottom = sysParams->getPageInsideBottom();
	int insideLeft = sysParams->getPageInsideLeft();
	int insideRight = sysParams->getPageInsideRight();
	viewParam->initInside(insideLeft, insideRight, insideTop, insideBottom);


	RelativeLayout * mgr = new RelativeLayout(dest);


	DrawRect * rect = new DrawRect(dest, UIConstants::FillRectType::ROUND_LINE_AND_FILL, UIConstants::MyColor::WHITE
		, UIConstants::MyColor::BLACK, 1);
	rect->setTopLineViewType(BaseConstants::ViewType::NORMAL);
	rect->setBottomLineViewType(BaseConstants::ViewType::NORMAL);
	rect->setLeftLineViewType(BaseConstants::ViewType::NORMAL);
	rect->setRightLineViewType(BaseConstants::ViewType::NORMAL);
	ObjDrawManager * drawMgr = dest->getDrawMgr();
	drawMgr->initDraws(rect, NULL);


	if (!title)
	{
		title = createPageTitle(writerMgr);
		RelativeChildrenParam * xParam = new RelativeChildrenParam(title, BaseConstants::RelativeDirectionType::INSIDE_TOP, dest);
		mgr->addChild(title, -1, true, BaseConstants::NONE, xParam);
	}
	else
		mgr->addChild(title, -1, true, BaseConstants::NONE, title->getRelativeParam());

	if (!bottom)
	{
		bottom = createPageBottom(writerMgr);
		RelativeChildrenParam * xParam = new RelativeChildrenParam(bottom, BaseConstants::RelativeDirectionType::INSIDE_BOTTOM, dest);
		mgr->addChild(bottom, -1, true, BaseConstants::NONE, xParam);
	}
	else
		mgr->addChild(bottom, -1, true, BaseConstants::NONE, bottom->getRelativeParam());



	ViewObj * content = createPageContent(writerMgr);
	RelativeChildrenParam * xParam = new RelativeChildrenParam(content, BaseConstants::RelativeDirectionType::BOTTOM, title
		, BaseConstants::RelativeDirectionType::TOP, bottom, NULL);
	mgr->addChild(content, -1, true, BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST, xParam);

	OperateMgr * operateMgr = writerMgr->getOperateMgr();
	int w = 0;
	int h = 0;
	BaseMath::refreshPageWHByPixel(sysParams->isPaperHorizontal(), sysParams->getPaperType(), w, h);// / UIConstants::WindowParams::W_H_RATIO;
	//h = 300;// / UIConstants::WindowParams::W_H_RATIO;
	//h /= UIConstants::WindowParams::W_H_RATIO;
	viewParam->setSelfW(w, BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_CHILDREN);
	viewParam->setSelfH(h, BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_CHILDREN);
	mgr->replaceChildren();

	return dest;
}


ViewObj * ViewFactory::createPageTitle(WriterManager * writerMgr)
{
	ViewObj * dest = new ViewObj(writerMgr, ConstantsID::ContainerId::PAGE_TITLE);
	CrossParentMgr * crossMgr = new CrossParentMgr(dest);
	VerticalViewParam * viewParam = new VerticalViewParam(dest);

	DrawRect * rect = new DrawRect(dest, UIConstants::FillRectType::ONLY_ROUND_RECT, UIConstants::MyColor::YELLOW
		, UIConstants::MyColor::BLACK, 1);
	rect->setBottomLineViewType(BaseConstants::ViewType::NORMAL);
	ObjDrawManager * drawMgr = dest->getDrawMgr();
	drawMgr->initDraws(rect, NULL);

	//viewParam->initMinH(UIConstants::PageParams::PAGE_MIN_H);
	viewParam->initInside(0, 0, 0, UIConstants::PageParams::PAGE_TITLE_DIS_BOTTOM);
	viewParam->init(0, 0, 50, 10);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_INSIDE, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	ViewObj * txtLayer = createLayer(writerMgr, ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER,
		UIConstants::MyColor::GREEN, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	VerticalLayout * layout = new VerticalLayout(dest);
	layout->addChild(txtLayer, -1, true, 0);
	//layout->pushChild(txtLayer, true);
	layout->replaceChildren();
	return dest;
}
ViewObj * ViewFactory::createPageBottom(WriterManager * writerMgr)
{
	ViewObj * dest = new ViewObj(writerMgr, ConstantsID::ContainerId::PAGE_BOTTOM);
	CrossParentMgr * crossMgr = new CrossParentMgr(dest);
	VerticalViewParam * viewParam = new VerticalViewParam(dest);

	//DrawRect * rect = new DrawRect(dest, UIConstants::FillRectType::ONLY_ROUND_RECT, UIConstants::MyColor::YELLOW
	//	, UIConstants::MyColor::BLACK, 1);
	//rect->setTopLineViewType(BaseConstants::ViewType::NORMAL);
	//ObjDrawManager * drawMgr = dest->getDrawMgr();
	//drawMgr->initDraws(rect, NULL);

	viewParam->init(0, 0, 50, 50);
	viewParam->initInside(0, 0, UIConstants::PageParams::PAGE_TITLE_DIS_BOTTOM, 0);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_INSIDE, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	//ViewObj * txtLayer = createLayer(writerMgr, UIConstants::MyColor::WHITE, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	ViewObj * txtLayer = createLayer(writerMgr, ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER
		, UIConstants::MyColor::GREEN, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	VerticalLayout * layout = new VerticalLayout(dest);
	layout->addChild(txtLayer, -1, true, 0);
	//layout->pushChild(txtLayer, true);
	layout->replaceChildren();
	return dest;
}
ViewObj * ViewFactory::createPageContent(WriterManager * writerMgr)
{
	ViewObj * dest = new ViewObj(writerMgr, ConstantsID::ContainerId::PAGE_CONTENT);
	VerticalViewParam * viewParam = new VerticalViewParam(dest);

#ifdef DEBUG_DRAW
	DrawRect * rect = new DrawRect(dest, UIConstants::FillRectType::ONLY_FILL, UIConstants::MyColor::YELLOW, BaseConstants::NONE, 1);
	ObjDrawManager * drawMgr = dest->getDrawMgr();
	drawMgr->initDraws(rect, NULL);
#endif
	viewParam->init(0, 0, 50, 10);
	viewParam->initInside(0, 0, UIConstants::PageParams::PAGE_CONTENT_INSIDE_TOP, UIConstants::PageParams::PAGE_CONTENT_INSIDE_BOTTOM);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_INSIDE, BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE);
	ViewObj * txtLayer = createLayer(writerMgr, ConstantsID::ContainerId::CONTENT_TXT_LAYER
		, UIConstants::MyColor::YELLOW, BaseConstants::DefaultWHType::FILL_PARENT_INSIDE);
	VerticalLayout * layout = new VerticalLayout(dest);
	layout->addChild(txtLayer, -1, true, 0);
	//layout->pushChild(txtLayer, true);
	layout->replaceChildren();
	return dest;
}
ViewObj * ViewFactory::createLayer(WriterManager * writerMgr, int layerID, int color, char defaultH)
{
	ViewObj * dest = new ViewObj(writerMgr, layerID);
	VerticalViewParam * viewParam = new VerticalViewParam(dest);
	if (layerID == ConstantsID::ContainerId::CONTENT_TXT_LAYER)
	{
		int * clip = new int[4];
		for (int i = 0; i < 4; i ++)
		{
			clip[i] = 0;
		}
		clip[0] = UIConstants::PageParams::PAGE_TXT_LAYER_CLIP_LEFT;
		clip[2] = UIConstants::PageParams::PAGE_TXT_LAYER_CLIP_RIGHT;

		viewParam->initClip(clip);
	}
//#ifdef DEBUG_DRAW
	//DrawRect * rect = new DrawRect(dest, UIConstants::FillRectType::ONLY_FILL, color, BaseConstants::NONE, 1);
	//ObjDrawManager * drawMgr = dest->getDrawMgr();
	//drawMgr->initDraws(rect, NULL);
//#endif

	PageContentTxtLayerLayout * layout = new PageContentTxtLayerLayout(dest);

	viewParam->init(0, 0, 0, 0);
	//viewParam->initInside(0, 0, insideT, insideB);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_INSIDE, defaultH);

	return dest;
}
ViewObj * ViewFactory::createExcelPage(WriterManager * writerMgr, int x, int y, int w, int h, ExcelTemplete* excelTemplate, bool isCreateOnLoad)
{
	ViewObj * dest = new TempleteViewObj(writerMgr, excelTemplate, ConstantsID::ContainerId::EXCEL_PAGE);
	ExcelCellPageMouse * excelMouse = new ExcelCellPageMouse(dest);
	ObjMouseManager *  mouseMgr = dest->getMouseMgr();
	mouseMgr->setMouse(excelMouse);
	VerticalViewParam * viewParam = new VerticalViewParam(dest);
	if (!isCreateOnLoad)
	{
		DrawRect * rect = new DrawRect(dest, UIConstants::FillRectType::ROUND_LINE_AND_FILL, 0, UIConstants::MyColor::BLACK, 1);
		// 设置是否显示边框
		if (excelTemplate->getIsShowBorader())
		{
			rect->initLineViewType(UIConstants::LineType::SINGLE_LINE, UIConstants::LineType::SINGLE_LINE, UIConstants::LineType::SINGLE_LINE, UIConstants::LineType::SINGLE_LINE);
		}
		else
		{

		}
		ObjDrawManager * drawMgr = dest->getDrawMgr();
		drawMgr->initDraws(rect, NULL);
	}


	viewParam->init(x, y, w, h);
	viewParam->initInside(UIConstants::PageParams::EXCEL_PAGE_INSIDE_LEFT, UIConstants::PageParams::EXCEL_PAGE_INSIDE_RIGHT
		, UIConstants::PageParams::EXCEL_PAGE_INSIDE_TOP, UIConstants::PageParams::EXCEL_PAGE_INSIDE_BOTTOM);
	viewParam->initDefaultWH(0, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	ViewObj * txtLayer = createExcelLayer(writerMgr, ConstantsID::ContainerId::EXCEL_TXT_LAYER
		, UIConstants::MyColor::GRAY, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	VerticalLayout * layout = new VerticalLayout(dest);
	layout->addChild(txtLayer, -1, true, 0);
	//layout->pushChild(txtLayer, true);
	layout->replaceChildren();
	return dest;
}
ViewObj * ViewFactory::createExcelLayer(WriterManager * writerMgr, int layerID, int color, char defaultH)
{
	ViewObj * dest = new ViewObj(writerMgr, layerID);
	VerticalViewParam * viewParam = new VerticalViewParam(dest);
	DrawRect * rect = new DrawRect(dest, BaseConstants::NONE, color, BaseConstants::NONE, 1);
	ObjDrawManager * drawMgr = dest->getDrawMgr();
	drawMgr->initDraws(rect, NULL);
	PageContentTxtLayerLayout * layout = new PageContentTxtLayerLayout(dest);

	viewParam->init(0, 0, 50, 20);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_INSIDE, defaultH);

	return dest;
}
ViewObj * ViewFactory::createRow(WriterManager * writerMgr, BaseContainer * paragraph)
{
	ViewObj * dest = new ViewObj(writerMgr, ConstantsID::ContainerId::NORMAL_ROW);
	//dest->setParent(paragraph);
	RowViewParam * viewParam = new RowViewParam(dest);

	//DrawRect * rect = new DrawRect(dest, UIConstants::FillRectType::ROUND_LINE_AND_FILL, UIConstants::MyColor::WHITE, UIConstants::MyColor::BLACK, 1);
	//rect->initLineViewType(0, 0, 0, UIConstants::LineType::SINGLE_DOT_LINE);

	DrawRowBottom * drawRowBottom = new DrawRowBottom(dest);
	DrawRowTop * drawRowTop = new DrawRowTop(dest);

	ObjDrawManager * drawMgr = dest->getDrawMgr();
	drawMgr->initDraws(drawRowBottom, drawRowTop);
	RowLayout * layout = new RowLayout(dest);

	viewParam->initInside(0, 0, UIConstants::RowParams::INSIDE_TOP, UIConstants::RowParams::INSIDE_BOTTOM);
	viewParam->init(0, 0, 50, 10);
	//viewParam->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_INSIDE, 0);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_INSIDE, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	return dest;
}
ViewObj * ViewFactory::createExcelRowContainer(ExcelTemplete * excelTemplete, ViewObj * viewParent)
{

	ViewObj * row = new TempleteViewObj(excelTemplete->getWriterMgr(), excelTemplete, ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW);
	BaseChildrenManager* childMgr = new BaseChildrenManager(row);
	// 显示参数
	BaseViewParam * rowParam = new  BaseViewParam(row);
	rowParam->init(0, 0, 50, 10);
	rowParam->initDefaultWH(0, 0);
	ObjDrawManager * drawMgr = row->getDrawMgr();
	DrawRect * rect = new DrawRect(row, UIConstants::FillRectType::ROUND_LINE_AND_FILL, 0, BaseConstants::NONE, 1);
	rect->initLineViewType(UIConstants::LineType::SINGLE_LINE, UIConstants::LineType::SINGLE_LINE, UIConstants::LineType::SINGLE_LINE
		, UIConstants::LineType::SINGLE_LINE);
	drawMgr->initDraws(rect, NULL);
	// 行布局
	//LayoutFactory::createRowLayout(paragraph, row);

	//viewParent->addChild(row, -1);
	return row;
}
ViewObj * ViewFactory::createMenstruationRow(WriterManager * writerMgr, BaseContainer * paragraph)
{
	ViewObj * dest = createRow(writerMgr, paragraph);
	RowViewParam* viewParam = (RowViewParam*)dest->getViewParam();
	viewParam->init(0, 0, 0, 0);
	//viewParam->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_INSIDE, 0);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	ObjDrawManager * drawMgr = dest->getDrawMgr();
	drawMgr->setBottomDraw(NULL);
	return dest;
}
ViewObj * ViewFactory::createEnter(WriterManager * writerMgr, int userID)
{
	ViewObj * enter = new ViewObj(writerMgr, ConstantsID::ContainerId::ENTER);
	ImgViewParam * param = new ImgViewParam(enter);
	ImgConTentData * data = new ImgConTentData(enter, NULL, StrConstants::MyKeyBoard::ENTER);
	enter->initContentData(data);
	DrawImage * img = new DrawImage(enter, data);
	ObjDrawManager * drawMgr = enter->getDrawMgr();
	drawMgr->initDraws(img, NULL);
	int w = data->getImg()->width();
	int h = data->getImg()->height();
	param->init(0, 0, w, h);
	return enter;
}
ViewObj * ViewFactory::createFlag(WriterManager * writerMgr)
{
	ViewObj * flag = new ViewObj(writerMgr, ConstantsID::ContainerId::FLAG);
	FlagViewParam * param = new FlagViewParam(flag);
	DrawRect * rect = new DrawRect(flag, UIConstants::FillRectType::ONLY_FILL, UIConstants::MyColor::BLACK, BaseConstants::NONE, 1);
	TxtWinDrawManager * drawMgr = (TxtWinDrawManager *)flag->getDrawMgr();
	drawMgr->initDraws(rect, NULL);

	LogicManager * logicMgr = new LogicManager(flag);
	BaseLogic * logic = LogicFactory::createLogic(flag);
	logicMgr->addLogic(logic);
	writerMgr->getLogicMgr()->addToRunningList(logic);

	param->init(0, 0, 2, 0);
	return flag;
}
ViewObj * ViewFactory::createHorizontalLine(WriterManager * writerMgr, int userID)
{
	ViewObj * dest = new ViewObj(writerMgr, ConstantsID::ContainerId::HORIZONTAL_LINE);
	BaseViewParam * param = new BaseViewParam(dest);
	BaseContentData * txtData = new BaseContentData(dest, ConstantsID::ClassId::BASE_CONTENT_DATA, NULL);
	dest->initContentData(txtData);

	DrawHorizontalLine * line = new DrawHorizontalLine(dest, UIConstants::FillRectType::ONLY_FILL);
	ObjDrawManager * drawMgr = dest->getDrawMgr();
	drawMgr->initDraws(line, NULL);

	param->init(0, 0, 0, 16);
	param->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_INSIDE, 0);
	return dest;
}
ViewObj * ViewFactory::createEndPageLine(WriterManager * writerMgr, int userID)
{
	ViewObj * dest = new ViewObj(writerMgr, ConstantsID::ContainerId::END_PAGE_LINE);
	BaseViewParam * param = new BaseViewParam(dest);
	BaseContentData * txtData = new BaseContentData(dest, ConstantsID::ClassId::BASE_CONTENT_DATA, NULL);
	dest->initContentData(txtData);

	DrawHorizontalLine * line = new DrawHorizontalLine(dest, UIConstants::FillRectType::ONLY_FILL);
	ObjDrawManager * drawMgr = dest->getDrawMgr();
	drawMgr->initDraws(line, NULL);

	param->init(0, 0, 0, 16);
	param->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_INSIDE, 0);
	return dest;
}
ViewObj * ViewFactory::createTxt(WriterManager * writerMgr, int userID, QString str)
{
	//StrContentData
	ViewObj * txt = new ViewObj(writerMgr, ConstantsID::ContainerId::TXT);
	TxtViewParam * param = new TxtViewParam(txt);
	StrContentData * txtData = new StrContentData(txt, NULL, str, writerMgr->getOperateMgr()->getFocusTxtData());
	txt->initContentData(txtData);
	DrawStr * draw = new DrawStr(txt);
	ObjDrawManager * drawMgr = txt->getDrawMgr();
	drawMgr->initDraws(draw, NULL);

	return txt;
}
ViewObj * ViewFactory::createPageNumber(WriterManager * writerMgr, CodeParams * codeParams, int objId)
{
	ViewObj * txt = new ViewObj(writerMgr, objId);
	TxtViewParam * param = new TxtViewParam(txt);
	StrContentData * txtData = new StrContentData(txt, codeParams, "", writerMgr->getOperateMgr()->getFocusTxtData());
	txt->initContentData(txtData);
	DrawStr * draw = new DrawStr(txt);
	ObjDrawManager * drawMgr = txt->getDrawMgr();
	drawMgr->initDraws(draw, NULL);

	return txt;
}
ViewObj * ViewFactory::createTemplateTxt(WriterManager * writerMgr, BaseTemplate* parentTemplate, QString str, StrContentData * data)
{
	TempleteViewObj * txt = new TempleteViewObj(writerMgr, parentTemplate, ConstantsID::ContainerId::TMPLATE_TXT);
	TxtViewParam * param = new TxtViewParam(txt);
	StrContentData * txtData = new StrContentData(txt, NULL, str, data);
	txt->initContentData(txtData);
	DrawStr * draw = new DrawStr(txt);
	ObjDrawManager * drawMgr = txt->getDrawMgr();
	drawMgr->initDraws(draw, NULL);
	int chooseObjId = parentTemplate->getChooseObjId();
	if (chooseObjId < 0)
	{
		txt->setChooseObjId(parentTemplate->getObjID());
	}
	else
	{
		txt->setChooseObjId(chooseObjId);
	}

	return txt;
}
StrContentData * ViewFactory::createDefaultStrContentData()
{
	StrContentData * txtData = new StrContentData(NULL, NULL, StrConstants::NONE_STR, UIConstants::DefaultFontParams::TXT_SIZE
		, StrConstants::DEFAULT_FONT_FAMILY, UIConstants::DefaultFontParams::FONT_COLOR, UIConstants::DefaultFontParams::BACKGROUND_COLOR
		, BaseConstants::NONE, BaseConstants::NONE, BaseConstants::NONE, BaseConstants::NONE
		, UIConstants::DefaultFontParams::ITALIC, UIConstants::DefaultFontParams::BOLD
		, BaseConstants::NONE);

	return txtData;
}

ViewObj * ViewFactory::createScrollButtonUp(WriterManager * writerMgr)
{
	ViewObj * dest = new ViewObj(writerMgr, ConstantsID::ContainerId::SCROLL_BUTTON_UP);
	BaseViewParam * viewParam = new BaseViewParam(dest);

	ObjMouseManager * mouseMgr = dest->getMouseMgr();
	ButtonInput * viewButton = new ButtonInput(dest, writerMgr, ":/EmrClient/ButtonUpRelease", ":/EmrClient/ButtonUpPress");
	viewButton->init(ConstantsID::ContainerId::SCROLL_BUTTON_UP);
	mouseMgr->setMouse((BaseMouse*)viewButton);

	return dest;
}

ViewObj * ViewFactory::createScrollButtonMiddle(WriterManager * writerMgr)
{
	ViewObj * dest = new ViewObj(writerMgr, ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE);
	MiddleButtonViewParam * viewParam = new MiddleButtonViewParam(dest);
	viewParam->init(0, 0, 16, 16);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_INSIDE
		, BaseConstants::DefaultWHType::FILL_PARENT_RELATIVE_INSIDE);

	DrawRect * rect = new DrawRect(dest, UIConstants::FillRectType::ONLY_FILL
		, UIConstants::MyColor::BACKG_ROUND, BaseConstants::NONE, 1);
	ObjDrawManager * drawMgr = dest->getDrawMgr();
	drawMgr->initDraws(rect, NULL);

	ScrollPlaceLayout * mgr = new ScrollPlaceLayout(dest);

	ViewObj * scrollBarDest = new ViewObj(writerMgr, ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE_BAR);
	//BaseViewParam * scrollBarViewParam = new BaseViewParam(scrollBarDest);

	ObjMouseManager * mouseMgr = scrollBarDest->getMouseMgr();
	ButtonInput * viewButton = new ButtonInput(scrollBarDest, writerMgr, ":/EmrClient/ButtonMiddleRelease", ":/EmrClient/ButtonMiddlePress");
	viewButton->init(ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE);
	mouseMgr->setMouse((BaseMouse*)viewButton);

	mgr->addChild(scrollBarDest, -1, true, BaseConstants::ReplaceType::REPLACE_NOW);
	//mgr->replaceChildren();

	return dest;
}

ViewObj * ViewFactory::createScrollButtonDown(WriterManager * writerMgr)
{
	ViewObj * dest = new ViewObj(writerMgr, ConstantsID::ContainerId::SCROLL_BUTTON_DOWN);
	BaseViewParam * viewParam = new BaseViewParam(dest);

	ObjMouseManager * mouseMgr = dest->getMouseMgr();
	ButtonInput * viewButton = new ButtonInput(dest, writerMgr, ":/EmrClient/ButtonDownRelease", ":/EmrClient/ButtonDownPress");
	viewButton->init(ConstantsID::ContainerId::SCROLL_BUTTON_DOWN);
	mouseMgr->setMouse((BaseMouse*)viewButton);

	return dest;
}

ViewObj * ViewFactory::createImg(WriterManager * writerMgr, CodeParams * codeParams, QString imgPath)
{
	// ":/EmrClient/test"
	ViewObj * img = new ViewObj(writerMgr, ConstantsID::ContainerId::IMG);
	ImgViewParam * param = new ImgViewParam(img);
	ImgConTentData * data = new ImgConTentData(img, codeParams, imgPath);
	DrawImage * drawImg = new DrawImage(img, data);
	img->initContentData(data);
	ObjDrawManager * drawMgr = img->getDrawMgr();
	drawMgr->initDraws(drawImg, NULL);
	int w = data->getImg()->width();
	int h = data->getImg()->height();
	initImgParam(w, h);
	param->init(0, 0, w, h);
	data->setWightAndHeight(w,h);
	ImageMouseInput * imgMouseInput = new ImageMouseInput(img);
	ObjMouseManager * mouseMrg = img->getMouseMgr();
	mouseMrg->setMouse(imgMouseInput);

	BaseScaleParam * scaleParam = new BaseScaleParam(img, ConstantsID::ContainerId::IMG);
	param->initScaleParam(scaleParam);

	return img;
}
ViewObj * ViewFactory::createExcel(BaseTemplate * templateParent, ViewObj * viewParent
	, int x, int y, int w, int h, int rowNumber, int colNumber, bool isCreateFromLoad)
{

	ViewObj * excel = new ViewObj(templateParent->getWriterMgr(), ConstantsID::ContainerId::EXCEL_VIEW);
	excel->setTempleteParent(templateParent);
	//LinearVHViewParam * rowViewParam = (LinearVHViewParam *)viewParent->getViewParam();
	// 显示参数
	LayoutViewParam * viewParam = new LayoutViewParam(excel);
	viewParam->init(0, 0, w, h);

	viewParam->initDefaultWH(0, 0);
	int paddingSize = 0;
	viewParam->initInside(paddingSize, paddingSize, paddingSize, paddingSize);

	DrawRect * rect = new DrawRect(excel, UIConstants::FillRectType::ROUND_LINE_AND_FILL
		, BaseConstants::NONE, UIConstants::MyColor::BLACK, 1);
	ObjDrawManager * drawMgr = excel->getDrawMgr();
	drawMgr->initDraws(rect, NULL);

	// 坐标布局
	ExcelLayout * excelLayout = new ExcelLayout(excel, isCreateFromLoad);
	//虚线
	ViewObj * dotLine = new   ViewObj(excel->getWriterMgr(), ConstantsID::ContainerId::EXCEL_LINE);
	BaseViewParam * lineViewParam = new BaseViewParam(dotLine);
	lineViewParam->init(0, 0, 0, 0);
	lineViewParam->initDefaultWH(BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE, 0);
	DrawLine * drawLine = new DrawLine(dotLine);
	drawLine->setLineParam(UIConstants::MyColor::GRAY, 1, UIConstants::LineType::SINGLE_DOT_LINE);
	ObjDrawManager * lineDrawMgr = dotLine->getDrawMgr();
	lineDrawMgr->initDraws(drawLine, NULL);
	excelLayout->mDragLine = dotLine;

	if (!isCreateFromLoad)
	{
		excelLayout->generateCells(rowNumber, colNumber, excel);
	}

	//createExcelCells(excel,4,4);
	return excel;
}
ViewObj * ViewFactory::createExcelCell(ViewObj * viewExcel, ExcelCellData * excelCellData)
{
	ExcelLayout * excelLayout = NULL;
	if (viewExcel->getObjID() != ConstantsID::ContainerId::EXCEL_VIEW)
	{
		using namespace std;
		cout << "ViewFactory::createExcelCell viewExcel not excel view";
		return NULL;
	}
	excelLayout = (ExcelLayout *)viewExcel->getLayout();

	ExcelTemplete* excelTemplete = (ExcelTemplete*)viewExcel->getTempParent();
	excelCellData->mRootTemplete = TempleteFactory::createExcelCellTemplate(excelTemplete);

	WriterManager* writerMgr = excelTemplete->getWriterMgr();

	ViewObj * cellPage = createExcelPage(writerMgr, excelCellData->mLeft->getPosition(), excelCellData->mTop->getPosition()
		, excelCellData->mRight->getPosition() - excelCellData->mLeft->getPosition(), excelCellData->mBottom->getPosition() - excelCellData->mTop->getPosition(), excelTemplete);

	excelCellData->setCellTempleteOwner(excelCellData->mRootTemplete);
	cellPage->initContentData(excelCellData);
	excelCellData->mRootTemplete->setExcelCellData(excelCellData);
	excelCellData->setViewOwner(cellPage);

	//生成页段落
	ViewObj* preFocus = NULL;
	char preDir = 0;
	excelTemplete->getFocusObjAndDir(preFocus, preDir);
	ViewObj * enter = createEnter(writerMgr, NULL);
	excelTemplete->getOperateMgr()->setFocusOfEndPress(NULL, -1);
	excelCellData->mRootTemplete->getChildrenMgr()->addChild(enter, -1, true);
	enter->getViewParent()->getViewParam()->refreshHByChildren(true);
	if (preFocus)
	{
		if (preDir == BaseConstants::Direction::RIGHT)
		{
			preFocus->setFocus(true);
		}
		else
		{
			preFocus->setFocus(false);
		}
	}

	//txtLayer->getParentObjById(BaseConstants::ContainerId::EXCEL_PAGE)->getViewParam()->refreshHByChildren(false);
	return cellPage;
}
ViewObj * ViewFactory::createChoiceBoxRectImg(WriterManager * writerMgr, BaseTemplate * templateParent, int userID)
{
	TempleteViewObj * choiceBoxView = new TempleteViewObj(writerMgr, templateParent, ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_RECT_VIEW);
	ImgViewParam * param = new ImgViewParam(choiceBoxView);
	ImgConTentData * data = new ImgConTentData(choiceBoxView, NULL, StrConstants::MyKeyBoard::CHOICE_BOX_RECT);
	choiceBoxView->initContentData(data);
	DrawImage * img = new DrawImage(choiceBoxView, data);
	ObjDrawManager * drawMgr = choiceBoxView->getDrawMgr();
	drawMgr->initDraws(img, NULL);
	int w = data->getImg()->width();
	int h = data->getImg()->height();
	param->init(0, 0, w, h);
	int chooseObjId = templateParent->getChooseObjId();
	if (chooseObjId < 0)
	{
		choiceBoxView->setChooseObjId(templateParent->getObjID());
	}
	else
	{
		choiceBoxView->setChooseObjId(chooseObjId);
	}
	return choiceBoxView;
}
ViewObj * ViewFactory::createChoiceBoxRadioImg(WriterManager * writerMgr, BaseTemplate * templateParent, int userID)
{
	TempleteViewObj * choiceBoxView = new TempleteViewObj(writerMgr, templateParent, ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_RADIO_MARK_VIEW);
	ImgViewParam * param = new ImgViewParam(choiceBoxView);
	ImgConTentData * data = new ImgConTentData(choiceBoxView, NULL, StrConstants::MyKeyBoard::CHOICE_BOX_RADIO_MARK);
	choiceBoxView->initContentData(data);
	DrawImage * img = new DrawImage(choiceBoxView, data);
	ObjDrawManager * drawMgr = choiceBoxView->getDrawMgr();
	drawMgr->initDraws(img, NULL);
	int w = data->getImg()->width();
	int h = data->getImg()->height();
	param->init(0, 0, w, h);
	int chooseObjId = templateParent->getChooseObjId();
	if (chooseObjId < 0)
	{
		choiceBoxView->setChooseObjId(templateParent->getObjID());
	}
	else
	{
		choiceBoxView->setChooseObjId(chooseObjId);
	}
	return choiceBoxView;
}
ViewObj * ViewFactory::createChoiceBoxMultipleImg(WriterManager * writerMgr, BaseTemplate * templateParent, int userID)
{
	TempleteViewObj * choiceBoxView = new TempleteViewObj(writerMgr, templateParent, ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_MULTPLE_MARK_VIEW);
	ImgViewParam * param = new ImgViewParam(choiceBoxView);
	ImgConTentData * data = new ImgConTentData(choiceBoxView, NULL, StrConstants::MyKeyBoard::CHOICE_BOX_MULTPLE_MARK);
	choiceBoxView->initContentData(data);
	DrawImage * img = new DrawImage(choiceBoxView, data);
	ObjDrawManager * drawMgr = choiceBoxView->getDrawMgr();
	drawMgr->initDraws(img, NULL);
	int w = data->getImg()->width();
	int h = data->getImg()->height();
	param->init(0, 0, w, h);
	return choiceBoxView;
}
ViewObj * ViewFactory::createChoiceBoxContainerView(WriterManager * writerMgr, BaseTemplate * templateParent, int userID)
{
	TempleteViewObj * choiceBoxView = new TempleteViewObj(writerMgr, templateParent, ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW);
	BaseViewParam * viewParam = new BaseViewParam(choiceBoxView);
	viewParam->init(0, 0, 0, 0);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	BaseChildrenManager * layout = new BaseChildrenManager(choiceBoxView);
	int chooseObjId = templateParent->getChooseObjId();
	if (chooseObjId < 0)
	{
		choiceBoxView->setChooseObjId(templateParent->getObjID());
	}
	else
	{
		choiceBoxView->setChooseObjId(chooseObjId);
	}
	return choiceBoxView;
}
void ViewFactory::initImgParam(int& w, int& h)
{
	float Scaling = 0;
	int diffMaxW = w - UIConstants::MAX_ROW_INSIDE_W;
	int diffMaxH = h - UIConstants::MAX_ROW_INSIDE_H;
	if ((diffMaxW > 0) || (diffMaxH > 0))
	{
		if (diffMaxW > diffMaxH)
		{
			Scaling = UIConstants::MAX_ROW_INSIDE_W / (w * 1.0);
		}
		else
		{
			Scaling = UIConstants::MAX_ROW_INSIDE_H / (h * 1.0);
		}
		w = w * Scaling + 0.5;
		h = h * Scaling + 0.5;
		return;
	}
	int diffMinW = UIConstants::MIN_ROW_INSIDE_W - w;
	int diffMinH = UIConstants::MIN_ROW_INSIDE_H - h;
	if ((diffMinW > 0) || (diffMinH > 0))
	{
		if (diffMinW > diffMinH)
		{
			Scaling = UIConstants::MIN_ROW_INSIDE_W / (w * 1.0);
		}
		else
		{
			Scaling = UIConstants::MIN_ROW_INSIDE_H / (h * 1.0);
		}
		w = w * Scaling + 0.5;
		h = h * Scaling + 0.5;
		return;
	}
}
