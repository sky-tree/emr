#ifndef _LINEMOVEACTION_H_
#define _LINEMOVEACTION_H_
#include "BaseAction.h"
#include "qlist.h"
class Line;
class ExcelTemplete;
class LineMoveAction :
	public BaseAction
{
public:
	LineMoveAction(StepActionManger * stepActonManger, ExcelTemplete* excelTemplete);
	~LineMoveAction();
	void addMoveLine(Line * line, int preposition, int preDragposition, int setPosition, int setDragPosition);
	void setRelayout(bool relayout);
	void unDo();
	void reDo();
private:
	QList<Line*> * mMoveLineList;
	QList<int> * mPrePositionList;
	QList<int> * mPreDragPositionList;
	QList<int>* mSetPositionList;
	QList<int>* mSetDragPositionList;
	bool mNeedRelayout;
	ExcelTemplete* mExcelTemplete;
};
#endif