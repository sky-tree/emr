#ifndef _BASEKEYBOARD_H_
#define _BASEKEYBOARD_H_
#include "BaseTempleteKeyboard.h"
#include "ExcelLayout.h"
class ExcelKeyboard :
	public BaseTempleteKeyboard
{
public:
	ExcelKeyboard(ExcelTemplete * excelTemplate);
	~ExcelKeyboard();
	void init();
public:
	int keyPress(QString str, BaseElement * obj);
	int keyRelease(QString str, BaseElement * obj);
	static bool isSystemExcelKey(const QString & keyStr);
	
	ExcelLayout * mExcelLayout;
	ExcelTemplete * mOwnerExcelTemplete;
	QString mDividStr = "D";
	QString mMergeStr = "M";
};
#endif
