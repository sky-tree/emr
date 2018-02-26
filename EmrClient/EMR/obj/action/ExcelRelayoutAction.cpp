#include "ExcelRelayoutAction.h"
#include "ConstantsID.h"
#include "iostream"
#include "ExcelTemplete.h"
ExcelRelayoutAction::ExcelRelayoutAction(StepActionManger * stepActonManger, ExcelTemplete* excelTemplete) : BaseAction(stepActonManger, ConstantsID::ClassId::EXCEL_RELAYOUT_ACTION)
{
	mExcelTemplete = excelTemplete;
}


ExcelRelayoutAction::~ExcelRelayoutAction()
{
	if (mExcelTemplete){
		mExcelTemplete = NULL;
	}
}
void ExcelRelayoutAction::unDo()
{
	mExcelTemplete->requestLayout();
}
void ExcelRelayoutAction::reDo()
{
	mExcelTemplete->requestLayout();
}
