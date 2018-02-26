#include "WriterManager.h"



#include <QHBoxLayout>
#include <QPushButton.h>
#include "qdatetime.h"
#include "QBuffer.h"
#include <QDebug>

#include "stddef.h"




#include "MyAppEntrance.h"
#include "UIInputManager.h"
#include "WriterInputManager.h"
#include "WriterLogicMgr.h"

#include "BaseLayout.h"

#include "FlagParam.h"
#include "BaseViewParam.h"
#include "FlagViewParam.h"

#include "ViewObj.h"
#include "BaseTemplate.h"
#include "BaseParagraph.h"

#include "ObjDrawManager.h"
#include "TempContainerMgr.h"

#include "OperateMgr.h"


#include "TxtWinData.h"
#include "FileData.h"

#include "LayoutReplaceManager.h"

#include "BaseChildrenManager.h"

#include "ViewFactory.h"
#include "TempleteFactory.h"

#include "ConstantsID.h"
#include "BaseConstants.h"
#include "AuthorityConstants.h"

#include "BaseSystem.h"
#include "BaseIOTools.h"
#include <QMessageBox.h>
#include "BaseDataType.h"
#include "MacroTemplete.h"
#include "TempleteTypeEnumeration.h"
#include "QFileDialog.h"
#include "MessageBox.h"
#include "VerticalViewParam.h"
#include "PageSettingDlg.h"
#include "ShortcutKeyInputParam.h"
#include "SetPagePropertyAction.h"
#include "UIConstants.h"
#include "BaseMath.h"
#include <QPrinter.h>
#include <QScrollArea.h>
#include <QDesktopWidget.h>
#include <QPrintDialog.h>
#include <QPrintPreviewDialog.h>
#include "SetParagraphOtherParamAction.h"
#include "RowLayout.h"
#include "htmlOutPutManger.h"
#include "BaseTools.h"
#include "AuthorityConstants.h"
#include "SystemParams.h"
WriterManager::WriterManager(MyAppEntrance* appEntrance, QString fileId, int writeAuthority) :QObject()
, mRootTemp(NULL)
, mRootView(NULL)
, mAppEntrance(appEntrance)
, mWriterInput(NULL)
, mUIInputMgr(NULL)
, mPatientData(NULL)
, mHtmlOutPutManager(NULL)
, mAreaWidget(NULL)
{
	mFileData = mAppEntrance->getFileData();
	mIOTools = new BaseIOTools(this);
	mLogicMgr = new WriterLogicMgr();
	mOperateMgr = new OperateMgr(this, mIOTools);

	//mTempViewList = new QList<ViewObj *>();
	mTempParagraphViewList = new QList<BaseContainer *>();
	// 段落添加view时临时用的待添加按enter分割后的viewList
	mTempParagraphSeperateAllViewChildren = new QList<QList<BaseContainer *> *>();

	mLayoutReplaceMgr = new LayoutReplaceManager();
	mWriterInput = new WriterInputManager(this);
	mUIInputMgr = new UIInputManager(this);

	//mAuthorityMgr = new AuthorityMgr(fileType, fileId, writeAuthority);

	
	mMacroTempleteList = new QList<MacroTemplete*>();
}

WriterManager::WriterManager(MyAppEntrance* appEntrance, QDataStream * stream, QString fileId, int writeAuthority) :QObject()
, mRootView(NULL)
, mAppEntrance(appEntrance)
, mPatientData(NULL)
, mHtmlOutPutManager(NULL)
, mAreaWidget(NULL)
{
	mFileData = mAppEntrance->getFileData();
	//mTempViewList = new QList<ViewObj *>();
	mLayoutReplaceMgr = new LayoutReplaceManager();
	mWriterInput = new WriterInputManager(this);
	mUIInputMgr = new UIInputManager(this);
	mMacroTempleteList = new QList<MacroTemplete*>();
}


WriterManager::~WriterManager()
{
	if (mHtmlOutPutManager)
	{
		delete mHtmlOutPutManager;
		mHtmlOutPutManager = NULL;
	}
	if (mWriterInput)
	{
		delete mWriterInput;
		mWriterInput = NULL;
	}
	if (mUIInputMgr)
	{
		delete mUIInputMgr;
		mUIInputMgr = NULL;
	}
	if (mLogicMgr)
	{
		delete mLogicMgr;
		mLogicMgr = NULL;
	}

	if (mMacroTempleteList)
	{
		//mMacroTempleteList->clear();

		QList<BaseContainer *> *  copyListTemp = BaseSystem::getInstance()->getCopyList();
		WriterManager* tempWriteManger = NULL;
		if (copyListTemp)
		{
			if (copyListTemp->size()>0)
			{
				tempWriteManger = copyListTemp->at(0)->getWriterMgr();
			}
		}
		MacroTemplete * tempMacroTemp;
		for (int i = 0; i < mMacroTempleteList->size();i++)
		{
			tempMacroTemp = mMacroTempleteList->at(i);
			//copyListTemp->removeAll(tempMacroTemp);
			if (tempWriteManger == this)
			{
				BaseSystem::getInstance()->setCopyListWriteManger(NULL,true);
			}
			tempMacroTemp->setWriteManager(NULL);
		}
		mMacroTempleteList->clear();
		delete mMacroTempleteList;
		mMacroTempleteList = NULL;
	}
	if (mPatientData)
	{
		delete mPatientData;
		mPatientData = NULL;
	}
	if (mIOTools)
	{
		delete mIOTools;
		mIOTools = NULL;
	}
	if (mOperateMgr)
	{
		delete mOperateMgr;
		mOperateMgr = NULL;
	}
	if (mAreaWidget)
	{
		 mAreaWidget = NULL;
	}
	if (mLayoutReplaceMgr)
	{
		delete mLayoutReplaceMgr;
		mLayoutReplaceMgr = NULL;
	}

	if (mTempParagraphViewList)
	{
		mTempParagraphViewList->clear();
		delete mTempParagraphViewList;
		mTempParagraphViewList = NULL;
	}
	if (mTempParagraphSeperateAllViewChildren)
	{
		for (int i = 0; i < mTempParagraphSeperateAllViewChildren->size(); i ++)
		{
			QList<BaseContainer *> * tempList = mTempParagraphSeperateAllViewChildren->at(i);
			tempList->clear();
			delete tempList;
			tempList = NULL;
		}
		mTempParagraphSeperateAllViewChildren->clear();
		delete mTempParagraphSeperateAllViewChildren;
		mTempParagraphSeperateAllViewChildren = NULL;
	}

	mRootView = NULL;
	mRootTemp = NULL;
	mAppEntrance = NULL;
	mFileData = NULL;
}

void WriterManager::doLoad()
{
	//mIOTools = new BaseIOTools(this);
	//mOperateMgr = new OperateMgr(this, mIOTools);

	//bool successful = mIOTools->openReadFileDialog();
	bool successful = mIOTools->openFile(mFileData->getFileName(), false);
	//TempContainerMgr * tempConainerMgr = NULL;
	// 如果为新建
	if (successful)
	{
		mOperateMgr->doLoad1(true);
		//loadDataOnly(true);
	}
	else
	{
		mOperateMgr->doLoad1(false);
		//loadDataOnly(false);
	}
	mIOTools->close();
}
void WriterManager::doLoadByteArray(QByteArray& byteArray)
{
	if (byteArray.size()<=0)
	{
		MessageBox::showAboutMessageBox(QStringLiteral("提示"), QStringLiteral("这是一个消息框"));
		return;
	}
	mIOTools->setByteArray(&byteArray);
	mIOTools->readFile();
	mOperateMgr->doLoad1(true);
	//loadDataOnly(true);
	mIOTools->close();
}
void WriterManager::doInsertRecordByteArray(QByteArray& byteArray, QString code, bool isCreate)
{
	if (byteArray.size() <= 0)
	{
		MessageBox::showAboutMessageBox(QStringLiteral("提示"), QStringLiteral("这是一个消息框"));
		return;
	}
	mIOTools->setByteArray(&byteArray);
	mIOTools->readFile();
	mOperateMgr->doInsertRecord(code, isCreate);
	//loadDataOnly(true);
	mIOTools->close();
}
//void WriterManager::loadDataOnly(bool isLoadFromIoTools)
//{
//}
/***************************************** 初始化窗口 *****************************************/
void WriterManager::init(BaseIOTools * iOTolls)
{
	mIOTools = iOTolls;
	//mRootView = ViewFactory::
	//doLoad(source);
	//doLoad(BaseConstants::MyPath::DIR_RES + mSourceFileName + BaseConstants::MyPath::FILE_SUFFIX);
}
void WriterManager::initRootTempAndView(BaseTemplate * temp, ViewObj * view)
{
	mRootTemp = temp;
	mRootView = view;
}

MyAppEntrance * WriterManager::getAppEntrance()
{
	return mAppEntrance;
}
WriterInputManager * WriterManager::getInput()
{
	return mWriterInput;
}
UIInputManager * WriterManager::getUIInput()
{
	return mUIInputMgr;
}
FileData * WriterManager::getFileData()
{
	return mFileData;
}
char WriterManager::getFileType()
{
	return mFileData->getFileType();
}
//int WriterManager::getFileAuthority()
//{
//	return mFileData->getFileAuthority();
//}
bool WriterManager::isFileReadOnly()
{
	return mFileData->isReadOnly();
}
ViewObj * WriterManager::getRootView()
{
	return mRootView;
}
BaseTemplate * WriterManager::getRootTemp()
{
	return mRootTemp;
}
void WriterManager::clearRootTempAndView()
{
	if (mRootTemp)
	{
		mRootTemp = NULL;
	}
	clearView();
}
void WriterManager::clearView()
{
	if (mRootView)
	{
		mRootView = NULL;
	}
	mLogicMgr->clearRunningList();
}
BaseContainer* WriterManager::getContantTemp()
{
	BaseContainer * contentContainer = mRootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false)
		->findChildrenByID(ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER, false);
	return contentContainer;
}
BaseIOTools * WriterManager::getIOTools()
{
	return mIOTools;
}
LayoutReplaceManager * WriterManager::getLayoutReplaceMgr()
{
	return mLayoutReplaceMgr;
}
WriterLogicMgr * WriterManager::getLogicMgr()
{
	return mLogicMgr;
}
OperateMgr * WriterManager::getOperateMgr()
{
	return mOperateMgr;
}
void WriterManager::checkIsNeedUndo()
{
	mOperateMgr->checkIsNeedUndo();
}
//void WriterManager::setPreFocus()
//{
//	mOperateMgr->setPreFocus();
//}
void WriterManager::setWindowPressState(short status, bool circleChildren)
{
	ViewObj* txtWindow = (ViewObj*)mRootView->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, true);
	txtWindow->changeStatus(status, circleChildren);
}
//QList<ViewObj *> * WriterManager::getTempViewList()
//{
//	return mTempViewList;
//}
QList<BaseContainer *> * WriterManager::getClearTempParagraphViewList()
{
	mTempParagraphViewList->clear();
	return mTempParagraphViewList;
}
QList<QList<BaseContainer *> *> * WriterManager::getTempParagraphSeperateViewList()
{
	return mTempParagraphSeperateAllViewChildren;
}
QPainter * WriterManager::getPainter()
{
	return mAppEntrance->getPainter();
}
//QFont* WriterManager::getFont()
//{
//	return mAppEntrance->getFont();
//}
int WriterManager::getUseId(){
	return mOperateMgr->getUserID();
}
void WriterManager::myUpdate()
{
	//BaseTools::println("WriterManager::myUpdate() 111 ", (int)this);
	//BaseTools::refreshStartTime();
	// 书写器输入处理
	if (mWriterInput)
		mWriterInput->myUpdate();
	//BaseTools::printSpendTime("WriterMgr::myUpdate() 111");
	// UI消息队列处理
	if (mUIInputMgr)
		mUIInputMgr->myUpdate();

	//BaseTools::printSpendTime("WriterMgr::myUpdate() 222");
	mLayoutReplaceMgr->replaceLayout();
	mOperateMgr->refreshEffectLogicPageByDestIndex();
	//BaseTools::printSpendTime("WriterMgr::myUpdate() 333");

	//if (mRootView)
	//{
	//	mRootView->myUpdate();
	//}
	// 对象逻辑
	mLogicMgr->myUpdate();
	//BaseTools::printSpendTime("WriterMgr::myUpdate() 666");

	// 根据光标刷新pagePanel坐标
	mOperateMgr->refreshPagePanelByFocus();

	if (mOperateMgr)
	{
		mOperateMgr->myUpdate();
	}
	//BaseTools::println("WriterManager::myUpdate() 222 ", (int)this);
}
void WriterManager::draw(QPainter &pPainter)
{
	if (!mRootView)
		return;
	//BaseTools::println("WriterManager::draw() 111 ", (int)this);
	long start = BaseTools::refreshStartTime();
	mOperateMgr->clearTotalClip();
	// 绘制窗口
	mRootView->getDrawMgr()->draw(pPainter, 0, 0, false);
	//BaseTools::printSpendTime(start, "WriterMgr::draw() 222");
	//BaseTools::println("WriterManager::draw() 222 ", (int)this);
}
void WriterManager::addContainerToContentTemp(BaseContainer* addContainer, int index)
{
	//BaseContainer * contentRoot = mRootTemp->findChildrenByIDNotCircle(ConstantsID::ContainerId::CONTENT_TEMP);
	BaseContainer * contentRoot = NULL;
	if (index ==-1)
	{
		BaseContainer * lastContainer = contentRoot->findLastContainer();
	}
}
ViewObj * WriterManager::getFlag()
{
	return mOperateMgr->getFlag();
}
void WriterManager::getFocusObjAndDir(ViewObj* & focusObj, char & dir)
{
	mOperateMgr->getFocusAndDirection(focusObj,dir);
}
StrContentData * WriterManager::getFocusTxtData()
{
	return mOperateMgr->getFocusTxtData();
}
void WriterManager::savePressStartAndEnd(char saveType) //0 表示都保存，1表示保存 start，2表示end 
{
	mOperateMgr->savePressStartAndEnd2(saveType);
}
void WriterManager::backSetPressStartAndEnd(bool freshChooseList, char saveType)
{
	mOperateMgr->backSetPressStartAndEnd2(freshChooseList,saveType);
}
void WriterManager::setMacroData(PatientData * data)
{
	if (!mPatientData){
		mPatientData = new PatientData();
	}
	*mPatientData = *data;
	/*int size = mMacroTempleteList->size();
	MacroTemplete * tempMacro;
	int id;
	for (int i = 0; i < size; i++)
	{
	tempMacro = mMacroTempleteList->at(i);
	id = tempMacro->getId();
	try
	{
	switch (id)
	{
	case TempleteTypeEnumeration::MACRO_NAME_ID:
	tempMacro->setContent(data->name);
	break;
	case TempleteTypeEnumeration::MACRO_BIRTHDAY_ID:
	tempMacro->setContent(data->birthday);
	break;
	case TempleteTypeEnumeration::MACRO_CITIZENSHIP_ID:
	tempMacro->setContent(data->citizenship);
	break;
	case TempleteTypeEnumeration::MACRO_BIRTH_PLACE_ID:
	tempMacro->setContent(data->birth_place);
	break;
	case TempleteTypeEnumeration::MACRO_OCCUPATION_ID:
	tempMacro->setContent(data->occupation);
	break;
	case TempleteTypeEnumeration::MACRO_NATION_ID:
	tempMacro->setContent(data->nation);
	break;
	case TempleteTypeEnumeration::MACRO_EDUCATION_ID:
	tempMacro->setContent(data->education);
	break;
	case TempleteTypeEnumeration::MACRO_MARITAL_STATUS_ID:
	tempMacro->setContent(data->marital_status);
	break;
	case TempleteTypeEnumeration::MACRO_MAILING_ADDRESS_ID:
	tempMacro->setContent(data->mailing_address);
	break;
	case TempleteTypeEnumeration::MACRO_ZIP_CODE_ID:
	tempMacro->setContent(data->zip_code);
	break;
	case TempleteTypeEnumeration::MACRO_PHONE_ID:
	tempMacro->setContent(data->phone);
	break;
	case TempleteTypeEnumeration::MACRO_SEX_ID:
	tempMacro->setContent(data->sex);
	break;
	case TempleteTypeEnumeration::MACRO_HOME_ADDRESS_ID:
	tempMacro->setContent(data->home_address);
	break;
	case TempleteTypeEnumeration::MACRO_HOME_ZIP_CODE_ID:
	tempMacro->setContent(data->home_zip_code);
	break;
	case TempleteTypeEnumeration::MACRO_CERTIFICATE_TYPE_ID:
	tempMacro->setContent(data->certificate_Type);
	break;
	case TempleteTypeEnumeration::MACRO_ID_NO_ID:
	tempMacro->setContent(data->id_No);
	break;
	case TempleteTypeEnumeration::MACRO_OUTP_CARD_NO_ID:
	tempMacro->setContent(data->outp_card_no);
	break;
	case TempleteTypeEnumeration::MACRO_INP_NO_ID:
	tempMacro->setContent(data->inp_NO);
	break;
	case TempleteTypeEnumeration::MACRO_ADMISSION_DATE_ID:
	tempMacro->setContent(data->admission_date);
	break;
	case TempleteTypeEnumeration::MACRO_BED_ID_ID:
	tempMacro->setContent(QString(data->bed_id));
	break;
	case TempleteTypeEnumeration::MACRO_DEPT_VALUE_ID:
	tempMacro->setContent(data->dept_Name);
	break;
	case TempleteTypeEnumeration::MACRO_CONTACT_ID:
	tempMacro->setContent(data->contact);
	break;
	case TempleteTypeEnumeration::MACRO_CONTACT_PHONE_ID:
	tempMacro->setContent(data->contact_phone);
	break;
	case TempleteTypeEnumeration::MACRO_CONTACT_RELATION_ID:
	tempMacro->setContent(data->contact_relation);
	break;
	case TempleteTypeEnumeration::MACRO_COMPANY_ID:
	tempMacro->setContent(data->company);
	break;
	case TempleteTypeEnumeration::MACRO_COMPANY_ADDR_ID:
	tempMacro->setContent(data->company_addr);
	break;
	case TempleteTypeEnumeration::MACRO_COMPANY_PHONE_ID:
	tempMacro->setContent(data->company_phone);
	break;
	case TempleteTypeEnumeration::MACRO_DIAGNOSIS_ID:
	tempMacro->setContent(data->diagnosis);
	break;
	case TempleteTypeEnumeration::MACRO_CURRENT_DIAG_NAME_ID:
	tempMacro->setContent(data->current_diag_name);
	break;
	case TempleteTypeEnumeration::MACRO_MR_NO_ID:
	tempMacro->setContent(data->mr_NO);
	break;
	case TempleteTypeEnumeration::MACRO_HOME_PHONE_ID:
	tempMacro->setContent(data->home_phone);
	break;

	}
	}
	catch (...)
	{
	continue;
	}

	}*/
}
void WriterManager::assignmentMacro(MacroTemplete* maroObj, bool isAddToList, bool isShowed)
{
	bool isNeedDeleteAndAddChild = isShowed;
	if (!mPatientData)
	{
		return;
	}
	int	id = maroObj->getId();
	try
	{
		switch (id)
		{
		case TempleteTypeEnumeration::MACRO_NAME_ID:
			maroObj->setContent(mPatientData->name.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_BIRTHDAY_ID:

		{
														   QString timeStrq = mPatientData->birthday;
														   QDate tempDate = QDate::fromString(timeStrq, Qt::DateFormat::ISODate);
														   QString timeFomat = tempDate.toString("yyyy年MM月dd日");
														   maroObj->setContent(timeFomat, isNeedDeleteAndAddChild);
		}
			break;
		case TempleteTypeEnumeration::MACRO_CITIZENSHIP_ID:
			maroObj->setContent(mPatientData->citizenship.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_BIRTH_PLACE_ID:
			maroObj->setContent(mPatientData->birth_place.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_OCCUPATION_ID:
			maroObj->setContent(mPatientData->occupation.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_NATION_ID:
			maroObj->setContent(mPatientData->nation.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_EDUCATION_ID:
			maroObj->setContent(mPatientData->education.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_MARITAL_STATUS_ID:
			maroObj->setContent(mPatientData->marital_status.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_MAILING_ADDRESS_ID:
			maroObj->setContent(mPatientData->mailing_address.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_ZIP_CODE_ID:
			maroObj->setContent(mPatientData->zip_code.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_PHONE_ID:
			maroObj->setContent(mPatientData->phone.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_SEX_ID:
			maroObj->setContent(mPatientData->sex.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_HOME_ADDRESS_ID:
			maroObj->setContent(mPatientData->home_address.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_HOME_ZIP_CODE_ID:
			maroObj->setContent(mPatientData->home_zip_code.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_CERTIFICATE_TYPE_ID:
			maroObj->setContent(mPatientData->certificate_Type.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_ID_NO_ID:
			maroObj->setContent(mPatientData->id_No.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_OUTP_CARD_NO_ID:
			maroObj->setContent(mPatientData->outp_card_no.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_INP_NO_ID:
			maroObj->setContent(mPatientData->inp_NO.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_ADMISSION_DATE_ID:

		{
																 QString timeStrq = mPatientData->admission_date;
																 QDate tempDate = QDate::fromString(timeStrq, Qt::DateFormat::ISODate);
																 QString timeFomat = tempDate.toString("yyyy-MM-dd ");
																 QString time = timeStrq.split("T").at(1);
																 maroObj->setContent(timeFomat + time, isNeedDeleteAndAddChild);
		}
			break;
		case TempleteTypeEnumeration::MACRO_BED_ID_ID:
			maroObj->setContent(/*QString::number(mPatientData->bed_id)*/mPatientData->bedName.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_DEPT_VALUE_ID:
			maroObj->setContent(mPatientData->dept_Name.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_CONTACT_ID:
			maroObj->setContent(mPatientData->contact.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_CONTACT_PHONE_ID:
			maroObj->setContent(mPatientData->contact_phone.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_CONTACT_RELATION_ID:
			maroObj->setContent(mPatientData->contact_relation.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_COMPANY_ID:
			maroObj->setContent(mPatientData->company.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_COMPANY_ADDR_ID:
			maroObj->setContent(mPatientData->company_addr.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_COMPANY_PHONE_ID:
			maroObj->setContent(mPatientData->company_phone.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_DIAGNOSIS_ID:
			maroObj->setContent(mPatientData->diagnosis.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_CURRENT_DIAG_NAME_ID:
			maroObj->setContent(mPatientData->current_diag_name.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_MR_NO_ID:
			maroObj->setContent(mPatientData->mr_NO.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_HOME_PHONE_ID:
			maroObj->setContent(mPatientData->home_phone.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_AGE_ID:
			maroObj->setContent(mPatientData->age.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_HOSPITALIZED_TIMES_ID:
			maroObj->setContent(mPatientData->hopspitalized_Times.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_PATIENT_COST_CODE_ID:
			maroObj->setContent(mPatientData->patiemt_Cost_Code.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_FROM_CODE_ID:
			maroObj->setContent(mPatientData->from_Code.trimmed(), isNeedDeleteAndAddChild);
			break;
		case TempleteTypeEnumeration::MACRO_DOCTOR_ID:
			maroObj->setContent(mPatientData->doctorName.trimmed(), isNeedDeleteAndAddChild);
			break;
		}
	}
	catch (...)
	{

	}
	if (isAddToList)
	{
		addMacro(maroObj);
	}
}
void WriterManager::reAssignmentMacro()
{
	if (mMacroTempleteList)
	{
		MacroTemplete* maroObj;
		ViewObj* parentView = NULL;
		ViewObj * window = (ViewObj *)mRootView->findChildrenByID
			(ConstantsID::ContainerId::TXT_WIN_VIEW, false);
		BaseWinData * winData = (BaseWinData *)window->getContentData();
		ViewObj * txtLayer = NULL;
		int idx = -1;
		for (int i = 0; i < mMacroTempleteList->size();)
		{
			maroObj = mMacroTempleteList->at(i);
			ViewObj* first = maroObj->getFirstChildView();
			if (first)
			{
				parentView = first->getViewParent();
				txtLayer = parentView->getViewParent();
				//winData->addToReplaceViewList(txtLayer);
				int index = first->getViewIndex();
				if (index >= 0)
				{
					idx = parentView->getViewIndex();
					if (idx >= 0)
					{
						assignmentMacro(maroObj, false,true);
						i++;
						continue;
					}
				}
			}
			mMacroTempleteList->removeAt(i);
		}
		//winData->replaceNeedReplaceList();
		mLayoutReplaceMgr->replaceLayout();
	}
}
void WriterManager::addMacro(MacroTemplete* macroTempleteObj)
{
	if (mMacroTempleteList)
	{
		int index = mMacroTempleteList->indexOf(macroTempleteObj);
		if (index<0)
		{
			mMacroTempleteList->push_back(macroTempleteObj);
		}
	}
}
void WriterManager::removeMacro(MacroTemplete* macroTempleteObj)
{
	if (mMacroTempleteList)
	{
		mMacroTempleteList->removeAll(macroTempleteObj);
	}
}
void WriterManager::clearMacroData()
{
	/*ViewObj* focusObj = NULL;
	char focusDirect = -1;
	mOperateMgr->getFocusAndDirection(focusObj, focusDirect);*/
	bool isNotInEditTemplate = true;
	if (mOperateMgr)
	{
		if (mOperateMgr->getFileData()->getFileType() == AuthorityConstants::FileType::TEMPLETE)
		{
			isNotInEditTemplate = false;
		}
	}
	if (mMacroTempleteList)
	{
		MacroTemplete * templeteMacro;
		ViewObj* panelView = NULL;
		ViewObj* txtLayer = NULL;
		ViewObj * window = (ViewObj *)mRootView->findChildrenByID
			(ConstantsID::ContainerId::TXT_WIN_VIEW, false);
		BaseWinData * winData = (BaseWinData *)window->getContentData();
		for (int i = 0; i < mMacroTempleteList->size();)
		{
			templeteMacro = mMacroTempleteList->at(i);
			if (templeteMacro)
			{
				int indexOfTemplate = -1;
				BaseContainer* rootTemp = templeteMacro->findTempFromParent(ConstantsID::ContainerId::ROOT_TEMP, true, indexOfTemplate);
				ViewObj* first = templeteMacro->getFirstChildView();
				if (first)
				{
					panelView = first->findViewFromParent(ConstantsID::ContainerId::PAGE_PANEL, true);
					BaseContainer* baseTempContainer = templeteMacro->findTempFromParent(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER, true);
					if (baseTempContainer)
					{
						BaseContainer* excelTemplate = templeteMacro->findTempFromParent(ConstantsID::ContainerId::EXCEL_TEMPLETE, true);
						if (excelTemplate)
						{

							BaseContainer* nomarlParent = excelTemplate->getTempParent();
							if (nomarlParent)
							{
								if (nomarlParent->haveChild(excelTemplate))
								{
									ViewObj* excelRowContainer = first->findViewFromParent(ConstantsID::ContainerId::PAGE_PANEL, true);
									if (excelRowContainer)
									{
										if (excelRowContainer->getViewParent() && isNotInEditTemplate&&rootTemp)
										{
											templeteMacro->setContent(BaseConstants::MACRO_DEFAULT_CONTENT_STR, true, false);
											i++;
										}
										else
										{
											mMacroTempleteList->removeAt(i);
										}
									}
									else
									{
										mMacroTempleteList->removeAt(i);
									}
								}
								else
								{
									mMacroTempleteList->removeAt(i);
								}
								
							}
							else
							{
								mMacroTempleteList->removeAt(i);
							}
							
						}
						else
						{
							mMacroTempleteList->removeAt(i);
						}
						
					}
					else
					{
						if (panelView&&isNotInEditTemplate&&rootTemp)
						{
							txtLayer = panelView->getViewParent();
							//winData->addToReplaceViewList(txtLayer);
							int index = first->getViewIndex();
							if (index >= 0)
							{
								int idx = panelView->getViewIndex();
								if (idx >= 0)
								{
									templeteMacro->setContent(BaseConstants::MACRO_DEFAULT_CONTENT_STR, true);
								}
							}
							i++;
						}
						else
						{
							mMacroTempleteList->removeAt(i);
						}
					}
					
					
				}
				
			}
			else
			{
				mMacroTempleteList->removeAt(i);
			}
		}
	}
	//mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE,focusObj,focusDirect,true);
}
void WriterManager::deleteMacro(MacroTemplete* macroTempleteObj)
{
	if (mMacroTempleteList)
	{
		mMacroTempleteList->removeAll(macroTempleteObj);
	}
}
//void WriterManager::setPageProperty()
//{
//	int index = 0;
//	ViewObj * pagePan = (ViewObj *)mRootView->findContainerFromChildren(ConstantsID::ContainerId::PAGE_PANEL, index);
//	QList<BaseContainer *> * childrenBase = pagePan->getChildren();
//	QList<ViewObj *> * children = (QList<ViewObj *>*)childrenBase;
//	QMap<QString, QString> map;
//	QMap<QString, QString> preMap;
//	if (!children || children->size() == 0)
//	{
//		return;
//	}
//
//	ViewObj * firstPage = children->at(0);
//	ViewObj * content = (ViewObj *)firstPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false);
//
//	ViewObj * contentTxtLayer = (ViewObj *)content->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
//
//	VerticalViewParam * contentTxtParam = (VerticalViewParam *)contentTxtLayer->getViewParam();
//
//	/*int topPadding = mWinData->getPageContentDisTop();
//	int bottomPadding = mWinData->getPageContentDisBottom();
//	int leftPadding = mWinData->getPageContentDisLeft();
//	int rightPadding = mWinData->getPageContentDisRight();
//	int headerTopPadding = mWinData->getPageTitleDisTop();
//	int footerBottomPadding = mWinData->getPageBottomDisBottom();
//	map.insert("topPadding", QString::number(topPadding));//上
//	map.insert("bottomPadding", QString::number(bottomPadding));//下
//	map.insert("leftPadding", QString::number(leftPadding));//左
//	map.insert("rightPadding", QString::number(rightPadding));//右
//	map.insert("headerTopPadding", QString::number(headerTopPadding)); //页眉
//	map.insert("footerBottomPadding", QString::number(footerBottomPadding));//页脚*/
//	//左边数据
//	map.insert("paperType", "A4");//纸张类型
//	//map.insert("paperDir", "1");//纸张方向
//	map.insert("paperDir", QString::number(1));
//	QString tem = map["paperDir"];
//	int paperWidth = contentTxtParam->getTotalW();
//	int paperHieght = contentTxtParam->getTotalH();
//	map.insert("paperWidth", QString::number(paperWidth));//纸宽度
//	map.insert("paperHieght", QString::number(paperHieght));//纸宽度
//	preMap = map;
//	PageSettingDlg m_Name;
//	m_Name.InitData(map);
//	m_Name.show();
//	if (m_Name.exec() == QDialog::Accepted)
//	{
//		sendSetPagePropertyDataSignal(map);
//		//BaseWinData * winData = (BaseWinData *)mTxtWindow->getContentData();
//		SetPagePropertyAction *setPageAction = new SetPagePropertyAction(mOperateMgr->getCurStepMgr());
//		setPageAction->initAction(this, preMap, map);
//		mOperateMgr->createNewAction();
//		mOperateMgr->addToLocalAction(setPageAction);
//	}
//}
//bool WriterManager::setPagePropertyData(QMap<QString, QString>& setMap)
//{
//	ViewObj * pagePan = (ViewObj *)mRootView->getChild(0)->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
//	QList<BaseContainer *> * childrenBase = pagePan->getChildren();
//	QList<ViewObj *> * children = (QList<ViewObj *>*)childrenBase;
//	if (!children || children->size() == 0)
//	{
//		return false;
//	}
//
//	int topPadding = setMap.value("topPadding").toInt();//上
//	int bottomPadding = setMap.value("bottomPadding").toInt();//下
//	int leftPadding = setMap.value("leftPadding").toInt();//左
//	int rightPadding = setMap.value("rightPadding").toInt();//右
//	int headerTopPadding = setMap.value("headerTopPadding").toInt(); //页眉
//	int footerBottomPadding = setMap.value("footerBottomPadding").toInt();//页脚
//	int pageSize = children->size();
//
//	//mWinData->resetPageParam(headerTopPadding, footerBottomPadding, topPadding, bottomPadding, leftPadding, rightPadding);
//
//	return true;
//}
//bool WriterManager::sendSetPagePropertyDataSignal(QMap<QString, QString>& setMap)
//{
//	ShortcutKeyInputParam * shortcutKeyParam = new ShortcutKeyInputParam(this, ShortcutType::SetPageProporty);
//	shortcutKeyParam->initSetPageProportyAction(setMap);
//	getUIInput()->addMsg(shortcutKeyParam);
//	return true;
//}
void WriterManager::doOnlyPrint(QPrinter *printer)
{
	UIConstants::MyColor::ONLY_READ_COLOR_OBJ = BaseMath::createColor(UIConstants::MyColor::ONLY_READ_PRINT_COLOR);
	QPainter painter(printer);
	QPaintDevice* device = painter.device();
	int deviceHeight = device->height();
	//QRect printerRect(printer.setp);
	double printerW(device->width());
	double printerH(deviceHeight);
	qreal xScale = 1, yScalse = 1,destScalse;

	ViewObj * pagePan = (ViewObj *)mRootView->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, true);
	QList<BaseContainer *> * childrenBase = pagePan->getChildren();
	QList<ViewObj *> * children = (QList<ViewObj *>*)childrenBase;
	int firstPage = printer->fromPage() - 1;
	if (firstPage >= children->size())
		return;
	if (firstPage == -1)
		firstPage = 0;
	int lastPage = printer->toPage() - 1;
	if (lastPage == -1 || lastPage >= children->size())
		lastPage = children->size() - 1;
	int numPages = lastPage - firstPage + 1;
	ViewObj* currentPage = NULL;
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;

	for (int i = 0; i < printer->numCopies(); ++i) {
		for (int j = 0; j < numPages; ++j) {
			if (i != 0 || j != 0)
				printer->newPage();
			int index;
			if (printer->pageOrder() == QPrinter::FirstPageFirst) {
				index = firstPage + j;
			}
			else {

				index = lastPage - j;
			}
			currentPage = children->at(index);
			LayoutViewParam * pageViewParam = (LayoutViewParam *)currentPage->getViewParam();
			//pageViewParam->doNeedRepaint();
			int pageW = pageViewParam->getTotalW();
			int pageH = pageViewParam->getTotalH();
			xScale = printerW / pageW;
			yScalse = printerH / pageH;
			QMatrix matrix;
			if (xScale<yScalse)
			{
				destScalse = xScale;
				//PRINT_SCALE = xScale;
			}
			else
			{
				destScalse = yScalse;
				
				//PRINT_SCALE = yScalse;
			}
			matrix.scale(destScalse, destScalse);
			painter.setMatrix(matrix);

			mOperateMgr->setEffectLoopPage(currentPage);
			x = pageViewParam->getX();
			y = pageViewParam->getY();
			w = (pageViewParam->getTotalW() + 2);
			h = (pageViewParam->getTotalH() + 2);
			painter.setClipRect(-1, -1, w, h);
			mOperateMgr->refreshTotalClipOutside(-1, -1, w, h);
			//currentPage->getDrawMgr()->draw(painter, -pageViewParam->getX(), -pageViewParam->getY(), false);
			currentPage->getDrawMgr()->draw(painter, -x, -y, true);
		}
	}
	//QVector<QRect> rectList = BaseTools::getCurClip(painter);
	//QRect rect = rectList.at(0);
	//x = rect.x();
	//y = rect.y();
	//w = rect.width();
	//h = rect.height();
	//painter.setClipRect(x, y, w, h, Qt::ReplaceClip);
	//painter.setClipRect(0, 0, w, h, Qt::ReplaceClip);
	//mOperateMgr->refreshTotalClipOutside(x, y, w, h);
	//PRINT_STATUS = false;
	UIConstants::MyColor::ONLY_READ_COLOR_OBJ = BaseMath::createColor(UIConstants::MyColor::ONLY_READ_COLOR);
}
void WriterManager::doPrint(QPrinter *printer)
{
	bool isTidy = isOnShowTidy();
	showTidy( true, false);

	mLayoutReplaceMgr->replaceLayout();

	doOnlyPrint(printer);
	showTidy( isTidy, false);
}
//打印预览
void WriterManager::doPrintPreview()
{
	mAppEntrance->doTimePause();
	mOperateMgr->clearChooseList(true);

	QScrollArea areaWidget;
	mAreaWidget = &areaWidget;
	areaWidget.installEventFilter(this);
	areaWidget.setWindowTitle("打印预览");

	QDesktopWidget *deskWgt = QApplication::desktop();
	int x = 0;
	int y = 30;
	QRect availableRect = deskWgt->availableGeometry();
	int avaiableHeight = availableRect.height();
	areaWidget.setGeometry(x, y, availableRect.width(), avaiableHeight);
	QPrinter printer;
	
	mOperateMgr->setPaperTypeBySystemParam(printer);
	QPrintPreviewDialog preview(&printer, &areaWidget);
	//preview.setModal();
	QList<QToolBar *> toolbarlist = preview.findChildren<QToolBar *>();
	if (!toolbarlist.isEmpty())
	{
		//toolbarlist.first()->actions().at(8)->setDisabled(true);
		//toolbarlist.first()->actions().at(12)->setDisabled(true);
		toolbarlist.first()->actions().at(17)->setVisible(false);
		toolbarlist.first()->actions().at(3)->setVisible(false);
		toolbarlist.first()->actions().at(4)->setVisible(false);
		toolbarlist.first()->actions().at(5)->setVisible(false);
	}

	QWidget * parentVidget = preview.parentWidget();
	int width = parentVidget->width();
	preview.setMinimumWidth(width);
	ViewObj * page = (ViewObj *)mRootView->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, false)
		->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false)->getChild(0);
	int pageH = page->getViewParam()->getTotalH();
	//int pageH = UIConstants::PageParams::PAGE_WIDTH / UIConstants::WindowParams::W_H_RATIO;
	preview.setMinimumHeight(pageH + 180);
	// 当要生成预览页面时，发射paintRequested()信号
	connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(doPrint(QPrinter*)));
	connect(&preview, SIGNAL(finished(int)), this, SLOT(printPreviewOver()));
	
	areaWidget.setWidget(&preview);
	areaWidget.setAutoFillBackground(true);
	areaWidget.setWidgetResizable(true);
	areaWidget.setSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);
	areaWidget.show();
	int res = preview.exec();
	if (res == QDialog::Accepted)
	{
		printPreviewOver();
	}
	else
	{
		int d = 0;
	}
//	mAppEntrance->timerResume();
	//printer.abort;
}
void WriterManager::outputPDF()
{
	QString file_path = QFileDialog::getSaveFileName(mAppEntrance, "Export PDF", QString(), "*.pdf");  //用文件对话框设置输出路径  
	if (!file_path.isEmpty())
	{
		//如果没有写后缀就自动加上  
		if (QFileInfo(file_path).suffix().isEmpty())
		{
			file_path.append(".pdf");  //或者 file_path+=".pdf"  
		}
	}
	QPrinter printer;
	printer.setPaperSize(QPrinter::A4);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(file_path); //直接设置输出文件路径，相对或者绝对路径  
	doPrint(&printer);
}
void WriterManager::printPreviewOver()
{
	getUIInput()->setHandleOverPreparRestart();
	mAppEntrance->timerResume();
	mAppEntrance->setTimeStart();
}
void WriterManager::printPreview()
{
	ShortcutKeyInputParam * shortcutKeyParam = new ShortcutKeyInputParam(this, ShortcutType::PrintPreview);
	getUIInput()->addMsg(shortcutKeyParam);
}
void WriterManager::sendPrintSign(QPrinter *printer)
{
	ShortcutKeyInputParam * shortcutKeyParam = new ShortcutKeyInputParam(this, ShortcutType::DoPrint);
	getUIInput()->addMsg(shortcutKeyParam);
	/*QEventLoop eventLoop;
	connect(this, SIGNAL(toPrint()), &eventLoop, SLOT(quit()));
	eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
	doPrint(printer);*/
	//emit toContinue();
}
void WriterManager::print()
{
	bool testHtmel = false;
	if (testHtmel)
	{
		//showTidy(true);
		QString returnStr;
		//getStr(returnStr);
		QString path = QFileDialog::getSaveFileName(NULL, "另存为", "txt", "*.txt");
		QFile file("hero.txt");
		if (!file.open(QIODevice::Append | QIODevice::Text))
		{
			qDebug("fail");
		}
		QTextStream write(&file);
		write << returnStr << endl;
		file.close();
		return;
	}
		QPrinter printer;
		QPrintDialog printDialog(&printer, mAppEntrance);
		if (printDialog.exec())
		{

			sendPrintSign(NULL);

		}
	
}
void WriterManager::sendContinuingPrintSign(int startParagraph, int endParagraph)
{
	ShortcutKeyInputParam * shortcutKeyParam = new ShortcutKeyInputParam(this, ShortcutType::ContinuePrint);
	shortcutKeyParam->initContinuePrint(startParagraph, endParagraph);
	getUIInput()->addMsg(shortcutKeyParam);
}
void WriterManager::continuingPrint(int startParagraph, int endParagraph)
{
	//装饰一下
	//bool isTidy = isOnShowTidy();
	//showTidy(true);
	//endParagraph++;
	UIConstants::MyColor::ONLY_READ_COLOR_OBJ = BaseMath::createColor(UIConstants::MyColor::ONLY_READ_PRINT_COLOR);
	QList<ViewObj *>  sourceViewList;//临时保存ViewObj 对象
	bool isSuccess = getRowListBetweenStartAndEnd(startParagraph, endParagraph, true, &sourceViewList);
	if (!isSuccess)
	{
		return;
	}
	ViewObj * startRow = sourceViewList.at(0);
	ViewObj * endRow = sourceViewList.at(sourceViewList.size() - 1);
	ViewObj * startPage = startRow->getParentViewById(ConstantsID::ContainerId::NORMAL_PAGE);
	ViewObj * endPage = endRow->getParentViewById(ConstantsID::ContainerId::NORMAL_PAGE);
	//把要打的页都放入sourceViewList
	sourceViewList.clear();
	ViewObj * pagePan = (ViewObj *)mRootView->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, true);
	QList<BaseContainer *> * childrenBase = pagePan->getChildren();
	QList<ViewObj *> * allPages = (QList<ViewObj *>*)childrenBase;
	int startPageIdx = startPage->getViewIndex();
	int endPageIdx = endPage->getViewIndex();
	int startRowIdx = startRow->getViewIndex();
	int endRowIdx = endRow->getViewIndex();
	if (startPage == endPage)
	{
		sourceViewList.push_back(startPage);
	}
	else
	{
		if (startPageIdx >= endPageIdx)
		{
			return;
		}
		sourceViewList.push_back(startPage);

		for (int i = startPageIdx + 1; i < endPageIdx; i++)
		{
			sourceViewList.push_back(allPages->at(i));
		}
		sourceViewList.push_back(endPage);
	}


	QPrinter printer;
	mOperateMgr->setPaperTypeBySystemParam(printer);
	QPainter painter(&printer);
	QPaintDevice* device = painter.device();
	int deviceHeight = device->height();
	//QRect printerRect(printer.setp);
	double printerW(device->width());
	double printerH(deviceHeight);
	qreal xScale = 1, yScalse = 1;
	int w = 0, h = 0;
	int numPages = endPageIdx - startPageIdx + 1;
	QList<ViewObj *> temptNotPrintContainer;//保存设置不显示的View 对象
	ViewObj* currentPage = NULL;
	for (int j = 0; j < numPages; ++j)
	{
		if (j != 0)
		{
			printer.newPage();
		}
		temptNotPrintContainer.clear();
		currentPage = sourceViewList.at(j);
		LayoutViewParam * pageViewParam = (LayoutViewParam *)currentPage->getViewParam();
		//pageViewParam->doNeedRepaint();
		int pageW = pageViewParam->getTotalW();
		int pageH = pageViewParam->getTotalH();
		xScale = printerW / pageW;
		yScalse = printerH / pageH;
		QMatrix matrix;
		if (xScale<yScalse)
		{
			matrix.scale(xScale, xScale);
			//PRINT_SCALE = xScale;
		}
		else
		{
			matrix.scale(yScalse, yScalse);
			//PRINT_SCALE = yScalse;
		}
		//PRINT_STATUS = true;
		painter.setMatrix(matrix);
		//currentPage->setPrintType();//代码改变，这行可以不用了
		//打印第一页
		if (j == 0)
		{
			if (startRowIdx>0)//把页眉页脚，和开打之前的行都设置不可见，并加入临时容器
			{
				ViewObj* pageHeader = (ViewObj*)currentPage->findChildrenByID(ConstantsID::ContainerId::PAGE_TITLE,false);
				ViewObj* pageBottom = (ViewObj*)currentPage->findChildrenByID(ConstantsID::ContainerId::PAGE_BOTTOM, false);
				pageHeader->setViewType(BaseConstants::ViewType::HIDE);
				temptNotPrintContainer.push_back(pageHeader);
				pageBottom->setViewType(BaseConstants::ViewType::HIDE);
				temptNotPrintContainer.push_back(pageBottom);

				ViewObj * txtLayer = startRow->getViewParent();
				ViewObj * tempRow = NULL;
				QList<ViewObj *> * rowList = (QList<ViewObj *> *)txtLayer->getChildren();
				int rowSize = rowList->size();
				for (int m = 0; m < startRowIdx; m++)
				{
					tempRow = rowList->at(m);
					tempRow->setViewType(BaseConstants::ViewType::HIDE);
					temptNotPrintContainer.push_back(tempRow);
				}
			}
		}
		if (j == numPages - 1)
		{
			ViewObj * txtLayer = endRow->getViewParent();
			ViewObj * tempRow = NULL;
			QList<ViewObj *> * rowList = (QList<ViewObj *> *)txtLayer->getChildren();
			int rowSize = rowList->size();
			for (int m = endRowIdx + 1; m < rowSize; m++)
			{
				tempRow = rowList->at(m);
				tempRow->setViewType(BaseConstants::ViewType::HIDE);
				temptNotPrintContainer.push_back(tempRow);
			}
		}

		mOperateMgr->setEffectLoopPage(currentPage);
		w = (pageViewParam->getTotalW() + 1);
		h = (pageViewParam->getTotalH() + 1);
		painter.setClipRect(0, 0, w, h);
		mOperateMgr->refreshTotalClipOutside(0, 0, w, h);
		BaseViewParam * currenPageViewParam = currentPage->getViewParam();
		currentPage->getDrawMgr()->draw(painter, 0, -currenPageViewParam->getY(), true);
		int rebackSize = temptNotPrintContainer.size();
		for (int m = 0; m < rebackSize; m++)
		{
			temptNotPrintContainer.at(m)->setViewType(BaseConstants::ViewType::NORMAL);
		}
	}
	UIConstants::MyColor::ONLY_READ_COLOR_OBJ = BaseMath::createColor(UIConstants::MyColor::ONLY_READ_COLOR);
	//PRINT_SCALE = 1;
	//PRINT_STATUS = false;
	//装饰一下
	//showTidy(isTidy);
}
bool WriterManager::getRowListBetweenStartAndEnd(int startParagraphIndex, int endParagraphIndex, bool needGoThrough, QList<ViewObj *> * sourceRowList)
{
	if (startParagraphIndex<0)
	{
		startParagraphIndex = 0;
	}
	if (endParagraphIndex<startParagraphIndex)
	{
		return false;
	}
	BaseParagraph * startParagraph = mOperateMgr->getContentParagraphByIndex(startParagraphIndex - 1);
	BaseParagraph * endParagraph = mOperateMgr->getContentParagraphByIndex(endParagraphIndex - 1);
	//ParagraphTemplete * startParagraph = mWinData->getParagraphByParagraphIndex(startParagraphIndex - 1);
	//ParagraphTemplete * endParagraph = mWinData->getParagraphByParagraphIndex(endParagraphIndex - 2);
	if (startParagraph == NULL || endParagraph == NULL)
	{
		return false;
	}
	ViewObj * startView = (ViewObj *)startParagraph->getChild(0);
	ViewObj * endView = endParagraph->getLastChildView();
	ViewObj * startRow = startView->getViewParent();
	ViewObj * endRow = endView->getViewParent();
	if (!needGoThrough)
	{
		sourceRowList->push_back(startRow);
		sourceRowList->push_back(endRow);
		return true;
	}
	ViewObj * startPage = startRow->getParentViewById(ConstantsID::ContainerId::NORMAL_PAGE);
	ViewObj * endPage = endRow->getParentViewById(ConstantsID::ContainerId::NORMAL_PAGE);
	int startRowIdx = startRow->getViewIndex();
	int endRowIdx = endRow->getViewIndex();
	if (startPage != endPage)
	{
		int startPageIdx = startPage->getViewIndex();
		int endPageIdx = endPage->getViewIndex();
		ViewObj * txtLayer = startRow->getViewParent();

		QList<ViewObj *> * pageList = (QList<ViewObj *> *)startPage->getViewParent()->getChildren();
		ViewObj * page = NULL;
		QList<ViewObj *> * rowList = (QList<ViewObj *> *)txtLayer->getChildren();
		// 起始页
		for (int i = startRowIdx; i < rowList->size(); i++)
			sourceRowList->push_back(rowList->at(i));
		// 中间页
		for (int i = startPageIdx + 1; i < endPageIdx; i++)
		{
			page = pageList->at(i);
			txtLayer = (ViewObj *)page->getChild(ConstantsID::ContainerId::PAGE_CONTENT)->getChild(ConstantsID::ContainerId::CONTENT_TXT_LAYER);
			rowList = (QList<ViewObj *> *)txtLayer->getChildren();
			for (int j = 0; j < rowList->size(); j++)
			{
				sourceRowList->push_back(rowList->at(j));
			}
		}
		// 结束页
		txtLayer = endRow->getViewParent();
		rowList = (QList<ViewObj *> *)txtLayer->getChildren();
		for (int i = 0; i <= endRowIdx; i++)
			sourceRowList->push_back(rowList->at(i));
	}
	else
	{
		//int startRowIdx = startRow->getViewIndex();
		//int endRowIdx = endRow->getViewIndex();
		ViewObj * txtLayer = startRow->getViewParent();
		QList<ViewObj *> * rowList = (QList<ViewObj *> *)txtLayer->getChildren();
		// 起始页
		for (int i = startRowIdx; i <= endRowIdx; i++)
			sourceRowList->push_back(rowList->at(i));
	}
	return true;
}
void WriterManager::showTidy(bool tidy, bool needAddAction)
{
	ViewObj* focus = NULL;
	char direction = 0;
	mOperateMgr->getFocusAndDirection(focus,direction);
	FileData * fileData = mAppEntrance->getFileData();
	
	bool currentTidy = mRootTemp->getIsShowTidy();
	if (tidy == currentTidy)
	{
		return;
	}
	mOperateMgr->showTidy(tidy);
	if (needAddAction)
	{
		mOperateMgr->createNewAction();
		SetParagraphOtherParamAction * action = new SetParagraphOtherParamAction(mOperateMgr->getCurStepMgr());
		action->showTidy(currentTidy, tidy);
		mOperateMgr->addToLocalAction(action);
	}
	if (currentTidy)
	{
		fileData->backupReadWriteAuthority();
		mRootTemp->showTidy();
	}
	else
	{
		fileData->saveReadWriteAuthorityLastReadWriteAuthority();
		fileData->setFileAuthority(AuthorityConstants::Authority::ONLY_READ_VIEW_NORMAL);
		mRootTemp->showTidy();
	}

	BaseContainer * contentTemp = mRootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	ViewObj* lastObj = contentTemp->getLastChildView();

	//ViewObj * firstChild = (ViewObj *)contentTemp->getFirstChildView();
	BaseContainer * firstPage = mOperateMgr->getPageByIndex(0);
	BaseContainer * firstRow = firstPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false)->getChild(0);

	ViewObj * firstChild = (ViewObj *)firstRow->getChild(0);

	ViewObj * row = firstChild->getViewParent();
	RowLayout * rowLayout = (RowLayout*)row->getChildrenMgr();
	rowLayout->judgeLessThanCircleUntilObj(lastObj);
	if (focus)
	{
		mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, direction);
	}
}
bool WriterManager::isOnShowTidy()
{
	return mRootTemp->getIsShowTidy();
}
bool WriterManager::haveUIIputNotHandle()
{
	return mUIInputMgr->haveUIIputNotHandle();
}
void WriterManager::handleUIInput()
{
	if (mWriterInput)
	{
		mUIInputMgr->myUpdate();
	}
}
void WriterManager::doGetHtml(QList<htmlBaseObj*> * resultList)
{
	if (!mHtmlOutPutManager)
	{
		mHtmlOutPutManager = new htmlOutPutManger();
	}
	//mWinData->replaceNeedReplaceList();
	mHtmlOutPutManager->getHtmlObj(resultList, mRootView);
}
void WriterManager::getStr(QString &str)
{
	bool isTidy = this->isOnShowTidy();
	showTidy( true, true);
	//mWinData->refreshAllPage();
	if (!mHtmlOutPutManager)
	{
		mHtmlOutPutManager = new htmlOutPutManger();
	}
	//mWinData->replaceNeedReplaceList();
	mHtmlOutPutManager->getTxt(mRootView, str);
	showTidy(isTidy, true);
}
bool WriterManager::eventFilter(QObject *target, QEvent *event)
{
	if (mAreaWidget)
	{
		if (mAreaWidget == target && event->type() == QEvent::Close) 
			{
				printPreviewOver();
				mAreaWidget = NULL;
			}
	}
	return false;
}
void WriterManager::keyPressOnRootView(QString str)
{
	mWriterInput->keyPressOnRootView(str);
}