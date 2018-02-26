#ifndef _EXCELCELLTEMPLET_H_
#define _EXCELCELLTEMPLET_H_
#include "ExcelBaseLayout.h"
//#include "ViewElement.h"
#include "CodeTempData.h"
#include "Line.h"
//#include "BaseTemplate.h"
class DrawRect;
class ExcelTemplete;
class BaseTemplate;
class VerticalViewParam;

enum ComposingType
{
	EXACTLY = 0,
	AT_MOST 
};
enum CellStatusType
{
	OnNomal = 0,
	OnMeause,
	OnLayout
};
class ExcelCellData :
	public CodeTempData, public ExcelBaseLayout
{
public:
	ExcelCellData(ViewObj * parent, BaseTemplate* templeteOwner, CodeParams * codeParams);
	~ExcelCellData();
	void initData();
	void initLines(Line * left, Line * top, Line * right, Line * bottom);
	void initFromOther(ExcelCellData* otherCellData);
	virtual void onMeasure(MeasureWH * measureWH, bool isFreshFromInnerChange);
	virtual void onLayout(bool isChanged, int l, int t, int r, int b);
	void replaceExcelPage(ViewObj * txtLayer, VerticalViewParam * layerViewParam, int disInWidth , bool isFreshFromInnerChange);
	void setCellTempleteOwner(BaseTemplate * templeteOwner);
public:
	void saveSelfData(BaseIOTools *iOTools);
	void loadSelfData(BaseIOTools *iOTools);
	virtual BaseContainer * createOwner(BaseContainer * tempParent1, BaseIOTools *iOTools,bool isWillDelete);
	void createCellPageByOther(WriterManager * wMgr, ExcelCellData *otherCellData, ExcelTemplete* excelTemplete);
	//BaseTemplete * mTempleteOwner;
	int getMaxWidthOfChildView();
	ViewObj * getViewOwner();
	void setViewOwner(ViewObj * viewOwner);
	void setTemplateOwner(BaseTemplate *templateOwner);
	bool getCanPrint();
	bool getCanEdit();
	int getCellLineHeight();
	ParagraphManager * getParagraphMgr();
	void setVerticalPosition(int position);
	int getVerticalPosition();
	int getIntegrityRowByMaxH(int bottomH);
	Line *mLeft;
	Line *mTop;
	Line *mRight;
	Line *mBottom;
	// 单元格段落根对象
	BaseTemplate * mRootTemplete;
	//是否可以打印
	bool mCanPrint;
	//是否可操作
	bool mCanEdit;
	//CodeTempData * mRootContainerData;
	//DrawRect * mRectParam;
	//ViewObj* mWindObj;
	ViewObj * mViewOwner; //单元格
	int mInsideWidth;
	int mVerticalPosition; //垂直方向上位置
	CellStatusType mCellStatus;
};
#endif

