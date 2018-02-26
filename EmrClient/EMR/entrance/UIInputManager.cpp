#include "UIInputManager.h"

#include <QDebug>




#include "BaseUIMsgData.h"


#include "WriterManager.h"
#include "OperateMgr.h"
#include "SystemParams.h"

#include "BaseViewParam.h"
#include "TempParagraphMgrData.h"

#include "BaseTemplate.h"
#include "ViewObj.h"
#include "ObjKeyManager.h"
#include "ObjMouseManager.h"

#include "ParagraphData.h"
#include "StrContentData.h"

#include "UIMsgSetTxtBoolParams.h"
#include "UIMsgSetTxtIntParams.h"
#include "UIMsgSetTxtMultipleIntParams.h"
#include "UIMsgSetTxtStrParams.h"
#include "UIMsgFindAndReplaceParams.h"

#include "UIMsgSetParagraphIntParams.h"
#include "UIMsgSetParagraphFloatParams.h"
#include "UIMsgSetParagraphIndentParams.h"

#include "UIMsgSetSysSignalParams.h"
#include "UIMsgSetSysBoolParams.h"
#include "UIMsgSetSysIntParams.h"
#include "UIMsgSetSysStrParams.h"
#include "UIMsgSetPageParams.h"
#include "UIMsgMedicalRecord.h"

#include "SetAutorityInput.h"
#include "MyAppEntrance.h"
#include "BaseConstants.h"
#include "QEventLoop.h"
#include "GlobalTemplate.h"
#include "ShortcutKeyInputParam.h"
#include "CompressTool.h"
UIInputManager::UIInputManager(WriterManager * writer) :QObject()
, mIsHandlingInputList(false)
{
	mWriter = writer;
	mOperateMgr = mWriter->getOperateMgr();
	mCurrentIndex = -1;
	mUIMsgList = new QList<BaseUIMsgData *>();
}


UIInputManager::~UIInputManager()
{
	if (mUIMsgList)
	{
		deleteVector(mUIMsgList);
		mUIMsgList = NULL;
	}
	mOperateMgr = NULL;
	mWriter = NULL;
}
void UIInputManager::addMsg(BaseUIMsgData * msg, bool removeSameType)
{
	if (removeSameType)
	{
		removeSameTypeMsg(msg);
	}
	mUIMsgList->push_back(msg);
}
void UIInputManager::removeSameTypeMsg(BaseUIMsgData * source)
{
	int classID = source->getClassId();
	//int size = mUIMsgList->size();
	BaseUIMsgData * data = NULL;
	for (int i = 0; i < mUIMsgList->size(); i++)
	{
		data = mUIMsgList->at(i);
		if (data->getClassId() == classID)
		{
			mUIMsgList->removeAt(i);
			delete data;
			i--;
		}
	}
}
bool UIInputManager::haveUIIputNotHandle()
{
	if (mUIMsgList->size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void UIInputManager::clearMsg(int haveDoneIndex)
{
	int size = mUIMsgList->size();
	if (size > 0)
	{
		for (int i = haveDoneIndex; i >= 0; i--)
		{
			BaseUIMsgData * msg = mUIMsgList->takeAt(i);
			delete msg;
		}
	}
}


/************************************************ 系统属性设置 开始 ***************************************************/
ParagraphData * UIInputManager::getFocusParagraphData()
{
	return mOperateMgr->getFocusParagraphData();
}

bool UIInputManager::setTitleView(bool isView)
{
	UIMsgSetSysBoolParams * msg = new UIMsgSetSysBoolParams(mWriter, BaseConstants::SetSystemParamType::SET_TITLE_VIEW_TYPE, isView);
	addMsg(msg);
	return true;
}
bool UIInputManager::setBottomView(bool isView)
{
	UIMsgSetSysBoolParams * msg = new UIMsgSetSysBoolParams(mWriter, BaseConstants::SetSystemParamType::SET_BOTTOM_VIEW_TYPE, isView);
	addMsg(msg);
	return true;
}
bool UIInputManager::setTitleNumViewType(char type)
{
	UIMsgSetSysIntParams * msg = new UIMsgSetSysIntParams(mWriter, BaseConstants::SetSystemParamType::SET_TITLE_NUMBER_VIEW_TYPE, type);
	addMsg(msg);
	return true;
}
bool UIInputManager::setBottomNumViewType(char type)
{
	UIMsgSetSysIntParams * msg = new UIMsgSetSysIntParams(mWriter, BaseConstants::SetSystemParamType::SET_BOTTOM_NUMBER_VIEW_TYPE, type);
	addMsg(msg);
	return true;
}
bool  UIInputManager::setParagraphNumView(bool isView)
{
	UIMsgSetSysBoolParams * msg = new UIMsgSetSysBoolParams(mWriter, BaseConstants::SetSystemParamType::SET_PARAGRAPH_NUM_VIEW_TYPE, isView);
	addMsg(msg);
	return true;
}
void UIInputManager::putoutPDF()
{
	ShortcutKeyInputParam * shortcutKeyParam = new ShortcutKeyInputParam(mWriter, ShortcutType::OutPutPDF);
	addMsg(shortcutKeyParam);
}
bool UIInputManager::getTitleView()
{
	return mOperateMgr->getSystemParams()->isTitleView();
}
bool UIInputManager::getBottomView()
{
	return mOperateMgr->getSystemParams()->isBottomView();
}
char UIInputManager::getTitleNumViewType()
{
	return mOperateMgr->getSystemParams()->getTitleNumViewType();
}
char UIInputManager::getBottomNumViewType()
{
	return mOperateMgr->getSystemParams()->getBottomNumViewType();
}
bool UIInputManager::getParagraphNumView()
{
	return mOperateMgr->getSystemParams()->isParagraphNumView();
}
bool UIInputManager::setPageParams(bool immediately, bool isHorizontal, short paperType, int insideT, int insideB, int insideL, int insideR
	, int titleMaxH, int bottomMaxH)
{
	if (immediately)
	{
		mOperateMgr->setPageParams(true, isHorizontal, paperType, insideT, insideB, insideL, insideR, titleMaxH, bottomMaxH);
	}
	else
	{
		UIMsgSetPageParams * msg = new UIMsgSetPageParams(mWriter, isHorizontal, paperType, insideT, insideB, insideL, insideR, titleMaxH, bottomMaxH);
		addMsg(msg);
	}
	return true;
}
void UIInputManager::getPageParams(bool &isHorizontal, short &paperType, int &insideT, int &insideB, int &insideL, int &insideR, int &titleMaxH, int &bottomMaxH)
{
	mOperateMgr->getPageParams(isHorizontal, paperType, insideT, insideB, insideL, insideR, titleMaxH, bottomMaxH);
}
//bool UIInputManager::setPaperType(short type)
//{
//	return mOperateMgr->setPaperType(type, true);
//}
void UIInputManager::selectAll()
{
	UIMsgSetSysSignalParams * msg = new UIMsgSetSysSignalParams(mWriter, BaseConstants::SetSystemParamType::SELECT_ALL);
	addMsg(msg);
}
bool UIInputManager::isShowTidy()
{
	return mWriter->isOnShowTidy();
}
bool UIInputManager::setFileAuthority(bool isImmediately, char authority)
{
	UIMsgSetSysIntParams * msg = new UIMsgSetSysIntParams(mWriter, BaseConstants::SetSystemParamType::SET_FILE_AUTHORITY, authority);
	if (isImmediately)
	{
		msg->doAction();
		delete msg;
	}
	else
		addMsg(msg);
	return true;
}
// 打印预览
bool UIInputManager::printPreView()
{
	mWriter->printPreview();
	return true;
}
// 打印
bool UIInputManager::doPrint()
{
	mWriter->sendPrintSign(NULL);
	return true;
}
void UIInputManager::sendContinuingPrintSign(int startParagraph, int endParagraph)
{
	mWriter->sendContinuingPrintSign(startParagraph, endParagraph);
}
// 添加日期
bool UIInputManager::addCurrentTime()
{
	UIMsgSetSysSignalParams * msg = new UIMsgSetSysSignalParams(mWriter, BaseConstants::SetSystemParamType::ADD_CURRENT_TIME);
	addMsg(msg);
	return true;
	return true;
}
bool UIInputManager::doCopy(MyAppEntrance* wedgit)
{
	//UIMsgSetSysSignalParams * msg = new UIMsgSetSysSignalParams(mWriter, BaseConstants::SetSystemParamType::DO_COPY);
	//addMsg(msg);
	QKeyEvent *leftKey = new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier);
	QCoreApplication::postEvent(wedgit, leftKey);
	return true;
}
bool UIInputManager::doCut(MyAppEntrance* wedgit)
{
	//UIMsgSetSysSignalParams * msg = new UIMsgSetSysSignalParams(mWriter, BaseConstants::SetSystemParamType::DO_CUT);
	//addMsg(msg);
	QKeyEvent *leftKey = new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::ControlModifier);
	QCoreApplication::postEvent(wedgit, leftKey);
	return true;
}
bool UIInputManager::doPast(MyAppEntrance* wedgit)
{
	//UIMsgSetSysSignalParams * msg = new UIMsgSetSysSignalParams(mWriter, BaseConstants::SetSystemParamType::DO_PAST);
	//addMsg(msg);
	QKeyEvent *leftKey = new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier);
	QCoreApplication::postEvent(wedgit, leftKey);
	return true;
}
bool UIInputManager::doUndo()
{
	UIMsgSetSysSignalParams * msg = new UIMsgSetSysSignalParams(mWriter, BaseConstants::SetSystemParamType::UNDO);
	addMsg(msg);
	return true;
}
bool UIInputManager::doRedo()
{
	UIMsgSetSysSignalParams * msg = new UIMsgSetSysSignalParams(mWriter, BaseConstants::SetSystemParamType::REDO);
	addMsg(msg);
	return true;
}
void UIInputManager::doSaveAndCompress(QByteArray &ba) //获取压缩过得ByteArray
{
	QByteArray saveData;
	doSave(saveData);
	CompressTool::compressByteArray(saveData, ba);
}
QByteArray UIInputManager::getByteArrayAndCompress(bool needCloseWriteAuthority, bool isFromRightMenu) //获取压缩过得ByteArray
{
	QByteArray saveData = getByteArray(needCloseWriteAuthority, isFromRightMenu);
	QByteArray  byarry;
	CompressTool::compressByteArray(saveData, byarry);
	return byarry;
}
void UIInputManager::doSave(QByteArray & byarry)
{
	ViewObj * focus = NULL;
	char dir = 0;
	ViewObj * pagePanel = mOperateMgr->getPagePanel();
	BaseViewParam * pagePanelViewParam = pagePanel->getViewParam();
	float pagePanelY = pagePanelViewParam->getY();
	mOperateMgr->getFocusAndDirection(focus, dir);
	mOperateMgr->doSaveToByteArry(byarry, true);

	pagePanelViewParam->setY(pagePanelY);
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, dir);
	mOperateMgr->changeStatus(BaseConstants::OperateStatus::FREE);
	//return mOperateMgr->getByteArray();
	//UIMsgSetSysSignalParams * msg = new UIMsgSetSysSignalParams(mWriter, BaseConstants::SetSystemParamType::DO_SAVE);
	//addMsg(msg);
}
QByteArray UIInputManager::getByteArray(bool needCloseWriteAuthority, bool isFromRightMenu)
{
	ViewObj* focus = NULL;
	char direction = 0;
	BaseViewParam * pagePanelViewParam = NULL;
	int pagePanelY = 0;
	mOperateMgr->getFocusAndDirectionAndPagePanY(focus, direction, pagePanelViewParam, pagePanelY);

	if (isFromRightMenu)
	{
		mWriter->clearMacroData();
	}
	else
	{
		clearMacroData();
	}

	QByteArray  byarry;
	mOperateMgr->changeStatus(BaseConstants::OperateStatus::SAVING);
	mOperateMgr->doSaveToByteArry(byarry, needCloseWriteAuthority);
	mWriter->reAssignmentMacro();

	pagePanelViewParam->setY(pagePanelY);
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, direction);

	if (!isFromRightMenu)
	{
		mWriter->getAppEntrance()->timerResume();
	}
	mOperateMgr->changeStatus(BaseConstants::OperateStatus::FREE);

	return byarry;
}
void UIInputManager::clearMacroData()
{
	ShortcutKeyInputParam * shortcutKeyParam = new ShortcutKeyInputParam(mWriter, ShortcutType::ClearMacroData);
	mWriter->getUIInput()->addMsg(shortcutKeyParam);
	QEventLoop eventLoop;
	connect(mWriter, SIGNAL(clearMacroOver()), &eventLoop, SLOT(quit()));
	eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
}
bool UIInputManager::addSpecialTxt(QString txt)
{
	UIMsgSetSysStrParams * msg = new UIMsgSetSysStrParams(mWriter, BaseConstants::SetSystemParamType::ADD_SPECIAL_TXT, txt);
	addMsg(msg);
	return true;
}
bool UIInputManager::addTxtFromOutside(QString txt)
{
	UIMsgSetSysStrParams * msg = new UIMsgSetSysStrParams(mWriter, BaseConstants::SetSystemParamType::COPY_TXT_FROM_OUTSIDE, txt);
	addMsg(msg);
	return true;
}
bool UIInputManager::addImg(QString imgPath)
{
	UIMsgSetSysStrParams * msg = new UIMsgSetSysStrParams(mWriter, BaseConstants::SetSystemParamType::ADD_IMG, imgPath);
	addMsg(msg);
	return true;
}
bool UIInputManager::setRowBottomLineType(char type)
{
	return mOperateMgr->setRowBottomLineType(type, true, false);
}
char UIInputManager::getRowBottomLineType()
{
	return mOperateMgr->getRowBottomLineType();
}
bool UIInputManager::addHorizontalLine()
{
	return mOperateMgr->addHorizontalLine();
}
bool UIInputManager::addEndPageLine()
{
	return mOperateMgr->addEndPageLine();
}
/************************************************ 系统属性设置 结束 ***************************************************/

void UIInputManager::setTitleCanWrite(bool canWrite)
{
	mOperateMgr->setTitleOrBottomCanWrite(ConstantsID::ContainerId::TITLE_TEMP, canWrite);
}
void UIInputManager::setBottomCanWrite(bool canWrite)
{
	mOperateMgr->setTitleOrBottomCanWrite(ConstantsID::ContainerId::BOTTOM_TEMP, canWrite);
}
// 设置左中右对齐
bool UIInputManager::setExcelParagraphAlignVerticalType(char alignType)
{
	UIMsgSetParagraphIntParams * msg = new UIMsgSetParagraphIntParams(mWriter, BaseConstants::UIInputType::SET_EXCEL_VERTICAL_ALIGN_TYPE, alignType);
	addMsg(msg);
	return true;
}
/************************************************ 段落属性设置 开始 ***************************************************/
// 设置左中右对齐
bool UIInputManager::setHorizonAlignType(char alignType)
{
	ParagraphData * paragraphData = mOperateMgr->getFocusParagraphData();
	if (!paragraphData->getHorizonAlignType() == alignType)
	{
		return false;
	}

	UIMsgSetParagraphIntParams * msg = new UIMsgSetParagraphIntParams(mWriter, BaseConstants::UIInputType::SET_PARAGRAPH_HORIZON_ALIGN_TYPE, alignType);
	addMsg(msg);
	return true;
}
char UIInputManager::getHorizonAlignType()
{
	return mOperateMgr->getFocusParagraphData()->getHorizonAlignType();
}
bool UIInputManager::setRowDisTimes(float times)
{
	ParagraphData * paragraphData = mOperateMgr->getFocusParagraphData();
	if (paragraphData->getRowDisHTimes() == times)
	{
		return false;
	}
	UIMsgSetParagraphFloatParams * msg = new UIMsgSetParagraphFloatParams(mWriter, BaseConstants::UIInputType::SET_PARAGRAPH_ROW_DIS_TIMES, times);
	addMsg(msg);
	return true;
}
bool UIInputManager::setParagraphIndentParams(int insideLeft, int insideRight, char indentType, int indentValue)
{
	QList<BaseContainer *> * chooseList = mOperateMgr->getChooseList();
	BaseContainer * obj = NULL;
	for (int i = 0; i < chooseList->size(); i++)
	{
		obj = chooseList->at(i);
		if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
		{
			return false;
		}
	}
	ParagraphData * paragraphData = mOperateMgr->getFocusParagraphData();
	if (paragraphData->getInsideLeft() != insideLeft || paragraphData->getInsideRight() != insideRight
		|| paragraphData->getIndentType() != indentType || paragraphData->getIndentValue() != indentValue)
	{
		UIMsgSetParagraphIndentParams * msg = new UIMsgSetParagraphIndentParams(mWriter, insideLeft, insideRight
			, indentType, indentValue);
		addMsg(msg);
		return true;
	}
	return false;
}
int UIInputManager::getParagraphDisTop()
{
	ParagraphData * paragraphData = mOperateMgr->getFocusParagraphData();
	return paragraphData->getParagraphDisTop();
}
int UIInputManager::getParagraphDisBottom()
{
	ParagraphData * paragraphData = mOperateMgr->getFocusParagraphData();
	return paragraphData->getParagraphDisBottom();
}
float UIInputManager::getParagraphRowDisHTimes()
{
	ParagraphData * paragraphData = mOperateMgr->getFocusParagraphData();
	return paragraphData->getRowDisHTimes();
}
int UIInputManager::getParagraphInsideLeft()
{
	ParagraphData * paragraphData = mOperateMgr->getFocusParagraphData();
	return paragraphData->getInsideLeft();
}
int UIInputManager::getParagraphInsideRight()
{
	ParagraphData * paragraphData = mOperateMgr->getFocusParagraphData();
	return paragraphData->getInsideRight();
}
char UIInputManager::getParagraphIndentType()
{
	ParagraphData * paragraphData = mOperateMgr->getFocusParagraphData();
	return paragraphData->getIndentType();
}
int UIInputManager::getParagraphIndentValue()
{
	ParagraphData * paragraphData = mOperateMgr->getFocusParagraphData();
	return paragraphData->getIndentValue();
}
//bool UIInputManager::setParagraphParams(int disTop, int disBottom, int rowDisH, int insideLeft, int insideRight, char indentType, int indentValue)
//{
//	refreshParagraphData();
//	mParagraphData->setParagraphDisTop(disTop);
//	mParagraphData->setParagraphDisTop(disBottom);
//	mParagraphData->setRowDisH(rowDisH);
//	mParagraphData->setInsideLeft(insideLeft, false);
//	mParagraphData->setInsideRight(insideRight, false);
//	mParagraphData->setIndentParam(indentType, indentValue, false);
//	UIMsgSetParagraphParams * msg = new UIMsgSetParagraphParams(mWriter, mParagraphData);
//	addMsg(msg);
//	return true;
//}
///************************************************ 段落属性设置 结束 ***************************************************/

/************************************************ 文字属性设置 开始 ***************************************************/
bool UIInputManager::setFontBold(bool bold)
{
	UIMsgSetTxtBoolParams * msg = new UIMsgSetTxtBoolParams(mWriter, BaseConstants::TxtParamActionType::SET_BOLD, bold);
	addMsg(msg);
	return true;
}
bool UIInputManager::setFontItalic(bool italic)
{
	UIMsgSetTxtBoolParams * msg = new UIMsgSetTxtBoolParams(mWriter, BaseConstants::TxtParamActionType::SET_ITALIC, italic);
	addMsg(msg);
	return true;
}
bool UIInputManager::setFontSize(int size)
{
	UIMsgSetTxtIntParams * msg = new UIMsgSetTxtIntParams(mWriter, BaseConstants::TxtParamActionType::SET_FONT_SIZE, size);
	addMsg(msg);
	return true;
}
bool UIInputManager::setFontColor(int color)
{
	UIMsgSetTxtIntParams * msg = new UIMsgSetTxtIntParams(mWriter, BaseConstants::TxtParamActionType::SET_FONT_COLOR, color);
	addMsg(msg);
	return true;
}
bool UIInputManager::setBackgroundColor(int color)
{
	UIMsgSetTxtIntParams * msg = new UIMsgSetTxtIntParams(mWriter, BaseConstants::TxtParamActionType::SET_BACKGROUND__COLOR, color);
	addMsg(msg);
	return true;
}
bool UIInputManager::setFlowType(char type)
{
	UIMsgSetTxtIntParams * msg = new UIMsgSetTxtIntParams(mWriter, BaseConstants::TxtParamActionType::SET_FLOW_TYPE, type);
	addMsg(msg);
	return true;
}
bool UIInputManager::setFontFamily(QString family)
{
	UIMsgSetTxtStrParams * msg = new UIMsgSetTxtStrParams(mWriter, BaseConstants::TxtParamActionType::SET_FAMILY, family);
	addMsg(msg);
	return true;
}
bool UIInputManager::setUnderlineParams(char type, int value)
{
	QList<int> * valueList = new QList<int>();
	valueList->push_back(type);
	valueList->push_back(value);
	UIMsgSetTxtMultipleIntParams * msg = new UIMsgSetTxtMultipleIntParams(mWriter, BaseConstants::TxtParamActionType::SET_UNDERLINE, valueList);
	addMsg(msg);
	return true;
}
bool UIInputManager::setDelLineParams(char type, int value)
{
	QList<int> * valueList = new QList<int>();
	valueList->push_back(type);
	valueList->push_back(value);
	UIMsgSetTxtMultipleIntParams * msg = new UIMsgSetTxtMultipleIntParams(mWriter, BaseConstants::TxtParamActionType::SET_DEL_LINE, valueList);
	addMsg(msg);
	return true;
}
bool UIInputManager::getFontBold()
{
	StrContentData * data = mOperateMgr->getFocusTxtData();
	return data->getBold();
}
bool UIInputManager::getFontItalic()
{
	StrContentData * data = mOperateMgr->getFocusTxtData();
	return data->getItalic();
}
int UIInputManager::getFontSize()
{
	StrContentData * data = mOperateMgr->getFocusTxtData();
	return data->getPointSize();
}
int UIInputManager::getFontColor()
{
	StrContentData * data = mOperateMgr->getFocusTxtData();
	return data->getFontColor();
}
int UIInputManager::getBackgroundColor()
{
	StrContentData * data = mOperateMgr->getFocusTxtData();
	return data->getBackgroundColor();
}
char UIInputManager::getFontFlowType()
{
	StrContentData * data = mOperateMgr->getFocusTxtData();
	return data->getFlowType();
}
QString UIInputManager::getFontFamily()
{
	StrContentData * data = mOperateMgr->getFocusTxtData();
	return data->getFamily();
}
void UIInputManager::setHandleOverPreparRestart()
{
	clearMsg(mCurrentIndex);
	mIsHandlingInputList = false;
}
/************************************************ 文字属性设置 结束 ***************************************************/

/************************************************ 查找、替换 开始 ***************************************************/
bool UIInputManager::findStr(char type, QString findStr)
{
	UIMsgFindAndReplaceParams * msg = new UIMsgFindAndReplaceParams(mWriter, type, findStr);
	addMsg(msg);
	return true;
}
bool UIInputManager::replaceStr(char type, QString findStr, QString replaceStr)
{
	UIMsgFindAndReplaceParams * msg = new UIMsgFindAndReplaceParams(mWriter, type, findStr, replaceStr);
	addMsg(msg);
	return true;
}
/************************************************ 查找、替换 结束 ***************************************************/

void UIInputManager::myUpdate()
{
	//int size = mUIMsgList->size();
	if (mIsHandlingInputList)
	{

	}
	else
	{
		mIsHandlingInputList = true;

		for (mCurrentIndex = 0; mCurrentIndex < mUIMsgList->size(); mCurrentIndex++)
		{
			BaseUIMsgData * msg = mUIMsgList->at(mCurrentIndex);
			msg->doAction();
			//delete msg;
		}
		clearMsg(--mCurrentIndex);
		mIsHandlingInputList = false;
	}
}
void UIInputManager::closeAllLevelOneTempleteWriteAutority(bool isImmediately)
{
	if (mWriter)
	{
		SetAutorityInput * setAutorityInput = new SetAutorityInput(mWriter, SetAutotityOperateType::CloseAllLevelOneTempleteWriteAutority);
		if (isImmediately)
		{
			setAutorityInput->doAction();
			delete setAutorityInput;
		}
		else
			mWriter->getUIInput()->addMsg(setAutorityInput);
	}

}
void UIInputManager::closeAllLevelOneTempleteWriteAutorityExcept(bool isImmediately, QString codeId)
{
	if (mWriter)
	{
		SetAutorityInput * setAutorityInput = new SetAutorityInput(mWriter, SetAutotityOperateType::CloseAllLevelOneTempleteWriteAutorityExcept, codeId);
		mWriter->getUIInput()->addMsg(setAutorityInput);
	}

}
void UIInputManager::deleteLevelOneTempleteByCodeId(bool isImmediately, QString sourceCode)
{
	if (mWriter)
	{
		SetAutorityInput * setAutorityInput = new SetAutorityInput(mWriter, SetAutotityOperateType::DeleteLevelOneTempleteByCodeId, sourceCode);
		if (isImmediately)
		{
			setAutorityInput->doAction();
			delete setAutorityInput;
		}
		else
			mWriter->getUIInput()->addMsg(setAutorityInput);
	}
}
void UIInputManager::gotoMedicalRecord(bool isImmediately, QString recodeId)
{
	if (mWriter)
	{
		UIMsgMedicalRecord * setAutorityInput = new UIMsgMedicalRecord(mWriter, recodeId, UIMsgMedicalRecord::MedicalRecordType::GotoMedicalRecord);
		if (isImmediately)
		{
			setAutorityInput->doAction();
			delete setAutorityInput;
		}
		else
			mWriter->getUIInput()->addMsg(setAutorityInput);
	}
}
//void UIInputManager::openMedicalRecordEditAuthority(bool isImmediately, QString recodeId)
//{
//	if (mWriter)
//	{
//		UIMsgMedicalRecord * setAutorityInput = new UIMsgMedicalRecord(mWriter, recodeId, UIMsgMedicalRecord::MedicalRecordType::OpenMedicalRecordEditAuthority);
//		if (isImmediately)
//		{
//			setAutorityInput->doAction();
//			delete setAutorityInput;
//		}
//		else
//			mWriter->getUIInput()->addMsg(setAutorityInput);
//	}
//}
void UIInputManager::openMedicalRecordEditAuthorityByFocus(QString & recodeId)
{
	if (mWriter)
	{
		mOperateMgr->activeRecordFromRecordListByFocus(recodeId);
		//UIMsgMedicalRecord * setAutorityInput = new UIMsgMedicalRecord(mWriter, recodeId, UIMsgMedicalRecord::MedicalRecordType::OpenMedicalRecordEditAuthorityByFocus);
		//if (isImmediately)
		//{
		//setAutorityInput->doAction();
		//recodeId = setAutorityInput->getID();
		//delete setAutorityInput;
		//}
		//else
		//	mWriter->getUIInput()->addMsg(setAutorityInput);
	}
}
void UIInputManager::loadOtherMedicalRecordInCompressedData(bool isImmediately, QString id, QByteArray * source)
{
	QByteArray desData1;
	CompressTool::extractByteArray(*source, desData1);
	loadOtherMedicalRecord(isImmediately, id, &desData1);
}
void UIInputManager::loadOtherMedicalRecord(bool isImmediately, QString id, QByteArray * source)
{
	if (mWriter)
	{
		UIMsgMedicalRecord * setAutorityInput = new UIMsgMedicalRecord(mWriter, id, source, UIMsgMedicalRecord::MedicalRecordType::LoadMedicalRecord);
		if (isImmediately)
		{
			setAutorityInput->doAction();
			delete setAutorityInput;
		}
		else
			mWriter->getUIInput()->addMsg(setAutorityInput);
	}
}
void UIInputManager::clearAllData()
{
}