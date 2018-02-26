#include "htmlOutPutManger.h"
#include "ViewObj.h"
#include "ConstantsID.h"
#include "qdebug.h"
#include "BaseViewParam.h"
#include "DrawStr.h"
#include "ObjDrawManager.h"
#include "TxtViewParam.h"
#include "htmlBaseObj.h"
#include "StrContentData.h"
#include "QColor.h"

#include "OperateMgr.h"

#include "ExcelCrossParentMgr.h"
#include "VerticalViewParam.h"

#include "ImgConTentData.h"
#include "BaseConstants.h"
#include "UIConstants.h"
#include "DrawImage.h"
#include "RowViewParam.h"
#include "qdir.h"
#include "QDateTime.h"
#include "QBuffer.h"
#include <cstdlib>
#include "DrawRect.h"
#include "CrossParentMgr.h"
#include "BaseMath.h"
const QString htmlOutPutManger::IMG_DIR_PATH = "./html_image/";
const QString htmlOutPutManger::IMG_SUFFIX = ".png";
htmlOutPutManger::htmlOutPutManger(int classid) :BaseElement(classid)
, mTemptResultList(NULL)
{

}


htmlOutPutManger::~htmlOutPutManger()
{
	if (mTemptResultList)
	{
		mTemptResultList = NULL;
	}
}
void htmlOutPutManger::getHtmlObj(QList<htmlBaseObj*> * resultList, ViewObj * rootView)
{
	if (!resultList)
	{
		return;
	}
	srand(1000);
	mTemptResultList = resultList;
	ViewObj * pagePan = (ViewObj *)rootView->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, true);
	handleViewObj(pagePan, 0, 0);
	removeSamePositionLine();

}
void htmlOutPutManger::removeSamePositionLine()
{
	htmlBaseObj* outObj = NULL;
	htmlBaseObj* tempObj = NULL;
	QPoint outStartPoint;
	QPoint outEndPoint;
	QPoint tempStartPoint;
	QPoint tempEndPoint;
	int MAX_VALUE = 3;
	for (int i = 0; i < mTemptResultList->size();)
	{
		outObj = mTemptResultList->at(i);
		if (outObj->getType() == htmlBaseObj::LINE)
		{
			for (int m = i + 1; m < mTemptResultList->size();)
			{
				tempObj = mTemptResultList->at(m);
				if (tempObj->getType() == htmlBaseObj::LINE)
				{
					outStartPoint = outObj->getStartPoint();
					outEndPoint = outObj->getEndPoint();
					tempStartPoint = tempObj->getStartPoint();
					tempEndPoint = tempObj->getEndPoint();
					if (BaseMath::getAbs(outStartPoint.x() - tempStartPoint.x()) <= MAX_VALUE &&BaseMath::getAbs(outStartPoint.y() - tempStartPoint.y()) <= MAX_VALUE && BaseMath::getAbs(outEndPoint.x() - tempEndPoint.x()) <= MAX_VALUE && BaseMath::getAbs(outEndPoint.y() - tempEndPoint.y()) <= MAX_VALUE)
					{
						mTemptResultList->removeAt(m);
					}
					else
					{
						m++;
					}
				}
				else
				{
					m++;
				}
			}
		}
		i++;
	}
}
void htmlOutPutManger::handleViewObj(ViewObj * view, int x, int y)
{
	if (!view)
	{
		return;
	}
	int preY = y;
	//bool isRowView = false;
	OperateMgr * operateMgr = view->getOperateMgr();
	int objId = view->getObjID();
	//if (objId == ConstantsID::ContainerId::TXT || objId == ConstantsID::ContainerId::PAGE_NUMBER || objId == ConstantsID::ContainerId::TMPLATE_TXT)
	if (view->isCommonTxt())
	{
		htmlBaseObj * htmlObj = NULL;
		htmlBaseObj * htmlLineObj = NULL;
		handleTextViewObj(view, x, y, htmlObj, htmlLineObj);
		return;
	}
	else if (objId == ConstantsID::ContainerId::IMG)
	{
		handleImgViewObj(view, x, y);
		return;
	}
	else if (objId == ConstantsID::ContainerId::EXCEL_PAGE)
	{
		/**************************************************** 2017.8.12 霍坤 *******************************************************/
		// 之前查找方式是往上找txtLayer层，但新版页眉页脚里的txtLayer和页内容里的txtLayer层的id不同，所以报错
		ViewObj * parentExcelRow = view->getParentViewById(ConstantsID::ContainerId::NORMAL_ROW);
		ExcelCrossParentMgr * crossParentMgr = (ExcelCrossParentMgr *)parentExcelRow->getCrossParentMgr();
		if (crossParentMgr&&crossParentMgr->haveCrossParent())
		{
			BaseViewParam * viewParam = view->getViewParam();
			ViewObj * curPage = operateMgr->getEffectLoopPage();
			ViewObj * layer = (ViewObj *)curPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			VerticalViewParam * layerParam = (VerticalViewParam *)layer->getViewParam();
			int h = viewParam->getTotalH();
			int absY = layerParam->getAbsY(curPage);
			int layerMinY = absY + layerParam->getInsideTop();
			int layerMaxClipY = layerMinY + layerParam->getInsideH();
			//int tempY = viewParam->getY();
			int tY = y;
			//int tY = tempY + y;
			if (tY > layerMaxClipY)
			{
				return;
			}
			if (tY + h < layerMinY)
			{
				return;
			}
		}
		else
		{
			ViewObj* txtLayer = parentExcelRow->getViewParent();
			//ViewObj* nomalRow = view->getParentViewById(ConstantsID::ContainerId::CONTENT_TXT_LAYER);
			/**************************************************** 2017.8.12 霍坤 *******************************************************/
			int wRow = txtLayer->getViewParam()->getTotalW();
			int hRow = txtLayer->getViewParam()->getTotalH();
			int xRelativeTxtLayer = view->getViewParam()->getAbsX(txtLayer);
			int yRelativeTxtLayer = view->getViewParam()->getAbsY(txtLayer);
			int pageW = view->getViewParam()->getTotalW();
			int pageH = view->getViewParam()->getTotalH();
			if (!BaseMath::isRectCross(0, 0, wRow, hRow, xRelativeTxtLayer, yRelativeTxtLayer
				, xRelativeTxtLayer + pageW, yRelativeTxtLayer + pageH))
			{
				return;
			}
		}
		handleExcelPageViewObj(view, x, y);
	}
	else if (objId == ConstantsID::ContainerId::PAGE_TITLE || objId == ConstantsID::ContainerId::PAGE_BOTTOM)
	{
		//handleExcelPageViewObj(view, x, y);
		//handleViewObj(view, x, y);
	}
	else if (objId == ConstantsID::ContainerId::EXCEL_LINE)
	{
		handleExcelPageViewObj(view, x, y);
	}
	else if (objId == ConstantsID::ContainerId::NORMAL_PAGE)
	{
		operateMgr->setEffectLoopPage(view);
	}
	else if (objId == ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW)
	{
		QList<BaseContainer *> * childrenBase = view->getChildren();
		if (childrenBase)
		{
			ViewObj * tempObj;
			BaseViewParam * viewParam;
			for (int i = 0; i < childrenBase->length(); i++)
			{
				tempObj = (ViewObj*)childrenBase->at(i);
				viewParam = tempObj->getViewParam();
				handleViewObj(tempObj, viewParam->getX()+2 + x, viewParam->getY() + y);
			}
		}
		return;
	}
	/*else if (objId == ConstantsID::ContainerId::NORMAL_ROW)
	{
	isRowView = true;
	}*/
	QList<BaseContainer *> * childrenBase = view->getChildren();
	if (!childrenBase)
	{
		return;
	}
	//QList<ViewObj *> * children = (QList<ViewObj *>*)childrenBase;
	//先检测一遍是否有相同
	ViewObj * viewGroup;
	BaseContainer * tempObj;
	ViewObj* tempViewObj;
	BaseContainer * tempNextObj;
	ViewObj* tempNextViewObj;
	QList<ViewObj *> tempSameTxtList;
	bool isSameInTxtStrData = false;
	int nextIndex;
	for (int i = 0; i < childrenBase->length(); i++)
	{
		if (objId == ConstantsID::ContainerId::EXCEL_TXT_LAYER && childrenBase->length() == 6)
		{
			int a = 0;
		}
		try
		{
			tempObj = childrenBase->at(i);
			isSameInTxtStrData = false;
			if (tempObj->isCommonTxt())
			{
				if (tempSameTxtList.size() == 0)
				{
					tempViewObj = (ViewObj *)tempObj;
					tempSameTxtList.push_back(tempViewObj);
				}
				nextIndex = i + 1;
				if (nextIndex < childrenBase->length())
				{
					tempNextObj = childrenBase->at(nextIndex);
					if (tempNextObj->isCommonTxt())
					{
						tempNextViewObj = (ViewObj *)tempNextObj;
						if (isSameTxtContentDataViewObj(tempViewObj, tempNextViewObj))
						{
							isSameInTxtStrData = true;
							tempSameTxtList.push_back(tempNextViewObj);
						}
					}
				}
			}

			if (isSameInTxtStrData)
			{
				continue;
			}
			else
			{
				if (tempSameTxtList.size() > 0)
				{
					handleSameTxtViewObjList(tempSameTxtList, x, y);
					continue;
				}
				else
				{
					viewGroup = (ViewObj *)tempObj;
					BaseViewParam * viewParam = viewGroup->getViewParam();
					int tempY = viewParam->getY();
					int h = viewParam->getTotalH();
					if (tempObj->isObj(ConstantsID::ContainerId::PAGE_TITLE) || tempObj->isObj(ConstantsID::ContainerId::PAGE_BOTTOM))
					{
					}
					else
					{
						ExcelCrossParentMgr * crossParentMgr = (ExcelCrossParentMgr *)viewGroup->getCrossParentMgr();
						if (crossParentMgr&&crossParentMgr->haveCrossParent())
						{
							ViewObj * curPage = operateMgr->getEffectLoopPage();
							ViewObj * layer = (ViewObj *)curPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
								->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
							VerticalViewParam * layerParam = (VerticalViewParam *)layer->getViewParam();
							int layerMinY = layerParam->getInsideTop();
							int layerMaxClipY = layerMinY + layerParam->getInsideH();
							int disY = crossParentMgr->getDisY(layer);
							tempY += disY;
							//int absY = viewParam->getAbsY(layer);
							if (disY >= layerMaxClipY)
								continue;
							if (disY + h < layerMinY)
							{
								continue;
							}
							//crossParentMgr->setCurParent(view);
						}
						else if (viewGroup->isRow() && viewGroup->getViewParent()->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER))
						{
							ViewObj * parentExcelRow = viewGroup->getViewParent()->getParentViewById(ConstantsID::ContainerId::NORMAL_ROW);
							if (parentExcelRow->isExcelRow())
							{
								crossParentMgr = (ExcelCrossParentMgr *)parentExcelRow->getCrossParentMgr();
								if (crossParentMgr&&crossParentMgr->haveCrossParent())
								{
									ViewObj * curPage = operateMgr->getEffectLoopPage();
									ViewObj * layer = (ViewObj *)curPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
										->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
									//ViewObj * layer = viewGroup->findViewFromParent(ConstantsID::ContainerId::CONTENT_TXT_LAYER, true);
									VerticalViewParam * layerParam = (VerticalViewParam *)layer->getViewParam();
									int h = viewParam->getTotalH();
									int absY = layerParam->getAbsY(curPage);
									int layerMinY = absY + layerParam->getInsideTop();
									int layerMaxClipY = layerMinY + layerParam->getInsideH();
									int tY = tempY + y;
									if (tY > layerMaxClipY)
									{
										continue;
									}
									if (tY + h < layerMinY)
									{
										continue;
									}
								}
							}
						}
					}
					handleViewObj(viewGroup, viewParam->getX() + x, tempY + y);
				}
			}


		}
		catch (...)
		{
		}

	}
}
void htmlOutPutManger::handleTextViewObj(ViewObj * view, int x, int y, htmlBaseObj  * &outHtmlObj, htmlBaseObj  * &outLineHtmlObj)
{
	ObjDrawManager * drawManger = view->getDrawMgr();
	DrawStr *  draw = (DrawStr*)drawManger->getBottomDraw();
	StrContentData * focusTxtData = (StrContentData *)view->getContentData();
	TxtViewParam* mTxtViewParam = (TxtViewParam *)view->getViewParam();
	QColor* fontColor = focusTxtData->getFontColorObj();
	//int color = fontColor->rgba();
	char flowType = focusTxtData->getFlowType();


	char underlineType = focusTxtData->getUnderLineType();
	short underlineThick = focusTxtData->getUnderLineThick();
	bool isBold = focusTxtData->getBold();
	//QFont *font = focusTxtData->getFont();
	//QString family = font->family();
	int fontSize = focusTxtData->getPointSize();
	//int weight = font->weight();
	bool italic = focusTxtData->getItalic();

	//QDateTime current_date_time=QDateTime::currentDateTime();
	QString str = focusTxtData->getString();
	if (view->isPageNumber())
	{
		ViewObj * page = NULL;
		ViewObj * container = view->getParentViewById(ConstantsID::ContainerId::PAGE_TITLE);
		if (!container)
		{
			container = view->getParentViewById(ConstantsID::ContainerId::PAGE_BOTTOM);
		}
		//page = container->getCrossParentMgr()->getCurParent();
		//int pageIdx = page->getViewIndex();
		//str = QString::number(pageIdx + 1);
	}
	TxtViewParam * viewData = (TxtViewParam *)view->getViewParam();

	//const int w = viewData->getTotalW();//宽度
	//if (w == 0)
	//{
	//mTxtParam->refreshTxtParam();
	//}
	//const int h = viewData->getTotalH();//高度
	//int parentH = mOwner->getParent()->mViewData->getInsideHeight();

	// 背景填充
	//if (fillColor->alpha() > 0)
	//	pPainter.fillRect(preX, preY, w, h, *fillColor);

	int w = mTxtViewParam->getTotalW();//宽度
	int h = mTxtViewParam->getTotalH();//高度

	// 背景填充
	//if (fillColor && fillColor->alpha() > 0)
	//	pPainter.fillRect(preX, preY, w, h, *fillColor);

	int strH = mTxtViewParam->getSelfH();
	int strAscent = mTxtViewParam->getAscent();
	int strDescent = mTxtViewParam->getDescent();
	RowViewParam * rowParam = draw->getRowParam();
	int rowH = rowParam->getTotalH();
	int rowDescent = rowParam->getDescent();
	//int rowH = rowParam->getSelfH();
	int rowInsideH = rowParam->getInsideH();
	int rowInsideTop = rowParam->getInsideTop();

	//QFontMetrics * graphics = focusTxtData->getGraphics();
	QFontMetrics * graphics = NULL;
	//pPainter.drawText(x, y, str);
	int destFontSize = fontSize / 2;
	if (destFontSize < 1)
	{
		destFontSize = 1;
	}
	y += rowInsideTop;
	int normalStrY = y + rowInsideH - rowDescent;
	int curStrH = strAscent;
	int destY = 0;
	int destX = x;
	int destSize = fontSize;
	switch (flowType)
	{
		// 正常绘制
	case BaseConstants::NONE:
		destY = normalStrY;
		break;
		// 上浮标
	case BaseConstants::Direction::UP:
		destY = y + strAscent;
		break;
		// 下浮标
	case BaseConstants::Direction::DOWN:
		destY = y + rowInsideH - strDescent;
		break;
	}

	htmlBaseObj * htmlObj = new htmlBaseObj(ConstantsID::ClassId::HTML_OBJ);
	htmlObj->setType(htmlBaseObj::STR);
	htmlObj->setStr(str);
	htmlObj->setFontColor(*fontColor);
	htmlObj->setFontSize(destSize);
	htmlObj->setItalic(italic);
	if (str.compare("M") == 0 || str.compare("m") == 0)
	{
		int deW = mTxtViewParam->getSelfW() / 4;
		htmlObj->setX(destX - deW);
	}
	else
	{
		htmlObj->setX(destX);
	}

	htmlObj->setY(destY);
	htmlObj->setBold(isBold);
	mTemptResultList->push_back(htmlObj);
	outHtmlObj = htmlObj;
	// 下划线
	bool isNeedDrawLine = false;
	switch (underlineType)
	{
		// 单下划实线
	case UIConstants::LineType::SINGLE_LINE:
		isNeedDrawLine = true;
		break;
		// 单下划虚线
	case UIConstants::LineType::SINGLE_DOT_LINE:
		isNeedDrawLine = true;
		break;
	case BaseConstants::NONE:
		return;
	}
	if (isNeedDrawLine)
	{
		htmlBaseObj * htmlObjLine = new htmlBaseObj(ConstantsID::ClassId::HTML_OBJ);
		htmlObjLine->setType(htmlBaseObj::LINE);
		htmlObjLine->setStartPoint(x, y + rowH);
		htmlObjLine->setEndPoint(x + w, y + rowH);
		mTemptResultList->push_back(htmlObjLine);
		outLineHtmlObj = htmlObjLine;
	}

}
void htmlOutPutManger::handleImgViewObj(ViewObj * view, int x, int y)
{
	BaseViewParam * viewParam = view->getViewParam();
	int w = viewParam->getTotalW();
	int h = viewParam->getTotalH();
	int selfW = viewParam->getSelfW();
	int selfH = viewParam->getSelfH();
	//int baseline = 0;
	ViewObj * row = view->getViewParent();
	int rowH = h;
	int tempY = y;
	if (row && row->getObjID() == ConstantsID::ContainerId::NORMAL_ROW)
	{
		RowViewParam * rowParam = ((RowViewParam *)row->getViewParam());
		rowH = rowParam->getTotalH() - rowParam->getInsideTop() - rowParam->getInsideBottom();//高度
		//baseline = rowParam->getAscent();
		tempY = y + rowH - selfH;
		//tempY = y;
		//tempY = y + rowH - selfH - baseline;
	}
	else
	{
		rowH = 0;
		tempY = y;
	}


	//// 正常图片

	///************add by cao kewen(test cutImage)********/
	ObjDrawManager * drawManger = view->getDrawMgr();
	DrawImage *  draw = (DrawImage *)drawManger->getBottomDraw();
	ImgConTentData * imgParam = (ImgConTentData*)view->getContentData();
	QImage * image = imgParam->getSnap();
	// 如果有截图，则直接绘制截图
	if (image != NULL)
	{
		//pPainter.drawImage(*rect, *snap);
	}
	///****************************************************/

	else
	{
		image = imgParam->getImg();
	}
	//QString path(saveImage(image)) ;
	//if (path.size()>0)
	{
		htmlBaseObj * htmlObjImg = new htmlBaseObj(ConstantsID::ClassId::HTML_OBJ);
		htmlObjImg->setType(htmlBaseObj::IMG);
		htmlObjImg->setX(x);
		htmlObjImg->setY(tempY);
		htmlObjImg->setWidth(selfW);
		htmlObjImg->setHeight(selfH);
		saveImageInByteArray(image, htmlObjImg->getByteArray());
		//htmlObjImg->setPicturePath(path);
		mTemptResultList->push_back(htmlObjImg);
	}
}
void htmlOutPutManger::handleExcelPageViewObj(ViewObj * view, int x, int y)
{
	ObjDrawManager * drawManger = view->getDrawMgr();
	DrawRect *  draw = (DrawRect *)drawManger->getBottomDraw();
	//DrawRectParam * rectParam = (DrawRectParam*)view->getParam();

	//需要判断是否显示


	char fillType = draw->getFillRectType();

	// 背景填充色
	//QColor *fillColor = rectParam->getFillColor();
	// 边框色
	QColor *roundLineColor = draw->getRoundLineColor();
	// 边框线厚度
	short roundLineThick = draw->getRoundLineThick();

	// 上下左右边线显示情况
	char topLineViewType = draw->getTopLineViewType();
	char bottomLineViewType = draw->getBottomLineViewType();
	char leftLineViewType = draw->getLeftLineViewType();
	char rightLineViewType = draw->getRightLineViewType();


	bool isWholeRoundLine = true;
	if (topLineViewType != UIConstants::LineType::SINGLE_LINE || bottomLineViewType != UIConstants::LineType::SINGLE_LINE
		|| leftLineViewType != UIConstants::LineType::SINGLE_LINE || rightLineViewType != UIConstants::LineType::SINGLE_LINE)
	{
		isWholeRoundLine = false;
	}
	//
	BaseViewParam * viewParam = view->getViewParam();

	if (fillType == BaseConstants::NONE)
		return;
	int w = viewParam->getTotalW();
	int h = viewParam->getTotalH();
	{
		switch (fillType)
		{
		case UIConstants::FillRectType::ONLY_ROUND_RECT:
			//pPainter.setPen(*roundLineColor);
			//pPainter.drawRect(mRect);
			//pPainter.drawRect(x, y, w, h);

			break;
		case UIConstants::FillRectType::ONLY_FILL:
		{
													 htmlBaseObj * htmlObjLine = new htmlBaseObj(ConstantsID::ClassId::HTML_OBJ);
													 htmlObjLine->setType(htmlBaseObj::LINE);
													 htmlObjLine->setStartPoint(x, y + h);
													 htmlObjLine->setEndPoint(x + w, y + h);
													 mTemptResultList->push_back(htmlObjLine);
													 return;
		}
			break;
		case UIConstants::FillRectType::ROUND_LINE_AND_FILL:
			//QPen pen1(Qt::darkGray, 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin);
			bool haveTopLine = false;
			bool haveBottomLine = false;
			bool haveLeftLine = false;
			bool haveRightLine = false;
			if (isWholeRoundLine)
			{
				//pPainter.setPen(*roundLineColor);
				//pPainter.setBrush(*fillColor);
				//pPainter.drawRect(x, y, w, h);
				haveTopLine = true;
				haveBottomLine = true;
				haveLeftLine = true;
				haveRightLine = true;
			}
			else
			{
				//pPainter.fillRect(x, y, w, h, *fillColor);
				if (topLineViewType > BaseConstants::NONE)
				{
					haveTopLine = true;
					//setLineType(pPainter, roundLineColor, roundLineThick, topLineViewType);

					//pPainter.drawLine(x, y, x + w, y);
				}
				if (bottomLineViewType > BaseConstants::NONE)
				{
					haveBottomLine = true;
					//setLineType(pPainter, roundLineColor, roundLineThick, bottomLineViewType);
					//pPainter.drawLine(x, y + h, x + w, y + h);
				}
				if (leftLineViewType > BaseConstants::NONE)
				{
					haveLeftLine = true;
					//setLineType(pPainter, roundLineColor, roundLineThick, leftLineViewType);
					//pPainter.drawLine(x, y, x, y + h);
				}
				if (rightLineViewType > BaseConstants::NONE)
				{
					haveRightLine = true;
					//setLineType(pPainter, roundLineColor, roundLineThick, rightLineViewType);
					//pPainter.drawLine(x + w, y, x + w, y + h);
				}
			}


			//top
			int lineThick = 1;
			if (haveTopLine)
			{
				htmlBaseObj * htmlObjLine = new htmlBaseObj(ConstantsID::ClassId::HTML_OBJ);
				htmlObjLine->setType(htmlBaseObj::LINE);
				htmlObjLine->setStartPoint(x, y);
				htmlObjLine->setEndPoint(x + w, y);
				mTemptResultList->push_back(htmlObjLine);
			}

			//bottom
			if (haveBottomLine)
			{
				htmlBaseObj * htmlObjLine = new htmlBaseObj(ConstantsID::ClassId::HTML_OBJ);
				htmlObjLine->setType(htmlBaseObj::LINE);
				htmlObjLine->setStartPoint(x, y + h);
				htmlObjLine->setEndPoint(x + w, y + h);
				mTemptResultList->push_back(htmlObjLine);
			}
			//left
			if (haveLeftLine)
			{
				htmlBaseObj * htmlObjLine = new htmlBaseObj(ConstantsID::ClassId::HTML_OBJ);
				htmlObjLine->setType(htmlBaseObj::LINE);
				htmlObjLine->setStartPoint(x, y);
				htmlObjLine->setEndPoint(x, y + h);
				mTemptResultList->push_back(htmlObjLine);
			}
			//right
			if (haveRightLine)
			{
				htmlBaseObj * htmlObjLine = new htmlBaseObj(ConstantsID::ClassId::HTML_OBJ);
				htmlObjLine->setType(htmlBaseObj::LINE);
				htmlObjLine->setStartPoint(x + w + lineThick, y);
				htmlObjLine->setEndPoint(x + w + lineThick, y + h);
				mTemptResultList->push_back(htmlObjLine);
			}
			break;
		}
	}
}
QString htmlOutPutManger::saveImage(QImage *image)
{
	QDir d;
	if (!d.exists(IMG_DIR_PATH))
	{
		d.mkpath(IMG_DIR_PATH);
	}
	QDateTime time = QDateTime::currentDateTime();
	QString str = time.toString("yyyy-MM-dd hh:mm:ss.z");
	int radom = rand() % 23;
	QString path = IMG_DIR_PATH + str + QString::number(radom) + IMG_SUFFIX;
	path.remove(":");
	QFile *file = new QFile(path);
	if (!file->open(QIODevice::ReadWrite))
	{
		return NULL;
	}
	QByteArray ba;
	QBuffer buffer(&ba);
	buffer.open(QIODevice::WriteOnly);
	image->save(&buffer, "PNG");
	file->write(ba);
	buffer.close();
	file->close();
	return path;
}
void htmlOutPutManger::saveImageInByteArray(QImage *image, QByteArray * byteArray)
{
	if (byteArray)
	{
		QBuffer buffer(byteArray);
		buffer.open(QIODevice::WriteOnly);
		image->save(&buffer, "PNG");
		buffer.close();
	}
}
void htmlOutPutManger::getTxt(ViewObj * rootView, QString & str)
{
	if (!rootView)
	{
		return;
	}
	int objId = rootView->getObjID();
	//if (objId == ConstantsID::ContainerId::TXT || objId == ConstantsID::ContainerId::PAGE_NUMBER || objId == ConstantsID::ContainerId::TMPLATE_TXT)
	if (rootView->isTxt())
	{
		ObjDrawManager * drawManger = rootView->getDrawMgr();
		DrawStr *  draw = (DrawStr*)drawManger->getBottomDraw();
		StrContentData * focusTxtData = (StrContentData *)rootView->getContentData();
		QString txt = focusTxtData->getString();
		str += txt;
		return;
	}
	else if (objId == ConstantsID::ContainerId::IMG)
	{
		return;
	}
	else if (objId == ConstantsID::ContainerId::EXCEL_PAGE)
	{

	}
	else if (objId == ConstantsID::ContainerId::PAGE_TITLE || objId == ConstantsID::ContainerId::PAGE_BOTTOM)
	{
		return;
	}
	else if (objId == ConstantsID::ContainerId::EXCEL_LINE)
	{
		return;
	}
	else if (objId == ConstantsID::ContainerId::ENTER)
	{
		str += "\n";
	}
	QList<BaseContainer *> * childrenBase = rootView->getChildren();
	if (!childrenBase)
	{
		return;
	}
	//QList<ViewObj *> * children = (QList<ViewObj *>*)childrenBase;
	ViewObj * viewGroup;
	BaseContainer * tempObj;
	for (int i = 0; i < childrenBase->length(); i++)
	{
		tempObj = childrenBase->at(i);
		viewGroup = (ViewObj *)tempObj;
		getTxt(viewGroup, str);

	}
}
bool htmlOutPutManger::isSameTxtContentDataViewObj(ViewObj * firstView, ViewObj * secondView)
{
	StrContentData * firstTxtData = (StrContentData *)firstView->getContentData();
	StrContentData * secondTxtData = (StrContentData *)secondView->getContentData();
	bool isSame = firstTxtData->isSameTxtType(secondTxtData);
	return isSame;
}
void htmlOutPutManger::handleSameTxtViewObjList(QList<ViewObj *>& txtViewList, int x, int y)
{
	//本应该排序，按x坐标
	ViewObj * tempView;
	QString str;
	for (int i = 0; i < txtViewList.size(); i++)
	{
		tempView = txtViewList.at(i);
		/*ObjDrawManager * drawManger = tempView->getDrawMgr();
		DrawStr *  draw = (DrawStr*)drawManger->getBottomDraw();*/
		StrContentData * focusTxtData = (StrContentData *)tempView->getContentData();
		QString txt = focusTxtData->getString();
		str += txt;
	}
	tempView = txtViewList.at(0);
	BaseViewParam * mViewParam = tempView->getViewParam();
	htmlBaseObj * htmlObj = NULL;
	htmlBaseObj * htmlLineObj = NULL;
	handleTextViewObj(tempView, x + mViewParam->getX(), y + mViewParam->getY(), htmlObj, htmlLineObj);
	if (txtViewList.size() > 1)
	{
		htmlObj->setStr(str);
		if (htmlLineObj)
		{
			ViewObj * tempView2 = txtViewList.at(txtViewList.size() - 1);
			TxtViewParam* mTxtViewParam = (TxtViewParam *)tempView2->getViewParam();
			int startX = htmlLineObj->getStartPoint().x();
			int endX = x + mTxtViewParam->getX() + mTxtViewParam->getTotalW();
			/*int strW = endX - startX;
			int retractW = strW*0.24;
			endX = endX - retractW;*/
			int endY = htmlLineObj->getEndPoint().y();
			htmlLineObj->setEndPoint(endX, endY);
		}

	}
	txtViewList.clear();
}