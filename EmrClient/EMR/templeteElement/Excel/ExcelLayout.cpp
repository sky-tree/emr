#include "ExcelLayout.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "qdebug.h"
#include "BaseViewParam.h"
#include "Line.h"
#include "ViewFactory.h"
#include "BaseMath.h"
#include "exception"
#include "ExcelCellDataRow.h"
#include "ExcelTemplete.h"
#include "ViewObj.h"
#include "views/propertySettingDlgs/cellpropertydlg.h"
#include "views/propertySettingDlgs/TablePropertyDlg.h"
#include"qmap.h"
#include "TempleteTypeEnumeration.h"
#include "DrawRect.h"
//#include "ParagraphTemplete.h"
#include "ParagraphData.h"
//#include "AbsLayoutViewParam.h"
//#include "DrawManager.h"
//#include "DrawRectParam.h"
#include "SetExcelPropertyAction.h"
#include "SetExcelCellPropertyAction.h"
//#include "BaseWinData.h"
#include "LineMoveAction.h"
#include "ExcelCellChangeLineAction.h"
#include "NewOrDeleteLineAction.h"
#include "NewOrDeleteCellDataAction.h"
#include "ExcelRelayoutAction.h"
#include "SetFocusAction.h"
#include "VerticalViewParam.h"
#include "OperateMgr.h"
#include "FlagParam.h"
#include "MessageBox.h"
#include "WriterManager.h"
#include "BaseParagraph.h"
#include "ObjDrawManager.h"
#include "TempParagraphMgrData.h"
#include "ParagraphManager.h"
#include "RowViewParam.h"
ExcelLayout::ExcelLayout(ViewObj * viewOwner, bool isCreateFromLoad) :ExcelBaseLayout(), BaseLayout(viewOwner,ConstantsID::ClassId::EXCEL_LAYOUT)
, mRowLines(NULL)
, mColLines(NULL)
, mCells(NULL)
, mCellsTempt1(NULL)
, mCellsTempt2(NULL)
, mCellsTemptUseForMerge(NULL)
, mCellsTempt4(NULL)
, mDragLine(NULL)
, mViewParam((ExcelLayoutViewParam*)viewOwner->getViewParam())
, mExcelTemplete((ExcelTemplete*)viewOwner->getTempParent())
, mTemptCellData(NULL)
, mTemptCellDataMergeReult(NULL)
, mTempLineList(NULL)
, mCellRowList(NULL)
{
	mIsOnMeasure = false;
	//mComposingHeightType = BaseConstants::ComposingType::AT_MOST;
	//mComposingWidthType = BaseConstants::ComposingType::EXACTLY;
	if (!isCreateFromLoad)
	{
		mRowLines = new QList<Line *>();
		mColLines = new QList<Line *>();
		mCells = new QList<ExcelCellData *>();
	}

	mTempLineList = new QList<Line *>();

	mCellsTempt1 = new QList<ExcelCellData *>();
	mCellsTempt2 = new QList<ExcelCellData *>();
	mCellsTemptUseForMerge = new QList<ExcelCellData *>();
	mCellsTempt4 = new QList<ExcelCellData *>();
	mCellsTempt5 = new QList<ExcelCellData *>();
	mTemptCellData = new ExcelCellData(NULL, NULL, NULL);
	mTemptCellDataMergeReult = new ExcelCellData(NULL, NULL, NULL);
	mCellRowList = new QList<ExcelCellDataRow*>();
}
void ExcelLayout::initByOther(WriterManager * wMgr, ExcelLayout* otherExcelLayout)
{
	mRowLines = new QList<Line *>();
	int rowSize = otherExcelLayout->mRowLines->size();
	Line* tempLine;
	for (int i = 0; i < rowSize; i++)
	{
		tempLine = new Line(otherExcelLayout->mRowLines->at(i));
		mRowLines->push_back(tempLine);
	}
	//竖线
	mColLines = new QList<Line *>();
	int colSize = otherExcelLayout->mColLines->size();
	//Line* tempLine;
	for (int i = 0; i < colSize; i++)
	{
		tempLine = new Line(otherExcelLayout->mColLines->at(i));
		mColLines->push_back(tempLine);
	}
	mCells = new QList<ExcelCellData *>();
	int cellSize = otherExcelLayout->mCells->size();
	ExcelCellData* cellData;
	int leftLineIndex;
	int rightLineIndex;
	int topLineIndex;
	int bottomLineIndex;
	ExcelCellData* otherCellData;
	for (int i = 0; i < cellSize; i++)
	{
		otherCellData = otherExcelLayout->mCells->at(i);
		leftLineIndex = otherExcelLayout->mColLines->indexOf(otherCellData->mLeft);
		rightLineIndex = otherExcelLayout->mColLines->indexOf(otherCellData->mRight);
		topLineIndex = otherExcelLayout->mRowLines->indexOf(otherCellData->mTop);
		bottomLineIndex = otherExcelLayout->mRowLines->indexOf(otherCellData->mBottom);
		cellData = new ExcelCellData(NULL, NULL, NULL);
		cellData->initLines(mColLines->at(leftLineIndex), mRowLines->at(topLineIndex), mColLines->at(rightLineIndex), mRowLines->at(bottomLineIndex));
		mCells->push_back(cellData);
		cellData->createCellPageByOther(wMgr, otherCellData, mExcelTemplete);
	}
}
ExcelLayout::~ExcelLayout()
{
	if (mExcelTemplete){
		mExcelTemplete = NULL;
	}
	if (mRowLines){
		int size = mRowLines->size();
		Line* line = NULL;
		for (int i = size - 1; i >= 0; i--){
			line = mRowLines->at(i);
			delete line;
			line = NULL;
		}
		delete mRowLines;
		mRowLines = NULL;

	}
	if (mColLines){
		int size = mColLines->size();
		Line* line = NULL;
		for (int i = size - 1; i >= 0; i--){
			line = mColLines->at(i);
			delete line;
			line = NULL;
		}
		delete mColLines;
		mColLines = NULL;

	}
	if (mTempLineList){
		delete mTempLineList;
		mTempLineList = NULL;
	}
	if (mCellsTempt1){
		mCellsTempt1->clear();
		delete mCellsTempt1;
		mCellsTempt1 = NULL;
	}
	if (mCellsTempt2){
		mCellsTempt2->clear();
		delete mCellsTempt2;
		mCellsTempt2 = NULL;
	}
	if (mCellsTempt4){
		mCellsTempt4->clear();
		delete mCellsTempt4;
		mCellsTempt4 = NULL;
	}
	if (mCellsTempt5){
		mCellsTempt5->clear();
		delete mCellsTempt5;
		mCellsTempt5 = NULL;
	}
	if (mCellsTemptUseForMerge){
		int size = mCellsTemptUseForMerge->size();
		ExcelCellData* cell = NULL;
		for (int i = 0; i < size; i++){
			cell = mCellsTemptUseForMerge->at(i);
			delete cell;
			cell = NULL;
		}
		mCellsTemptUseForMerge->clear();
		delete mCellsTemptUseForMerge;
		mCellsTemptUseForMerge = NULL;
	}
	if (mCells){
		int size = mCells->size();
		ExcelCellData* cell = NULL;
		for (int i = size - 1; i >= 0; i--){
			cell = mCells->at(i);
			cell->getViewOwner()->initContentData(NULL);
			delete cell;
			cell = NULL;
		}
		delete mCells;
		mCells = NULL;
	}
	if (mDragLine){
		delete mDragLine;
		mDragLine = NULL;
	}
	if (mTemptCellData){
		delete mTemptCellData;
		mTemptCellData = NULL;
	}
	if (mTemptCellDataMergeReult){
		delete mTemptCellDataMergeReult;
		mTemptCellDataMergeReult = NULL;
	}
	if (mCellRowList){
		int size = mCellRowList->size();
		ExcelCellDataRow* temptCellDataList;
		for (int i = size - 1; i >= 0; i--){
			temptCellDataList = mCellRowList->at(i);
			delete temptCellDataList;
			temptCellDataList = NULL;
		}
		delete mCellRowList;
		mCellRowList = NULL;
	}
	if (mViewParam)
	{
		mViewParam = NULL;
	}
}
void ExcelLayout::setSelectLineVisability(bool visable, int x, int y, int w, int h){
	/*QList<BaseContainer *> * children = mViewOwner->getChildren();
	if (visable){
	int preIndex = children->indexOf(mDragLine);
	if (preIndex >= 0 ){
	int lastIndex = children->size() - 1;
	if (preIndex != lastIndex){
	children->swap(preIndex, lastIndex);
	}
	}
	else{
	children->push_back(mDragLine);
	}
	BaseViewParam * dragLineViewParam = mDragLine->getViewParam();
	dragLineViewParam->setX(x);
	dragLineViewParam->setY(y);
	dragLineViewParam->setSelfW(w);
	dragLineViewParam->setSelfH(h);
	}
	else{
	int preIndex = children->indexOf(mDragLine);
	if (preIndex >= 0){
	children->removeAt(preIndex);
	}
	}*/
	if (visable)
	{
		BaseViewParam * dragLineViewParam = mDragLine->getViewParam();
		dragLineViewParam->setX(x);
		dragLineViewParam->setY(y);
		dragLineViewParam->setSelfW(w);
		dragLineViewParam->setSelfH(h);
		mExcelTemplete->addExcelDragLineToContainer(mDragLine);

	}
	else
	{
		mExcelTemplete->removeExcelDragLineFromContainer(mDragLine);
	}
}
void ExcelLayout::generateCells(int row, int col, ViewObj * containerView){
	using namespace std;
	if (row <= 0 || col <= 0){
		return;
	}
	int cellW = 0;
	int cellH = defaultCellH;
	if (containerView){
		//BaseViewParam * viewp = containerView->getViewParam();
		
		cellW = getCellWidth(row, col, containerView);
	}
	if (cellW < defaultCellW){
		cellW = defaultCellW;
	}
	Line* line = NULL;
	//第一条线
	//生成横线
	int position;
	for (int i = 0; i <= row; i++){
		position = i*cellH + mViewParam->getInsideTop();
		line = new Line(position);
		line->setDragPosition(position);
		mRowLines->push_back(line);
	}
	//生成竖线
	//第一条线
	for (int i = 0; i <= col; i++){
		//position = i*cellW + mViewParam->getInsideLeft();
		position = i*cellW + defaultInside;
		line = new Line(position);
		line->setDragPosition(position);
		mColLines->push_back(line);
	}
	sortLines(mRowLines);
	sortLines(mColLines);
	//生成单元格
	int currentW = 0;
	//int currentH = 0;
	ExcelCellData * cell = NULL;
	//ViewObj * cellView = NULL;
	for (int i = 0; i < row; i++){
		for (int m = 0; m < col; m++){
			cell = createCellData(mColLines->at(m), mRowLines->at(i), mColLines->at(m + 1), mRowLines->at(i + 1));
			mCells->push_back(cell);
			ViewFactory::createExcelCell(containerView, cell);
		}

	}
	//composing();
}
int ExcelLayout::getCellWidth(int row, int col, ViewObj * containerView)
{
	BaseViewParam * viewParam = containerView->getViewParam();
	float rowW = viewParam->getSelfW();
	//float rowL = mViewParam->getInsideLeft();
	//float rowR = mViewParam->getInsideRight();
	float dest = (rowW - defaultInside * 2) / col;
	return dest;
}
bool ExcelLayout::canGenerateExcel(int row, int col, ViewObj * containerView)
{
	if (row <= 0 || col <= 0)
	{
		return false;
	}
	int celWidth = getCellWidth(row,col,containerView);
	BaseViewParam * viewParam = containerView->getParentViewById(ConstantsID::ContainerId::NORMAL_PAGE)->getViewParam();
	int containerH = viewParam->getSelfH();
	int cellH = containerH / row;
	//if (celWidth<defaultCellW || cellH<defaultCellH)
	if (celWidth<defaultCellW)
	{
		return false;
	}
	else
	{
		return true;
	}
}
ExcelCellData * ExcelLayout::createCellData(Line * left, Line * top, Line * right, Line * bottom){
	ExcelCellData * cell = new ExcelCellData(NULL, NULL, NULL);
	cell->mLeft = left;
	cell->mRight = right;
	cell->mTop = top;
	cell->mBottom = bottom;
	return cell;
}
void ExcelLayout::addOrRomoveCellData(ExcelCellData* cellData, bool isAdd)
{
	if (!cellData)
	{
		return;
	}
	
	if (isAdd)
	{
		if (mCells->contains(cellData))
		{
			return;
		}
		OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
	//	operateMgr->createNewAction();
		NewOrDeleteCellDataAction* action = new NewOrDeleteCellDataAction(operateMgr->getCurStepMgr(), mExcelTemplete);
		action->addCellData(cellData,true);
		mCells->push_back(cellData);
		operateMgr->addToLocalAction(action);
	}
	else
	{
		if (!mCells->contains(cellData))
		{
			return;
		}
		OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
		NewOrDeleteCellDataAction* action = new NewOrDeleteCellDataAction(operateMgr->getCurStepMgr(), mExcelTemplete);
		action->addCellData(cellData, false);
		mCells->removeAll(cellData);
		operateMgr->addToLocalAction(action);
	}

}
void ExcelLayout::moveSomeLines(QList<Line *> * mLines, int start, int end, int distance){
	using namespace std;
	if (!mLines){
		return;
	}
	if (distance==0)
	{
		return;
	}
	bool needCreateLine = true;
	int size = mLines->size();

	if (end<0 || end>size){
		return;
	}
	LineMoveAction *moveLineAction = NULL;
	//撤销 前进
	if (needCreateLine)
	{
		OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
		moveLineAction = new LineMoveAction(operateMgr->getCurStepMgr(), mExcelTemplete);
		operateMgr->addToLocalAction(moveLineAction);
	}
	Line * line;
	int prePosition;
	int preDragPosition;
	//判断移动后，当前的线不能小于上一条线的值
	//if (distance<0)
	{
		int afterMove = mLines->at(start)->getPosition() + distance;
		int lastIndex = start - 1;
		if (lastIndex >= 0){
			if (afterMove <= mLines->at(lastIndex)->getPosition()){
			}
		}
		line = mLines->at(start);
		//prePosition = line->getDragPosition();
		prePosition = line->getPosition();
		preDragPosition = line->getDragPosition();
		line->addAllPosition(distance);
		if (needCreateLine)
		{
			moveLineAction->addMoveLine(line, prePosition, preDragPosition,line->getPosition(), line->getDragPosition());
		}
		
	}
	for (int i = start + 1; i < end; i++){
		line = mLines->at(i);
		prePosition = line->getPosition();
		preDragPosition = line->getDragPosition();
		line->addAllPosition(distance);
		if (needCreateLine)
		{
			moveLineAction->addMoveLine(line, prePosition, preDragPosition, line->getPosition(), line->getDragPosition());
		}
	}
}
/*升序排序*/
void ExcelLayout::sortLines(QList<Line *> * mLines){
	int size = mLines->size();
	if (size < 2){
		return;
	}
	Line* lineCurrent = NULL;
	Line* lineCompare = NULL;
	for (int i = 1; i < size; i++){
		lineCurrent = mLines->at(i);
		for (int m = i - 1; m >= 0; m--){
			lineCompare = mLines->at(m);
			if (lineCurrent->getPosition() < lineCompare->getPosition()){
				mLines->swap(m, m + 1);
			}
			else{
				break;
			}
		}
	}
}
/*插入线，保持升序排序*/
void ExcelLayout::insertLine(Line* line, QList<Line *> * lineList){
	int preIndex = lineList->indexOf(line);
	if (preIndex >= 0){
		return;
	}
	OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
	NewOrDeleteLineAction * tempAction = new NewOrDeleteLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
	operateMgr->addToLocalAction(tempAction);
	if (lineList == mRowLines){
		tempAction->addLine(line, true, true);
	}
	else
	{
		tempAction->addLine(line, true, false);
	}
	lineList->push_back(line);
	int laseIndex = lineList->size() - 1;
	Line* lineCurrent = lineList->at(laseIndex);
	Line* lineCompare = NULL;
	for (int m = laseIndex - 1; m >= 0; m--){
		lineCompare = lineList->at(m);
		if (lineCurrent->getPosition() < lineCompare->getPosition()){
			lineList->swap(m, m + 1);
		}
		else{
			break;
		}
	}
}
/*
lineList 要寻找的线的容器
position 寻找的位置
amountOfError 误差范围，小于0 时，表示任意的误差范围
*/
Line * ExcelLayout::findNearestLine(QList<Line *> * lineList, int position, int amountOfError){
	Line * nearestLine = lineList->at(0);
	int size = lineList->size();
	Line * temptLine;
	for (int i = 1; i < size; i++){
		temptLine = lineList->at(i);
		if (BaseMath::getAbs(position - temptLine->getPosition()) < BaseMath::getAbs(position - nearestLine->getPosition())){
			nearestLine = temptLine;
		}
	}
	if (amountOfError >= 0)
	{
		if (BaseMath::getAbs(position - nearestLine->getPosition()) <= amountOfError)
		{
			return nearestLine;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return nearestLine;
	}
}
void ExcelLayout::removeUselessLine(bool isCreateAction ){
	removeUseLessRowLine(isCreateAction);
	removeUseLessColLine(isCreateAction);
}
void ExcelLayout::removeUseLessRowLine(bool isCreateAction){
	int cellsSize = mCells->size();
	ExcelCellData * cell = NULL;
	Line * temptLine = NULL;
	bool isUse = false;
	NewOrDeleteLineAction * tempAction = NULL;
	OperateMgr * operateMgr = NULL;
	if (isCreateAction)
	{
		 operateMgr = mExcelTemplete->getOperateMgr();
		tempAction = new NewOrDeleteLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
	}
	
	bool isDeleteSomeLine = false;
	for (int lineIndex = 0; lineIndex < mRowLines->size();){
		temptLine = mRowLines->at(lineIndex);
		isUse = false;
		for (int i = 0; i < cellsSize; i++){
			cell = mCells->at(i);
			if (cell->mTop == temptLine || cell->mBottom == temptLine){
				isUse = true;
				break;
			}
		}
		if (isUse){
			lineIndex++;
		}
		else{
			temptLine = mRowLines->at(lineIndex);
			mRowLines->removeAt(lineIndex);
			if (tempAction)
			{
				tempAction->addLine(temptLine, false, true);
			}
			isDeleteSomeLine = true;
		}
	}
	if (isDeleteSomeLine)
	{
		if (operateMgr)
		{
			operateMgr->addToLocalAction(tempAction);
		}

	}
	else
	{
		if (tempAction)
		{
			delete tempAction;
			tempAction = NULL;
		}
	}
}
void ExcelLayout::removeUseLessColLine(bool isCreateAction){
	int cellsSize = mCells->size();
	ExcelCellData * cell = NULL;
	Line * temptLine = NULL;
	bool isUse = false;
	bool isDeleteSomeLine = false;
	NewOrDeleteLineAction * tempAction = NULL;
	OperateMgr * operateMgr = NULL;
	if (isCreateAction)
	{
		operateMgr = mExcelTemplete->getOperateMgr();
		tempAction = new NewOrDeleteLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
	}
	//OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
	//NewOrDeleteLineAction * tempAction = new NewOrDeleteLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
	for (int lineIndex = 0; lineIndex < mColLines->size();){
		temptLine = mColLines->at(lineIndex);
		isUse = false;
		for (int i = 0; i < cellsSize; i++){
			cell = mCells->at(i);
			if (cell->mLeft == temptLine || cell->mRight == temptLine){
				isUse = true;
				break;
			}
		}
		if (isUse)
		{
			lineIndex++;
		}
		else
		{
			temptLine = mColLines->at(lineIndex);
			mColLines->removeAt(lineIndex);
			if (tempAction)
			{
				tempAction->addLine(temptLine, false, false);
			}
			isDeleteSomeLine = true;
		}
	}
	if (isDeleteSomeLine)
	{
		if (operateMgr)
		{
			operateMgr->addToLocalAction(tempAction);
		}
	}
	else
	{
		if (tempAction)
		{
			delete tempAction;
			tempAction = NULL;
		}

	}
}
Line * ExcelLayout::getLineNextOrLastLine(Line * currentLine, bool isRowLine, bool isLast){
	Line * returnLine = NULL;
	//横线
	if (isRowLine){
		int index = mRowLines->indexOf(currentLine);
		if (index >= 0){
			if (isLast){
				//判断是不是第一个
				if (index > 0){
					returnLine = mRowLines->at(index - 1);
				}
			}
			else{
				//判断是不是最后一个
				if (index < mRowLines->size() - 1){
					returnLine = mRowLines->at(index + 1);
				}
			}
		}
	}
	//竖线
	else{
		int index = mColLines->indexOf(currentLine);
		if (index >= 0){
			if (isLast){
				//判断是不是第一个
				if (index > 0){
					returnLine = mColLines->at(index - 1);
				}
			}
			else{
				//判断是不是最后一个
				if (index < mColLines->size() - 1){
					returnLine = mColLines->at(index + 1);
				}
			}
		}
	}
	return returnLine;
}
Line * ExcelLayout::getLineNextOrLastValidLine(QList<ExcelCellData *>* cellDatas, bool isRowLine, bool isLast){
	Line * returnLine = NULL;
	if (cellDatas->size() == 0)
	{
		return returnLine;
	}
	//横线
	if (isRowLine){
		if (isLast){
			Line * bigestLine = cellDatas->at(0)->mTop;
			int size = cellDatas->size();
			Line* temptLine = NULL;
			for (int i = 1; i < size; i++){
				temptLine = cellDatas->at(i)->mTop;
				if (temptLine->getPosition()>bigestLine->getPosition()){
					bigestLine = temptLine;
				}
			}
			returnLine = bigestLine;
		}
		else{
			Line * minimumLine = cellDatas->at(0)->mBottom;
			int size = cellDatas->size();
			Line* temptLine = NULL;
			for (int i = 1; i < size; i++){
				temptLine = cellDatas->at(i)->mBottom;
				if (temptLine->getPosition() < minimumLine->getPosition()){
					minimumLine = temptLine;
				}
			}
			returnLine = minimumLine;
		}
	}
	//竖线
	else{
		if (isLast){
			Line * bigestLine = cellDatas->at(0)->mLeft;
			int size = cellDatas->size();
			Line* temptLine = NULL;
			for (int i = 1; i < size; i++){
				temptLine = cellDatas->at(i)->mLeft;
				if (temptLine->getPosition()>bigestLine->getPosition()){
					bigestLine = temptLine;
				}
			}
			returnLine = bigestLine;
		}
		else{
			Line * minimumLine = cellDatas->at(0)->mRight;
			int size = cellDatas->size();
			Line* temptLine = NULL;
			for (int i = 1; i < size; i++){
				temptLine = cellDatas->at(i)->mRight;
				if (temptLine->getPosition() < minimumLine->getPosition()){
					minimumLine = temptLine;
				}
			}
			returnLine = minimumLine;
		}
	}
	return returnLine;
}
int ExcelLayout::getIndexOfLine(Line * currentLine, bool isRowLine){
	if (isRowLine){
		return  mRowLines->indexOf(currentLine);
	}
	else{
		return mColLines->indexOf(currentLine);
	}
}
bool ExcelLayout::changeCellsPositionLine(QList<ExcelCellData *>* mCellsSrc, Line* line, char Direction){
	if (!mCellsSrc || mCellsSrc->length() == 0 || !line)
	{
		return false;
	}
	OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
	ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
	operateMgr->addToLocalAction(changeLineAction);
	Line* preLine;
	if (Direction == BaseConstants::Direction::UP){
		int size = mCellsSrc->size();
		ExcelCellData * cell = NULL;
		for (int i = 0; i < size; i++){
			cell = mCellsSrc->at(i);
			preLine = cell->mTop;
			cell->mTop = line;
			changeLineAction->addChangeLine(cell, BaseConstants::Direction::UP, preLine, cell->mTop);
		}
	}
	else if (Direction == BaseConstants::Direction::DOWN){
		int size = mCellsSrc->size();
		ExcelCellData * cell = NULL;
		for (int i = 0; i < size; i++){
			cell = mCellsSrc->at(i);
			preLine = cell->mBottom;
			cell->mBottom = line;
			changeLineAction->addChangeLine(cell, BaseConstants::Direction::DOWN, preLine, cell->mBottom);
		}
	}
	else if (Direction == BaseConstants::Direction::LEFT){
		int size = mCellsSrc->size();
		ExcelCellData * cell = NULL;
		for (int i = 0; i < size; i++){
			cell = mCellsSrc->at(i);
			preLine = cell->mLeft;
			cell->mLeft = line;
			changeLineAction->addChangeLine(cell, BaseConstants::Direction::LEFT, preLine, cell->mLeft);
		}
	}
	else if (Direction == BaseConstants::Direction::RIGHT){
		int size = mCellsSrc->size();
		ExcelCellData * cell = NULL;
		for (int i = 0; i < size; i++){
			cell = mCellsSrc->at(i);
			preLine = cell->mRight;
			cell->mRight = line;
			changeLineAction->addChangeLine(cell, BaseConstants::Direction::RIGHT, preLine, cell->mRight);
		}
	}
	return true;
}
void ExcelLayout::seekCellsHaveLine(char Direction, Line* line, QList<ExcelCellData *>* mCellsSrc, QList<ExcelCellData *>* mCellsResult){
	if (Direction == BaseConstants::Direction::UP){
		int size = mCellsSrc->size();
		ExcelCellData * cell = NULL;
		for (int i = 0; i < size; i++){
			cell = mCellsSrc->at(i);
			if (cell->mTop == line){
				mCellsResult->push_back(cell);
			}
		}
	}
	else if (Direction == BaseConstants::Direction::DOWN){
		int size = mCellsSrc->size();
		ExcelCellData * cell = NULL;
		for (int i = 0; i < size; i++){
			cell = mCellsSrc->at(i);
			if (cell->mBottom == line){
				mCellsResult->push_back(cell);
			}
		}
	}
	else if (Direction == BaseConstants::Direction::LEFT){
		int size = mCellsSrc->size();
		ExcelCellData * cell = NULL;
		for (int i = 0; i < size; i++){
			cell = mCellsSrc->at(i);
			if (cell->mLeft == line){
				mCellsResult->push_back(cell);
			}
		}
	}
	else if (Direction == BaseConstants::Direction::RIGHT){
		int size = mCellsSrc->size();
		ExcelCellData * cell = NULL;
		for (int i = 0; i < size; i++){
			cell = mCellsSrc->at(i);
			if (cell->mRight == line){
				mCellsResult->push_back(cell);
			}
		}
	}
}
void ExcelLayout::onMeasure(MeasureWH * measureWH,bool isFreshFromInnerChange){
	if (mIsOnMeasure)
		return;
	mIsOnMeasure = true;
	MeasureWH tempMeasure;
	tempMeasure.measureWidth = measureWH->measureWidth;
	tempMeasure.measureHeight = INT_MAX;
	//tempMeasure.measureWidthMode = mComposingWidthType;
	//tempMeasure.measureHeightMode = mComposingHeightType;
	int width = 0;
	int height = 0;
	int size = mCells->size();
	ExcelCellData * cell = NULL;
	for (int i = 0; i < size; i++){
		cell = mCells->at(i);
		cell->onMeasure(&tempMeasure, isFreshFromInnerChange);
	}
	//根据单元格大小，重新设置线的位置
	size = mRowLines->size();
	Line* topLine = NULL;
	Line* bottomLine = NULL;
	int lineNumber = 0;
	int rowMaxH = 0;
	int temptHieght = 0;
	for (int i = 1; i < size; i++)
	{
		topLine = mRowLines->at(i - 1);
		bottomLine = mRowLines->at(i);
		if (mCellsTempt1->size()>0)
		{
			mCellsTempt1->clear();
		}
		if (mCellsTempt2->size() > 0)
		{
			mCellsTempt2->clear();
		}
		//seekCellsHaveLine(BaseConstants::Direction::UP, topLine, mCells, mCellsTempt1);
		//seekCellsHaveLine(BaseConstants::Direction::DOWN, bottomLine, mCellsTempt1, mCellsTempt2);
		//每一行单独存入列表
		ExcelCellDataRow* temptCellDataRow = getCellDataList(i - 1);
		temptCellDataRow->refresh();

		seekCellsHaveLine(BaseConstants::Direction::DOWN, bottomLine, mCells, mCellsTempt2);
		lineNumber = mCellsTempt2->size();
		rowMaxH = 0;
		for (int m = 0; m < lineNumber; m++){
			cell = mCellsTempt2->at(m);
			temptHieght = cell->mMeasureHeight + cell->mTop->getPosition();
			if (temptHieght>rowMaxH){
				rowMaxH = temptHieght;
			}
			temptCellDataRow->addCellDataToList(cell); //加入celldatalist
		}
		//判断当前线位置是否小于 拖动设置的位置
		if (rowMaxH < bottomLine->getDragPosition()){
			bottomLine->setPosition(bottomLine->getDragPosition());
		}
		else{
			bottomLine->setPosition(rowMaxH);
		}
	}

	Line* lastBottomLine = mRowLines->at(mRowLines->size() - 1);
	Line* lastRightLine = mColLines->at(mColLines->size() - 1);
	int w = lastRightLine->getPosition();
	int h = lastBottomLine->getPosition();
	BaseViewParam  * viewData = mViewOwner->getViewParam();
	viewData->setSelfH(h);
	viewData->setSelfW(w);
	setMeasureWH(w, h,h);
	mIsOnMeasure = false;
}
void ExcelLayout::onPrepareLayout(bool isChanged, int l, int t, int r, int b){//维护多个个联通的行
	/*int size = mCells->size();
	ExcelCellData * cell = NULL;
	for (int i = 0; i < size; i++){
	cell = mCells->at(i);
	cell->onLayout(isChanged,l,t,r,b);
	}

	Line* lastBottomLine = mRowLines->at(mRowLines->size() - 1);
	Line* lastRightLine = mColLines->at(mColLines->size() - 1);
	int w = lastRightLine->getPosition();
	int h = lastBottomLine->getPosition();
	mViewOwner->getViewParam()->setSelfW(w + mViewParam->getInsideRight());
	mViewOwner->getViewParam()->setSelfH(h + mViewParam->getInsideBottom());*/
	//ViewObj * viewParent = mViewOwner->getViewParent();
	//if (viewParent){
	//	viewParent->getViewParam()->refreshHByChildren();
	//}
	if (mIsOnMeasure)
		return;
	//判断是否联通的行
	ExcelCellDataRow* temptCellDataRow = NULL;
	int sizeRow = mRowLines->size() - 1;
	for (int i = 0; i < sizeRow; i++){
		temptCellDataRow = mCellRowList->at(i);
		bool isThrough = isCellSOnThroughRow(temptCellDataRow->getCellDataList());
		temptCellDataRow->isThrough = isThrough;
	}
	//排列到行去

}
void ExcelLayout::composing(bool isFreshFromInnerChange){
	MeasureWH tempMeasure;
	onMeasure(&tempMeasure, isFreshFromInnerChange);
	//onLayout(true,0,0,0,0);
	onPrepareLayout(true, 0, 0, 0, 0);
}

int ExcelLayout::judgeCanHaveNum(BaseContainer * child, int idx, bool isAddOrChildWHChange)
{
	return 0;
}
int ExcelLayout::conditionAddChild(BaseContainer * child, int idx)
{
	return 0;
}
//int ExcelLayout::conditionAddChildren(QList<ViewObj *> * children, int idx)
//{
//	return addChildren(children, 0, children->size(), idx);
//}
ViewObj *ExcelLayout::seekCell(int x, int y){
	int size = mCells->size();
	ExcelCellData * cell = NULL;
	for (int i = 0; i < size; i++){
		cell = mCells->at(i);
		if (BaseMath::isInsideTotalArea(x, y, cell->getViewOwner())){
			return cell->getViewOwner();
			break;
		}
	}
	return NULL;
}
void ExcelLayout::clearCellsTempT1(){
	if (mCellsTempt1->size()>0){
		mCellsTempt1->clear();
	}
}
void ExcelLayout::clearCellsTempT2(){

}
bool ExcelLayout::canMerge(QList<ExcelCellData *>* cellDataList, ExcelCellData * resultCellData){
	int size = cellDataList->size();
	if (size<2){
		return false;
	}
	if (mCellsTempt2->size()>0){
		mCellsTempt2->clear();
	}
	//判断临时celldata列表里内容大小，生成新的celldata
	int size3 = mCellsTemptUseForMerge->size();
	int numberNeedCreate = size - size3;
	ExcelCellData * creatData = NULL;
	ExcelCellData * cellData = NULL;
	for (int i = 0; i < numberNeedCreate; i++){
		creatData = createCellData(NULL, NULL, NULL, NULL);
		mCellsTemptUseForMerge->push_back(creatData);
	}
	//新生成临时的celldata,不能损坏原有celldata 数据
	cellData = NULL;
	creatData = NULL;
	for (int i = 0; i < size; i++){
		cellData = cellDataList->at(i);
		creatData = mCellsTemptUseForMerge->at(i);
		creatData->initLines(cellData->mLeft, cellData->mTop, cellData->mRight, cellData->mBottom);//需要复制其它属性
		mCellsTempt2->push_back(creatData);
	}
	resultCellData->initData();
	mergCellDatas(mCellsTempt2, resultCellData);
	if (resultCellData->mLeft){
		return true;
	}
	else{
		return false;
	}
}
void ExcelLayout::mergCellDatas(QList<ExcelCellData *>* cellDatas, ExcelCellData * resultCellData){
	int size = cellDatas->size();
	int maxIndex = size - 1;
	if (maxIndex == 0){
		ExcelCellData * finalCellData = cellDatas->at(maxIndex);
		resultCellData->initLines(finalCellData->mLeft, finalCellData->mTop, finalCellData->mRight, finalCellData->mBottom);;
		return;
	}
	QList<ExcelCellData *>* resultCellDataList = NULL;
	if (cellDatas == mCellsTempt1){
		resultCellDataList = mCellsTempt2;
	}
	else{
		resultCellDataList = mCellsTempt1;
	}
	if (resultCellDataList->size() > 0){
		resultCellDataList->clear();
	}
	ExcelCellData * cellDataTempt = NULL;
	bool hasMergeOne = false;
	//bool isMergerd = false;
	for (int i = 0; i < cellDatas->size(); i++){
		cellDataTempt = cellDatas->at(i);
		//isMergerd = false;
		for (int m = i + 1; m < cellDatas->size(); m++){
			mTemptCellData->initData();
			mergeTwoCellData(cellDataTempt, cellDatas->at(m), mTemptCellData);
			if (mTemptCellData->mLeft){
				cellDataTempt->initLines(mTemptCellData->mLeft, mTemptCellData->mTop, mTemptCellData->mRight, mTemptCellData->mBottom);
				cellDatas->removeAt(m);
				hasMergeOne = true;
				//isMergerd = true;
				break;
			}
		}
		resultCellDataList->push_back(cellDataTempt);
	}
	if (hasMergeOne){
		mergCellDatas(resultCellDataList, resultCellData);
	}
}
ExcelCellData * ExcelLayout::getCellDataFromCell(BaseElement * cell)
{
	ExcelCellData * cellData = NULL;
	ViewObj* viewObj = (ViewObj*)cell;
	if (viewObj->getObjID() != ConstantsID::ContainerId::EXCEL_PAGE)
	{
		return NULL;
	}
	BaseContentData * contentParam = viewObj->getContentData();

	using namespace std;
	//cellData = NULL;
	try{
		cellData = (ExcelCellData *)contentParam;
	}
	catch (exception& e){
	}
	return cellData;
}
void  ExcelLayout::getCellBaseInformation(int & defaultH, int & position, BaseElement * cell)
{
	ExcelCellData * cellData = NULL;
	ViewObj* viewObj = (ViewObj*)cell;
	if (viewObj->getObjID() != ConstantsID::ContainerId::EXCEL_PAGE)
	{
		return ;
	}
	BaseContentData * contentParam = viewObj->getContentData();
	cellData = (ExcelCellData *)contentParam;
	int cellLineHeight = cellData->getCellLineHeight();
	if (cellData->mCellStatus == CellStatusType::OnMeause)
	{
		defaultH = cellData->mMeasureHeight;
	}
	else if (cellData->mCellStatus == CellStatusType::OnLayout)
	{
		defaultH =  cellLineHeight;
	}

	position = cellData->mVerticalPosition;
}
int ExcelLayout::getWidthFormCellData(ExcelCellData* cellData)
{
	return cellData->mRight->getPosition() - cellData->mLeft->getPosition();
}
int ExcelLayout::getWidthFormCellData(BaseElement * cell)
{
	ExcelCellData * cellData = getCellDataFromCell(cell);
	return getWidthFormCellData(cellData);
}
void ExcelLayout::getCellDatasFormCellObjs(QList<ViewObj *> * cellsObjList, QList<ExcelCellData *>* resultCellDataList){
	if (resultCellDataList->size() > 0){
		resultCellDataList->clear();
	}
	int size = cellsObjList->size();
	ViewObj * cell = NULL;
	ExcelCellData * cellData = NULL;
	for (int i = 0; i < size; i++){
		cellData = NULL;
		cell = cellsObjList->at(i);
		cellData = getCellDataFromCell(cell);
		if (cellData){
			resultCellDataList->push_back(cellData);
		}
	}
}
bool ExcelLayout::mergeTwoCellData(ExcelCellData * cellData1, ExcelCellData * cellData2, ExcelCellData * resultCellData){
	bool isMerged = false;
	//如果相同位置有2条相同的线，且还有另外不同位置，有一边相等，就可以合并
	if (cellData1->mLeft == cellData2->mLeft&&cellData1->mRight == cellData2->mRight){
		if (cellData1->mTop == cellData2->mTop || cellData1->mBottom == cellData2->mBottom){
		}
		else if (cellData1->mTop == cellData2->mBottom){
			resultCellData->mTop = cellData2->mTop;
			resultCellData->mBottom = cellData1->mBottom;
			resultCellData->mLeft = cellData1->mLeft;
			resultCellData->mRight = cellData1->mRight;
			isMerged = true;
		}
		else if (cellData1->mBottom == cellData2->mTop){
			resultCellData->mTop = cellData1->mTop;
			resultCellData->mBottom = cellData2->mBottom;
			resultCellData->mLeft = cellData1->mLeft;
			resultCellData->mRight = cellData1->mRight;
			isMerged = true;
		}
	}
	else if (cellData1->mTop == cellData2->mTop&&cellData1->mBottom == cellData2->mBottom){
		if (cellData1->mLeft == cellData2->mLeft || cellData1->mRight == cellData2->mRight){
		}
		else if (cellData1->mLeft == cellData2->mRight){
			resultCellData->mTop = cellData1->mTop;
			resultCellData->mBottom = cellData1->mBottom;
			resultCellData->mLeft = cellData2->mLeft;
			resultCellData->mRight = cellData1->mRight;
			isMerged = true;
		}
		else if (cellData1->mRight == cellData2->mLeft){
			resultCellData->mTop = cellData1->mTop;
			resultCellData->mBottom = cellData1->mBottom;
			resultCellData->mLeft = cellData1->mLeft;
			resultCellData->mRight = cellData2->mRight;
			isMerged = true;
		}
	}
	return isMerged;
}
bool ExcelLayout::mergeCell(QList<ViewObj *> * cellsObjList)
{
	mTemptCellDataMergeReult->initData();
	getCellDatasFormCellObjs(cellsObjList,mCellsTempt1);
	canMerge(mCellsTempt1, mTemptCellDataMergeReult);
	if (mTemptCellDataMergeReult->mLeft){
		OperateMgr *operateMgr =  mExcelTemplete->getOperateMgr();
		operateMgr->createNewAction();
		//删除除了第一个单元格以外的所有单元格
		ViewObj * LeftTopObj = findLeftAndTopCellObj(cellsObjList, mTemptCellDataMergeReult);
		ExcelCellData* leftTopCellData = getCellDataFromCell(LeftTopObj);//先变化焦点
		//设置焦点对象操作回退
		ViewObj * preFocus = NULL;
		char preDir = 0;
		operateMgr->getFocusAndDirection(preFocus, preDir);
		//ViewObj* cellViewObj = leftTopCellData->getViewOwner();
		//ViewObj* setAfterFocus = ((BaseParagraph*)cellViewObj->getTempParent()->getChild(0))->getFirstChildView();
		ViewObj* setAfterFocus = leftTopCellData->mRootTemplete->getFirstChildView();
		setAfterFocus->setFocus(false);
		SetFocusAction*  setFocusAction = new SetFocusAction(operateMgr->getCurStepMgr());
		setFocusAction->initAction(preFocus, preDir, setAfterFocus, BaseConstants::Direction::LEFT);
		operateMgr->addToLocalAction(setFocusAction);

		int size = cellsObjList->size();
		ViewObj * temptViewObj;
		for (int i = 0; i < size; i++){
			temptViewObj = cellsObjList->at(i);
			if (temptViewObj != LeftTopObj){
				deleteCellObj(temptViewObj);
			}
		}
			
		//改变当前单元格的大小
		
		//换线动作加入撤销管理
		if (leftTopCellData->mLeft != mTemptCellDataMergeReult->mLeft)
		{
			ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
			operateMgr->addToLocalAction(changeLineAction);
			changeLineAction->addChangeLine(leftTopCellData, BaseConstants::Direction::LEFT, leftTopCellData->mLeft, mTemptCellDataMergeReult->mLeft);
		}
		if (leftTopCellData->mTop != mTemptCellDataMergeReult->mTop)
		{
			ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
			operateMgr->addToLocalAction(changeLineAction);
			changeLineAction->addChangeLine(leftTopCellData, BaseConstants::Direction::UP, leftTopCellData->mTop, mTemptCellDataMergeReult->mTop);
		}
		if (leftTopCellData->mRight != mTemptCellDataMergeReult->mRight)
		{
			ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
			operateMgr->addToLocalAction(changeLineAction);
			changeLineAction->addChangeLine(leftTopCellData, BaseConstants::Direction::RIGHT, leftTopCellData->mRight, mTemptCellDataMergeReult->mRight);
		}
		if (leftTopCellData->mBottom != mTemptCellDataMergeReult->mBottom)
		{
			ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
			operateMgr->addToLocalAction(changeLineAction);
			changeLineAction->addChangeLine(leftTopCellData, BaseConstants::Direction::DOWN, leftTopCellData->mBottom, mTemptCellDataMergeReult->mBottom);
		}
		//改变当前选中颜色
		LeftTopObj->setMaskColor(BaseConstants::NONE);
		leftTopCellData->initLines(mTemptCellDataMergeReult->mLeft, mTemptCellDataMergeReult->mTop, mTemptCellDataMergeReult->mRight, mTemptCellDataMergeReult->mBottom);
		

		//移除多余的线
		removeUselessLine();
		ExcelRelayoutAction* relayoutAction = new ExcelRelayoutAction(operateMgr->getCurStepMgr(), mExcelTemplete);
		operateMgr->addToLocalActionAt(relayoutAction, 1);
		mExcelTemplete->requestLayout();
		return true;
	}
	return false;
}
ViewObj * ExcelLayout::findLeftAndTopCellObj(QList<ViewObj *> * cellsObjList, ExcelCellData* mergedCellData){
	getCellDatasFormCellObjs(cellsObjList, mCellsTempt1);
	int size = mCellsTempt1->size();
	ExcelCellData* temptData;
	for (int i = 0; i < size; i++){
		temptData = mCellsTempt1->at(i);
		if (temptData->mLeft == mergedCellData->mLeft&&temptData->mTop == mergedCellData->mTop){
			return temptData->getViewOwner();
		}
	}
	return NULL;
}
void ExcelLayout::deleteCellObj(ViewObj * cell){
	ExcelCellData* cellData = getCellDataFromCell(cell);
	addOrRomoveCellData(cellData,false);
	cell->getViewParent()->getChildrenMgr()->delChild(cell, true);
	//QList<BaseContainer *> * childrenList = mViewOwner->getChildren();
	//childrenList->removeOne(cell);
	//delete cellData;
	//delete cell;
}
void ExcelLayout::deleteCellData(ExcelCellData * cellData)
{
	addOrRomoveCellData(cellData, false);
	ViewObj * cell = cellData->getViewOwner();
	cell->getViewParent()->getChildrenMgr()->delChild(cell, true);
	//delete cell;
	//delete cellData;
}
bool ExcelLayout::dividCell(ViewObj * cellObj, bool isHorizontal, int number){
	if (isHorizontal){
		return dividCellOnHorizontal(cellObj, number);
	}
	else{
		dividCellOnVertical(cellObj, number);
		return true;
	}
}
Line * ExcelLayout::getNearestColOrRowLine(bool isRowLine, int postion, int amountOfError){
	Line * nearestLine = NULL;
	if (isRowLine){
		nearestLine = findNearestLine(mRowLines, postion, amountOfError);
	}
	else{
		nearestLine = findNearestLine(mColLines, postion, amountOfError);
	}
	if (!nearestLine){
		nearestLine = new Line(postion);
		nearestLine->setDragPosition(postion);
	}
	return nearestLine;
}
bool ExcelLayout::dividCellOnHorizontal(ViewObj * cellObj, int number){
	ExcelCellData* cellData = getCellDataFromCell(cellObj);
	bool isSuccess = dividCellOnHorizontal(cellData, number);
	if (isSuccess){

	}
	else
	{
		MessageBox::showAboutMessageBox(QStringLiteral("设置参数错误"), QStringLiteral("单元格不能再拆分!"));
		return false;
	}
	 mExcelTemplete->requestLayout();
	 return true;
}
bool ExcelLayout::setCellWidth(ExcelCellData * cellData, int setWidth)
{
	Line * rightLine = cellData->mRight;
	ViewObj* cellView = cellData->getViewOwner();
	int currentCellWidth = cellData->mRight->getPosition() - cellData->mLeft->getPosition();
	int currentExcelWidth = getExcelWidth();
	int needMoveCellLineDistance = setWidth - currentCellWidth;
	int ifSetExcelWidth = needMoveCellLineDistance + currentExcelWidth;
	ViewObj* row = cellView->getParentViewById(ConstantsID::ContainerId::NORMAL_ROW);
	BaseViewParam * viewParam = row->getViewParam();
	int rowWidth = viewParam->getSelfW();
	int indexOfstart = getIndexOfLine(rightLine, false);
	if (ifSetExcelWidth>rowWidth)
	{
		int needNarrowWidth = ifSetExcelWidth - rowWidth;
		//if (singleWidth<defaultCellW)
			//int indexOfstart = getIndexOfLine(cellData->mRight, false);
			//singleWidth = defaultCellW;
			//整体移动线
			//needMoveDistance = defaultCellW*number - totalWidth;
			/*int indexOfstart = getIndexOfLine(cellData->mRight, false);
			moveSomeLines(mColLines, indexOfstart, mColLines->size(), needMoveDistance);*/
			QList<int> lineMoveData;
			QList<int> fixedLineList;
			fixedLineList.clear();
			fixedLineList.push_back(indexOfstart);
			checkNarrowEachLineDistance(needNarrowWidth, mColLines, lineMoveData, fixedLineList);
			if (lineMoveData.size() == 0)
			{
				
				return false;
			}
			//createActionToRemeberOperate();
			narrowEachLineDistance(needNarrowWidth, mColLines, lineMoveData);
			//int temptLinePostion = cellData->mLeft->getPosition();
			moveSomeLines(mColLines, indexOfstart, mColLines->size(), needMoveCellLineDistance);
			return true;
	}
	else
	{
		if (mCellsTempt1->size())
		{
			mCellsTempt1->clear();
		}
		seekCellsHaveLine(BaseConstants::Direction::RIGHT, rightLine, mCells, mCellsTempt1);
		int tempCellMinWidth = getXCanSet(BaseConstants::Direction::RIGHT, mCellsTempt1);
		if (setWidth<tempCellMinWidth)
		{
			return false;
		}
		moveSomeLines(mColLines, indexOfstart, mColLines->size(), needMoveCellLineDistance);
		return true;
	}
}
	
bool ExcelLayout::dividCellOnHorizontal(ExcelCellData * cellData, int number)
{
	if (mCellsTempt4->size() > 0)
	{
		mCellsTempt4->clear();
	}
	if (number == 1){
		mCellsTempt4->push_back(cellData);
		return true;
	}
	else if (number<1){
		return false;
	}
	mIsOnMeasure = true;
	int currentCellWidth = cellData->mRight->getPosition() - cellData->mLeft->getPosition();
	ViewObj* row = cellData->getViewOwner()->getParentViewById(ConstantsID::ContainerId::NORMAL_ROW);
	BaseViewParam * viewParamOfRow = row->getViewParam();
	int rowWidth = viewParamOfRow->getSelfW();
	int cellMiniWidth = cellData->getMaxWidthOfChildView();
	if (cellMiniWidth<defaultCellW)
	{
		cellMiniWidth = defaultCellW;
	}
	int newMiniCellSTotalWidth = (number - 1)*defaultCellW;
	int miniWidthOfTotalSet = cellMiniWidth + newMiniCellSTotalWidth;
	int firsCellWidth = 0;
	/*if (miniWidthOfTotalSet>rowWidth)
	{
		return false;
	}
	else*/
	{
		//先用自己的宽度来满足生成新的单元格
		if (miniWidthOfTotalSet>currentCellWidth)
		{
			if (setCellWidth(cellData, miniWidthOfTotalSet))
			{
				firsCellWidth = cellMiniWidth;
			}
			else
			{
				mIsOnMeasure = false;
				return false;
			}
		}
		else
		{
			//firsCellWidth = currentCellWidth - newMiniCellSTotalWidth;
			firsCellWidth = currentCellWidth / number;
		}
	}
	
	if (mTempLineList->size()>0)
	{
		mTempLineList->clear();
	}
	
	mTempLineList->push_back(cellData->mLeft);
	Line *temptLine;
	int startLeft = cellData->mLeft->getPosition();
	int temptLinePostion = 0;
	for (int i = 1; i < number; i++){
		if (i==1)
		{
			temptLinePostion = startLeft + firsCellWidth;
		}
		else
		{
			temptLinePostion = startLeft + firsCellWidth + (i - 1)*firsCellWidth;
		}
		
		temptLine = getNearestColOrRowLine(false, temptLinePostion,0);
		insertLine(temptLine, mColLines);
		mTempLineList->push_back(temptLine);
	}
	mTempLineList->push_back(cellData->mRight);

	
	//单独处理第一个单元格
	Line * lineStartLeft = cellData->mLeft;
	//Line * next = getLineNextOrLastLine(lineStartLeft,false,false);
	Line * next = mTempLineList->at(1);
	//撤销前进动作记录
	OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
	ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
	operateMgr->addToLocalAction(changeLineAction);
	changeLineAction->addChangeLine(cellData, BaseConstants::Direction::RIGHT, cellData->mRight, next);

	cellData->mRight = next;
	ViewObj* tempPage = cellData->getViewOwner();
	VerticalViewParam * viewParam = (VerticalViewParam *)tempPage->getViewParam();
	int insideWidth = viewParam->getInsideLeft();
	mCellsTempt4->push_back(cellData);
	ExcelCellData* temptCellData;
	for (int i = 1; i < number; i++)
	{
		lineStartLeft = next;
		//next = getLineNextOrLastLine(lineStartLeft, false, false);
		next = mTempLineList->at(i + 1);
		temptCellData = createCellData(lineStartLeft, cellData->mTop, next, cellData->mBottom);
		addOrRomoveCellData(temptCellData,true);
		ViewFactory::createExcelCell(mViewOwner, temptCellData);
		mCellsTempt4->push_back(temptCellData);
		ExcelLayout::setExcelInsideProperty(tempPage, insideWidth);
	}
	mIsOnMeasure = false;
	return true;
}
void ExcelLayout::dividCellOnVertical(ViewObj * cellObj, int number){
	ExcelCellData* cellData = getCellDataFromCell(cellObj);
	dividCellOnVertical(cellData, number);
	mExcelTemplete->requestLayout();
}
void ExcelLayout::dividCellOnVertical(ExcelCellData * cellData, int number)
{
	if (number < 2)
	{
		return;
	}
	mIsOnMeasure = true;
	int totalHeight = cellData->mBottom->getPosition() - cellData->mTop->getPosition();
	int singleHeight = totalHeight / number;
	int needMoveDistance = 0;
	if (singleHeight<defaultCellH)
	{
		singleHeight = defaultCellH;
		//整体移动线
		needMoveDistance = defaultCellH*number - totalHeight;
		int indexOfstart = getIndexOfLine(cellData->mBottom, true);
		moveSomeLines(mRowLines, indexOfstart, mRowLines->size(), needMoveDistance);
	}
	if (mTempLineList->size()>0)
	{
		mTempLineList->clear();
	}
	mTempLineList->push_back(cellData->mTop);
	Line *temptLine;
	int startTop = cellData->mTop->getPosition();
	int temptLinePostion = 0;
	for (int i = 1; i < number; i++)
	{
		temptLinePostion = startTop + i*singleHeight;
		temptLine = getNearestColOrRowLine(true, temptLinePostion,0);
		insertLine(temptLine, mRowLines);
		mTempLineList->push_back(temptLine);
	}
	mTempLineList->push_back(cellData->mBottom);
	//单独处理第一个单元格
	Line * lineStartTop = cellData->mTop;
	//Line * next = getLineNextOrLastLine(lineStartTop, true, false);
	Line * next = mTempLineList->at(1);
	OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
	ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
	operateMgr->addToLocalAction(changeLineAction);
	changeLineAction->addChangeLine(cellData, BaseConstants::Direction::DOWN, cellData->mBottom, next);//撤销前进动作记录
	

	cellData->mBottom = next;
	ViewObj* tempPage = cellData->getViewOwner();
	VerticalViewParam * viewParam = (VerticalViewParam *)tempPage->getViewParam();
	int insideWidth = viewParam->getInsideLeft();
	ExcelCellData* temptCellData;
	for (int i = 1; i < number; i++)
	{
		lineStartTop = next;
		//next = getLineNextOrLastLine(lineStartTop, true, false);
		next = mTempLineList->at(i + 1);;
		temptCellData = createCellData(cellData->mLeft, lineStartTop, cellData->mRight, next);
		addOrRomoveCellData(temptCellData, true);
		ViewFactory::createExcelCell(mViewOwner, temptCellData);
		ExcelLayout::setExcelInsideProperty(tempPage, insideWidth);
	}
	mIsOnMeasure = false;
}
ExcelCellDataRow* ExcelLayout::getCellDataList(int rowIndex)
{
	int size = mCellRowList->size();
	if (rowIndex >= size)
	{
		ExcelCellDataRow* temptCellDataList;
		for (int i = size; i <= rowIndex; i++)
		{
			temptCellDataList = new ExcelCellDataRow();
			mCellRowList->push_back(temptCellDataList);
		}
	}
	return mCellRowList->at(rowIndex);
}
int ExcelLayout::getCellRowSize()
{
	return mCellRowList->size();
}
void ExcelLayout::removeNotShowCell(QList<BaseContainer *> * showList)
{
	int indexOfshow;
	ExcelCellData * tempCellData;
	for (int i = 0; i < mCells->size();)
	{
		tempCellData = mCells->at(i);
		indexOfshow = showList->indexOf(tempCellData->getViewOwner());
		if (indexOfshow < 0)
		{
			mCells->removeAll(tempCellData);
		}
		else
		{
			i++;
		}

	}
	removeUselessLine(false);
}
bool ExcelLayout::isCellSOnThroughRow(QList<ExcelCellData *>* cells)
{
	ExcelCellData * tempCellData1 = NULL;
	ExcelCellData * tempCellData2 = NULL;
	int lessIndex = 0;
	int size = cells->size();//左边线升序
	for (int i = 1; i < size; i++)
	{
		tempCellData1 = cells->at(i);
		for (lessIndex = i - 1; lessIndex >= 0; lessIndex--)
		{
			tempCellData2 = cells->at(lessIndex);
			if (tempCellData1->mLeft->getPosition() >= tempCellData2->mLeft->getPosition())
			{
				break;
			}
			else
			{
				cells->swap(lessIndex, lessIndex + 1);
			}
		}
	}
	//判断相邻
	for (int i = 1; i < size; i++)
	{
		tempCellData1 = cells->at(i);
		tempCellData2 = cells->at(i - 1);
		if (tempCellData1->mBottom != tempCellData2->mBottom)
		{
			return false;
		}
		if (tempCellData1->mLeft != tempCellData2->mRight)
		{
			return false;
		}
	}
	tempCellData1 = cells->at(0);
	if (tempCellData1->mLeft == mColLines->at(0))
	{

	}
	else{

		if (mCellsTempt2->size() > 0)
		{
			mCellsTempt2->clear();
		}
		seekCellsHaveLine(BaseConstants::Direction::LEFT, mColLines->at(0), mCells, mCellsTempt2);
		int size2 = mCellsTempt2->size();
		for (int i = 1; i < size2; i++)
		{
			tempCellData2 = mCellsTempt2->at(i);
			if (tempCellData1->mTop->getPosition() >= tempCellData2->mTop->getPosition() && tempCellData1->mBottom->getPosition() <= tempCellData2->mBottom->getPosition())
			{
				return false;
			}
		}
	}
	tempCellData1 = cells->at(size - 1);
	int colLineSize = mColLines->size();
	if (tempCellData1->mRight == mColLines->at(colLineSize - 1))
	{

	}
	else{
		if (mCellsTempt2->size() > 0)
		{
			mCellsTempt2->clear();
		}
		seekCellsHaveLine(BaseConstants::Direction::RIGHT, mColLines->at(colLineSize - 1), mCells, mCellsTempt2);
		int size2 = mCellsTempt2->size();
		for (int i = 1; i < size2; i++)
		{
			tempCellData2 = mCellsTempt2->at(i);
			if (tempCellData1->mTop->getPosition() >= tempCellData2->mTop->getPosition() && tempCellData1->mBottom->getPosition() <= tempCellData2->mBottom->getPosition())
			{
				return false;
			}
		}
	}
	return true;
}
ExcelTemplete* ExcelLayout::getExcelTemplete()
{
	return mExcelTemplete;
}
void ExcelLayout::showTidy()
{
	int size = mCells->size();
	ExcelCellData * cellData;
	for (int i = 0; i < size; i++)
	{
		cellData = mCells->at(i);
		cellData->mRootTemplete->showTidy();
	}
}
void ExcelLayout::dividCell(ExcelCellData * cellData, int row, int col)
{
	ViewObj* rowObj = cellData->getViewOwner()->getParentViewById(ConstantsID::ContainerId::NORMAL_ROW);
	if (!ExcelLayout::canGenerateExcel(row, col, rowObj))
	{
		return;
	}
	OperateMgr *operateMgr = mExcelTemplete->getOperateMgr();
	operateMgr->createNewAction();
	//设置焦点对象操作回退
	
	ViewObj * preFocus = NULL;
	char preDir =0;
	operateMgr->getFocusAndDirection(preFocus, preDir);
	SetFocusAction*  setFocusAction = new SetFocusAction(operateMgr->getCurStepMgr());
	setFocusAction->initAction(preFocus, preDir, preFocus, preDir);
	operateMgr->addToLocalAction(setFocusAction);

	mIsOnMeasure = true;
	dividCellOnHorizontal(cellData, col);
	int size = mCellsTempt4->size();
	if (size==0){
		//winData->deleteAction();
		mIsOnMeasure = false;
		MessageBox::showAboutMessageBox(QStringLiteral("设置参数错误"), QStringLiteral("单元格不能拆分！拆分的行或列太大"));
		return;
	}
	for (int i = 0; i < size; i++)
	{
		dividCellOnVertical(mCellsTempt4->at(i), row);
	}
	mIsOnMeasure = false;
	mExcelTemplete->requestLayout();
	ExcelRelayoutAction* relayoutAction = new ExcelRelayoutAction(operateMgr->getCurStepMgr(), mExcelTemplete);
	operateMgr->addToLocalActionAt(relayoutAction, 1);
}
void ExcelLayout::createActionToRemeberOperate()
{

	OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
	operateMgr->createNewAction();
	//设置焦点对象操作回退
	ViewObj * preFocus = NULL;
	char preDir = 0;
	operateMgr->getFocusAndDirection(preFocus, preDir);
	SetFocusAction*  setFocusAction = new SetFocusAction(operateMgr->getCurStepMgr());
	setFocusAction->initAction(preFocus, preDir, preFocus, preDir);
	operateMgr->addToLocalAction(setFocusAction);
}
/**
* direction 表示插入行或列 1 代表上一行，2代表下一行，3代表左一列，4代表右一列
*/
void ExcelLayout::addRowOrCol(ExcelCellData * cellData, int direction)
{
	if (mCellsTempt1->size()){
		mCellsTempt1->clear();
	}
	OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
	ViewObj* currentCellPage = cellData->getViewOwner();
	if (direction == 1)
	{
		createActionToRemeberOperate();
		int temptLinePostion = cellData->mTop->getPosition();
		int indexOfstart = getIndexOfLine(cellData->mTop, true);
		moveSomeLines(mRowLines, indexOfstart, mRowLines->size(), defaultCellH);
		Line * temptLine = getNearestColOrRowLine(true, temptLinePostion,0);
		insertLine(temptLine, mRowLines);
		if (indexOfstart > 0){
			if (mCellsTempt1->size())
			{
				mCellsTempt1->clear();
			}
			seekCellsHaveLine(BaseConstants::Direction::DOWN, cellData->mTop, mCells, mCellsTempt1);
			int handleBottomSize = mCellsTempt1->size();
			ExcelCellData * tempCellData;
			for (int i = 0; i < handleBottomSize; i++)
			{
				tempCellData = mCellsTempt1->at(i);
				ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
				operateMgr->addToLocalAction(changeLineAction);
				changeLineAction->addChangeLine(tempCellData, BaseConstants::Direction::DOWN, tempCellData->mBottom, temptLine);
				tempCellData->mBottom = temptLine;
			}
		}
		generateCellData(temptLine, cellData->mTop, mColLines, currentCellPage);
	}
	else if (direction == 2)
	{
		createActionToRemeberOperate();
		int temptLinePostion = cellData->mBottom->getPosition();
		int indexOfstart = getIndexOfLine(cellData->mBottom, true) + 1;
		int currenLineSize = mRowLines->size();
		if (indexOfstart < currenLineSize){
			moveSomeLines(mRowLines, indexOfstart, mRowLines->size(), defaultCellH);
		}
		Line * temptLine = getNearestColOrRowLine(true, temptLinePostion + defaultCellH, 0);
		insertLine(temptLine, mRowLines);
		if (indexOfstart < currenLineSize){
			if (mCellsTempt1->size())
			{
				mCellsTempt1->clear();
			}
			seekCellsHaveLine(BaseConstants::Direction::UP, cellData->mBottom, mCells, mCellsTempt1);
			int handleBottomSize = mCellsTempt1->size();
			ExcelCellData * tempCellData;
			for (int i = 0; i < handleBottomSize; i++)
			{
				tempCellData = mCellsTempt1->at(i);
				ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
				operateMgr->addToLocalAction(changeLineAction);
				changeLineAction->addChangeLine(tempCellData, BaseConstants::Direction::UP, tempCellData->mTop, temptLine);
				tempCellData->mTop = temptLine;
			}
		}
		generateCellData(cellData->mBottom, temptLine, mColLines, currentCellPage);
	}
	else if (direction == 3)
	{
		QList<int> lineMoveData;
		QList<int> fixedLineList;
		fixedLineList.clear();
		checkNarrowEachLineDistance(defaultCellW, mColLines, lineMoveData, fixedLineList);
 		if (lineMoveData.size()==0)
		{
			return;
		}

		createActionToRemeberOperate();
		/*if (isSuccess)
		{
			narrowEachLineDistance(GenerateCellW, mColLines);
		}*/
		narrowEachLineDistance(defaultCellW, mColLines, lineMoveData);
		int temptLinePostion = cellData->mLeft->getPosition();
		int indexOfstart = getIndexOfLine(cellData->mLeft, false);
		moveSomeLines(mColLines, indexOfstart, mColLines->size(), defaultCellW);
		Line * temptLine = getNearestColOrRowLine(false, temptLinePostion,0);
		insertLine(temptLine, mColLines);
		if (indexOfstart > 0)
		{
			if (mCellsTempt1->size())
			{
				mCellsTempt1->clear();
			}
			seekCellsHaveLine(BaseConstants::Direction::RIGHT, cellData->mLeft, mCells, mCellsTempt1);
			int handleBottomSize = mCellsTempt1->size();
			ExcelCellData * tempCellData;
			for (int i = 0; i < handleBottomSize; i++)
			{
				tempCellData = mCellsTempt1->at(i);
				ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
				operateMgr->addToLocalAction(changeLineAction);
				changeLineAction->addChangeLine(tempCellData, BaseConstants::Direction::RIGHT, tempCellData->mRight, temptLine);
				tempCellData->mRight = temptLine;
			}
		}
		generateCellData(temptLine, cellData->mLeft, mRowLines, currentCellPage);
	}
	else if (direction == 4)
	{
	//	bool isSuccess =checkNarrowEachLineDistance(GenerateCellW, mColLines);
		QList<int> lineMoveData;
		QList<int> fixedLineList;
		fixedLineList.clear();
		checkNarrowEachLineDistance(defaultCellW, mColLines, lineMoveData, fixedLineList);
		if (lineMoveData.size() == 0)
		{
			return;
		}
		createActionToRemeberOperate();
		narrowEachLineDistance(defaultCellW, mColLines, lineMoveData);
		int temptLinePostion = cellData->mRight->getPosition();
		int indexOfstart = getIndexOfLine(cellData->mRight, false) + 1;
		int currenLineSize = mColLines->size();
		if (indexOfstart < currenLineSize){
			moveSomeLines(mColLines, indexOfstart, mColLines->size(), defaultCellW);
		}
		Line * temptLine = getNearestColOrRowLine(false, temptLinePostion + defaultCellW, 0);
		insertLine(temptLine, mColLines);
		if (indexOfstart < currenLineSize){
			if (mCellsTempt1->size())
			{
				mCellsTempt1->clear();
			}
			seekCellsHaveLine(BaseConstants::Direction::LEFT, cellData->mRight, mCells, mCellsTempt1);
			int handleBottomSize = mCellsTempt1->size();
			ExcelCellData * tempCellData;
			for (int i = 0; i < handleBottomSize; i++)
			{
				tempCellData = mCellsTempt1->at(i);
				ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
				operateMgr->addToLocalAction(changeLineAction);
				changeLineAction->addChangeLine(tempCellData, BaseConstants::Direction::LEFT, tempCellData->mLeft, temptLine);
				tempCellData->mLeft = temptLine;
			}
		}
		generateCellData(cellData->mRight, temptLine, mRowLines, currentCellPage);
	}
	mExcelTemplete->requestLayout();
	ExcelRelayoutAction* relayoutAction = new ExcelRelayoutAction(operateMgr->getCurStepMgr(), mExcelTemplete);
	operateMgr->addToLocalActionAt(relayoutAction, 1);
}
void ExcelLayout::narrowEachLineDistance(int totalDistance, QList<Line *> * mLines, QList<int>& moveData)
{
	int size = moveData.size();
	if (size <= 1){
		return;
	}
	Line* tempLine;
	int indexOfLine;
	for (int i = 0; i < size; i +=2)
	{
		indexOfLine = moveData.at(i);
		tempLine = mLines->at(indexOfLine);
		moveSomeLines(mLines, indexOfLine, mLines->size(), moveData.at(i + 1));
		/*BaseWinData * winData = mExcelTemplete->getWinData();
		LineMoveAction *moveLineAction = new LineMoveAction(winData, mExcelTemplete);
		winData->addToLocalAction(moveLineAction);
		prePosition = tempLine->getDragPosition();
		tempLine->addAllPosition(narrowEachDistance*i);
		moveLineAction->addMoveLine(tempLine, prePosition, tempLine->getDragPosition());*/
	}
}
/*
*采用从最大的直接减少一个
宽度，不能缩小的值往后面累加
*moveData数据结构线索引号+移动距离
*/
void ExcelLayout::checkNarrowEachLineDistance(int totalDistance, QList<Line *> * mLines, QList<int>& moveData, QList<int> &fixedLineIndexList)
{
	moveData.clear();
	int size = mLines->size();
	if (size <= 1){
		return ;
	}
	int fixedSiz = fixedLineIndexList.size();
	int fixedIndex;
	bool isFixed;
	for (int m = 1; m < size; m++)
	{
		isFixed = false;
		for (int n = 0; n < fixedSiz; n++)
		{
			fixedIndex = fixedLineIndexList.at(n);
			if (fixedIndex ==m)
			{
				isFixed = true;
				break;
			}
		}
		if (!isFixed)
		{
			moveData.push_back(m);
		}
	}
	QList<int> tempSortLineList;
	int width;
	Line* tempLine;
	Line* preLine;
	int maxWidth;
	int lineIndex;
	int maxWidthLineIndex;
	int innerMaxIndex;
	while (moveData.size()>0)
	{
		maxWidthLineIndex = -1;
		maxWidth = -1;
		for (int i = 0; i <moveData.size(); i++)
		{
			lineIndex = moveData.at(i);
			tempLine = mLines->at(lineIndex);
			preLine = mLines->at(lineIndex - 1);
			width = tempLine->getDragPosition() - preLine->getDragPosition();
			if (width > maxWidth)
			{
				maxWidth = width;
				maxWidthLineIndex = lineIndex;
				innerMaxIndex = i;
			}
			
		}
		moveData.removeAt(innerMaxIndex);
		tempSortLineList.push_back(maxWidthLineIndex);
	}
	
	moveData.clear();
	int narrowEachDistance = 0;
	//Line* tempLine;
	//Line* preLine;
	
	int lastRemainingWidth = totalDistance;
	size = tempSortLineList.size();
	int tempCellMinWidth;
	int canSetMinX;
	//先检测缩小后是否大于最小宽度
	for (int i = 0; i < size; i++)
	{
		lineIndex = tempSortLineList.at(i);
		tempLine = mLines->at(lineIndex);
		preLine = mLines->at(lineIndex - 1);
		int shouldTotalMove = narrowEachDistance + lastRemainingWidth;
		width = tempLine->getDragPosition() - preLine->getDragPosition() - shouldTotalMove;
		if (mCellsTempt1->size())
		{
			mCellsTempt1->clear();
		}
		seekCellsHaveLine(BaseConstants::Direction::RIGHT, tempLine, mCells, mCellsTempt1);
		canSetMinX = getXCanSet(BaseConstants::Direction::RIGHT, mCellsTempt1);
		tempCellMinWidth = canSetMinX - preLine->getDragPosition();
	
		if (width<tempCellMinWidth)
		{
			lastRemainingWidth = tempCellMinWidth - width;
			int canMoveWidth = tempLine->getDragPosition() - preLine->getDragPosition() - tempCellMinWidth;
			moveData.push_back(lineIndex);//索引
			moveData.push_back(-canMoveWidth);//移动宽度
		}
		else
		{
			moveData.push_back(lineIndex);//索引
			moveData.push_back(-shouldTotalMove);//移动宽度
			shouldTotalMove = 0;
			lastRemainingWidth = 0;
		}
		if (lastRemainingWidth == 0){
			return;
		}
	}
	if (lastRemainingWidth>0)
	{
		moveData.clear();
	}
}
void ExcelLayout::generateCellData(Line* first, Line* second, QList<Line *> * mLines, ViewObj* focusPageCell)
{
	VerticalViewParam  * viewParam = NULL;
	viewParam = (VerticalViewParam *)focusPageCell->getViewParam();
	int insideWidth = viewParam->getInsideLeft();
	Line * tempFirst;
	Line * tempSecond;
	ExcelCellData* temptCellData;
	bool isGenerateRow = mLines == mColLines ? true : false;
	int maxIndex = mLines->size() - 1;
	ViewObj* tempPage;
	for (int i = 0; i < maxIndex; i++)
	{
		tempFirst = mLines->at(i);
		tempSecond = mLines->at(i + 1);
		//bool isCross = false;
		if (isGenerateRow){
			if (isRectCrossSomeCellData(tempFirst->getPosition(), first->getPosition(), tempSecond->getPosition(), second->getPosition())){
				continue;
			}
			else{
				temptCellData = createCellData(tempFirst, first, tempSecond, second);
			}
		}
		else{
			if (isRectCrossSomeCellData(first->getPosition(), tempFirst->getPosition(), second->getPosition(), tempSecond->getPosition())){
				continue;
			}
			else{
				temptCellData = createCellData(first, tempFirst, second, tempSecond);
			}
		}
		addOrRomoveCellData(temptCellData, true);
		tempPage = ViewFactory::createExcelCell(mViewOwner, temptCellData);
		ExcelLayout::setExcelInsideProperty(tempPage, insideWidth);
	}
}
bool ExcelLayout::isRectCrossSomeCellData(int left1, int top1, int right1, int bottom1)
{
	int cellDataSize = mCells->size();
	ExcelCellData* temptCellData;
	for (int i = 0; i < cellDataSize; i++){
		temptCellData = mCells->at(i);
		if (BaseMath::isRectCrossExceptSameSide(left1, top1, right1, bottom1, temptCellData->mLeft->getPosition(), temptCellData->mTop->getPosition()
			, temptCellData->mRight->getPosition(), temptCellData->mBottom->getPosition())){
			return true;
		}
	}
	return false;
}
void ExcelLayout::deleteRowOrCol(ExcelCellData * cellData, bool isRow)
{
	//ViewObj * cell = cellData->getViewOwner();
	//ViewObj* panlObj = cell->getParentViewById(ConstantsID::ContainerId::PAGE_PANEL);
	//panlObj->setFocus(true,false);
	OperateMgr *operateMgr = mExcelTemplete->getOperateMgr();
	operateMgr->createNewAction();
	//设置焦点对象操作回退
	ViewObj * preFocus = NULL;
	char preDir = 0;
	operateMgr->getFocusAndDirection(preFocus, preDir);
	ViewObj * setAfterFocus = NULL;
	char setAfterDir = BaseConstants::Direction::LEFT;

	if (isRow)
	{
		ExcelCellData * tempCellData;
		Line * topLine = cellData->mTop;
		int top = topLine->getPosition();
		int indexOfstart = getIndexOfLine(topLine, true);
		Line * bottomLine = mRowLines->at(indexOfstart+1);
		int bottom = bottomLine->getPosition();
		//处理焦点
		if (mCellsTempt1->size()){
			mCellsTempt1->clear();
		}
		if (mCellsTempt4->size()){
			mCellsTempt4->clear();
		}
		bool haveSetFocus = false;
		seekCellsHaveLine(BaseConstants::Direction::LEFT, cellData->mLeft, mCells, mCellsTempt1);
		seekCellsHaveLine(BaseConstants::Direction::RIGHT, cellData->mRight, mCellsTempt1, mCellsTempt4);
		if (mCellsTempt1->size()){
			mCellsTempt1->clear();
		}
		seekCellsHaveLine(BaseConstants::Direction::DOWN, topLine, mCellsTempt4, mCellsTempt1);
		if (mCellsTempt1->size() >= 1){
			ExcelCellData * cellData6 = mCellsTempt1->at(0);
			setCellDataFocus(cellData6, operateMgr, preFocus, preDir, setAfterDir);
			haveSetFocus = true;
		}
		else{
			seekCellsHaveLine(BaseConstants::Direction::UP, bottomLine, mCellsTempt4, mCellsTempt1);
			if (mCellsTempt1->size() >= 1){
				ExcelCellData * cellData6 = mCellsTempt1->at(0);
				setCellDataFocus(cellData6, operateMgr, preFocus, preDir, setAfterDir);
				haveSetFocus = true;
			}
		}
		//找出需要删除的单元格
		if (mCellsTempt1->size()){
			mCellsTempt1->clear();
		}
		if (mCellsTempt4->size()){
			mCellsTempt4->clear();
		}
		seekCellsHaveLine(BaseConstants::Direction::UP, topLine, mCells, mCellsTempt1);
		seekCellsHaveLine(BaseConstants::Direction::DOWN, bottomLine, mCellsTempt1, mCellsTempt4);
		int willDeleteSize = mCellsTempt4->size();
		/*for (int i = 0; i < willDeleteSize; i++){
			tempCellData = mCellsTempt4->at(i);
			addOrRomoveCellData(tempCellData, false);
		}*/
		if (mCells->size() == willDeleteSize){
			ViewObj * setAfterView = mExcelTemplete->getNextView();
			setAfterView->setFocus(false);
			//设置焦点对象操作回退
			/*SetFocusAction*  setFocusAction = new SetFocusAction(winData);
			setFocusAction->initAction(preFocus, preDir, setAfterView, setAfterDir);
			winData->addToLocalAction(setFocusAction);*/
			mExcelTemplete->deleteSelf(false);
			return;
		}
		//再次找删除后的焦点
		if (!haveSetFocus)
		{
			int sizeOfAll = mCells->size();
			ExcelCellData * tempCellData1;
			ExcelCellData * tempCellData2;
			for (int i = 0; i < sizeOfAll;i++)
			{
				tempCellData1 = mCells->at(i);
				for (int m = 0; m < willDeleteSize;m++)
				{
					tempCellData2 = mCellsTempt4->at(m);
					if (tempCellData1 != tempCellData2)
					{
						goto breakLoop;
					}
				}
			}
		breakLoop:
			setCellDataFocus(tempCellData1, operateMgr, preFocus, preDir, setAfterDir);
			haveSetFocus = true;
		}
	
		moveSomeLines(mRowLines, indexOfstart, mRowLines->size(), top - bottom);
		if (indexOfstart < mRowLines->size() - 1)
		{
			if (mCellsTempt1->size()){
				mCellsTempt1->clear();
			}
			seekCellsHaveLine(BaseConstants::Direction::UP, topLine, mCells, mCellsTempt1);
			int handleBottomSize = mCellsTempt1->size();
			ExcelCellData * tempCellData1 = NULL;
			for (int i = 0; i < handleBottomSize; i++)
			{
				tempCellData1 = mCellsTempt1->at(i);
				ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
				operateMgr->addToLocalAction(changeLineAction);
				changeLineAction->addChangeLine(tempCellData1, BaseConstants::Direction::UP, tempCellData1->mTop, bottomLine);
				tempCellData1->mTop = bottomLine;
				
			}
		}
		if (mCellsTempt1->size()){
			mCellsTempt1->clear();
		}
		seekCellsHaveLine(BaseConstants::Direction::DOWN, topLine, mCells, mCellsTempt1);
		int handleBottomSize = mCellsTempt1->size();
		tempCellData = NULL;
		for (int i = 0; i < handleBottomSize; i++)
		{
			tempCellData = mCellsTempt1->at(i);
			ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
			operateMgr->addToLocalAction(changeLineAction);
			changeLineAction->addChangeLine(tempCellData, BaseConstants::Direction::DOWN, tempCellData->mBottom, bottomLine);
			tempCellData->mBottom = bottomLine;
		}
		mRowLines->removeAll(topLine);
		OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
		NewOrDeleteLineAction * tempAction = new NewOrDeleteLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
		tempAction->addLine(topLine,false,true);
		operateMgr->addToLocalAction(tempAction);
		//removeUseLessColLine();
		//deleteRowOrColCellData(topLine, bottomLine, true);
		for (int i = 0; i < willDeleteSize; i++){
			tempCellData = mCellsTempt4->at(i);
			deleteCellData(tempCellData);
		}
	}
	else
	{
		ExcelCellData * tempCellData;
		Line * leftLine = cellData->mLeft;
		Line * rightLine = getLineNextOrLastLine(leftLine, false, false);
		//Line * rightLine = cellData->mRight;
		//deleteRowOrColCellData(leftLine, rightLine, false);
		//处理焦点
		if (mCellsTempt1->size()){
			mCellsTempt1->clear();
		}
		if (mCellsTempt4->size()){
			mCellsTempt4->clear();
		}
		bool haveSetFocus = false;
		seekCellsHaveLine(BaseConstants::Direction::UP, cellData->mTop, mCells, mCellsTempt1);
		seekCellsHaveLine(BaseConstants::Direction::DOWN, cellData->mBottom, mCellsTempt1, mCellsTempt4);
		if (mCellsTempt1->size()){
			mCellsTempt1->clear();
		}
		seekCellsHaveLine(BaseConstants::Direction::RIGHT, leftLine, mCellsTempt4, mCellsTempt1);
		if (mCellsTempt1->size() >= 1){
			ExcelCellData * cellData6 = mCellsTempt1->at(0);
			setCellDataFocus(cellData6, operateMgr, preFocus, preDir, setAfterDir);
			haveSetFocus = true;
		}
		else{
			seekCellsHaveLine(BaseConstants::Direction::LEFT, rightLine, mCellsTempt4, mCellsTempt1);
			if (mCellsTempt1->size() >= 1){
				ExcelCellData * cellData6 = mCellsTempt1->at(0);
				setCellDataFocus(cellData6, operateMgr, preFocus, preDir, setAfterDir);
				haveSetFocus = true;
			}
		}
		//
		if (mCellsTempt1->size())
		{
			mCellsTempt1->clear();
		}
		if (mCellsTempt4->size())
		{
			mCellsTempt4->clear();
		}
		seekCellsHaveLine(BaseConstants::Direction::LEFT, leftLine, mCells, mCellsTempt1);
		seekCellsHaveLine(BaseConstants::Direction::RIGHT, rightLine, mCellsTempt1, mCellsTempt4);
		int willDeleteSize = mCellsTempt4->size();
		/*for (int i = 0; i < willDeleteSize; i++){
			tempCellData = mCellsTempt4->at(i);
			addOrRomoveCellData(tempCellData, false);
		}*/
		if (mCells->size() == willDeleteSize){
			ViewObj * setAfterView =  mExcelTemplete->getNextView();
			setAfterView->setFocus(false);
			//设置焦点对象操作回退
			SetFocusAction*  setFocusAction = new SetFocusAction(operateMgr->getCurStepMgr());
			setFocusAction->initAction(preFocus, preDir, setAfterView, setAfterDir);
			operateMgr->addToLocalAction(setFocusAction);
			mExcelTemplete->deleteSelf(false);
			return;
		}
		//再次找删除后的焦点
		if (!haveSetFocus)
		{
			int sizeOfAll = mCells->size();
			ExcelCellData * tempCellData1;
			ExcelCellData * tempCellData2;
			for (int i = 0; i < sizeOfAll; i++)
			{
				tempCellData1 = mCells->at(i);
				for (int m = 0; m < willDeleteSize; m++)
				{
					tempCellData2 = mCellsTempt4->at(m);
					if (tempCellData1 != tempCellData2)
					{
						goto breakLoop1;
					}
				}
			}
		breakLoop1:
			setCellDataFocus(tempCellData1, operateMgr, preFocus, preDir, setAfterDir);
			haveSetFocus = true;
		}
		int left = leftLine->getPosition();
		int indexOfstart = getIndexOfLine(leftLine, false);
		int right = rightLine->getPosition();
		moveSomeLines(mColLines, indexOfstart, mColLines->size(), left - right);
		if (indexOfstart < mColLines->size() - 1)
		{
			if (mCellsTempt1->size()){
				mCellsTempt1->clear();
			}
			seekCellsHaveLine(BaseConstants::Direction::LEFT, leftLine, mCells, mCellsTempt1);
			int handleBottomSize = mCellsTempt1->size();
			ExcelCellData * tempCellData;
			for (int i = 0; i < handleBottomSize; i++)
			{
				tempCellData = mCellsTempt1->at(i);
				ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
				operateMgr->addToLocalAction(changeLineAction);
				changeLineAction->addChangeLine(tempCellData, BaseConstants::Direction::LEFT, tempCellData->mLeft, rightLine);
				tempCellData->mLeft = rightLine;
			}
		}
		if (mCellsTempt1->size()){
			mCellsTempt1->clear();
		}
		seekCellsHaveLine(BaseConstants::Direction::RIGHT, leftLine, mCells, mCellsTempt1);
		int handleBottomSize = mCellsTempt1->size();

		for (int i = 0; i < handleBottomSize; i++)
		{
			tempCellData = mCellsTempt1->at(i);
			ExcelCellChangeLineAction *changeLineAction = new ExcelCellChangeLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
			operateMgr->addToLocalAction(changeLineAction);
			changeLineAction->addChangeLine(tempCellData, BaseConstants::Direction::RIGHT, tempCellData->mRight, rightLine);
			tempCellData->mRight = rightLine;
		}
		mColLines->removeAll(leftLine);
		OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
		NewOrDeleteLineAction * tempAction = new NewOrDeleteLineAction(operateMgr->getCurStepMgr(), mExcelTemplete);
		tempAction->addLine(leftLine, false, false);
		operateMgr->addToLocalAction(tempAction);
		for (int i = 0; i < willDeleteSize; i++){
			tempCellData = mCellsTempt4->at(i);
			deleteCellData(tempCellData);
		}
	}
	//移除多余的线
	removeUselessLine();
	mExcelTemplete->requestLayout();
	ExcelRelayoutAction* relayoutAction = new ExcelRelayoutAction(operateMgr->getCurStepMgr(), mExcelTemplete);
	operateMgr->addToLocalActionAt(relayoutAction, 1);
}
void ExcelLayout::deleteRowOrColCellData(Line* first, Line* second, bool isRow)
{
	if (mCellsTempt1->size()){
		mCellsTempt1->clear();
	}
	if (mCellsTempt2->size()){
		mCellsTempt2->clear();
	}
	if (isRow)
	{
		seekCellsHaveLine(BaseConstants::Direction::UP, first, mCells, mCellsTempt1);
		seekCellsHaveLine(BaseConstants::Direction::DOWN, second, mCellsTempt1, mCellsTempt2);
	}
	else
	{
		seekCellsHaveLine(BaseConstants::Direction::LEFT, first, mCells, mCellsTempt1);
		seekCellsHaveLine(BaseConstants::Direction::RIGHT, second, mCellsTempt1, mCellsTempt2);
	}
	int size = mCellsTempt2->size();
	ExcelCellData * tempCellData;
	for (int i = 0; i < size; i++)
	{
		tempCellData = mCellsTempt2->at(i);
		deleteCellData(tempCellData);
	}

}
bool ExcelLayout::setCellProperty(ExcelCellData * cellData, QMap<QString, QString>& map, int currentWidth, int setWidth, OperateMgr  * operateMgr, bool isUndo)
{
	bool isSuccess = true;
	ViewObj * cellView = cellData->getViewOwner();
	if (!isUndo)
	{
		Line * rightLine = cellData->mRight;
		if (setWidth<defaultCellW)
		{
			MessageBox::showAboutMessageBox(QStringLiteral("设置参数错误"), QStringLiteral("单元格宽度太小！"));
			isSuccess = false;
			return isSuccess;
		}
		if (currentWidth != setWidth)
		{
			/*if (setWidth >= GenerateCellW)
			{
			int excelWidth = (setWidth - currentWidth) + getExcelWidth();
			ViewObj* row = cellView->getParentViewById(ConstantsID::ContainerId::NORMAL_ROW);
			BaseViewParam * viewParam = row->getViewParam();
			int rowWidth = viewParam->getSelfW();
			int indexOfstart = getIndexOfLine(rightLine, false);
			moveSomeLines(mColLines, indexOfstart, mColLines->size(), setWidth - currentWidth);
			mExcelTemplete->requestLayout();
			}*/
			if (setCellWidth(cellData, setWidth))
			{
				mExcelTemplete->requestLayout();
				ExcelRelayoutAction* relayoutAction = new ExcelRelayoutAction(operateMgr->getCurStepMgr(), mExcelTemplete);
				operateMgr->addToLocalActionAt(relayoutAction, 1);
			}
			else
			{
				MessageBox::showAboutMessageBox(QStringLiteral("设置参数错误"), QStringLiteral("单元格宽度太大！"));
				isSuccess = false;
				return isSuccess;
			}
		}
	}
	
	DrawRect::setExcelCellPropertyByMap(map, cellView);
	return isSuccess;
}
void ExcelLayout::showSetCellProperty(ExcelCellData * cellData)
{
	CellPropertyDlg w;
	QMap<QString, QString> map;
	Line * leftLine = cellData->mLeft;
	Line * rightLine = cellData->mRight;
	int width = rightLine->getPosition() - leftLine->getPosition();
	ViewObj * cellView = cellData->getViewOwner();


	map.insert(TempleteTypeEnumeration::TYPE, "1");
	map.insert("columnWidth", QString::number(width));
	DrawRect::initExcelCellPropertyMap(map, cellView);
	QMap<QString, QString> beforMap = map;
	w.InitData(map);
	w.show();
	if (w.exec() == QDialog::Accepted)
	{
		//获取回传的值,在map里面取值即可
		QString widthStr = map.value("columnWidth");
		int getWidth = widthStr.toInt();
		OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
		operateMgr->createNewAction();
		if (!setCellProperty(cellData, map, width, getWidth, mExcelTemplete->getOperateMgr(),false))
		{
			operateMgr->deleteAction();
			return;
		}
		SetExcelCellPropertyAction *setCellPropertyAction = new SetExcelCellPropertyAction(operateMgr->getCurStepMgr());
		setCellPropertyAction->initAction(this, cellData, beforMap, map, width, getWidth);
		
		operateMgr->addToLocalAction(setCellPropertyAction);
	}
}
void ExcelLayout::setExcelProperty(ExcelCellData * cellData, QMap<QString, QString>& map, bool isShowBorder, int borderThick, int insideWidth, QString preAlign, bool setIsShowBorder, int setBorderThick, int setInsideWidth, QString setAlign)
{
	bool needFresh = false;

	bool isNeedSetCellData = false;
	//if (setIsShowBorder != isShowBorder)
	{
		mExcelTemplete->mIsShowBorder = setIsShowBorder;
		isNeedSetCellData = true;
	}
	if (map.contains("canDrag"))
	{
		if (map["canDrag"] == "true")
		{
			mExcelTemplete->mCanDragExcel=true;
		}
		else
		{
			mExcelTemplete->mCanDragExcel = false;
		}
	}
	if (setBorderThick != borderThick){
		isNeedSetCellData = true;
	}
	if (isNeedSetCellData){
		int cellSize = mCells->size();
		ExcelCellData * tempCellData;
		for (int i = 0; i < cellSize; i++)
		{
			tempCellData = mCells->at(i);
			DrawRect::setExcelPropertyByMap(map, tempCellData->getViewOwner());
		}
		//needFresh = true;
	}

	if (setAlign != preAlign){
		BaseParagraph* paragraph = NULL;
		ParagraphData* paragaphData = NULL;
		char currenAlign;
		if (setAlign == "1"){
			currenAlign = BaseConstants::Direction::LEFT;
		}
		else if (setAlign == "2"){
			currenAlign = BaseConstants::Direction::MIDDLE;

		}
		else{
			currenAlign = BaseConstants::Direction::RIGHT;
		}
		int cellSize = mCells->size();
		ExcelCellData * tempCellData;
		for (int i = 0; i < cellSize; i++)
		{
			tempCellData = mCells->at(i);
			paragraph = ((TempParagraphMgrData*)tempCellData->mRootTemplete->getContentData())->getParagraphMgr()->getParagrapAt(0);
			paragaphData = (ParagraphData*)paragraph->getContentData();
			paragaphData->setHorizonAlignType(currenAlign,true);
		}
		//needFresh = true;
	}
	ViewObj* tempView;

	if (setInsideWidth != insideWidth){
		if (setInsideWidth >= 0 && setInsideWidth <= 20){
			int cellSize = mCells->size();
			ExcelCellData * tempCellData;
			
			for (int i = 0; i < cellSize; i++)
			{
				tempCellData = mCells->at(i);
				tempView = tempCellData->getViewOwner();
				//tempView->setPageRepaint();
				setExcelInsideProperty(tempView,setInsideWidth);
				//tempView->getLayout()->refreshChildrenPlace();
			}
			needFresh = true;
		}
	}
	if (needFresh)
	{
		mExcelTemplete->requestLayout();
	}
}
void ExcelLayout::setExcelInsideProperty(ViewObj* cellPage, int setInsideWidth)
{
	VerticalViewParam  * viewParam = NULL;
	viewParam = (VerticalViewParam *)cellPage->getViewParam();
	viewParam->setInsideLeft(setInsideWidth);
	viewParam->setInsideRight(setInsideWidth);
	QList<BaseContainer *> * layers = cellPage->getChildren();
	for (int i = 0; i < layers->size(); i++)
	{
		ViewObj * layer = (ViewObj *)layers->at(i);
		BaseViewParam * param = layer->getViewParam();
		if (param->getDefaultW() == BaseConstants::DefaultWHType::FILL_PARENT_INSIDE)
		{
			param->setX(setInsideWidth);
			param->refreshHByParent(0);
			param->refreshWByParent(0);
			//param->refreshWHByParent();
		}
	}
}
void ExcelLayout::showSetExcelProperty(ExcelCellData * cellData)
{
	TablePropertyDlg w;
	QMap<QString, QString> map;
	QMap<QString, QString> beforMap;
	ViewObj * cellView = cellData->getViewOwner();
	ObjDrawManager * drawManger = cellView->getDrawMgr();
	DrawRect *  draw = (DrawRect *)drawManger->getBottomDraw();
	
	char fillType = draw->getFillRectType();
	int borderThick = draw->getRoundLineThick();
	map.insert("frameValue", QString::number(borderThick));
	if (mExcelTemplete->mCanDragExcel)
	{
		map.insert("canDrag", "true");
	}
	else
	{
		map.insert("canDrag", "false");
	}

	bool  isShowBorder;
	//if (fillType == BaseConstants::NONE)
	if (mExcelTemplete->mIsShowBorder)
	{
		isShowBorder = true;
		map.insert("frame", "true");
	}
	else
	{
		isShowBorder = false;
		map.insert("frame", "false");
	}
	//	int borderThick = DrawRect::initExcelPropertyMap(map, cellView);
	//先找到段落，然后设置段落属性
	/*bool  isShowBorder = mExcelTemplete->getIsShowBorader();
	if (isShowBorder){
	map.insert("frame", "true");
	}
	else
	{
	map.insert("frame", "false");
	}*/
	BaseParagraph* paragraph = NULL;
	ParagraphData* paragaphData = NULL;
	ExcelCellData *  tempCellData = (ExcelCellData*)cellView->getContentData();
	paragraph = ((TempParagraphMgrData*)tempCellData->mRootTemplete->getContentData())->getParagraphMgr()->getParagrapAt(0);
	paragaphData = (ParagraphData*)paragraph->getContentData();
	char alignType = paragaphData->getHorizonAlignType();
	QString preAlign;
	if (alignType == BaseConstants::Direction::LEFT){
		preAlign = QString::number(1);


	}
	else if (alignType == BaseConstants::Direction::MIDDLE){
		preAlign = QString::number(2);

	}
	else{
		preAlign = QString::number(3);
	}
	map.insert("Align", preAlign);
	VerticalViewParam  * viewParam = (VerticalViewParam *)cellView->getViewParam();
	int insideWidth = viewParam->getInsideLeft();
	map.insert("borderWidth", QString::number(insideWidth));
	beforMap = map;
	w.InitData(map);
	if (w.exec() == QDialog::Accepted)
	{
		bool setIsShowBorder = isShowBorder;
		if (map.contains("frame"))
		{
			QString frameStr = map.value("frame");
			if (frameStr == "true")
			{
				setIsShowBorder = true;
			}
			else
			{
				setIsShowBorder = false;
			}
		}
		int setBorderThick;
		if (map.contains("frameValue"))
		{
			setBorderThick = map.value("frameValue").toInt();

		}
		QString setAlign = map.value("Align");
		int setBorderWidth = 0;
		if (map.contains("borderWidth"))
		{

			setBorderWidth = map.value("borderWidth").toInt();

		}
		setExcelProperty(cellData, map, isShowBorder, borderThick, insideWidth, preAlign, setIsShowBorder, setBorderThick, setBorderWidth, setAlign);
		OperateMgr * operateMgr = mExcelTemplete->getOperateMgr();
		operateMgr->createNewAction();
		SetExcelPropertyAction *setCellPropertyAction = new SetExcelPropertyAction(operateMgr->getCurStepMgr());
		setCellPropertyAction->initAction(this, cellData, beforMap, map, isShowBorder, borderThick, insideWidth, preAlign, setIsShowBorder, setBorderThick, setBorderWidth, setAlign);
		//operateMgr->createNewAction();
		operateMgr->addToLocalAction(setCellPropertyAction);
	}
}

bool ExcelLayout::isMyCell(ViewObj * cell)
{
	int cellDataSize = mCells->size();
	ExcelCellData* temptCellData;
	for (int i = 0; i < cellDataSize; i++)
	{
		temptCellData = mCells->at(i);
		if (temptCellData->getViewOwner() == cell)
		{
			return true;
		}
	}
	return false;
}
int ExcelLayout::getRowCellMinHieght(Line * line)
{
	int indexOfRowLine = mRowLines->indexOf(line);
	if (indexOfRowLine<0)
	{
		return indexOfRowLine;
	}
	QList<ExcelCellData *>* rowCellList = mCellRowList->at(indexOfRowLine-1)->getCellDataList();
	int rowMaxH = 0;
	int temptHieght = 0;
	int cellNumber = rowCellList->size();
	ExcelCellData * cell;
	for (int m = 0; m < cellNumber; m++)
	{
		cell = rowCellList->at(m);
		temptHieght = cell->mContentHeight + cell->mTop->getPosition();
		if (temptHieght>rowMaxH)
		{
			rowMaxH = temptHieght;
		}
	}
	if (rowMaxH<defaultCellH)
	{
		rowMaxH = defaultCellH;
	}
	return rowMaxH;
}
/*
*selectLineType  代表点击当前传入的cellData 的方向
*/
bool ExcelLayout::isAbleToSetX(char selectLineType, int setX, QList<ExcelCellData *>* cellDataList)
{
	if (!cellDataList || cellDataList->size() == 0)
	{
		return true;
	}
	bool canSet = true;
	ExcelCellData * tempCellData;
	ViewObj * cellPage;
	int sizeOfCellData = cellDataList->size();
	int tempWidth = 0;
	int widthBetweenLine;
	int canUseWidthBetweenLine;
	for (int i = 0; i < sizeOfCellData; i++)
	{
		tempCellData = cellDataList->at(i);
		tempWidth = tempCellData->getMaxWidthOfChildView();
		if (selectLineType == BaseConstants::Direction::LEFT)
		{
			widthBetweenLine = tempCellData->mRight->getPosition() - setX;
			
		}
		else if (selectLineType == BaseConstants::Direction::RIGHT)
		{
			widthBetweenLine = setX-tempCellData->mLeft->getPosition();
		}
		cellPage =  tempCellData->getViewOwner();
		VerticalViewParam * viewParam = (VerticalViewParam*)cellPage->getViewParam();
		ViewObj * txtLayer = (ViewObj *)cellPage->findChildrenByID(ConstantsID::ContainerId::EXCEL_TXT_LAYER, false);
		VerticalViewParam * layerViewParam = (VerticalViewParam *)txtLayer->getViewParam();
		ViewObj * row = (ViewObj *)txtLayer->findChildrenByID(ConstantsID::ContainerId::NORMAL_ROW, false);
		RowViewParam * rowParam = (RowViewParam *)row->getViewParam();
		canUseWidthBetweenLine = widthBetweenLine - (viewParam->getInsideLeft() + viewParam->getInsideRight() 
			+ layerViewParam->getInsideLeft() + layerViewParam->getInsideRight()
			+ rowParam->getInsideLeft() + rowParam->getInsideRight());
		if (canUseWidthBetweenLine <= tempWidth)
		{
			return false;
		}
	}
	return true;
}
int ExcelLayout::getXCanSet(char selectLineType, QList<ExcelCellData *>* cellDataList)
{
	if (!cellDataList || cellDataList->size() == 0)
	{
		return 0;
	}
	ExcelCellData * tempCellData;
	ViewObj * cellPage;
	int sizeOfCellData = cellDataList->size();
	int tempWidth = 0;
	int postionOfTempCell;
	int returnX;
	if (selectLineType == BaseConstants::Direction::LEFT)
	{
		returnX = INT_MAX;
	}
	else
	{
		returnX = -1;
	}
	for (int i = 0; i < sizeOfCellData; i++)
	{
		tempCellData = cellDataList->at(i);
		tempWidth = tempCellData->getMaxWidthOfChildView();
		if (tempWidth<defaultCellW)
		{
			tempWidth = defaultCellW;
		}
		if (selectLineType == BaseConstants::Direction::LEFT)
		{
			postionOfTempCell = tempCellData->mRight->getPosition() - tempWidth;
			if (postionOfTempCell<returnX)
			{
				returnX = postionOfTempCell;
			}
		}
		else if (selectLineType == BaseConstants::Direction::RIGHT)
		{
			postionOfTempCell = tempWidth + tempCellData->mLeft->getPosition();
			if (postionOfTempCell>returnX)
			{
				returnX = postionOfTempCell;
			}
		}
	}
	return returnX;
}
int ExcelLayout::getExcelWidth()
{
	int left = mColLines->at(0)->getPosition();
	int right = mColLines->at(mColLines->size()-1)->getPosition();
	return right - left;
}
void ExcelLayout::setCellDataFocus(ExcelCellData * cellData, OperateMgr  * operateMgr, ViewObj*preFocus, char preDir, char setAfterDir)
{
	//ViewObj * cell = cellData->getViewOwner();
	//((ViewObj *)cell->findContainerById(ConstantsID::ContainerId::NORMAL_ROW))->setFocus(false, false);
	//ViewObj * setAfterView = ((BaseParagraph*)cell->getTempParent()->getChild(0))->getFirstChildView();
	ViewObj* setAfterView = cellData->mRootTemplete->getFirstChildView();
	setAfterView->setFocus(false);
	//设置焦点对象操作回退
	SetFocusAction*  setFocusAction = new SetFocusAction(operateMgr->getCurStepMgr());
	setFocusAction->initAction(preFocus, preDir, setAfterView, setAfterDir);
	operateMgr->addToLocalAction(setFocusAction);
}
void ExcelLayout::clearAllCellTxtLayerForWinData(WriterManager * wMgr)
{
	if (wMgr)
	{
		/*BaseWinData * windata = (BaseWinData *)wMgr->getRootView()->getChild(ConstantsID::IndexId::TXT_WINDOW)->getContentData();
		int size = mCells->size();
		ExcelCellData * cellData;
		ViewObj * cell;
		for (int i = 0; i < size; i++)
		{
			cellData = mCells->at(i);
			cell = cellData->getViewOwner();
			ViewObj * txtLayer = (ViewObj *)cell->getChildren()->at(ConstantsID::IndexId::LAYER_TXT);
			windata->RemoveFromReplaceExcelList(txtLayer);
		}*/
	}
}
bool ExcelLayout::addChild(BaseContainer * child, int idx, bool refreshIdx)
{
	return BaseChildrenManager::addChild(child,idx,refreshIdx);
}
void ExcelLayout::addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx)
{
	BaseChildrenManager::addChildren(children, start, num,idx,refreshIdx);
}
int ExcelLayout::delChild(int idx, bool refreshIdx)
{
	return BaseChildrenManager::delChild( idx, refreshIdx);
}
int ExcelLayout::delChild(BaseContainer * child, bool refreshIdx)
{
	return BaseChildrenManager::delChild(child, refreshIdx);
}
int ExcelLayout::delChildren(int start, int num, bool refreshIdx)
{
	return BaseChildrenManager::delChildren(start, num, refreshIdx);
}
int ExcelLayout::getWholeRowCrossPageH(int curH)
{
	/*Line * nearestRow = findNearestLine(mRowLines, curH, -1);
	if (nearestRow->getPosition()>curH)
	{
		nearestRow = mRowLines->at(mRowLines->indexOf(nearestRow) - 1);
	}
	*/
	if (mCellsTempt5->size())
	{
		mCellsTempt5->clear();
	}
	//seekCellsHaveLine(BaseConstants::Direction::UP, nearestRow, mCells, mCellsTempt5);
	int minx = mColLines->at(0)->getPosition();
	int miny = curH-2;
	int maxx = mColLines->at(mColLines->size()-1)->getPosition();
	int maxy = curH;
	int cellDataSize = mCells->size();
	ExcelCellData* temptCellData;
	for (int i = 0; i < cellDataSize; i++)
	{
		temptCellData = mCells->at(i);

		if (BaseMath::isRectCross(minx, miny, maxx, maxy, temptCellData->mLeft->getPosition(), temptCellData->mTop->getPosition()
			, temptCellData->mRight->getPosition(), temptCellData->mBottom->getPosition()))
		{
			mCellsTempt5->push_back(temptCellData);
		}
	}
	if (mCellsTempt5->size()==0)
	{
		return -1;
	}
	ExcelCellData * excelDataOfMiniTop = mCellsTempt5->at(0);
	ExcelCellData * excelDataTempt;
	for (int i = 1; i < mCellsTempt5->size();i++)
	{
		excelDataTempt = mCellsTempt5->at(i);
		if (excelDataTempt->mTop->getPosition()<excelDataOfMiniTop->mTop->getPosition())
		{
			excelDataOfMiniTop = excelDataTempt;
		}
	}
	int  indexOfCellDataTop = mRowLines->indexOf(excelDataOfMiniTop->mTop);
		ExcelCellDataRow* temptCellDataRow = getCellDataList(indexOfCellDataTop);
		if (temptCellDataRow->getCellDataList()->size() == 0 || (!temptCellDataRow->isThrough) || indexOfCellDataTop == 0)
		{
			int TemptCurH = curH;
			int miniCurH = TemptCurH;
			for (int i = 1; i < mCellsTempt5->size(); i++)
			{
				excelDataTempt = mCellsTempt5->at(i);
				TemptCurH = excelDataTempt->getIntegrityRowByMaxH(curH);
				if (TemptCurH<miniCurH)
				{
					miniCurH = TemptCurH;
				}
			}
			return miniCurH;
		}
		else
		{
			return temptCellDataRow->getCellDataList()->at(0)->mTop->getPosition();
		}
	
	
	//return excelData->getIntegrityRowByMaxH(curH);
}
void ExcelLayout::setWriteManager(WriterManager* writeManger, bool isCircle)
{
	if (mCells)
	{
		int size = mCells->size();
		ExcelCellData * tempCellData;
		for (int i = 0; i < size;i++)
		{
			tempCellData = mCells->at(i);
			tempCellData->mRootTemplete->setWriteManager(writeManger,isCircle);
		}
	}
}