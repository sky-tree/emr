#ifndef _EXCELCELLCHANGELINEACTION_H_
#define _EXCELCELLCHANGELINEACTION_H_
#include "BaseAction.h"
#include "qlist.h"
class ExcelTemplete;
class ExcelCellData;
class Line;
class ExcelCellChangeLineAction :
	public BaseAction
{
public:
	ExcelCellChangeLineAction(StepActionManger * stepActonManger, ExcelTemplete* excelTemplete);
	~ExcelCellChangeLineAction();
	void addChangeLine(ExcelCellData * cellData, char position, Line* preLine,Line* setLine);
	void setRelayout(bool relayout);
	void unDo();
	void reDo();
	QList<ExcelCellData*> * mChangeCellDataList;
	QList<char> * mPositionList;
	QList<Line*> * mPreLineList;
	QList<Line*>* mSetLineList;
	bool mNeedRelayout;
	ExcelTemplete* mExcelTemplete;
};
#endif