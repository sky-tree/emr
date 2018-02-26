#ifndef CONSTANTS_ID_H_
#define CONSTANTS_ID_H_


#include <QString>

class ConstantsID
{
public:

	// classId
	class ClassId
	{
	public:
		static const int BASE_CONTAINER = 1;									// 基本容器
		static const int VIEW_OBJ = 2;											// 显示对象
		static const int TEMPLATE_VIEW_OBJ_CLASS = VIEW_OBJ + 1;						//模板视图对象
		static const int PARAGRAPH_OBJ = TEMPLATE_VIEW_OBJ_CLASS + 1;											// 段落对象
		static const int SELECT_VIEW_OBJ = PARAGRAPH_OBJ + 1;									// 表格鼠标选择时对象
		/******************************* 模板对象范围：< 0 开始 ************************************/
		static const int BASE_TEMPLATE = -1;											// 基本模板
		/******************************* 模板对象范围 结束 ************************************/
		/****************************************************** 键盘输入：10000000-10099999 *************************************************************/
		/******************************* 键盘输入管理：10000000-10009999 ************************************/
		static const int OBJ_KEY_MANAGER = 10000000;									// 对象按键管理
		static const int  TEMP_VIEWOBJ_KEY_MANAGER = OBJ_KEY_MANAGER + 1;				//模板视图对象键盘输入管理器
		static const int  TEMP_VIEWOBJ_MOUSE_MANAGER = TEMP_VIEWOBJ_KEY_MANAGER + 1;				 //模板视图对象鼠标输入管理器
		/*********************************** 键盘输入：10010000-10019999 ************************************/
		static const int BASE_KEY = TEMP_VIEWOBJ_MOUSE_MANAGER + 1;
		static const int TEXT_WIDGET_KEYBOARD = BASE_KEY + 1;					 //文字元素键盘输入
		static const int INPUT_FILTER = TEXT_WIDGET_KEYBOARD + 1;				//输入过滤
		static const int NUMBER_INPUT_FILTER = INPUT_FILTER + 1;						//输入过滤
		static const int EXCEL_KEYBOARD = NUMBER_INPUT_FILTER + 1;					//表格输入
		/****************************************************** 鼠标输入：10100000-10199999 *************************************************************/
		/******************************* 鼠标输入管理：10100000-10109999 ************************************/
		static const int OBJ_MOUSE_MANAGER = 10100000;									// 对象鼠标管理
		/*********************************** 鼠标输入：10110000-10119999 ************************************/
		static const int TXT_WIN_MOUSE = OBJ_MOUSE_MANAGER + 1;							// txtWindow鼠标输入
		static const int BUTTON_MOUSE = TXT_WIN_MOUSE + 1;							// 按钮鼠标输入
		static const int TEXT_WIDGET_MOUSE = BUTTON_MOUSE + 1;						//文字元素鼠标输入
		static const int EXCEL_MOUSE = TEXT_WIDGET_MOUSE + 1;					//表格元素鼠标输入
		static const int EXCEL_PAGE_MOUSE = EXCEL_MOUSE + 1;					//表格单元格鼠标输入
		/****************************************************** 逻辑：10200000-10219999 *************************************************************/
		/******************************* 逻辑管理：10200000-10209999 ************************************/
		static const int OBJ_LOGIC_MANAGER = 10200000;									// 对象基本逻辑管理
		static const int TXT_WIN_LOGIC_MANAGER = 10200001;									// txtWindow逻辑管理
		/*********************************** 逻辑：10210000-10219999 ************************************/
		static const int FLAG_FLASH_LOGIC = 10210000;									// flag光标闪烁逻辑
		static const int JUDGE_PAGE_PANEL_SCROLL_LOGIC = 10210001;									// 判断pagePanel滚动逻辑
		/******************************* 跨父对象管理：10220000-10229999 ************************************/
		static const int CROSS_PARENT_MGR = 10220000;									// 基本跨父对象管理
		static const int EXCEL_CROSS_PARENT_MGR = 10220001;									// 表格跨父对象管理

		/****************************************************** 绘制：10300000-10399999 *************************************************************/
		/******************************* 绘制管理：10300000-10309999 ************************************/
		static const int OBJ_DRAW_MANAGER = 10300000;									// 对象绘制管理
		static const int TXT_WINDOW_DRAW_MANAGER = 10300001;									// txt窗口绘制管理
		/*********************************** 绘制：10310000-10319999 ************************************/
		static const int DRAW_RECT = 10310000;									// 矩形绘制
		static const int DRAW_IMAGE = 10310001;									// 矩形图片
		static const int DRAW_TXT = 10310002;									// 绘制文字
		static const int DRAW_LINE = DRAW_TXT + 1;							// 绘制线
		static const int DRAW_ROW_TOP = 10310100;									// 行上层绘制
		static const int DRAW_ROW_BOTTOM = 10310101;									// 行下层绘制
		static const int DRAW_HORIZONTAL_LINE = 10310200;									// 行下层绘制


		/****************************************************** 子对象管理：10600000-10699999 *************************************************************/
		static const int BASE_CHILDREN_MANAGER = 10600000;									// 基本子对象管理
		/******************************* 绘制子对象管理：10600001-10609999 ************************************/
		static const int BASE_LAYOUT = 10600001;								// 基本布局
		static const int RELATIVE_LAYOUT = 10600002;								// 相对布局
		static const int VERTICAL_LAYOUT = 10600010;								// 纵向布局
		static const int BASE_TXT_LAYER_LAYOUT = 10600011;						// 基本文本层布局
		static const int PAGE_CONTENT_TXT_LAYER_LAYOUT = 10600012;						// 页内容文本层布局
		static const int HORIZONTAL_LAYOUT = 10600020;								// 横向布局
		static const int ROW_LAYOUT = 10600021;								// 行布局
		static const int EXCEL_LAYOUT = ROW_LAYOUT + 1;						// 表格布局
		/******************************* 模板子对象管理：10610000-10619999 ************************************/
		static const int TEMP_CONTAINER_MGR = 10610000;								// 模板容器子对象管理器(拥有段落的模板容器)
		static const int NODE_MGR = 10610100;								// 页眉、页脚、页内容有效容器，含节点的子对象管理器
		static const int NODE_CHILDREN_MGR = 10610101;								// 节点自身的子对象管理器
		static const int PARAGRAPH_MGR = 10619000;								// 段落管理器
		/******************************* 模板元素：10620000-10629999 ************************************/
		static const int TEMPLETE_ELEMENT_MIX = -110;

		static const int TXT_WIDGET = TEMPLETE_ELEMENT_MIX + 1;         //文字模板   
		static const int NOCOPYTEXTWIDGET = TXT_WIDGET + 1; //不可复制文字模板
		static const int STRUCTURATION_TEXT_TEMPLETE = NOCOPYTEXTWIDGET + 1;//结构化文字模板
		static const int BRAKETWIDGET_TEMPLETE = STRUCTURATION_TEXT_TEMPLETE + 1;//带括号文字模板
		static const int MENSTRUATION_TEMPLETE = BRAKETWIDGET_TEMPLETE + 1;//月经史公式
		static const int MACRO_WIDGET_TEMPLETE = MENSTRUATION_TEMPLETE + 1;	//	宏元素
		static const int CHOICE_WIDGET_TEMPLETE = MACRO_WIDGET_TEMPLETE + 1;//选择项元素
		static const int EXCEL_TEMPLETE = CHOICE_WIDGET_TEMPLETE + 1;	//表格
		static const int UNFOLD_CHOICE_BUTTON_TEMPLETE = EXCEL_TEMPLETE + 1;	//展开选择项按钮
		static const int UNFOLD_CHOICE_ITEM_TEMPLETE = UNFOLD_CHOICE_BUTTON_TEMPLETE + 1;	//展开选择项单项
		static const int UNFOLD_CHOICE_WIDGET_TEMPLETE = UNFOLD_CHOICE_ITEM_TEMPLETE + 1;//展开项选项模板
		static const int TEMPLETE_ELEMENT_MAX = -80;


		/****************************************************** 前进后退：10800000-10899999 *************************************************************/
		static const int ACTION_MANAGER = 10800000;									// 前进后退管理器
		static const int STEP_ACTION_MANAGER = 10800001;									// 一步动作管理器（一步动作可以是多个动作的组合）

		static const int ADD_OBJ_LIST_ACTOIN = 10801000;									// 添加对象列表动作，包括添加字符串、图片、回车、粘贴等
		static const int DEL_OBJ_LIST_ACTOIN = 10801001;									// 删除对象列表动作
		static const int SET_SYS_BOOL_PARAM_ACTOIN = 10801003;									// 设置页面属性动作
		static const int SET_SYS_INT_PARAM_ACTOIN = 10801005;									// 设置系统int属性动作
		static const int SET_TXT_NUM_PARAM_ACTOIN = 10801008;									// 设置文字数字属性动作
		static const int SET_TXT_MULTIPLE_NUM_PARAM_ACTOIN = SET_TXT_NUM_PARAM_ACTOIN + 1;									// 设置文字数字属性动作
		static const int SET_TXT_STR_PARAM_ACTOIN = SET_TXT_MULTIPLE_NUM_PARAM_ACTOIN + 1;									// 设置文字字符串属性动作
		static const int ZOOM_IMAGE_ACTION = SET_TXT_STR_PARAM_ACTOIN + 1;										// 图片缩放动作

		static const int SET_TXT_TEMPLETE_ELEMENT_ACTION = 10801050;						// 设置文字元素属性动作
		static const int  ADD_TEMPLETE_WIDGET_ACTION = SET_TXT_TEMPLETE_ELEMENT_ACTION + 1;// 添加文字元素属性动作
		static const int  SET_CHOICE_TEMPLETE_ELEMENT_ACTION = ADD_TEMPLETE_WIDGET_ACTION + 1;//设置选择项属性
		static const int  SET_MACRO_TEMPLETE_ELEMENT_ACTION = SET_CHOICE_TEMPLETE_ELEMENT_ACTION + 1;//设置红元素属性
		static const int  SET_MENSTRUATION_TEMPLETE_ELEMENT_ACTION = SET_MACRO_TEMPLETE_ELEMENT_ACTION + 1;//设置月经史公式属性
		static const int  EXCEL_CELL_CHANGE_LINE_ACTION = SET_MENSTRUATION_TEMPLETE_ELEMENT_ACTION + 1;//表格变换线动作
		static const int  MOVE_EXCEL_LINE_ACTION = EXCEL_CELL_CHANGE_LINE_ACTION + 1;//表格线移动动作
		static const int EXCEL_RELAYOUT_ACTION = MOVE_EXCEL_LINE_ACTION + 1;//表格排版动作
		static const int SET_FOCUS_ACTION = EXCEL_RELAYOUT_ACTION + 1;//设置焦点动作
		static const int SET_EXCEL_PROPERTY_ACTION = SET_FOCUS_ACTION + 1;//设置表格属性动作
		static const int SET_EXCEL_CELL_PROPERTY_ACTION = SET_EXCEL_PROPERTY_ACTION + 1;//设置表格单元格属性动作
		static const int  NEW_OR_DELETE_LINE_ACTION = SET_EXCEL_CELL_PROPERTY_ACTION + 1;//添加删除线
		static const int  NEW_OR_DELETE_CELLDATA_ACTION = NEW_OR_DELETE_LINE_ACTION + 1;//添加删除表格单元格
		static const int  SET_PAGE_PROPERTY = NEW_OR_DELETE_CELLDATA_ACTION + 1;	// 设置页面动作
		static const int  SET_PARAGRAPH_OTHER_PARAM_ACTION = SET_PAGE_PROPERTY + 1;// 设置段落其他参数动作
		static const int  SET_EXCEL_VERTICAL_LAYOUT_ACTION = SET_PARAGRAPH_OTHER_PARAM_ACTION + 1;// 设置表格纵向排版动作
		static const int SET_UNFOLD_CHOICE_TEMPLETE_ELEMENT_ACTION = SET_EXCEL_VERTICAL_LAYOUT_ACTION + 1;//设置展开型选择项属性动作
		static const int SET_PARAGRAPH_INT_PARAM_ACTOIN = 10801100;									// 设置段落int属性动作
		static const int SET_PARAGRAPH_FLOAT_PARAM_ACTOIN = 10801101;									// 设置段落float属性动作
		static const int SET_PARAGRAPH_INDENT_PARAM_ACTOIN = 10801102;									// 设置段落indent属性动作

		/******************************* UI信号数据：11000000-11099999 ************************************/
		static const int UI_DATA_RESIZE_WINDOW = 11000000;									// 刷新窗口宽高
		static const int UI_DATA_SET_SYS_SIGNAL_PARAMS = 11000001;									// 设置系统信号参数，仅需要类型
		static const int UI_DATA_SET_SYS_BOOL_PARAMS = 11000002;									// 设置系统bool参数
		static const int UI_DATA_SET_SYS_STR_PARAMS = 11000003;									// 设置系统str参数
		static const int UI_DATA_SET_SYS_INT_PARAMS = 11000005;									// 设置系统int参数

		static const int UI_DATA_FIND_AND_REPLACE_PARAMS = 11000007;									// 查找及替换

		static const int UI_DATA_SET_PAGE_PARAMS = 11000008;									// 设置页面参数

		static const int UI_DATA_SET_TXT_BOOL_PARAMS = 11000010;									// 设置文字bool参数
		static const int UI_DATA_SET_TXT_INT_PARAMS = 11000011;									// 设置文字int参数
		static const int UI_DATA_SET_TXT_STR_PARAMS = 11000012;									// 设置文字str参数
		static const int UI_DATA_SET_TXT_MULTIPLE_INT_PARAMS = 11000013;									// 设置文字QList<int>参数

		static const int UI_DATA_SET_PARAGRAPH_INDENT_PARAMS = 11000020;						// 设置段落缩进参数:inL,inR,indentType,indentValue
		static const int UI_DATA_SET_PARAGRAPH_INT_PARAMS = 11000021;						// 设置段落int参数:左中右对齐方式
		static const int UI_DATA_SET_PARAGRAPH_FLOAT_PARAMS = 11000022;						// 设置段落float参数:行间距
		static const int SET_SHORTCUT_KEY = 11000023;					// 快捷键信号类

		/******************************* 对象content数据：11100000-11199999 ************************************/
		/******************************* temp对象content数据：11100000-11100999 ************************************/
		static const int ROOT_TEMP_DATA = 11100000;									// 根temp数据
		static const int CODE_TEMP_DATA = 11100001;									// 带codeID的temp
		static const int TITLE_BOTTOM_TEMP_DATA = 11100011;									// 页眉、页脚temp数据
		static const int CONTENT_TEMP_DATA = 11100012;									// 页内容temp数据
		//static const int TITLE_BOTTOM_TEMP_DATA = 11100013;									// 页眉、页脚或页内容下1级子temp）
		static const int TEMP_PARAGRAPH_MGR_DATA = 11100020;									// 常规带段落的数据（页眉、页脚或页内容下1级子temp）
		static const int PARAGRAPH_DATA = 11100100;									// 段落数据

		/******************************* temp对象id数据：11101000-11101999 ************************************/
		static const int BASE_CODE_DATA = 11101000;									// 基本id、name数据
		static const int NODE_DATA = 11101001;									// 节点数据
		static const int PARAGRAPH_CODE_DATA = 11101002;									// 段落id、权限等相关数据
		/****************** temp元素对象content数据：11105000-11105999 ***************************/
		static const int TEXT_WIDGET_PARAM = 11105000;								// 文本元素数据
		static const int STRUCRATION_TEXT_DATA_PARAM = TEXT_WIDGET_PARAM + 1;		//结构化录入元素数据
		static const int MENSTRUATION_DATA_PARAM = STRUCRATION_TEXT_DATA_PARAM + 1;//月经史公式元素数据
		static const int MACRO_DATA_PARAM = MENSTRUATION_DATA_PARAM + 1;			//宏元素数据
		static const int CHOICEAA_WIDGET_PARAM = MACRO_DATA_PARAM + 1;			//选择项数据
		static const int  EXCEL_DATA_PARAM = CHOICEAA_WIDGET_PARAM + 1;		//表格数据
		static const int  EXCEL_CELL_DATA = EXCEL_DATA_PARAM + 1;		//表格单元格数据
		static const int UNFOID_CHOICEAA_WIDGET_PARAM = EXCEL_CELL_DATA + 1;			//展开型选择项数据
		/******************************* view对象content数据：11110000-11119999 ************************************/
		static const int TXT_WIN_DATA = 11110000;									// 文本窗口数据
		static const int IMG_CONTENT_DATA = 11119000;									// 图片数据
		static const int STR_CONTENT_DATA = 11119001;									// 文字数据
		static const int BASE_CONTENT_DATA = 11119100;									// 基本数据

		/******************************* view对象viewParam：11200000-11219999 ************************************/
		static const int BASE_VIEW_PARAM = 11200000;									// 普通显示参数
		static const int LAYOUT_VIEW_PARAM = 11200001;									// layout显示参数
		static const int VERTICAL_LAYOUT_VIEW_PARAM = 11200002;									// 纵向layout显示参数
		static const int ROW_VIEW_PARAM = 11200003;									// 行显示参数

		static const int TXT_VIEW_PARAM = 11210000;									// 文字显示参数
		static const int IMG_VIEW_PARAM = 11210001;									// 图片显示参数
		static const int FLAG_VIEW_PARAM = 11210002;									// 光标显示参数
		static const int EXCEL_VIEW_PARAM = FLAG_VIEW_PARAM + 1;							// 表格单元格显示参数
		static const int IMG_EDIT_PARAM = 11210004;									// 图片编辑参数

		static const int  	HTML_OBJ = IMG_EDIT_PARAM + 1;		//导出的文字对象
		static const int   HTML_MANAGER = HTML_OBJ + 1;		//导出的管理对象
		static const int SET_MEDICAL_RECORD = 11210007;					//设置病程记录

		/******************************* 权限绘制对象属性：11220000-11239999 ************************************/
		static const int AUTHORITY_VIEW_LINE_AND_STR = 11220000;					// 普通权限绘制
	};

	class ContainerId
	{
	public:
		static const int MAX_TEMP_ID = 9999999;

		/******************************* temp：0-8999999 ************************************/
		static const int ROOT_TEMP = 0;											// 根temp
		static const int TITLE_TEMP = 100;											// 页眉temp
		static const int BOTTOM_TEMP = 101;											// 页脚temp
		static const int CONTENT_TEMP = 102;											// 页内容temp

		static const int TITLE_NORMAL_TEMP_CONTAINER = 200;											// 页眉下的1级子temp容器
		static const int BOTTOM_NORMAL_TEMP_CONTAINER = 201;											// 页脚下的1级子temp容器
		static const int CONTENT_NORMAL_TEMP_CONTAINER = 202;											// 页内容下的1级子temp容器
		static const int EXCEL_CONTENT_NORMAL_TEMP_CONTAINER = 212;					// 表格单元格页内容下的1级子temp容器

		static const int NODE_CONTAINER = 300;											// 节点容器

		static const int NORMAL_PARAGRAPH = 9999999;											// 普通段落
		/******************************* view：10000000-19999999 ************************************/
		static const int ROOT_VIEW = 10000000;									// 根view
		static const int TXT_WIN_VIEW = 10000100;									// txt窗口
		static const int PAGE_PANEL = 10000500;									// 页面板
		static const int SCROLL_PANEL = 10000501;									// 拖动面板


		static const int NORMAL_PAGE = 10000600;									// 普通页
		static const int PAGE_TITLE = 10000700;									// 页眉
		static const int PAGE_BOTTOM = 10000701;									// 页脚
		static const int PAGE_CONTENT = 10000702;									// 页内容
		static const int TITLE_BOTTOM_TXT_LAYER = 10000800;									// 页眉、页脚文字层
		static const int CONTENT_TXT_LAYER = 10000801;									// 页内容文字层
		static const int EXCEL_TXT_LAYER = 10000802;									// 表格文字层
		static const int NORMAL_ROW = 10000900;									// 页内容行

		/******************************* 按钮：10100000-10199999 ************************************/
		static const int SCROLL_BUTTON_UP = 10100000;						//滚动条按钮上
		static const int SCROLL_BUTTON_MIDDLE = 10100001;					//滚动条按钮中
		static const int SCROLL_BUTTON_DOWN = 10100002;						//滚动条按钮下
		static const int SCROLL_BUTTON_MIDDLE_BAR = 10100004;					//滚动条按钮中
		/******************************* 具体行上的view对象：18000000-18999999 ************************************/
		static const int ENTER = 18000000;									// 18000000、enter
		static const int TXT = 18000001;									// 18000001、文字
		static const int TMPLATE_TXT = TXT + 1;								// 18000002、元素文字
		static const int TITLE_PAGE_NUMBER = TMPLATE_TXT + 1;						// 18000003、页眉页码
		static const int BOTTOM_PAGE_NUMBER = TITLE_PAGE_NUMBER + 1;						// 18000004、页脚页码
		static const int IMG = 18000005;									// 18000005、图片
		static const int EXCEL_ROW_CONTAINER_VIEW = 18000020;							// 表格包裹行
		static const int EXCEL_PAGE = EXCEL_ROW_CONTAINER_VIEW + 1;                   // 18000021、表格单元格
		static const int EXCEL_VIEW = EXCEL_PAGE + 1;						// 18000022、虚拟表格
		static const int UNFOLD_CHOICE_BOX_RECT_VIEW = EXCEL_VIEW + 1;						// 18000023、展开型选择底框
		static const int UNFOLD_CHOICE_BOX_RADIO_MARK_VIEW = UNFOLD_CHOICE_BOX_RECT_VIEW + 1;						// 18000024、展开型单选标示视图
		static const int UNFOLD_CHOICE_BOX_MULTPLE_MARK_VIEW = UNFOLD_CHOICE_BOX_RADIO_MARK_VIEW + 1;						// 18000025、展开型 多选标示视图
		static const int UNFOLD_CHOICE_BOX_CONTAINER_VIEW = UNFOLD_CHOICE_BOX_MULTPLE_MARK_VIEW + 1;						// 18000026、展开型选择项包裹视图
	//	static const int CHOICE_BOX_MULTPLE_VIEW = CHOICE_BOX_RADIO_VIEW + 1;						// 多选视图
		static const int HORIZONTAL_LINE = 18000030;						// 18000030、横线
		static const int END_PAGE_LINE = 18000031;						// 18000031、分页符
		static const int FLAG = 18000060;									// flag
		static const int TEMPLATE_VIEW_OBJ = FLAG + 1;						//模板视图对象
	
		/******************************* 模板元素：10003000-10005000************************************/
		static const int TEMPLETE_MAX = 10005000;                          //模板元素最大值，
		static const int TXT_Widget = 10003000;								//文本
		static const int BRAKETWIDGET_TEMPLETE = TXT_Widget + 1;					//带括号文本1
		static const int INPUT_TEXT_TEMPLETE = BRAKETWIDGET_TEMPLETE + 1;					//输入文本2
		static const int FIX_TEXT_WIDGET_TEMPLETE = INPUT_TEXT_TEMPLETE + 1;					//固定文本3
		static const int  STRUCTURATION_TEXT_TEMPLETE_OBJ = FIX_TEXT_WIDGET_TEMPLETE + 1;			//结构化文本4
		static const int MACRO_TEXT_TEMPLETE_OBJ = STRUCTURATION_TEXT_TEMPLETE_OBJ + 1;  //宏元素文本5
		static const int TIME_TEXT_WIDGET_TEMPLETE = MACRO_TEXT_TEMPLETE_OBJ + 1;  //时间控件6
		static const int  MENSTRUATION_TEXT_TEMPLETE_OBJ = TIME_TEXT_WIDGET_TEMPLETE + 1;  //公式文本7
		static const int TEXT_WIDGET_TEMPLETE = MENSTRUATION_TEXT_TEMPLETE_OBJ + 1;			//文本元素8
		static const int CHOICE_TEXT_TEMPLETE_OBJ = TEXT_WIDGET_TEMPLETE + 1;			//选择项文本9
		static const int TEXT_WIDGET_AND_NUMBER_FILTER_TEMPLETE = CHOICE_TEXT_TEMPLETE_OBJ + 1; //结构化文本10
		static const int MENSTRUATION_TEMPLETE_OBJ = TEXT_WIDGET_AND_NUMBER_FILTER_TEMPLETE + 1;//月经史公式11
		static const int MACRO_TEMPLETE_OBJ = MENSTRUATION_TEMPLETE_OBJ + 1;		//宏元素12
		static const int  CHOICEWIDGET_TEMPLETE = MACRO_TEMPLETE_OBJ + 1;			// 10003013、选择项13
		//static const int  EXCEL_CELL_TEMPLETE = CHOICEWIDGET_TEMPLETE + 1;    //表格单元格模板14
		static const int TEXT_WIDGET_OF_MENSTRUATION = CHOICEWIDGET_TEMPLETE + 1;//公式外包围14
		static const int  EXCEL_LINE = TEXT_WIDGET_OF_MENSTRUATION + 1;			//表格线15
		static const int   EXCEL_TEMPLETE = EXCEL_LINE + 1;		//表格元素16
		static const int   UNFOLD_CHOICE_BUTTON_TEMPLETE = EXCEL_TEMPLETE + 1;		//展开型项选择项按钮元素17
		static const int   UNFOLD_CHOICE_TEMPLETE = UNFOLD_CHOICE_BUTTON_TEMPLETE + 1;		//展开型项选择项元素18
		static const int UNFOLD_CHOICE_TEXT_TEMPLETE_OBJ = UNFOLD_CHOICE_TEMPLETE + 1;			//展开选择项文本19
		static const int UNFOLD_CHOICE_TITLE_TEMPLETE_OBJ = UNFOLD_CHOICE_TEXT_TEMPLETE_OBJ + 1;			//展开选择项标题20
		static const int NODE_TITLE = 10003500;					//节点标题
		
	};
};


#endif // CONSTANTS_ID_H_
