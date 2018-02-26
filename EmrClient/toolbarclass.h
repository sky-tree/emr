#ifndef TOOLBARCLASS_H
#define TOOLBARCLASS_H

#include <QToolBar>
#include<QMap>
#include "dataModel/DBDataModel/DBManager.h"
#include "dataModel/BaseDataType.h"
#include<QTreeWidgetItem>
#include <QColor>
#include <QIcon>
#include <QToolButton>
#include "version.h"

class StrContentData;
class QAction;
class QMenu;
class QFontComboBox;
class QComboBox;
class MyAppEntrance;
class QActionGroup;
class DataBaseControl;
class DBManager;
class QSqlQueryModel;


#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
Q_DECLARE_METATYPE(QTreeWidgetItem*)
class ToolBarClass : public QToolBar
{
	Q_OBJECT

public:
	ToolBarClass(QWidget *parent=0,int isTemplate=0);
	//static ToolBarClass * getInstance();
	~ToolBarClass();
	enum type
	{
		blod = 1, Italic, TextSize, Underline, TextStyle};
	enum actionType
	{
		//Blod:加粗
		//Italic:倾斜
		//UnderLine 下划线
		//SuperscriptAct 字体上标
		//SubscriptAct 字体下标
		//LeftAlignAct 左对齐
		//CenterAct 居中对齐
		//RightAlignAct 右对齐
		//JustifyAct  两端对齐
		Blod = 1, ItalicAct, UnderLine, SuperscriptAct, SubscriptAct, LeftAlignAct, CenterAct, RightAlignAct, JustifyAct
	};
	public:
	 //文字操作
	QToolBar *fileToolBar;
	QAction *undoAct;//撤销
	QAction *redoAct;//恢复
	QAction *cutAct;//剪切
	QAction *copyAct;//复制
	QAction *pasteAct;//粘贴
    
	//字体操作
	QToolBar *fontToolBar;
	QAction *boldAct; //加粗
	QAction *italicAct;//倾斜
	QAction *underlineAct;//下划线
	QAction *superscriptAct;//字体上标
	QAction *subscriptAct;//字体下标


	//对齐方式
	QToolBar *alignToolBar;
	QAction *leftAlignAct; //左对齐
	QAction *centerAct;    //居中
	QAction *rightAlignAct; //右对齐
	QAction *justifyAct;//两端对齐

	/*2017.09.25 yinlu*/
	QToolBar*			m_pUpDownAlignToolBar;			//上下对齐
	QAction*			m_pUpAlignAct;					//上对齐
	QAction*			m_pCenterAlignAct;				//中对齐
	QAction*			m_pDownAlignAct;				//下对齐


	//数字签名
	QToolBar *finishToolBar;
	QAction *finishAct;//数字签名

	//字体设置
	QToolBar *comboToolBar;
	QAction *colorAct;//字体颜色
	QToolButton* mTbTextColor;//字体颜色
	QToolButton* mTbTextBackgroundColor;//背景颜色
	QFontComboBox *comboFont;//字体样式
	QComboBox *comboSize;//字体大小

	//书写助手
	QToolBar *writeHelpToolBar;
	QAction *writeHelp;
	QMenu *popMenu;

	//书写助手弹出列表
	QAction *icd_10Code;//icd_10
	QAction *drugDataBase;//药品资料库
	QAction *diagnosisTreatment;//常见疾病诊疗规范
	QAction *commonDictionary;//常用字典

	//表格的拆分合并

	QToolBar *tableToolBar;
	QMenu *tablePopMenu;
	QMenu *splitPopMenu;
	QAction *tableOptions;
	QAction *mergeCell;//合并单元格
	QAction *splitCell;//拆分单元格
	QAction *landscapeCell;//横向拆分单元格
	QAction *portraitCell;//竖向拆分单元格

	//病历提交
	QToolBar *submitToolBar;
	QAction * submitCur;

	QAction *giveUpModifyAct;//放弃修改
	QAction *exitAct;//退出

	QToolBar *exitToolBar;

	QAction *printAct;//打印
	QToolBar *printToolBar;


	//保存
	QToolBar *saveToolBar;
	QAction *saveAct;

	MyAppEntrance *pObject;
	bool mCanResetApp;
	QActionGroup *grpAlign;
	QActionGroup *grp;
	QActionGroup*			m_pgroupUpDownAlign;           //文字上下对齐方式

	int isTemplate;

	//数据库操作
	DataBaseControl *m_DataBaseControl;
	DBManager *m_DbManager;
	QSqlQueryModel* model;
	public slots:
	bool eventFilter(QObject *watched, QEvent *event);
private:
	void initPointer();
	void createActions();
	void setFontCombox();
	void setToolBarStyle();
	void createToolBars();
	void va_listToolBars(QToolBar * toolBar,int argc, QAction* ...);
	void createConnectSlots();
private slots:
	void writeHelpClick();
	void textColor();
	void changeFont(int index);
	void printeClick();
	void tableOptionsClick();
	void splitCellClick();
	//插入特殊符号
	void inputParticularSign();
	void landscapeCellClick();
	void portraitCellClick();
	void mergeCellCellClick();
	void dividCellClick(int row, int col);//拆分单元格
	void addCells(int direction); //插入行或列 1 代表上一行，2代表下一行，3代表左一列，4代表右一列
	void deleteRowOrCol(bool isRow);//删除当前行或者列 isRow 为true  时。表示删除行，false表示删除列
	void setExcelProperty(bool isExelCellProperty);//设置表格属性。isExelCellProperty 为true 时。是设置单元格属性
	//放弃修改和退出
	void giveUpModifyClick();
	void exitClick();
	void createColorTb(QToolButton* tb);//创建颜色下拉框并连接信号槽
	QIcon createColorIcon(QColor color);

	void submitMdc();

public:
	/*设置字体大小*/
	void setTextSize(QString &str);
	void InitObjcet(MyAppEntrance *object);
	//type:1:加粗，2：倾斜，3,字体大小，4：下划线，5：字体,str:字体大小，可不填
	void setActionIsChecked(int type, bool isCheck = false, QString str = "");
public slots :
	void textAlign(QAction *a);
	void textStyle(QAction *a);
	void textUpDownAlign(QAction* a);
	void saveFile();
	void finishFile();
	void icd_10CodeFunc();
	void drugDataBaseFunc();
	void diagnosisTreatmentFunc();
	void commonDictionaryFunc();
	void textBold();
	void textItalic();
	void textUnderline();
	void superscrip();
	void subscript();
	void setIsEnable(bool pIsEnable);
	void changeSize(int index);
	void setTextColor();
	void setBackgroundColor();

public slots: 
	//复制粘贴剪切撤销
	static void copyData();
	static void cutData();
	static void pasteData();
	static void undoData();
	static void redoData();
	//void modifyItemData(QTreeWidgetItem *pitem);

public:
	//可变参数
	void setToolBarIsChecked(StrContentData * data, char alignType);
	void setToolBarIsChecked(StrContentData * data, char alignType, char upDownAlignType);
	void setActionsEnabled(bool state);

	void setAlignment(int type);
	void setToolBarStatus(StrContentData& strContenData);
	void setTbtextColorIcon(int actionSeed);//设置文字颜色按钮图片
	void setTbBackgroundColorIcon(int actionSeed);//设置背景颜色按钮图片
	void setSomeActionsEnable(bool isEnabled);
private:
	//static ToolBarClass * instance;
};		 
#endif // TOOLBARCLASS_H
