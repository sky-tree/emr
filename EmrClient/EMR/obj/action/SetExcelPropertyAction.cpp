#include "SetExcelPropertyAction.h"
#include "ConstantsID.h"
#include "iostream"
#include "ExcelLayout.h"
#include "ExcelCellData.h"
SetExcelPropertyAction::SetExcelPropertyAction(StepActionManger * stepActonManger) : BaseAction(stepActonManger, ConstantsID::ClassId::SET_EXCEL_PROPERTY_ACTION)
, mCellData(NULL)
{

}


SetExcelPropertyAction::~SetExcelPropertyAction()
{
}
void SetExcelPropertyAction::initAction(ExcelLayout * excelLayout, ExcelCellData * cellData, QMap<QString, QString> preMap, QMap<QString, QString> setMap, bool preIsShowBorder, int preBorderThick, int preInsideWidth, QString preAlign, bool setIsShowBorder, int setBorderThick, int setInsideWidth, QString setAlign)
{
	//mIsSetCell = isSetCell;
	mExcelLayout = excelLayout;
	mCellData = cellData;
	mPreMap = preMap;
	mSetMap = setMap;
	mPreIsShowBorder = preIsShowBorder;
	mPreborderThick = preBorderThick;
	mPreInsideWidth = preInsideWidth;
	mPreAlign = preAlign;
	mSetIsShowBorder = setIsShowBorder;
	mSetBorderThick = setBorderThick;
	mSetInsideWidth = setInsideWidth;
	mSetAlign = setAlign;
}
void SetExcelPropertyAction::unDo()
{
	//if (mIsSetCell)
	mExcelLayout->setExcelProperty(mCellData, mPreMap, mSetIsShowBorder, mSetBorderThick, mSetInsideWidth, mSetAlign, mPreIsShowBorder, mPreborderThick, mPreInsideWidth, mPreAlign);

	
}
void SetExcelPropertyAction::reDo()
{
	//if (mIsSetCell)
	mExcelLayout->setExcelProperty(mCellData, mSetMap, mPreIsShowBorder, mPreborderThick, mPreInsideWidth, mPreAlign, mSetIsShowBorder, mSetBorderThick, mSetInsideWidth,mSetAlign);
}
