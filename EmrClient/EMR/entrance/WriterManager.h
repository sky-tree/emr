#ifndef WRITER_MANAGER_H_
#define WRITER_MANAGER_H_



//#define DEBUG_MODE
//#define DEBUG_DRAW

#include <QList>
#include <QPainter>
#include"BaseDataType.h"






class MyAppEntrance;
class BaseIOTools;
class LayoutReplaceManager;
class WriterLogicMgr;
class OperateMgr;
class BaseContainer;
class ViewObj;
class BaseTemplate;

class FlagParam;

class FileData;
class QPrinter;
class MousePressParams;
class UIInputManager;
class WriterInputManager;
class MyAppEntrance;
class StrContentData;
class MacroTemplete;
class BaseIOTools;
class htmlBaseObj;
class htmlOutPutManger;
class QScrollArea;
// 书写器对象
class WriterManager:public QObject
{
	Q_OBJECT
public:
	WriterManager(MyAppEntrance* appEntrance, QString fileId, int writeAuthority);
	WriterManager(MyAppEntrance* appEntrance, QDataStream * stream, QString fileId, int writeAuthority);
	~WriterManager();

public:
	// 调试时候，用于打开本地文件的函数
	void doLoad();

	void doLoadByteArray(QByteArray& byteArray);

	void doInsertRecordByteArray(QByteArray& byteArray, QString code, bool isCreate);
	//void loadDataOnly(bool isLoadFromIoTools);
private:

private:

public:
	FileData * getFileData();
	char getFileType();
	//int getFileAuthority();
	bool isFileReadOnly();
	ViewObj * getRootView();
	BaseTemplate * getRootTemp();
	void clearRootTempAndView();
	void clearView();
	BaseContainer* getContantTemp();
	WriterInputManager * getInput();
	UIInputManager * getUIInput();
	BaseIOTools * getIOTools();
	MyAppEntrance * getAppEntrance();
	LayoutReplaceManager * getLayoutReplaceMgr();
	WriterLogicMgr * getLogicMgr();

	OperateMgr * getOperateMgr();
	void checkIsNeedUndo();
	//void setPreFocus();
	void setWindowPressState(short status, bool circleChildren=false);
public:
	QList<ViewObj *> * getTempContainerList();
	//往书写器加入显示对象
	void addContainerToContentTemp(BaseContainer* addContainer,int index = -1);
	ViewObj * getFlag();
	void getFocusObjAndDir(ViewObj* & focusObj, char & dir);
	StrContentData * getFocusTxtData();
	void savePressStartAndEnd(char saveType = 0); //0 表示都保存，1表示保存 start，2表示end 
	void backSetPressStartAndEnd(bool freshChooseList, char saveType = 0);
	//QList<ViewObj *> * getTempViewList();
	QList<BaseContainer *> * getClearTempParagraphViewList();
	QList<QList<BaseContainer *> *> * getTempParagraphSeperateViewList();
	QPainter * getPainter();
	//QFont* getFont();
	int getUseId();
public:
	//void refreshWindowWH(int w, int h);
	//宏元素相关
	void setMacroData(PatientData* data);
	void addMacro(MacroTemplete* macroTempleteObj);
	void removeMacro(MacroTemplete* macroTempleteObj);
	void assignmentMacro(MacroTemplete* maroObj, bool isAddToList ,bool isShowed);
	void reAssignmentMacro();
	void clearMacroData();
	void deleteMacro(MacroTemplete* macroTempleteObj);
public:
	//void unDo();
	//void reDo();
	//void doCut();
	//void doCopy();
	//void doPast(QList<BaseContainer *> * copyList, bool needAddAction);
	//设置页面属性
	//bool sendSetPagePropertyDataSignal(QMap<QString, QString>& setMap);
	//void setPageProperty();
	//bool setPagePropertyData(QMap<QString, QString>& setMap);

	void doOnlyPrint(QPrinter *printer);
signals:
	void clearMacroOver();
public	slots:
	void doPrint(QPrinter *printer);
public:
	void printPreviewOver();
	void doPrintPreview();//打印预览
	void outputPDF();
	void printPreview();
	void sendPrintSign(QPrinter *printer);
	void print();//打印
	void sendContinuingPrintSign(int startParagraph, int endParagraph);
	void continuingPrint(int startParagraph, int endParagraph);//续打
	// 根据起始和结束段落号获取起始及结束段之间的行
	bool getRowListBetweenStartAndEnd(int startParagraphIndex, int endParagraphIndex, bool needGoThrough, QList<ViewObj *> * sourceRowList);
	void showTidy( bool tidy, bool needAddAction);
	bool isOnShowTidy();
	bool haveUIIputNotHandle();
	void handleUIInput();
public:
	// 全局逻辑
	void myUpdate();

	// 全局绘制
	void draw(QPainter &pPainter);
public:
	// 初始化
	void init(BaseIOTools * iOTolls);
	void initRootTempAndView(BaseTemplate * temp, ViewObj * view);
	void doGetHtml(QList<htmlBaseObj*> * resultList);
	void getStr(QString &str); //获取文字

	void keyPressOnRootView(QString str);
protected:
	bool eventFilter(QObject *target, QEvent *event);
private:
	OperateMgr * mOperateMgr;
	LayoutReplaceManager * mLayoutReplaceMgr;
	WriterLogicMgr * mLogicMgr;
	// 根逻辑对象
	BaseTemplate * mRootTemp;
	// 最基本的绘制对象，系统能操作的唯一绘制根对象
	ViewObj * mRootView;

	MyAppEntrance * mAppEntrance;

	FileData * mFileData;

	BaseIOTools * mIOTools;

	// 全局输入管理，将鼠标和按键输入缓存，统一在myUpdate里进行处理
	WriterInputManager * mWriterInput;
	// 全局UI消息输入管理，统一在myUpdate里进行处理
	UIInputManager * mUIInputMgr;

	// 临时用的对象列表
	//QList<ViewObj *> * mTempViewList;
	// 段落添加view时临时用的viewList
	QList<BaseContainer *> * mTempParagraphViewList;
	// 段落添加view时临时用的待添加按enter分割后的viewList
	QList<QList<BaseContainer *> *> * mTempParagraphSeperateAllViewChildren;

	//宏元素相关
	QList<MacroTemplete*> *mMacroTempleteList;
	PatientData* mPatientData;
	htmlOutPutManger *mHtmlOutPutManager;
	QScrollArea * mAreaWidget;
};

#endif
