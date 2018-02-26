#ifndef GLOBALVARIABLE
#define GLOBALVARIABLE
#include "qstring.h"
#include"views/globalvariable.h"
extern const int kPushBtnFontPointSize;
extern const int kPushBtnFontWeight;
 
extern const char * kPushBtnStyle;
extern const char * kListViewStyle;
extern const QString exeImagePath;
extern const QString  progressNote;

enum  dlgType
{
	//表格属性，           输入提示，    关键词，    页面设置          单选                   多选                        有无选择                   格式化录入      表格设置           时间选项     数值                 分页符       固定文本      横线   公式    图像
	cellPropertyDlg = 1, inputPromptDlg, keyWordDlg, pageSettingDlg, selectionPropertyDlg,selectionPropertyDlg_multity, selectionPropertyDlg_bool, structuringDlg, tablePropertyDlg,timeSelection, numericalValue,	 pageBreak, changelessText,       line, formula, picture, 
	//特殊符号      //常用医学单位     月经史公式            宏元素								段落
	specialSymbol, commonMedicalUnit, formulaDlg, macroPropertyPopupDlg, paragraphSettingDlg,unfoldRedioDlg,unfoldMultpleChoiceDlg
		
};
#endif // GLOBALVARIABLE

