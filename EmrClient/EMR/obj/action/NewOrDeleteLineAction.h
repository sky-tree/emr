#ifndef _NEWORDELETELINEACTION_H_
#define _NEWORDELETELINEACTION_H_
#include "BaseAction.h"
#include "qlist.h"
class ExcelTemplete;
class Line;
class ExcelLayout;
class NewOrDeleteLineAction :
	public BaseAction
{
public:
	NewOrDeleteLineAction(StepActionManger * stepActonManger, ExcelTemplete* excelTemplete);
	~NewOrDeleteLineAction();
	void addLine(Line * line, bool isNewLine,bool isRow);
	void setRelayout(bool relayout);
	void unDo();
	void reDo();
	QList<Line*> * mLineList;
	QList<bool> * mSignOfNewOrDelList; // true  表示 new ，false 表示 delete
	QList<bool> * mSignOfRowOrColLineList; // true  表示 row ，false 表示 col
	bool mNeedRelayout;
	ExcelTemplete* mExcelTemplete;
	ExcelLayout* mExcelLayout;
	bool isUndo;
};

#endif