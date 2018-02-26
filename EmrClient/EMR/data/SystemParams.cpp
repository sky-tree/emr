#include "SystemParams.h"

#include "BaseIOTools.h"


#include "MainViewContainer.h"

#include "WriterManager.h"
#include "OperateMgr.h"


#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseTemplate.h"

#include "CodeParams.h"

#include "ParagraphManager.h"
#include "BaseParagraph.h"
#include "ParagraphData.h"

#include "FlagParam.h"
#include "StrContentData.h"

#include "TxtWinData.h"
#include "TempParagraphMgrData.h"

#include "BaseChildrenManager.h"
#include "TempContainerMgr.h"
#include "BaseLayout.h"
#include "NodeChildrenMgr.h"
#include <QtCore/qmath.h> 


#include "BaseViewParam.h"


#include "RowLayout.h"

#include "UserInfo.h"
#include "PatientInfo.h"

#include "ViewFactory.h"

#include "BaseTools.h"
#include "UIConstants.h"
#include "StrConstants.h"
#include "GlobalTemplate.h"
#include "ContentDataFactory.h"
//#define CHAOYANG_VERSION
SystemParams::SystemParams(OperateMgr * mgr)
{
	mOperateMgr = mgr;


	mTitleCanWriteByViewSide = false;
	mBottomCanWriteByViewSide = false;
	mIsTidy = false;

	mMarkType = 0;

	mIsPaperHorizontal = false;
	mPaperType = UIConstants::PaperTypeAndWH::A4;
	mRowBottomLineViewType = BaseConstants::NONE;

	mVersion = StrConstants::VERSION;
	mVersionInt = BaseTools::convertQStringOfVersionToInt(mVersion);
	mDestSaveVersionInt = -1;

	mPageInsideTop = UIConstants::PageParams::INSIDE_TOP;
	mPageInsideBottom = UIConstants::PageParams::INSIDE_BOTTOM;
	mPageInsideLeft = UIConstants::PageParams::INSIDE_LEFT;
	mPageInsideRight = UIConstants::PageParams::INSIDE_RIGHT;
	mTitleMaxH = UIConstants::PageParams::DEFAULT_PAGE_TITLE_MAX_H;
	mBottomMaxH = UIConstants::PageParams::DEFAULT_PAGE_TITLE_MAX_H;

	mIsTitleView = true;
	mIsBottomView = true;
	mTitleNumberViewType = BaseConstants::SystemParams::ONLY_VIEW_PAGE_NUMBER;
	mBottomNumberViewType = BaseConstants::SystemParams::ONLY_VIEW_PAGE_NUMBER;
	mIsParagraphNumberView = false;

	mTitleNumTempIdx = 0;
	mBottomNumTempIdx = 0;
	mTitleNumber = NULL;
	mBottomNumber = NULL;
	mTitleNumNext = NULL;
	mBottomNumNext = NULL;
	mTitleView = NULL;
	mBottomView = NULL;
	mTitleTempContainer = NULL;
	mBottomTempContainer = NULL;


	mCurrentUserInfo = NULL;
	mPatientInfo = new PatientInfo();
	mUserInfoList = new QList<UserInfo *>();
}

SystemParams::~SystemParams()
{
	mOperateMgr = NULL;
	if (mPatientInfo)
	{
		delete mPatientInfo;
		mPatientInfo = NULL;
	}
	if (mUserInfoList)
	{
		deleteVector(mUserInfoList);
		mUserInfoList = NULL;
	}
	mCurrentUserInfo = NULL;

	mTitleView = NULL;
	mBottomView = NULL;
	mTitleNumNext = NULL;
	mBottomNumNext = NULL;

	if (mTitleNumber)
	{
		delete mTitleNumber;
		mTitleNumber = NULL;
	}
	if (mBottomNumber)
	{
		delete mBottomNumber;
		mBottomNumber = NULL;
	}
}

void SystemParams::setDestSaveVersion(int dest)
{
	mDestSaveVersionInt = dest;
}
void SystemParams::saveFirstParamsWithDifferentVersions(BaseIOTools * ioTools)
{
	ioTools->writeString(StrConstants::VERSION);
	int version5 = BaseTools::convertQStringOfVersionToInt(StrConstants::VERSION_0_0_0_5);
	if (mDestSaveVersionInt < 0 || mDestSaveVersionInt >= version5)
	{
		ioTools->writeByte(mMarkType);
	}
}
void SystemParams::loadFirstParamsWithDifferentVersions(BaseIOTools * ioTools)
{
	QString version = ioTools->readString();
	//if (needChangeSysParam)
	mVersion = version;
	mVersionInt = BaseTools::convertQStringOfVersionToInt(mVersion);

	int version5 = BaseTools::convertQStringOfVersionToInt(StrConstants::VERSION_0_0_0_5);
	if (mVersionInt >= version5)
	{
		mMarkType = ioTools->readByte();
	}
}
void SystemParams::saveNormalParamsWithDifferentVersions(BaseIOTools * ioTools)
{
	ioTools->writeBool(mIsPaperHorizontal);
	ioTools->writeShort(mPaperType);
	ioTools->writeByte(mRowBottomLineViewType);
}
void SystemParams::loadNormalParamsWithDifferentVersions(BaseIOTools * ioTools)
{
	mRowBottomLineViewType = BaseConstants::NONE;
	if (mVersion == StrConstants::VERSION_0_0_0_1)
	{
		mIsPaperHorizontal = false;
		mPaperType = UIConstants::PaperTypeAndWH::A4;
	}
	else
	{
		mIsPaperHorizontal = ioTools->readBool();
		mPaperType = ioTools->readShort();

		int version3 = BaseTools::convertQStringOfVersionToInt(StrConstants::VERSION_0_0_0_3);
		if (mVersionInt >= version3)
		{
			mRowBottomLineViewType = ioTools->readByte();
		}
	}
}
void SystemParams::doSave(BaseIOTools * ioTools)
{
	//setDestSaveVersion(4);
	saveFirstParamsWithDifferentVersions(ioTools);

	saveUserInfo(ioTools);
	savePatientInfo(ioTools);


	saveNormalParamsWithDifferentVersions(ioTools);

	ioTools->writeInt(mPageInsideTop);
	ioTools->writeInt(mPageInsideBottom);
	ioTools->writeInt(mPageInsideLeft);
	ioTools->writeInt(mPageInsideRight);
	ioTools->writeInt(mTitleMaxH);
	ioTools->writeInt(mBottomMaxH);

	ioTools->writeBool(mIsTitleView);
	ioTools->writeBool(mIsBottomView);
	ioTools->writeByte(mTitleNumberViewType);
	ioTools->writeByte(mBottomNumberViewType);

	if (mTitleNumber)
	{
		if (mTitleNumberViewType != BaseConstants::NONE)
			ioTools->writeInt(mTitleNumber->getTempIndex());
		else
			ioTools->writeInt(mTitleNumTempIdx);
	}
	else
		ioTools->writeInt(-1);

	if (mBottomNumber)
	{
		if (mBottomNumberViewType != BaseConstants::NONE)
			ioTools->writeInt(mBottomNumber->getTempIndex());
		else
			ioTools->writeInt(mBottomNumTempIdx);
	}
	else
		ioTools->writeInt(-1);

	ioTools->writeBool(mIsParagraphNumberView);
}

void SystemParams::doLoad(BaseIOTools * ioTools)
{
	loadFirstParamsWithDifferentVersions(ioTools);
	//QString version = ioTools->readString();

	////if (needChangeSysParam)
	//	mVersion = version;
	//黄洪 2017.12.22
	//int versionInt = BaseTools::convertQStringOfVersionToInt(mVersion);
	//mVersionInt = versionInt;
#ifdef CHAOYANG_VERSION
	int version3 = BaseTools::convertQStringOfVersionToInt(StrConstants::VERSION_0_0_0_4);
#else
		int version3 = BaseTools::convertQStringOfVersionToInt(StrConstants::VERSION_0_0_0_3);
#endif
	if (mVersionInt < version3)
	{
		ContentDataFactory::isSaveUnfoldChoicePaddingLeft = false;
	}
	else
	{
		ContentDataFactory::isSaveUnfoldChoicePaddingLeft = true;
	}
	loadUserInfoList(ioTools);
	loadPatientInfo(ioTools);

	loadNormalParamsWithDifferentVersions(ioTools);

	int pageInsideTop = ioTools->readInt();
	int pageInsideBottom = ioTools->readInt();
	int pageInsideLeft = ioTools->readInt();
	int pageInsideRight = ioTools->readInt();
	int titleMaxH = ioTools->readInt();
	int bottomMaxH = ioTools->readInt();
	bool isTitleView = ioTools->readBool();
	bool isBottomView = ioTools->readBool();
	char titleNumberViewType = ioTools->readByte();
	char bottomNumberViewType = ioTools->readByte();
	int titleNumTempIdx = ioTools->readInt();
	int bottomNumTempIdx = ioTools->readInt();
	bool isParagraphNumberView = ioTools->readBool();
	//if (needChangeSysParam)
	{
		mPageInsideTop = pageInsideTop;
		mPageInsideBottom = pageInsideBottom;
		mPageInsideLeft = pageInsideLeft;
		mPageInsideRight = pageInsideRight;
		mTitleMaxH = titleMaxH;
		mBottomMaxH = bottomMaxH;

		mIsTitleView = isTitleView;
		mIsBottomView = isBottomView;
		mTitleNumberViewType = titleNumberViewType;
		mBottomNumberViewType = bottomNumberViewType;

		mTitleNumTempIdx = titleNumTempIdx;
		mBottomNumTempIdx = bottomNumTempIdx;

		mIsParagraphNumberView = isParagraphNumberView;
	}
}
void SystemParams::saveUserInfo(BaseIOTools * ioTools)
{
	// 医生信息
	int size = mUserInfoList->size();
	ioTools->writeInt(size);
	//int curInfoIdx = -1;
	for (int i = 0; i < size; i++)
	{
		UserInfo * info = mUserInfoList->at(i);
		info->doSave(ioTools);
		//if (mCurrentUserInfo == info)
		//{
		//	curInfoIdx = i;
		//}
	}
	//// 当前用户索引
	//ioTools->writeInt(curInfoIdx);
}
void SystemParams::savePatientInfo(BaseIOTools * ioTools)
{
	mPatientInfo->doSave(ioTools);
}
void SystemParams::loadUserInfoList(BaseIOTools * ioTools)
{
	int userSize = ioTools->readInt();
	for (int i = 0; i < userSize; i++)
	{
		//QString id = ioTools->readString();
		//QString name = ioTools->readString();
		////if (needChangeSysParam)
		//{
		//	UserInfo * info = new UserInfo(id, name);
		//	info->initInfo(id, name);
		//	mUserInfoList->push_back(info);
		//}
		UserInfo * info = new UserInfo(mOperateMgr);
		info->doLoad(ioTools);
		mUserInfoList->push_back(info);
	}
}
void SystemParams::loadPatientInfo(BaseIOTools * ioTools)
{
	QString id = ioTools->readString();
	QString name = ioTools->readString();
	//if (needChangeSysParam)
	mPatientInfo->initInfo(id, name);
}
void SystemParams::initParams(bool needCreateNumber)
{
	mTitleView = NULL;
	mBottomView = NULL;
	mTitleTempContainer = NULL;
	mBottomTempContainer = NULL;
	//mTitleNumber = NULL;
	//mBottomNumber = NULL;
	mTitleNumNext = NULL;
	mBottomNumNext = NULL;
	initTitleAndBottomView();
	setParagraphNumberView(mIsParagraphNumberView, false);

	bool preTitleView = mIsTitleView;
	bool preBottomView = mIsBottomView;
	initTitleOrBottomPageNumber(needCreateNumber);

	mIsTitleView = preTitleView + 1;
	mIsBottomView = preBottomView + 1;
	setTitleView(preTitleView, false);
	setBottomView(preBottomView, false);
	//WriterManager * writerMgr = mOperateMgr->getWriterMgr();

	//int userID = mOperateMgr->getUserID();
	//if (mTitleNumTempIdx >= 0)
	//{
	//	if (!mTitleTempContainer)
	//	{
	//		mTitleTempContainer = writerMgr->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false)
	//			->findChildrenByID(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, false);
	//	}
	//	if (mIsTitleNumberView)
	//		mTitleNumber = createTitleNumber(writerMgr, mTitleNumTempIdx);
	//	else
	//		mTitleNumNext = mTitleTempContainer->getChild(mTitleNumTempIdx)->getFirstView();
	//}
	//if (mBottomNumTempIdx >= 0)
	//{
	//	if (!mBottomTempContainer)
	//	{
	//		mBottomTempContainer = writerMgr->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false)
	//			->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
	//	}
	//	if (mIsBottomNumberView)
	//		mBottomNumber = createBottomNumber(writerMgr, mBottomNumTempIdx);
	//	else
	//		mBottomNumNext = mBottomTempContainer->getChild(mBottomNumTempIdx)->getFirstView();
	//}
	//setTitleNumberView(mIsTitleNumberView, false);
	//setBottomNumberView(mIsBottomNumberView, false);
}
void SystemParams::refreshTitleAndBottomNumIndex()
{
	if (mTitleNumber)
	{
		mTitleNumTempIdx = mTitleNumber->getTempIndex();
	}
	if (mBottomNumber)
	{
		mBottomNumTempIdx = mBottomNumber->getTempIndex();
	}
}
void SystemParams::initTitleOrBottomPageNumber(bool needCreateNumber)
{
	WriterManager * writerMgr = mOperateMgr->getWriterMgr();
	if (mTitleNumTempIdx >= 0)
	{
		if (!mTitleTempContainer)
		{
			BaseContainer * rootTemp = writerMgr->getRootTemp();
			if (!rootTemp)
				return;
			BaseContainer * container = rootTemp->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false);
			if (!container)
				return;
			mTitleTempContainer = container->findChildrenByID(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, false);
		}
		if (mTitleNumberViewType != BaseConstants::NONE)
		{
			if (needCreateNumber)
				mTitleNumber = createTitleNumber(writerMgr, mTitleNumTempIdx);
			else
				mTitleNumber = (ViewObj *)mTitleTempContainer->findChildrenByID(ConstantsID::ContainerId::TITLE_PAGE_NUMBER, false);
		}
		else
		{
			//mTitleNumTempIdx = mTitleTempContainer->getChildren()->indexOf(mTitleNumber);
			mTitleNumNext = mTitleTempContainer->getChild(mTitleNumTempIdx)->getFirstView();
		}
	}
	if (mBottomNumTempIdx >= 0)
	{

		if (!mBottomTempContainer)
		{
			BaseContainer * rootTemp = writerMgr->getRootTemp();
			if (!rootTemp)
				return;
			BaseContainer * container = rootTemp->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false);
			if (!container)
				return;
			mBottomTempContainer = container->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
		}
		if (mBottomNumberViewType != BaseConstants::NONE)
		{
			if (needCreateNumber)
				mBottomNumber = createBottomNumber(writerMgr, mBottomNumTempIdx);
			else
				mBottomNumber = (ViewObj *)mBottomTempContainer->findChildrenByID(ConstantsID::ContainerId::BOTTOM_PAGE_NUMBER, false);
		}
		else
			mBottomNumNext = mBottomTempContainer->getChild(mBottomNumTempIdx)->getFirstView();
	}
	setTitleNumberViewType(mTitleNumberViewType, false);
	setBottomNumberViewType(mBottomNumberViewType, false);
}
void SystemParams::initUserInfo(UserInfo * source)
{
	mCurrentUserInfo = source;
	if (mUserInfoList->indexOf(source) < 0)
		mUserInfoList->push_back(source);
}
void SystemParams::initPatientInfo(PatientInfo * source)
{
	mPatientInfo = source;
}
UserInfo * SystemParams::getCurrentUserInfo()
{
	return mCurrentUserInfo;
}
PatientInfo * SystemParams::getPatientInfo()
{
	return mPatientInfo;
}
void SystemParams::initTitleAndBottomView()
{
	WriterManager * writer = mOperateMgr->getWriterMgr();
	TxtWinData * winData = mOperateMgr->getWinData();
	if (!winData)
	{
		mTitleView = NULL;
		mBottomView = NULL;
		return;
	}
	//if (!isAbsCreate)
	{
		if (!mTitleView)
			mTitleView = winData->getTitleView();
		if (!mBottomView)
			mBottomView = winData->getBottomView();
	}
	//else
	//{
	//	mTitleView = NULL;
	//	mBottomView = NULL;
	//}
}

QString SystemParams::getVersion()
{
	return mVersion;
}
//void SystemParams::setPaperType(short type)
//{
//	mPaperType = type;
//}
bool SystemParams::isPaperHorizontal()
{
	return mIsPaperHorizontal;
}
char SystemParams::getRowBottomLineType()
{
	return mRowBottomLineViewType;
}
void SystemParams::setRowBottomLineType(char type)
{
	mRowBottomLineViewType = type;
}
short SystemParams::getPaperType()
{
	return mPaperType;
}
int SystemParams::getPageInsideTop()
{
	return mPageInsideTop;
}
int SystemParams::getPageInsideBottom()
{
	return mPageInsideBottom;
}
int SystemParams::getPageInsideLeft()
{
	return mPageInsideLeft;
}
int SystemParams::getPageInsideRight()
{
	return mPageInsideRight;
}
int SystemParams::getTitleMaxH()
{
	return mTitleMaxH;
}
int SystemParams::getBottomMaxH()
{
	return mBottomMaxH;
}
ViewObj * SystemParams::getTitlePageNumber()
{
	return mTitleNumber;
}
ViewObj * SystemParams::getBottomPageNumber()
{
	return mBottomNumber;
}
bool SystemParams::isTitleView()
{
	return mIsTitleView;
}
bool SystemParams::isBottomView()
{
	return mIsBottomView;
}
char SystemParams::getTitleNumViewType()
{
	return mTitleNumberViewType;
}
char SystemParams::getBottomNumViewType()
{
	return mBottomNumberViewType;
}
bool SystemParams::isParagraphNumView()
{
	return mIsParagraphNumberView;
}
bool SystemParams::isTidy()
{
	return mIsTidy;
}
bool SystemParams::refreshTitleNumNext(BaseContainer * obj)
{
	if (!mTitleNumNext || mTitleNumNext != obj)
	{
		return false;
	}
	mTitleNumNext = mTitleNumNext->getNextViewFromParagraphParent();
	BaseTemplate * tempParent = mTitleNumNext->getTempParent();
	BaseTools::refreshTempContainerAndTempIdx(mTitleNumNext, tempParent, mTitleNumTempIdx);
	return true;
}
bool SystemParams::refreshTitleNumNext(QList<BaseContainer *> * list, int start, int num)
{
	if (!mTitleNumNext)
	{
		return false;
	}
	int idx = list->indexOf(mTitleNumNext);
	if (num == -1)
	{
		num = list->size() - start;
	}
	if (idx < start || idx >= start + num)
	{
		return false;
	}
	mTitleNumNext = list->at(start + num)->getFirstView();
	BaseContainer * first = list->at(start);
	BaseTemplate * tempParent = first->getTempParent();
	BaseTools::refreshTempContainerAndTempIdx(first, tempParent, mTitleNumTempIdx);
	return true;
}
bool SystemParams::refreshBottomNumNext(BaseContainer * obj)
{
	if (!mBottomNumNext || mBottomNumNext != obj)
	{
		return false;
	}
	mBottomNumNext = mBottomNumNext->getNextViewFromParagraphParent();
	BaseTemplate * tempParent = mBottomNumNext->getTempParent();
	BaseTools::refreshTempContainerAndTempIdx(mBottomNumNext, tempParent, mBottomNumTempIdx);
	return true;
}
bool SystemParams::refreshBottomNumNext(QList<BaseContainer *> * list, int start, int num)
{
	if (!mBottomNumNext)
	{
		return false;
	}
	int idx = list->indexOf(mBottomNumNext);
	if (num == -1)
	{
		num = list->size() - start;
	}
	if (idx < start || idx >= start + num)
	{
		return false;
	}
	mBottomNumNext = list->at(start + num)->getFirstView();
	BaseContainer * first = list->at(start);
	BaseTemplate * tempParent = first->getTempParent();
	BaseTools::refreshTempContainerAndTempIdx(first, tempParent, mBottomNumTempIdx);
	return true;
}
bool SystemParams::haveChanged(bool isHorisontal, short paperType, int pageInsideT, int pageInsideB, int pageInsideL, int pageInsideR, int titleMaxH, int bottomMaxH)
{
	if (mPageInsideTop != pageInsideT)
	{
		return true;
	}
	if (mPageInsideBottom != pageInsideB)
	{
		return true;
	}
	if (mPageInsideLeft != pageInsideL)
	{
		return true;
	}
	if (mPageInsideRight != pageInsideR)
	{
		return true;
	}
	if (mTitleMaxH != titleMaxH)
	{
		return true;
	}
	if (mBottomMaxH != bottomMaxH)
	{
		return true;
	}
	if (paperType != mPaperType)
	{
		return true;
	}
	if (mIsPaperHorizontal != isHorisontal)
	{
		return true;
	}
	return false;
}
char SystemParams::setPageParams(bool isHorisontal, short paperType, int pageInsideT, int pageInsideB, int pageInsideL
	, int pageInsideR, int titleMaxH, int bottomMaxH)
{
	char haveChange = 0;
	if (mPageInsideTop != pageInsideT)
	{
		mPageInsideTop = pageInsideT;
		haveChange = 1;
	}
	if (mPageInsideBottom != pageInsideB)
	{
		mPageInsideBottom = pageInsideB;
		haveChange = 1;
	}
	if (mPageInsideLeft != pageInsideL)
	{
		mPageInsideLeft = pageInsideL;
		haveChange = 1;
	}
	if (mPageInsideRight != pageInsideR)
	{
		mPageInsideRight = pageInsideR;
		haveChange = 1;
	}
	if (mTitleMaxH != titleMaxH)
	{
		mTitleMaxH = titleMaxH;
		haveChange = 1;
	}
	if (mBottomMaxH != bottomMaxH)
	{
		mBottomMaxH = bottomMaxH;
		haveChange = 1;
	}
	if (paperType != mPaperType)
	{
		mPaperType = paperType;
		haveChange = 2;
	}
	if (mIsPaperHorizontal != isHorisontal)
	{
		mIsPaperHorizontal = isHorisontal;
		haveChange = 2;
	}
	return haveChange;
}
void SystemParams::setTitleView(bool isView, bool absSet)
{
	if (!absSet && mIsTitleView == isView)
		return;
	if (isView)
		showTitle();
	else
		hideTitle();
}
void SystemParams::setBottomView(bool isView, bool absSet)
{
	if (!absSet && mIsBottomView == isView)
		return;
	if (isView)
		showBottom();
	else
		hideBottom();
}
void SystemParams::setParagraphNumberView(bool isView, bool absSet)
{
	if (!absSet && mIsParagraphNumberView == isView)
		return;
	if (isView)
		showParagraphNumber();
	else
		hideParagraphNumber();
}
void SystemParams::setTidy(bool tidy)
{
	if (mIsTidy == tidy)
		return;
	mIsTidy = tidy;
}
void SystemParams::showTitle()
{
	if (mIsTitleView)
	{
		return;
	}
	mIsTitleView = true;
	initTitleAndBottomView();
	mTitleView->getViewParam()->setViewType(BaseConstants::ViewType::NORMAL);
	//setTitleNumberViewType()

	MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::PageOfHeader, true);
}
void SystemParams::showBottom()
{
	if (mIsBottomView)
	{
		return;
	}
	mIsBottomView = true;
	initTitleAndBottomView();
	mBottomView->getViewParam()->setViewType(BaseConstants::ViewType::NORMAL);
	MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::PageOfFooter, true);
}
//void SystemParams::setTitleNumberViewTypeOnly(char type)
//{
//	mTitleNumberViewType = type;
//}
//void SystemParams::setBottomNumberViewTypeOnly(char type)
//{
//	mBottomNumberViewType = type;
//}
void SystemParams::setTitleNumberViewType(char type, bool isAbs)
{
	if (!isAbs && mTitleNumberViewType == type)
	{
		if (mTitleNumberViewType != BaseConstants::NONE && !isTitleView())
		{
			setTitleView(true);
		}
		return;
	}
	char preType = mTitleNumberViewType;
	mTitleNumberViewType = type;
	WriterManager * writer = mOperateMgr->getWriterMgr();

	FlagParam * pressEnd = mOperateMgr->getPressEnd();
	ViewObj * preFocus = pressEnd->getFocusObj();
	char preDir = pressEnd->getDirection();
	BaseContainer * contentTemp = preFocus->findTempFromParent(ConstantsID::ContainerId::CONTENT_TEMP, true);

	// 如果设置为显示
	if (mTitleNumberViewType != BaseConstants::NONE)
	{
		initTitleAndBottomView();
		if (!isTitleView())
		{
			setTitleView(true);
		}
		mTitleView->changeStatus(BaseConstants::ObjStatus::ACTIVE, false);
		mBottomView->changeStatus(BaseConstants::ObjStatus::FREE, false);
		if (!mTitleTempContainer)
		{
			mTitleTempContainer = writer->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false)
				->findChildrenByID(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, false);
		}
		// 如果当前没有页码，则插到最开始
		if (!mTitleNumber)
		{
			if (mTitleNumTempIdx < 0)
				createTitleNumber(writer, 0);
			else
			{
				//ViewObj * focus = mTitleNumNext;
				BaseTemplate * tempParent = mTitleNumNext->getTempParent();
				BaseTools::refreshTempContainerAndTempIdx(mTitleNumNext, tempParent, mTitleNumTempIdx);
				createTitleNumber(writer, mTitleNumTempIdx);
			}
		}
		// 如果当前有页码
		else
		{
			mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mTitleNumNext, BaseConstants::Direction::LEFT);

			if (!mTitleTempContainer)
			{
				mTitleTempContainer = writer->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false)
					->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
			}
			BaseTemplate * tempParent = mTitleNumNext->getTempParent();
			BaseTools::refreshTempContainerAndTempIdx(mTitleNumNext, tempParent, mTitleNumTempIdx);

			StrContentData * data = (StrContentData *)mTitleNumber->getContentData();
			if (mTitleNumberViewType != preType)
				data->refreshTxtWH();
			// 如果设置前为隐藏
			if (preType == BaseConstants::NONE)
			{
				mTitleTempContainer->getChildrenMgr()->addChild(mTitleNumber, mTitleNumTempIdx, true);
			}
			// 如果设置前已为显示，则刷新页码内容
			else
			{
				if (mTitleNumberViewType != preType)
				{
					RowLayout * rowLayout = (RowLayout *)mTitleNumber->getViewParent()->getLayout();
					rowLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
					if (mTitleNumberViewType == BaseConstants::SystemParams::ONLY_VIEW_PAGE_NUMBER)
					{
						rowLayout->judgeLessThanCircle(true);
					}
					else
						rowLayout->judgeOverFlowCircle();
				}
			}
		}
		// 如果之前聚焦在页眉页脚上
		if (!contentTemp)
			mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mTitleNumber, BaseConstants::Direction::RIGHT);
		else
			mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, preFocus, preDir);
		MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::TopPageNumber, true);
	}
	else
	{
		if (mTitleNumber)
		{
			mTitleNumTempIdx = mTitleNumber->getTempIndex();
			mTitleNumNext = mTitleTempContainer->getChild(mTitleNumTempIdx + 1)->getFirstView();
			mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mTitleNumNext, BaseConstants::Direction::LEFT);
			mTitleTempContainer->delChild(mTitleNumber, true);
			// 如果聚焦在页内容上，则将光标恢复到设置前
			if (contentTemp)
				mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, preFocus, preDir);
		}
		MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::TopPageNumber, false);
	}

}
void SystemParams::setBottomNumberViewType(char type, bool isAbs)
{
	if (!isAbs && mBottomNumberViewType == type)
	{
		if (mBottomNumberViewType != BaseConstants::NONE && !isBottomView())
		{
			setBottomView(true);
		}
		return;
	}
	char preType = mBottomNumberViewType;
	mBottomNumberViewType = type;
	WriterManager * writer = mOperateMgr->getWriterMgr();
	FlagParam * pressEnd = mOperateMgr->getPressEnd();
	ViewObj * preFocus = pressEnd->getFocusObj();
	char preDir = pressEnd->getDirection();
	BaseContainer * contentTemp = preFocus->findTempFromParent(ConstantsID::ContainerId::CONTENT_TEMP, true);
	// 如果设置为显示
	if (mBottomNumberViewType != BaseConstants::NONE)
	{
		initTitleAndBottomView();
		if (!isBottomView())
		{
			setBottomView(true);
		}
		mBottomView->changeStatus(BaseConstants::ObjStatus::FREE, false);
		mBottomView->changeStatus(BaseConstants::ObjStatus::ACTIVE, false);
		if (!mBottomTempContainer)
		{
			mBottomTempContainer = writer->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false)
				->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
		}
		// 如果当前没有页码，则插到最开始
		if (!mBottomNumber)
		{
			if (mBottomNumTempIdx < 0)
				createBottomNumber(writer, 0);
			else
			{
				//ViewObj * focus = mBottomNumNext;
				BaseTemplate * tempParent = mBottomNumNext->getTempParent();
				BaseTools::refreshTempContainerAndTempIdx(mBottomNumNext, tempParent, mBottomNumTempIdx);
				createBottomNumber(writer, mBottomNumTempIdx);
			}
		}
		// 如果当前有页码
		else
		{
			mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mBottomNumNext, BaseConstants::Direction::LEFT);

			if (!mBottomTempContainer)
			{
				mBottomTempContainer = writer->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false)
					->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
			}
			BaseTemplate * tempParent = mBottomNumNext->getTempParent();
			BaseTools::refreshTempContainerAndTempIdx(mBottomNumNext, tempParent, mBottomNumTempIdx);

			StrContentData * data = (StrContentData *)mBottomNumber->getContentData();
			if (mBottomNumberViewType != preType)
				data->refreshTxtWH();
			// 如果设置前为隐藏
			if (preType == BaseConstants::NONE)
			{
				mBottomTempContainer->getChildrenMgr()->addChild(mBottomNumber, mBottomNumTempIdx, true);
			}
			// 如果设置前已为显示，则刷新页码内容
			else
			{
				if (mBottomNumberViewType != preType)
				{
					RowLayout * rowLayout = (RowLayout *)mBottomNumber->getViewParent()->getLayout();
					rowLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
					if (mBottomNumberViewType == BaseConstants::SystemParams::ONLY_VIEW_PAGE_NUMBER)
					{
						rowLayout->judgeLessThanCircle(true);
					}
					else
						rowLayout->judgeOverFlowCircle();
				}
			}
		}
		// 如果之前聚焦在页眉页脚上
		if (!contentTemp)
			mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mBottomNumber, BaseConstants::Direction::RIGHT);
		else
			mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, preFocus, preDir);

		MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::BottomPageNumber, true);
	}
	else
	{
		if (mBottomNumber)
		{
			mBottomNumTempIdx = mBottomNumber->getTempIndex();
			mBottomNumNext = mBottomTempContainer->getChild(mBottomNumTempIdx + 1)->getFirstView();
			mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mBottomNumNext, BaseConstants::Direction::LEFT);
			mBottomTempContainer->delChild(mBottomNumber, true);
			// 如果聚焦在页内容上，则将光标恢复到设置前
			if (contentTemp)
				mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, preFocus, preDir);
		}
		MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::BottomPageNumber, false);
	}

	//initTitleAndBottomView();
	//if (!isBottomView())
	//{
	//	setBottomView(true);
	//}
	//mTitleView->changeStatus(BaseConstants::ObjStatus::ACTIVE, false);
	//mBottomView->changeStatus(BaseConstants::ObjStatus::FREE, false);
	//if (!mBottomTempContainer)
	//{
	//	mBottomTempContainer = writer->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false)
	//		->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
	//}
	//// 如果当前没有页码，则插到最开始
	//if (!mBottomNumber)
	//{
	//	if (mBottomNumTempIdx < 0)
	//		createBottomNumber(writer, 0);
	//	else
	//	{
	//		//ViewObj * focus = mTitleNumNext;
	//		BaseTemplate * tempParent = mBottomNumNext->getTempParent();
	//		BaseTools::refreshTempContainerAndTempIdx(mBottomNumNext, tempParent, mBottomNumTempIdx);
	//		createBottomNumber(writer, mBottomNumTempIdx);
	//	}
	//}
	//else
	//{
	//	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mBottomNumNext, BaseConstants::Direction::LEFT);

	//	if (!mBottomTempContainer)
	//	{
	//		mBottomTempContainer = writer->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false)
	//			->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
	//	}
	//	BaseTemplate * tempParent = mBottomNumNext->getTempParent();
	//	BaseTools::refreshTempContainerAndTempIdx(mBottomNumNext, tempParent, mBottomNumTempIdx);

	//	mBottomTempContainer->getChildrenMgr()->addChild(mBottomNumber, mBottomNumTempIdx, true);
	//}
	//mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mBottomNumber, BaseConstants::Direction::RIGHT);
	//MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::BottomPageNumber, true);
}
ViewObj * SystemParams::createTitleNumber(WriterManager * writer, int tempIdx)
{
	NodeChildrenMgr * nodeMgr = mTitleTempContainer->getNodeChildrenMgr();
	ParagraphManager * mgr = nodeMgr->getParagraphMgr();
	ViewObj * firstView = mTitleTempContainer->getChild(tempIdx)->getFirstView();

	BaseParagraph * paragraph = firstView->getParagraphParent();
	ParagraphData * paragraphData = paragraph->getParagraphData();
	CodeParams * codeParams = paragraphData->getCodeParams();
	if (codeParams)
	{
		// 如果当前对象不可写,则取下一段
		if (codeParams->judgeAuthority() == 0)
		{
			paragraph = mgr->getParagrapAt(paragraph->getIndexFromParagraphMgr() + 1);
			firstView = (ViewObj *)paragraph->getChild(0);
			BaseTemplate * tempParent = firstView->getTempParent();
			BaseTools::refreshTempContainerAndTempIdx(firstView, tempParent, tempIdx);
		}
	}

	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, firstView, BaseConstants::Direction::LEFT);

	mTitleNumber = ViewFactory::createPageNumber(writer, codeParams, ConstantsID::ContainerId::TITLE_PAGE_NUMBER);
	if (!mTitleTempContainer)
	{
		mTitleTempContainer = writer->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::TITLE_TEMP, false)
			->findChildrenByID(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, false);
	}
	mTitleTempContainer->getChildrenMgr()->addChild(mTitleNumber, tempIdx, true);
	mTitleNumNext = firstView;
	mTitleNumTempIdx = tempIdx;
	return mTitleNumber;
}
ViewObj *  SystemParams::createBottomNumber(WriterManager * writer, int tempIdx)
{
	ViewObj * firstView = mBottomTempContainer->getChild(tempIdx)->getFirstView();
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, firstView, BaseConstants::Direction::LEFT);

	mBottomNumber = ViewFactory::createPageNumber(writer, NULL, ConstantsID::ContainerId::BOTTOM_PAGE_NUMBER);
	if (!mBottomTempContainer)
	{
		mBottomTempContainer = writer->getRootTemp()->findChildrenByID(ConstantsID::ContainerId::BOTTOM_TEMP, false)
			->findChildrenByID(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, false);
	}
	mBottomTempContainer->getChildrenMgr()->addChild(mBottomNumber, tempIdx, true);
	mBottomNumNext = firstView;
	mBottomNumTempIdx = tempIdx;
	return mBottomNumber;
}
void SystemParams::showParagraphNumber()
{
	if (mIsParagraphNumberView)
	{
		return;
	}
	mIsParagraphNumberView = true;
	MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::ParagraphNumber, true);
}
void SystemParams::hideTitle()
{
	if (!mIsTitleView)
	{
		return;
	}
	mIsTitleView = false;
	initTitleAndBottomView();
	if (mTitleView)
		mTitleView->getViewParam()->setViewType(BaseConstants::ViewType::HIDE);
	FlagParam * pressEnd = mOperateMgr->getPressEnd();
	ViewObj * endFocus = pressEnd->getFocusObj();
	ViewObj * pagePart = endFocus->findViewFromParent(ConstantsID::ContainerId::PAGE_TITLE, true);
	ViewObj * page = mOperateMgr->getEffectLogicPage();

	if (!page)
	{
		page = endFocus->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	}
	// 如果当前光标在页眉中
	if (pagePart)
	{
		pagePart = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false);
		if (page->getViewParent())
		{
			// 页内容中第一行里的第一个view对象
			endFocus = (ViewObj *)pagePart->getChild(0)->getChild(0)->getChild(0);
		}
		else
		{
			QList<BaseContainer *> * children = mOperateMgr->getPagePanel()->getChildren();
			BaseContainer * lastPage = children->at(children->size() - 1);
			endFocus = (ViewObj *)lastPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false)
				->getChild(0)->getChild(0);
		}
		mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, endFocus, BaseConstants::Direction::RIGHT);
	}
	MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::PageOfHeader, false);
}
void SystemParams::hideBottom()
{
	if (!mIsBottomView)
	{
		return;
	}
	mIsBottomView = false;
	initTitleAndBottomView();
	if (mBottomView)
		mBottomView->getViewParam()->setViewType(BaseConstants::ViewType::HIDE);

	FlagParam * pressEnd = mOperateMgr->getPressEnd();
	ViewObj * endFocus = pressEnd->getFocusObj();
	ViewObj * pagePart = endFocus->findViewFromParent(ConstantsID::ContainerId::PAGE_BOTTOM, true);
	ViewObj * page = mOperateMgr->getEffectLogicPage();
	if (!page)
	{
		page = endFocus->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
	}
	// 如果当前光标在页眉中
	if (pagePart)
	{
		pagePart = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false);
		if (page->getViewParent())
		{
			// 页内容中第一行里的第一个view对象
			endFocus = (ViewObj *)pagePart->getChild(0)->getChild(0)->getChild(0);
		}
		else
		{
			QList<BaseContainer *> * children = mOperateMgr->getPagePanel()->getChildren();
			BaseContainer * lastPage = children->at(children->size() - 1);
			endFocus = (ViewObj *)lastPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false)
				->getChild(0)->getChild(0);
		}
		mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, endFocus, BaseConstants::Direction::RIGHT);
	}
	MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::PageOfFooter, false);
}
//void SystemParams::hideTitleNumber(bool absSet)
//{
//	if (!absSet && !mIsTitleNumberView)
//	{
//		return;
//	}
//	mIsTitleNumberView = false;
//	if (mTitleNumber)
//	{
//		mTitleNumTempIdx = mTitleNumber->getTempIndex();
//		mTitleNumNext = mTitleTempContainer->getChild(mTitleNumTempIdx + 1)->getFirstView();
//		mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mTitleNumNext, BaseConstants::Direction::LEFT);
//		mTitleTempContainer->delChild(mTitleNumber, true);
//	}
//	MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::TopPageNumber, false);
//}
//void SystemParams::hideBottomNumber(bool absSet)
//{
//	if (!absSet && !mIsBottomNumberView)
//	{
//		return;
//	}
//	mIsBottomNumberView = false;
//	if (mBottomNumber)
//	{
//		mBottomNumTempIdx = mBottomNumber->getTempIndex();
//		mBottomNumNext = mBottomTempContainer->getChild(mBottomNumTempIdx + 1)->getFirstView();
//		mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, mBottomNumNext, BaseConstants::Direction::LEFT);
//		mBottomTempContainer->delChild(mBottomNumber, true);
//	}
//	MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::BottomPageNumber, false);
//}
void SystemParams::hideParagraphNumber()
{
	if (!mIsParagraphNumberView)
	{
		return;
	}
	mIsParagraphNumberView = false;
	MainViewContainer::getInstance()->modifyMenuStatus(MainViewContainer::ParagraphNumber, false);
}
void SystemParams::setTitleCanWriteByViewSide(bool canWrite)
{
	mTitleCanWriteByViewSide = canWrite;
}
bool SystemParams::canTitleWriteByViewSide()
{
	return mTitleCanWriteByViewSide;
}
void SystemParams::setBottomCanWriteByViewSide(bool canWrite)
{
	mBottomCanWriteByViewSide = canWrite;
}
bool SystemParams::canBottomWriteByViewSide()
{
	return mBottomCanWriteByViewSide;
}
//int SystemParams::convertQStringOfVersionToInt(QString version1)
//{
//	QList<QString> versionList = version1.split(".");
//	int size = versionList.size();
//	int index = size;
//	int currentInt = 0;
//	int totalInt = 0;
//	for (int i = 0; i < size; i++)
//	{
//		index--;
//		currentInt = versionList.at(i).toInt();
//		currentInt = currentInt*qPow(10, index);
//		totalInt += currentInt;
//	}
//	return totalInt;
//}
int SystemParams::getVersionInt()
{
	return mVersionInt;
}
//bool SystemParams::isOverOrEqualsVersion(QString dest)
//{
//	return BaseTools::isOverOrEqualsVersion(mVersionInt, dest);
//}