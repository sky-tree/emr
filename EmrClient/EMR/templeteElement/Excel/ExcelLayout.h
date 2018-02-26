#ifndef _EXCELLAYOUT_H_
#define _EXCELLAYOUT_H_
#include "Line.h"
#include "ExcelCellData.h"
#include "ViewObj.h"
#include "MeasureWH.h"
#include "ExcelBaseLayout.h"
#include "BaseLayout.h"
#include "ExcelLayoutViewParam.h"
class ExcelTemplete;
class ExcelCellDataRow;
class ExcelLayout :
	public ExcelBaseLayout, public BaseLayout
{
public:
	static const int defaultCellH = 27;
	static const int defaultCellW = 20;
	//const int GenerateCellW = defaultCellW;
	static const int defaultInside = 0;
public:
	ExcelLayout(ViewObj * parent,bool isCreateFromLoad);
	~ExcelLayout();
	void setSelectLineVisability(bool visable,int x,int y,int w,int h);
	virtual int judgeCanHaveNum(BaseContainer * child, int idx, bool isAddOrChildWHChange);
	int conditionAddChild(BaseContainer * child, int idx);
	//int conditionAddChildren(QList<ViewObj *> * children, int idx);

	void generateCells(int row, int col, ViewObj * containerView);
	static int getCellWidth(int row, int col, ViewObj * containerView);
	static bool canGenerateExcel(int row, int col, ViewObj * containerView);
	ExcelCellData * createCellData(Line * left, Line * top, Line * right, Line * bottom);
	void addOrRomoveCellData(ExcelCellData* cellData,bool isAdd);
	void moveSomeLines(QList<Line *> * mLines, int start, int end, int distance);
	static void sortLines(QList<Line *> * mLines);
	void insertLine(Line* line, QList<Line *> * mLines);
	Line * findNearestLine(QList<Line *> * lineList,int position,int amountOfError);
	void removeUselessLine(bool isCreateAction = true);
	void removeUseLessRowLine(bool isCreateAction = true);
	void removeUseLessColLine(bool isCreateAction = true);
	Line * getLineNextOrLastLine(Line * currentLine,bool isRowLine,bool isLast );
	Line * getLineNextOrLastValidLine(QList<ExcelCellData *>* cellDatas, bool isRowLine, bool isLast);
	int getIndexOfLine(Line * currentLine, bool isRowLine);
	virtual void onMeasure(MeasureWH * measureWH, bool isFreshFromInnerChange);
	virtual void onPrepareLayout(bool isChanged, int l, int t, int r, int b);
	virtual void onLayout(bool isChanged, int l, int t, int r, int b){ }
	virtual void composing(bool isFreshFromInnerChange);
	void seekCellsHaveLine(char placeType, Line* line, QList<ExcelCellData *>* mCellsSrc, QList<ExcelCellData *>* mCellsResult);
	bool changeCellsPositionLine(QList<ExcelCellData *>* mCellsSrc, Line* line,char placeType);
	ViewObj * seekCell(int x,int y);
	void clearCellsTempT1();
	void clearCellsTempT2();
	static ExcelCellData * getCellDataFromCell(BaseElement * cell);
	static void  getCellBaseInformation(int & defaultH, int & position, BaseElement * cell);
	static int getWidthFormCellData(ExcelCellData* cellData);
	static int getWidthFormCellData(BaseElement * cell);
	void getCellDatasFormCellObjs(QList<ViewObj *> * cellsObjList, QList<ExcelCellData *>* resultCellDataList);
	bool mergeTwoCellData(ExcelCellData * cellData1, ExcelCellData * cellData2,ExcelCellData * resultCellData);
	void mergCellDatas(QList<ExcelCellData *>* cellDatas, ExcelCellData * resultCellData);
	bool canMerge(QList<ExcelCellData *>* cellDataList, ExcelCellData * resultCellData);
	bool mergeCell(QList<ViewObj *> * cellsObjList);
	ViewObj * findLeftAndTopCellObj(QList<ViewObj *> * cellsObjList, ExcelCellData* mergedCellData);
	void deleteCellObj(ViewObj * cell);
	void deleteCellData(ExcelCellData * cellData);
	bool dividCell(ViewObj * cellObj, bool isHorizontal, int number);
	void dividCell(ExcelCellData * cellData, int row, int col);
	bool dividCellOnHorizontal(ViewObj * cellObj, int number);
	bool dividCellOnHorizontal(ExcelCellData * cellData, int number);
	bool setCellWidth(ExcelCellData * cellData,int setWidth);
	Line * getNearestColOrRowLine(bool isRowLine, int postion, int amountOfError=5);
	void dividCellOnVertical(ViewObj * cellObj, int number);
	void dividCellOnVertical(ExcelCellData * cellData, int number);
	ExcelCellDataRow* getCellDataList(int rowIndex);
	int getCellRowSize();
	void removeNotShowCell(QList<BaseContainer *> * showList);
	bool isCellSOnThroughRow(QList<ExcelCellData *>* cells);
	ExcelTemplete* getExcelTemplete();
	void showTidy();
	void addRowOrCol(ExcelCellData * cellData, int direction);
	void createActionToRemeberOperate();
	void checkNarrowEachLineDistance(int totalDistance, QList<Line *> * mLines, QList<int>& moveData,QList<int> &fixedLineIndexList);
	void narrowEachLineDistance(int totalDistance, QList<Line *> * mLines,  QList<int>& moveData);
	void generateCellData(Line* first, Line* second, QList<Line *> * mLines,ViewObj* focusPageCell);
	bool isRectCrossSomeCellData(int left1, int top1, int right1, int bottom1);
	void deleteRowOrCol(ExcelCellData * cellData, bool isRow);
	void deleteRowOrColCellData(Line* first, Line* second, bool isRow);
	bool setCellProperty(ExcelCellData * cellData, QMap<QString, QString>& map, int currentWidth, int setWidth, OperateMgr *operateMgr, bool isUndo = true);
	void setExcelProperty(ExcelCellData * cellData, QMap<QString, QString>& map, bool isShowBorder, int borderThick, int insideWidth, QString preAlign, bool setIsShowBorder, int setBorderThick, int setInsideWidth, QString setAlign);
	static void setExcelInsideProperty(ViewObj* cellPage,int insideWidth);
	void initByOther(WriterManager * wMgr, ExcelLayout* otherExcelLayout);
	void showSetCellProperty(ExcelCellData * cellData);
	void showSetExcelProperty(ExcelCellData * cellData);
	bool isMyCell(ViewObj * cell);
	int getRowCellMinHieght(Line * line);
	bool isAbleToSetX(char selectLineType, int setX, QList<ExcelCellData *>* cellDataList); //判断是否可以设置当前的坐标
	int getXCanSet(char selectLineType, QList<ExcelCellData *>* cellDataList);
	int getExcelWidth();
	void setCellDataFocus(ExcelCellData * cellData, OperateMgr *operateMgr, ViewObj*preFocus, char preDir, char setAfterDir);
	//撤销回退
	void clearAllCellTxtLayerForWinData(WriterManager * wMgr);
	//新功能，做完子对象管理器
	 bool addChild(BaseContainer * child, int idx, bool refreshIdx);
	 void addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx);
	 int delChild(int idx, bool refreshIdx);
	 int delChild(BaseContainer * child, bool refreshIdx);
	 int delChildren(int start, int num, bool refreshIdx);

	 int getWholeRowCrossPageH(int curH);
	 void setWriteManager(WriterManager* writeManger, bool isCircle = true);
	QList<Line *> * mRowLines;
	QList<Line *> * mColLines;
	QList<ExcelCellData *>* mCells;
	QList<ExcelCellDataRow *> *mCellRowList;
	QList<ExcelCellData *>* mCellsTempt1;
	QList<ExcelCellData *>* mCellsTempt2;
	QList<ExcelCellData *>* mCellsTemptUseForMerge;//保存合并时生成的临时变量
	QList<ExcelCellData *>* mCellsTempt4;
	QList<ExcelCellData *>* mCellsTempt5;
	QList<Line *> * mTempLineList;
	ExcelCellData* mTemptCellData;
	ExcelCellData* mTemptCellDataMergeReult;
	ExcelLayoutViewParam * mViewParam;
	ExcelTemplete* mExcelTemplete;
	/// 拖动显示虚线
	ViewObj * mDragLine;
	bool mIsOnMeasure;
};
#endif
