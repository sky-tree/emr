#include "ShortcutKeyInputParam.h"
#include "BaseConstants.h"
#include "OperateMgr.h"
#include "WriterManager.h"
#include "MyAppEntrance.h"
#include "QEventLoop.h"
#include "TempleteTypeEnumeration.h"
#include "ViewObj.h"
#include "WriterInputManager.h"
#include "WriterManager.h"
#include "SystemParams.h"
#include <QPrinter.h>
#include "UIConstants.h"
ShortcutKeyInputParam::ShortcutKeyInputParam(WriterManager * writerMgr, ShortcutType oprateType)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::SET_SHORTCUT_KEY)
, mAppEntrance(NULL)
, mPrinter(NULL)
{
	mOprateType = oprateType;
}
ShortcutKeyInputParam::ShortcutKeyInputParam(WriterManager * writerMgr, MyAppEntrance *appEntrance, ShortcutType oprateType)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::SET_SHORTCUT_KEY)
, mAppEntrance(NULL)
, mPrinter(NULL)
{
	mAppEntrance = appEntrance;
	mOprateType = oprateType;
}
ShortcutKeyInputParam::ShortcutKeyInputParam(WriterManager * writerMgr, ShortcutType oprateType, WriterManager * writerManager)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::SET_SHORTCUT_KEY)
, mAppEntrance(NULL)
, mPrinter(NULL)
{
	mOprateType = oprateType;
}
ShortcutKeyInputParam::ShortcutKeyInputParam(WriterManager * writerMgr, ShortcutType oprateType, WriterManager * writerManager, QSize size)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::SET_SHORTCUT_KEY)
, mAppEntrance(NULL)
, mPrinter(NULL)
{
	mOprateType = oprateType;
	mSize = size;
}
ShortcutKeyInputParam::ShortcutKeyInputParam(WriterManager * writerMgr, ShortcutType oprateType, QString path)
: BaseUIMsgData(writerMgr, ConstantsID::ClassId::SET_SHORTCUT_KEY)
, mAppEntrance(NULL)
, mPrinter(NULL)
{
	mOprateType = oprateType;
	mImgPath = path;
}

ShortcutKeyInputParam::~ShortcutKeyInputParam()
{

	mAppEntrance = NULL;
	mPrinter = NULL;
}
void ShortcutKeyInputParam::doAction()
{
	/*QList<BaseContainer *> * windows = NULL;
	BaseContainer * window = NULL;
	BaseWinData * winData = NULL;
	if (mPageTxtWinData)
	{
		windows = mPageTxtWinData->getViewOwner()->getViewParent()->getChildren();
		window = windows->at(windows->size() - 1);
		winData = (BaseWinData *)window->getContentData();
	}*/

	switch (mOprateType)
	{
	case ShortcutType::DoCopy:
		mOperateMgr->doCopy();
		break;
	case ShortcutType::DoCut:
		mOperateMgr->doCut();
		break;
	case ShortcutType::DoPast:
		mOperateMgr->doPast();
		break;
	case ShortcutType::Redo:
	case ShortcutType::Undo:
		//windows = mPageTxtWinData->getViewOwner()->getViewParent()->getChildren();
		//window = windows->at(windows->size() - 1);
		
		if (mOprateType == ShortcutType::Undo)
			mOperateMgr->doUndo();
		else
			mOperateMgr->doRedo();
		break;
	case ShortcutType::DoSelectAll:
		//mOperateMgr->doSelectAll();
		break;
	case ShortcutType::AddImg:
		mOperateMgr->addImg(mImgPath);
		break;
	case ShortcutType::ResizeEvent:
		//mOperateMgr->refreshWindowWH(mSize.width(), mSize.height());
		break;
	case ShortcutType::DoPrint:
	{
								  QPrinter printer;
								  mOperateMgr->setPaperTypeBySystemParam(printer);
								  mWriterMgr->doPrint(&printer);
								  // emit mAppEntrance->toPrint();
								  // mAppEntrance->toBlok();
	}

		break;
	case ShortcutType::PrintPreview:
	{
									   mWriterMgr->doPrintPreview();
	}
		break;
	case ShortcutType::ShowTidy:
	{
								   mWriterMgr->showTidy( mIsTidy, true);
								   //emit mAppEntrance->ShowTidyOver();
								   // mAppEntrance->timerPause();
	}
		break;
	case ShortcutType::AddExcel:
	{
								   QMap<QString, QString> qMap;
								   qMap.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::EXCEL);
								   mWriterMgr->getInput()->mouseDrop(0, 0, 0, &qMap);
								 
	}
		break;
	//case ShortcutType::SetPageProporty:
	//	mWriterMgr->setPagePropertyData(mSetMap);
	//	break;
	case ShortcutType::ClearAllData:
		//mOperateMgr->clearAllData();
		break;
	case ShortcutType::ClearMacroData:
		mWriterMgr->clearMacroData();
		emit mWriterMgr->clearMacroOver();
		mWriterMgr->getAppEntrance()->doTimePause();
		break;
	case ShortcutType::AddStr:
	{
								/* if (mWinData)
								 {
									 mWinData->createNewAction();
									 mWinData->addStr(mAddStr);
								 }*/
	}
		break;
	case ShortcutType::GoToMedicalRecord:
	{
										/*	if (mOperateMgr)
											{
												mWinData->createNewAction();
												mAppEntrance->getWriteManager()->doGotoMedicalRecord(mAddStr);
											}*/
	}
		break;
	case ShortcutType::GenerateExcel:
		mOperateMgr->clearChooseList(true);
		mOperateMgr->addExcel(mGenerateRow, mGenerateCol, mGenerateIsframe);
		break;
	case ShortcutType::ContinuePrint:
		mWriterMgr->continuingPrint(mStartParagragh, mEndParagraph);
		break;
	case ShortcutType::OutPutPDF:
		mWriterMgr->outputPDF();
		break;
	}
}
void ShortcutKeyInputParam::initShowTidyAction(bool isTidy)
{
	mIsTidy = isTidy;
}
void ShortcutKeyInputParam::initSetPageProportyAction(QMap<QString, QString>& setMap)
{
	mSetMap = setMap;
}
void ShortcutKeyInputParam::initAddString(QString addStr)
{
	mAddStr = addStr;
}
void ShortcutKeyInputParam::initGenerateExcel(int row, int col, bool isframe)
{
	mGenerateRow = row;
	mGenerateCol = col;
	mGenerateIsframe = isframe;
}
void ShortcutKeyInputParam::initContinuePrint(int startParagragh, int endParagraph)
{
	mStartParagragh = startParagragh;
	mEndParagraph = endParagraph;
}