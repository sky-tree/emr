#include "OperateMgr.h"

#include <QDebug>
#include <QTimer>

#include "toolbarclass.h"

#include "AutoSaveMgr.h"



#include "MainViewContainer.h"
#include "MyAppEntrance.h"
#include "WriterManager.h"
#include "LayoutReplaceManager.h"
#include "ParagraphManager.h"
#include "NodeMgr.h"

#include "WriterLogicMgr.h"
#include "FlagParam.h"
#include "FlagViewParam.h"
#include "VerticalViewParam.h"

#include "StrContentData.h"
#include "TxtWinData.h"
#include "ParagraphData.h"
#include "ExcelCellData.h"

#include "ExcelCrossParentMgr.h"

#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseTemplate.h"
#include "BaseParagraph.h"

#include "CodeParams.h"
#include "NodeParams.h"
#include "ParagraphCodeParams.h"

#include "BaseLogic.h"

#include "RowLayout.h"
#include "ExcelLayout.h"
#include "LayoutViewParam.h"

#include "BaseParagraph.h"
#include "TempParagraphMgrData.h"


#include "AddObjListAction.h"

#include "UIInputManager.h"
#include "ActionMgr.h"
#include "TempContainerMgr.h"
#include "FileData.h"
#include "CodeTempData.h"

#include "BaseIOTools.h"
#include "SystemParams.h"

#include "ViewFactory.h"
#include "ActionFactory.h"
#include "TempleteFactory.h"
#include "ContentDataFactory.h"

#include "UIConstants.h"
#include "ConstantsID.h"
#include "StrConstants.h"
#include "BaseConstants.h"

#include "BaseMath.h"
#include "BaseTools.h"
#include "BaseSystem.h"
#include "GlobalTemplate.h"

#include "TextWidget.h"
#include "TextWidgetParam.h"
#include "TempleteTypeEnumeration.h"
#include "StructurationTextWidget.h"
#include "StructurationTextParam.h"
#include "MenstruationTemplete.h"
#include "MessageBox.h"
#include "MacroTemplete.h"
#include "ChoiceWidgetTemplete.h"
#include "ExcelTemplete.h"
#include "RowViewParam.h"
#include "AddTempletWidgetAction.h"
#include "AuthorityConstants.h"
#include "ObjMouseManager.h"
#include "ExcelVerticalLayoutAction.h"
#include "UnfoldChoiceWidgetTemplete.h"
#include "qprinter.h"
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
#define FIND_TEMPLATE_PARAENT_POSITION \
	char dir = 0; \
	ViewObj * focus = NULL; \
	getFocusAndDirection(focus, dir); \
	BaseContainer* tempParent = NULL; \
	BaseContainer* focusTempParent = NULL; \
	/*ViewObj * addRow = NULL; */\
	\
	BaseContainer* tempContainer = NULL; \
	int tempIdx = -1; \
	focus->findNormalTempContainerFromParent(tempContainer, tempIdx); /*应为focus是最低层的元素。*/\
	tempParent = focus->getTempParent(); \
if (tempParent == tempContainer)/*focus是普通文字*/\
{\
if (dir == BaseConstants::Direction::RIGHT)\
{\
	tempIdx++; \
}\
	focusTempParent = tempParent; \
	/*addRow = focus->getViewParent(); */ \
}\
else\
{\
	tempParent = tempContainer->getChildrenMgr()->getChildren()->at(tempIdx); \
	BaseContainer* firstView = tempParent->getFirstChildView(); \
	focusTempParent = tempParent; \
if (firstView == focus&& dir == BaseConstants::Direction::LEFT)\
{\
	/*addRow = firstView->getViewParent(); */ \
}\
else\
{\
	tempIdx++; \
	focus = (ViewObj*)focusTempParent->getLastChildView(); \
	dir = BaseConstants::Direction::RIGHT; \
	focus->setFocus(true, false); \
	/*addRow = focus->getViewParent(); */ \
}\
	tempParent = tempContainer; \
}
#define FIND_POSITION \
	ViewObj * focusFirst = mPressEnd->getFocusObj(); \
if (!focusFirst)\
	return; \
	FIND_TEMPLATE_PARAENT_POSITION

QTime time;

OperateMgr::OperateMgr(WriterManager * writer, BaseIOTools * ioTools)
{
	mDestEffectLogicPageIndex = -1;
	mWriter = writer;
	mFileData = mWriter->getFileData();
	mIOTools = ioTools;
	mChooseList = new QList<BaseContainer *>();
	mPressStart = new FlagParam();
	mPressEnd = new FlagParam();
	mSavedPressStart = new FlagParam();
	mSavedPressStart2 = new FlagParam();
	mPrePressStart = new FlagParam();
	mPrePressEnd = new FlagParam();
	mSavedPressEnd = new FlagParam();
	mSavedPressEnd2 = new FlagParam();
	mFlag = ViewFactory::createFlag(mWriter);

	mNeedUndo = false;
	changeStatus(BaseConstants::OperateStatus::FREE);
	mIsUpsidedown = false;
	mNeedRefreshPagePanelYByFlag = true;

	mTotalClip = new int[4];

	mWindow = NULL;
	mWinData = NULL;

	mPagePanel = NULL;

	mEffectLogicPage = NULL;
	mEffectLoopPage = NULL;

	//mTitleTemp = NULL;
	//mBottomTemp = NULL;

	mFocusParagraphVerticalAlignType = BaseConstants::Direction::UP;

	mFocusTxtData = ContentDataFactory::createDefaultTxtData();
	//mFocusTxtData = new StrContentData(NULL);
	mFocusParagraphData = NULL;

	mActionMgr = new ActionMgr(this);

	mSystemParams = new SystemParams(this);
	mTempSystemParams = NULL;

	mDrawParamList = new QList<StrContentData *>();
	mExcelTempelateListForFresh = new QList<ExcelTemplete*>();

	mAutoSaveMgr = new AutoSaveMgr(this);
}


OperateMgr::~OperateMgr()
{
	if (mDrawParamList)
	{
		mDrawParamList->clear();
		delete mDrawParamList;
		mDrawParamList = NULL;
	}
	if (mActionMgr)
	{
		delete mActionMgr;
		mActionMgr = NULL;
	}
	if (mExcelTempelateListForFresh)
	{
		mExcelTempelateListForFresh->clear();
		delete mExcelTempelateListForFresh;
		mExcelTempelateListForFresh = NULL;
	}


	mWriter = NULL;

	mIOTools = NULL;

	mFileData = NULL;

	if (mSystemParams)
	{
		delete mSystemParams;
		mSystemParams = NULL;
	}
	if (mTempSystemParams)
	{
		delete mTempSystemParams;
		mTempSystemParams = NULL;
	}
	// 当前选中项列表
	if (mChooseList)
	{
		mChooseList->clear();
		delete mChooseList;
		mChooseList = NULL;
	}
	// 当前文字属性
	if (mFocusTxtData)
	{
		delete mFocusTxtData;
		mFocusTxtData = NULL;
	}

	mFocusParagraphData = NULL;

	// 光标
	mFlag = NULL;
	// 按下光标
	if (mPressStart)
	{
		delete mPressStart;
		mPressStart = NULL;
	}
	if (mPressEnd)
	{
		delete mPressEnd;
		mPressEnd = NULL;
	}
	if (mSavedPressStart)
	{
		delete mSavedPressStart;
		mSavedPressStart = NULL;
	}
	if (mSavedPressStart2)
	{
		delete mSavedPressStart2;
		mSavedPressStart2 = NULL;
	}
	if (mSavedPressEnd)
	{
		delete mSavedPressEnd;
		mSavedPressEnd = NULL;
	}
	if (mSavedPressEnd2)
	{
		delete mSavedPressEnd2;
		mSavedPressEnd2 = NULL;
	}
	if (mPrePressStart)
	{
		delete mPrePressStart;
		mPrePressStart = NULL;
	}
	if (mPrePressEnd)
	{
		delete mPrePressEnd;
		mPrePressEnd = NULL;
	}

	mWindow = NULL;
	mWinData = NULL;

	mPagePanel = NULL;

	mEffectLogicPage = NULL;
	mEffectLoopPage = NULL;

	if (mTotalClip)
	{
		delete[] mTotalClip;
		mTotalClip = NULL;
	}

	if (mActionMgr)
	{
		delete mActionMgr;
		mActionMgr = NULL;
	}
	if (mAutoSaveMgr)
	{
		delete mAutoSaveMgr;
		mAutoSaveMgr = NULL;
	}
}
void OperateMgr::myUpdate()
{
	mAutoSaveMgr->myUpdate();
}
void OperateMgr::doSaveDataOnly(bool needCloseWriteAuthority)
{
	if (needCloseWriteAuthority)
	{
		mWriter->getFileData()->setFileAuthority(AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
		clearAllAction();
	}
	mSystemParams->doSave(mIOTools);

	BaseTemplate * rootTemp = mWriter->getRootTemp();
	BaseContainer * contentRootTemp = rootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	BaseContentData * contentRootData = contentRootTemp->getContentData();

	//refreshTitleTemp();
	//refreshBottomTemp();
	BaseContainer * titleTemp = (BaseTemplate *)mWriter->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false)
		->findChildrenByID(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, false);
	BaseContainer * bottomTemp = (BaseTemplate *)mWriter->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false)
		->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);


	BaseContentData * data = titleTemp->getTempParent()->getContentData();
	data->doSave(mIOTools);
	data = bottomTemp->getTempParent()->getContentData();
	data->doSave(mIOTools);
	contentRootData->doSave(mIOTools);
	clearChooseList(true);
	mAutoSaveMgr->changeStatus(BaseConstants::NONE, true);
}
void OperateMgr::doSaveToByteArry(QByteArray&byarry, bool needCloseWriteAuthority)
{

	mIOTools->writeFile();
	doSaveDataOnly(needCloseWriteAuthority);
	byarry = mIOTools->getByteArray();
	mIOTools->close();
}
void OperateMgr::autoSave()
{
	QByteArray ba;
	doSaveToByteArry(ba, false);
}
void OperateMgr::clearView(bool needAddFlagToLogicList)
{
	mWindow = NULL;
	mWinData = NULL;
	mPagePanel = NULL;
	mWriter->clearView();
	if (needAddFlagToLogicList)
	{
		BaseLogic * flagLogic = mFlag->getLogicByClassID(ConstantsID::ClassId::FLAG_FLASH_LOGIC);
		mWriter->getLogicMgr()->addToRunningList(flagLogic);
	}
}
void OperateMgr::clearRootTempAndView()
{
	mWindow = NULL;
	mWinData = NULL;
	mPagePanel = NULL;
	ViewObj * rootView = mWriter->getRootView();
	BaseViewParam * rootViewParam = rootView->getViewParam();
	int w = rootViewParam->getTotalW();
	int h = rootViewParam->getTotalH();
	resizeWindow(w, h);
	mWriter->clearRootTempAndView();
	mSystemParams->initParams();
	BaseLogic * flagLogic = mFlag->getLogicByClassID(ConstantsID::ClassId::FLAG_FLASH_LOGIC);
	mWriter->getLogicMgr()->addToRunningList(flagLogic);
}
void OperateMgr::addHospitalTitle(BaseContainer * titleContainer, bool isLoad)
{
	ViewObj * preFocus = mPressEnd->getFocusObj();

	ViewObj * firstEnter = titleContainer->getFirstView();

	// 获取初始段落
	BaseParagraph * firstParagraph = firstEnter->getParagraphParent();
	BaseContentData * data = firstParagraph->getContentData();

	if (isLoad)
	{
		int enterIdx = -1;
		QList<BaseContainer *> * children = titleContainer->getChildren();
		BaseContainer * child = NULL;
		for (int i = 0; i < children->size(); i++)
		{
			child = children->at(i);
			if (child->isEnter())
			{
				enterIdx = i;
				firstEnter = (ViewObj *)child;
				break;
			}
		}
		NodeMgr * nodeMgr = (NodeMgr *)titleContainer->getChildrenMgr();
		data->getParagraphCodeParams()->setAuthority(1);
		if (enterIdx > 0)
		{
			nodeMgr->delChildren(0, enterIdx, true);
		}
	}
	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, firstEnter, BaseConstants::Direction::LEFT);

	mFocusTxtData->setBold(true);
	mFocusTxtData->setPointSize(UIConstants::DefaultFontParams::DEFAULT_HOSPITAL_TITLE_FONT_SIZE);
	mFocusTxtData->setFamily(StrConstants::DEFAULT_HOSPITAL_TITLE_FONT_FAMILY);
	addStr(StrConstants::DEFAULT_HOSPITAL_TITLE, true, false);
	// 添加回车
	if (!isLoad)
		pressEnter(getUserID());

	// 设回默认文字属性
	mFocusTxtData->setBold(false);
	mFocusTxtData->setPointSize(UIConstants::DefaultFontParams::TXT_SIZE);
	mFocusTxtData->setFamily(StrConstants::DEFAULT_FONT_FAMILY);

	// 设置段落权限
	if (!isLoad)
	{
		ParagraphCodeParams * params = new ParagraphCodeParams("titleID", "titleName", 0, 0);
		data->initCodeParams(params);
	}
	else
		data->getParagraphCodeParams()->setAuthority(0);


	mActionMgr->clearAllAction();

	// 光标设置回页内容第一个回车
	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, preFocus, BaseConstants::Direction::LEFT);
}
void OperateMgr::doLoad1(bool isLoad)
{
	time.start();
	ViewObj * focus = NULL;
	BaseTemplate * rootTemp = NULL;
	ViewObj * rootView = NULL;
	bool isSecondLoad = false;

	BaseContainer * titleContainer = NULL;
	BaseContainer * bottomContainer = NULL;
	BaseContainer * contentContainer = NULL;
	if (isLoad)
	{
		mSystemParams->doLoad(mIOTools);
		rootTemp = mWriter->getRootTemp();
		if (rootTemp)
		{
			isSecondLoad = true;

			clearRootTempAndView();
		}

		int userID = getUserID();
		// 创建rootTemp
		rootTemp = TempleteFactory::createRootTemp(mWriter, mIOTools);

		rootView = ViewFactory::createRoot(mWriter);

		mWriter->initRootTempAndView(rootTemp, rootView);
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, NULL, BaseConstants::Direction::LEFT);

		titleContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false)
			->findChildrenByID(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, false);
		TempContainerMgr * containerChildrenMgr = (TempContainerMgr *)titleContainer->getChildrenMgr();
		containerChildrenMgr->addAllChildrenToParagraphAndView();

		bottomContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false)
			->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
		containerChildrenMgr = (TempContainerMgr *)bottomContainer->getChildrenMgr();
		containerChildrenMgr->addAllChildrenToParagraphAndView();


		contentContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
		QList<BaseContainer *> * contentContainerList = contentContainer->getChildren();
		//BaseContainer * contentContainer = NULL;
		ViewObj * lastView = NULL;






		// 如果最后一个不为回车
		int size = contentContainerList->size();
		containerChildrenMgr = (TempContainerMgr *)contentContainerList->at(size - 1)->getChildrenMgr();
		removeObjAfterLastEnter(containerChildrenMgr);

		for (int i = 0; i < contentContainerList->size(); i++)
		{
			contentContainer = contentContainerList->at(i);
			containerChildrenMgr = (TempContainerMgr *)contentContainer->getChildrenMgr();
			if (size == 1 || i == 0)
			{
				containerChildrenMgr->addAllChildrenToParagraphAndView();
				if (size > 1)
				{
					lastView = contentContainer->getLastView();
					setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, lastView
						, BaseConstants::Direction::LEFT);
				}
			}
			else
			{
				containerChildrenMgr->addRecordChildrenToParagraphAndView(lastView);
				lastView = contentContainer->getLastView();
			}
		}

		//if (!isSecondLoad)
		//{
		//ViewObj *titleNum = NULL;
		//ViewObj * bottomNum = NULL;
		//titleNum = (ViewObj *)titleContainer->findChildrenByID(ConstantsID::ContainerId::PAGE_NUMBER, false);
		//bottomNum = (ViewObj *)bottomContainer->findChildrenByID(ConstantsID::ContainerId::PAGE_NUMBER, false);
		mSystemParams->initParams();
		//}
		//else
		//{
		//	mSystemParams->initTitleOrBottomPageNumber();
		//}
		focus = contentContainerList->at(0)->getFirstChildView();

		MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::PageSetType::LineType, mSystemParams->getRowBottomLineType() != 0);
	}
	else
	{
		int userID = 0;
		// 创建rootTemp
		rootTemp = TempleteFactory::createRootTemp(mWriter, NULL);
		rootView = ViewFactory::createRoot(mWriter);
		mWriter->initRootTempAndView(rootTemp, rootView);

		titleContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false)
			->findChildrenByID(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, false);
		focus = ViewFactory::createEnter(mWriter, userID);
		titleContainer->getChildrenMgr()->addChild(focus, -1, true);

		bottomContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false)
			->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
		focus = ViewFactory::createEnter(mWriter, userID);
		bottomContainer->getChildrenMgr()->addChild(focus, -1, true);

		contentContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false)
			->findChildrenByID(ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER, false);
		focus = ViewFactory::createEnter(mWriter, userID);
		contentContainer->getChildrenMgr()->addChild(focus, -1, true);

		mSystemParams->initParams();
		MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::PageSetType::LineType, mSystemParams->getRowBottomLineType() != 0);
	}
	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::LEFT);
	BaseContainer * pagePanel = rootView->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, false)
		->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
	ViewObj * firstPage = (ViewObj *)pagePanel->getChild(0);
	setEffectLogicPage(firstPage);

	// 如果为初始创建，则自动在页眉添加医院标题
	//if (!isLoad)
	addHospitalTitle(titleContainer, isLoad);

	//setTitleOrBottomCanWrite(ConstantsID::ContainerId::TITLE_TEMP, false);
}
void OperateMgr::doInsertRecord(QString code, bool isCreate)
{
	ViewObj * focus = NULL;
	BaseTemplate * rootTemp = NULL;
	ViewObj * rootView = NULL;
	rootTemp = mWriter->getRootTemp();
	if (rootTemp)
	{
		clearRootTempAndView();
	}
	else
		mSystemParams->doLoad(mIOTools);
	// 创建rootTemp
	rootTemp = TempleteFactory::createRootTemp(mWriter, mIOTools);
	//if (!isSecondLoad)
	{
		rootView = ViewFactory::createRoot(mWriter);
		mWriter->initRootTempAndView(rootTemp, rootView);
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, NULL, BaseConstants::Direction::LEFT);
	}

	BaseContainer * titleContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false)
		->findChildrenByID(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, false);
	TempContainerMgr * containerChildrenMgr = (TempContainerMgr *)titleContainer->getChildrenMgr();
	containerChildrenMgr->addAllChildrenToParagraphAndView();

	BaseContainer * bottomContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false)
		->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
	containerChildrenMgr = (TempContainerMgr *)bottomContainer->getChildrenMgr();
	containerChildrenMgr->addAllChildrenToParagraphAndView();

	BaseContainer * contentTemp = rootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	QList<BaseContainer *> * contentContainerList = contentTemp->getChildren();
	BaseContainer * contentContainer = NULL;
	int size = contentContainerList->size();
	ViewObj * lastView = NULL;
	// 如果最后一个不为回车
	containerChildrenMgr = (TempContainerMgr *)contentContainerList->at(size - 1)->getChildrenMgr();
	removeObjAfterLastEnter(containerChildrenMgr);
	for (int i = 0; i < contentContainerList->size(); i++)
	{
		contentContainer = contentContainerList->at(i);
		containerChildrenMgr = (TempContainerMgr *)contentContainer->getChildrenMgr();
		if (size > 1)
		{
			//contentContainer->getCodeIdFromTemp();
		}
		else
		{
			contentContainer->setCodeId(code);
		}
		if (size == 1 || i == 0)
		{
			containerChildrenMgr->addAllChildrenToParagraphAndView();
			if (size > 1)
			{
				lastView = contentContainer->getLastView();
				setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, lastView
					, BaseConstants::Direction::LEFT);
			}
		}
		else
		{
			containerChildrenMgr->addRecordChildrenToParagraphAndView(lastView);
			lastView = contentContainer->getLastView();
		}
	}


	if (isCreate)
	{
		//ViewObj * titleNum = (ViewObj *)titleContainer->findChildrenByID(ConstantsID::ContainerId::PAGE_NUMBER, false);
		//ViewObj * bottomNum = (ViewObj *)bottomContainer->findChildrenByID(ConstantsID::ContainerId::PAGE_NUMBER, false);
		mSystemParams->initParams();
	}
	MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::PageSetType::LineType, mSystemParams->getRowBottomLineType() != 0);
	focus = contentContainerList->at(0)->getFirstChildView();

	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::LEFT);
	BaseContainer * pagePanel = rootView->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, false)
		->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
	ViewObj * firstPage = (ViewObj *)pagePanel->getChild(0);
	setEffectLogicPage(firstPage);
	mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);

	addHospitalTitle(titleContainer, true);
}
//void OperateMgr::activeRecordFromRecordList(QString recordID)
//{
//	BaseTemplate * rootTemp = mWriter->getRootTemp();
//	BaseContainer * contentTemp = rootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
//	QList<BaseContainer *> * containerList = contentTemp->getChildren();
//	BaseContainer * container = NULL;
//	CodeTempData * data = NULL;
//	ViewObj * firstView = NULL;
//	for (int i = 0; i < containerList->size(); i ++)
//	{
//		container = containerList->at(i);
//		data = (CodeTempData *)container->getContentData();
//		if (container->getCodeId() == recordID)
//		{
//			data->setAuthority(AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT);
//			firstView = container->getFirstView();
//		}
//		else
//			data->setAuthority(AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
//	}
//	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, firstView, BaseConstants::Direction::LEFT);
//}
void OperateMgr::activeRecordFromRecordListByFocus(QString & recodeId)
{
	ViewObj * focus = mPressEnd->getFocusObj();
	BaseContainer * focusContainer = focus->findTempFromParent(ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER, true);
	CodeTempData * data = (CodeTempData *)focusContainer->getContentData();

	BaseTemplate * rootTemp = mWriter->getRootTemp();
	BaseContainer * contentTemp = rootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	QList<BaseContainer *> * containerList = contentTemp->getChildren();
	BaseContainer * container = NULL;
	for (int i = 0; i < containerList->size(); i++)
	{
		container = containerList->at(i);
		data = (CodeTempData *)container->getContentData();
		if (focusContainer == container)
		{
			data->setAuthority(AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT);
			recodeId = data->getCodeId();
		}
		else
			data->setAuthority(AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
	}
	mWriter->getFileData()->setFileAuthority(AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT);
}
QByteArray OperateMgr::getByteArray()
{
	return mIOTools->getByteArray();
}
WriterManager * OperateMgr::getWriterMgr()
{
	return mWriter;
}
BaseContainer * OperateMgr::getTitleTemp()
{
	return mWriter->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false);
}
BaseContainer * OperateMgr::getBottomTemp()
{
	return mWriter->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false);
}
FileData * OperateMgr::getFileData()
{
	return mFileData;
}
SystemParams * OperateMgr::getSystemParams()
{
	if (mTempSystemParams)
	{
		return mTempSystemParams;
	}
	return mSystemParams;
}
int * OperateMgr::getTotalClip()
{
	return mTotalClip;
}
void OperateMgr::refreshTotalClipInside(int x, int y, int w, int h)
{
	if (x > mTotalClip[0])
	{
		mTotalClip[0] = x;
	}
	if (y > mTotalClip[1])
	{
		mTotalClip[1] = y;
	}
	int right = x + w;
	int bottom = y + h;
	if (right > mTotalClip[2])
	{
		mTotalClip[2] = right;
	}
	if (bottom > mTotalClip[3])
	{
		mTotalClip[3] = bottom;
	}
}
void OperateMgr::refreshTotalClipOutside(int x, int y, int w, int h)
{
	//if (x > mTotalClip[0])
	{
		mTotalClip[0] = x;
	}
	//if (y > mTotalClip[1])
	{
	mTotalClip[1] = x;
}
	int right = x + w;
	int bottom = y + h;
	//if (right > mTotalClip[2])
	{
		mTotalClip[2] = right;
	}
	//if (bottom > mTotalClip[3])
	{
		mTotalClip[3] = bottom;
	}
}
void OperateMgr::clearTotalClip()
{
	for (int i = 0; i < 4; i++)
	{
		mTotalClip[i] = 0;
	}
}
void OperateMgr::changeStatus(char status)
{
	mStatus = status;
}
BaseParagraph * OperateMgr::getContentParagraphByIndex(int idx)
{
	BaseContainer * contentTemp = mWriter->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	TempParagraphMgrData * data = (TempParagraphMgrData *)contentTemp->getContentData();
	ParagraphManager * paragraphMgr = data->getParagraphMgr();
	return paragraphMgr->getParagrapAt(idx);
}
void OperateMgr::setNeedUndo(bool needUndo)
{
	mNeedUndo = needUndo;
}
bool OperateMgr::getNeedUndo()
{
	return mNeedUndo;
}
void OperateMgr::setNeedRefreshPagePanelYByFlag(bool needRefresh)
{
	mNeedRefreshPagePanelYByFlag = needRefresh;
}
void OperateMgr::refreshPagePanelByFocus()
{
	//if (true)
	//{
	//	return;
	//}
	if (!mNeedRefreshPagePanelYByFlag || mPagePanel->getStatus() != BaseConstants::ObjStatus::FREE)
	{
		//mNeedRefreshPagePanelYByFlag = true;
		return;
	}
	LayoutViewParam * winParam = (LayoutViewParam *)mWindow->getViewParam();
	int winMaxY = 0;
	int winH = 0;
	ViewObj * pagePanel = getPagePanel();
	BaseViewParam * pagePanelParam = pagePanel->getViewParam();

	ViewObj * focus = mPressEnd->getFocusObj();
	BaseViewParam * focusParam = focus->getViewParam();

	ViewObj * row = focus->getViewParent();
	BaseViewParam * rowParam = row->getViewParam();

	ViewObj * layer = row->getViewParent();

	int flagY = 0;
	int flagH = 0;
	int destY = 0;
	int pagePanelH = pagePanelParam->getTotalH();
	// 如果焦点在页内容行上
	if (layer->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
	{
		bool isCross = false;
		if (row->isExcelRow())
		{
			ExcelCrossParentMgr * crossMgr = row->getExcelCrossParentMgr();
			// 如果处于跨页
			if (crossMgr && crossMgr->haveCrossParent())
			{
				isCross = true;
				ViewObj * focusPage = getEffectLogicPage();
				if (!focusPage->getViewParent())
				{
					focusPage = crossMgr->getCrossParents()->at(crossMgr->getCrossParents()->size() - 1)
						->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
				}
				BaseViewParam * pageParam = focusPage->getViewParam();
				ViewObj * layer = (ViewObj *)focusPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
					->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
				int pageAbsY = pageParam->getAbsY(mWindow);
				int rowY = crossMgr->getDisY(layer);
				int focusY = focusParam->getY();
				int preSpendH = crossMgr->getTotalPreSpendH(layer);
				int spendH = crossMgr->getSpendH(layer);
				int layerDisY = layer->getViewParam()->getAbsY(layer->getViewParent());
				destY = pageAbsY + rowY + preSpendH + layerDisY;

				int bottom = destY + spendH;
				winMaxY = getWinMaxY(winParam, winH);
				// 如果光标低于最小值

				if (destY < UIConstants::UI_TOOLBAR_H)
				{
					if (bottom < winMaxY && (bottom - UIConstants::UI_TOOLBAR_H) < UIConstants::REFRESH_FLAG_MIN_EFFECT_H)
					{
						int tempDis = UIConstants::UI_TOOLBAR_H - destY;
						pagePanelParam->addY(tempDis);
					}
				}
				else if (bottom > winMaxY && (winMaxY - destY) < UIConstants::REFRESH_FLAG_MIN_EFFECT_H)
				{
					if (destY > UIConstants::UI_TOOLBAR_H)
					{
						pagePanelParam->addY(winMaxY - bottom);
					}
				}
				//else if (destY > winMaxY)
				//{
				//	pagePanelParam->addY(winMaxY - destY);
				//}
				//else
				//{

				//	if (focusPage == pagePanel->getLastChild(false) && focusPage->getViewIndex() > 0)
				//	{
				//		int pagePanelBottom = pagePanelParam->getAbsY(mWindow) + pagePanelParam->getTotalH();
				//		if (pagePanelBottom < winMaxY - UIConstants::WindowParams::WINDOW_PAGE_PANEL_INSIDE_BOTTOM)
				//		{
				//			pagePanelParam->addY(winMaxY - UIConstants::WindowParams::WINDOW_PAGE_PANEL_INSIDE_BOTTOM - pagePanelBottom);
				//		}
				//	}
				//}
				refreshMiddleButtonPlace(pagePanelParam->getY(), winH, pagePanelParam->getTotalH());
				return;
			}
		}
		if (!isCross)
		{
			destY = focusParam->getAbsY(mWindow);
		}

		int rowH = rowParam->getTotalH();
		int bottom = destY + rowH;
		//LayoutViewParam * winParam = (LayoutViewParam *)mWindow->getViewParam();
		//int winH = winParam->getTotalH();
		//int * clip = winParam->getClip();
		//int maxY = winParam->getY() + winH - clip[3];
		winMaxY = getWinMaxY(winParam, winH);
		// 如果光标低于最小值
		if (destY < UIConstants::UI_TOOLBAR_H)
		{
			if (bottom < winMaxY && (bottom - UIConstants::UI_TOOLBAR_H) < UIConstants::REFRESH_FLAG_MIN_EFFECT_H)
			{
				pagePanelParam->addY(UIConstants::UI_TOOLBAR_H - destY);
				int pagePanelY = pagePanelParam->getAbsY(NULL);
				if (pagePanelY + pagePanelH < winMaxY)
				{
					pagePanelParam->addY(winMaxY - (pagePanelY + pagePanelH));
				}
			}
		}
		else if (destY > UIConstants::UI_TOOLBAR_H)
		{
			if (bottom > winMaxY)
			{
				if (
					(bottom - UIConstants::UI_TOOLBAR_H) < UIConstants::REFRESH_FLAG_MIN_EFFECT_H ||
					destY > winMaxY)
					pagePanelParam->addY(winMaxY - bottom);
			}
			else
			{
				ViewObj * focusPage = layer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
				if (focusPage == pagePanel->getLastChild(false) && focusPage->getViewIndex() > 0)
				{
					int pagePanelBottom = pagePanelParam->getAbsY(mWindow) + pagePanelParam->getTotalH();
					if (pagePanelBottom < winMaxY - UIConstants::WindowParams::WINDOW_PAGE_PANEL_INSIDE_BOTTOM)
					{
						pagePanelParam->addY(winMaxY - UIConstants::WindowParams::WINDOW_PAGE_PANEL_INSIDE_BOTTOM - pagePanelBottom);
						int pagePanelY = pagePanelParam->getY();
						refreshMiddleButtonPlace(pagePanelY, winH, pagePanelH);
					}
				}
				return;
			}
		}
		int pagePanelY = pagePanelParam->getY();
		refreshMiddleButtonPlace(pagePanelY, winH, pagePanelH);
	}
	// 如果在表格页内
	else if (layer->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER))
	{
		ViewObj * normalRow = layer->findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, true);
		ExcelCrossParentMgr * crossMgr = normalRow->getExcelCrossParentMgr();
		// 如果表格跨页
		if (crossMgr && crossMgr->haveCrossParent())
		{
			int focusY = focusParam->getAbsY(normalRow);
			QList<ViewObj *> * crossParents = crossMgr->getCrossParents();
			ViewObj * focusPage = getEffectLogicPage();
			BaseViewParam * pageParam = focusPage->getViewParam();
			ViewObj * layer = (ViewObj *)focusPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			int pageAbsY = pageParam->getAbsY(mWindow);
			int rowY = 0;
			//rowY += normalRow->getViewParam()->getAbsY(focusPage);
			int spendH = focusParam->getTotalH();
			//LayoutViewParam * winParam = (LayoutViewParam *)mWindow->getViewParam();
			//int winH = winParam->getTotalH();
			//int * clip = winParam->getClip();
			//int maxY = winParam->getY() + winH - clip[3];
			winMaxY = getWinMaxY(winParam, winH);
			for (int i = 0; i < crossParents->size(); i++)
			{
				layer = crossParents->at(i);
				VerticalViewParam * layerParam = (VerticalViewParam *)layer->getViewParam();
				focusPage = layer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
				pageParam = focusPage->getViewParam();
				int layerInsideTop = layerParam->getInsideTop();
				int layerInsideH = layerParam->getInsideH();
				pageAbsY = pageParam->getAbsY(mWindow);
				rowY = crossMgr->getDisY(layer);
				int tY = normalRow->getViewParam()->getAbsY(normalRow->getViewParent()->getViewParent());
				rowY += tY;
				destY = rowY + focusY;
				if (destY + spendH < layerInsideTop || destY > layerInsideTop + layerInsideH)
				{
					continue;
				}
				else
				{
					destY = pageAbsY + rowY + focusY;
					int bottom = destY + spendH;
					// 如果光标低于最小值
					if (destY < UIConstants::UI_TOOLBAR_H)
					{
						pagePanelParam->addY(UIConstants::UI_TOOLBAR_H - destY);
					}
					else
					{
						//int rowH = rowParam->getTotalH();
						if (bottom > winMaxY)
						{
							pagePanelParam->addY(winMaxY - bottom);
						}
					}
					refreshMiddleButtonPlace(pagePanelParam->getY(), winH, pagePanelH);
					return;
				}
			}
		}
		// 如果未跨页
		else
		{
			ViewObj * tempLayer = normalRow->getViewParent();
			// 如果为页内容
			if (tempLayer->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
			{
				destY = focusParam->getAbsY(mWindow);
				// 如果光标低于最小值
				if (destY < UIConstants::UI_TOOLBAR_H)
				{
					pagePanelParam->addY(UIConstants::UI_TOOLBAR_H - destY);
				}
				else
				{
					LayoutViewParam * winParam = (LayoutViewParam *)mWindow->getViewParam();
					int winH = winParam->getTotalH();
					int * clip = winParam->getClip();
					int maxY = winParam->getY() + winH - clip[3];
					int rowH = rowParam->getTotalH();
					int bottom = destY + rowH;
					if (bottom > maxY)
					{
						pagePanelParam->addY(maxY - bottom);
					}
					else
						return;
				}
			}
			// 如果为页眉页脚
			else if (tempLayer->isObj(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER))
			{
				ViewObj * focusPage = getEffectLogicPage();
				BaseViewParam * pageParam = focusPage->getViewParam();
				int pageAbsY = pageParam->getAbsY(mWindow);
				int focusY = focusParam->getAbsY(tempLayer);
				destY = pageAbsY + focusY;
				ViewObj * pagePart = tempLayer->getViewParent();
				BaseViewParam * partParam = pagePart->getViewParam();
				int partH = partParam->getTotalH();
				// 如果光标低于最小值
				if (destY < UIConstants::UI_TOOLBAR_H)
				{
					pagePanelParam->addY(UIConstants::UI_TOOLBAR_H - destY);
				}
				else
				{
					//LayoutViewParam * winParam = (LayoutViewParam *)mWindow->getViewParam();
					//int winH = winParam->getTotalH();
					//int * clip = winParam->getClip();
					//int maxY = winParam->getY() + winH - clip[3];
					////int rowH = rowParam->getTotalH();
					int bottom = destY + partH;
					winMaxY = getWinMaxY(winParam, winH);
					if (bottom > winMaxY)
					{
						pagePanelParam->addY(winMaxY - bottom);
					}
				}
			}
			int pagePanelY = pagePanelParam->getY();
			//int winH = mWindow->getViewParam()->getTotalH();
			refreshMiddleButtonPlace(pagePanelY, winH, pagePanelH);
		}
	}
	else if (layer->isObj(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER))
	{
		ViewObj * focusPage = getEffectLogicPage();
		if (!focusPage->getViewParent())
		{
			focusPage = (ViewObj *)pagePanel->getLastChild(false);
		}
		ViewObj * pagePart = layer->getViewParent();
		BaseViewParam * pageParam = focusPage->getViewParam();
		//ViewObj * layer = row->getViewParent();
		int pageAbsY = pageParam->getAbsY(mWindow);
		int focusY = focusParam->getAbsY(pagePart);
		destY = pageAbsY + focusY;
		BaseViewParam * partParam = pagePart->getViewParam();
		int partH = partParam->getTotalH();
		// 如果光标低于最小值
		if (destY < UIConstants::UI_TOOLBAR_H)
		{
			pagePanelParam->addY(UIConstants::UI_TOOLBAR_H - destY);
		}
		else
		{
			winMaxY = getWinMaxY(winParam, winH);
			int bottom = destY + partH;
			if (bottom > winMaxY)
			{
				pagePanelParam->addY(winMaxY - bottom);
			}
		}
		refreshMiddleButtonPlace(pagePanelParam->getY(), winH, pagePanelH);
		return;
	}
}
int OperateMgr::getWinMaxY(LayoutViewParam * winParam, int & winH)
{
	winH = winParam->getTotalH();
	int * clip = winParam->getClip();
	int maxY = winParam->getY() + winH - clip[3];
	return maxY;
}

void OperateMgr::setPress(char pressType, ViewObj * focus, char dir, bool isFreshStrData)
{
	if (focus && focus->isEnter())
	{
		if (dir != BaseConstants::Direction::LEFT)
		{
			dir = BaseConstants::Direction::LEFT;
		}
	}
	if (pressType == BaseConstants::MousePressType::RELEASE || pressType == BaseConstants::MousePressType::DRAG)
	{
		BaseContainer * excelLayer = focus->findViewFromParent(ConstantsID::ContainerId::EXCEL_TXT_LAYER, true);
		BaseContainer * excelLayer1 = NULL;
		// 如果当前在表格内
		if (excelLayer)
		{
			excelLayer1 = mPressStart->getFocusObj()->findViewFromParent(ConstantsID::ContainerId::EXCEL_TXT_LAYER, true);
			if (!excelLayer1)
			{
				pressType = BaseConstants::MousePressType::PRESS_AND_RELEASE;
			}
		}
		// 如果当前在表格外
		else
		{
			excelLayer1 = mPressStart->getFocusObj()->findViewFromParent(ConstantsID::ContainerId::EXCEL_TXT_LAYER, true);
			if (excelLayer1)
			{
				pressType = BaseConstants::MousePressType::PRESS_AND_RELEASE;
			}
		}
	}
	switch (pressType)
	{
	case BaseConstants::MousePressType::PRESS:
		mPressStart->setParam(focus, dir);
		break;
	case BaseConstants::MousePressType::DRAG:
		mPressEnd->setParam(focus, dir);
		break;
	case BaseConstants::MousePressType::RELEASE:
		mPressEnd->setParam(focus, dir);
		break;
	case BaseConstants::MousePressType::PRESS_AND_RELEASE:
		mPressStart->setParam(focus, dir);
		mPressEnd->setParam(focus, dir);
		break;
	}
	FlagViewParam * flagViewParam = (FlagViewParam *)mFlag->getViewParam();
	flagViewParam->setFocus(focus, dir);

	//if (isFreshStrData && focus)
	//{//处理输入元素被点击，字体变灰色的问题
	//	bool canFreshTxtColor = true;
	//	BaseTemplate * parentTemplate = focus->getTempParent();
	//	if (parentTemplate->isObj(ConstantsID::ContainerId::INPUT_TEXT_TEMPLETE))
	//	{
	//		int index = focus->getTempIndex();
	//		if (index == 0 || (index == (parentTemplate->getChildren()->size() - 1)))
	//		{
	//			canFreshTxtColor = true;
	//		}
	//		else
	//		{
	//			canFreshTxtColor = false;
	//		}
	//	}
	//	if (canFreshTxtColor)
	//	{
	//		refreshFocusTxtData();
	//	}
	//}
	repointFocusParagraphDataAndExcelVerticalAlignType(focus);
	if (focus)
	{
		ViewObj * row = focus->findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, true);
		ViewObj * layer = row->getViewParent();
		if (layer->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER))
		{
			row = layer->findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, true);
		}
		bool haveCross = false;
		if (row->isExcelRow())
		{
			ExcelCrossParentMgr * mgr = row->getExcelCrossParentMgr();
			if (mgr && mgr->haveCrossParent())
			{
				haveCross = true;
			}
		}
		if (!haveCross)
		{
			// 仅在页内容时，才设置当前页为激活页，页眉、页脚时无需设置
			if (focus->findViewFromParent(ConstantsID::ContainerId::PAGE_CONTENT, true))
			{
				ViewObj * curPage = focus->getParentPage();
				setEffectLogicPage(curPage);
			}
		}
		//if (!focus->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
		//{
		//	ViewObj * row = focus->findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, true);
		//	if (!row->getViewParent()->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER))
		//	{
		//		ExcelCrossParentMgr * mgr = row->getExcelCrossParentMgr();
		//		if (!mgr || !mgr->haveCrossParent())
		//		{
		//			ViewObj * curPage = focus->getParentPage();
		//			setEffectLogicPage(curPage);
		//		}
		//	}
		//}
	}
	mNeedRefreshPagePanelYByFlag = true;
	if (isFreshStrData)
	{
		refreshFocusTxtData();
		refreshToolbarParams();
	}
}
void OperateMgr::savePressStartAndEnd(char saveType)
{
	if (saveType == 0)
	{
		mSavedPressStart->setParam(mPressStart);
		mSavedPressEnd->setParam(mPressEnd);
	}
	else if (saveType == 1)
	{
		mSavedPressStart->setParam(mPressStart);
	}
	else if (saveType == 2)
	{
		mSavedPressEnd->setParam(mPressEnd);
	}
}
void OperateMgr::backSetPressStartAndEnd(bool freshChooseList, char saveType)
{
	if ((!mSavedPressStart->getFocusObj()) || (!mSavedPressEnd->getFocusObj()))
	{
		return;
	}
	if (saveType == 0)
	{
		mPressStart->setParam(mSavedPressStart);
		mPressEnd->setParam(mSavedPressEnd);
	}
	else if (saveType == 1)
	{
		mPressStart->setParam(mSavedPressStart);
	}
	else if (saveType == 2)
	{
		mPressEnd->setParam(mSavedPressEnd);
	}
	if (freshChooseList)
	{
		refreshChooseList(true, true, true);
		if (!needRefreshChooseList())
		{
			ViewObj * endPressView = mSavedPressEnd->getFocusObj();
			int endPressViewIdx = endPressView->getViewIndex();
			if (endPressViewIdx < 0)
			{
#ifdef DEBUG
				qdebugh() << "OperateMgr::backSetPressStartAndEnd endPressViewIdx <0";
#endif // DEBUG
				return;
			}
			ViewObj * endRow = endPressView->getViewParent();
			int endRowIdx = endRow->getViewIndex();
			if (endRowIdx < 0)
			{
#ifdef DEBUG
				qdebugh() << "OperateMgr::backSetPressStartAndEnd endRowIdx <0";
#endif // DEBUG

				return;
			}
			setPress(BaseConstants::MousePressType::RELEASE, endPressView, mSavedPressEnd->getDirection(), false);
		}
	}
}
void OperateMgr::savePressStartAndEnd2(char saveType)
{
	if (saveType == 0)
	{
		mSavedPressStart2->setParam(mPressStart);
		mSavedPressEnd2->setParam(mPressEnd);
	}
	else if (saveType == 1)
	{
		mSavedPressStart2->setParam(mPressStart);
	}
	else if (saveType == 2)
	{
		mSavedPressEnd2->setParam(mPressEnd);
	}
}
void OperateMgr::backSetPressStartAndEnd2(bool freshChooseList, char saveType)
{
	if ((!mSavedPressStart2->getFocusObj()) || (!mSavedPressEnd2->getFocusObj()))
	{
		return;
	}
	if (saveType == 0)
	{
		mPressStart->setParam(mSavedPressStart2);
		mPressEnd->setParam(mSavedPressEnd2);
	}
	else if (saveType == 1)
	{
		mPressStart->setParam(mSavedPressStart2);
	}
	else if (saveType == 2)
	{
		mPressEnd->setParam(mSavedPressEnd2);
	}
	if (freshChooseList)
	{
		refreshChooseList(true, true, true);
		if (!needRefreshChooseList())
		{
			ViewObj * endPressView = mSavedPressEnd2->getFocusObj();
			int endPressViewIdx = endPressView->getViewIndex();
			if (endPressViewIdx < 0)
			{
#ifdef DEBUG
				qdebugh() << "OperateMgr::backSetPressStartAndEnd endPressViewIdx <0";
#endif // DEBUG
				return;
			}
			ViewObj * endRow = endPressView->getViewParent();
			int endRowIdx = endRow->getViewIndex();
			if (endRowIdx < 0)
			{
#ifdef DEBUG
				qdebugh() << "OperateMgr::backSetPressStartAndEnd endRowIdx <0";
#endif // DEBUG

				return;
			}
			setPress(BaseConstants::MousePressType::RELEASE, endPressView, mSavedPressEnd2->getDirection(), false);
		}
	}
}
void OperateMgr::checkIsNeedUndo()
{
	if (mNeedUndo)
	{
		doUndo();
	}
}
int OperateMgr::getFocusRowInsideW()
{
	ViewObj * focus = mPressEnd->getFocusObj();
	ViewObj * focusRow = focus->getViewParent();
	RowViewParam * rowParam = (RowViewParam *)focusRow->getViewParam();
	return rowParam->getInsideW();
}
ViewObj * OperateMgr::getFlag()
{
	return mFlag;
}
void OperateMgr::refreshFlagLogic()
{
	mFlag->getLogicByClassID(ConstantsID::ClassId::FLAG_FLASH_LOGIC)->refreshLogic();
}
ViewObj * OperateMgr::getFocus()
{
	FlagViewParam * flagViewParam = (FlagViewParam *)mFlag->getViewParam();
	return flagViewParam->getFocus();
}
void OperateMgr::setFocusOfEndPress(ViewObj * foucus, char dir)
{
	mPressEnd->setParam(foucus, dir);
}
void OperateMgr::getFocusAndDirection(ViewObj * &focus, char &dir)
{
	FlagViewParam * flagViewParam = (FlagViewParam *)mFlag->getViewParam();
	flagViewParam->getFocusAndDirection(focus, dir);
}
void OperateMgr::getFocusAndDirectionAndPagePanY(ViewObj * &focus, char &dir, BaseViewParam *& pagePanelViewParam, int &pagePanY)
{
	getFocusAndDirection(focus, dir);
	ViewObj * pagePanel = getPagePanel();
	pagePanelViewParam = pagePanel->getViewParam();
	pagePanY = pagePanelViewParam->getY();
}
FlagParam * OperateMgr::getPressStart()
{
	return mPressStart;
}
FlagParam * OperateMgr::getPressEnd()
{
	return mPressEnd;
}
ViewObj * OperateMgr::getWindow()
{
	if (!mWindow)
	{
		ViewObj * rootView = mWriter->getRootView();
		if (!rootView)
		{
			return NULL;
		}
		mWindow = (ViewObj *)rootView->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, false);
		mWinData = (TxtWinData *)mWindow->getContentData();
	}
	return mWindow;
}
TxtWinData * OperateMgr::getWinData()
{
	if (!mWindow)
		getWindow();
	return mWinData;
}
ViewObj * OperateMgr::getPagePanel()
{
	if (!mPagePanel)
	{
		ViewObj * window = getWindow();
		mPagePanel = (ViewObj *)window->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
	}
	return mPagePanel;
}
ViewObj * OperateMgr::getEffectLoopPage()
{
	return mEffectLoopPage;
}
void OperateMgr::setEffectLogicPage(ViewObj * source)
{
	if (source->getViewIndex() == 1)
	{
		int a = 0;
	}
	mEffectLogicPage = source;
}
ViewObj * OperateMgr::getEffectLogicPage()
{
	return mEffectLogicPage;
}
void OperateMgr::setEffectLoopPage(ViewObj * source)
{
	mEffectLoopPage = source;
}
// 设置文件读写权限
bool OperateMgr::setFileAuthority(int authority)
{
	return mFileData->setFileAuthority(authority);
}

void OperateMgr::setTitleView(bool isView)
{
	if (mSystemParams->isTitleView() == isView)
		return;
	mActionMgr->createNewAction();
	ActionFactory::pushSetSysBoolParamAction(mActionMgr, BaseConstants::SetSystemParamType::SET_TITLE_VIEW_TYPE, isView);

	mSystemParams->setTitleView(isView);
}
void OperateMgr::setBottomView(bool isView)
{
	if (mSystemParams->isBottomView() == isView)
		return;

	mActionMgr->createNewAction();
	ActionFactory::pushSetSysBoolParamAction(mActionMgr, BaseConstants::SetSystemParamType::SET_BOTTOM_VIEW_TYPE, isView);

	mSystemParams->setBottomView(isView);
}
void OperateMgr::setTitleNumberViewType(char type)
{
	if (mSystemParams->getTitleNumViewType() == type)
		return;

	mActionMgr->createNewAction();
	ActionFactory::pushSetSysIntParamAction(mActionMgr, BaseConstants::SetSystemParamType::SET_TITLE_NUMBER_VIEW_TYPE, type);

	mSystemParams->setTitleNumberViewType(type, false);
}
void OperateMgr::setBottomNumberViewType(char type)
{
	if (mSystemParams->getBottomNumViewType() == type)
		return;

	mActionMgr->createNewAction();
	ActionFactory::pushSetSysBoolParamAction(mActionMgr, BaseConstants::SetSystemParamType::SET_BOTTOM_NUMBER_VIEW_TYPE, type);

	mSystemParams->setBottomNumberViewType(type, false);
}
void OperateMgr::setParagraphNumberView(bool isView)
{
	if (mSystemParams->isParagraphNumView() == isView)
		return;

	mActionMgr->createNewAction();
	ActionFactory::pushSetSysBoolParamAction(mActionMgr, BaseConstants::SetSystemParamType::SET_PARAGRAPH_NUM_VIEW_TYPE, isView);

	mSystemParams->setParagraphNumberView(isView);
}
void OperateMgr::setUpsidedown(bool upsidedown)
{
	mIsUpsidedown = upsidedown;
}
bool OperateMgr::isFree()
{
	return mStatus == BaseConstants::OperateStatus::FREE;
}
bool OperateMgr::isSaving()
{
	return mStatus == BaseConstants::OperateStatus::SAVING;
}
char OperateMgr::findContainerChildren(QList<BaseContainer *> * children, char type, QString findStr, int startIdx, int & startIndexInSourceChldren, QString replaceStr)
{
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;
	QString str;
	int start = 0;
	int end = 0;
	// 0、未找到；BaseConstants::FindType::NORMAL_TXT_CONTENT、找到且不为模板，正常处理；BaseConstants::FindType::TEMPLATE、找到，但为模板
	char haveFindSame = BaseConstants::NONE;
	QList<BaseContainer *> * excelPages = NULL;
	QList<BaseContainer *> * excelChildren = NULL;
	// 向前找
	if (type == BaseConstants::SetSystemParamType::FIND_PRE_STR || type == BaseConstants::SetSystemParamType::REPLACE_PRE_STR)
	{
		for (int j = startIdx - 1; j >= 0; j--)
		{
			obj = children->at(j);
			// 如果为普通文字
			if (obj->isObj(ConstantsID::ContainerId::TXT) || obj->isObj(ConstantsID::ContainerId::TMPLATE_TXT))
			{
				data = obj->getStrContentData();
				str = data->getString();
				if (str == findStr.at(0))
				{
					haveFindSame = BaseConstants::FindType::NORMAL_TXT_CONTENT;
					start = j + 1;
					startIndexInSourceChldren = start - 1;
					end = start + findStr.size() - 1;
					if (end <= children->size())
					{
						for (int k = start; k < end; k++)
						{
							obj = children->at(k);
							if (obj->isTxt())
							{
								data = obj->getStrContentData();
								str = data->getString();
								// 如果对比值不同，则中断后续循环
								if (str != findStr.at(k - start + 1))
								{
									haveFindSame = BaseConstants::NONE;
									break;
								}
							}
							else
							{
								haveFindSame = BaseConstants::NONE;
								break;
							}
						}
					}
					else
					{
						haveFindSame = 0;

					}

					// 如果未找到，则继续后续循环
					if (haveFindSame == 0)
					{
						continue;
					}
					// 如果匹配成功，则中断后续循环,并将当前焦点设置为选中项
					else
					{
						setPress(BaseConstants::MousePressType::RELEASE, (ViewObj *)children->at(j), BaseConstants::Direction::LEFT);
						setPress(BaseConstants::MousePressType::PRESS, (ViewObj *)children->at(j + findStr.size() - 1), BaseConstants::Direction::RIGHT);
						refreshChooseList(true, false, true);
						break;
					}
				}
			}
			// 表格
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
			{
				ExcelTemplete * excelTemp = (ExcelTemplete *)obj;
				excelPages = BaseSystem::getInstance()->getClearTempContainerList();
				excelTemp->getCellPageRootTemplate(*excelPages);
				int lastIdx = excelPages->size() - 1;
				for (int i = lastIdx; i >= 0; i--)
				{
					excelChildren = excelPages->at(i)->getChildren();
					int startIndexInSource = 0;
					haveFindSame = findContainerChildren(excelChildren, type, findStr, excelChildren->size(), startIndexInSource);
					if (haveFindSame > 0)
					{
						return haveFindSame;
					}
				}
			}
			else if (!obj->isTemp())
			{
				return haveFindSame;
			}
			// 其他模板
			else
			{
				haveFindSame = findTemplateChildren(obj, type, findStr, replaceStr);
				if (haveFindSame > 0)
				{
					return haveFindSame;
				}

			}
		}
		return haveFindSame;
	}
	// 向后找
	else
	{
		for (int j = startIdx; j < children->size(); j++)
		{
			obj = children->at(j);
			// 如果为普通文字
			if (obj->isObj(ConstantsID::ContainerId::TXT) || obj->isObj(ConstantsID::ContainerId::TMPLATE_TXT))
			{
				data = obj->getStrContentData();
				str = data->getString();
				if (str == findStr.at(0))
				{
					haveFindSame = BaseConstants::FindType::NORMAL_TXT_CONTENT;
					start = j + 1;
					end = start + findStr.size() - 1;
					for (int k = start; k < end; k++)
					{
						obj = children->at(k);
						if (obj->isTxt())
						{
							data = obj->getStrContentData();
							str = data->getString();
							// 如果对比值不同，则中断后续循环
							if (str != findStr.at(k - start + 1))
							{
								haveFindSame = BaseConstants::NONE;
								break;
							}
						}
						else
						{
							haveFindSame = BaseConstants::NONE;
							break;
						}
					}
					// 如果未找到，则继续后续循环
					if (haveFindSame == 0)
					{
						continue;
					}
					// 如果匹配成功，则中断后续循环,并将当前焦点设置为选中项
					else
					{
						setPress(BaseConstants::MousePressType::PRESS, (ViewObj *)children->at(j), BaseConstants::Direction::LEFT);
						//ViewObj * nextView = obj->getTempParent()->getChild(obj->getTempIndex() + 1)->getFirstView();
						BaseTemplate * tempParent = obj->getTempParent();
						int tempIdx = obj->getTempIndex();
						BaseTools::refreshTempContainerAndTempIdx(obj, tempParent, tempIdx);
						ViewObj * nextView = tempParent->getChild(tempIdx + 1)->getFirstView();
						setPress(BaseConstants::MousePressType::RELEASE, nextView, BaseConstants::Direction::LEFT);
						//setPress(BaseConstants::MousePressType::RELEASE, (ViewObj *)children->at(j + findStr.size() - 1), BaseConstants::Direction::RIGHT);
						refreshChooseList(true, false, true);
						break;
					}
				}
			}
			// 表格
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
			{
				ExcelTemplete * excelTemp = (ExcelTemplete *)obj;
				excelPages = BaseSystem::getInstance()->getClearTempContainerList();
				excelTemp->getCellPageRootTemplate(*excelPages);
				for (int i = 0; i < excelPages->size(); i++)
				{
					int startIndexInSource = 0;
					excelChildren = excelPages->at(i)->getChildren();
					haveFindSame = findContainerChildren(excelChildren, type, findStr, 0, startIndexInSource);
					if (haveFindSame > 0)
					{
						return haveFindSame;
					}
				}
			}
			else if (!obj->isTemp())
			{
				return haveFindSame;
			}
			// 其他模板
			else
			{

				haveFindSame = findTemplateChildren(obj, type, findStr, replaceStr);
				if (haveFindSame > 0)
				{
					return haveFindSame;
				}
			}
		}
	}

	return haveFindSame;
}
char OperateMgr::findTemplateChildren(BaseContainer * templateObj, char type, QString findStr, QString replaceStr)
{
	char haveFindSame = BaseConstants::NONE;
	//公式需要特殊处理
	if (templateObj->isObj(ConstantsID::ContainerId::MENSTRUATION_TEMPLETE_OBJ))
	{
		haveFindSame = BaseConstants::FindType::TEMPLATE;
		return haveFindSame;
	}

	QList<BaseContainer *>  tempSourcList;
	tempSourcList.push_back(templateObj);
	QList<BaseContainer *>  tempViewList;
	BaseTools::exchangeListToViewList(&tempSourcList, 0, 1, &tempViewList);
	//装原本的子视图对象
	BaseTools::copyObjListToObjList(&tempViewList, &tempSourcList, true);
	int startIndexInSource = 0;
	//去除前后的括号
	if ((!templateObj->getIsShowTidy()) &&
		(templateObj->isObj(ConstantsID::ContainerId::INPUT_TEXT_TEMPLETE) || templateObj->isObj(ConstantsID::ContainerId::STRUCTURATION_TEXT_TEMPLETE_OBJ) || templateObj->isObj(ConstantsID::ContainerId::CHOICEWIDGET_TEMPLETE)))
	{
		tempViewList.removeFirst();
		tempViewList.removeLast();
	}
	int tempStart = 0;
	if (type == BaseConstants::SetSystemParamType::FIND_PRE_STR || type == BaseConstants::SetSystemParamType::REPLACE_PRE_STR)
	{
		tempStart = tempViewList.size();
	}
	haveFindSame = findContainerChildren(&tempViewList, type, findStr, tempStart, startIndexInSource);
	if (haveFindSame > 0)
	{
		haveFindSame = BaseConstants::FindType::TEMPLATE;
	}
	else
	{
		return 0;
	}
	if (type == BaseConstants::SetSystemParamType::FIND_PRE_STR || type == BaseConstants::SetSystemParamType::FIND_NEXT_STR)
	{
		/*if (haveFindSame > 0)
		{
		haveFindSame = BaseConstants::FindType::TEMPLATE;
		}*/
		return haveFindSame;
	}
	else if (type == BaseConstants::SetSystemParamType::REPLACE_PRE_STR || type == BaseConstants::SetSystemParamType::REPLACE_NEXT_STR)
	{
		if (templateObj->isObj(ConstantsID::ContainerId::MACRO_TEMPLETE_OBJ))
		{
			/*if (haveFindSame > 0)
			{
			haveFindSame = BaseConstants::FindType::TEMPLATE;
			}*/
		}
		else if (templateObj->isObj(ConstantsID::ContainerId::FIX_TEXT_WIDGET_TEMPLETE))
		{
			/*if (haveFindSame > 0)
			{
			haveFindSame = BaseConstants::FindType::TEMPLATE;
			}*/
		}
		else if (templateObj->isObj(ConstantsID::ContainerId::INPUT_TEXT_TEMPLETE) || templateObj->isObj(ConstantsID::ContainerId::STRUCTURATION_TEXT_TEMPLETE_OBJ))
		{
			clearChooseList(true);
			BracketWidget* textWidget = (BracketWidget*)templateObj;
			QString  structStr = textWidget->getTitle();
			int index = 0;
			if (type == BaseConstants::SetSystemParamType::REPLACE_NEXT_STR)
			{
				index = structStr.indexOf(findStr);
			}
			else if (type == BaseConstants::SetSystemParamType::REPLACE_PRE_STR)
			{
				index = structStr.lastIndexOf(findStr);
			}
			if (index >= 0)
			{
				structStr.replace(index, findStr.length(), replaceStr);
				textWidget->setTitleOnShow(structStr);
			}
			textWidget->getFirstChildView()->setFocus(false, true, BaseConstants::MousePressType::PRESS);
			textWidget->getLastChildView()->setFocus(true, true, BaseConstants::MousePressType::RELEASE);
			//haveFindSame = BaseConstants::FindType::TEMPLATE;			
		}
		else if (templateObj->isObj(ConstantsID::ContainerId::CHOICEWIDGET_TEMPLETE))
		{
			clearChooseList(true);
			//不为整洁显示时
			if (!templateObj->getIsShowTidy())
			{
				ChoiceWidgetTemplete* choiceWidget = (ChoiceWidgetTemplete*)templateObj;
				choiceWidget->setIsCheckTitleShow(false);
				ViewObj* startViewObj = (ViewObj*)tempViewList.at(startIndexInSource);
				startViewObj->setFocus(false, false, BaseConstants::MousePressType::PRESS_AND_RELEASE);
				for (int i = 0; i < findStr.length(); i++)
				{
					mWriter->keyPressOnRootView(StrConstants::MyKeyBoard::DEL);
				}
				/*ViewObj* focusObj = NULL;
				char direct = 0;
				getFocusAndDirection(focusObj,direct);*/
				mWriter->keyPressOnRootView(replaceStr);
				choiceWidget->setIsCheckTitleShow(true);
				choiceWidget->getFirstChildView()->setFocus(false, true, BaseConstants::MousePressType::PRESS);
				choiceWidget->getLastChildView()->setFocus(true, true, BaseConstants::MousePressType::RELEASE);
			}

			//haveFindSame = BaseConstants::FindType::TEMPLATE;
		}
		return haveFindSame;
		//else if ()
		//{
		//	
		//}
	}
	else
	{
		return haveFindSame;
	}
}
bool OperateMgr::findStr(short type, QString findStr)
{
	ViewObj * focus = mPressEnd->getFocusObj();
	BaseContainer * tempContainer = NULL;
	BaseTemplate * tempParent = focus->getTempParent();
	int tempIdx = 0;
	BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);
	int parentIdx = tempParent->getTempIndex();
	BaseContainer * container = NULL;
	QList<BaseContainer *> * containerList = NULL;
	int size = 0;
	QList<BaseContainer *> * txtContent = NULL;
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;
	QString str = NULL;
	int start = 0;
	int end = 0;
	char haveFindSame = 0;

	// 如果光标在表格内部
	if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER))
	{
		tempContainer = tempParent;
		containerList = tempContainer->getChildren();
		size = containerList->size();
		int startIndexInSource = 0;
		// 向前找
		if (type == BaseConstants::SetSystemParamType::FIND_PRE_STR)
		{
			for (int i = tempIdx; i >= 0; i--)
			{
				haveFindSame = findContainerChildren(containerList, type, findStr, tempIdx, startIndexInSource);
				// 如果本格有
				if (haveFindSame > 0)
				{
					return true;
				}
				// 本格未找到对应项，则遍历表格前面格
				else
				{
					ExcelTemplete * excelTemp = (ExcelTemplete *)tempParent->findTempFromParent(ConstantsID::ContainerId::EXCEL_TEMPLETE, true);
					containerList = BaseSystem::getInstance()->getClearTempContainerList();
					excelTemp->getCellPageRootTemplate(*containerList);
					container = tempContainer;
					int excelPageIdx = containerList->indexOf(container);
					for (int j = excelPageIdx - 1; j >= 0; j--)
					{
						container = containerList->at(j);
						txtContent = container->getChildren();
						haveFindSame = findContainerChildren(txtContent, type, findStr, txtContent->size(), startIndexInSource);
						// 如果本表格内部前面格子内有
						if (haveFindSame > 0)
						{
							return true;
						}
					}
					// 本表格没有，则遍历从外部向前找
					focus = focus->findViewFromParent(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW, true);
					tempParent = excelTemp;
					BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);
					break;
				}
			}
		}
		// 向后找
		else
		{
			for (int i = tempIdx; i < size; i++)
			{
				haveFindSame = findContainerChildren(containerList, type, findStr, tempIdx, startIndexInSource);
				// 如果本格有
				if (haveFindSame > 0)
				{
					return true;
				}
				// 本格未找到对应项，则遍历表格前面格
				else
				{
					ExcelTemplete * excelTemp = (ExcelTemplete *)tempParent->findTempFromParent(ConstantsID::ContainerId::EXCEL_TEMPLETE, true);
					containerList = BaseSystem::getInstance()->getClearTempContainerList();
					excelTemp->getCellPageRootTemplate(*containerList);
					container = tempContainer;
					int excelPageIdx = containerList->indexOf(container);
					for (int j = excelPageIdx + 1; j < containerList->size(); j++)
					{
						container = containerList->at(j);
						txtContent = container->getChildren();
						haveFindSame = findContainerChildren(txtContent, type, findStr, 0, startIndexInSource);
						// 如果本表格内部前面格子内有
						if (haveFindSame > 0)
						{
							return true;
						}
					}
					// 本表格没有，则遍历从外部向前找
					focus = focus->findViewFromParent(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW, true);
					tempParent = excelTemp;
					BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);
					tempIdx++;
					break;
				}
			}
		}

	}

	// 光标在表格外部
	tempContainer = tempParent->getTempParent();
	parentIdx = tempParent->getTempIndex();
	containerList = tempContainer->getChildren();
	size = containerList->size();
	int startIndexInSource = 0;
	// 向前找
	if (type == BaseConstants::SetSystemParamType::FIND_PRE_STR)
	{
		for (int i = parentIdx; i >= 0; i--)
		{
			container = containerList->at(i);
			txtContent = container->getChildren();
			haveFindSame = findContainerChildren(txtContent, type, findStr, tempIdx, startIndexInSource);
			if (haveFindSame > 0)
			{
				return true;
			}
			// 未找到对应项
			else
			{
				// 如果光标在excel内部
				if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER))
				{
					ExcelTemplete * excelTemp = (ExcelTemplete *)tempParent->findTempFromParent(ConstantsID::ContainerId::EXCEL_TEMPLETE, true);
					containerList = BaseSystem::getInstance()->getClearTempContainerList();
					excelTemp->getCellPageRootTemplate(*containerList);
					int excelPageIdx = containerList->indexOf(container);
					for (int j = excelPageIdx - 1; j >= 0; j--)
					{
						container = containerList->at(j);
						txtContent = container->getChildren();
						haveFindSame = findContainerChildren(txtContent, type, findStr, tempIdx, startIndexInSource);
					}
					if (haveFindSame > 0)
					{
						return true;
					}
				}
			}
		}
	}
	// 向后找
	else
	{
		for (int i = parentIdx; i < size; i++)
		{
			container = containerList->at(i);
			txtContent = container->getChildren();
			haveFindSame = findContainerChildren(txtContent, type, findStr, tempIdx, startIndexInSource);
			if (haveFindSame > 0)
			{
				return true;
			}
			// 未找到对应项
			else
			{
				// 如果光标在excel内部
				if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER))
				{
					ExcelTemplete * excelTemp = (ExcelTemplete *)tempParent->findTempFromParent(ConstantsID::ContainerId::EXCEL_TEMPLETE, true);
					containerList = BaseSystem::getInstance()->getClearTempContainerList();
					excelTemp->getCellPageRootTemplate(*containerList);
					int excelPageIdx = containerList->indexOf(container);
					for (int j = excelPageIdx + 1; j < containerList->size(); j++)
					{
						container = containerList->at(j);
						txtContent = container->getChildren();
						haveFindSame = findContainerChildren(txtContent, type, findStr, tempIdx, startIndexInSource);
					}
					if (haveFindSame > 0)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}
bool OperateMgr::replaceStr(short type, QString findStr, QString replaceStr, bool needCreateAction)
{
	if (type == BaseConstants::SetSystemParamType::REPLACE_ALL_STR)
	{
		ViewObj * preStart = mPressStart->getFocusObj();
		char preStartDir = mPressStart->getDirection();
		ViewObj * preEnd = mPressEnd->getFocusObj();
		char preEndDir = mPressEnd->getDirection();
		char isSuccessful = 0;

		if (needCreateAction)
			mActionMgr->createNewAction();
		char temp = this->replaceStr(BaseConstants::SetSystemParamType::REPLACE_PRE_STR, findStr, replaceStr, false);
		if (temp > 0)
		{
			isSuccessful = true;
		}
		while (temp > 0)
		{
			temp = this->replaceStr(BaseConstants::SetSystemParamType::REPLACE_PRE_STR, findStr, replaceStr, false);
		}
		setPress(BaseConstants::MousePressType::PRESS, preStart, preStartDir);
		setPress(BaseConstants::MousePressType::RELEASE, preEnd, preEndDir);
		refreshChooseList(true, false, true);

		temp = this->replaceStr(BaseConstants::SetSystemParamType::REPLACE_NEXT_STR, findStr, replaceStr, false);
		if (temp > 0)
		{
			if (isSuccessful == 0)
				isSuccessful = temp;
		}

		if (isSuccessful == 0)
		{
			mActionMgr->delAction();
		}
		while (temp > 0)
		{
			temp = this->replaceStr(BaseConstants::SetSystemParamType::REPLACE_NEXT_STR, findStr, replaceStr, false);
		}
		if (isSuccessful == 0)
		{
		}
		return isSuccessful;
	}
	ViewObj * focus = mPressEnd->getFocusObj();
	BaseContainer * tempContainer = NULL;
	BaseTemplate * tempParent = focus->getTempParent();
	int tempIdx = 0;
	BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);
	int parentIdx = tempParent->getTempIndex();
	BaseContainer * container = NULL;
	QList<BaseContainer *> * containerList = NULL;
	int size = 0;
	QList<BaseContainer *> * txtContent = NULL;
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;
	QString str = NULL;
	int start = 0;
	int end = 0;
	char haveFindSame = 0;

	int startIndexInSource = 0;
	// 如果光标在表格内部
	if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER))
	{
		tempContainer = tempParent;
		containerList = tempContainer->getChildren();
		size = containerList->size();
		// 向前找
		if (type == BaseConstants::SetSystemParamType::REPLACE_PRE_STR)
		{
			for (int i = tempIdx; i >= 0; i--)
			{
				haveFindSame = findContainerChildren(containerList, type, findStr, tempIdx, startIndexInSource, replaceStr);
				// 如果本格有，且不为模板
				if (haveFindSame > 0)
				{
					if (haveFindSame == BaseConstants::FindType::NORMAL_TXT_CONTENT)
						addStr(replaceStr, needCreateAction);
					return true;
				}
				// 本格未找到对应项，则遍历表格前面格
				else
				{
					ExcelTemplete * excelTemp = (ExcelTemplete *)tempParent->findTempFromParent(ConstantsID::ContainerId::EXCEL_TEMPLETE, true);
					containerList = BaseSystem::getInstance()->getClearTempContainerList();
					excelTemp->getCellPageRootTemplate(*containerList);
					container = tempContainer;
					int excelPageIdx = containerList->indexOf(container);
					for (int j = excelPageIdx - 1; j >= 0; j--)
					{
						container = containerList->at(j);
						txtContent = container->getChildren();
						haveFindSame = findContainerChildren(txtContent, type, findStr, txtContent->size(), startIndexInSource, replaceStr);
						// 如果本表格内部前面格子内有
						if (haveFindSame > 0)
						{
							if (haveFindSame == BaseConstants::FindType::NORMAL_TXT_CONTENT)
								addStr(replaceStr, needCreateAction);
							return true;
						}
					}
					// 本表格没有，则遍历从外部向前找
					focus = focus->findViewFromParent(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW, true);
					tempParent = excelTemp;
					BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);
					break;
				}
			}
		}
		// 向后找
		else if (type == BaseConstants::SetSystemParamType::REPLACE_NEXT_STR)
		{
			for (int i = tempIdx; i < size; i++)
			{
				haveFindSame = findContainerChildren(containerList, type, findStr, tempIdx, startIndexInSource, replaceStr);
				// 如果本格有
				if (haveFindSame > 0)
				{
					if (haveFindSame == BaseConstants::FindType::NORMAL_TXT_CONTENT)
						addStr(replaceStr, needCreateAction);
					return true;
				}
				// 本格未找到对应项，则遍历表格前面格
				else
				{
					ExcelTemplete * excelTemp = (ExcelTemplete *)tempParent->findTempFromParent(ConstantsID::ContainerId::EXCEL_TEMPLETE, true);
					containerList = BaseSystem::getInstance()->getClearTempContainerList();
					excelTemp->getCellPageRootTemplate(*containerList);
					container = tempContainer;
					int excelPageIdx = containerList->indexOf(container);
					for (int j = excelPageIdx + 1; j < containerList->size(); j++)
					{
						container = containerList->at(j);
						txtContent = container->getChildren();
						haveFindSame = findContainerChildren(txtContent, type, findStr, 0, startIndexInSource, replaceStr);
						// 如果本表格内部前面格子内有
						if (haveFindSame > 0)
						{
							if (haveFindSame == BaseConstants::FindType::NORMAL_TXT_CONTENT)
								addStr(replaceStr, needCreateAction);
							return true;
						}
					}
					// 本表格没有，则遍历从外部向前找
					focus = focus->findViewFromParent(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW, true);
					tempParent = excelTemp;
					BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);
					tempIdx++;
					break;
				}
			}
		}

	}

	// 光标在表格外部
	tempContainer = tempParent->getTempParent();
	parentIdx = tempParent->getTempIndex();
	containerList = tempContainer->getChildren();
	size = containerList->size();
	// 向前找
	if (type == BaseConstants::SetSystemParamType::REPLACE_PRE_STR)
	{
		for (int i = parentIdx; i >= 0; i--)
		{
			container = containerList->at(i);
			txtContent = container->getChildren();
			haveFindSame = findContainerChildren(txtContent, type, findStr, tempIdx, startIndexInSource, replaceStr);
			if (haveFindSame > 0)
			{
				if (haveFindSame == BaseConstants::FindType::NORMAL_TXT_CONTENT)
					addStr(replaceStr, needCreateAction);
				return true;
			}
			// 未找到对应项
			else
			{
				// 如果光标在excel内部
				if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER))
				{
					ExcelTemplete * excelTemp = (ExcelTemplete *)tempParent->findTempFromParent(ConstantsID::ContainerId::EXCEL_TEMPLETE, true);
					containerList = BaseSystem::getInstance()->getClearTempContainerList();
					excelTemp->getCellPageRootTemplate(*containerList);
					int excelPageIdx = containerList->indexOf(container);
					for (int j = excelPageIdx - 1; j >= 0; j--)
					{
						container = containerList->at(j);
						txtContent = container->getChildren();
						haveFindSame = findContainerChildren(txtContent, type, findStr, tempIdx, startIndexInSource, replaceStr);
					}
					if (haveFindSame > 0)
					{
						if (haveFindSame == BaseConstants::FindType::NORMAL_TXT_CONTENT)
							addStr(replaceStr, needCreateAction);
						return true;
					}
				}
			}
		}
	}
	// 向后找
	else if (type == BaseConstants::SetSystemParamType::REPLACE_NEXT_STR)
	{
		for (int i = parentIdx; i < size; i++)
		{
			container = containerList->at(i);
			txtContent = container->getChildren();
			haveFindSame = findContainerChildren(txtContent, type, findStr, tempIdx, startIndexInSource, replaceStr);
			if (haveFindSame > 0)
			{
				if (haveFindSame == BaseConstants::FindType::NORMAL_TXT_CONTENT)
					addStr(replaceStr, needCreateAction);
				return true;
			}
			// 未找到对应项
			else
			{
				// 如果光标在excel内部
				if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER))
				{
					ExcelTemplete * excelTemp = (ExcelTemplete *)tempParent->findTempFromParent(ConstantsID::ContainerId::EXCEL_TEMPLETE, true);
					containerList = BaseSystem::getInstance()->getClearTempContainerList();
					excelTemp->getCellPageRootTemplate(*containerList);
					int excelPageIdx = containerList->indexOf(container);
					for (int j = excelPageIdx + 1; j < containerList->size(); j++)
					{
						container = containerList->at(j);
						txtContent = container->getChildren();
						haveFindSame = findContainerChildren(txtContent, type, findStr, tempIdx, startIndexInSource, replaceStr);
					}
					if (haveFindSame > 0)
					{
						if (haveFindSame == BaseConstants::FindType::NORMAL_TXT_CONTENT)
							addStr(replaceStr, needCreateAction);
						return true;
					}
				}
			}
		}
	}

	return false;
}
void OperateMgr::pressLeft()
{
	ViewObj * focus = mPressEnd->getFocusObj();
	char dir = mPressEnd->getDirection();
	int idx = focus->getViewIndex();
	if (idx == 0 && dir == BaseConstants::Direction::RIGHT)
	{
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::LEFT);
	}
	else
	{
		ViewObj * dest = focus->getPreViewFromViewParent();
		dest = focus->getPreViewFromParagraphParent();
		if (!dest)
		{
			dest = focus;
		}
		else if (dest->isPageNumber() && !dest->getViewParam()->isView())
		{
			dest = dest->getPreViewFromParagraphParent();
			if (!dest)
			{
				dest = focus;
			}
		}
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, dest, dir);
	}
	refreshFlagLogic();
}
void OperateMgr::pressRight()
{
	ViewObj * focus = mPressEnd->getFocusObj();
	char dir = mPressEnd->getDirection();
	int idx = focus->getViewIndex();
	int size = 0;
	QList<BaseContainer *> * children = focus->getViewParent()->getChildren();
	if (idx == children->size() - 1 && dir == BaseConstants::Direction::LEFT && !focus->isEnter())
	{
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::RIGHT);
	}
	else
	{
		ViewObj * dest = focus->getNextViewFromViewParent();
		dest = focus->getNextViewFromParagraphParent();
		if (!dest)
		{
			dest = focus;
		}
		else if (dest->isPageNumber() && !dest->getViewParam()->isView())
		{
			dest = dest->getNextViewFromParagraphParent();
			if (!dest)
			{
				dest = focus;
			}
		}
		if (dest->isEnter())
		{
			if (dir == BaseConstants::Direction::LEFT)
			{
				dir = BaseConstants::Direction::RIGHT;
			}
		}
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, dest, dir);
	}
	refreshFlagLogic();
}
void OperateMgr::pressUp()
{
	ViewObj * dest = refreshNearestObj(BaseConstants::Direction::UP);
	if (!dest)
	{
		return;
	}
	if (dest->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
	{
		ExcelCrossParentMgr * mgr = dest->getExcelCrossParentMgr();
		if (mgr && mgr->haveCrossParent())
		{
			ViewObj * focusPage = mgr->getLastCrossParent();
			setEffectLogicPage(focusPage);
		}
	}
}
void OperateMgr::pressDown()
{
	ViewObj * dest = refreshNearestObj(BaseConstants::Direction::DOWN);
	if (!dest)
	{
		return;
	}
	if (dest->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
	{
		ViewObj * row = dest->findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, true);
		ExcelCrossParentMgr * mgr = row->getExcelCrossParentMgr();
		if (mgr && mgr->haveCrossParent())
		{
			ViewObj * layer = mgr->getFirstCrossParent();
			ViewObj * focusPage = layer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
			setEffectLogicPage(focusPage);
		}
	}
}
void OperateMgr::pressEnter(int userID, bool needCreateAction)
{
	ViewObj * focus = mPressEnd->getFocusObj();
	char dir = mPressEnd->getDirection();
	int tempIdx = -1;
	BaseTemplate * tempParent = focus->getTempParent();
	if (!judgeCurFocusCanWrite(focus, dir, tempParent, tempIdx, false, true))
	{
		return;
	}
	if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
	{
		tempIdx = tempParent->getTempIndex();
		tempParent = tempParent->getTempParent();
	}
	if (needCreateAction)
		mActionMgr->createNewAction();

	if (mChooseList->size() > 0)
	{
		pressBackspace(BaseConstants::DoActionType::ONLY_ADD_ACTION);
	}
	ViewObj * preStartFocus = mPressStart->getFocusObj();
	char preStartDir = mPressStart->getDirection();
	ViewObj * preEndFocus = mPressEnd->getFocusObj();
	char preEndDir = mPressEnd->getDirection();


	ViewObj * next = NULL;
	focus = mPressEnd->getFocusObj();
	dir = mPressEnd->getDirection();
	tempParent = focus->getTempParent();
	BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);
	//if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
	//{
	//	tempIdx = tempParent->getTempIndex();
	//	tempParent = tempParent->getTempParent();
	//}
	if (dir == BaseConstants::Direction::RIGHT)
	{
		tempIdx++;
		next = focus->getNextViewFromViewParent();
	}
	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	ViewObj * enter = ViewFactory::createEnter(mWriter, userID);
	tempMgr->addChild(enter, tempIdx, true, NULL, -1);

	if (enter->getTempIndex() != enter->getViewIndex())
	{
		int a = 0;
	}
	if (dir == BaseConstants::Direction::RIGHT)
	{
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, next, BaseConstants::Direction::LEFT);
	}
	else
		repointFocusParagraphDataAndExcelVerticalAlignType(focus);

	ActionFactory::pushAddObjAction(mActionMgr, enter, preEndFocus, preEndDir);

	refreshFlagLogic();
	if (mNeedUndo)
	{
		doUndo();
	}
	else
	{
		if (preStartFocus == focus && preStartDir == dir)
			mNeedRefreshPagePanelYByFlag = true;
		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
	}
}
int OperateMgr::pressBackspace(char addActionType, bool isJudgeType)
{
	ViewObj * focus = mPressEnd->getFocusObj();
	char dir = mPressEnd->getDirection();
	int tempIdx = -1;
	BaseTemplate * tempParent = focus->getTempParent();
	if (!judgeCurFocusCanWrite(focus, dir, tempParent, tempIdx, true, true))
	{
		return -1;
	}
	if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
	{
		tempIdx = tempParent->getTempIndex();
		tempParent = tempParent->getTempParent();
	}
	// 如果当前有选中项，则删除aaaa选中项列表
	if (mChooseList->size() > 0)
	{
		return delChooseList(addActionType, isJudgeType);
	}
	ViewObj *preStartFocus = mPressStart->getFocusObj();
	char preStartDir = mPressStart->getDirection();
	ViewObj *preEndFocus = mPressEnd->getFocusObj();
	char preEndDir = mPressEnd->getDirection();

	//BaseContainer* tempParent = focus->getTempParent();
	ViewObj * next = focus;
	ViewObj * dest = next;

	BaseContainer * destTemp = NULL;
	if (dir == BaseConstants::Direction::LEFT)
	{
		if (tempIdx > 0)
		{
			tempIdx--;
			destTemp = tempParent->getChild(tempIdx);
			if (!judgeCanDelObj(destTemp, ActionTypeOnDeleteObj::BackSpace))
			{
				if (ConstantsID::ContainerId::EXCEL_TEMPLETE == destTemp->getObjID())
				{

				}
				else
				{
					pressLeft();
				}

				return -1;
			}
			//dest = tempParent->getChild(tempIdx - 1)->getLastChildView(true);
		}
		else
		{
			return -1;
		}
		//dest = focus->getPreViewFromParagraphParent();
	}
	else
	{
		destTemp = tempParent->getChild(tempIdx);
		if (!judgeCanDelObj(destTemp, ActionTypeOnDeleteObj::BackSpace))
		{
			if (ConstantsID::ContainerId::EXCEL_TEMPLETE == destTemp->getObjID())
			{

			}
			else
			{
				pressLeft();
			}
			return -1;
		}
		next = tempParent->getChild(tempIdx + 1)->getFirstChildView(true);
		//next = focus->getNextViewFromParagraphParent();
		if (next->isPageNumber())
		{
			if (!next->getViewParam()->isView())
			{
				next = next->getNextViewFromParagraphParent();
				//dir = BaseConstants::Direction::LEFT;
			}
		}
	}
	if (!dest)
	{
		return -1;
	}
	else if (destTemp->isEnter())
	{
		if (!judgeCanDelEnter(destTemp))
			return -1;
	}
	else if (destTemp->isPageNumber())
	{
		ViewObj * pageNum = (ViewObj *)destTemp;
		// 如果pageNumber处于显示状态则返回
		if (pageNum->getViewParam()->isView())
		{
			return -1;
		}
		pageNum = pageNum->getPreViewFromParagraphParent();
		if (!pageNum)
		{
			return -1;
		}
	}
	if (!judgeCanDelFixTempOnMedicalRecod(destTemp))
	{
		return -1;
	}
	ViewObj * preEffectLogicPage = mEffectLogicPage;
	if (addActionType == BaseConstants::DoActionType::CREATE_STEP_AND_ADD_ACTION)
		mActionMgr->createNewAction();
	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	BaseParagraph * nextParagraph = NULL;
	if (destTemp->isEnter())
	{
		nextParagraph = destTemp->getParagraphParent()->getNextParagraph();
	}
	tempMgr->delChild(destTemp, true);
	//tempMgr->delChild(dest, true);
	if (dir == BaseConstants::Direction::RIGHT)
	{
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, next, BaseConstants::Direction::LEFT);
	}
	else
		repointFocusParagraphDataAndExcelVerticalAlignType(focus);

	if (addActionType >= BaseConstants::DoActionType::ONLY_ADD_ACTION)
		ActionFactory::pushDelObjAction(mActionMgr, nextParagraph, preEffectLogicPage, destTemp, preStartFocus, preStartDir, preEndFocus, preEndDir
		, mPressEnd->getFocusObj(), mPressEnd->getDirection(), mIsUpsidedown);
	mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
	return 1;
}
int OperateMgr::pressDel(char addActionType, bool isJudgeType)
{
	ViewObj * focus = mPressEnd->getFocusObj();
	char dir = mPressEnd->getDirection();

	BaseTemplate * tempParent = focus->getTempParent();
	ViewObj * afterFocus = focus;
	char afterDir = BaseConstants::Direction::RIGHT;

	int tempIdx = -1;
	if (!judgeCurFocusCanWrite(focus, dir, tempParent, tempIdx, false, true))
	{
		return -1;
	}
	if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
	{
		tempIdx = tempParent->getTempIndex();
		tempParent = tempParent->getTempParent();
	}
	// 如果当前有选中项，则删除选中项列表
	if (mChooseList->size() > 0)
	{
		return delChooseList(addActionType, isJudgeType);
	}

	ViewObj *preStartFocus = mPressStart->getFocusObj();
	char preStartDir = mPressStart->getDirection();
	ViewObj *preEndFocus = mPressEnd->getFocusObj();
	char preEndDir = mPressEnd->getDirection();



	ViewObj * dest = focus;
	BaseContainer * destTemp = NULL;
	if (dir == BaseConstants::Direction::RIGHT)
	{
		tempIdx++;
		destTemp = tempParent->getChild(tempIdx);
		if (!judgeCanDelObj(destTemp, ActionTypeOnDeleteObj::Delete))
		{
			if (ConstantsID::ContainerId::EXCEL_TEMPLETE == destTemp->getObjID())
			{

			}
			else
			{
				pressRight();
			}
			return -1;
		}
		dest = destTemp->getLastChildView(true);
		//dest = focus->getNextViewFromViewParent();
	}
	else
	{
		destTemp = tempParent->getChild(tempIdx);
		if (!judgeCanDelObj(destTemp, ActionTypeOnDeleteObj::Delete))
		{
			if (ConstantsID::ContainerId::EXCEL_TEMPLETE == destTemp->getObjID())
			{

			}
			else
			{
				pressRight();
			}
			return -1;
		}
		//afterFocus = focus->getPreViewFromViewParent();
		if (tempIdx > 0)
		{
			BaseContainer * tempDest = tempParent->getChild(tempIdx - 1);
			if (tempDest->isEnter())
			{
				BaseContainer * nextView = destTemp->getNextView();
				if (nextView)
				{
					tempDest = nextView;
					afterDir = BaseConstants::Direction::LEFT;
				}
			}
			afterFocus = tempDest->getLastChildView(true);
		}
		else
		{
			QList<BaseContainer *> * tempChildren = tempParent->getChildren();
			if (tempIdx < tempChildren->size() - 1)
			{
				afterFocus = tempChildren->at(tempIdx + 1)->getFirstChildView(true);
				//if (afterFocus && afterFocus->isPageNumber())
				{
					if (!afterFocus->getViewParam()->isView())
					{
						afterFocus = afterFocus->getNextViewFromParagraphParent();
					}
				}
			}
			//afterFocus = focus->getNextViewFromViewParent();
			afterDir = BaseConstants::Direction::LEFT;
		}
	}
	if (!judgeCanDelObj(destTemp, ActionTypeOnDeleteObj::Delete))
	{
		return -1;
	}
	if (!judgeCanDelFixTempOnMedicalRecod(destTemp))
	{
		return -1;
	}
	if (!dest)
	{
		return -1;
	}
	else if (dest->isPageNumber())
	{
		// 如果pageNumber处于显示状态则返回
		if (dest->getViewParam()->isView())
			return -1;
		dest = dest->getNextViewFromParagraphParent();
		if (!dest)
		{
			return -1;
		}
	}
	else if (dest->isEnter())
	{
		if (!judgeCanDelEnter(dest))
			return -1;
		//BaseParagraph * paragraph = dest->getParagraphParent();
		//if (paragraph->isLastParagraph())
		//	return -1;
	}
	if (addActionType == BaseConstants::DoActionType::CREATE_STEP_AND_ADD_ACTION)
		mActionMgr->createNewAction();
	ViewObj *preEffectLogicPage = mEffectLogicPage;
	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	BaseParagraph * nextParagraph = NULL;
	if (destTemp->isEnter())
	{
		nextParagraph = destTemp->getParagraphParent()->getNextParagraph();
	}
	tempMgr->delChild(destTemp, true);
	if (dir == BaseConstants::Direction::LEFT)
	{
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, afterFocus, afterDir);
	}
	else
		repointFocusParagraphDataAndExcelVerticalAlignType(focus);
	if (addActionType >= BaseConstants::DoActionType::ONLY_ADD_ACTION)
	{
		ActionFactory::pushDelObjAction(mActionMgr, nextParagraph, preEffectLogicPage, dest, preStartFocus, preStartDir, preEndFocus, preEndDir
			, mPressEnd->getFocusObj(), mPressEnd->getDirection(), mIsUpsidedown);
	}
	mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
	return 1;
}
void OperateMgr::doUndo()
{
	if (mNeedUndo)
	{
		mNeedUndo = false;
		mActionMgr->undo(true);
	}
	else
		mActionMgr->undo();
}
void OperateMgr::doRedo()
{
	mActionMgr->redo();
}
void OperateMgr::preliminaryTreatmentChooseList(bool& isContinue)
{
	if (mChooseList && mChooseList->size() > 0)
	{
		bool isAllExcel = true;
		bool isAllNotExcel = true;
		bool havePageNum = false;
		for (int i = 0; i < mChooseList->size(); i++)
		{
			BaseContainer * obj = mChooseList->at(i);
			if (obj->isNode())
			{
				isContinue = false;
				return;
			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
			{
				isAllNotExcel = false;
			}
			else
			{
				isAllExcel = false;
				if (obj->isPageNumber())
				{
					havePageNum = true;
					break;
				}
				//break;
			}
		}
		if (isAllExcel)
		{
			BaseContainer * obj = mChooseList->at(0);
			int tempParentId = 0;
			ExcelTemplete * excelTemp = (ExcelTemplete*)obj->findTempFromParent(ConstantsID::ContainerId::EXCEL_TEMPLETE, true, tempParentId);
			if (excelTemp)
			{
				if (ExcelTemplete::isMyCell(mChooseList, excelTemp) && mChooseList->size() == excelTemp->getCellSize())
				{
					clearChooseList(true);
					ViewObj* focusObj = excelTemp->getLastChildView();
					setPress(BaseConstants::MousePressType::PRESS, focusObj, BaseConstants::Direction::LEFT);
					setPress(BaseConstants::MousePressType::RELEASE, focusObj, BaseConstants::Direction::RIGHT);
					refreshChooseList(true, true, true);
					//addToChooseList(excelTemp);
					mWriter->setWindowPressState(BaseConstants::ObjStatus::DRAG_CHOOSE);
					isContinue = true;
					return;
				}
				else
				{
					isContinue = false;
					return;
				}
			}
			else
			{
				isContinue = false;
				return;
			}

		}
		else
		{
			if (isAllNotExcel)
			{
				//如果全部都不是excel cell，还是要继续执行
				if (havePageNum)
					isContinue = false;
				else
					isContinue = true;
				return;
			}
			else
			{
				isContinue = false;
				return;
			}

		}
	}
	else
	{
		isContinue = false;
		return;
	}
}
void OperateMgr::doCopy()
{

	/*int ix = 1;
	if (ix)
	{
	QByteArray bytArray;
	QBuffer buffer(&bytArray);
	buffer.open(QBuffer::ReadWrite);
	QDataStream in(&buffer);
	QString str = "how hello world";
	in << str;
	buffer.close();
	BaseSystem::getInstance()->saveToClipboard(&bytArray);
	return;
	}*/
	if (mChooseList->size() < 1)
	{
		return;
	}
	bool isContinue;
	preliminaryTreatmentChooseList(isContinue);
	if (isContinue)
	{
		BaseSystem::getInstance()->setCopyList(mWriter, mChooseList, false, true, NULL);
	}
}
void OperateMgr::doCut()
{
	if (mChooseList->size() < 1)
	{
		return;
	}
	//mActionMgr->createNewAction();
	BaseContainer * obj = NULL;
	BaseTemplate * tempParent = NULL;
	for (int i = 0; i < mChooseList->size(); i++)
	{
		obj = mChooseList->at(i);
		tempParent = obj->getTempParent();
		if (!tempParent->canWrite())
		{
			return;
		}
	}

	resetFocusWhenCopy();
	bool isContinue;
	preliminaryTreatmentChooseList(isContinue);
	if (isContinue)
	{
		BaseSystem::getInstance()->setCopyList(mWriter, mChooseList, false, true, NULL);
		delChooseList(BaseConstants::DoActionType::CREATE_STEP_AND_ADD_ACTION, true);
		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
	}
}
void OperateMgr::doPast()
{
	int size = 0;

	ViewObj * focus = mPressEnd->getFocusObj();
	char dir = mPressEnd->getDirection();
	int tempIdx = -1;
	BaseTemplate * tempParent = focus->getTempParent();
	BaseTemplate * preTempParent = tempParent;
	if (!judgeCurFocusCanWrite(focus, dir, tempParent, tempIdx, false, true))
	{
		return;
	}
	if (!preTempParent->isTempContainer() && !preTempParent->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
	{
		return;
	}

	QByteArray bytArray;
	BaseSystem::getInstance()->loadFromClipboard(&bytArray);
	if (bytArray.size() > 0)
	{
		mIOTools->setByteArray(&bytArray);
		mIOTools->readFile();
		int paragraphSize = mIOTools->readInt();
		if (paragraphSize > 0)
		{
			long copyId = mIOTools->readInt();
			if (copyId != BaseSystem::getInstance()->getSystemId())
			{
				QList<BaseContainer *> * copyList = new QList<BaseContainer *>();
				BaseContainer * obj = NULL;
				ParagraphData * data = NULL;
				ParagraphData * preData = NULL;
				QList<ParagraphData *> * paragraphDataList = new QList<ParagraphData *>();
				for (int i = 0; i < paragraphSize; i++)
				{
					int objID = tempParent->getObjID();
					BaseContainer * tParent = tempParent;
					if (objID >= ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER)
					{
						tParent = tempParent->getTempParent();
					}
					TempParagraphMgrData * paragraphMgrData = (TempParagraphMgrData *)tParent->getContentData();
					BaseParagraph * paragraph = (BaseParagraph *)ContentDataFactory::loadData(mWriter, mIOTools, paragraphMgrData->getOwner());
					data = paragraph->getParagraphData();
					if (data != preData)
					{
						preData = data;
						paragraphDataList->push_back(data);
					}
				}

				size = mIOTools->readInt();
				for (int i = 0; i < size; i++)
				{
					obj = ContentDataFactory::loadData(mWriter, mIOTools, tempParent, 2);
					copyList->push_back(obj);
				}
				BaseSystem::getInstance()->setCopyList(mWriter, copyList, false, false, paragraphDataList);

				paragraphDataList->clear();
				delete paragraphDataList;
			}
		}
		mIOTools->close();
	}

	QList<BaseContainer *> * systemCopyList = BaseSystem::getInstance()->getCopyList();



	if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
	{
		tempIdx = tempParent->getTempIndex();
		tempParent = tempParent->getTempParent();
	}
	// 如果当前焦点在表格层里
	BaseContainer * row = focus->getViewParent();
	if (row)
	{
		BaseContainer * layer = row->getViewParent();
		if (layer->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER))
		{
			bool haveExcel = BaseTools::haveExcel(systemCopyList);
			if (haveExcel)
			{
				return;
			}
			//判断如果加入表格里 是否内容纳下
			if (!canAddTemplateList(*systemCopyList, focus))
			{
				//tempParent->getChildrenMgr()->delChild(templete, true);
				MessageBox::showErrorMessageBox(QStringLiteral("加入错误"), QStringLiteral("表格不能容纳某些元素"));
				return;
			}
		}
	}
	QList<BaseContainer *> * list = BaseSystem::getInstance()->createCopyFromList(mWriter, systemCopyList);
	if (!list)
	{
		return;
	}
	mActionMgr->createNewAction();
	// 如果当前有选中项，则删除选中项列表，并刷新光标
	if (mChooseList->size() > 0)
	{
		// 如果有选中项不能被删除，则取消粘贴
		if (delChooseList(BaseConstants::DoActionType::ONLY_ADD_ACTION, true) < 1)
		{
			mActionMgr->delAction();
			return;
		}
		resetFocusWhenCopy();
	}
	//BaseSystem::getInstance()->clearBufferParagraphDataIndex();
	tempParent = focus->getTempParent();
	BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);
	//if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
	//{
	//	tempIdx = tempParent->getTempIndex();
	//	tempParent = tempParent->getTempParent();
	//}
	dir = mPressEnd->getDirection();
	//ViewObj * focus = mPressEnd->getFocusObj();
	//char dirFocus = mPressEnd->getDirection();
	ViewObj * preStartFocus = mPressStart->getFocusObj();
	char preStartDir = mPressStart->getDirection();
	ViewObj * preEndFocus = mPressEnd->getFocusObj();
	char preEndDir = mPressEnd->getDirection();

	if (dir == BaseConstants::Direction::RIGHT)
	{
		tempIdx++;
	}

	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	size = list->size();

	tempMgr->addChildren(list, 0, size, tempIdx, true, BaseSystem::getInstance()->getBufParagraphDataForCopyList(), 1);

	if (dir == BaseConstants::Direction::RIGHT)
	{
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, (ViewObj *)list->at(size - 1)->getLastChildView(true)
			, BaseConstants::Direction::RIGHT);
	}
	ActionFactory::pushAddObjListAction(mActionMgr, list, preEndFocus, preEndDir);


	if (mNeedUndo)
	{
		doUndo();
	}
	else
	{
		mNeedRefreshPagePanelYByFlag = true;
		mPagePanel->changeStatus(BaseConstants::ObjStatus::FREE, false);
		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
	}
}
bool OperateMgr::judgeCurFocusCanWrite(bool isPressBackspace, bool needJudgeFileType)
{
	ViewObj * focus = mPressEnd->getFocusObj();
	char dir = mPressEnd->getDirection();
	int tempIdx = -1;
	BaseTemplate * tempParent = focus->getTempParent();
	return judgeCurFocusCanWrite(focus, dir, tempParent, tempIdx, false, needJudgeFileType);
}
bool OperateMgr::judgeCurFocusCanWrite(ViewObj * focus, char dir, BaseTemplate *& tempParent, int & tempIdx, bool isPressBackspace
	, bool needJudgeFileType)
{
	//ViewObj * focus = mPressEnd->getFocusObj();
	//char dir = mPressEnd->getDirection();
	//int tempIdx = -1;
	//BaseTemplate * tempParent = focus->getTempParent();
	if (needJudgeFileType && mFileData->isReadOnly())
	{
		return false;
	}
	QList<BaseContainer *> * tempParentList = BaseSystem::getInstance()->getClearTempContainerList();
	int size = mChooseList->size();
	if (size < 1)
	{
		ViewObj * dest = focus;
		tempParent = focus->getTempParent();
		BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);
		if (!tempParent->canWrite())
		{
			return false;
		}

		// 如果按backspace键盘
		if (isPressBackspace)
		{
			// 如果在焦点对象左边按backspace，则为删除当前光标左边的对象
			if (dir == BaseConstants::Direction::LEFT)
			{
				dest = focus->getPreView();
				if (!dest)
				{
					dest = focus;
				}
			}
		}
		BaseParagraph * paragraph = dest->getParagraphParent();
		ParagraphCodeParams * params = paragraph->getContentData()->getParagraphCodeParams();
		if (params)
		{
			int authority = params->judgeAuthority();
			if (authority == 0)
				return false;
		}
		return true;
	}
	else
	{
		BaseContainer * child = NULL;
		BaseParagraph * paragraph = NULL;
		for (int i = 0; i < size; i++)
		{
			child = mChooseList->at(i);
			if (child->isView())
			{
				tempParent = child->getTempParent();
				paragraph = child->getParagraphParent();
			}
			else
			{
				BaseTools::refreshTempContainerAndTempIdx(child, tempParent, tempIdx);
				paragraph = child->getFirstView()->getParagraphParent();
			}
			if (!tempParent->canWrite())
			{
				return false;
			}
			if (paragraph)
			{
				ParagraphCodeParams * params = paragraph->getContentData()->getParagraphCodeParams();
				if (params)
				{
					int authority = params->judgeAuthority();
					if (authority == 0)
						return false;
				}
			}
		}
		return true;
	}

}
void OperateMgr::resetFocusWhenCopy()
{
	bool focusInChooseList = isFocusInChooseList();
	// 如果当前光标在选中项内
	if (focusInChooseList)
	{
		BaseContainer * obj = mChooseList->at(0);
		ViewObj * dest = NULL;
		if (obj->isView())
		{
			dest = ((ViewObj *)obj)->getPreViewFromViewParent();
			if (dest)
			{
				setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, dest, BaseConstants::Direction::RIGHT);
			}
			else
			{
				dest = ((ViewObj *)obj)->getNextViewFromViewParent();
				setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, dest, BaseConstants::Direction::LEFT);
			}
		}
		else
		{
			dest = obj->getFirstChildView();
			dest = dest->getPreViewFromViewParent();
			if (dest)
			{
				setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, dest, BaseConstants::Direction::RIGHT);
			}
			else
			{
				dest = obj->getLastChildView();
				dest = dest->getNextViewFromViewParent();
				setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, dest, BaseConstants::Direction::LEFT);
			}
		}
	}
}
bool OperateMgr::isFocusInChooseList()
{
	int size = mChooseList->size();
	if (size < 1)
	{
		return false;
	}
	ViewObj * focus = mPressEnd->getFocusObj();
	BaseContainer * obj = NULL;
	for (int i = 0; i < size; i++)
	{
		obj = mChooseList->at(i);
		if (obj->isView())
		{
			if (obj == focus)
			{
				return true;
			}
		}
		else
		{
			if (focus->isTempleteParent(obj, true))
			{
				return true;
			}
		}
	}
	return false;
}
void OperateMgr::refreshToolbarParams()
{
	ToolBarClass * toolbar = mWriter->getAppEntrance()->getToolbar();
	if (toolbar)
		toolbar->setToolBarIsChecked(mFocusTxtData, mFocusParagraphData->getHorizonAlignType());
}
void OperateMgr::setDestEffectLogicPageIndex(int idx)
{
	mDestEffectLogicPageIndex = idx;
}
void OperateMgr::refreshEffectLogicPageByDestIndex()
{
	if (mDestEffectLogicPageIndex >= 0)
	{
		ViewObj * page = (ViewObj *)mPagePanel->getChild(mDestEffectLogicPageIndex);
		setEffectLogicPage(page);
		mDestEffectLogicPageIndex = -1;
	}
}
//bool OperateMgr::judgeCanDelObj(int operateType, BaseContainer * obj)
//{
//	if (obj->isPageNumber())
//	{
//		return false;
//	}
//	return true;
//}
bool OperateMgr::judgeCanDelChooseList()
{
	int size = mChooseList->size();
	//BaseContainer * obj = NULL;
	//for (int i = 0; i < size; i ++)
	//{
	//	obj = mChooseList->at(i);
	//	if (!judgeCanDelEnter(obj))
	//	{
	//		return false;
	//	}
	//}
	if (size == 1)
	{
		BaseContainer * delObj = mChooseList->at(0);
		if (delObj->isEnter())
		{
			BaseContainer * tempParent = delObj->getTempParent();
			// 如果为父对象里的最后一个回车，则禁止删除
			if (delObj == tempParent->getLastChild(false))
			{
				return false;
			}
			// 如果为node前的回车，也禁止删除
			int idx = delObj->getTempIndex();
			BaseContainer * nextObj = tempParent->getChild(idx + 1);
			if (nextObj->isNode())
			{
				return false;
			}
		}
	}

	for (int i = 0; i < size; i++)
	{
		BaseContainer *  obj = mChooseList->at(i);
		if (!judgeCanDelObj(obj, ActionTypeOnDeleteObj::ChooseDelete))
		{
			return false;
		}
	}
	return true;
}
bool OperateMgr::judgeCanDelFixTempOnMedicalRecod(BaseContainer * obj)
{
	if (obj->getObjID() == ConstantsID::ContainerId::FIX_TEXT_WIDGET_TEMPLETE)
	{
		if (mFileData->getFileType() == AuthorityConstants::FileType::MEDICAL_RECORDS)
		{
			return false;
		}
	}
	return true;
}
bool OperateMgr::judgeCanDelEnter(BaseContainer * obj)
{
	if (!obj->isEnter())
		return true;
	BaseContainer * tempParent = obj->getTempParent();
	// 如果为父对象里的最后一个回车，则禁止删除
	if (obj == tempParent->getLastChild(false))
	{
		return false;
	}
	// 如果为node前的回车，也禁止删除
	int idx = obj->getTempIndex();
	BaseContainer * nextObj = tempParent->getChild(idx + 1);
	if (nextObj->isNode())
	{
		return false;
	}
	return true;
}
bool OperateMgr::judgeCanDelObj(BaseContainer * obj, ActionTypeOnDeleteObj actionType)
{
	bool isNotEditTemplete = true;
	int fileType = getFileData()->getFileType();
	if (fileType == AuthorityConstants::FileType::TEMPLETE)
	{
		isNotEditTemplete = false;
	}
	int objId = obj->getObjID();
	if (isNotEditTemplete)
	{
		bool canDelete = false;
		switch (objId)
		{
		case ConstantsID::ContainerId::EXCEL_PAGE:
			break;
		case ConstantsID::ContainerId::EXCEL_TEMPLETE:
			if (actionType == ActionTypeOnDeleteObj::ChooseDelete)
			{
				canDelete = true;
			}
			break;
		case ConstantsID::ContainerId::TITLE_PAGE_NUMBER:
		case ConstantsID::ContainerId::BOTTOM_PAGE_NUMBER:
			break;

		case ConstantsID::ContainerId::INPUT_TEXT_TEMPLETE:
			if (actionType == ActionTypeOnDeleteObj::ChooseDelete)
			{
				canDelete = true;
			}
			break;
		case ConstantsID::ContainerId::STRUCTURATION_TEXT_TEMPLETE_OBJ:
			if (actionType == ActionTypeOnDeleteObj::ChooseDelete)
			{
				canDelete = true;
			}
			break;
		case ConstantsID::ContainerId::MACRO_TEMPLETE_OBJ:
			if (actionType == ActionTypeOnDeleteObj::ChooseDelete)
			{
				canDelete = true;
			}
			break;
		case ConstantsID::ContainerId::MENSTRUATION_TEMPLETE_OBJ:
			canDelete = true;
			break;
		case ConstantsID::ContainerId::CHOICEWIDGET_TEMPLETE:
			if (actionType == ActionTypeOnDeleteObj::ChooseDelete)
			{
				canDelete = true;
			}
			break;
		case ConstantsID::ContainerId::UNFOLD_CHOICE_TEMPLETE:
			if (actionType == ActionTypeOnDeleteObj::ChooseDelete)
			{
				canDelete = true;
			}
			break;

		case ConstantsID::ContainerId::TXT:
		case ConstantsID::ContainerId::ENTER:
		case ConstantsID::ContainerId::IMG:
		case ConstantsID::ContainerId::HORIZONTAL_LINE:
		case ConstantsID::ContainerId::END_PAGE_LINE:
			canDelete = true;
			break;
			/*case ConstantsID::ContainerId::EXCEL_TEMPLETE:
				canDelete = true;
				break;*/
		}
		return canDelete;
	}
	else
	{
		bool canDelete = false;
		CodeParams * codeParams = NULL;
		switch (objId)
		{
		case ConstantsID::ContainerId::NODE_CONTAINER:
			codeParams = obj->getContentData()->getCodeParams();
			if (codeParams->judgeAuthority() == 1)
				canDelete = true;
			break;
		case ConstantsID::ContainerId::EXCEL_PAGE:
			break;
		case ConstantsID::ContainerId::EXCEL_TEMPLETE:
			if (actionType == ActionTypeOnDeleteObj::ChooseDelete)
			{
				canDelete = true;
			}
			break;
		case ConstantsID::ContainerId::TITLE_PAGE_NUMBER:
		case ConstantsID::ContainerId::BOTTOM_PAGE_NUMBER:
			break;
		case ConstantsID::ContainerId::FIX_TEXT_WIDGET_TEMPLETE:
			break;
		case ConstantsID::ContainerId::INPUT_TEXT_TEMPLETE:
			if (actionType == ActionTypeOnDeleteObj::ChooseDelete)
			{
				canDelete = true;
			}
			break;
		case ConstantsID::ContainerId::STRUCTURATION_TEXT_TEMPLETE_OBJ:
			if (actionType == ActionTypeOnDeleteObj::ChooseDelete)
			{
				canDelete = true;
			}
			break;
		case ConstantsID::ContainerId::MACRO_TEMPLETE_OBJ:
			if (actionType == ActionTypeOnDeleteObj::ChooseDelete)
			{
				canDelete = true;
			}
			break;
		case ConstantsID::ContainerId::MENSTRUATION_TEMPLETE_OBJ:
			canDelete = true;
			break;
		case ConstantsID::ContainerId::CHOICEWIDGET_TEMPLETE:
			if (actionType == ActionTypeOnDeleteObj::ChooseDelete)
			{
				canDelete = true;
			}
			break;
		case ConstantsID::ContainerId::UNFOLD_CHOICE_TEMPLETE:
			if (actionType == ActionTypeOnDeleteObj::ChooseDelete)
			{
				canDelete = true;
			}
			break;
		case ConstantsID::ContainerId::TXT:
		case ConstantsID::ContainerId::ENTER:
		case ConstantsID::ContainerId::IMG:
		case ConstantsID::ContainerId::HORIZONTAL_LINE:
		case ConstantsID::ContainerId::END_PAGE_LINE:
			canDelete = true;
			break;
			/*case ConstantsID::ContainerId::EXCEL_TEMPLETE:
				canDelete = true;
				break;*/
		}
		return canDelete;
	}
}
int OperateMgr::delChooseList(char addActionType, bool isJudgeType)
{
	if (isJudgeType)
	{
		if (!judgeCanDelChooseList())
			return -1;
	}
	if (addActionType == BaseConstants::DoActionType::CREATE_STEP_AND_ADD_ACTION)
		mActionMgr->createNewAction();

	ViewObj *preStartFocus = mPressStart->getFocusObj();
	char preStartDir = mPressStart->getDirection();
	ViewObj *preEndFocus = mPressEnd->getFocusObj();
	char preEndDir = mPressEnd->getDirection();

	BaseContainer * startChoose = mChooseList->at(0);
	BaseContainer * startTempParent = startChoose->getTempParent();
	int chooseListSize = mChooseList->size();
	BaseContainer * endChoose = mChooseList->at(chooseListSize - 1);
	BaseContainer * endTempParent = endChoose->getTempParent();

	ViewObj * preEffectLogicPage = mEffectLogicPage;
	BaseParagraph * nextParagraph = NULL;
	BaseContainer * lastChoose = mChooseList->at(chooseListSize - 1);
	if (lastChoose->isEnter())
	{
		nextParagraph = lastChoose->getParagraphParent()->getNextParagraph();
	}
	// 如果首尾选项在同一个模板容器中,包括普通内容选中，或者前面为普通内容，后面选中整个节点
	if (startTempParent == endTempParent)
	{
		TempContainerMgr * tempMgr = (TempContainerMgr *)startTempParent->getChildrenMgr();
		int startIndexFromTempContainer = 0;
		int endIndexFromTempContainer = 0;
		BaseTemplate * tempParent = startChoose->getTempParent();
		BaseTools::refreshTempContainerAndTempIdx(startChoose, tempParent, startIndexFromTempContainer);
		BaseTools::refreshTempContainerAndTempIdx(endChoose, tempParent, endIndexFromTempContainer);
		//startIndexFromTempContainer = startChoose->getIndexFromTempContainer();
		//endIndexFromTempContainer = endChoose->getIndexFromTempContainer();

		ViewObj * endView = NULL;
		if (endChoose->isView())
			endView = (ViewObj *)endChoose;
		else
			endView = endChoose->getLastChildView();
		ViewObj * destFocus = endView->getNextViewFromViewParent();
		if (!destFocus)
		{
			ViewObj * startView = startChoose->getPreView();
			if (startView)
				destFocus = startView->getPreViewFromViewParent();
		}
		if (endIndexFromTempContainer < startIndexFromTempContainer)
		{
			int a = 0;
		}
		int returnType = tempMgr->delChildren(startIndexFromTempContainer
			, endIndexFromTempContainer - startIndexFromTempContainer + 1, true);
		if (returnType != BaseConstants::ReturnType::HAVE_NOT_DEL_ENTER)
			setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, destFocus, BaseConstants::Direction::LEFT);
	}
	// 病程记录跨病程
	else if (startTempParent->isTempContainer() && endTempParent->isTempContainer())
	{
		if (addActionType == BaseConstants::DoActionType::CREATE_STEP_AND_ADD_ACTION)
		{
			mActionMgr->delAction();
		}
		return -1;
	}
	// 前面选中节点部分内容，后面选中整个节点
	else
	{

		//if (startTempParent->isNode() || endTempParent->isNode())
		//{
		TempContainerMgr * tempMgr = (TempContainerMgr *)startTempParent->getChildrenMgr();
		int startIndexFromTempContainer = 0;
		int endIndexFromTempContainer = 0;
		int size = startTempParent->getChildren()->size();
		BaseTemplate * tempParent = startChoose->getTempParent();
		BaseTools::refreshTempContainerAndTempIdx(startChoose, tempParent, startIndexFromTempContainer);
		// 删除前面节点选中的后半部分内容
		tempMgr->delChildren(startIndexFromTempContainer, size - startIndexFromTempContainer, true);

		int startNodeLevel = startTempParent->getContentData()->getNodeParams()->getLevel();
		int endLevel = 1;
		if (endTempParent->isNode())
			endTempParent->getContentData()->getNodeParams()->getLevel();
		int disLevel = endLevel - startNodeLevel;
		startIndexFromTempContainer = startTempParent->getTempIndex();
		for (int i = 0; i < disLevel; i++)
		{
			startTempParent = startTempParent->getTempParent();
			startIndexFromTempContainer = startTempParent->getTempIndex();
		}
		// 删除后面选中的整个节点
		BaseTools::refreshTempContainerAndTempIdx(endChoose, tempParent, endIndexFromTempContainer);

		//tempParent = endTempParent->getTempParent();
		tempMgr = (TempContainerMgr *)endTempParent->getChildrenMgr();
		tempMgr->delChildren(startIndexFromTempContainer + 1, endIndexFromTempContainer - startIndexFromTempContainer, true);
	}

	if (addActionType >= BaseConstants::DoActionType::ONLY_ADD_ACTION)
	{
		ActionFactory::pushDelObjListAction(mActionMgr, nextParagraph, preEffectLogicPage, mChooseList, true, preStartFocus, preStartDir, preEndFocus, preEndDir
			, mPressEnd->getFocusObj(), mPressEnd->getDirection(), mIsUpsidedown);
	}

	clearChooseList(true);


	return 1;
}
void OperateMgr::refreshFocusTxtData()
{
	ViewObj * focus = mPressEnd->getFocusObj();
	if (!focus)
	{
		return;
	}
	if (focus->isTxt())
	{
		mFocusTxtData->refreshTxtParamBySourceParam((StrContentData *)focus->getContentData(), false);
	}
	else
	{
		StrContentData * destData = NULL;
		char dir = mPressEnd->getDirection();
		if (dir == BaseConstants::Direction::LEFT)
		{
			focus = focus->getPreViewFromViewParent();
			if (focus && focus->isTxt())
			{
				destData = (StrContentData *)focus->getContentData();
			}
		}
		else if (dir == BaseConstants::Direction::RIGHT)
		{
			focus = focus->getNextViewFromViewParent();
			if (focus && focus->isTxt())
			{
				destData = (StrContentData *)focus->getContentData();
			}
		}
		//if (!destData)
		//{
		//	destData = BaseSystem::getInstance()->getDefaultStrData();
		//}
		if (destData)
			mFocusTxtData->refreshTxtParamBySourceParam(destData, false);
	}
}
StrContentData * OperateMgr::getFocusTxtData()
{
	return mFocusTxtData;
}
void OperateMgr::setHorizonAlignType(char type)
{
	char preType = mFocusParagraphData->getHorizonAlignType();
	if (preType == type)
	{
		return;
	}
	if (!judgeCurFocusCanWrite(false, true))
		return;
	ViewObj * startFocus = mPressStart->getFocusObj();
	char startDir = mPressStart->getDirection();
	ViewObj * endFocus = mPressEnd->getFocusObj();
	char endDir = mPressEnd->getDirection();

	savePressStartAndEnd();
	mActionMgr->createNewAction();
	ActionFactory::pushSetParagraphIntParamAction(mActionMgr, mChooseList, BaseConstants::UIInputType::SET_PARAGRAPH_HORIZON_ALIGN_TYPE, type
		, startFocus, startDir, endFocus, endDir, mIsUpsidedown);

	mFocusParagraphData->setHorizonAlignType(type, false);
	int chooseSize = mChooseList->size();
	if (chooseSize > 1)
	{
		BaseContainer * obj = NULL;
		BaseContainer * preParagraph = NULL;
		BaseContainer * paragraph = NULL;
		ParagraphData * paragraphData = NULL;
		QList<BaseContainer *> * paragraphList = BaseSystem::getInstance()->getClearTempContainerList();
		ParagraphManager * mgr = NULL;
		for (int i = 0; i < chooseSize; i++)
		{
			obj = mChooseList->at(i);
			if (obj->isView())
			{
				// 选中表格页
				if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
				{
					ExcelCellData *	cellData = (ExcelCellData *)obj->getContentData();
					mgr = cellData->getParagraphMgr();
					QList<BaseParagraph *> * tempParagraphList = mgr->getParagraphList();
					for (int j = 0; j < tempParagraphList->size(); j++)
					{
						paragraph = tempParagraphList->at(j);
						paragraphList->push_back(paragraph);
						//mParagraphDataList->push_back(data);
						//preValue = getValue(data, mType);
						//mPreParagraphValueList->push_back(preValue);
					}
				}
				else
				{
					paragraph = obj->getFirstView()->getParagraphParent();
					if (preParagraph != paragraph)
					{
						preParagraph = paragraph;
						paragraphList->push_back(paragraph);
					}
				}
			}
			// 选中整个表格
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
			{
				paragraph = obj->getFirstView()->getParagraphParent();
				if (preParagraph != paragraph)
				{
					preParagraph = paragraph;
					paragraphList->push_back(paragraph);
				}
			}
			// 其他模板
			else
			{
				paragraph = obj->getFirstView()->getParagraphParent();
				if (preParagraph != paragraph)
				{
					preParagraph = paragraph;
					paragraphList->push_back(paragraph);
				}
			}
		}

		for (int i = 0; i < paragraphList->size(); i++)
		{
			paragraph = paragraphList->at(i);
			paragraphData = paragraph->getParagraphData();
			paragraphData->setHorizonAlignType(type, false);
			paragraphData->replaceRowAignType();
		}
	}
	else
	{
		mFocusParagraphData->replaceRowAignType();
	}
	backSetPressStartAndEnd(true);
	mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
}
void OperateMgr::setRowDisTimes(float times)
{
	if (!judgeCurFocusCanWrite(false, true))
		return;
	float preTimes = mFocusParagraphData->getRowDisHTimes();

	ViewObj * startFocus = mPressStart->getFocusObj();
	char startDir = mPressStart->getDirection();
	ViewObj * endFocus = mPressEnd->getFocusObj();
	char endDir = mPressEnd->getDirection();
	BaseParagraph * startParagraph = startFocus->getParagraphParent();
	BaseParagraph * endParagraph = endFocus->getParagraphParent();
	if (mIsUpsidedown)
	{
		startParagraph = endFocus->getParagraphParent();
		endParagraph = startFocus->getParagraphParent();
	}
	QList<BaseContainer *> * paragraphList = BaseSystem::getInstance()->getClearTempContainerList();
	paragraphList->push_back(startParagraph);
	while (startParagraph != endParagraph)
	{
		startParagraph = startParagraph->getNextParagraph();
		paragraphList->push_back(startParagraph);
	}
	int size = paragraphList->size();
	ParagraphData * data = NULL;
	if (preTimes == times)
	{
		bool haveOtherTimesValue = false;
		for (int i = 0; i < size; i++)
		{
			data = (ParagraphData *)paragraphList->at(i)->getContentData();
			if (data->getRowDisHTimes() != times)
			{
				haveOtherTimesValue = true;
				break;
			}
		}
		if (!haveOtherTimesValue)
		{
			return;
		}
	}
	savePressStartAndEnd();
	mActionMgr->createNewAction();
	ActionFactory::pushSetParagraphFloatParamAction(mActionMgr, mChooseList, BaseConstants::UIInputType::SET_PARAGRAPH_ROW_DIS_TIMES, times
		, startFocus, startDir, endFocus, endDir, mIsUpsidedown);

	for (int i = 0; i < size; i++)
	{
		data = (ParagraphData *)paragraphList->at(i)->getContentData();
		data->setRowDisHTimes(times, true);
	}
	backSetPressStartAndEnd(true);
	mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
}
void OperateMgr::setFocusParagraphIndentParam(int inL, int inR, char indentType, int indentValue)
{
	if (!judgeCurFocusCanWrite(false, true))
		return;
	ViewObj * startFocus = mPressStart->getFocusObj();
	char startDir = mPressStart->getDirection();
	ViewObj * endFocus = mPressEnd->getFocusObj();
	char endDir = mPressEnd->getDirection();

	savePressStartAndEnd();
	mActionMgr->createNewAction();
	ActionFactory::pushSetParagraphIndentParamAction(mActionMgr, mChooseList, inL, inR, indentType, indentValue
		, startFocus, startDir, endFocus, endDir, mIsUpsidedown);

	mFocusParagraphData->setIndentParam(inL, inR, indentType, indentValue, false);

	BaseParagraph * startParagraph = startFocus->getParagraphParent();
	BaseParagraph * endParagraph = endFocus->getParagraphParent();
	if (mIsUpsidedown)
	{
		startParagraph = endFocus->getParagraphParent();
		endParagraph = startFocus->getParagraphParent();
	}
	QList<BaseContainer *> * paragraphList = BaseSystem::getInstance()->getClearTempContainerList();
	paragraphList->push_back(startParagraph);
	while (startParagraph != endParagraph)
	{
		startParagraph = startParagraph->getNextParagraph();
		paragraphList->push_back(startParagraph);
	}
	int size = paragraphList->size();
	ParagraphData * data = NULL;

	BaseParagraph * paragraph = NULL;
	BaseContainer * firstObj = NULL;
	ViewObj * firstRow = NULL;
	RowLayout * firstRowLayout = NULL;

	BaseContainer * lastObj = NULL;
	for (int i = 0; i < size; i++)
	{
		data = (ParagraphData *)paragraphList->at(i)->getContentData();

		data->setIndentParam(inL, inR, indentType, indentValue, false);
		paragraph = (BaseParagraph *)data->getOwner();
		firstObj = paragraph->getChild(0);
		firstRow = (ViewObj *)firstObj->getViewParent();
		firstRowLayout = (RowLayout *)firstRow->getLayout();

		lastObj = paragraph->getLastChild(false);
		BaseContainer * lastRow = lastObj->getViewParent();
		if (lastRow != firstRow)
		{
			BaseLayout * nextLayout = firstRowLayout;
			BaseContainer * tempRow = nextLayout->getOwner();
			BaseLayout * lastLayout = (RowLayout *)lastRow->getChildrenMgr();
			while (nextLayout)
			{
				nextLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
				if (nextLayout == lastLayout)
					break;
				nextLayout = nextLayout->getNextLayout(false);
			}
		}
		else
			firstRowLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
		firstRowLayout->openReplace();
		firstRowLayout->judgeLessThanCircleUntilObj(lastObj);
		firstRowLayout->judgeOverFlowCircleUntilObj(lastObj);
	}

	backSetPressStartAndEnd(true);
	mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
}
void OperateMgr::repointFocusParagraphDataAndExcelVerticalAlignType(ViewObj * focus)
{
	repointFocusParagraphData(focus);
	repointFocusExcelVerticalAlignType(focus);
}
void OperateMgr::repointFocusParagraphData(ViewObj * focus)
{
	if (!focus)
	{
		mFocusParagraphData = BaseSystem::getInstance()->getDefaultParagraphData();
		return;
	}



	BaseParagraph * paragraph = focus->getParagraphParent();
	if (mFocusParagraphData && mFocusParagraphData->getOwner() == paragraph)
	{
		return;
	}
	ParagraphData * data = (ParagraphData *)paragraph->getContentData();
	mFocusParagraphData = data;
}
void OperateMgr::repointFocusExcelVerticalAlignType(ViewObj * focus)
{
	if (!focus)
	{
		mFocusParagraphVerticalAlignType = BaseConstants::Direction::UP;
		return;
	}
	BaseContainer * row = focus->findParentRow(true);
	BaseContainer * layer = row->getViewParent();
	bool isAllInExcelPage = layer->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER);
	int size = mChooseList->size();
	if (!isAllInExcelPage)
	{
		return;
	}
	ViewObj * excelPage = focus->findViewFromParent(ConstantsID::ContainerId::EXCEL_PAGE, true);
	int minH = 0;
	ExcelLayout::getCellBaseInformation(minH, mFocusParagraphVerticalAlignType, excelPage);

	ToolBarClass * toolbar = mWriter->getAppEntrance()->getToolbar();
	if (toolbar)
		toolbar->setToolBarIsChecked(NULL, 0, mFocusParagraphVerticalAlignType);
}
ParagraphData * OperateMgr::getFocusParagraphData()
{
	return mFocusParagraphData;
}
bool OperateMgr::isOnShowTidy()
{
	return false;
}
void OperateMgr::showTidy(bool isTidy)
{
	if (mSystemParams->isTidy() == isTidy)
		return;
	//mActionMgr->createNewAction();
	//ActionFactory::pushSetSysBoolParamAction(mActionMgr, BaseConstants::SetSystemParamType::SET_SHOW_TIDY, isTidy);
	mSystemParams->setTidy(isTidy);
}
void OperateMgr::doPrint()
{
}
void OperateMgr::resizeWindow(int w, int h)
{
	ViewObj * rootView = mWriter->getRootView();
	BaseViewParam * rootViewParam = rootView->getViewParam();
	if (w == rootViewParam->getSelfW() && h == rootViewParam->getSelfH())
	{
		return;
	}
	rootViewParam->setSelfW(w, BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_CHILDREN);
	rootViewParam->setSelfH(h, BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_CHILDREN);
	ViewObj * window = (ViewObj *)rootView->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, false);
	BaseLayout * layout = (BaseLayout *)window->getChildrenMgr();
	layout->replaceChildren();
}
char OperateMgr::setPageParams(bool needAddAction, bool isHorizontal, short paperType, int pageInsideTop, int pageInsideBottom, int pageInsideLeft, int pageInsideRight
	, int titleMaxH, int bottomMaxH)
{
	int preLeft = mSystemParams->getPageInsideLeft();
	int preRight = mSystemParams->getPageInsideRight();
	int dis = pageInsideLeft + pageInsideRight - (preLeft + preRight);

	ViewObj * rootView = mWriter->getRootView();
	ViewObj * firstPage = (ViewObj *)rootView->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, false)
		->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false)->getChild(0);
	ViewObj * titleView = (ViewObj *)firstPage->findChildrenByID(ConstantsID::ContainerId::PAGE_TITLE, false);
	BaseViewParam * param = titleView->getViewParam();
	if (titleMaxH < param->getTotalH())
	{
		return false;
	}
	ViewObj * bottomView = (ViewObj *)firstPage->findChildrenByID(ConstantsID::ContainerId::PAGE_BOTTOM, false);
	param = bottomView->getViewParam();
	if (bottomMaxH < param->getTotalH())
	{
		return false;
	}
	if (!mSystemParams->haveChanged(isHorizontal, paperType, pageInsideTop, pageInsideBottom, pageInsideLeft, pageInsideRight
		, titleMaxH, bottomMaxH))
	{
		return false;
	}
	if (needAddAction)
	{
		mActionMgr->createNewAction();
		ActionFactory::pushSetPageParamsAction(mActionMgr, isHorizontal, paperType, pageInsideTop, pageInsideBottom, pageInsideLeft, pageInsideRight
			, titleMaxH, bottomMaxH);
	}
	char haveChanged = mSystemParams->setPageParams(isHorizontal, paperType, pageInsideTop, pageInsideBottom, pageInsideLeft, pageInsideRight
		, titleMaxH, bottomMaxH);
	if (haveChanged > 0)
	{
		if (haveChanged == 1)
		{
			QList<BaseContainer *> * pageList = mPagePanel->getChildren();
			int size = pageList->size();
			ViewObj * page = NULL;
			BaseLayout * pageLayout = NULL;
			LayoutViewParam * pageParam = NULL;
			LayoutReplaceManager * layoutReplaceMgr = mWriter->getLayoutReplaceMgr();
			for (int i = 0; i < size; i++)
			{
				page = (ViewObj *)pageList->at(i);
				pageLayout = page->getLayout();
				pageParam = (LayoutViewParam *)page->getViewParam();
				pageParam->setInsideTop(pageInsideTop);
				pageParam->setInsideBottom(pageInsideBottom);
				pageParam->setInsideLeft(pageInsideLeft);
				pageParam->setInsideRight(pageInsideRight);

				pageLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);

				pageParam->refreshInsideW(BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_CHILDREN);
				pageParam->refreshInsideH(BaseConstants::CircleChildrenOrParentType::DIRECT_REFREHSH_CHILDREN);
			}

			page = (ViewObj *)pageList->at(0);
			BaseTemplate * rootTemp = mWriter->getRootTemp();
			BaseContainer * titleTemp = rootTemp->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false);
			BaseContainer * titleContainer = titleTemp->getChild(0);
			ViewObj * lastChild = titleContainer->getLastView();

			ViewObj * focus = mPressEnd->getFocusObj();
			//ViewObj * page = focus->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
			BaseContainer * pageTitle = page->findChildrenByID(ConstantsID::ContainerId::PAGE_TITLE, false);
			BaseContainer * titleLayer = pageTitle->getChild(0);
			ViewObj * firstRow = (ViewObj *)titleLayer->getChild(0);
			RowLayout * rowLayout = (RowLayout *)firstRow->getLayout();

			if (dis > 0)
			{
				rowLayout->judgeOverFlowCircleUntilObj(lastChild);
			}
			else if (dis < 0)
			{
				rowLayout->judgeLessThanCircleUntilObj(lastChild);
			}

			BaseContainer * bottomTemp = rootTemp->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false);
			BaseContainer * bottomContainer = bottomTemp->getChild(0);
			lastChild = bottomContainer->getLastView();

			BaseContainer * pageBottom = page->findChildrenByID(ConstantsID::ContainerId::PAGE_BOTTOM, false);
			BaseContainer * bottomLayer = pageBottom->getChild(0);
			firstRow = (ViewObj *)bottomLayer->getChild(0);
			rowLayout = (RowLayout *)firstRow->getLayout();
			if (dis > 0)
			{
				rowLayout->judgeOverFlowCircleUntilObj(lastChild);
			}
			else if (dis < 0)
			{
				rowLayout->judgeLessThanCircleUntilObj(lastChild);
			}


			BaseContainer * contentTemp = rootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
			BaseContainer * contentContainer = contentTemp->getChild(0);
			lastChild = contentContainer->getLastView();

			BaseContainer * pageContent = page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false);
			BaseContainer * contentLayer = pageContent->getChild(0);
			firstRow = (ViewObj *)contentLayer->getChild(0);
			rowLayout = (RowLayout *)firstRow->getLayout();
			if (dis > 0)
			{
				rowLayout->judgeOverFlowCircleUntilObj(lastChild);
			}
			else if (dis < 0)
			{
				rowLayout->judgeLessThanCircleUntilObj(lastChild);
			}
		}
		else if (haveChanged == 2)
		{
			addAllContentToViewSideWithOutParagraph();
		}
	}
	//else
	//{
	//	if (needAddAction)
	//	{
	//		doUndo();
	//	}
	//}
	return haveChanged;
}
void OperateMgr::getPageParams(bool &isHorizontal, short &paperType, int &pageInsideTop, int &pageInsideBottom, int &pageInsideLeft, int &pageInsideRight, int &titleMaxH, int &bottomMaxH)
{
	isHorizontal = mSystemParams->isPaperHorizontal();
	paperType = mSystemParams->getPaperType();
	pageInsideTop = mSystemParams->getPageInsideTop();
	pageInsideBottom = mSystemParams->getPageInsideBottom();
	pageInsideLeft = mSystemParams->getPageInsideLeft();
	pageInsideRight = mSystemParams->getPageInsideRight();
	titleMaxH = mSystemParams->getTitleMaxH();
	bottomMaxH = mSystemParams->getBottomMaxH();
}
void OperateMgr::addAllContentToViewSideWithOutParagraph()
{
	ViewObj * rootView = mWriter->getRootView();
	BaseViewParam * rootViewParam = rootView->getViewParam();
	int preW = rootViewParam->getTotalW();
	int preH = rootViewParam->getTotalH();
	char preTitleNumViewType = mSystemParams->getTitleNumViewType();
	char preBottomNumViewType = mSystemParams->getBottomNumViewType();
	mSystemParams->refreshTitleAndBottomNumIndex();
	clearView(true);

	rootView = ViewFactory::createRoot(mWriter);


	BaseTemplate * rootTemp = mWriter->getRootTemp();
	mWriter->initRootTempAndView(rootTemp, rootView);
	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, NULL, BaseConstants::Direction::LEFT);

	BaseContainer * titleContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false)
		->findChildrenByID(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, false);
	TempContainerMgr * containerChildrenMgr = (TempContainerMgr *)titleContainer->getChildrenMgr();
	containerChildrenMgr->addAllChildrenToParagraphAndView(false);

	BaseContainer * bottomContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false)
		->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
	containerChildrenMgr = (TempContainerMgr *)bottomContainer->getChildrenMgr();
	containerChildrenMgr->addAllChildrenToParagraphAndView(false);


	BaseContainer * contentContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	QList<BaseContainer *> * contentContainerList = contentContainer->getChildren();
	contentContainer = NULL;
	int size = contentContainerList->size();
	ViewObj * lastView = NULL;
	for (int i = 0; i < contentContainerList->size(); i++)
	{
		contentContainer = contentContainerList->at(i);
		containerChildrenMgr = (TempContainerMgr *)contentContainer->getChildrenMgr();
		if (size == 1 || i == 0)
		{
			containerChildrenMgr->addAllChildrenToParagraphAndView(false);
			if (size > 1)
			{
				lastView = contentContainer->getLastView();
				setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, lastView
					, BaseConstants::Direction::LEFT);
			}
		}
		else
		{
			containerChildrenMgr->addRecordChildrenToParagraphAndView(lastView);
			lastView = contentContainer->getLastView();
		}
	}
	ViewObj * focus = contentContainerList->at(0)->getFirstChildView();
	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::LEFT);
	mSystemParams->initParams(false);
	focus = contentContainerList->at(0)->getFirstChildView();
	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::LEFT);
	BaseContainer * pagePanel = rootView->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, false)
		->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
	ViewObj * firstPage = (ViewObj *)pagePanel->getChild(0);
	setEffectLogicPage(firstPage);

	resizeWindow(preW, preH);
}
//bool OperateMgr::setPaperType(short type, bool needAddAction)
//{
//	if (mSystemParams->getPaperType() == type)
//	{
//		return false;
//	}
//	if (needAddAction)
//	{
//		mActionMgr->createNewAction();
//		ActionFactory::pushSetSysIntParamAction(mActionMgr, BaseConstants::SetSystemParamType::SET_PAPER_TYPE, type);
//	}
//
//	ViewObj * rootView = mWriter->getRootView();
//	BaseViewParam * rootViewParam = rootView->getViewParam();
//	int preW = rootViewParam->getTotalW();
//	int preH = rootViewParam->getTotalH();
//	char preTitleNumViewType = mSystemParams->getTitleNumViewType();
//	char preBottomNumViewType = mSystemParams->getBottomNumViewType();
//	mSystemParams->refreshTitleAndBottomNumIndex();
//	mSystemParams->setPaperType(type);
//	clearView();
//
//	rootView = ViewFactory::createRoot(mWriter);
//
//
//	BaseTemplate * rootTemp = mWriter->getRootTemp();
//	mWriter->initRootTempAndView(rootTemp, rootView);
//	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, NULL, BaseConstants::Direction::LEFT);
//
//	BaseContainer * titleContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false)
//		->findChildrenByID(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, false);
//	TempContainerMgr * containerChildrenMgr = (TempContainerMgr *)titleContainer->getChildrenMgr();
//	containerChildrenMgr->addAllChildrenToParagraphAndView(false);
//
//	BaseContainer * bottomContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false)
//		->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
//	containerChildrenMgr = (TempContainerMgr *)bottomContainer->getChildrenMgr();
//	containerChildrenMgr->addAllChildrenToParagraphAndView(false);
//
//
//	BaseContainer * contentContainer = rootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
//	QList<BaseContainer *> * contentContainerList = contentContainer->getChildren();
//	contentContainer = NULL;
//	int size = contentContainerList->size();
//	ViewObj * lastView = NULL;
//	for (int i = 0; i < contentContainerList->size(); i++)
//	{
//		contentContainer = contentContainerList->at(i);
//		containerChildrenMgr = (TempContainerMgr *)contentContainer->getChildrenMgr();
//		if (size == 1 || i == 0)
//		{
//			containerChildrenMgr->addAllChildrenToParagraphAndView(false);
//			if (size > 1)
//			{
//				lastView = contentContainer->getLastView();
//				setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, lastView
//					, BaseConstants::Direction::LEFT);
//			}
//		}
//		else
//		{
//			containerChildrenMgr->addRecordChildrenToParagraphAndView(lastView);
//			lastView = contentContainer->getLastView();
//		}
//	}
//	ViewObj * focus = contentContainerList->at(0)->getFirstChildView();
//	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::LEFT);
//	mSystemParams->initParams(false);
//	focus = contentContainerList->at(0)->getFirstChildView();
//	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::LEFT);
//	BaseContainer * pagePanel = rootView->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, false)
//		->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
//	ViewObj * firstPage = (ViewObj *)pagePanel->getChild(0);
//	setEffectLogicPage(firstPage);
//
//	resizeWindow(preW, preH);
//	return true;
//}
void OperateMgr::selectAll()
{
	BaseContainer * root = mWriter->getRootTemp();
	BaseContainer * contentContainer = root->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	ViewObj * firstView = contentContainer->getFirstChildView(true);
	ViewObj * lastView = contentContainer->getLastChildView(true);
	if (firstView == lastView && firstView->isEnter())
	{
		return;
	}
	setPress(BaseConstants::MousePressType::PRESS, firstView, BaseConstants::Direction::LEFT);
	setPress(BaseConstants::MousePressType::RELEASE, lastView, BaseConstants::Direction::RIGHT);
	refreshChooseList(true, false, true);
}
void OperateMgr::addCurrentTime()
{
	QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
	QString str = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
	QString blankSpaceStr = " ";
	addStr(blankSpaceStr + str + blankSpaceStr, true);
}
QString OperateMgr::getFocusMedicalCodeID()
{
	return "";
}
void OperateMgr::clearAllAction()
{
	mActionMgr->deleteAllAction();
}
bool OperateMgr::judgeHaveOtherStrParamTxt(char type, QString dest)
{
	QString curValue = 0;
	bool haveOtherSize = false;
	int chooseSize = mChooseList->size();
	if (chooseSize > 0)
	{
		ViewObj * obj = NULL;
		StrContentData * data = NULL;
		for (int i = 0; i < chooseSize; i++)
		{
			obj = (ViewObj *)mChooseList->at(i);
			if (obj->isTxt())
			{
				data = (StrContentData *)obj->getContentData();
				switch (type)
				{
				case BaseConstants::TxtParamActionType::SET_FAMILY:
					curValue = data->getFamily();
					break;
				}
				if (curValue != dest)
				{
					return true;
				}
			}
		}
	}
	return false;
}
void OperateMgr::setExcelParagraphAlignVerticalType(int position)
{
	if (!judgeCurFocusCanWrite(false, true))
		return;
	// 改变全局纵向对齐值
	mFocusParagraphVerticalAlignType = position;

	// 添加对应动作
	//mActionMgr->createNewAction();
	//ActionFactory::pushSetTxtNumParamAction(mActionMgr, mChooseList, BaseConstants::TxtParamActionType::SET_FONT_SIZE, size);

	ViewObj * focus = mPressEnd->getFocusObj();
	BaseContainer * row = focus->findParentRow(true);
	BaseContainer * layer = row->getViewParent();
	bool isAllInExcelPage = layer->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER);
	int size = mChooseList->size();
	// 如果没有拖选项
	if (size < 1)
	{
		// 如果当前焦点在表格页内部，则直接设置表格页对其属性并刷新内部排版
		if (isAllInExcelPage)
		{
			BaseContainer * excelTemp = focus->findTempFromParent(ConstantsID::ContainerId::EXCEL_TEMPLETE, true);
			if (excelTemp)
			{
				createNewAction();
				bool isSetSuccess = false;
				QList<ExcelCellData *> changCellList;
				ViewObj* cellPage = focus->getParentViewById(ConstantsID::ContainerId::EXCEL_PAGE);
				ExcelCellData * changeCell = ExcelLayout::getCellDataFromCell(cellPage);
				changCellList.push_back(changeCell);
				QList<int> undoPositionList;
				undoPositionList.push_back(changeCell->getVerticalPosition());
				QList<int> redoPositionList;
				redoPositionList.push_back(position);
				createSetExcelLayoutAction(&changCellList, undoPositionList, redoPositionList, isSetSuccess);
				if (!isSetSuccess)
				{
					deleteAction();
				}
			}

		}
	}
	// 如果有拖选项
	else
	{
		savePressStartAndEnd();
		createNewAction();
		bool isSetSuccess = false;
		BaseContainer * obj = NULL;
		ExcelTemplete* excelTemplate = NULL;
		QList<ExcelCellData *> changCellList;
		ExcelVerticalLayoutAction *setVerticalAction = NULL;
		QList<int> undoPositionList;
		QList<int> redoPositionList;
		QList<ExcelTemplete *> excelTempList;
		for (int i = 0; i < size; i++)
		{
			obj = mChooseList->at(i);
			// 如果为view
			if (obj->isView())
			{
				if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
				{
					BaseContainer * excelTemp = obj->getTempParent();
					if (excelTemp)
					{
						bool isSuccess = false;
						ViewObj* cellPage = obj->getParentViewById(ConstantsID::ContainerId::EXCEL_PAGE);
						ExcelCellData * changeCell = ExcelLayout::getCellDataFromCell(cellPage);

						if (excelTemp != excelTemplate)
						{
							changCellList.clear();
							undoPositionList.clear();
							redoPositionList.clear();
							changCellList.push_back(changeCell);
							undoPositionList.push_back(changeCell->getVerticalPosition());
							redoPositionList.push_back(position);
							setVerticalAction = createSetExcelLayoutAction(&changCellList, undoPositionList, redoPositionList, isSuccess);
							if (isSuccess)
							{
								isSetSuccess = true;
								setVerticalAction->setPressStartAndEndParam(mPressStart, mPressEnd);
								excelTemplate = (ExcelTemplete*)excelTemp;
								if (excelTempList.indexOf(excelTemplate) < 0)
								{
									excelTempList.push_back(excelTemplate);
								}
							}
						}
						else
						{
							setVerticalAction->addChangeCell(changeCell, changeCell->getVerticalPosition(), position);
							changeCell->setVerticalPosition(position);
						}

					}
				}
				// 如果在表页内部，则设置本表页对齐参数，然后终止循环
				else if (isAllInExcelPage)
				{
					bool isSuccess = false;
					ViewObj* cellPage = obj->getParentViewById(ConstantsID::ContainerId::EXCEL_PAGE);
					ExcelCellData * changeCell = ExcelLayout::getCellDataFromCell(cellPage);

					BaseContainer * excelTemp = obj->getTempParent();
					changCellList.clear();
					undoPositionList.clear();
					redoPositionList.clear();
					changCellList.push_back(changeCell);
					undoPositionList.push_back(changeCell->getVerticalPosition());
					redoPositionList.push_back(position);
					setVerticalAction = createSetExcelLayoutAction(&changCellList, undoPositionList, redoPositionList, isSuccess);
					if (isSuccess)
					{
						isSetSuccess = true;
						setVerticalAction->setPressStartAndEndParam(mPressStart, mPressEnd);
						excelTemplate = (ExcelTemplete*)excelTemp;
						if (excelTempList.indexOf(excelTemplate) < 0)
						{
							excelTempList.push_back(excelTemplate);
						}
					}
					break;
				}
			}
			// 如果为模板
			else
			{
				// 如果为整体表格,则让表格整体刷新内部所有表页的纵向对齐参数
				if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
				{
					if (obj)
					{
						bool isSuccess = false;
						ExcelTemplete* excelTemp = (ExcelTemplete*)obj;
						excelTemplate = excelTemp;
						undoPositionList.clear();
						redoPositionList.clear();
						QList<ExcelCellData *>* cellDataList = excelTemp->getCellList();
						ExcelCellData* tempCellData;
						for (int m = 0; m < cellDataList->size(); m++)
						{
							tempCellData = cellDataList->at(m);
							undoPositionList.push_back(tempCellData->getVerticalPosition());
							redoPositionList.push_back(position);
						}
						setVerticalAction = createSetExcelLayoutAction(cellDataList, undoPositionList, redoPositionList, isSuccess);
						if (isSuccess)
						{
							isSetSuccess = true;
							setVerticalAction->setPressStartAndEndParam(mPressStart, mPressEnd);
							if (excelTempList.indexOf(excelTemplate) < 0)
							{
								excelTempList.push_back(excelTemplate);
							}
						}
					}
				}
				// 如果在表页内部，则设置本表页对齐参数，然后终止循环
				else if (isAllInExcelPage)
				{
					bool isSuccess = false;
					ViewObj* firstView = obj->getFirstChildView();
					if (!firstView)
					{
						firstView = obj->getNextView();
					}
					ViewObj* cellPage = firstView->getParentViewById(ConstantsID::ContainerId::EXCEL_PAGE);
					ExcelCellData * changeCell = ExcelLayout::getCellDataFromCell(cellPage);

					BaseContainer * excelTemp = obj->getTempParent();
					changCellList.clear();
					undoPositionList.clear();
					redoPositionList.clear();
					changCellList.push_back(changeCell);
					undoPositionList.push_back(changeCell->getVerticalPosition());
					redoPositionList.push_back(position);
					setVerticalAction = createSetExcelLayoutAction(&changCellList, undoPositionList, redoPositionList, isSuccess);
					if (isSuccess)
					{
						isSetSuccess = true;
						setVerticalAction->setPressStartAndEndParam(mPressStart, mPressEnd);
						excelTemplate = (ExcelTemplete*)excelTemp;
						if (excelTempList.indexOf(excelTemplate) < 0)
						{
							excelTempList.push_back(excelTemplate);
						}
					}
					break;
				}
			}
		}
		if (!isSetSuccess)
		{
			deleteAction();
		}
		else
		{
			int temptSize = excelTempList.size();
			for (int t = 0; t < temptSize; t++)
			{
				excelTempList.at(t)->requestLayout();
			}
			backSetPressStartAndEnd(true);
		}
	}
}
ExcelVerticalLayoutAction * OperateMgr::createSetExcelLayoutAction(QList<ExcelCellData *>*changeCells, QList<int> unDoPositionList, QList<int> redoPositionList, bool & isSuccess)
{
	//int beforPosition = excelTemplate->getVerticalLayoutType();;
	isSuccess = ExcelTemplete::setExcelTemplateVerticalType(redoPositionList, changeCells);
	if (isSuccess)
	{
		ViewObj* focusObj = NULL;
		char direction;
		getFocusAndDirection(focusObj, direction);
		ExcelVerticalLayoutAction * setVerticalAction = new ExcelVerticalLayoutAction(getCurStepMgr());
		setVerticalAction->InitAction(focusObj, direction);
		setVerticalAction->addChangeCells(changeCells, unDoPositionList, redoPositionList);
		addToLocalAction(setVerticalAction);
		return setVerticalAction;
	}
	return NULL;
}
bool OperateMgr::judgeHaveOtherParamTxt(char type, int dest)
{
	int curValue = 0;
	bool haveOtherSize = false;
	int chooseSize = mChooseList->size();
	if (chooseSize > 0)
	{
		ViewObj * obj = NULL;
		StrContentData * data = NULL;
		for (int i = 0; i < chooseSize; i++)
		{
			obj = (ViewObj *)mChooseList->at(i);
			if (obj->isView())
			{
				if (obj->isTxt())
				{
					data = (StrContentData *)obj->getContentData();
					switch (type)
					{
					case BaseConstants::TxtParamActionType::SET_ITALIC:
						curValue = data->getItalic();
						break;
					case BaseConstants::TxtParamActionType::SET_BOLD:
						curValue = data->getBold();
						break;
					case BaseConstants::TxtParamActionType::SET_FONT_SIZE:
						curValue = data->getPointSize();
						break;
					case BaseConstants::TxtParamActionType::SET_FONT_COLOR:
						curValue = data->getFontColor();
						break;
					case BaseConstants::TxtParamActionType::SET_BACKGROUND__COLOR:
						curValue = data->getBackgroundColor();
						break;
					case BaseConstants::TxtParamActionType::SET_FLOW_TYPE:
						curValue = data->getFlowType();
						break;
					case BaseConstants::TxtParamActionType::SET_UNDERLINE:
						curValue = data->getUnderLineType();
						break;
					case BaseConstants::TxtParamActionType::SET_DEL_LINE:
						curValue = data->getDelLineType();
						break;
					}
					if (curValue != dest)
					{
						return true;
					}
				}
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}
void OperateMgr::setFontBold(bool bold)
{
	if (!judgeCurFocusCanWrite(false, true))
		return;
	bool preBold = mFocusTxtData->getBold();
	if (preBold == bold)
	{
		bool haveOtherSize = judgeHaveOtherParamTxt(BaseConstants::TxtParamActionType::SET_BOLD, bold);
		if (!haveOtherSize)
			return;
	}
	savePressStartAndEnd();
	mActionMgr->createNewAction();

	ActionFactory::pushSetTxtNumParamAction(mActionMgr, mChooseList, BaseConstants::TxtParamActionType::SET_BOLD, bold);
	mFocusTxtData->setBold(bold);

	setTxtNumParams(BaseConstants::TxtParamActionType::SET_BOLD, bold);
	backSetPressStartAndEnd(true);

	//mFocusTxtData->setBold(bold);
	//ToolBarClass::getInstance()->setToolBarIsChecked(mFocusTxtData, mFocusParagraphData->getHorizonAlignType());
}
void OperateMgr::setFontItalic(bool italic)
{
	if (!judgeCurFocusCanWrite(false, true))
		return;
	bool preItalic = mFocusTxtData->getItalic();
	if (preItalic == italic)
	{
		bool haveOtherSize = judgeHaveOtherParamTxt(BaseConstants::TxtParamActionType::SET_ITALIC, italic);
		if (!haveOtherSize)
			return;
	}
	savePressStartAndEnd();
	mActionMgr->createNewAction();
	ActionFactory::pushSetTxtNumParamAction(mActionMgr, mChooseList, BaseConstants::TxtParamActionType::SET_ITALIC, italic);

	mFocusTxtData->setItalic(italic);

	setTxtNumParams(BaseConstants::TxtParamActionType::SET_ITALIC, italic);
	backSetPressStartAndEnd(true);
}
void OperateMgr::setFontSize(int size)
{
	if (!judgeCurFocusCanWrite(false, true))
		return;
	int preSize = mFocusTxtData->getPointSize();
	if (preSize == size)
	{
		bool haveOtherSize = judgeHaveOtherParamTxt(BaseConstants::TxtParamActionType::SET_FONT_SIZE, size);
		if (!haveOtherSize)
			return;
	}
	savePressStartAndEnd();
	mActionMgr->createNewAction();
	ActionFactory::pushSetTxtNumParamAction(mActionMgr, mChooseList, BaseConstants::TxtParamActionType::SET_FONT_SIZE, size);

	mFocusTxtData->setPointSize(size);

	setTxtNumParams(BaseConstants::TxtParamActionType::SET_FONT_SIZE, size);
	backSetPressStartAndEnd(true);
}
void OperateMgr::setFontColor(int color)
{
	if (!judgeCurFocusCanWrite(false, true))
		return;
	int preColor = mFocusTxtData->getFontColor();
	if (preColor == color)
	{
		bool haveOtherSize = judgeHaveOtherParamTxt(BaseConstants::TxtParamActionType::SET_FONT_COLOR, color);
		if (!haveOtherSize)
			return;
	}

	savePressStartAndEnd();
	mActionMgr->createNewAction();
	ActionFactory::pushSetTxtNumParamAction(mActionMgr, mChooseList, BaseConstants::TxtParamActionType::SET_FONT_COLOR, color);

	mFocusTxtData->setFontColor(color);

	setTxtNumParams(BaseConstants::TxtParamActionType::SET_FONT_COLOR, color);
	backSetPressStartAndEnd(true);
}
void OperateMgr::setBackgroundColor(int color)
{
	if (!judgeCurFocusCanWrite(false, true))
		return;
	int preColor = mFocusTxtData->getBackgroundColor();
	if (preColor == color)
	{
		bool haveOtherSize = judgeHaveOtherParamTxt(BaseConstants::TxtParamActionType::SET_BACKGROUND__COLOR, color);
		if (!haveOtherSize)
			return;
	}
	savePressStartAndEnd();
	mActionMgr->createNewAction();
	ActionFactory::pushSetTxtNumParamAction(mActionMgr, mChooseList, BaseConstants::TxtParamActionType::SET_BACKGROUND__COLOR, color);

	mFocusTxtData->setBackgroundColor(color);

	setTxtNumParams(BaseConstants::TxtParamActionType::SET_BACKGROUND__COLOR, color);
	backSetPressStartAndEnd(true);

}
void OperateMgr::setFontFamily(QString family)
{
	if (!judgeCurFocusCanWrite(false, true))
		return;
	QString preFamily = mFocusTxtData->getFamily();
	if (preFamily == family)
	{
		if (!judgeHaveOtherStrParamTxt(BaseConstants::TxtParamActionType::SET_FAMILY, family))
		{
			return;
		}
	}

	savePressStartAndEnd();
	mActionMgr->createNewAction();
	ActionFactory::pushSetTxtStrParamAction(mActionMgr, mChooseList, BaseConstants::TxtParamActionType::SET_FAMILY, family);

	mFocusTxtData->setFamily(family);

	int size = mChooseList->size();
	bool needReplaceRow = true;
	if (size > 0)
	{
		BaseContainer * obj = NULL;
		StrContentData * data = NULL;
		bool needReplaceRow = true;
		for (int i = 0; i < size; i++)
		{
			obj = mChooseList->at(i);
			if (obj->isView())
			{
				if (obj->isTxt())
				{
					data = (StrContentData *)obj->getContentData();

					if (data->getFamily() != family)
						data->setFamily(family);
				}
				else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
				{
					mFocusTxtData->setOperateType(Family);
					obj->setDrawTxtParam(mFocusTxtData);
					needReplaceRow = false;
				}
			}
			else
			{
				mFocusTxtData->setOperateType(Family);
				obj->setDrawTxtParam(mFocusTxtData);
			}
		}
	}
	if (!needReplaceRow)
	{
		return;
	}
	refreshExcel();

	judgeListLess(mChooseList, true);
	judgeListOverflow(mChooseList, false);

	backSetPressStartAndEnd(true);
}
void OperateMgr::setFlowType(char type)
{
	if (!judgeCurFocusCanWrite(false, true))
		return;
	char preType = mFocusTxtData->getFlowType();
	if (preType == type)
	{
		bool haveOtherSize = judgeHaveOtherParamTxt(BaseConstants::TxtParamActionType::SET_FLOW_TYPE, type);
		if (!haveOtherSize)
			return;
	}
	savePressStartAndEnd();
	mActionMgr->createNewAction();
	ActionFactory::pushSetTxtNumParamAction(mActionMgr, mChooseList, BaseConstants::TxtParamActionType::SET_FLOW_TYPE, type);

	mFocusTxtData->setFlowType(type);

	setTxtNumParams(BaseConstants::TxtParamActionType::SET_FLOW_TYPE, type);
	backSetPressStartAndEnd(true);
}
void OperateMgr::setUnderlineParam(char type, short value)
{
	if (!judgeCurFocusCanWrite(false, true))
		return;
	char preType = mFocusTxtData->getUnderLineType();
	short preThick = mFocusTxtData->getUnderLineThick();
	if (preType == type && preThick == value)
	{
		bool haveOtherSize = judgeHaveOtherParamTxt(BaseConstants::TxtParamActionType::SET_UNDERLINE, type);
		if (!haveOtherSize)
			return;
	}

	savePressStartAndEnd();
	mActionMgr->createNewAction();
	int * dest = new int[2];
	dest[0] = type;
	dest[1] = value;
	ActionFactory::pushSetTxtMutipleNumParamAction(mActionMgr, mChooseList, BaseConstants::TxtParamActionType::SET_UNDERLINE
		, 2, dest);

	mFocusTxtData->setUnderLine(type, value);

	int size = mChooseList->size();
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;

	for (int i = 0; i < size; i++)
	{
		obj = mChooseList->at(i);
		// 如果为显示对象
		if (obj->isView())
		{
			// 普通文字，正常设置
			if (obj->isTxt())
			{
				data = (StrContentData *)obj->getContentData();
				if (data->getUnderLineType() != type || data->getUnderLineThick() != value)
					data->setUnderLine(type, value);
			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
			{
				mFocusTxtData->setOperateType(UnderLineType);
				obj->setDrawTxtParam(mFocusTxtData);
			}
		}
		// 模板等
		else
		{
			mFocusTxtData->setOperateType(UnderLineType);
			obj->setDrawTxtParam(mFocusTxtData);
		}
	}
	backSetPressStartAndEnd(true);
}
void OperateMgr::setDelLineParam(char type, short value)
{
	if (!judgeCurFocusCanWrite(false, true))
		return;
	char preType = mFocusTxtData->getUnderLineType();
	short preThick = mFocusTxtData->getUnderLineThick();
	if (preType == type && preThick == value)
	{
		if (preType == type && preThick == value)
		{
			bool haveOtherSize = judgeHaveOtherParamTxt(BaseConstants::TxtParamActionType::SET_UNDERLINE, type);
			if (!haveOtherSize)
				return;
		}
	}

	savePressStartAndEnd();
	mActionMgr->createNewAction();
	int * dest = new int[2];
	dest[0] = type;
	dest[1] = value;
	ActionFactory::pushSetTxtMutipleNumParamAction(mActionMgr, mChooseList, BaseConstants::TxtParamActionType::SET_DEL_LINE
		, 2, dest);

	mFocusTxtData->setDelLine(type, value);

	int size = mChooseList->size();
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;

	for (int i = 0; i < size; i++)
	{
		obj = mChooseList->at(i);
		// 如果为显示对象
		if (obj->isView())
		{
			// 普通文字，正常设置
			if (obj->isTxt())
			{
				data = (StrContentData *)obj->getContentData();
				if (data->getDelLineType() != type || data->getDelLineThick() != value)
					data->setDelLine(type, value);
			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
			{
				mFocusTxtData->setOperateType(DelLineType);
				obj->setDrawTxtParam(mFocusTxtData);
			}
		}
		// 模板等
		else
		{
			mFocusTxtData->setOperateType(DelLineType);
			obj->setDrawTxtParam(mFocusTxtData);
		}
	}
	backSetPressStartAndEnd(true);
}
void OperateMgr::doSetFontBold(QList<BaseContainer *> * list, int value)
{
	int size = list->size();
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;
	bool dest = false;
	if (value == 1)
	{
		dest = true;
	}
	bool needReplaceRow = true;
	ViewObj * preViewParent = NULL;
	for (int i = 0; i < size; i++)
	{
		obj = list->at(i);
		if (obj->isView())
		{
			if (obj->isTxt())
			{
				data = (StrContentData *)obj->getContentData();

				if (data->getBold() != dest)
				{
					ViewObj * viewParent = obj->getViewParent();
					//data->setBold(value);
					mFocusTxtData->setOperateType(Bold);
					obj->setDrawTxtParam(mFocusTxtData);
					if (preViewParent != viewParent)
					{
						preViewParent = viewParent;
						viewParent->getLayout()->openReplace();
					}
				}
			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
			{
				mFocusTxtData->setOperateType(Bold);
				obj->setDrawTxtParam(mFocusTxtData);
				needReplaceRow = false;
			}
		}
		else
		{
			mFocusTxtData->setOperateType(Bold);
			obj->setDrawTxtParam(mFocusTxtData);
			BaseTools::openAllRowReplace(obj);
		}
	}
	if (!needReplaceRow)
	{
		return;
	}
	if (value)
		judgeListOverflow(list, true);
	else
		judgeListLess(list, true);
}
void OperateMgr::doSetFontItalic(QList<BaseContainer *> * list, int value)
{
	int size = list->size();
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;
	bool dest = false;
	if (value == 1)
	{
		dest = true;
	}
	//for (int i = 0; i < size; i++)
	//{
	//	view = list->at(i);
	//	if (view->isTxt())
	//	{
	//		data = (StrContentData *)view->getContentData();
	//		if (data->getItalic() != dest)
	//			data->setItalic(value);
	//	}
	//}
	for (int i = 0; i < size; i++)
	{
		obj = list->at(i);
		if (obj->isView())
		{
			if (obj->isTxt())
			{
				data = (StrContentData *)obj->getContentData();

				if (data->getItalic() != dest)
				{
					//data->setItalic(value);
					mFocusTxtData->setOperateType(FontItalic);
					obj->setDrawTxtParam(mFocusTxtData);
				}

			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
			{
				mFocusTxtData->setOperateType(FontItalic);
				obj->setDrawTxtParam(mFocusTxtData);
			}
		}
		else
		{
			mFocusTxtData->setOperateType(FontItalic);
			obj->setDrawTxtParam(mFocusTxtData);
		}
	}
}
void OperateMgr::doSetFontSize(QList<BaseContainer *> * list, int fontSize)
{
	int size = list->size();
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;
	//for (int i = 0; i < size; i++)
	//{
	//	obj = list->at(i);
	//	if (obj->isTxt())
	//	{
	//		data = (StrContentData *)obj->getContentData();
	//		if (data->getPointSize() != fontSize)
	//			data->setPointSize(fontSize);
	//	}
	//}
	bool needReplaceRow = true;
	ViewObj * preViewParent = NULL;
	for (int i = 0; i < size; i++)
	{
		obj = list->at(i);
		if (obj->isView())
		{
			if (obj->isTxt())
			{
				data = (StrContentData *)obj->getContentData();

				if (data->getPointSize() != fontSize)
				{
					ViewObj * viewParent = obj->getViewParent();
					//data->setPointSize(fontSize);
					mFocusTxtData->setOperateType(FontSize);
					obj->setDrawTxtParam(mFocusTxtData);
					if (preViewParent != viewParent)
					{
						preViewParent = viewParent;
						viewParent->getLayout()->openReplace();
					}
				}
			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
			{
				mFocusTxtData->setOperateType(FontSize);
				obj->setDrawTxtParam(mFocusTxtData);
				needReplaceRow = false;
			}
		}
		else
		{
			mFocusTxtData->setOperateType(FontSize);
			obj->setDrawTxtParam(mFocusTxtData);
			BaseTools::openAllRowReplace(obj);
		}
	}

	if (!needReplaceRow)
	{
		return;
	}
	judgeListLess(list, true);
	judgeListOverflow(list, false);
}
void OperateMgr::doSetFontColor(QList<BaseContainer *> * list, int color)
{
	int size = list->size();
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;
	//for (int i = 0; i < size; i++)
	//{
	//	view = list->at(i);
	//	if (view->isTxt())
	//	{
	//		data = (StrContentData *)view->getContentData();
	//		if (data->getFontColor() != color)
	//			data->setFontColor(color);
	//	}
	//}

	for (int i = 0; i < size; i++)
	{
		obj = list->at(i);
		if (obj->isView())
		{
			if (obj->isTxt())
			{
				data = (StrContentData *)obj->getContentData();

				if (data->getFontColor() != color)
					data->setFontColor(color);
			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
			{
				mFocusTxtData->setOperateType(FontColor);
				obj->setDrawTxtParam(mFocusTxtData);
			}
		}
		else
		{
			mFocusTxtData->setOperateType(FontColor);
			obj->setDrawTxtParam(mFocusTxtData);
		}
	}
}
void OperateMgr::doSetBackgroundColor(QList<BaseContainer *> * list, int color)
{
	int size = list->size();
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;
	//for (int i = 0; i < size; i++)
	//{
	//	view = viewList->at(i);
	//	if (view->isTxt())
	//	{
	//		data = (StrContentData *)view->getContentData();
	//		if (data->getBackgroundColor() != color)
	//			data->setBackgroundColor(color);
	//	}
	//}
	for (int i = 0; i < size; i++)
	{
		obj = list->at(i);
		if (obj->isView())
		{
			if (obj->isTxt())
			{
				data = (StrContentData *)obj->getContentData();

				if (data->getBackgroundColor() != color)
					data->setBackgroundColor(color);
			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
			{
				mFocusTxtData->setOperateType(BackgroundColor);
				obj->setDrawTxtParam(mFocusTxtData);
			}
		}
		else
		{
			mFocusTxtData->setOperateType(BackgroundColor);
			obj->setDrawTxtParam(mFocusTxtData);
		}
	}
}
void OperateMgr::doSetFlowType(QList<BaseContainer *> * list, int type)
{
	int size = list->size();
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;
	//for (int i = 0; i < size; i++)
	//{
	//	view = viewList->at(i);
	//	if (view->isTxt())
	//	{
	//		data = (StrContentData *)view->getContentData();
	//		if (data->getFlowType() != type)
	//			data->setFlowType(type);
	//	}
	//}
	bool needReplaceRow = true;
	ViewObj * preViewParent = NULL;
	for (int i = 0; i < size; i++)
	{
		obj = list->at(i);
		if (obj->isView())
		{
			if (obj->isTxt())
			{
				data = (StrContentData *)obj->getContentData();

				if (data->getFlowType() != type)
				{
					mFocusTxtData->setOperateType(FlowType);
					obj->setDrawTxtParam(mFocusTxtData);
					//data->setFlowType(type);
					ViewObj * viewParent = obj->getViewParent();
					if (preViewParent != viewParent)
					{
						preViewParent = viewParent;
						viewParent->getLayout()->openReplace();
					}
				}
			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
			{
				mFocusTxtData->setOperateType(FlowType);
				obj->setDrawTxtParam(mFocusTxtData);
				needReplaceRow = false;
			}
		}
		else
		{
			mFocusTxtData->setOperateType(FlowType);
			obj->setDrawTxtParam(mFocusTxtData);
			BaseTools::openAllRowReplace(obj);
		}
	}
	if (!needReplaceRow)
		return;

	judgeListLess(list, true);
	judgeListOverflow(list, false);
}
void OperateMgr::setTxtNumParams(char type, int value)
{
	int size = mChooseList->size();
	if (size < 1)
		return;

	//QList<BaseContainer *> * viewList = BaseSystem::getInstance()->getClearTempContainerList();
	//BaseTools::exchangeListToViewList(mChooseList, viewList);
	switch (type)
	{
	case BaseConstants::TxtParamActionType::SET_BOLD:
		doSetFontBold(mChooseList, value);
		break;
	case BaseConstants::TxtParamActionType::SET_ITALIC:
		doSetFontItalic(mChooseList, value);
		break;
	case BaseConstants::TxtParamActionType::SET_FONT_SIZE:
		doSetFontSize(mChooseList, value);
		break;
	case BaseConstants::TxtParamActionType::SET_FONT_COLOR:
		doSetFontColor(mChooseList, value);
		break;
	case BaseConstants::TxtParamActionType::SET_BACKGROUND__COLOR:
		doSetBackgroundColor(mChooseList, value);
		break;
	case BaseConstants::TxtParamActionType::SET_FLOW_TYPE:
		doSetFlowType(mChooseList, value);
		break;
	}
	refreshExcel();
}
void OperateMgr::judgeListLess(ViewObj * first, ViewObj * last, bool needOpenFirstRowReplace)
{
	ViewObj * firstRow = first->getViewParent();
	if (!firstRow->isRow())
	{
		return;
	}
	RowLayout * rowLayout = (RowLayout *)firstRow->getLayout();
	if (needOpenFirstRowReplace)
		rowLayout->openReplace();
	rowLayout->judgeLessThanCircleUntilObj(last);
}
void OperateMgr::judgeListOverflow(ViewObj * first, ViewObj * last, bool needOpenFirstRowReplace)
{
	ViewObj * firstRow = first->getViewParent();
	if (!firstRow->isRow())
	{
		return;
	}
	RowLayout * rowLayout = (RowLayout *)firstRow->getLayout();
	if (needOpenFirstRowReplace)
		rowLayout->openReplace();
	rowLayout->judgeOverFlowCircleUntilObj(last);
}
void OperateMgr::judgeListLess(QList<BaseContainer *> * list, bool needOpenFirstRowReplace)
{
	if (list->size() < 1)
	{
		return;
	}
	BaseContainer * first = list->at(0);
	BaseContainer * last = list->at(list->size() - 1);
	ViewObj * firstView = NULL;
	ViewObj * lastView = NULL;
	if (first->isView())
	{
		firstView = (ViewObj *)first;
	}
	else
	{
		firstView = first->getFirstChildView(false);
	}
	if (last->isView())
	{
		lastView = (ViewObj *)last;
	}
	else
	{
		lastView = last->getFirstChildView(false);
	}
	judgeListLess(firstView, lastView, needOpenFirstRowReplace);
	//ViewObj * firstRow = firstView->getViewParent();
	//RowLayout * rowLayout = (RowLayout *)firstRow->getLayout();
	//BaseContainer * last = list->at(list->size() - 1);
	//if (needOpenFirstRowReplace)
	//	rowLayout->openReplace();
	//rowLayout->judgeLessThanCircleUntilObj(last);
}
void OperateMgr::judgeListOverflow(QList<BaseContainer *> * list, bool needOpenFirstRowReplace)
{
	if (list->size() < 1)
	{
		return;
	}
	BaseContainer * first = list->at(0);
	BaseContainer * last = list->at(list->size() - 1);
	ViewObj * firstView = NULL;
	ViewObj * lastView = NULL;
	if (first->isView())
	{
		firstView = (ViewObj *)first;
	}
	else
	{
		firstView = first->getFirstChildView(false);
	}
	if (last->isView())
	{
		lastView = (ViewObj *)last;
	}
	else
	{
		lastView = last->getFirstChildView(false);
	}
	judgeListOverflow(firstView, lastView, needOpenFirstRowReplace);
	//ViewObj * firstView = (ViewObj *)list->at(0);
	//ViewObj * firstRow = firstView->getViewParent();
	//BaseContainer * last = list->at(list->size() - 1);
	//RowLayout * rowLayout = (RowLayout *)firstRow->getLayout();
	//if (needOpenFirstRowReplace)
	//	rowLayout->openReplace();
	//rowLayout->judgeOverFlowCircleUntilObj(last);
}

ViewObj * OperateMgr::refreshNearestObj(char dir)
{
	ViewObj * row = NULL;
	ViewObj * focus = mPressEnd->getFocusObj();
	char focusDir = mPressEnd->getDirection();
	ViewObj * focusRow = focus->getViewParent();
	BaseViewParam * focusParam = focus->getViewParam();
	float sourceX = focusParam->getX();
	if (dir == BaseConstants::Direction::UP)
	{
		row = focusRow->getPreViewFromViewParent();
	}
	else if (dir == BaseConstants::Direction::DOWN)
	{
		row = focusRow->getNextViewFromViewParent();
	}
	if (!row)
		return NULL;
	QList<BaseContainer *> * children = row->getChildren();
	int size = children->size();
	int minDis = BaseConstants::MAX_SHORT;

	int temp = 0;
	int curX = 0;
	int x = 0;

	ViewObj * dest = NULL;
	for (int i = 0; i < size; i++)
	{
		ViewObj * obj = (ViewObj *)children->at(i);
		BaseViewParam * objParam = obj->getViewParam();
		x = objParam->getX();
		temp = BaseMath::getAbs(x - sourceX);
		if (temp < minDis)
		{
			minDis = temp;
			dest = obj;
		}
	}
	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, dest, BaseConstants::Direction::LEFT);
	refreshFlagLogic();
	//setPressStartOrEnd(children->at(destIdx), BaseConstants::Direction::LEFT, false);
	return dest;
}
bool OperateMgr::refreshChooseListUpsidedown(ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir, ViewObj * startRow, ViewObj * endRow)
{
	ViewObj * pagePanel = (ViewObj *)mWriter->getRootView()->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, false)
		->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
	//ViewObj * startFocus = mPressStart->getFocusObj();
	BaseViewParam * startViewParam = startFocus->getViewParam();
	int startFocusIdx = startFocus->getViewIndex();
	if (startDir == BaseConstants::Direction::RIGHT)
	{
		startFocusIdx++;
	}
	BaseViewParam * startRowParam = startRow->getViewParam();
	int startRowIdx = startRow->getViewIndex();
	float startX = startViewParam->getX();
	float startRowY = startRowParam->getAbsY(pagePanel);

	BaseViewParam * endViewParam = endFocus->getViewParam();
	BaseViewParam * endRowParam = endRow->getViewParam();
	int endRowIdx = endRow->getViewIndex();
	float endX = endViewParam->getX();
	float endRowY = endRowParam->getAbsY(pagePanel);

	mIsUpsidedown = false;
	if (startRowY > endRowY)
	{
		mIsUpsidedown = true;
	}
	else if (startRowY == endRowY)
	{
		if (startX > endX)
		{
			mIsUpsidedown = true;
		}
		else if (startX == endX)
		{
			if (startDir == BaseConstants::Direction::RIGHT && endDir == BaseConstants::Direction::LEFT)
			{
				mIsUpsidedown = true;
			}
		}
	}
	return mIsUpsidedown;
}
bool OperateMgr::needRefreshChooseList()
{
	if (mPrePressStart->isSame(mPressStart) && mPrePressEnd->isSame(mPressEnd))
	{
		return false;
	}
	return true;
}
void OperateMgr::clearChooseList(bool needChangeStatus)
{
	if (needChangeStatus)
	{
		int size = mChooseList->size();
		BaseContainer * obj = NULL;
		for (int i = 0; i < size; i++)
		{
			obj = mChooseList->at(i);
			BaseTools::setChooseColor(obj, 0);
			obj->changeStatus(BaseConstants::ObjStatus::FREE, true);
		}
	}
	mChooseList->clear();
}
void OperateMgr::refreshChooseList(bool needChangeStatus, bool needJudgePrePress, bool needRefreshUpsidedown)
{
	if (needJudgePrePress && !needRefreshChooseList())
		return;
	clearChooseList(needChangeStatus);
	//ViewObj * pagePanel = (ViewObj *)mWriter->getRootView()->findChildrenByID(ConstantsID::ContainerId::TXT_WIN_VIEW, false)
	//	->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
	mPrePressStart->setParam(mPressStart->getFocusObj(), mPressStart->getDirection());
	mPrePressEnd->setParam(mPressEnd->getFocusObj(), mPressEnd->getDirection());


	ViewObj * startFocus = mPressStart->getFocusObj();
	//BaseViewParam * startViewParam = startFocus->getViewParam();
	char startDir = mPressStart->getDirection();
	int startFocusIdx = startFocus->getViewIndex();
	if (startFocusIdx < 0)
	{
#ifdef DEBUG
		qdebugh() << "OperateMgr::refreshChooseList startFocusIdx <0";
#endif // DEBUG
		int a = 0;
		return;
	}
	if (startDir == BaseConstants::Direction::RIGHT)
	{
		startFocusIdx++;
	}
	ViewObj * startRow = startFocus->getViewParent();
	int startRowIdx = startRow->getViewIndex();
	if (startRowIdx < 0)
	{
#ifdef DEBUG
		qdebugh() << "OperateMgr::refreshChooseList startRowIdx <0";
#endif // DEBUG

		int a = 0;
		return;
	}
	ViewObj * startTxtLayer = startRow->getViewParent();
	ViewObj * startPage = startTxtLayer->getParentPage();
	int startPageIdx = startPage->getViewIndex();

	ViewObj * endFocus = mPressEnd->getFocusObj();
	BaseViewParam * endViewParam = endFocus->getViewParam();
	char endDir = mPressEnd->getDirection();
	int endFocusIdx = endFocus->getViewIndex();
	if (endFocusIdx < 0)
	{
#ifdef DEBUG
		qdebugh() << "OperateMgr::refreshChooseList endFocusIdx <0";
#endif // DEBUG

		int a = 0;
		return;
	}
	if (endDir == BaseConstants::Direction::RIGHT)
	{
		endFocusIdx++;
	}
	ViewObj * endRow = endFocus->getViewParent();
	int endRowIdx = endRow->getViewIndex();
	if (endRowIdx < 0)
	{
#ifdef DEBUG
		qdebugh() << "OperateMgr::refreshChooseList endRowIdx <0";
#endif // DEBUG

		int a = 0;
		return;
	}
	ViewObj * endTxtLayer = endRow->getViewParent();
	ViewObj * endPage = endTxtLayer->getParentPage();
	int endPageIdx = endPage->getViewIndex();


	if (endFocusIdx == endRow->getChildren()->size())
	{
		int a = 0;
	}
	if (needRefreshUpsidedown)
		refreshChooseListUpsidedown(startFocus, startDir, endFocus, endDir, startRow, endRow);
	if (mIsUpsidedown)
	{
		ViewObj * temp = startTxtLayer;
		startTxtLayer = endTxtLayer;
		endTxtLayer = temp;

		int tempIdx = startFocusIdx;
		startFocusIdx = endFocusIdx;
		endFocusIdx = tempIdx;

		tempIdx = startRowIdx;
		startRowIdx = endRowIdx;
		endRowIdx = tempIdx;

		tempIdx = startPageIdx;
		startPageIdx = endPageIdx;
		endPageIdx = tempIdx;
	}






	if (startTxtLayer->getObjID() != endTxtLayer->getObjID())
	{
		int a = 0;
	}
	int size = mChooseList->size();
	addRowChildrenToChooseList(startTxtLayer, endTxtLayer, startPageIdx, endPageIdx, startRowIdx, endRowIdx, startFocusIdx, endFocusIdx);
	int size2 = mChooseList->size();
	//if (mChooseList->size() > 0)
	//{
	//	int a = 0;
	//}
}
void OperateMgr::addToChooseList(BaseContainer * row, int startObjIdx, int endObjIdx)
{
	if (startObjIdx == endObjIdx)
	{
		return;
	}
	QList<BaseContainer *> * children = row->getChildren();
	if (!children)
	{
		return;
	}
	int size = children->size();
	if (startObjIdx >= size)
	{
		return;
	}
	if (startObjIdx < 0)
	{
		startObjIdx = 0;
	}
	if (endObjIdx == BaseConstants::IndexType::MORE_THAN_RIGHT)
		endObjIdx = size;

	ViewObj * obj = NULL;
	BaseContainer * dest = NULL;
	ViewObj * startAdd = (ViewObj *)children->at(startObjIdx);

	for (int i = startObjIdx; i < endObjIdx; i++)
	{
		obj = (ViewObj *)children->at(i);
		dest = obj->getChooseObj();
		if (!dest)
			continue;
		if (dest->isView())
		{
			addToChooseList(obj);
		}
		else
		{
			addToChooseList(dest);
		}
	}
}
void OperateMgr::addToChooseList(BaseContainer * obj)
{
	int size = mChooseList->size();
	// 判断是否已经有对应父对象被加入
	if (size > 0)
	{
		BaseContainer * last = mChooseList->at(size - 1);
		if (last == obj)
		{
			return;
		}
		BaseContainer * temp = last;
		if (!temp->isView())
		{
			if (temp->isTempleteParent(obj, true))
			{
				return;
			}
			if (!obj->isView())
			{
				if (obj->isTempleteParent(temp, true))
				{
					BaseTools::removeAllChildrenFromList(mChooseList, obj);
				}
			}
		}
	}
	if (mChooseList->indexOf(obj) >= 0)
	{
		return;
	}
	if (mChooseList->size() > 1)
	{
		if (obj->getTempIndex() < mChooseList->at(mChooseList->size() - 1)->getTempIndex())
		{
			int a = 0;
		}
	}
	mChooseList->push_back(obj);
	BaseTools::setChooseColor(obj, UIConstants::MyColor::CHOOSEN_COLOR);
}
void OperateMgr::addRowChildrenToChooseList(ViewObj * startPageLayer, ViewObj * endPageLayer, int startPageIdx, int endPageIdx
	, int startRowIdx, int endRowIdx, int startObjIdx, int endObjIdx)
{
	QList<BaseContainer *> * pageList = NULL;
	QList<BaseContainer *> * rowList = NULL;
	if (startPageIdx != endPageIdx)
	{
		pageList = startPageLayer->findViewFromParent(ConstantsID::ContainerId::PAGE_PANEL, true)->getChildren();
	}
	// 首页
	// 首行
	BaseContainer * row = NULL;
	rowList = startPageLayer->getChildren();

	// 跨页
	if (startPageIdx < endPageIdx)
	{
		// 加尾页
		QList<BaseContainer *> * lastRowList = pageList->at(endPageIdx)->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT
			, false)->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false)->getChildren();
		BaseContainer * lastRow = lastRowList->at(endRowIdx);
		// 首页
		// 首行
		row = rowList->at(startRowIdx);
		BaseContainer * preRow = row;
		if (row != lastRow)
		{
			addToChooseList(row, startObjIdx, BaseConstants::IndexType::MORE_THAN_RIGHT);
		}
		// 后续行
		for (int i = startRowIdx + 1; i < rowList->size(); i++)
		{
			row = rowList->at(i);
			if (preRow == row)
				continue;
			else
			{
				if (row == lastRow)
				{
					continue;
				}
				preRow = row;
			}
			addToChooseList(row, 0, BaseConstants::IndexType::MORE_THAN_RIGHT);
		}

		// 加中间页
		for (int i = startPageIdx + 1; i < endPageIdx; i++)
		{
			rowList = pageList->at(i)->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false)->getChildren();
			for (int i = 0; i < rowList->size(); i++)
			{
				row = rowList->at(i);
				if (preRow == row)
					continue;
				else
				{
					if (row == lastRow)
					{
						continue;
					}
					preRow = row;
				}
				addToChooseList(row, 0, BaseConstants::IndexType::MORE_THAN_RIGHT);
			}
		}
		// 加尾页
		// 前面行
		for (int i = 0; i < endRowIdx; i++)
		{
			row = lastRowList->at(i);
			if (preRow == row)
				continue;
			else
				preRow = row;
			addToChooseList(row, 0, BaseConstants::IndexType::MORE_THAN_RIGHT);
		}
		// 尾行
		row = lastRowList->at(endRowIdx);
		addToChooseList(row, 0, endObjIdx);
	}
	// 同页
	else
	{
		if (startObjIdx != endObjIdx)
		{
			int a = 0;
		}
		// 首行
		row = rowList->at(startRowIdx);
		if (startRowIdx != endRowIdx)
			addToChooseList(row, startObjIdx, BaseConstants::IndexType::MORE_THAN_RIGHT);
		else
			addToChooseList(row, startObjIdx, endObjIdx);
		// 后续行
		for (int i = startRowIdx + 1; i < endRowIdx; i++)
		{
			row = rowList->at(i);
			addToChooseList(row, 0, BaseConstants::IndexType::MORE_THAN_RIGHT);
		}
		// 尾行
		if (startRowIdx != endRowIdx)
		{
			//if (endObjIdx < 7)
			//{
			//	int a = 0;
			//}
			row = rowList->at(endRowIdx);
			addToChooseList(row, 0, endObjIdx);
		}
	}
}
bool OperateMgr::isChoosed(BaseContainer *obj)
{
	if (mChooseList->contains(obj))
	{
		return true;
	}
	return false;
}
int OperateMgr::getChoosedSize()
{
	if (mChooseList)
	{
		return mChooseList->size();
	}
	else
	{
		return 0;
	}
}
QList<BaseContainer *> *OperateMgr::getChooseList()
{
	return mChooseList;
}
bool OperateMgr::hadExcelCellInChooseList()
{
	bool isHaveExcelCell = false;
	int size = mChooseList->size();
	BaseContainer* tempObj;
	for (int i = 0; i < size; i++)
	{
		tempObj = mChooseList->at(i);
		if (tempObj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
		{
			isHaveExcelCell = true;
			break;
		}
	}
	return isHaveExcelCell;
}
int OperateMgr::getUserID()
{
	return 0;
}
void OperateMgr::addStr(QString str, bool needCreateAction, bool needJudgeFileType)
{
	str.remove("\t");
	ViewObj * focus = mPressEnd->getFocusObj();
	char dir = mPressEnd->getDirection();
	int tempIdx = -1;
	int preTempIdx = -1;
	for (int i = 0; i < mChooseList->size(); i++)
	{
		BaseContainer * obj = mChooseList->at(i);
		//if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
		//{
		//	return;
		//}
		if (obj->isTemplateElement())
		{
			clearChooseList(true);
			return;
		}
	}
	BaseTemplate * tempParent = focus->getTempParent();
	preTempIdx = tempParent->getTempIndex();
	BaseTemplate * preTempParent = tempParent;
	if (!judgeCurFocusCanWrite(focus, dir, tempParent, tempIdx, false, needJudgeFileType))
	{
		return;
	}
	// 如果不为容器，代表在模板中，则清空当前选中项，并将光标设置在聚焦模板的最后一项
	if (!preTempParent->isTempContainer() && !preTempParent->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE)
		&& !preTempParent->isObj(ConstantsID::ContainerId::MENSTRUATION_TEMPLETE_OBJ))
	{
		if (dir == BaseConstants::Direction::LEFT && focus == preTempParent->getFirstChildView(true))
		{

		}
		else
		{
			//BaseTemplate * tTempParent = tempParent;
			//while (preTempParent->getTempParent() != tTempParent)
			//{
			//	preTempIdx = preTempParent->getTempIndex();
			//	preTempParent = preTempParent->getTempParent();
			//}
			focus = tempParent->getChild(tempIdx)->getLastChildView(true);
			dir = BaseConstants::Direction::RIGHT;
			clearChooseList(true);
			setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, dir);
		}
	}
	if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
	{
		tempIdx = tempParent->getTempIndex();
		tempParent = tempParent->getTempParent();
	}
	if (needCreateAction)
		mActionMgr->createNewAction();

	ViewObj * row = focus->getViewParent();
	int preW = -1;
	bool needAddAtPreRow = false;
	if (mChooseList->size() > 0)
	{
		ViewObj * firstChooseView = mChooseList->at(0)->getFirstChildView(true);
		if (firstChooseView->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
		{
			needAddAtPreRow = true;
		}
		if (firstChooseView->getViewIndex() == 0 && dir == BaseConstants::Direction::LEFT)
		{
			preW = firstChooseView->getViewParam()->getSelfW();
		}
		if (!pressBackspace(BaseConstants::DoActionType::ONLY_ADD_ACTION))
		{
			mActionMgr->delAction();
			return;
		}
		if (!needAddAtPreRow)
		{
			focus = mPressEnd->getFocusObj();
			dir = mPressEnd->getDirection();
			tempParent = focus->getTempParent();
			BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);
		}
	}
	// 如果焦点在行的第0个左边,代表从第行的第0个开始删除和插入
	else if (focus->getViewIndex() == 0 && dir == BaseConstants::Direction::LEFT)
	{
		if (focus->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
		{
			needAddAtPreRow = true;
		}
		preW = focus->getViewParam()->getSelfW();
	}
	if (needAddAtPreRow)
	{
		focus = mPressEnd->getFocusObj()->getPreViewFromParagraphParent();
		if (focus && !focus->isEnter())
		{
			setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, BaseConstants::Direction::RIGHT);
			dir = mPressEnd->getDirection();
			tempParent = focus->getTempParent();
			BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);
		}
		else
		{
			focus = mPressEnd->getFocusObj();
			dir = mPressEnd->getDirection();
			tempParent = focus->getTempParent();
			BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);
		}
	}

	int userID = getUserID();
	ViewObj * preEndFocus = mPressStart->getFocusObj();
	char preEndDir = mPressStart->getDirection();

	int size = str.length();


	ViewObj * txtLayer = row->getViewParent();
	QList<BaseContainer *> * txtList = new QList<BaseContainer *>();
	//QList<ViewObj *> viewContainer;k
	bool isNeedPressEnter = false;
	QString nextFrame;
	int maxWidth = 0;
	int tempTxtWidth;
	for (int i = 0; i < size; i++)
	{
		QString tempStr = str.mid(i, 1);
		if (tempStr.compare("\n") == 0)
		{
			isNeedPressEnter = true;
			nextFrame = str.mid(i + 1);
			size = txtList->size();
			break;
		}
		ViewObj * txt = ViewFactory::createTxt(mWriter, userID, tempStr);
		txtList->append(txt);
		tempTxtWidth = txt->getViewParam()->getTotalW();
		if (maxWidth < tempTxtWidth)
		{
			maxWidth = tempTxtWidth;
		}
		//viewContainer.append(txt);
	}
	bool isOnlyPressEnter = false;
	if (txtList->size() == 0 && isNeedPressEnter)
	{
		isOnlyPressEnter = true;
	}
	if (!isOnlyPressEnter)
	{
		if (dir == BaseConstants::Direction::RIGHT)
		{
			tempIdx++;
		}
		//是否在表格内

		if (!isContinueIfInExcelCell(focus, maxWidth))
		{
			deleteVector(txtList);
			mActionMgr->delAction();
			MessageBox::showErrorMessageBox(QStringLiteral("加入错误"), QStringLiteral("表格不能容纳此字体"));
			isNeedPressEnter = false;
			nextFrame.clear();
			return;
		}

		ViewObj * tempTxtLayer = focus->findViewFromParent(ConstantsID::ContainerId::CONTENT_TXT_LAYER, true);
		if (tempTxtLayer == NULL)
		{
			int a = 0;
		}
		TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
		tempMgr->addChildren(txtList, 0, txtList->size(), tempIdx, true, NULL, -1);
		//tempMgr->addChild(txtList->at(0), tempIdx, true);
		ViewObj * firstTxt = (ViewObj *)txtList->at(0);
		if (firstTxt->getTempIndex() != firstTxt->getViewIndex())
		{
			int a = 0;
		}
		if (dir == BaseConstants::Direction::RIGHT)
		{
			setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, (ViewObj *)txtList->at(size - 1), BaseConstants::Direction::RIGHT);
		}
		ActionFactory::pushAddObjListAction(mActionMgr, txtList, preEndFocus, preEndDir);

		if (mNeedUndo)
		{
			doUndo();
		}
		else
		{
			if (preEndFocus == focus && preEndDir == dir)
			{
				mNeedRefreshPagePanelYByFlag = true;
			}

			ViewObj * firstStr = (ViewObj *)txtList->at(0);
			int firstW = firstStr->getViewParam()->getSelfW();
			// 如果从第0个开始插，则对比插入前后第0个的宽度,如果宽度减少，则上一行需判断缩进
			if (!needAddAtPreRow && preW > firstW)
			{
				row = firstStr->getViewParent();
				RowLayout * layout = (RowLayout *)row->getLayout();
				RowLayout * preLayout = (RowLayout *)layout->getPreLayout(false);
				preLayout->judgeLessThanCircle(true);
			}
		}
	}

	if (isNeedPressEnter)
	{
		pressEnter(getUserID(), needCreateAction);
	}
	if (nextFrame.length() > 0)
	{
		addStr(nextFrame, needCreateAction);
	}

	mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
}
void OperateMgr::addImg(QString str)
{
	ViewObj * focus = mPressEnd->getFocusObj();
	char dir = mPressEnd->getDirection();
	BaseTemplate * tempParent = focus->getTempParent();
	if (focus->isObj(ConstantsID::ContainerId::TMPLATE_TXT))
	{

		if (focus == tempParent->getFirstView() && dir != BaseConstants::Direction::LEFT)
		{
			return;
		}
		if (focus == tempParent->getLastChildView() && dir != BaseConstants::Direction::RIGHT)
		{
			return;
		}
	}
	mActionMgr->createNewAction();

	if (mChooseList->size() > 0)
	{
		pressBackspace(BaseConstants::DoActionType::ONLY_ADD_ACTION);
	}

	int tempIdx = -1;
	if (!judgeCurFocusCanWrite(focus, dir, tempParent, tempIdx, false, true))
	{
		return;
	}
	if (tempParent->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
	{
		tempIdx = tempParent->getTempIndex();
		tempParent = tempParent->getTempParent();
	}
	int userID = getUserID();
	//BaseContainer* tempParent = focus->getTempParent();
	//int tempIdx = focus->getTempIndex();
	ViewObj * next = NULL;
	if (dir == BaseConstants::Direction::RIGHT)
	{
		tempIdx++;
		next = focus->getNextViewFromViewParent();
	}
	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	ViewObj * img = ViewFactory::createImg(mWriter, NULL, str);
	//如果再表格内，需要判断加入的视图宽度是否大于表格宽度
	ViewObj* excelPage = focus->findViewFromParent(ConstantsID::ContainerId::EXCEL_PAGE, true);
	if (excelPage)
	{
		ViewObj* rowInExcel = focus->findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, true);
		BaseViewParam *  rowParam = rowInExcel->getViewParam();
		int rowW = rowParam->getSelfW();
		BaseViewParam *  imgParam = img->getViewParam();
		if (imgParam->getSelfW() > rowW)
		{
			delete img;
			MessageBox::showErrorMessageBox(QStringLiteral("加入错误"), QStringLiteral("无法插入此图片"));
			mNeedUndo = true;
			doUndo();
			return;
		}
	}
	tempMgr->addChild(img, tempIdx, true, NULL, -1);

	if (img->getTempIndex() != img->getViewIndex())
	{
		int a = 0;
	}
	if (dir == BaseConstants::Direction::RIGHT)
	{
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, next, BaseConstants::Direction::LEFT);
	}

	ViewObj * preStartFocus = mPressStart->getFocusObj();
	char preStartDir = mPressStart->getDirection();
	ViewObj * preEndFocus = mPressEnd->getFocusObj();
	char preEndDir = mPressEnd->getDirection();
	QList<BaseContainer *> * txtList = new QList<BaseContainer *>();
	txtList->push_back(img);
	ActionFactory::pushAddObjListAction(mActionMgr, txtList, preEndFocus, preEndDir);

	if (mNeedUndo)
	{
		doUndo();
	}
	else
	{
		if (preStartFocus == focus && preStartDir == dir)
		{
			mNeedRefreshPagePanelYByFlag = true;
		}
		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
	}
}
void OperateMgr::addFixTextWidget(QMap<QString, QString> *testMap)
{
	FIND_POSITION
		//tempParent = tempParent->getParentTempleteById(ConstantsID::ContainerId::NORMAL_PARAGRAPH_TEMPLETE);
		QString fixName = "固定文本";
	TextWidget *fixTextWidget = TempleteFactory::createNoEditTextWidgetTemplete(tempParent, NULL, -1, false, ConstantsID::ContainerId::FIX_TEXT_WIDGET_TEMPLETE, fixName, true);
	//fixTextWidget->SetElementName(fixName);
	//QString name = testMap->value(TempleteTypeEnumeration::NAME);
	//fixTextWidget->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
	//fixTextWidget->setStr(name);
	//TEMPLETE_CHECK_ADD_TITLE_BOTTOM(fixTextWidget);
	//tempParent->addTempleteChild(fixTextWidget, idx, false, false);
	//fixTextWidget->addWidgetToRow();
	ViewObj *lastObj = fixTextWidget->getLastChildView();

	if (!canAddTemplate(fixTextWidget, focus))
	{
		return;
	}
	//last->setFocus(true, false);
	//mActionMgr->createNewAction();
	AddTempletWidgetAction * addTempleteAction = new AddTempletWidgetAction(mActionMgr->getCurStepMgr());
	addTempleteAction->initAddAction(fixTextWidget, (BaseTemplate*)tempParent, tempIdx, focus, dir, lastObj, BaseConstants::Direction::RIGHT);
	mActionMgr->addToLocalAction(addTempleteAction);


	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	tempMgr->addChild(fixTextWidget, tempIdx, true, NULL, -1);
	lastObj->setFocus(true);

	if (mNeedUndo)
	{
		doUndo();
	}
	else
		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
}
BaseContainer * OperateMgr::createNodeTile(CodeParams * codeParams)
{
	//FIND_POSITION
	ViewObj * focus = mPressEnd->getFocusObj();
	BaseTemplate * tempParent = focus->getTempParent();
	int tempIdx = focus->getTempIndex();
	BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);

	TextWidget *fixTextWidget = TempleteFactory::createNoEditTextWidgetTemplete(tempParent, NULL
		, ConstantsID::ContainerId::NODE_CONTAINER
		, false, ConstantsID::ContainerId::NODE_TITLE, codeParams->getName(), true);
	return fixTextWidget;
}
void OperateMgr::addStructurationTextWidget(QMap<QString, QString> *testMap)
{
	FIND_POSITION

		StructurationTextWidget* structurationTextWidget = TempleteFactory::createStructurationTemplete((BaseTemplate*)tempParent, false);
	//structurationTextWidget->SetElementName("格式化录入");
	QString name = testMap->value(TempleteTypeEnumeration::NAME);
	//structurationTextWidget->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
	//structurationTextWidget->setTitle(name);
	//structurationTextWidget->setTitle("结构化录入");
	//TEMPLETE_CHECK_ADD_TITLE_BOTTOM(structurationTextWidget);
	//tempParent->addTempleteChild(structurationTextWidget, idx, false, false);
	//structurationTextWidget->addWidgetToRow();
	ViewObj* lastObj = structurationTextWidget->getLastChildView();

	if (!canAddTemplate(structurationTextWidget, focus))
	{
		return;
	}
	//lastObj->setFocus(true, false);
	//mActionMgr->createNewAction();
	AddTempletWidgetAction * addTempleteAction = new AddTempletWidgetAction(mActionMgr->getCurStepMgr());
	addTempleteAction->initAddAction(structurationTextWidget, (BaseTemplate*)tempParent, tempIdx, focus, dir, lastObj, BaseConstants::Direction::RIGHT);
	mActionMgr->addToLocalAction(addTempleteAction);
	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	tempMgr->addChild(structurationTextWidget, tempIdx, true, NULL, -1);
	lastObj->setFocus(true);
	if (mNeedUndo)
	{
		doUndo();
	}
	else
		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
}
void OperateMgr::addInputTextWidget(QMap<QString, QString> *testMap)
{
	FIND_POSITION

		//tempParent = tempParent->getParentTempleteById(ConstantsID::ContainerId::NORMAL_PARAGRAPH_TEMPLETE);
		BracketWidget * inputText = TempleteFactory::createInputTextTemplete((BaseTemplate*)tempParent, false);
	QString name = testMap->value(TempleteTypeEnumeration::NAME);
	inputText->setTempleteStatus(BaseConstants::ObjStatus::EDIT_STATUS);
	//inputText->setHint(name);
	inputText->setHint("请输入");
	//inputText->SetElementName("请输入");
	//TEMPLETE_CHECK_ADD_TITLE_BOTTOM(inputText);
	//tempParent->addTempleteChild(inputText, idx, false, false);
	//inputText->addWidgetToRow();
	ViewObj* lastObj = inputText->getLastChildView();

	if (!canAddTemplate(inputText, focus))
	{
		return;
	}
	//lastObj->setFocus(true, false);
	//mActionMgr->createNewAction();
	AddTempletWidgetAction * addTempleteAction = new AddTempletWidgetAction(mActionMgr->getCurStepMgr());
	addTempleteAction->initAddAction(inputText, (BaseTemplate*)tempParent, tempIdx, focus, dir, lastObj, BaseConstants::Direction::RIGHT);
	mActionMgr->addToLocalAction(addTempleteAction);
	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	tempMgr->addChild(inputText, tempIdx, true, NULL, -1);
	lastObj->setFocus(true);
	if (mNeedUndo)
	{
		doUndo();
	}
	else
		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
}
void OperateMgr::addMenStruationTemplete(QMap<QString, QString> *testMap)
{
	FIND_POSITION

		//tempParent = tempParent->getParentTempleteById(ConstantsID::ContainerId::NORMAL_PARAGRAPH_TEMPLETE);
		MenstruationTemplete * templete = TempleteFactory::createMenstruationTemplete((BaseTemplate*)tempParent, false);
	//templete->setValue("{月经初潮期}", "{2009-05-10日}", "4-5", "28-30LMP");

	//TEMPLETE_CHECK_ADD_TITLE_BOTTOM(templete);
	//tempParent->addTempleteChild(templete, idx, false, false);
	templete->setUpAndDownValue("4-5", "28-30LMP", false);
	///判断加入表格的情况


	if (!isContinueIfInExcelCell(focus, templete->getWidth()))
	{
		//tempParent->getChildrenMgr()->delChild(templete, true);
		delete templete;
		templete = NULL;
		mActionMgr->delAction();
		MessageBox::showErrorMessageBox(QStringLiteral("加入错误"), QStringLiteral("表格不能容纳此元素"));
		return;
	}

	//templete->addWidgetToRow();
	//mActionMgr->createNewAction();
	ViewObj * lastView1 = templete->getLastChildView();
	AddTempletWidgetAction * addTempleteAction = new AddTempletWidgetAction(mActionMgr->getCurStepMgr());
	addTempleteAction->initAddAction(templete, (BaseTemplate*)tempParent, tempIdx, focus, dir, lastView1, BaseConstants::Direction::RIGHT);
	mActionMgr->addToLocalAction(addTempleteAction);
	//lastView1->setFocus(true);
	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();

	tempMgr->addChild(templete, tempIdx, true, NULL, -1);
	//templete->addWidgetToRow(row, focus->getViewIndexFromParent() + disIdx);
	//if (dir == BaseConstants::Direction::RIGHT)
	//pressRight();
	lastView1->setFocus(true);
	if (mNeedUndo)
	{
		doUndo();
	}
	else
		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
}
void OperateMgr::addMacroTemplet(QMap<QString, QString> *testMap)
{
	int userID = getUserID();
	FIND_POSITION
		//tempParent = tempParent->getParentTempleteById(ConstantsID::ContainerId::NORMAL_PARAGRAPH_TEMPLETE);
		MacroTemplete * templete = TempleteFactory::createMacroTemplete((BaseTemplate*)tempParent, userID, false);
	//QString type = testMap->value(TempleteTypeEnumeration::TYPE);
	QMapIterator<QString, QString> i(*testMap);
	QString name = testMap->value(TempleteTypeEnumeration::NAME);
	int id = TempleteTypeEnumeration::getMacroIdBaseIdString(testMap->value(TempleteTypeEnumeration::ID));
	templete->setTitle(name);
	//templete->setTitle(name);
	templete->setId(id);
	//
	//TEMPLETE_CHECK_ADD_TITLE_BOTTOM(templete)

	//	tempParent->addTempleteChild(templete, idx, false, false);
	//templete->addWidgetToRow();
	ViewObj * lastView1 = templete->getLastChildView();

	if (!canAddTemplate(templete, focus))
	{
		return;
	}
	//judgeFlagChangePage(mPressEnd, lastView1);
	//mActionMgr->createNewAction();
	AddTempletWidgetAction * addTempleteAction = new AddTempletWidgetAction(mActionMgr->getCurStepMgr());
	addTempleteAction->initAddAction(templete, (BaseTemplate*)tempParent, tempIdx, focus, dir, lastView1, BaseConstants::Direction::RIGHT);
	mActionMgr->addToLocalAction(addTempleteAction);
	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	tempMgr->addChild(templete, tempIdx, true, NULL, -1);
	lastView1->setFocus(true);
	if (mNeedUndo)
	{
		doUndo();
	}
	else
		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
}
void OperateMgr::addTempleteChoiceWedget(QMap<QString, QString> *testMap)
{
	FIND_POSITION
		QString type = testMap->value(TempleteTypeEnumeration::TYPE);
	QString name = testMap->value(TempleteTypeEnumeration::NAME);

	char widgetType = 0;
	if (type == TempleteTypeEnumeration::RADIO)
	{
		widgetType = ChoiceWidgetTemplete::RADIO;

	}
	else if (type == TempleteTypeEnumeration::MULTIPLE_CHOICE)
	{
		widgetType = ChoiceWidgetTemplete::MULTIPLE_CHOICE;
	}
	else if (type == TempleteTypeEnumeration::HAVE_CHOICE)
	{
		widgetType = ChoiceWidgetTemplete::HAVE_CHOICE;
	}
	//int userID = getUserID();
	//tempParent = tempParent->getParentTempleteById(ConstantsID::ContainerId::NORMAL_PARAGRAPH_TEMPLETE);
	ChoiceWidgetTemplete *  choiceTemplete = TempleteFactory::createChoiceWidgetTemplete((BaseTemplate*)tempParent, widgetType, false);
	choiceTemplete->setTitleHintName("选择项");

	/*TEMPLETE_CHECK_ADD_TITLE_BOTTOM(choiceTemplete);
	tempParent->addTempleteChild(choiceTemplete, idx, false, false);
	choiceTemplete->addWidgetToRow();*/
	ViewObj* lastView1 = choiceTemplete->getLastChildView();
	if (!canAddTemplate(choiceTemplete, focus))
	{
		return;
	}

	AddTempletWidgetAction * addTempleteAction = new AddTempletWidgetAction(mActionMgr->getCurStepMgr());
	addTempleteAction->initAddAction(choiceTemplete, (BaseTemplate*)tempParent, tempIdx, focus, dir, lastView1, BaseConstants::Direction::RIGHT);
	mActionMgr->addToLocalAction(addTempleteAction);
	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	tempMgr->addChild(choiceTemplete, tempIdx, true, NULL, -1);
	lastView1->setFocus(true);
	if (mNeedUndo)
	{
		doUndo();
	}
	else
		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
}
void OperateMgr::addUnfoldChoiceWedget(QMap<QString, QString> *testMap)
{
	FIND_POSITION
		QString type = testMap->value(TempleteTypeEnumeration::TYPE);
	QString name = testMap->value(TempleteTypeEnumeration::NAME);

	char widgetType = 0;
	if (type == TempleteTypeEnumeration::UNFOLD_RADIO)
	{
		widgetType = ChoiceWidgetTemplete::RADIO;

	}
	else if (type == TempleteTypeEnumeration::UNFOLD_MULTIPLE_CHOICE)
	{
		widgetType = ChoiceWidgetTemplete::MULTIPLE_CHOICE;
	}
	//int userID = getUserID();
	//tempParent = tempParent->getParentTempleteById(ConstantsID::ContainerId::NORMAL_PARAGRAPH_TEMPLETE);
	UnfoldChoiceWidgetTemplete *  choiceTemplete = TempleteFactory::createUnfoldChoiceWidget((BaseTemplate*)tempParent, widgetType, false);
	choiceTemplete->setTitleHintName("选择项");

	/*TEMPLETE_CHECK_ADD_TITLE_BOTTOM(choiceTemplete);
	tempParent->addTempleteChild(choiceTemplete, idx, false, false);
	choiceTemplete->addWidgetToRow();*/
	ViewObj* lastView1 = choiceTemplete->getLastChildView();
	if (!canAddTemplate(choiceTemplete, focus))
	{
		return;
	}

	AddTempletWidgetAction * addTempleteAction = new AddTempletWidgetAction(mActionMgr->getCurStepMgr());
	addTempleteAction->initAddAction(choiceTemplete, (BaseTemplate*)tempParent, tempIdx, focus, dir, lastView1, BaseConstants::Direction::RIGHT);
	mActionMgr->addToLocalAction(addTempleteAction);
	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	tempMgr->addChild(choiceTemplete, tempIdx, true, NULL, -1);
	lastView1->setFocus(true);
	if (mNeedUndo)
	{
		doUndo();
	}
	else
	{
		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
	}
	/*QList<QString> strList;
	strList.push_back("1111");
	strList.push_back("222");
	strList.push_back("3333");
	int paddingLeft = 10;
	choiceTemplete->setOptions(strList, paddingLeft);*/
}
void OperateMgr::addExcel(int rowNumber, int colNumber, bool isShowBorder)
{
	//FIND_POSITION
	char dir = 0;
	ViewObj * preFocus = NULL;
	char preDir = NULL;
	ViewObj * focus = NULL;
	getFocusAndDirection(focus, dir);
	BaseTemplate* tempParent = NULL;
	BaseContainer* focusTempParent = NULL;
	ViewObj * addRow = NULL;

	BaseContainer* tempContainer = NULL;
	int tempIdx = -1;
	focus->findNormalTempContainerFromParent(tempContainer, tempIdx); /*应为focus是最低层的元素。*/
	tempParent = focus->getTempParent();
	if (tempParent == tempContainer)/*focus是普通文字*/
	{
		if (dir == BaseConstants::Direction::RIGHT)
		{
			tempIdx++;
		}
		focusTempParent = tempParent;
		addRow = focus->getViewParent();
	}
	else
	{
		tempParent = (BaseTemplate *)tempContainer->getChildrenMgr()->getChildren()->at(tempIdx);
		BaseContainer* firstView = tempParent->getFirstChildView();
		focusTempParent = tempParent;
		if (firstView == focus&& dir == BaseConstants::Direction::LEFT)
		{
			addRow = firstView->getViewParent();
		}
		else
		{
			tempIdx++;
			dir = BaseConstants::Direction::RIGHT;
			focus = (ViewObj*)focusTempParent->getLastChildView();
			focus->setFocus(true, false);
			addRow = focus->getViewParent();
		}
		tempParent = (BaseTemplate *)tempContainer;
	}

	ExcelTemplete * excelTemplete = TempleteFactory::createExcelTemplete((BaseTemplate*)tempParent, false);
	//focus->findNormalTempContainerFromParent(tempContainer, tempIdx);
	RowViewParam * rowViewParam = (RowViewParam *)addRow->getViewParam();
	int rowW = rowViewParam->getEffectW();
	excelTemplete->initExcelTemplete(rowNumber, colNumber, rowW, rowViewParam->getInsideH(), addRow, isShowBorder);

	QList<ViewObj *> viewContianer;
	//excelTemplete->getWillShowObjList(&viewContianer);
	/*if (!checkCanAddViewObj(&viewContianer))
	{
	excelTemplete->getTempleteParent()->newDelChild(excelTemplete, true, true, true, true, true);
	if (isPressEnter)
	{
	pressDelOrBackspace(BaseConstants::Direction::LEFT, false);
	}
	deleteAction();
	delete excelTemplete;
	excelTemplete = NULL;
	return;
	}*/

	ViewObj* lastView1 = excelTemplete->getLastChildView();

	AddTempletWidgetAction * addTempleteAction = new AddTempletWidgetAction(mActionMgr->getCurStepMgr());
	addTempleteAction->initAddAction(excelTemplete, (BaseTemplate*)tempParent, tempIdx, focus, dir, excelTemplete->getLastChildView(), mPressEnd->getDirection());
	mActionMgr->addToLocalAction(addTempleteAction);
	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	//mPressEnd->setParam()
	tempMgr->addChild(excelTemplete, tempIdx, true);
	lastView1->setFocus(true);
	if (mNeedUndo)
	{
		doUndo();
	}
	else
		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
}

char OperateMgr::getRowBottomLineType()
{
	return mSystemParams->getRowBottomLineType();
}
bool OperateMgr::setRowBottomLineType(char type, bool needAddAction, bool needRefreshToolbar)
{
	if (mSystemParams->getRowBottomLineType() == type)
	{
		return false;
	}
	if (needAddAction)
	{
		mActionMgr->createNewAction();
		ActionFactory::pushSetSysIntParamAction(mActionMgr, BaseConstants::SetSystemParamType::SET_ROW_BOTTOM_LINE_TYPE, type);
	}
	if (needRefreshToolbar)
	{
		bool isView = (type != 0);
		MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::PageSetType::LineType, isView);
	}

	mSystemParams->setRowBottomLineType(type);
	return true;
}
bool OperateMgr::addHorizontalLine()
{
	ViewObj * focus = mPressEnd->getFocusObj();
	// 元素中不能插横线
	if (focus->isObj(ConstantsID::ContainerId::TMPLATE_TXT))
	{
		return false;
	}
	char dir = mPressEnd->getDirection();
	int tempIdx = -1;
	BaseTemplate * tempParent = focus->getTempParent();
	if (!judgeCurFocusCanWrite(focus, dir, tempParent, tempIdx, false, true))
	{
		return false;
	}
	if (focus->isInDest(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
		return false;

	mActionMgr->createNewAction();

	if (mChooseList->size() > 0)
	{
		pressBackspace(BaseConstants::DoActionType::ONLY_ADD_ACTION);
	}

	focus = mPressEnd->getFocusObj();
	dir = mPressEnd->getDirection();
	if (!judgeCurFocusCanWrite(focus, dir, tempParent, tempIdx, false, true))
	{
		return false;
	}
	int userID = getUserID();
	ViewObj * preEndFocus = mPressEnd->getFocusObj();
	char preEndDir = mPressEnd->getDirection();

	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	ViewObj * line = ViewFactory::createHorizontalLine(mWriter, userID);
	tempMgr->addChild(line, tempIdx, true, NULL, -1);

	QList<BaseContainer *> * list = new QList<BaseContainer *>();
	list->push_back(line);
	ActionFactory::pushAddObjListAction(mActionMgr, list, preEndFocus, preEndDir);

	if (dir == BaseConstants::Direction::RIGHT)
	{
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, line, BaseConstants::Direction::RIGHT);
	}
	if (mNeedUndo)
	{
		doUndo();
		return false;
	}
	else
	{
		mNeedRefreshPagePanelYByFlag = true;

		mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
	}
	return true;
}
bool OperateMgr::addEndPageLine()
{
	ViewObj * focus = mPressEnd->getFocusObj();
	// 元素中不能插横线
	if (focus->isObj(ConstantsID::ContainerId::TMPLATE_TXT))
	{
		return false;
	}
	char dir = mPressEnd->getDirection();
	// 如果不在表格中，则返回
	if (focus->isInDest(ConstantsID::ContainerId::EXCEL_PAGE))
	{
		return false;
	}
	// 如果不在页内容，则返回
	if (!focus->isInDest(ConstantsID::ContainerId::PAGE_CONTENT))
	{
		return false;
	}
	int tempIdx = -1;
	BaseTemplate * tempParent = focus->getTempParent();
	if (!judgeCurFocusCanWrite(focus, dir, tempParent, tempIdx, false, true))
	{
		return false;
	}

	mActionMgr->createNewAction();

	if (mChooseList->size() > 0)
	{
		pressBackspace(BaseConstants::DoActionType::ONLY_ADD_ACTION);
	}

	focus = mPressEnd->getFocusObj();
	dir = mPressEnd->getDirection();
	if (!judgeCurFocusCanWrite(focus, dir, tempParent, tempIdx, false, true))
	{
		return false;
	}
	if (dir == BaseConstants::Direction::RIGHT)
	{
		tempIdx++;
	}
	int userID = getUserID();
	ViewObj * preEndFocus = mPressEnd->getFocusObj();
	char preEndDir = mPressEnd->getDirection();

	TempContainerMgr * tempMgr = (TempContainerMgr *)tempParent->getChildrenMgr();
	ViewObj * line = ViewFactory::createEndPageLine(mWriter, userID);
	tempMgr->addChild(line, tempIdx, true, NULL, -1);

	QList<BaseContainer *> * list = new QList<BaseContainer *>();
	list->push_back(line);
	ActionFactory::pushAddObjListAction(mActionMgr, list, preEndFocus, preEndDir);

	if (dir == BaseConstants::Direction::RIGHT)
	{
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, line, BaseConstants::Direction::RIGHT);
	}
	mNeedRefreshPagePanelYByFlag = true;
	// 激活自动保存的计数器
	mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
	return true;
}


BaseContainer * OperateMgr::getPageByIndex(int pageIndex)
{
	if (pageIndex == -1)
	{
		return mPagePanel->getLastChild(false);
	}
	return mPagePanel->getChild(pageIndex);
}
void OperateMgr::addImgScaleAction(BaseViewParam * viewParam, int w, int h)
{
	mActionMgr->createNewAction();
	ActionFactory::pushZoomImageParamAction(mActionMgr, viewParam, w, h);
	//if (mNeedUndo)
	//{
	//	doUndo();
	//}
}
ActionMgr* OperateMgr::getActionMgr()
{
	return mActionMgr;
}
StepActionManger * OperateMgr::getCurStepMgr()
{
	return mActionMgr->getCurStepMgr();
}
void OperateMgr::createNewAction()
{
	mActionMgr->createNewAction();
}
void OperateMgr::deleteAction()
{
	mActionMgr->delAction();
}
void OperateMgr::addToLocalAction(BaseAction * action)
{
	mActionMgr->addToLocalAction(action);
}
void OperateMgr::addToLocalActionAt(BaseAction * action, int executeOrder)
{
	mActionMgr->addToLocalActionAt(action, executeOrder);
}
QList<BaseAction *> * OperateMgr::getLocalAction()
{
	return mActionMgr->getCurActionList();
}
BaseAction * OperateMgr::getLocalActionLastAction()
{
	return mActionMgr->getCurLastAction();
}
bool OperateMgr::CanFileWrite()
{
	return mFileData->canWrite();
}

QList<StrContentData *> * OperateMgr::getBufDrawParamList()
{
	return mDrawParamList;
}
void OperateMgr::setTitleOrBottomCanWrite(int id, bool canWrite)
{
	if (id == ConstantsID::ContainerId::TITLE_TEMP)
		mSystemParams->setTitleCanWriteByViewSide(canWrite);
	else
		mSystemParams->setBottomCanWriteByViewSide(canWrite);
}
void OperateMgr::addNoTitleNode(QString nodeID, QString nodeName, int level)
{
	addNode(nodeID, nodeName, level, NULL);
}
void OperateMgr::addTitleNode(QString nodeID, QString nodeName, int level, QString titleName)
{
	CodeParams * titleParams = new CodeParams("titleID", titleName);
	addNode(nodeID, nodeName, level, titleParams);
}
void OperateMgr::addNode(QString nodeID, QString nodeName, int level, CodeParams * titleParams)
{
	int returnType = 0;
	if (mChooseList->size() > 0)
	{
		if (!judgeCanDelChooseList())
		{
			return;
		}
		returnType = pressBackspace(BaseConstants::DoActionType::CREATE_STEP_AND_ADD_ACTION, true);
		if (returnType < 0)
		{
			return;
		}
	}
	ViewObj * focus = mPressEnd->getFocusObj();
	char dir = mPressEnd->getDirection();
	BaseTemplate * tempParent = focus->getTempParent();
	int tempIdx = focus->getTempIndex();
	BaseTools::refreshTempContainerAndTempIdx(focus, tempParent, tempIdx);

	if (dir == BaseConstants::Direction::RIGHT)
	{
		// 如果焦点父temp直接为容器temp,则需根据当前光标方向修正idx
		if (focus->getTempParent()->isTempContainer())
		{
			tempIdx++;
		}
	}
	// 前面backspace时已创建动作
	if (returnType >= 0)
		mActionMgr->createNewAction();

	ParagraphManager * paragraphMgr = ((TempContainerMgr *)tempParent->getChildrenMgr())->getParagraphMgr();
	NodeParams * nodeParams = new NodeParams(nodeID, nodeName, level);
	BaseContainer * node = TempleteFactory::createNode(mWriter, paragraphMgr, nodeParams, titleParams);

	NodeMgr * mgr = (NodeMgr *)tempParent->getChildrenMgr();
	bool success = mgr->addChild(node, tempIdx, true, NULL, -1);
	if (success)
		ActionFactory::pushAddObjAction(mActionMgr, node, focus, dir);
	else if (returnType >= 0)
		mActionMgr->delAction();
}


void OperateMgr::loadOtherMedicalRecord(QString id, QByteArray * source)
{
	mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
	mIOTools->setByteArray(source);
	QDataStream * fileStream = mIOTools->readFile();
	return loadOtherMedicalRecord(id, fileStream);
}

void OperateMgr::loadOtherMedicalRecord(QString id, QDataStream * source)
{
	if (!source)
		return;

	ViewObj * focus = NULL;
	//int userID = getUserID();
	BaseTemplate* totalRootTemp = mWriter->getRootTemp();


	//读档
	//if (totalRootTemp)
	//	mSystemParams->doLoad(mIOTools, true);
	//else
	mTempSystemParams = new SystemParams(this);
	mTempSystemParams->doLoad(mIOTools);

	BaseTemplate * curRootTemp = TempleteFactory::createRootTemp(mWriter, mIOTools);
	BaseContainer * contentContainer = curRootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	TempContainerMgr * containerChildrenMgr = (TempContainerMgr *)contentContainer->getChildrenMgr();
	TempParagraphMgrData * curTempContentData = (TempParagraphMgrData *)contentContainer->getContentData();

	BaseContainer * totalTempContent = totalRootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	TempContainerMgr * totalContainerChildrenMgr = (TempContainerMgr *)totalTempContent->getChildrenMgr();
	TempParagraphMgrData * totalTempContentData = (TempParagraphMgrData *)totalTempContent->getContentData();
	QList<BaseContainer *> * recordList = totalTempContent->getChildren();
	int size = recordList->size();
	CodeTempData * data = NULL;
	for (int i = 0; i < size; i++)
	{
		BaseContainer * temp = recordList->at(i);
		data = (CodeTempData *)temp->getContentData();
		data->setAuthority(AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
	}
	BaseContainer * lastTemp = totalContainerChildrenMgr->getLastChild();
	ViewObj * lastView = lastTemp->getLastView();


	//setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, lastView, BaseConstants::Direction::RIGHT);
	//pressEnter();
	// 目标内容
	BaseContainer * destContainer = containerChildrenMgr->getChild(0);
	destContainer->setCodeId(id);
	data = (CodeTempData *)destContainer->getContentData();
	data->setAuthority(AuthorityConstants::Authority::WRITE_ALL);
	totalContainerChildrenMgr->pushChild(destContainer, true);

	TempContainerMgr * destChildrenMgr = (TempContainerMgr *)destContainer->getChildrenMgr();
	QList<BaseParagraph *> * curParagraphList = curTempContentData->getParagraphMgr()->getParagraphList();
	ParagraphManager * totalParagraphMgr = totalTempContentData->getParagraphMgr();
	for (int i = 0; i < curParagraphList->size(); i++)
	{
		BaseParagraph * paragraph = curParagraphList->at(i);
		totalParagraphMgr->addParagraph(paragraph, -1, true);
	}
	destChildrenMgr->setParagraphManager(totalParagraphMgr);
	destChildrenMgr->addRecordChildrenToParagraphAndView(lastView);
	lastView = destContainer->getLastView();
	setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, lastView, BaseConstants::Direction::LEFT);

	delete mTempSystemParams;
	mTempSystemParams = NULL;

	mAutoSaveMgr->changeStatus(BaseConstants::AutoSaveStatus::ACTIVE, BaseConstants::AutoSaveClearTimeType::CLEAR_WHEN_STATUS_CHANGE);
}


void OperateMgr::gotoMedicalRecord(QString id)
{
	//sssss
	ViewObj* focus = NULL;
	char direction = 0;
	BaseViewParam * pagePanelViewParam = NULL;
	int pagePanelY = 0;
	getFocusAndDirectionAndPagePanY(focus, direction, pagePanelViewParam, pagePanelY);
	BaseTemplate* rootTemp = mWriter->getRootTemp();
	BaseContainer * tempContent = rootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	TempContainerMgr * containerChildrenMgr = (TempContainerMgr *)tempContent->getChildrenMgr();
	QList<BaseContainer *> * childrenList = containerChildrenMgr->getChildren();
	ViewObj * destView = NULL;
	BaseContainer * container = NULL;
	for (int i = 0; i < childrenList->size(); i++)
	{
		container = childrenList->at(i);
		if (id == container->getCodeId())
		{
			break;
		}
	}
	bool needSetBackFocus = false;
	BaseContainer * curContainer = focus->findTempFromParent(ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER, true);
	if (curContainer)
	{
		if (curContainer == container)
		{
			needSetBackFocus = true;
		}
	}
	if (needSetBackFocus)
	{

		if (focus)
		{
			setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, direction);
		}
		pagePanelViewParam->setY(pagePanelY);
	}
	else
	{
		focus = container->getFirstView();
		direction = BaseConstants::Direction::LEFT;
		setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, focus, direction);
		//refreshPagePanelByFocus();
	}

}

bool OperateMgr::removeObjAfterLastEnter(BaseChildrenManager * mgr)
{
	if (!mgr->getLastChild()->isEnter())
	{
		QList<BaseContainer *> * tChildren = mgr->getChildren();
		int size = tChildren->size();
		int lastIdx = size - 1;
		int keepNum = -1;
		for (int i = lastIdx; i >= 0; i--)
		{
			BaseContainer * lastEnter = tChildren->at(i);
			if (lastEnter->isEnter())
			{
				keepNum = i + 1;
				break;
			}
		}
		while (tChildren->size() > keepNum)
		{
			tChildren->removeAt(keepNum);
		}
		//BaseContainer * lastEnter = NULL;
		//int enterIdx = -1;
		//for (int i = lastIdx; i >= 0; i--)
		//{
		//	lastEnter = tChildren->at(i);
		//	if (lastEnter->isEnter())
		//	{
		//		enterIdx = i;
		//		lastEnter = tChildren->at(i);
		//		break;
		//	}
		//}
		//if (enterIdx >= 0 && enterIdx < lastIdx)
		//{
		//	tChildren->removeAt(enterIdx);
		//	tChildren->push_back(lastEnter);
		//	BaseContainer * parent = lastEnter->getViewParent();
		//	if (parent)
		//		parent->refreshChildrenIndex(enterIdx);

		//	parent = lastEnter->getTempParent();
		//	if (parent)
		//		parent->refreshChildrenIndex(enterIdx);

		//	parent = ((ViewObj *)lastEnter)->getParagraphParent();
		//	if (parent)
		//		parent->refreshChildrenIndex(enterIdx);
		//}
		return true;
	}
	return false;
}
void OperateMgr::refreshMiddleButtonPlace(float y, float pageH, float totalH)
{
	//ViewObj* rootView = mWriter->getRootView();
	if (!mWindow)
		return;
	ViewObj * scrollBtnMid = (ViewObj *)mWindow->findChildrenByID(ConstantsID::ContainerId::SCROLL_PANEL, false)
		->findChildrenByID(ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE, false)
		->findChildrenByID(ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE_BAR, false);
	if (!scrollBtnMid)
		return;
	ObjMouseManager * muoseMgr = scrollBtnMid->getMouseMgr();
	if (!muoseMgr)
		return;
	muoseMgr->setMiddleButtonParam(y, pageH, totalH);
}
void OperateMgr::addExcelTempelateToOperate(ExcelTemplete* excelTemp)
{
	int index = mExcelTempelateListForFresh->indexOf(excelTemp);
	if (index >= 0)
	{
		return;
	}
	mExcelTempelateListForFresh->push_back(excelTemp);
}
void OperateMgr::refreshExcel()
{
	int size = mExcelTempelateListForFresh->size();
	ExcelTemplete* excel;
	bool isShowed;
	for (int i = 0; i < size; i++)
	{
		excel = mExcelTempelateListForFresh->at(i);
		//isShowed = BaseContainer::isShow(excel);
		//if (isShowed)
		{
			excel->requestLayout(true, true);
		}
	}
	mExcelTempelateListForFresh->clear();
}
void OperateMgr::clearBufferExcelTemp()
{
	mExcelTempelateListForFresh->clear();
}
bool OperateMgr::isContinueIfInExcelCell(ViewObj * focus, int maxWidthOfAddView)
{
	ViewObj * excelCellView = focus->getParentViewById(ConstantsID::ContainerId::EXCEL_PAGE);
	if (excelCellView)
	{
		ViewObj * excelRowView = (ViewObj *)excelCellView->findChildrenByID(ConstantsID::ContainerId::NORMAL_ROW, true);
		int tempMenstruationWidth = maxWidthOfAddView;
		int cellWidth = 0;
		if (excelRowView)
		{
			cellWidth = excelRowView->getViewParam()->getSelfW();
		}
		else
		{
			cellWidth = excelCellView->getViewParam()->getSelfW();
		}

		if (tempMenstruationWidth >= cellWidth)
		{
			return false;
		}
	}
	return true;
}
int OperateMgr::findMaxWidthInTemplate(QList<BaseContainer *> &sourceList)
{
	QList<BaseContainer *> * tempAllViewChildren = getWriterMgr()->getClearTempParagraphViewList();
	int copSize = sourceList.size();
	BaseTools::exchangeListToViewList(&sourceList, 0, copSize, tempAllViewChildren);
	BaseContainer * tempObj = NULL;
	int maxWidth = 0;
	int tempObjWidth = 0;
	for (int i = 0; i < tempAllViewChildren->size(); i++)
	{
		tempObj = tempAllViewChildren->at(i);
		tempObjWidth = ((ViewObj*)tempObj)->getViewParam()->getSelfW();
		if (maxWidth < tempObjWidth)
		{
			maxWidth = tempObjWidth;
		}
	}
	return maxWidth;
}
bool OperateMgr::canExcelCellContainObjList(QList<BaseContainer *> &sourceList, ViewObj* focusInAddPage)
{
	// 如果当前焦点在表格层里
	BaseContainer * row = focusInAddPage->getParentViewById(ConstantsID::ContainerId::NORMAL_ROW);
	if (row)
	{
		BaseContainer * layer = row->getViewParent();
		if (layer->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER))
		{
			bool haveExcel = BaseTools::haveExcel(&sourceList);
			if (haveExcel)
			{
				return false;
			}
			//判断如果加入表格里 是否内容纳下
			if (!canAddTemplateList(sourceList, focusInAddPage))
			{
				//tempParent->getChildrenMgr()->delChild(templete, true);
				MessageBox::showErrorMessageBox(QStringLiteral("加入错误"), QStringLiteral("表格不能容纳某些元素"));
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}

}
bool OperateMgr::canExcelCellContainObj(BaseContainer * addContainer, ViewObj* focusInAddPage)
{
	QList<BaseContainer *> sourceList;
	sourceList.push_back(addContainer);
	return canExcelCellContainObjList(sourceList, focusInAddPage);
}
bool OperateMgr::canAddTemplateList(QList<BaseContainer *> &sourceList, ViewObj* focus)
{
	int maxWidth = findMaxWidthInTemplate(sourceList);
	if (isContinueIfInExcelCell(focus, maxWidth))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool OperateMgr::canAddTemplate(BaseContainer * addContainer, ViewObj* focus)
{
	QList<BaseContainer *> sourceList;
	sourceList.push_back(addContainer);
	if (canAddTemplateList(sourceList, focus))
	{
		return true;

	}
	else
	{
		delete addContainer;
		mActionMgr->delAction();
		MessageBox::showErrorMessageBox(QStringLiteral("加入错误"), QStringLiteral("表格不能容纳此元素"));
		return false;
	}

}
void OperateMgr::setPaperTypeBySystemParam(QPrinter& printer)
{
	if (getSystemParams()->getPaperType() == UIConstants::PaperTypeAndWH::A4)
	{
		printer.setPaperSize(QPrinter::A4);
	}
	else if (getSystemParams()->getPaperType() == UIConstants::PaperTypeAndWH::B5)
	{
		printer.setPaperSize(QPrinter::B5);
	}
}
int OperateMgr::getVersionInt()
{
	return mSystemParams->getVersionInt();
}
QString OperateMgr::getVersion()
{
	return mSystemParams->getVersion();
}