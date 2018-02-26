#ifndef _EXCELVERTICALLAYOUTACTION_
#define _EXCELVERTICALLAYOUTACTION_
#include "BaseAction.h"
#include "qlist.h"
//class ExcelTemplete;
class ViewObj;
class ExcelCellData;
class ExcelVerticalLayoutAction :
	public BaseAction
{
public:
	ExcelVerticalLayoutAction(StepActionManger * stepActonManger);
	~ExcelVerticalLayoutAction();
	void InitAction( ViewObj* focus, char direction);
	void addChangeCells(QList<ExcelCellData *>* cellList, QList<int> unDoPositionList, QList<int> redoPositionList);
	void addChangeCell(ExcelCellData* cellData,int undPostion,int reduPosition);
	void unDo();
	void reDo();
	//ExcelTemplete* mExcelTemplete;
	QList<ExcelCellData *>* mChangeCells;
	QList<int> mUndoVerticalLayoutList;
	QList<int> mRedoVerticalLayoutList;
	ViewObj* mFoucus;
	char mDirection;
};
#endif