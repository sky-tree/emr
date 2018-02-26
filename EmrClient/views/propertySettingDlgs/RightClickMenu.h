#pragma once
#ifndef RIGHTCLICKMENU_H
#define RIGHTCLICKMENU_H
#include<QMenu>
#include<QPoint>
#include<QAction>
#include<QList>
#include<QMap>
#include<QString>
#include"views/propertySettingDlgs/attributeClick.h"
#include"MyAppEntrance.h"
#include<QTreeWidget>
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class RightClickMenu :public QMenu
{
	Q_OBJECT
public:
	explicit RightClickMenu(QWidget *parent = 0);
	~RightClickMenu();
	void showMenu(bool isControlClick,QMap<QString,QString> *map,bool isMedical=false);
	
private:
	void show(){}
	void Init();
	void IsShowTidy();
	void addActions(bool isControlClick);
	void addMedicalActions();
	MyAppEntrance* getInstance();
	bool isModify();
	QTreeWidgetItem* getChildItem(QTreeWidgetItem *pItem, QString id);

private:
	bool m_bIsCrusRcd;
	bool m_isMedical;//判断是否是模板进来
	QMenu *m_menu;
	QAction *m_insertFile;
	QAction *m_fixedText;
	QAction *m_keyWord;
	QAction *m_DocumentLine;//文档提纲
	QAction *m_headerAndFooter;
	QAction *m_medicalFormula;//医学公式
	QAction *m_specialFormula;//特殊符号
	QAction *m_radio;//单选
	QAction *m_multiSelect;//多选
	QAction *m_IsSelect;//有无选择
	QAction *m_writeTips;//录入提示
	QAction *m_image;//图像
	QAction *m_Line;//横线
	QAction *m_pageBreak;//分页符
	QAction *m_pageNumber;//页号
	QAction *m_table;
	QAction *m_timerAndDate;
	QAction *m_deleteElement;
	QAction *m_attribute;

	QAction *m_saveEdit;//保存编辑
	QAction *m_giveUpEdit;//放弃编辑
	QAction *m_copy;//复制
	QAction *m_paste;//粘贴
	QAction *m_checkResult;//检查结果
	QAction *m_tidyView;//整洁显示
	QAction *m_currentTime;
	QAction *m_modify;

	QList<QAction *> *m_listActions;

	bool   m_Isattribute;
	QMap<QString, QString> *m_map;
	bool m_bIsEdit;



	private slots:
	void saveEdit();
	void giveUpEdit();
	void copy();
	void paste();
	void checkResult();
	void tidyView(bool isTidy);
	void insertFile();
	void fixedText();
	void keyWord();
	void DocumentLine();//文档提纲
	void headerAndFooter();
	void medicalFormula(); 
	void specialFormula(); 
	void radio();//单选
	void multiSelect();//多
	void IsSelect();//有无选
	void writeTips();//录入
	void image();//图像
	void Line();//横线
	void pageBreak();//分页
	void pageNumber();//页号
	void timerAndDate();
	void InsertTable();//表格
	void deleteElement();
	void attribute();
	void setCurrentTime();

	void modifyTab();
	


};
#endif
