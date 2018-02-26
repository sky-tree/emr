#include "ExcelTempleteParamData.h"
#include "ContentDataFactory.h"
#include "ConstantsID.h"
#include "BaseTemplate.h"
#include "ExcelTemplete.h"
#include "TempleteFactory.h"
#include "BaseIOTools.h"
#include "ExcelLayout.h"
#include "Line.h"
//#include "ContainerDataFactory.h"
ExcelTempleteParamData::ExcelTempleteParamData(BaseTemplate * templeteOwner, CodeParams * codeParams) : CodeTempData(templeteOwner, ConstantsID::ClassId::EXCEL_DATA_PARAM
	, codeParams)
, mTempleteOwner((ExcelTemplete*)templeteOwner)
{
	setNeedLoadSelfChildren(false);
}


ExcelTempleteParamData::~ExcelTempleteParamData()
{
	if (mTempleteOwner)
	{
		mTempleteOwner = NULL;
	}
	if (mRowLines)
	{
		mRowLines = NULL;
	}
	if (mColLines)
	{
		mColLines = NULL;
	}
	if (mCells)
	{
		mCells = NULL;
	}
}
void ExcelTempleteParamData::setExcelTempleteOwner(ExcelTemplete * templeteOwner)
{
	mTempleteOwner = templeteOwner;
	mOwner = templeteOwner;
}
void ExcelTempleteParamData::saveSelfData(BaseIOTools * ioTools)
{
	//return;
	mOwnerObjId = mTempleteOwner->getObjID();
	if (mOwnerObjId == ConstantsID::ContainerId::EXCEL_TEMPLETE)
	{
		ioTools->writeBool(mTempleteOwner->mIsShowBorder);
		ExcelLayout *  excelLayout = mTempleteOwner->mExcelLayout;
		//横线
		QList<Line *> * rowLines=excelLayout->mRowLines;
		int rowSize = rowLines->size();
		ioTools->writeInt(rowSize);
		Line* tempLine;
		for (int i = 0; i < rowSize; i++)
		{
			tempLine = rowLines->at(i);
			ioTools->writeInt(tempLine->getPosition());
			ioTools->writeInt(tempLine->getDragPosition());
		}
		//竖线
		QList<Line *> * colLines = excelLayout->mColLines;
		int colSize = colLines->size();
		ioTools->writeInt(colSize);
		 tempLine = NULL;
		for (int i = 0; i < colSize; i++)
		{
			tempLine = colLines->at(i);
			ioTools->writeInt(tempLine->getPosition());
			ioTools->writeInt(tempLine->getDragPosition());
		}

		//单元格位置参数
		QList<ExcelCellData *>* cellDataList = excelLayout->mCells;
		int cellSize = cellDataList->size();
		ioTools->writeInt(cellSize);
		ExcelCellData* cellData;
		int colIndex;
		int rowIndex;
		for (int i = 0; i < cellSize; i++)
		{
			cellData = cellDataList->at(i);
			colIndex = colLines->indexOf(cellData->mLeft);
			ioTools->writeInt(colIndex);
			colIndex = colLines->indexOf(cellData->mRight);
			ioTools->writeInt(colIndex);
			rowIndex = rowLines->indexOf(cellData->mTop);
			ioTools->writeInt(rowIndex);
			rowIndex = rowLines->indexOf(cellData->mBottom);
			ioTools->writeInt(rowIndex);
			cellData->saveSelfData( ioTools);
		}
	}

}

void ExcelTempleteParamData::loadSelfData(BaseIOTools * ioTools)
{
	//return;
	//int i = 200;
	if (mOwnerObjId == ConstantsID::ContainerId::EXCEL_TEMPLETE)
	{
		mIsShowBorder = ioTools->readBool();
		mRowLines = new QList<Line *>();
		int rowSize = ioTools->readInt();
		Line* tempLine;
		for (int i = 0; i < rowSize; i++)
		{
			tempLine = new Line(ioTools->readInt());
			tempLine->setDragPosition(ioTools->readInt());
			mRowLines->push_back(tempLine);
		}
		//ExcelLayout::sortLines(mRowLines);
		//竖线
		mColLines = new QList<Line *>();
		int colSize = ioTools->readInt();
		//Line* tempLine;
		for (int i = 0; i < colSize; i++)
		{
			tempLine = new Line(ioTools->readInt());
			tempLine->setDragPosition(ioTools->readInt());
			mColLines->push_back(tempLine);
		}
		ExcelLayout::sortLines(mColLines);
	}

}
BaseContainer * ExcelTempleteParamData::createOwner(BaseContainer * tempParent1, BaseIOTools *iOTools, bool isAddToParenTemplate)
{
	BaseTemplate* tempParent = (BaseTemplate*)tempParent1;
	ExcelTemplete * excelTemplete;
	if (mOwnerObjId == ConstantsID::ContainerId::EXCEL_TEMPLETE)
	{

		excelTemplete = TempleteFactory::createExcelTemplete(tempParent,true);
		excelTemplete->initContentData(this);
		setExcelTempleteOwner(excelTemplete);
		//choiceWidget->initWidget();
		
		
		mCells = new QList<ExcelCellData *>();
		
		int cellSize = iOTools->readInt();
		ExcelCellData* cellData;
		int leftLineIndex;
		int rightLineIndex;
		int topLineIndex;
		int bottomLineIndex;
		bool isNeedDelete = false;
		for (int i = 0; i < cellSize; i++)
		{
			leftLineIndex = iOTools->readInt();
			rightLineIndex = iOTools->readInt();
			topLineIndex = iOTools->readInt();
			bottomLineIndex = iOTools->readInt();
			if (leftLineIndex >= rightLineIndex || topLineIndex >= bottomLineIndex)
			{
				isNeedDelete = true;
			}
			else
			{
				isNeedDelete = false;
			}
			if (i == 5)
			{
				int a = 0;
			}
			//ContentDataFactory::loadData(excelTemplete->getWriterMgr(), iOTools, excelTemplete);
			cellData = new ExcelCellData(NULL,NULL,NULL);
			cellData->initLines(mColLines->at(leftLineIndex), mRowLines->at(topLineIndex), mColLines->at(rightLineIndex), mRowLines->at(bottomLineIndex));
			cellData->loadSelfData(iOTools);
			cellData->createOwner(excelTemplete, iOTools, isNeedDelete);
			if (isNeedDelete)
			{
				delete cellData;
				cellData = NULL;
			}
			else
			{
				mCells->push_back(cellData);
			}
		}
		excelTemplete->getChildrenMgr()->delChildren(0,-1,true);
		/*ExcelLayout *  excelLayout = mTempleteOwner->mExcelLayout;
		excelLayout->mCells = mCells;
		excelLayout->mColLines = mColLines;
		excelLayout->mRowLines = mRowLines;*/
		excelTemplete->setInitOver();
		excelTemplete->initExcelTemplete(mIsShowBorder,true);
		if (isAddToParenTemplate)
		{
			tempParent->getChildrenMgr()->pushChild(excelTemplete, true);
		}
		/*tempParent->addTempleteChild(mOwner, -1, false, false);
		if (mChildrenDataList)
		{
			for (int i = 0; i < mChildrenDataList->size(); i++)
			{
				BaseContainerData * data = mChildrenDataList->at(i);
				BaseContainer * obj = data->createOwner((BaseTemplete *)mOwner, viewParent);
				((BaseTemplete *)mOwner)->addTempleteChild(obj, -1, false, false);
			}
		}*/
		return mOwner;
	}
	return NULL;
}