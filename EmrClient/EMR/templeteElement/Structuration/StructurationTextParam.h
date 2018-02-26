#ifndef _STRUCTURATIONTEXTPARAM_H_
#define _STRUCTURATIONTEXTPARAM_H_
#include "CodeTempData.h"
#include "qstring.h"
class StructurationTextWidget;
class DrawTxtParam;
class InputFilter;
class StrContentData;
class BaseTemplate;
class StructurationTextParam :
	public CodeTempData
{
public:
	StructurationTextParam(BaseTemplate * templeteOwner, CodeParams * codeParams);
	~StructurationTextParam();
	void setTextTempleteOwner(StructurationTextWidget * templeteOwner);
public:
	void saveSelfData(BaseIOTools * ioTools);
	void loadSelfData(BaseIOTools * ioTools);
	BaseContainer * createOwner(BaseContainer * tempParent, BaseIOTools * ioTools, bool isAddToParenTemplate);
	StructurationTextWidget * mTempleteOwner;
private:

	void saveStrAndDefaultStr(BaseIOTools * ioTools);
	void loadStrAndDefaultStr(BaseIOTools * ioTools);

	QString mStr;
	QString mDecimal;
	InputFilter * mInputFilter; //输入过滤器
	QString mDefaultString;// 用mDefaultString 空格字符填充
	int mTemptStatus;

	//数字过滤器时专用
	QString mValueType;//数值类型true，日期类型值为 "false"
	//QString mdataTime;//日期
	QString mMaxValue;
	QString mMinValue;

	//QString content;//数值时类容
	QString mLength;//总长度
	QString mDecimalLen;//小数长度
	QString mTimeFomat;//时间格式
	bool mHadSetCurrentTime;
	bool  mIsTwentyFourFomat;
	StrContentData * mFocusTxtData;
	//名称
	QString mNameStr;
};
#endif
