#include "ExcelLayoutViewParam.h"
#include "ConstantsID.h"


ExcelLayoutViewParam::ExcelLayoutViewParam(ViewObj * viewOwner) : BaseViewParam(viewOwner,ConstantsID::ClassId::EXCEL_VIEW_PARAM)
, mInsideLeft(0)
, mInsideRight(0)
, mInsideTop(0)
, mInsideBottom(0)
{
}
ExcelLayoutViewParam::~ExcelLayoutViewParam()
{
}
void ExcelLayoutViewParam::setInsideLeft(int insideLeft)
{
	mInsideLeft = insideLeft;
}
void ExcelLayoutViewParam::setInsideRight(int insideLeft)
{
	mInsideRight = insideLeft;
}
void ExcelLayoutViewParam::setInsideTop(int insideTop)
{
	mInsideTop = insideTop;
}
void ExcelLayoutViewParam::setInsideBottom(int insideBottom)
{
	mInsideBottom = insideBottom;
}

int ExcelLayoutViewParam::getInsideLeft()
{
	return mInsideLeft;
}
int ExcelLayoutViewParam::getInsideRight()
{
	return mInsideRight;
}
int ExcelLayoutViewParam::getInsideTop()
{
	return mInsideTop;
}
int ExcelLayoutViewParam::getInsideBottom()
{
	return mInsideBottom;
}
