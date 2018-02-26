#ifndef _NEWORDELETECELLDATAACTION_H_
#define _NEWORDELETECELLDATAACTION_H_
#include "BaseAction.h"
#include "qlist.h"
class ExcelTemplete;
class ExcelCellData;
class ExcelLayout;
class NewOrDeleteCellDataAction :
	public BaseAction
{
public:
	NewOrDeleteCellDataAction(StepActionManger * stepActonManger, ExcelTemplete* excelTemplete);
	~NewOrDeleteCellDataAction();
	void addCellData(ExcelCellData * cellData, bool isNewCellData);
	void unDo();
	void reDo();
private: 
	QList<ExcelCellData*> * mCellDataList;
	QList<bool> * mSignOfNewOrDelList; // true  表示 new ，false 表示 delete
	ExcelTemplete* mExcelTemplete;
	ExcelLayout* mExcelLayout;
	bool isUndo;
};
#endif