#ifndef _SETEXCELPROPERTYACTION_H_
#define _SETEXCELPROPERTYACTION_H_
#include "BaseAction.h"
#include "qmap.h"
#include "qstring.h"
class ExcelCellData;
class ExcelLayout;
class SetExcelPropertyAction :
	public BaseAction
{
public:
	SetExcelPropertyAction(StepActionManger * stepActonManger);
	~SetExcelPropertyAction();
	void initAction(ExcelLayout * excelLayout, ExcelCellData * cellData, QMap<QString, QString> preMap, QMap<QString, QString> setMap, bool preIsShowBorder, int preBorderThick, int preInsideWidth, QString preAlign, bool setIsShowBorder, int setBorderThick, int setInsideWidth, QString setAlign);
	void unDo();
	void reDo();
	bool mPreIsShowBorder;
	int mPreborderThick;
	int mPreInsideWidth; 
	QString mPreAlign;
	bool mSetIsShowBorder;
	int mSetBorderThick;
	int mSetInsideWidth;
	QString mSetAlign;
	ExcelLayout * mExcelLayout;
	ExcelCellData * mCellData;
	QMap<QString, QString> mPreMap;
	QMap<QString, QString> mSetMap;
	//bool mIsSetCell;

};
#endif