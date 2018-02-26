#ifndef STR_CONSTANTS_H_
#define STR_CONSTANTS_H_


#include <QString>
//#define DEBUG_MODE

class StrConstants
{
public:
	/*********************************************************** 版本号 *****************************************************************/
	static const QString VERSION_0_0_0_1;									// 初始版本
	static const QString VERSION_0_0_0_2;									// SystemParams里添加页面类型属性
	static const QString VERSION_0_0_0_3;									// SystemParams里添加网格线属性
	static const QString VERSION_0_0_0_4;									// 勾选项设置左边距为属性
	static const QString VERSION_0_0_0_5;									// 添加三级检诊相关属性
	static const QString VERSION;											// 当前版本号
	/**************************************************************************************************************************************/

	static const QString NONE;
	static const QString MACRO_DEFAULT_CONTENT_STR ;                                  //宏元素内容默认空格

	static const QString BACK_SPACE;

	static const QString NONE_STR;												// 空字符

	static const QString PAGE_NUMBER_FIRST_WORD;									// 第
	static const QString PAGE_NUMBER_LAST_WORD;									// 页
	static const QString HORIZONTAL_LINE_STR;												// 横线
	static const QString END_PAGE_LINE_STR;												// 分页符

	static const QString DEFAULT_FONT_FAMILY;												// 默认字体

	static const QString DEFAULT_HOSPITAL_TITLE_FONT_FAMILY;												// 默认医院标题字体
	static QString DEFAULT_HOSPITAL_TITLE;												// 默认医院标题

	static const QString NUMBER_9;												// 9、用于计算页码宽度
	static const QString NUMBER_99;												// 99、用于计算页码宽度
	static const QString NUMBER_999;												// 999、用于计算页码宽度
	static const QString NUMBER_9999;												// 9999、用于计算页码宽度
	static const QString NUMBER_99999;												// 99999、用于计算页码宽度

public:

	// 路径
	class MyPath
	{
	public:
		static QString DIR_RES;
		static const QString FILE_SUFFIX;
	};
	// 按键
	class MyKeyBoard
	{
	public:
		static const QString BACK_SPACE;
		static const QString DEL;
		static const QString TAB;
		static const QString LEFT;
		static const QString RIGHT;
		static const QString UP;
		static const QString DOWN;

		static const QString ESC;
		static const QString ENTER;
		static const QString CHOICE_BOX_RECT;
		static const QString CHOICE_BOX_RADIO_MARK;
		static const QString CHOICE_BOX_MULTPLE_MARK;
		static const QString EQUAL;
		static const QString MINUS ;
		static const QString NINE;
		static const QString EIGHT;
		static const QString SHIFT;

		static const QString CUT;
		static const QString COPY;
		static const QString PAST;

		static const QString UNDO;
		static const QString REDO;

		static const QString SELECT_ALL;
	};
};


#endif // BASE_CONSTANTS_H_
