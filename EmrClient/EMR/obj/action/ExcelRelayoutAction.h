#ifndef _EXCELRELAYOUTACTION_H_
#define _EXCELRELAYOUTACTION_H_
#include "BaseAction.h"
class ExcelTemplete;
class ExcelRelayoutAction :
	public BaseAction
{
public:
	ExcelRelayoutAction(StepActionManger * stepActonManger, ExcelTemplete* excelTemplete);
	~ExcelRelayoutAction();
	void unDo();
	void reDo();
	ExcelTemplete* mExcelTemplete;
};
#endif