#include "ExcelCellChangeLineAction.h"
#include "ConstantsID.h"
#include "iostream"
#include "ExcelTemplete.h"
#include "Line.h"
#include "ExcelCellData.h"
#include "ExcelLayout.h"
ExcelCellChangeLineAction::ExcelCellChangeLineAction(StepActionManger * stepActonManger, ExcelTemplete* excelTemplete) : BaseAction(stepActonManger, ConstantsID::ClassId::EXCEL_CELL_CHANGE_LINE_ACTION)
{
	mExcelTemplete = excelTemplete;
	mChangeCellDataList = new QList<ExcelCellData *>();
	mPositionList = new QList<char>();
	mPreLineList = new QList<Line*>();
	mSetLineList = new QList<Line*>();
	mNeedRelayout = false;
}


ExcelCellChangeLineAction::~ExcelCellChangeLineAction()
{
	if (mChangeCellDataList){
		mChangeCellDataList->clear();
		delete	mChangeCellDataList;
		mChangeCellDataList = NULL;
	}
	if (mPositionList){
		mPositionList->clear();
		delete	mPositionList;
		mPositionList = NULL;
	}
	if (mPreLineList){
		mPreLineList->clear();
		delete	mPreLineList;
		mPreLineList = NULL;
	}
	if (mSetLineList){
		mSetLineList->clear();
		delete	mSetLineList;
		mSetLineList = NULL;
	}
	if (mExcelTemplete){
		mExcelTemplete = NULL;
	}
}
void ExcelCellChangeLineAction::addChangeLine(ExcelCellData * cellData, char position, Line* preLine, Line* setLine)
{
	mChangeCellDataList->push_back(cellData);
	mPositionList->push_back(position);
	mPreLineList->push_back(preLine);
	mSetLineList->push_back(setLine);
}
void ExcelCellChangeLineAction::setRelayout(bool relayout)
{
	mNeedRelayout = relayout;
}
void ExcelCellChangeLineAction::unDo()
{
	ExcelCellData * temporary;
	char placeType;
	Line * line;
	int size = mChangeCellDataList->size();
	for (int i = size - 1; i >= 0; i--)
	{
		temporary = mChangeCellDataList->at(i);
		placeType = mPositionList->at(i);
		line = mPreLineList->at(i);
		if (placeType == BaseConstants::Direction::UP){
			temporary->mTop = line;

		}
		else if (placeType == BaseConstants::Direction::DOWN){
			temporary->mBottom = line;
		}
		else if (placeType == BaseConstants::Direction::LEFT){
			temporary->mLeft = line;
		}
		else if (placeType == BaseConstants::Direction::RIGHT){
			temporary->mRight = line;
		}
	}
	if (mNeedRelayout)
	{
		mExcelTemplete->requestLayout();
	}
}
void ExcelCellChangeLineAction::reDo()
{
	ExcelCellData * temporary;
	char placeType;
	Line * line;
	int size = mChangeCellDataList->size();
	for (int i = size - 1; i >= 0; i--)
	{
		temporary = mChangeCellDataList->at(i);
		placeType = mPositionList->at(i);
		line = mSetLineList->at(i);
		if (placeType == BaseConstants::Direction::UP){
			temporary->mTop = line;

		}
		else if (placeType == BaseConstants::Direction::DOWN){
			temporary->mBottom = line;
		}
		else if (placeType == BaseConstants::Direction::LEFT){
			temporary->mLeft = line;
		}
		else if (placeType == BaseConstants::Direction::RIGHT){
			temporary->mRight = line;
		}
	}
	if (mNeedRelayout)
	{
		mExcelTemplete->requestLayout();
	}
}
