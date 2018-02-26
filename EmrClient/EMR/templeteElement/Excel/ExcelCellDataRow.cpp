#include "ExcelCellDataRow.h"


ExcelCellDataRow::ExcelCellDataRow():
mCellsList(NULL)
{
	isThrough = false;
	mCellsList = new QList<ExcelCellData *>();
}


ExcelCellDataRow::~ExcelCellDataRow()
{
	if (mCellsList){
		delete mCellsList;
		mCellsList = NULL;
	}
}
void ExcelCellDataRow::refresh(){
	mCellsList->clear();
	isThrough = false;
}
void ExcelCellDataRow::addCellDataToList(ExcelCellData * celldata){
	mCellsList->push_back(celldata);
}
QList<ExcelCellData *>* ExcelCellDataRow::getCellDataList(){
	return mCellsList;
}


