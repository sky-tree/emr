#ifndef UI_CONSTANTS_H_
#define UI_CONSTANTS_H_


#include <QColor>

class UIConstants
{
public:


	static const int UI_TOOLBAR_H = 25;

	static const short MAX_ROW_INSIDE_W = 600;
	static const short MAX_ROW_INSIDE_H = 600;
	static const short MIN_ROW_INSIDE_W = 50;
	static const short MIN_ROW_INSIDE_H = 50;

public:

	static const float DEFAULT_ROW_DIS_H;								// 默认行间距

	static const int DEFAULT_PAGE_PANEL_DRAG_DIS_Y = 5;								// 默认页面板拖动disY

	static const int DEFAULT_PAGE_BOTTOM_DRAG_EFFECT_AREA = 5;								// 默认页面板拖动有效偏移H

	static const char FLAG_WIDTH = 2;									// 光标宽度

	static const short IMG_MIN_WH = 10;

	static const float IMG_DRAG_ALPHA;								// 图片拖动时，透明度

	static const int SCROLL_UP_DOWN_BUTTON_START_MOVE_FRAME = 30;			// 滚动条上下按钮长按启动隔帧
	static const int SCROLL_UP_DOWN_BUTTON_PRESS_DIS_FRAME = 5;			// 滚动条上下按钮长按启动滚动后，执行移动间隔帧
	static const float SCROLL_DIS_PERCENT_Y;
	static const int SCROLL_DISTANCE = 1200;							
	static const int SCROLL_DENOMINATOR = 1800;							// 滚动条分母，滚动一次分子为120

	static const int IMG_EDIT_WINDOW_TOOLBAR_DIS_LEFT = 10;
	static const int IMG_EDIT_WINDOW_TOOLBAR_DIS_BOTTOM = 10;
	static const int IMG_EDIT_WINDOW_TOOLBAR_DIS_TOP = 10;
	static const int IMG_EDIT_WINDOW_TOOLBAR_DIS_RIGHT = 10;

	static const int HORIZONTAL_LINE_OFFSET_HEIGHT = 8;				//横线偏移高度

	static const int REFRESH_FLAG_MIN_EFFECT_H = 100;						// 光标自动归位有效最小值（有效值小于此值才自动贵为）



	// 纸张类型
	class PaperTypeAndWH
	{
	public:
		static const char A4 = 1;
		static const char B5 = 2;

		static const int A4_W = 210;								// A4纸宽度，厘米
		static const int A4_H = 297;								// A4纸高度，厘米
		static const int B5_W = 182;								// B5纸宽度，厘米
		static const int B5_H = 257;								// B5纸高度，厘米


		static const short A4_W_PIX = 840;				// A4宽度，像素
	};
	class WindowParams
	{
	public:
		//static const short WINDOW_START_X = 10;
		static const short WINDOW_START_Y = 46;
		static const short WINDOW_PAGE_PANEL_X = 30;
		//static const short WINDOW_PAGE_PANEL_Y = 30;
		static const short WINDOW_PAGE_PANEL_BOTTOM = 80;
		static const short WINDOW_PAGE_PANEL_INSIDE_LEFT_RIGHT = 30;
		static const short WINDOW_PAGE_PANEL_INSIDE_TOP = 60;
		static const short WINDOW_PAGE_PANEL_INSIDE_BOTTOM = 200;
		//static const short WINDOW_PAGE_CONTENT_DIS_RIGHT = 30;
		//static const short WINDOW_PAGE_CONTENT_DIS_LEFT = 30;
		//static const short WINDOW_PAGE_CONTENT_DIS_TOP = 80;
		//static const short WINDOW_PAGE_CONTENT_DIS_BOTTOM = 80;
		static const short DEFAULT_W = 1000;
		static const short DEFAULT_H = 900;
		//static const float W_H_RATIO;
		static const short EDIT_IMG_WINDOW_INSIDE_LEFT = 30;
		static const short EDIT_IMG_WINDOW_INSIDE_TOP = 30;

		static const short MIN_EDIT_IMG_WINDOW_W = 180;
		static const short MIN_EDIT_IMG_WINDOW_H = 180;
	};

	// 页相关变量
	class PageParams
	{
	public:

		static const short DEFAULT_PAGE_TITLE_MAX_H = 300;

		static const short PAGE_TXT_LAYER_CLIP_LEFT = -150;
		static const short PAGE_TXT_LAYER_CLIP_RIGHT = -100;

		static const short WINDOW_FIRST_PAGE_START_Y = 30;

		static const short PAGE_MIN_CONTENT_Y = 70;

		static const short PAGE_TITLE_DIS_TOP = 30;
		static const short PAGE_TITLE_DIS_BOTTOM = 1;

		static const short PAGE_TITLE_MIN_H = 300;
		static const short PAGE_TITLE_MAX_H = 300;
		static const short PAGE_BOTTOM_MAX_H = 300;

		static const short PAGE_MIN_W = 60;
		static const short PAGE_MIN_H = 60;

		static const short PAGE_DIS_Y = 20;

		static const short INSIDE_LEFT = 63;
		static const short INSIDE_RIGHT = 58;
		static const short INSIDE_TOP = 50;
		static const short INSIDE_BOTTOM = 50;

		static const short PAGE_CONTENT_INSIDE_TOP = 20;
		static const short PAGE_CONTENT_INSIDE_BOTTOM = 20;

		static const short TITLE_OR_BOTTOM_LAYER_DIS = 10;

		static const short START_X = 50;
		static const short START_Y = 50;
		static const short END_Y = 50;

		static const short EDIT_IMG_PAGE_START_X = 10;
		static const short EDIT_IMG_PAGE_START_Y = 10;
		static const short EDIT_IMG_PAGE_WIDTH = 150;
		static const short EDIT_IMG_PAGE_HEIGHT = 90;

		static const short EDIT_IMG_PAGE_DIS = 6;

		static const short EXCEL_PAGE_START_X = 10;
		static const short EXCEL_PAGE_START_Y = 10;

		static const short EXCEL_PAGE_INSIDE_LEFT = 1;
		static const short EXCEL_PAGE_INSIDE_TOP = 1;
		static const short EXCEL_PAGE_INSIDE_RIGHT = 1;
		static const short EXCEL_PAGE_INSIDE_BOTTOM = 1;


	};
	// 行相关变量
	class RowParams
	{
	public:
		static const short INSIDE_TOP = 0;
		static const short INSIDE_BOTTOM = 1;

		static const short DIS_LEFT = 10;
		static const short DIS_RIGHT = 10;
		static const short DIS_TOP = 10;
		static const short DIS_BOTTOM = 10;

		static const short ROW_HEIGHT = 60;


		static const int TAB_WIDTH = 30;								// tab宽
		static const int TAB_HEIGTH = 6;								// tab高


		static const int PARAGRAPH_NUMBER_FONT_SIZE = 10;				// 段落号字体大小
		static const int PARAGRAPH_NUMBER_FONT_WEIGHT = 1;				// 段落号字体厚度
		static const int PARAGRAPH_NUMBER_FONT_DIS_X = -12;				// 段落号字x偏移
	};
	// 文字上、下标类型
	class UpBottomOrMiddle
	{
	public:
		static const char UP = 1;
		static const char DOWN = 2;
	};
	// 矩形填充类型
	class FillRectType
	{
	public:
		static const char ONLY_ROUND_RECT = 1;									// 仅边框
		static const char ONLY_FILL = 2;									// 仅填充
		static const char ROUND_LINE_AND_FILL = 3;							// 边框+填充
		static const char ROW_NO_FILL = 100;							// 行不绘制矩形
	};
	// 下划线类型
	class LineType
	{
	public:
		static const char SINGLE_LINE = 1;									// 单下划实现
		static const char DOUBLE_LINE = 2;									// 双下划实现

		static const char SINGLE_DOT_LINE = 11;									// 单下划虚线
		static const char DASH_DOT_LINE = SINGLE_DOT_LINE + 1;
	};
	// 颜色常量
	class MyColor
	{
	public:
		static const int WHITE = 0xffffffff;										//  白色
		static const int BLACK = 0xff000000;										//  黑色
		static const int GRAY = 0xff444444;
		static const int GRAYER = 0x99888888;
		static const int MACRO_GRAY = 0xff444488;
		static const int RED = 0xffff0000;											//  红色
		static const int GREEN = 0xff00ff00;										//  绿
		static const int BLUE = 0xff0000ff;											//  蓝
		static const int YELLOW = 0xffffff00;											//  黄色
		static const int BACKG_ROUND = 0xffdbdbdb;											//  黄色

		static const int WINDOW_BACKGROUND_COLOR = 0xff6f93c3;

		static const int CHOOSEN_COLOR = 0x880000ff;											//  选中色
		static const int PRESS_FOCUS_COLOR = 0x8800ff00;											//  选中色


		static const int ONLY_READ_COLOR = 0xff555555;											//  只读文字颜色
		static const int ONLY_READ_PRINT_COLOR = 0xff000000;											//  只读文字打印时颜色
		static const int PARAGRAPH_NUM_COLOR = 0xff888888;												// 段落号字体颜色值


		static QColor * WHITE_COLOR_OBJ;											//  白色
		static QColor * BLACK_COLOR_OBJ;											//  黑色
		static const QColor * ONLY_READ_COLOR_OBJ;											//  只读文字打印时颜色
		static const QColor * PARAGRAPH_NUM_COLOR_OBJ;												// 段落号字体颜色对象

	};

	class DefaultFontParams
	{
	public:
		static const short TXT_SIZE = 12;
		static const short DEFAULT_HOSPITAL_TITLE_FONT_SIZE = 20;
		static const int FONT_COLOR = 0xff000000;
		static const int BACKGROUND_COLOR = 0;

		static const bool ITALIC = false;
		static const bool BOLD = false;
	};
};


#endif
