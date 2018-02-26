#ifndef MY_APP_ENTRANCE_H
#define MY_APP_ENTRANCE_H
#pragma once

#include "qwidget.h"
#include "qmainwindow.h"

#include <QTimer>
#include <QKeyEvent>
#include<QPaintEvent>
#include "emrclient.h"
#include "DataBaseControl.h"
class FileData;

class ToolBarClass;


class WriterManager;
class UIInputManager;
class ImgConTentData;
class htmlBaseObj;
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif

// 入口类
class MyAppEntrance :public QMainWindow
{
	Q_OBJECT
public:
	//重载构造函数都需要初始化parent
	//例如:explicit MyAppEntrance(QString str,QWidget *parent=0); 
	explicit MyAppEntrance(QString sourceFileName, int fileType, int authority, QWidget *parent = 0);
	~MyAppEntrance();

	void init(QByteArray * byteArray, PatientData* data);//以前的未压缩数据，初始化调用
	void initInCompressedData(QByteArray * byteArray, PatientData* data);// 当传入的 QByteArray * byteArray 是压缩过得时

	void initTimer();
public:

	WriterManager * getWriterMgr();
	UIInputManager * getUIInputMgr();
	ToolBarClass * getToolbar();

	FileData * getFileData();

	// 当传入的 QByteArray * byteArray 是原始的，未经过压缩过得时，调用insertRecord
	void insertRecord(QByteArray * byteArray, QString code, PatientData* data, bool isCreate);
	// 当传入的 QByteArray * byteArray 是压缩过得时，调用insertRecordInCompressed
	void insertRecordInCompressed(QByteArray * byteArray, QString code, PatientData* data, bool isCreate);

	/************************************** 键盘相关 *********************************************/
	// 键盘按下
	void keyPressEvent(QKeyEvent *keyEvent);
	// 键盘松开
	//void keyReleaseEvent(QKeyEvent *keyEvent);
	// 输入法
	void inputMethodEvent(QInputMethodEvent *);
	/************************************** 键盘相关 *********************************************/

	/************************************** 鼠标相关 *********************************************/ 
	void mousePressEvent(QMouseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	/************************************** 鼠标相关 *********************************************/

	void resizeEvent(QResizeEvent *resizeEvent);

	// 全局绘制
	void paintEvent(QPaintEvent *);

	void OpenEidtImageWindow(ImgConTentData* ImageData);
	void getImageSnap(ImgConTentData* ImageData);
	bool isReadOnly();
	//int getFileAuthority();
private:
	void setFrequency(int frequency);
public:
	void timerPause();
public:
	void doTimePause();

public:
	void timerResume();
	void setTimeStart();
	QPainter * getPainter();
	//QFont* getFont();
private slots:
	void myUpdate();
public :
	//接受数据
	void sendKeyEvent(QString signStr);
	void addExcel();
	//void setPageProperty();
	void showTidy(bool tidy, bool isLoop = true); //整洁显示
	bool isOnShowTidy();
	void getHtml(QList<htmlBaseObj*> * resultList);
	void getStr(QString &str); //获取文字
	//void setMacroData(PatientData* data);
	static void saveByteArrayBeforOpen(QByteArray  data,QString fileName);
private:
	QTimer * mTimer;
	ToolBarClass * mToolBar;
	WriterManager * mWriterManager;
	FileData * mFileData;
	QPainter * mPainter;
	//QFont *mFont;

	int mIsTimePause;
};
#endif

