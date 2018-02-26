#ifndef _SETEXCELCELLPROPERTYACTION_H_
#define _SETEXCELCELLPROPERTYACTION_H_
#include "BaseAction.h"
#include "qmap.h"
class ExcelCellData;
class ExcelLayout;
class SetExcelCellPropertyAction :
	public BaseAction
{
public:
	SetExcelCellPropertyAction(StepActionManger * stepActonManger);
	~SetExcelCellPropertyAction();
	void initAction( ExcelLayout * excelLayout, ExcelCellData * cellData, QMap<QString, QString> preMap, QMap<QString, QString> setMap, int preWidth, int setWidth);
	void unDo();
	void reDo();
	int mPreWidth;
	int mSetWidth;
	ExcelLayout * mExcelLayout;
	ExcelCellData * mCellData;
	QMap<QString, QString> mPreMap;
	QMap<QString, QString> mSetMap;
	//bool mIsSetCell;

};
#endif