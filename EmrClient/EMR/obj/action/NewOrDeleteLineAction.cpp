#include "NewOrDeleteLineAction.h"
#include "ConstantsID.h"
#include "iostream"
#include "Line.h"
#include "ExcelTemplete.h"
#include "ExcelLayout.h"
NewOrDeleteLineAction::NewOrDeleteLineAction(StepActionManger * stepActonManger, ExcelTemplete* excelTemplete) : BaseAction(stepActonManger, ConstantsID::ClassId::NEW_OR_DELETE_LINE_ACTION)
{
	mExcelTemplete = excelTemplete;
	mExcelLayout = mExcelTemplete->mExcelLayout;
	mLineList = new QList<Line*>();
	mSignOfNewOrDelList = new QList<bool>();
	mSignOfRowOrColLineList = new QList<bool>();
	mNeedRelayout = false;
	isUndo = false;
}


NewOrDeleteLineAction::~NewOrDeleteLineAction()
{
	if (mExcelTemplete)
	{
		mExcelTemplete = NULL;
	}
	if (mExcelLayout){
		mExcelLayout = NULL;
	}
	Line * tempLine;
	bool newOrDelSign;// true  表示 new ，false 表示 delete
	if (mLineList){
		int size = mLineList->size();
	
		for (int i = 0; i < size;i++)
		{
			newOrDelSign = mSignOfNewOrDelList->at(i);
			if (newOrDelSign)
			{
				if (isUndo)
				{
					tempLine = mLineList->at(i);
					delete tempLine;
				}
			}
			else
			{
				if (!isUndo)
				{
					tempLine = mLineList->at(i);
					delete tempLine;
				}
			}
		}
		mLineList->clear();
		delete mLineList;
		mLineList = NULL;
	}
	if (mSignOfNewOrDelList){
		mSignOfNewOrDelList->clear();
		delete mSignOfNewOrDelList;
		mSignOfNewOrDelList = NULL;
	}
	if (mSignOfRowOrColLineList){
		mSignOfRowOrColLineList->clear();
		delete mSignOfRowOrColLineList;
		mSignOfRowOrColLineList = NULL;
	}
}
void NewOrDeleteLineAction::addLine(Line * line, bool isNewLine, bool isRow)
{
	mLineList->push_back(line);
	mSignOfNewOrDelList->push_back(isNewLine);
	mSignOfRowOrColLineList->push_back(isRow);
}
void NewOrDeleteLineAction::setRelayout(bool relayout)
{
	mNeedRelayout = relayout;
}
void NewOrDeleteLineAction::unDo()
{
	isUndo = true;
	Line * tempLine;
	bool newOrDelSign;// true  表示 new ，false 表示 delete
	bool isRow;
	int size = mLineList->size();
	QList<Line *> * tempLines;
	for (int i = 0; i < size; i++)
	{
		newOrDelSign = mSignOfNewOrDelList->at(i);
		if (newOrDelSign)
		{
			if (isUndo)
			{
				tempLine = mLineList->at(i);
				isRow = mSignOfRowOrColLineList->at(i);
				if (isRow)
				{
					mExcelLayout->mRowLines->removeAll(tempLine);
				}
				else
				{
					mExcelLayout->mColLines->removeAll(tempLine);
				}
			}
		}
		else
		{
			if (isUndo)
			{
				tempLine = mLineList->at(i);
				isRow = mSignOfRowOrColLineList->at(i);
				if (isRow)
				{
					tempLines = mExcelLayout->mRowLines;
					
				}
				else
				{
					tempLines = mExcelLayout->mColLines;
				}
				tempLines->push_back(tempLine);
				//排序
				int laseIndex = tempLines->size() - 1;
				Line* lineCurrent = tempLines->at(laseIndex);
				Line* lineCompare = NULL;
				for (int m = laseIndex - 1; m >= 0; m--){
					lineCompare = tempLines->at(m);
					if (lineCurrent->getDragPosition()<lineCompare->getDragPosition()){
						tempLines->swap(m, m + 1);
					}
					else{
						break;
					}
				}
			}
		}
	}
	if (mNeedRelayout)
	{
		mExcelTemplete->requestLayout();
	}
}
void NewOrDeleteLineAction::reDo()
{
	isUndo = false;
	Line * tempLine;
	bool newOrDelSign;// true  表示 new ，false 表示 delete
	bool isRow;
	int size = mLineList->size();
	QList<Line *> * tempLines;
	for (int i = 0; i < size; i++)
	{
		newOrDelSign = mSignOfNewOrDelList->at(i);
		if (newOrDelSign)
		{
			if (!isUndo)
			{
				tempLine = mLineList->at(i);
				isRow = mSignOfRowOrColLineList->at(i);
				if (isRow)
				{
					tempLines = mExcelLayout->mRowLines;

				}
				else
				{
					tempLines = mExcelLayout->mColLines;
				}
				tempLines->push_back(tempLine);
				//排序
				int laseIndex = tempLines->size() - 1;
				Line* lineCurrent = tempLines->at(laseIndex);
				Line* lineCompare = NULL;
				for (int m = laseIndex - 1; m >= 0; m--){
					lineCompare = tempLines->at(m);
					if (lineCurrent->getPosition()<lineCompare->getPosition()){
						tempLines->swap(m, m + 1);
					}
					else{
						break;
					}
				}
			}
		}
		else
		{
			if (!isUndo)
			{
				tempLine = mLineList->at(i);
				isRow = mSignOfRowOrColLineList->at(i);
				if (isRow)
				{
					mExcelLayout->mRowLines->removeAll(tempLine);
				}
				else
				{
					mExcelLayout->mColLines->removeAll(tempLine);
				}
			}
		}
	}
	if (mNeedRelayout)
	{
		mExcelTemplete->requestLayout();
	}
}