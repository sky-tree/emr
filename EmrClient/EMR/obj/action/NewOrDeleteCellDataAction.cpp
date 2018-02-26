#include "NewOrDeleteCellDataAction.h"
#include "ConstantsID.h"
#include "iostream"
#include "ExcelTemplete.h"
#include "ExcelLayout.h"
#include "ExcelCellData.h"
#include "ExcelLayout.h"
NewOrDeleteCellDataAction::NewOrDeleteCellDataAction(StepActionManger * stepActonManger, ExcelTemplete* excelTemplete) : BaseAction(stepActonManger, ConstantsID::ClassId::NEW_OR_DELETE_CELLDATA_ACTION)
{
	mExcelTemplete = excelTemplete;
	mExcelLayout = mExcelTemplete->mExcelLayout;
	mCellDataList = new QList<ExcelCellData*>();
	mSignOfNewOrDelList = new QList<bool>();
	isUndo = false;
}


NewOrDeleteCellDataAction::~NewOrDeleteCellDataAction()
{
	if (mExcelTemplete)
	{
		mExcelTemplete = NULL;
	}
	if (mExcelLayout){
		mExcelLayout = NULL;
	}
	ExcelCellData * tempCellData;
	bool newOrDelSign;// true  表示 new ，false 表示 delete
	if (mCellDataList){
		int size = mCellDataList->size();

		for (int i = 0; i < size; i++)
		{
			newOrDelSign = mSignOfNewOrDelList->at(i);
			if (newOrDelSign)
			{
				if (isUndo)
				{
					tempCellData = mCellDataList->at(i);
					delete tempCellData;
				}
			}
			else
			{
				if (!isUndo)
				{
					tempCellData = mCellDataList->at(i);
					delete tempCellData;
				}
			}
		}
		mCellDataList->clear();
		delete mCellDataList;
		mCellDataList = NULL;
	}
	if (mSignOfNewOrDelList){
		mSignOfNewOrDelList->clear();
		delete mSignOfNewOrDelList;
		mSignOfNewOrDelList = NULL;
	}
}
void NewOrDeleteCellDataAction::addCellData(ExcelCellData * cellData, bool isNewCellData)
{
	mCellDataList->push_back(cellData);
	mSignOfNewOrDelList->push_back(isNewCellData);
}
void NewOrDeleteCellDataAction::unDo()
{
	isUndo = true;
	ExcelCellData * tempCellData;
	bool newOrDelSign;// true  表示 new ，false 表示 delete
	int size = mCellDataList->size();
	QList<Line *> * tempLines;
	for (int i = 0; i < size; i++)
	{
		newOrDelSign = mSignOfNewOrDelList->at(i);
		tempCellData = mCellDataList->at(i);
		if (newOrDelSign)
		{
				
				
				mExcelLayout->mCells->removeAll(tempCellData);
		}
		else
		{
			
			mExcelLayout->mCells->push_back(tempCellData);
		}
	}
}
void NewOrDeleteCellDataAction::reDo()
{
	isUndo = false;
	ExcelCellData * tempCellData;
	bool newOrDelSign;// true  表示 new ，false 表示 delete
	int size = mCellDataList->size();
	QList<Line *> * tempLines;
	for (int i = 0; i < size; i++)
	{
		newOrDelSign = mSignOfNewOrDelList->at(i);
		tempCellData = mCellDataList->at(i);
		if (newOrDelSign)
		{
			mExcelLayout->mCells->push_back(tempCellData);
		
		}
		else
		{
			mExcelLayout->mCells->removeAll(tempCellData);
		}
	}
}