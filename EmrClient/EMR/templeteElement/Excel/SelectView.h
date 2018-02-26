#ifndef _SELECTVIEW_H_
#define _SELECTVIEW_H_
#include "BaseElement.h"
#include "ViewObj.h"
#include "SelectView.h"
#include "Line.h"
class ExcelCellData;
class SelectView :BaseElement
{
	
public:
	enum Side{ Right, Left, Top, Bottom, Middle };
	SelectView();
	~SelectView();
	void init();
	void SetSelect(ViewObj * selectObj,Side side);
	Line* getSelectLine();
	ViewObj * mSelectObj;
	ExcelCellData * mSelectCellData;
	Line * mSelectLine;
	Side mSideType;
};
#endif

