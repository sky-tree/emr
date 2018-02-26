#ifndef _DRAW_FACTORY_H_
#define _DRAW_FACTORY_H_


class ViewObj;
class BaseDraw;
class DrawRectParam;
class DrawRect;
class DrawImageParam;
class DrawImage;
class DrawTxtParam;
class FocusTxtData;
class DrawStr;
class DrawLine;
class DrawLineParam;
class DrawRow;

class DrawFreeLineParam;
class DrawFreeLine;
class WriterManager;
class DrawBaseImgButtonParam;
class DrawBaseImgButton;

class NormalRowParam;

#include <QColor>
#include <QFont>

class DrawFactory
{
public:
	DrawFactory();
	~DrawFactory();

public:

	// 创建图片参数
	static DrawImageParam * createDrawImageParam(ViewObj * viewOwner, NormalRowParam * rowParam, QString imgName);
	static DrawImageParam * createDrawImageParam(ViewObj * viewOwner, QString imgName);
	static DrawImage * createImage(ViewObj * viewOwner, DrawImageParam * imgParam);

	// 创建矩形绘制参数
	static DrawRectParam * createDrawRectParam(ViewObj * viewOwner, char fillRoundType, int roundLineColor, int fillColor, short roundRectThick);
	// 创建矩形
	static DrawRect * createRect(ViewObj * viewOwner, DrawRectParam * rectParam);
	static DrawRow * createDrawRow(ViewObj * viewOwner, DrawRectParam * rectParam);

	// 创建文字参数
	static DrawTxtParam * createDrawTxtParam(ViewObj * viewOwner, QString str, DrawTxtParam * focusData);
	static DrawTxtParam * createDrawTxtParam(WriterManager * writ, ViewObj * viewOwner, QString str, DrawTxtParam * focusData);
	// 创建文字绘制
	static DrawStr * createDrawTxt(ViewObj * viewOwner, DrawTxtParam * contentData);
	//创建线
	static DrawLine * createDrawExcelLine(ViewObj * viewOwner, QColor *lineColor,int lineWidth,Qt::PenStyle penStyle);
	// 创建线参数
	static DrawLineParam * createDrawExcelLineParam(ViewObj * viewOwner, QColor *lineColor, int lineWidth, Qt::PenStyle penStyle);

	static BaseDraw * createDraw(ViewObj * viewOwner);

	// 创建基本按钮绘制参数
	static DrawBaseImgButtonParam * createDrawBaseImgButtonParam(ViewObj * viewOwner, QList<QString> * imgNameList);
	static DrawBaseImgButton * createDrawBaseImgButton(ViewObj * viewOwner, DrawBaseImgButtonParam * drawParam);
	
private:
	// 创建图像编辑画线参数
	static DrawFreeLineParam * createFreeLineDrawParam(ViewObj * viewOwner, int thick, int color);

	
};

#endif // _DRAW_FACTORY_H_
