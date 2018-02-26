#include "SetExcelCellPropertyAction.h"
#include "ConstantsID.h"
#include "iostream"
#include "ExcelLayout.h"
#include "ExcelCellData.h"
SetExcelCellPropertyAction::SetExcelCellPropertyAction(StepActionManger * stepActonManger) : BaseAction(stepActonManger, ConstantsID::ClassId::SET_EXCEL_CELL_PROPERTY_ACTION)
, mCellData(NULL)
{

}


SetExcelCellPropertyAction::~SetExcelCellPropertyAction()
{
}
void SetExcelCellPropertyAction::initAction( ExcelLayout * excelLayout, ExcelCellData * cellData, QMap<QString, QString> preMap, QMap<QString, QString> setMap, int preWidth, int setWidth)
{
	mExcelLayout = excelLayout;
	mCellData = cellData;
	mPreMap = preMap;
	mSetMap = setMap;
	mPreWidth = preWidth;
	mSetWidth = setWidth;
}
void SetExcelCellPropertyAction::unDo()
{
	//if (mIsSetCell)
	//{
		mExcelLayout->setCellProperty(mCellData, mPreMap, mSetWidth, mPreWidth,NULL);
	//}
	
}
void SetExcelCellPropertyAction::reDo()
{
	//if (mIsSetCell)
	//{
		mExcelLayout->setCellProperty(mCellData, mSetMap, mPreWidth, mSetWidth,NULL);
	//}
}
