#ifndef DRAW_RECT_H_
#define DRAW_RECT_H_


#include <QColor>

#include "BaseDraw.h"
#include "BaseIOTools.h"
class ViewObj;


#include <QFont>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")      
#endif
class DrawRect : public BaseDraw
{
public:
	DrawRect(ViewObj * viewOwner, char type, int backColor, int lineColor, short thick,bool isCreateOnLoad = false);
	DrawRect(ViewObj * viewOwner, DrawRect*otherRect);
	~DrawRect();

public:
	void initLineViewType(char leftLineViewType, char rightLineViewType, char topLineViewType, char bottomLineViewType);

	bool drawLines(QPainter &pPainter, int x, int y, int w, int h);
	bool draw(QPainter &pPainter, int x, int y, int w, int h);
	char getFillRectType();
	void setFillRectType(char fillType);
	QColor * getRoundLineColor();
	int getRoundLineThick();
	void setRoundLineThick(int roundLineThick);
	char getTopLineViewType();
	void setTopLineViewType(char lineType);
	char getBottomLineViewType();
	void setBottomLineViewType(char lineType);
	char getLeftLineViewType();
	void setLeftLineViewType(char lineType);
	char getRightLineViewType();
	void setRightLineViewType(char lineType);
	void setBackGroundColor(int backGroundColord);
	void setRoundLineColor(int RoundGroundColord);
	int getRoundLineIntColor();
	int getBackGroundIntColor();
	void saveSelfData(BaseIOTools *iOTools);
	void loadSelfData(BaseIOTools *iOTools);

	static void initExcelCellPropertyMap(QMap<QString, QString> & map, ViewObj* view);
	static void setExcelCellPropertyByMap(QMap<QString, QString> & map, ViewObj* view);
	static int initExcelPropertyMap(QMap<QString, QString> & map, ViewObj* view);
	static void setExcelPropertyByMap(QMap<QString, QString> & map, ViewObj* view);
private:

	// 填充类型
	char mFillRectType;

	// 填充色
	int mBackGroundColor;
	QColor * mBackGroundColorObj;

	// 边线颜色
	int mRoundLineColor;
	QColor * mRoundLineColorObj;
	int mTopLineColor;
	QColor * mTopLineColorObj;
	int mBottomLineColor;
	QColor * mBottomLineColorObj;
	int mLeftLineColor;
	QColor * mLeftLineColorObj;
	int mRightLineColor;
	QColor * mRightLineColorObj;

	// 边线显示类型
	char mTopLineViewType;
	char mBottomLineViewType;
	char mLeftLineViewType;
	char mRightLineViewType;

	// 边线厚度
	short mRoundLineThick;

};

#endif // _DRAW_RECT_H_
