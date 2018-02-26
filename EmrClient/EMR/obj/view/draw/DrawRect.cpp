#include "DrawRect.h"

#include "ViewObj.h"
#include "BaseViewParam.h"

#include "BaseIOTools.h"
#include "ConstantsID.h"
#include "UIConstants.h"
#include "BaseConstants.h"
#include "ConstantsNumber.h"
#include "ObjDrawManager.h"
#include "BaseMath.h"
#include "BaseTools.h"
#include "qmap.h"
DrawRect::DrawRect(ViewObj * viewOwner, char type, int backColor, int lineColor, short thick, bool isCreateOnLoad) : BaseDraw(viewOwner, ConstantsID::ClassId::DRAW_RECT)
{
	if (isCreateOnLoad)
	{
		mBackGroundColor = 0;
		mRoundLineColor = 0;
		mTopLineColor = 0;
		mTopLineColorObj = NULL;
		mBottomLineColor = 0;
		mBottomLineColorObj = NULL;
		mLeftLineColor = 0;
		mLeftLineColorObj = NULL;
		mRightLineColor = 0;
		mRightLineColorObj = NULL;
		mFillRectType = 0;
		mBackGroundColorObj =NULL;
		mRoundLineColorObj = NULL;
		mRoundLineThick = 0;
	}
	else
	{
		mBackGroundColor = backColor;
		mRoundLineColor = lineColor;
		mFillRectType = type;
		mBackGroundColorObj = BaseMath::createColor(mBackGroundColor);
		mRoundLineColorObj = BaseMath::createColor(mRoundLineColor);
		mRoundLineThick = thick;
		mTopLineColor = 0;
		mTopLineColorObj = NULL;
		mBottomLineColor = 0;
		mBottomLineColorObj = NULL;
		mLeftLineColor = 0;
		mLeftLineColorObj = NULL;
		mRightLineColor = 0;
		mRightLineColorObj = NULL;
	}
	mTopLineViewType = BaseConstants::NONE;
	mBottomLineViewType = BaseConstants::NONE;
	mLeftLineViewType = BaseConstants::NONE;
	mRightLineViewType = BaseConstants::NONE;
}
DrawRect::DrawRect(ViewObj * viewOwner, DrawRect*otherRect) : BaseDraw(viewOwner,otherRect)
{
	mBackGroundColor = otherRect->mBackGroundColor;
	mRoundLineColor = otherRect->mRoundLineColor;
	mFillRectType = otherRect->mFillRectType;
	mBackGroundColorObj = BaseMath::createColor(mBackGroundColor);
	mRoundLineColorObj = BaseMath::createColor(mRoundLineColor);
	mTopLineViewType = otherRect->mTopLineViewType;
	mBottomLineViewType = otherRect->mBottomLineViewType;
	mLeftLineViewType = otherRect->mLeftLineViewType;
	mRightLineViewType = otherRect->mRightLineViewType;
	mRoundLineThick = otherRect->mRoundLineThick;

}
DrawRect::~DrawRect()
{
	if (mBackGroundColorObj)
	{
		delete mBackGroundColorObj;
		mBackGroundColorObj = NULL;
	}
	if (mRoundLineColorObj)
	{
		delete mRoundLineColorObj;
		mRoundLineColorObj = NULL;
	}
}
void DrawRect::initLineViewType(char leftLineViewType, char rightLineViewType, char topLineViewType, char bottomLineViewType)
{
	mLeftLineViewType = leftLineViewType;
	mRightLineViewType = rightLineViewType;
	mTopLineViewType = topLineViewType;
	mBottomLineViewType = bottomLineViewType;
}

bool DrawRect::drawLines( QPainter &pPainter, int x, int y, int w, int h)
{
	if (mTopLineViewType > BaseConstants::NONE)
	{
		BaseTools::drawLines(pPainter, mRoundLineColorObj, mRoundLineThick, mTopLineViewType, x, y, x + w, y);
		//BaseTools::setLineType(pPainter, mRoundLineColorObj, mRoundLineThick, mTopLineViewType);
		//pPainter.drawLine(x, y, x + w, y);
	}
	if (mBottomLineViewType > BaseConstants::NONE)
	{
		BaseTools::drawLines(pPainter, mRoundLineColorObj, mRoundLineThick, mBottomLineViewType, x, y + h, x + w, y + h);
		//BaseTools::setLineType(pPainter, mRoundLineColorObj, mRoundLineThick, mBottomLineViewType);
		//pPainter.drawLine(x, y + h, x + w, y + h);
	}
	if (mLeftLineViewType > BaseConstants::NONE)
	{
		BaseTools::drawLines(pPainter, mRoundLineColorObj, mRoundLineThick, mLeftLineViewType, x, y, x, y + h);
		//BaseTools::setLineType(pPainter, mRoundLineColorObj, mRoundLineThick, mLeftLineViewType);
		//pPainter.drawLine(x, y, x, y + h);
	}
	if (mRightLineViewType > BaseConstants::NONE)
	{
		BaseTools::drawLines(pPainter, mRoundLineColorObj, mRoundLineThick, mRightLineViewType, x + w, y, x + w, y + h);
		//BaseTools::setLineType(pPainter, mRoundLineColorObj, mRoundLineThick, mRightLineViewType);
		//pPainter.drawLine(x + w, y, x + w, y + h);
	}
	return true;
}
bool DrawRect::draw(QPainter &pPainter, int x, int y, int w, int h)
{
	if (mOwner->isObj(ConstantsID::ContainerId::FLAG))
	{
		int a = 0;
	}
	bool isWholeRoundLine = true;
	if (mTopLineViewType != UIConstants::LineType::SINGLE_LINE || mBottomLineViewType != UIConstants::LineType::SINGLE_LINE
		|| mLeftLineViewType != UIConstants::LineType::SINGLE_LINE || mRightLineViewType != UIConstants::LineType::SINGLE_LINE)
	{
		isWholeRoundLine = false;
	}
	if (mOwner->isObj(ConstantsID::ContainerId::PAGE_TITLE))
	{
		int a = 0;
	}
	BaseViewParam * viewParam = mViewOwner->getViewParam();

	if (mFillRectType == BaseConstants::NONE)
		return false;
	if (w == ConstantsNumber::MIN_CHAR)
	{
		w = viewParam->getTotalW();
	}
	if (h == ConstantsNumber::MIN_CHAR)
	{
		h = viewParam->getTotalH();
	}
	//int w = viewParam->getTotalW();
	//int h = viewParam->getTotalH();
	/*if (mViewOwner->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
	{
		int a = 0;
	}*/
	switch (mFillRectType)
	{
	case UIConstants::FillRectType::ONLY_ROUND_RECT:
		//BaseTools::drawLines(pPainter, mRoundLineColorObj, mRoundLineThick, mTopLineViewType, x, y, w, h);
		//BaseTools::setLineType(pPainter, mRoundLineColorObj, mRoundLineThick, mTopLineViewType);
		drawLines(pPainter, x, y, w, h);
		//pPainter.drawRect(x, y, w, h);
		break;
	case UIConstants::FillRectType::ONLY_FILL:
		pPainter.fillRect(x, y, w, h, *mBackGroundColorObj);
		break;
	case UIConstants::FillRectType::ROUND_LINE_AND_FILL:
		if (isWholeRoundLine)
		{
			pPainter.setBrush(*mBackGroundColorObj);
			BaseTools::setLineType(pPainter, mRoundLineColorObj, mRoundLineThick, mTopLineViewType);
			pPainter.drawRect(x, y, w, h);
		}
		else
		{
			//BaseTools::drawLines(pPainter, mRoundLineColorObj, mRoundLineThick, mTopLineViewType, x, y, w, h);
			drawLines(pPainter, x, y, w, h);
			pPainter.fillRect(x, y, w, h, *mBackGroundColorObj);
		}
		break;
	}
	//if (mViewOwner->isObj(ConstantsID::ContainerId::NORMAL_PAGE))
	//{
	//	QColor c = QColor(0xff0000ff);
	//	pPainter.fillRect(0, 26, 500, 100, c);
	//	QColor d = QColor(0xffff0000);
	//	pPainter.fillRect(500, 25, 500, 100, d);
	//}
	return false;
}
char DrawRect::getFillRectType()
{
	return mFillRectType;
}
void DrawRect::setFillRectType(char fillType)
{
	mFillRectType = fillType;
	if (mFillRectType == UIConstants::FillRectType::ONLY_ROUND_RECT || mFillRectType == UIConstants::FillRectType::ROUND_LINE_AND_FILL)
	{
		initLineViewType(UIConstants::LineType::SINGLE_LINE, UIConstants::LineType::SINGLE_LINE, UIConstants::LineType::SINGLE_LINE, UIConstants::LineType::SINGLE_LINE);
	}
}
QColor * DrawRect::getRoundLineColor()
{
	return mRoundLineColorObj;
}
int DrawRect::getRoundLineThick()
{
	return mRoundLineThick;
}
void DrawRect::setRoundLineThick(int roundLineThick)
{
	mRoundLineThick = roundLineThick;
}
char DrawRect::getTopLineViewType()
{
	return mTopLineViewType;
}
void DrawRect::setTopLineViewType(char lineType)
{
	mTopLineViewType = lineType;
}
char DrawRect::getBottomLineViewType()
{
	return mBottomLineViewType;
}
void DrawRect::setBottomLineViewType(char lineType)
{
	mBottomLineViewType = lineType;
}
char DrawRect::getLeftLineViewType()
{
	return mLeftLineViewType;
}
void DrawRect::setLeftLineViewType(char lineType)
{
	mLeftLineViewType = lineType;
}
char DrawRect::getRightLineViewType()
{
	return mRightLineViewType;
}
void DrawRect::setRightLineViewType(char lineType)
{
	mRightLineViewType = lineType;
}
void DrawRect::setBackGroundColor(int backGroundColord)
{
	if (mBackGroundColorObj)
	{
		delete mBackGroundColorObj;
		mBackGroundColorObj = NULL;
	}
	
}
void DrawRect::setRoundLineColor(int RoundGroundColord)
{
	if (mRoundLineColorObj)
	{
		delete mRoundLineColorObj;
		mRoundLineColorObj = NULL;
	}
}
int DrawRect::getRoundLineIntColor()
{
	return mRoundLineColor;
}
int DrawRect::getBackGroundIntColor()
{
	return mBackGroundColor;
}
void DrawRect::saveSelfData(BaseIOTools *iOTools)
{
	iOTools->writeInt(mBackGroundColor);
	iOTools->writeInt(mRoundLineColor);

	iOTools->writeInt(mTopLineColor);
	iOTools->writeInt(mBottomLineColor);
	iOTools->writeInt(mLeftLineColor);
	iOTools->writeInt(mRightLineColor);

	iOTools->writeByte( mFillRectType);
	//mBackGroundColorObj = BaseMath::createColor(mBackGroundColor);
	//mRoundLineColorObj = BaseMath::createColor(mRoundLineColor);
	iOTools->writeByte(mTopLineViewType);
	iOTools->writeByte(mBottomLineViewType);
	iOTools->writeByte(mLeftLineViewType);
	iOTools->writeByte(mRightLineViewType);
	iOTools->writeInt(mRoundLineThick);
}
void DrawRect::loadSelfData(BaseIOTools *iOTools)
{
	mBackGroundColor = iOTools->readInt();
	mRoundLineColor = iOTools->readInt();

	mTopLineColor = iOTools->readInt();
	mBottomLineColor = iOTools->readInt();
	mLeftLineColor = iOTools->readInt();
	mRightLineColor = iOTools->readInt();

	mFillRectType = iOTools->readByte();
	mBackGroundColorObj = BaseMath::createColor(mBackGroundColor);
	mRoundLineColorObj = BaseMath::createColor(mRoundLineColor);
	mTopLineViewType = iOTools->readByte();
	mBottomLineViewType = iOTools->readByte();
	mLeftLineViewType = iOTools->readByte();
	mRightLineViewType = iOTools->readByte();
	mRoundLineThick = iOTools->readInt();
}
void DrawRect::initExcelCellPropertyMap(QMap<QString, QString> & map, ViewObj* view)
{
	ObjDrawManager * drawManger = view->getDrawMgr();
	DrawRect *  draw = (DrawRect *)drawManger->getBottomDraw();
	//DrawRectParam * rectParam = (DrawRectParam*)draw->getParam();
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

	/*if (fillType == BaseConstants::NONE)
	return;*/
	int w = viewParam->getTotalW();
	//int h = viewParam->getTotalH();
	{
		switch (fillType)
		{
		case UIConstants::FillRectType::ONLY_ROUND_RECT:
			//pPainter.setPen(*roundLineColor);
			//pPainter.drawRect(mRect);
			//pPainter.drawRect(x, y, w, h);

			break;
		case UIConstants::FillRectType::ONLY_FILL:

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
				map.insert("topCheck", "true");
			}
			else
			{
				map.insert("topCheck", "false");
			}

			//bottom
			if (haveBottomLine)
			{
				map.insert("bottomCheck", "true");
			}
			else
			{
				map.insert("bottomCheck", "false");
			}
			//left
			if (haveLeftLine)
			{
				map.insert("leftCheck", "true");
			}
			else
			{
				map.insert("leftCheck", "false");
			}
			//right
			if (haveRightLine)
			{
				map.insert("rightCheck", "true");
			}
			else
			{
				map.insert("rightCheck", "false");
			}
			break;
		}
	}
}
void DrawRect::setExcelCellPropertyByMap(QMap<QString, QString> & map, ViewObj* view)
{
	ObjDrawManager * drawManger = view->getDrawMgr();
	DrawRect *  draw = (DrawRect *)drawManger->getBottomDraw();
	char fillType = draw->getFillRectType();

	// 背景填充色
	//QColor *fillColor = rectParam->getFillColor();
	// 边框色
	//QColor *roundLineColor = rectParam->getRoundLineColor();
	// 边框线厚度
	//short roundLineThick = rectParam->getRoundLineThick();

	QString topCheck = map.value("topCheck");
	QString leftCheck = map.value("leftCheck");
	QString bottomCheck = map.value("bottomCheck");
	QString rightCheck = map.value("rightCheck");

	bool isNeedSetSetFillRectType = false;

	if (topCheck.compare("true") == 0)
	{
		draw->setTopLineViewType(UIConstants::LineType::SINGLE_LINE);
		isNeedSetSetFillRectType = true;
	}
	else
	{
		draw->setTopLineViewType(BaseConstants::NONE);
	}
	if (leftCheck.compare("true") == 0)
	{
		draw->setLeftLineViewType(UIConstants::LineType::SINGLE_LINE);
		isNeedSetSetFillRectType = true;
	}
	else
	{
		draw->setLeftLineViewType(BaseConstants::NONE);
	}
	if (bottomCheck.compare("true") == 0)
	{
		draw->setBottomLineViewType(UIConstants::LineType::SINGLE_LINE);
		isNeedSetSetFillRectType = true;
	}
	else
	{
		draw->setBottomLineViewType(BaseConstants::NONE);
	}
	if (rightCheck.compare("true") == 0)
	{
		draw->setRightLineViewType(UIConstants::LineType::SINGLE_LINE);
		isNeedSetSetFillRectType = true;
	}
	else
	{
		draw->setRightLineViewType(BaseConstants::NONE);
	}
	if (isNeedSetSetFillRectType)
	{
		if (fillType == BaseConstants::NONE)
		{
			draw->setFillRectType(UIConstants::FillRectType::ROUND_LINE_AND_FILL);
		}
	}
}
int DrawRect::initExcelPropertyMap(QMap<QString, QString> & map, ViewObj* view)
{
	ObjDrawManager * drawManger = view->getDrawMgr();
	DrawRect *  draw = (DrawRect *)drawManger->getBottomDraw();
	char fillType = draw->getFillRectType();
	short roundLineThick = draw->getRoundLineThick();
	map.insert("frameValue", QString::number(roundLineThick));
	if (fillType == BaseConstants::NONE)
	{
		map.insert("frame", "false");
	}
	else
	{
		map.insert("frame", "true");
	}
	return roundLineThick;
}
void DrawRect::setExcelPropertyByMap(QMap<QString, QString> & map, ViewObj* view)
{
	ObjDrawManager * drawManger = view->getDrawMgr();
	DrawRect *  draw = (DrawRect *)drawManger->getBottomDraw();
	if (map.contains("frameValue"))
	{
		int roundLineThick = map.value("frameValue").toInt();
		if (roundLineThick > 0 && roundLineThick <= 20)
		{
			draw->setRoundLineThick(roundLineThick);
		}
	}
	QString frameStr = map.value("frame");
	if (map.contains("frame"))
	{
		QString frameStr = map.value("frame");
		if (frameStr == "true")
		{
			draw->setFillRectType(UIConstants::FillRectType::ROUND_LINE_AND_FILL);
		}
		else
		{
			draw->setFillRectType(BaseConstants::NONE);
		}
	}

	//BaseWinData * winData = view->getWinData();
	//ViewObj* txtLayer = (ViewObj*)view->getChildren()->at(ConstantsID::IndexId::LAYER_TXT);
	//winData->addToReplaceViewList(txtLayer);
	//winData->replaceNeedReplaceList();
}