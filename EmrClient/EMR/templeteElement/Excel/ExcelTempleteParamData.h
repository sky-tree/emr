#ifndef _EXCELTEMPLETEPARAMDATA_H_
#define _EXCELTEMPLETEPARAMDATA_H_
#include "CodeTempData.h"
class ExcelTemplete;
class Line;
class ExcelCellData;
class BaseTemplate;
class ExcelTempleteParamData :public CodeTempData
{
public:
	ExcelTempleteParamData(BaseTemplate * templeteOwner, CodeParams * codeParams);
	~ExcelTempleteParamData();
	void setExcelTempleteOwner(ExcelTemplete * templeteOwner);
public:
	void saveSelfData(BaseIOTools * ioTools);
	void loadSelfData(BaseIOTools * ioTools);
	BaseContainer * createOwner(BaseContainer * tempParent1, BaseIOTools *iOTools, bool isAddToParenTemplate);
	ExcelTemplete * mTempleteOwner;
	bool mIsShowBorder;
	QList<Line *> * mRowLines;
	QList<Line *> * mColLines;
	QList<ExcelCellData *>* mCells;
};
#endif
