#ifndef _TEXTWIDGETPARAM_H_
#define _TEXTWIDGETPARAM_H_
#include "CodeTempData.h"
#include "qstring.h"
class TextWidget;
class DrawTxtParam;
class InputFilter;
class BaseTemplate;
class BaseIOTools;
class TextWidgetParam :
	public CodeTempData
{
public:
	TextWidgetParam(BaseTemplate * templeteOwner, CodeParams * codeParams);
	~TextWidgetParam();
	void setTextTempleteOwner(TextWidget * templeteOwner);
public:
	void saveSelfData(BaseIOTools * ioTools);
	void loadSelfData(BaseIOTools * ioTools);
	BaseContainer * createOwner(BaseContainer * tempParent, BaseIOTools *iOTools, bool isAddToParenTemplate);
	TextWidget * mTempleteOwner;
private:

	void saveStrAndDefaultStr(BaseIOTools * ioTools);
	void loadStrAndDefaultStr(BaseIOTools * ioTools);
	
	QString mStr;
	QString mDecimal;
	InputFilter * mInputFilter; //输入过滤器
	QString mDefaultString;// 用mDefaultString 空格字符填充
	int mTemptStatus;

	//数字过滤器时专用
	int mStrMaxSize;
	int mDecimalPointLenght;
	StrContentData *mFocusTxtData;
public:
	//名称
	QString mNameStr;
};
#endif

