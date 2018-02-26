#ifndef _SHORTCUTKEY_INPUT_PARAM_H_
#define _SHORTCUTKEY_INPUT_PARAM_H_
#include "BaseUIMsgData.h"
#include "QSize.h"
#include "QMap.h"
class NormalPageTxtWinData;
class WriterManager;
class MyAppEntrance;
class QPrinter;

enum ShortcutType
{
	DoCopy,
	DoCut,
	DoPast,
	Redo,
	Undo,
	DoSelectAll,
	AddImg,
	ResizeEvent,
	DoPrint,
	PrintPreview,
	ShowTidy,
	AddExcel,
	GenerateExcel,
	//SetPageProporty,
	ClearAllData,
	ClearMacroData,
	AddStr,
	GoToMedicalRecord,
	ContinuePrint,
	OutPutPDF
};
class ShortcutKeyInputParam :
	public BaseUIMsgData
{

public:
	ShortcutKeyInputParam(WriterManager * writerMgr, ShortcutType oprateType);
	ShortcutKeyInputParam(WriterManager * writerMgr, MyAppEntrance *appEntrance, ShortcutType oprateType);
	ShortcutKeyInputParam(WriterManager * writerMgr, ShortcutType oprateType, WriterManager * writerManager);
	ShortcutKeyInputParam(WriterManager * writerMgr, ShortcutType oprateType, WriterManager * writerManager, QSize size);
	ShortcutKeyInputParam(WriterManager * writerMgr,  ShortcutType oprateType, QString path);
	~ShortcutKeyInputParam();
public:
	void doAction();
	void initShowTidyAction(bool isTidy);
	void initSetPageProportyAction(QMap<QString, QString>& setMap);
	void initAddString( QString addStr);
	void initGenerateExcel(int row,int col,bool isframe);
	void initContinuePrint(int startParagragh,int endParagraph);
	//记录设置文字属性时，设置的是哪一种属性
	ShortcutType mOprateType;
	//NormalPageTxtWinData *mPageTxtWinData;
	QString mImgPath;
	QSize mSize;
	MyAppEntrance *mAppEntrance;
	bool mIsTidy;
	QMap<QString, QString> mSetMap;
	QString mAddStr;
	int mGenerateRow;
	int mGenerateCol;
	bool mGenerateIsframe;
	int mStartParagragh;
	int mEndParagraph;
	QPrinter* mPrinter;
};
#endif