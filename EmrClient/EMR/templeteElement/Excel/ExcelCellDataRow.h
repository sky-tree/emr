#ifndef _EXCELCELLDATAROW_H_
#define _EXCELCELLDATAROW_H_
#include "QList.h"
class ExcelCellData;
class ExcelCellDataRow
{
public:
	ExcelCellDataRow();
	~ExcelCellDataRow();
	void refresh();
	void addCellDataToList(ExcelCellData * celldata);
	QList<ExcelCellData *>* getCellDataList();
	QList<ExcelCellData *>* mCellsList;
	bool isThrough;
};
#endif

