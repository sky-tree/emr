#ifndef BASE_CONSTANTS_H_
#define BASE_CONSTANTS_H_

#include "qstring.h"


class BaseConstants
{
public:
	static const QString MACRO_DEFAULT_CONTENT_STR;                                  //宏元素内容默认空格
	static const QString NONE_STR  ;
	static const int MIN_PRINT_MS = 5;												// 无

	static const short DEFAULT_FREQUENCY = 30;								// 默认帧率为30帧
	static const int DEFAULT_FLASH_VIEW_FRAME = DEFAULT_FREQUENCY * 2 / 3;
	static const int DEFAULT_FLASH_CIRCLE_FRAME = DEFAULT_FREQUENCY;


	static const int AUTO_SAVE_DIS_TIME = 600000 / DEFAULT_FREQUENCY;												// 每多少秒，自动存一次档
	//static const int AUTO_SAVE_DIS_TIME = 600000 / DEFAULT_FREQUENCY;												// 每多少秒，自动存一次档

	//static const QString DEFAULT_FONT;												// 默认字体
	//static const QString LOAD_ROOT_PATH;
	static const char NONE = 0;												// 无
	static const char MAX_MOUSE_BUFFER = 1;									// 每帧鼠标操作做大缓存数量
	static const char MAX_KEY_BUFFER = 10;									// 每帧键盘操作做大缓存数量

	static const int MAX_SHORT = 32767;									// 最大short
	static const int MIN_SHORT = -32768;									// 最小short
	static const int MIN_INT = -100000000;								// 最小int

	static const int MAX_ACTION_STEP = 100;								// 最大保存步骤
	//static const int DEFAULT_FONT_SIZE = 200;								// 默认文字大小


	static const int DEFAULT_EDIT_IMG_LINE_THICK = 2;								// 默认图像编辑线的厚度

	static const int MAX_IMG_W = 700;								// 最大图片宽度
	static const int MAX_IMG_H = 1000;								// 最大图片高度

public:

	// 返回类型
	class ReturnType
	{
	public:
		static const char RETURN_ONE_LEVEL = 1;


		static const char HAVE_NOT_DEL_ENTER = -100;
	};
	class ReplaceType
	{
	public:
		static const char ADD_TO_REPLACE_LIST = 1;
		static const char REPLACE_NOW = 2;
	};
	// 返回类型
	class AutoSaveClearTimeType
	{
	public:
		static const char CLEAR_WHEN_STATUS_CHANGE = 1;						// 状态改变时，时间清0
		static const char CLEAR_ABS = 2;									// 无条件时间清0
	};
	class AutoSaveStatus
	{
	public:
		static const short ACTIVE = 1;				// 激活计时状态
	};
	class OperateStatus
	{
	public:
		static const short FREE = 1;							// 空闲状态
		static const short MOUSE_PRESS_DOWN = 2;				// 鼠标按下状态
		//static const short MOUSE_DRAG = 3;						// 鼠标按下状态

		static const short SAVING = 100;						// 存档状态
	};
	class ObjStatus
	{
	public:
		   static const short FREE = 1;								// 空闲状态
		   static const short DRAG_CHOOSE = 2;						// 按下后拖选
		   static const short DRAG_CHOOSE_RELEASE = 3;				// 拖选后松开
		   static const short CLICK_CHOOSE = 4;						//点击选中
		   static const short CLICK_UN_CHOOSE = 5;					//点击未选中
		   static const short EXCEL = CLICK_UN_CHOOSE + 1;			//处理表格
		   static const short DRAG_SCALE = 15;						//缩放

		   static const int ACTIVE = 100;										//  页眉、页脚激活状态
		   static const char NEW_STATUS = 1;//新建状态
		   static const char EDIT_STATUS = 2;//编辑状态
		   static const char NONEDIT_STATUS = 3;//不可编辑状态，可内部选中
		   static const char NO_FOCOUS_INSIDE_STATUS = 4;//内部不可选中状态
		   static const char FOCOUS_STATUS = 5;

		   static const short PAGE_PANEL_MOVE_UP = 2;						// 页面板向上滚动
		   static const short PAGE_PANEL_MOVE_DOWN = 3;						// 页面板向下滚动
		   static const short PAGE_PANEL_SCROLL_DRAG = 5;						// 拖动页面板滚动条
	};

	class Direction
	{
	public:
		static const char LEFT = -1;
		static const char RIGHT = 1;
		static const char UP = -2;
		static const char DOWN = 2;
		static const char MIDDLE = -100;
		static const char LEFT_TOP = -10;
		static const char LEFT_BOTTOM = -11;
		static const char RIGHT_TOP = -12;
		static const char RIGHT_BOTTOM = -13;
	};

	// 显示类型
	class ViewType
	{
	public:
		static const char HIDE = -2;												// 隐藏
		static const char CONDITION_NONE_VIEW = -1;												// 条件隐藏
		static const char NORMAL = 1;												// 正常显示
		static const char PRINT = 2;												// 打印显示
		static const char CLIP_CHILDREN_INSIDE_AREA = 11;												// 仅显示自身范围
	};
	// 显示类型
	class DoActionType
	{
	public:
		static const char ONLY_ADD_ACTION = 1;												// 仅添加动作
		static const char CREATE_STEP_AND_ADD_ACTION = 2;												// 创建步骤，同时添加动作
	};
	// 设置系统属性类型
	class SetSystemParamType
	{
	public:
		static const short SET_TITLE_VIEW_TYPE = 0;												// 设置页眉显示类型
		static const short SET_BOTTOM_VIEW_TYPE = 1;												// 设置页脚显示类型
		static const short SET_TITLE_NUMBER_VIEW_TYPE = 2;												// 设置页眉页码显示类型
		static const short SET_BOTTOM_NUMBER_VIEW_TYPE = 3;												// 设置页脚页码显示类型
		//static const short SET_PAPER_TYPE = 5;												// 设置纸张类型

		static const short SET_PARAGRAPH_NUM_VIEW_TYPE = 6;												// 设置段落号显示类型

		static const short SET_SHOW_TIDY = 10;												// 设置是否整洁显示
		static const short SET_ROW_BOTTOM_LINE_TYPE = 20;												// 设置网格线类型

		static const short SET_FILE_AUTHORITY = 30;												// 设置文件读写权限

		static const short ADD_CURRENT_TIME = 50;												// 添加当前系统时间
		static const short DO_PRINT = 51;												// 打印
		static const short UNDO = 52;												// 后退
		static const short REDO = 53;												// 前进
		static const short DO_COPY = 60;												// 复制
		static const short DO_CUT = 61;												// 剪切
		static const short DO_PAST = 62;												// 粘贴
		static const short SELECT_ALL = 65;												// 全选

		static const short ADD_SPECIAL_TXT = 70;												// 插入特殊字符
		static const short ADD_IMG = 71;												// 插入图片
		static const short COPY_TXT_FROM_OUTSIDE = 75;												// 从外部复制字符串进来

		static const short FIND_PRE_STR = 80;												// 查找上一个
		static const short FIND_NEXT_STR = 81;												// 查找下一个
		static const short REPLACE_PRE_STR = 82;												// 查找及替换上一个
		static const short REPLACE_NEXT_STR = 83;												// 查找及替换下一个
		static const short REPLACE_ALL_STR = 85;												// 查找及全部替换

		static const short DO_SAVE = 100;												// 存档
	};

	// UI输入类型
	class UIInputType
	{
	public:
		static const char SET_COLOR = 1;												// 设置字体或背景色
		static const char SET_FAMILY = 2;												// 设置字体
		static const char SET_TXT_NORMAL_PARAM = 3;												// 设置文字常规属性
		static const char SET_UNDERLINE_TYPE = 4;												// 设置文字下划线
		//static const char SET_PARAGRAPH_IDENT_TYPE = 5;												// 设置段落缩进
		static const char SET_TITLE_OR_BOTTOM_AUTHORITY = 6;												// 设置页眉页脚权限
		static const char SET_TITLE_OR_BOTTOM_VIEW_TYPE = 7;												// 设置页眉页脚显示与否
		static const char SET_SHORTCUT_KEY = 9;												// 设置快捷键
		static const char GET_PARAM = 10;												// 设置快捷键
		static const char SET_AUTORITITY = 11;												// 设置权限类操作

		static const char SET_PARAGRAPH_INDENT_PARAM = 20;												// 设置段落缩进参数
		static const char SET_PARAGRAPH_HORIZON_ALIGN_TYPE = 21;										// 设置段落左中右对齐
		static const char SET_PARAGRAPH_ROW_DIS_TIMES = 22;												// 设置段落行间距倍数

		static const char SET_EXCEL_VERTICAL_ALIGN_TYPE = 30;										// 设置表格页内部上中下对齐
	};

	// 创建对象类型
	class CreateObjType
	{
	public:
		static const char ONLY_CREATE = 1;					// 仅仅创建并加进父模板
		static const char CREATE_AND_ADD_TO_VIEW = 1;					// 创建并加进view
	};
	// 索引
	class IndexType
	{
	public:
		static const char LESS_THAN_LEFT = -10;
		static const char MORE_THAN_RIGHT = -11;
	};
	// 缩进类型
	class IndentType
	{
	public:
		static const char INDENT_FIRST_ROW = 1;
		static const char INDENT_SECOND_ROW = 2;
	};
	class MousePressType
	{
	public:
		static const char PRESS = 1;
		static const char RELEASE = 2;
		static const char DRAG = 3;
		static const char MOVE = 4;

		static const char DOUBLE_PRESS = 11;

		static const char PRESS_AND_RELEASE = 127;
	};
	// 阻挡类型
	class BlockType
	{
	public:
		static const char ONLY_MOUSE = 3;
		static const char ONLY_KEY = 4;
		static const char ONLY_LOGIC = 8;
		static const char MOUSE_INSIDE_AND_LOGIC = 9;
		static const char MOUSE_ALL_SIDE_AND_LOGIC = 11;
		static const char MOUSE_ALL_SIDE_AND_KEY_AND_LOGIC = 15;
	};
	// 页眉、页脚、内容序号
	class TitleBottomPageIndex
	{
	public:
		static const char PAGE_TITLE = 0;
		static const char PAGE_BOTTOM = PAGE_TITLE + 1;
		static const char PAGE_CONTENT = PAGE_BOTTOM + 1;
	};
	// 系统相关常量属性
	class SystemParams
	{
	public:
		static const char ONLY_VIEW_PAGE_NUMBER = 1;					// 仅显示页码
		static const char VIEW_PAGE_NUMBER_AND_ALL_PAGE_NUM = 2;					// 显示页码及总页数
	};
	// 默认宽高类型
	class DefaultWHType
	{
	public:
		static const char FILL_PARENT_INSIDE = -1;					// 填充父对象inside范围
		static const char FILL_PARENT_TOTAL = -2;					// 填充父对象total范围(暂时没用)
		static const char FILL_PARENT_RELATIVE_INSIDE = -3;					// 填充父对象relativeLayout相对inside范围
		static const char WRAP_CONTENT_INSIDE = -11;				// 根据子对象来适配自身的inside范围
		//static const char CHILDREN_CAN_OUTSIDE = -3;				// 子对象可以超出自身范围
	};
	// 文字属性修改动作类型
	class TxtParamActionType
	{
	public:
		static const char SET_BOLD = 0;												// 设置文字是否粗体
		static const char SET_ITALIC = 1;												// 设置文字倾斜
		static const char SET_UNDERLINE = 2;												// 设置文字下划线
		static const char SET_FONT_COLOR = 3;												// 设置文字色
		static const char SET_BACKGROUND__COLOR = 4;												// 设置文字背景色
		static const char SET_FLOW_TYPE = 5;												// 设置文字上下标
		static const char SET_FAMILY = 6;												// 设置字体
		static const char SET_FONT_SIZE = 7;												// 设置文字大小
		static const char SET_DEL_LINE = 10;												// 设置文字删除线
	};


	class CanCreateType
	{
	public:
		static const short DIRECT_CREATE_ON_SELF = 1;							// 直接创建，不考虑越界
		static const short ONLY_GET_FROM_NEXT_PARENT = 2;						// 仅在下一页有时，获取下一行
		static const short CAN_CREATE_FROM_NEXT_PARENT = 3;						// 可在下一页没有时，在下一页创建下一行
	};
	// 相对布局中相对位置关系
	class RelativeDirectionType
	{
	public:
		static const char LEFT = 1;							//在某对象左边
		static const char RIGHT = 2;						//在某对象右边
		static const char TOP = 3;							//在某对象上边
		static const char BOTTOM = 4;						//在某对象下边
		static const char MIDDLE_X = 5;						//横向居中
		static const char MIDDLE_Y = 6;						//纵向居中
		static const char INSIDE_LEFT = 11;							//在某对象内部左边
		static const char INSIDE_RIGHT = 12;						//在某对象内部右边
		static const char INSIDE_TOP = 13;							//在某对象内部上边
		static const char INSIDE_BOTTOM = 14;						//在某对象内部下边

	};
	class CircleChildrenOrParentType
	{
	public:
		//static const char REFREHSH_CHILDREN_WHEN_CHANGE = 1;							//当数值变化时，递归判断子对象
		//static const char REFREHSH_PARENT_WHEN_CHANGE = 2;							//当数值变化时，递归判断父对象
		static const char DIRECT_REFREHSH_CHILDREN = 1;							//无条件进行递归判断子对象
		static const char DIRECT_REFREHSH_PARENT = 2;							//无条件进行递归判断父对象
		static const char DIRECT_REFREHSH_PARENT_AND_CHILDREN = 3;							//无条件进行递归父子对象
	};
	// 查找、替换类型
	class FindType
	{
	public:
		static const char NORMAL_TXT_CONTENT = 1;					// 常规文字内容
		static const char TEMPLATE = 2;					// 查找到模板
	};

};


#endif // BASE_CONSTANTS_H_
