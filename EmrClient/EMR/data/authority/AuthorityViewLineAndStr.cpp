#include "AuthorityViewLineAndStr.h"

#include "ViewObj.h"


#include "BaseIOTools.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

AuthorityViewLineAndStr::AuthorityViewLineAndStr() : BaseElement(ConstantsID::ClassId::AUTHORITY_VIEW_LINE_AND_STR)
, mLineNumber(-1)
, mLinePosition(0)
, mLineColor(0)
, mStrColor(0)
{
}
AuthorityViewLineAndStr::AuthorityViewLineAndStr(int lineNumber, char linePosition, int lineColor, int strColor)
: BaseElement(ConstantsID::ClassId::AUTHORITY_VIEW_LINE_AND_STR)
, mLineNumber(lineNumber)
, mLinePosition(linePosition)
, mLineColor(lineColor)
, mStrColor(strColor)
{
}

AuthorityViewLineAndStr::~AuthorityViewLineAndStr()
{
}
int AuthorityViewLineAndStr::getLineNum()
{
	return mLineNumber;
}
char AuthorityViewLineAndStr::getLinePosition()
{
	return mLinePosition;
}
int AuthorityViewLineAndStr::getLineColor()
{
	return mLineColor;
}
int AuthorityViewLineAndStr::getStrColor()
{
	return mStrColor;
}
