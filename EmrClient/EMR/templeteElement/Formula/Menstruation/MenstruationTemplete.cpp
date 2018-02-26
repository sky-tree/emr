#include "MenstruationTemplete.h"
#include "TempleteFactory.h"
#include "TextWidget.h"
//#include "ViewParamFactory.h"
#include "ViewFactory.h"
#include "TempleteViewObj.h"
#include "TextWidget.h"
//#include "NormalRowParam.h"
#include "BaseLayout.h"
#include "RowLayout.h"
//#include "DrawTxtParam.h"
//#include "AbsLayoutViewParam.h"
#include "BaseMath.h"
//#include "LayoutFactory.h"
//#include "ParagraphTemplete.h"
#include "TempleteTypeEnumeration.h"
//#include "BaseOperateMgr.h"
#include "ParagraphData.h"
#include "ConstantsID.h"
#include "UIConstants.h"
#include "FlagParam.h"
#include "qdebug.h"
#include "NoCopyTextWidget.h"
#include "MenStruationDataParam.h"
#include "QFont.h"
#include"views/propertySettingDlgs/FormulaDlg.h"
#include "StrContentData.h"
#include "BaseConstants.h"
#include "BaseParagraph.h"
#include "DrawFactory.h"
#include "BaseViewParam.h"
#include "DrawRect.h"
#include "ObjDrawManager.h"
#include "RowViewParam.h"
#include "OperateMgr.h"
#include "StrConstants.h"
//#include "NormalRowParam.h"
#include "SetTxtWidgetElementAction.h"
#include "SetMenstruationWidgetAction.h"
const float MenstruationTemplete::mTxtRio = 1.3f;

MenstruationTemplete::MenstruationTemplete(WriterManager * wMgr, BaseTemplate * templeteParent, int objId, QString codeId, bool createFromLoad)
: BaseTemplate(wMgr, ConstantsID::ClassId::MENSTRUATION_TEMPLETE, objId, codeId)
, mFocusTxtData(NULL)
, mParagraph(NULL)
{
	mIsShowed = false;
	//ParagraphData * pagagraphData = TempleteFactory::createParaghData(mParagraph);
	mParagraph = TempleteFactory::createParagraph(wMgr, NULL, NULL);
	//pagagraphData->initOwner(mParagraph);
	//pagagraphData->setRowMultiple(0);
	if (!createFromLoad){
		StrContentData *  tempFocusData = getFocusStrData();
		char underLineType = tempFocusData->getUnderLineType();
		int underLineThick = tempFocusData->getUnderLineThick();
		tempFocusData->setUnderLine(BaseConstants::NONE, 0);
		
		mLeftTextWidget = TempleteFactory::createNoEditTextWidgetTemplete(mParagraph, NULL, -1, false, ConstantsID::ContainerId::MENSTRUATION_TEXT_TEMPLETE_OBJ, "1", true);
		mLeftTextWidget->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
		mLeftTextWidget->setStr(" ");
		
		//mRightTextWidget = TempleteFactory::createKeywordTitleTextWidgetTemplete(this, NULL, BaseConstants::ContainerId::TEXT_WIDGET_OF_MENSTRUATION);
		mRightTextWidget = TempleteFactory::createNoEditTextWidgetTemplete(mParagraph, NULL, -1, false, ConstantsID::ContainerId::MENSTRUATION_TEXT_TEMPLETE_OBJ, "2", true);
		mRightTextWidget->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
		mRightTextWidget->setStr(" ");

		tempFocusData->setUnderLine(underLineType, underLineThick);
		mUpTextWidget = TempleteFactory::createNoEditTextWidgetTemplete(mParagraph, NULL, -1, false, ConstantsID::ContainerId::MENSTRUATION_TEXT_TEMPLETE_OBJ, "3", true);
		mUpTextWidget->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
		//mUpTextWidget->setStr("4-5");
		mDownTextWidget = TempleteFactory::createNoEditTextWidgetTemplete(mParagraph, NULL, -1, false, ConstantsID::ContainerId::MENSTRUATION_TEXT_TEMPLETE_OBJ, "4", true);
		mDownTextWidget->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
		//mDownTextWidget->setStr("28 - 30LMP");
	}
	//addTempleteChild(mLeftTextWidget, -1, false);
	//addTempleteChild(mRightTextWidget, -1, false);
	//addTempleteChild(mUpTextWidget, -1, false);
	//addTempleteChild(mDownTextWidget, -1, false);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
	mContainerObj = new TempleteViewObj(mWriterMgr, this, ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION);
	childrenMgr->addChild(mContainerObj, -1, true);
	//mContainerObj->getOperateMgr()->setCanAddObjParentId(templeteParent->getObjId());
	//mContainerObj->getOperateMgr()->setCanBeChoosenObjId(getObjId());
	//AbsLayoutViewParam * rowParam = (AbsLayoutViewParam *)ViewParamFactory::createViewParam(ConstantsID::ClassId::ABS_LAYOUT_VIEW_PARAM, mContainerObj, 0, 0
	//	, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	// 矩形绘制参数
	//DrawRectParam * rectParam = DrawFactory::createDrawRectParam(mContainerObj, UIConstants::FillRectType::ROUND_LINE_AND_FILL
	//	, BaseConstants::NONE, BaseConstants::NONE, 1);
	//, BaseConstants::MyColor::GREEN, BaseConstants::NONE, 1);
	//DrawRect * rect = DrawFactory::createRect(mContainerObj, rectParam);
	//LayoutFactory::createAbsLayout(mContainerObj);
	ObjDrawManager * drawMgr = mContainerObj->getDrawMgr();
	BaseViewParam * viewParam = new BaseViewParam(mContainerObj);
	viewParam->init(0, 0, 0, 0);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	/*DrawRect * rect = new DrawRect(mContainerObj, UIConstants::FillRectType::ONLY_ROUND_RECT, UIConstants::MyColor::GREEN, UIConstants::MyColor::GREEN, 1);
	drawMgr->initDraws(rect, NULL);*/
	
	BaseLayout * layout = new BaseLayout(mContainerObj);


	mLeftRow = ViewFactory::createMenstruationRow(wMgr, mParagraph);
	//RowLayout * layout = (RowLayout *)mLeftRow->getLayout();
	//layout->setChildrenLimitType(BaseConstants::NONE);
	mRightRow = ViewFactory::createMenstruationRow(wMgr, mParagraph);
	//layout = (RowLayout *)mRightRow->getLayout();
	//layout->setChildrenLimitType(BaseConstants::NONE);
	mUpRow = ViewFactory::createMenstruationRow(wMgr, mParagraph);
	//layout = (RowLayout *)mUpRow->getLayout();
	//layout->setChildrenLimitType(BaseConstants::NONE);
	mDownRow = ViewFactory::createMenstruationRow(wMgr, mParagraph);
	//layout = (RowLayout *)mDownRow->getLayout();
	//layout->setChildrenLimitType(BaseConstants::NONE);
	layout->addChild(mLeftRow, -1, false, BaseConstants::NONE);
	layout->addChild(mRightRow, -1, false, BaseConstants::NONE);
	layout->addChild(mUpRow, -1, false, BaseConstants::NONE);
	layout->addChild(mDownRow, -1, false, BaseConstants::NONE);
	creatLine();
	layout->addChild(mMiddleLine, -1, false, BaseConstants::NONE);
	if (!createFromLoad)
	{
		initTxtWidgetChooseId();
	}
}
MenstruationTemplete::MenstruationTemplete(WriterManager * wMgr, MenstruationTemplete* otherMenstruation) : BaseTemplate(wMgr, otherMenstruation, false)
, mParagraph(NULL)
, mFocusTxtData(NULL)
{
	mFocusTxtData = new StrContentData(this, NULL, otherMenstruation->getSelfFocusData()->getString(), otherMenstruation->getSelfFocusData());
	mIsShowed = false;
	//if (!createFromLoad)
	{
		mLeftTextWidget = new NoCopyTextWidget(wMgr, (NoCopyTextWidget*)otherMenstruation->mLeftTextWidget);
		//mRightTextWidget = TempleteFactory::createKeywordTitleTextWidgetTemplete(this, NULL, BaseConstants::ContainerId::TEXT_WIDGET_OF_MENSTRUATION);
		/*mRightTextWidget = TempleteFactory::createNoEditTextWidgetTemplete(this, NULL, false, ConstantsID::ContainerId::MENSTRUATION_TEXT_TEMPLETE_OBJ);
		mRightTextWidget->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
		mRightTextWidget->setStr(" ");*/
		mRightTextWidget = new NoCopyTextWidget(wMgr, (NoCopyTextWidget*)otherMenstruation->mRightTextWidget);
		/*mUpTextWidget = TempleteFactory::createNoEditTextWidgetTemplete(this, NULL, false, ConstantsID::ContainerId::MENSTRUATION_TEXT_TEMPLETE_OBJ);
		mUpTextWidget->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);*/
		mUpTextWidget = new NoCopyTextWidget(wMgr, (NoCopyTextWidget*)otherMenstruation->mUpTextWidget);
		/*mDownTextWidget = TempleteFactory::createNoEditTextWidgetTemplete(this, NULL, false, ConstantsID::ContainerId::MENSTRUATION_TEXT_TEMPLETE_OBJ);
		mDownTextWidget->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);*/
		mDownTextWidget = new NoCopyTextWidget(wMgr, (NoCopyTextWidget*)otherMenstruation->mDownTextWidget);
	
	}
	mContainerObj = new TempleteViewObj(mWriterMgr, this, ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
	childrenMgr->addChild(mContainerObj, -1, true);
	//mContainerObj = new ViewObj(this, NULL, ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION);
	//AbsLayoutViewParam * rowParam = (AbsLayoutViewParam *)ViewParamFactory::createViewParam(ConstantsID::ClassId::ABS_LAYOUT_VIEW_PARAM, mContainerObj, 0, 0
	//	, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	// 矩形绘制参数
	//DrawRectParam * rectParam = DrawFactory::createDrawRectParam(mContainerObj, UIConstants::FillRectType::ROUND_LINE_AND_FILL
	//	, BaseConstants::NONE, BaseConstants::NONE, 1);
	//, BaseConstants::MyColor::GREEN, BaseConstants::NONE, 1);
	//DrawRect * rect = DrawFactory::createRect(mContainerObj, rectParam);
	//LayoutFactory::createAbsLayout(mContainerObj);
	ObjDrawManager * drawMgr = mContainerObj->getDrawMgr();
	BaseViewParam * viewParam = new BaseViewParam(mContainerObj);
	viewParam->init(0, 0, 0, 0);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE, BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE);
	/*DrawRect * rect = new DrawRect(mContainerObj, UIConstants::FillRectType::ONLY_ROUND_RECT, UIConstants::MyColor::YELLOW, BaseConstants::NONE, 1);
	drawMgr->initDraws(rect, NULL);*/

	BaseLayout * layout = new BaseLayout(mContainerObj);
	//ParagraphData * pagagraphData = TempleteFactory::createParaghData(mParagraph);
	//mParagraph = TempleteFactory::createParagraph(this, NULL, pagagraphData);
	mParagraph = TempleteFactory::createParagraph(wMgr, NULL, NULL);
	mLeftTextWidget->setTempleteParent(mParagraph);
	mRightTextWidget->setTempleteParent(mParagraph);
	mUpTextWidget->setTempleteParent(mParagraph);
	mDownTextWidget->setTempleteParent(mParagraph);

	//pagagraphData->initOwner(mParagraph);
	//pagagraphData->setRowMultiple(0);

	mLeftRow = ViewFactory::createMenstruationRow(wMgr, mParagraph);
	//RowLayout * layout = (RowLayout *)mLeftRow->getLayout();
	//layout->setChildrenLimitType(BaseConstants::NONE);
	mRightRow = ViewFactory::createMenstruationRow(wMgr, mParagraph);
	//layout = (RowLayout *)mRightRow->getLayout();
	//layout->setChildrenLimitType(BaseConstants::NONE);
	mUpRow = ViewFactory::createMenstruationRow(wMgr, mParagraph);
	//layout = (RowLayout *)mUpRow->getLayout();
	//layout->setChildrenLimitType(BaseConstants::NONE);
	mDownRow = ViewFactory::createMenstruationRow(wMgr, mParagraph);
	//layout = (RowLayout *)mDownRow->getLayout();
	//layout->setChildrenLimitType(BaseConstants::NONE);
	layout->addChild(mLeftRow, -1, false, BaseConstants::NONE);
	layout->addChild(mRightRow, -1, false, BaseConstants::NONE);
	layout->addChild(mUpRow, -1, false, BaseConstants::NONE);
	layout->addChild(mDownRow, -1, false, BaseConstants::NONE);
	creatLine();
	layout->addChild(mMiddleLine, -1, false, BaseConstants::NONE);
	//复制数据
	/*setValue(otherMenstruation->getLeftTextWidget()->getTitle(), otherMenstruation->getRightTTextWidget()->getTitle(), 
		otherMenstruation->getUpTextWidget()->getTitle(), otherMenstruation->getDownTextWidget()->getTitle());*/
	addTextObjToRowContainer(false);
	//setUpAndDownValue();
	mContentData = TempleteFactory::createMenstruationParam(this);
	initTxtWidgetChooseId();
}
void MenstruationTemplete::initTxtWidgetChooseId()
{
	 mLeftTextWidget->setChooseObjId(mObjID);
	 mRightTextWidget->setChooseObjId(mObjID);
	 mUpTextWidget->setChooseObjId(mObjID);
	 mDownTextWidget->setChooseObjId(mObjID);
	 mContainerObj->setChooseObjId(mObjID);
}
MenstruationTemplete::~MenstruationTemplete()
{
	if (mLeftTextWidget){
		ViewObj* tempView;
		for (int i = 0; i < mLeftTextWidget->mStrObjList->size(); i++)
		{
			tempView = mLeftTextWidget->mStrObjList->at(i);
			delete tempView;
		}
		mLeftTextWidget->getChildren()->clear();
		delete mLeftTextWidget;
		mLeftTextWidget = NULL;
	}
	if (mRightTextWidget){
		ViewObj* tempView;
		for (int i = 0; i < mRightTextWidget->mStrObjList->size(); i++)
		{
			tempView = mRightTextWidget->mStrObjList->at(i);
			delete tempView;
		}
		mRightTextWidget->getChildren()->clear();
		delete mRightTextWidget;
		mRightTextWidget = NULL;
	}
	if (mUpTextWidget){
		ViewObj* tempView;
		for (int i = 0; i < mUpTextWidget->mStrObjList->size(); i++)
		{
			tempView = mUpTextWidget->mStrObjList->at(i);
			delete tempView;
		}
		mUpTextWidget->getChildren()->clear();
		delete mUpTextWidget;
		mUpTextWidget = NULL;
	}
	if (mDownTextWidget){
		ViewObj* tempView;
		for (int i = 0; i < mDownTextWidget->mStrObjList->size(); i++)
		{
			tempView = mDownTextWidget->mStrObjList->at(i);
			delete tempView;
		}
		mDownTextWidget->getChildren()->clear();
		delete mDownTextWidget;
		mDownTextWidget = NULL;
	}
	if (mFocusTxtData){
		delete mFocusTxtData;
		mFocusTxtData = NULL;
	}
	if (mMiddleLine){
		mMiddleLine = NULL;
	}
	if (mParagraph)
	{
		mParagraph->getChildren()->clear();
		delete mParagraph;
		mParagraph = NULL;
	}
	if (mLeftRow){
		QList<BaseContainer *> * children = mLeftRow->getChildren();
		if (children)
		{
			children->clear();
		}
		delete mLeftRow;
		mLeftRow = NULL;
	}
	if (mRightRow){
		QList<BaseContainer *> * children = mRightRow->getChildren();
		if (children)
		{
			children->clear();
		}
		delete mRightRow;
		mRightRow = NULL;
	}
	if (mUpRow){
		QList<BaseContainer *> * children = mUpRow->getChildren();
		if (children)
		{
			children->clear();
		}

		delete mUpRow;
		mUpRow = NULL;
	}
	if (mDownRow){
		QList<BaseContainer *> * children = mDownRow->getChildren();
		if (children)
		{
			children->clear();
		}
		delete mDownRow;
		mDownRow = NULL;
	}
	getChildren()->clear();
	if (mContainerObj){
		mContainerObj->getChildren()->clear();
		delete mContainerObj;
		mContainerObj = NULL;
	}
	
}

void MenstruationTemplete::addAllChildToView( bool needResetFlag){
	
}
void MenstruationTemplete::addAllChildToBufList(QList<BaseContainer *> * children, int start, int num, QList<BaseContainer *> * destViewList)
{
	QList<BaseContainer *> *mChildren =  getChildren();
	if (mChildren&&mChildren->size()>0)
	{
		int size = mChildren->size();
		if (size==1)
		{
			if (mChildren->indexOf(mContainerObj) >= 0)
			{
				destViewList->push_back(mContainerObj);
				return;
			}
			else
			{
				mChildren->clear();
			}
		}
		else
		{
			mChildren->clear();
		}
		
	
	}
}
void MenstruationTemplete:: creatLine(){
	//中线
	mMiddleLine = new ViewObj(mWriterMgr, ConstantsID::ContainerId::EXCEL_LINE);
	ObjDrawManager * drawMgr = mMiddleLine->getDrawMgr();
	BaseViewParam * viewParam = new BaseViewParam(mMiddleLine);
	viewParam->init(0, 0, 0, 0);
	viewParam->initDefaultWH(BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE, 0);
	DrawRect * rect = new DrawRect(mMiddleLine, UIConstants::FillRectType::ONLY_FILL, UIConstants::MyColor::BLACK, UIConstants::MyColor::BLACK, 1);
	drawMgr->initDraws(rect, NULL);
}
void MenstruationTemplete::initWidget(){
	if (!mFocusTxtData){
		StrContentData *focusTxtData = getFocusStrData();

		mFocusTxtData = new StrContentData(this, NULL, "", focusTxtData);
		mLeftTextWidget->setTxtDataWithValue(mFocusTxtData);
		mRightTextWidget->setTxtDataWithValue(mFocusTxtData);
		mUpTextWidget->setTxtDataWithValue(mFocusTxtData);
		mDownTextWidget->setTxtDataWithValue(mFocusTxtData);
	}
	setUpAndDownValue("aa","bb",false);
}
void MenstruationTemplete::setValue(QString left, QString right, QString up, QString down){
	int fontSize = mFocusTxtData->getPointSize();
	int changeSize = fontSize / mTxtRio;
	mLeftTextWidget->setTxtPointSize(changeSize);
	mRightTextWidget->setTxtPointSize(changeSize);
	mUpTextWidget->setTxtPointSize(changeSize);
	mDownTextWidget->setTxtPointSize(changeSize);
	mLeftTextWidget->setStr(left);
	mRightTextWidget->setStr(right);
	mUpTextWidget->setStr(up);
	mDownTextWidget->setStr(down);
	addTextObjToRowContainer();
	mFocusTxtData->setPointSize(fontSize);
}
void MenstruationTemplete::setUpAndDownValue(QString up, QString down, bool setContainerFocus)
{
	//QFont *  font = mFocusTxtData->getFont();
	int fontSize = mFocusTxtData->getPointSize();
	int changeSize = fontSize / mTxtRio;
	//font->setPointSize(changeSize);
	mUpTextWidget->setTxtPointSize(changeSize);
	mDownTextWidget->setTxtPointSize(changeSize);
	mUpTextWidget->setStr(up);
	mDownTextWidget->setStr(down);
	addTextObjToRowContainer(setContainerFocus);
	mFocusTxtData->setPointSize(fontSize);
}
void MenstruationTemplete::addTextObjToRowContainer(bool setContainerFocus)
{
	mLeftRow->getChildrenMgr()->delChildren(0,-1,true);
	mRightRow->getChildrenMgr()->delChildren(0, -1, true);
	mUpRow->getChildrenMgr()->delChildren(0, -1, true);
	mDownRow->getChildrenMgr()->delChildren(0, -1, true);
	mParagraph->getChildrenMgr()->delChildren(0,-1,true);
	addObjListToObjView(mLeftTextWidget->mStrObjList, mLeftRow);
	addObjListToObjView(mRightTextWidget->mStrObjList, mRightRow);

	addObjListToObjView(mUpTextWidget->mStrObjList, mUpRow);

	addObjListToObjView(mDownTextWidget->mStrObjList, mDownRow);
	QList<BaseContainer *> * containerChildrenList = mContainerObj->getChildren();
	if (containerChildrenList->indexOf(mLeftRow)<0)
	{
		mContainerObj->getChildrenMgr()->addChild(mLeftRow, -1, true);
	}
	if (containerChildrenList->indexOf(mRightRow)<0)
	{
		mContainerObj->getChildrenMgr()->addChild(mRightRow, -1, true);
	}
	if (containerChildrenList->indexOf(mUpRow)<0)
	{
		mContainerObj->getChildrenMgr()->addChild(mUpRow, -1, true);
	}
	if (containerChildrenList->indexOf(mDownRow)<0)
	{
		mContainerObj->getChildrenMgr()->addChild(mDownRow, -1, true);
	}
	setPostion(setContainerFocus);
}
void MenstruationTemplete::setPostion(bool setContainerFocus){
	//设置个元素的Y
	RowViewParam * upRowParam = (RowViewParam *)mUpRow->getViewParam();
	int totalH = upRowParam->getTotalH();
	BaseViewParam * middleLineParam = mMiddleLine->getViewParam();
	middleLineParam->setY(totalH);
	//设置线高
	int lineH = totalH / 5;
	middleLineParam->setSelfH(lineH);
	totalH += lineH;
	RowViewParam * downRowParam = (RowViewParam *)mDownRow->getViewParam();
	downRowParam->setY(totalH);
	totalH += downRowParam->getTotalH();
	int middlH = (totalH >> 1);
	RowViewParam * leftRowParam = (RowViewParam *)mLeftRow->getViewParam();
	int leftH = leftRowParam->getTotalH();
	leftRowParam->setY(middlH - leftH/2);
	RowViewParam * rightRowParam = (RowViewParam *)mRightRow->getViewParam();
	int rightH = rightRowParam->getTotalH();
	rightRowParam->setY(middlH - rightH / 2);
	//设置个元素的X
	int totalW = leftRowParam->getTotalW();
	middleLineParam->setX(totalW);
	//上下元素比较出 最宽的元素。 设置窄的元素居中
	int upW = upRowParam->getTotalW();
	int downW = downRowParam->getTotalW();
	if (upW>=downW){
		upRowParam->setX(totalW);
		downRowParam->setX(totalW + (upW - downW)/2);
		middleLineParam->setSelfW(upW);
		totalW += upW;
	}
	else{
		upRowParam->setX(totalW + (downW-upW) / 2);
		downRowParam->setX(totalW);
		middleLineParam->setSelfW(downW);
		totalW += downW;
	}
	rightRowParam->setX(totalW);
	int rightW = rightRowParam->getTotalW();
	totalW += rightW;
	BaseViewParam * containerParam = (BaseViewParam *)mContainerObj->getViewParam();
	containerParam->setSelfW(totalW);
	containerParam->setSelfH(totalH);
	//ViewObj * lastView = this->getNextView();
	if (setContainerFocus)
	{
		mContainerObj->setFocus(true);
	}
	refreshParentWH();
	/*if (lastView){
		lastView->setFocus(false, false);
	}*/
}
int MenstruationTemplete::getWidth()
{
	BaseViewParam * containerParam = (BaseViewParam *)mContainerObj->getViewParam();
	return containerParam->getSelfW();
}
void MenstruationTemplete::refreshParentWH()
{
	ViewObj*  viewParent = mContainerObj->getViewParent();
	
	if (viewParent){
		try{
			RowLayout * layout = (RowLayout *)viewParent->getLayout();
			RowViewParam * containerParentParam = (RowViewParam *)viewParent->getViewParam();
			//viewParent->getLayout()->replaceByType(BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
			//containerParentParam->refreshWByChildren(false);
			//containerParentParam->refreshHByChildren(false);
			//layout->judgeLessThanCircle(true);
			layout->openReplace();
			layout->judgeLessThanCircle(true);
			layout->judgeOverFlowCircle();
		
			//viewParent->getLayout()->judgeIdent();
			//viewParent->getLayout()->judgeMiner();
			//viewParent->getLayout()->refreshSelfAndParent(BaseConstants::ReplaceType::REPLACE_ONLY_SELF);
			int indexOfViewInParent = mContainerObj->getViewIndex();
			if (indexOfViewInParent==0)
			{
				QList<BaseContainer *>  * rowList = viewParent->getViewParent()->getChildren();
				int rowIndex = viewParent->getViewIndex();
				if (rowIndex>0)
				{
					ViewObj* preRow = (ViewObj*)rowList->at(rowIndex - 1);
					preRow->getLayout()->replaceChildren();
				}

			}
		}
		catch (...){

		}

	}
}
void MenstruationTemplete::addObjListToObjView(QList<ViewObj*> * objList, ViewObj* container){
	QListIterator<ViewObj*> i(*objList);
	ViewObj * tempt;
	while (i.hasNext())
	{
		tempt = i.next();
		container->getLayout()->addChild(tempt, -1, true, 0);
		mParagraph->getChildrenMgr()->addChild(tempt, 0, true);
	}
	container->getLayout()->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
	container->getViewParam()->refreshWByChildren(false);
}
int MenstruationTemplete::mousePress(int type, int x, int y, BaseContainer * obj)
{
	if (type == Qt::RightButton)
	{
		HandeRightMousePress();
		return 1;
	}
	else if (type == Qt::LeftButton)
	{
		mContainerObj->setFocusByX(x, BaseConstants::MousePressType::PRESS);
		return 0;
	}

	return 0;
}
int MenstruationTemplete::mouseRelease(int type, int x, int y, BaseContainer * obj)
{
	if (type == Qt::LeftButton)
	{
		mContainerObj->setFocusByX(x, BaseConstants::MousePressType::RELEASE);
		return 1;
	}
	return 0;
}
int MenstruationTemplete::mouseMove(int type, int x, int y, BaseContainer * obj)
{
	
	return 0;
}
int MenstruationTemplete::mouseDrag(int type, int x, int y, BaseContainer * obj)
{
	
	return 0;
}
int MenstruationTemplete::mouseDoublePress(int type, int x, int y, BaseContainer * obj)
{
	//int returnType = BaseConstants::NONE;
	
	if (type == Qt::LeftButton){
		FormulaDlg w;
		QMap<QString, QString> map;
		map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::MENSTRUATION);
		QString topLast = mUpTextWidget->getString();
		QString downLast = mDownTextWidget->getString();
		map.insert(TempleteTypeEnumeration::MENSTRUATION_TOP, topLast);
		map.insert(TempleteTypeEnumeration::MENSTRUATION_BOTTOM, downLast);
		w.InitData(map);
		//w.show();
		if (w.exec() == QDialog::Accepted)
		{
			int rowWidth = mContainerObj->getViewParent()->getViewParam()->getSelfW();
			QString topStr = map.value(TempleteTypeEnumeration::MENSTRUATION_TOP);
			if (topStr.length()==0)
			{
				topStr = " ";
			}
			QString downStr = map.value(TempleteTypeEnumeration::MENSTRUATION_BOTTOM);
			if (downStr.length() == 0)
			{
				downStr = " ";
			}
			//setUpAndDownValue(topStr,downStr);
			bool isNeedFresh = false;
			QList<ViewObj *> showContainer;
			if (topLast.compare(topStr) != 0)
			{
				mUpTextWidget->setStrAndGenatateStrObj(topStr);
				mUpTextWidget->getWillShowObjList(&showContainer);
				isNeedFresh = true;
				bool isCanPutInRow = checkHorizontalWidthThanRow(&showContainer, rowWidth);
				if (!isCanPutInRow)
				{
					mUpTextWidget->setStrAndGenatateStrObj(topLast, true);
				}
			}
			
			if (downLast.compare(downStr) != 0)
			{
				mDownTextWidget->setStrAndGenatateStrObj(downStr);
				showContainer.clear();
				mDownTextWidget->getWillShowObjList(&showContainer);
				isNeedFresh = true;
				bool isCanPutInRow = checkHorizontalWidthThanRow(&showContainer, rowWidth);
				if (!isCanPutInRow)
				{
					mDownTextWidget->setStrAndGenatateStrObj(downLast, true);
				}
			}
			
			if (isNeedFresh)
			{
				addTextObjToRowContainer();
				refreshParentWH();
				replaceNeedReplaceList(0);
			}
		}
		
	}
	return 1;
}
int MenstruationTemplete::mouseScroll(int angle, BaseContainer * obj)
{
	int returnType = BaseConstants::NONE;
	return returnType;
}
int MenstruationTemplete::keyPress(QString str, BaseContainer * obj)
{
	if (obj == mContainerObj)
	{
		ViewObj* focus = NULL;
		char dir = 0;
		getFocusObjAndDir(focus, dir);
		if (focus == obj)
		{
			OperateMgr *  opreateMgr = getOperateMgr();
			if (opreateMgr->isChoosed(this) || opreateMgr->getChoosedSize()>0)
			{
				return BaseConstants::NONE;
			}
			if (str.compare(StrConstants::MyKeyBoard::BACK_SPACE) == 0)
			{
				if (dir == BaseConstants::Direction::RIGHT)
				{
					/*BaseTemplete * parentTemplete = getTempleteParent();
					ViewObj *newtView = mContainerObj->getNextView();
					if (newtView)
					{
						newtView->setFocus(false, false);
					}*/
					//parentTemplete->delChild(this, true);
					/*ViewObj *preView = mContainerObj->getPreView();
					if (preView)
					{
						preView->setFocus(true, false);
					}
					else
					{
						mContainerObj->setFocus(false,false);
					}*/
					BaseTemplate::deleteSelf();
					return 1;
				}
			}
			else if (str.compare(StrConstants::MyKeyBoard::DEL) == 0)
			{
				if (dir == BaseConstants::Direction::LEFT)
				{
					/*ViewObj *newtView = mContainerObj->getNextView();
					if (newtView)
					{
						newtView->setFocus(false, false);
					}*/
					//getTempleteParent()->delChild(this, true);
					BaseTemplate::deleteSelf();
					return 1;
				}
				
			}
		}
		else
		{
			return  BaseConstants::NONE;
		}
	}
	

	//int returnType = BaseConstants::NONE;

	return  BaseConstants::NONE;
}
int MenstruationTemplete::keyRelease(QString str, BaseContainer * obj)
{

	int returnType = BaseConstants::NONE;
	return returnType;
}
void MenstruationTemplete::setFocusData(StrContentData* focusData)
{
	mFocusTxtData = focusData;
}
StrContentData*MenstruationTemplete::getSelfFocusData()
{
	return mFocusTxtData;
}
void MenstruationTemplete::setLeftTextWidget(TextWidget* textWidget)
{
	mLeftTextWidget = textWidget;
}
void MenstruationTemplete::setRightTTextWidget(TextWidget* textWidget)
{
	mRightTextWidget = textWidget;
}
void MenstruationTemplete::setUpTextWidget(TextWidget* textWidget)
{
	mUpTextWidget = textWidget;
}
void MenstruationTemplete::setDownTextWidget(TextWidget* textWidget)
{
	mDownTextWidget = textWidget;
}
TextWidget* MenstruationTemplete::getLeftTextWidget()
{
	return mLeftTextWidget;
}
TextWidget* MenstruationTemplete::getRightTTextWidget()
{
	return mRightTextWidget;
}
TextWidget* MenstruationTemplete::getUpTextWidget()
{
	return mUpTextWidget;
}
TextWidget* MenstruationTemplete::getDownTextWidget()
{
	return mDownTextWidget;
}
void MenstruationTemplete::HandeRightMousePress()
{
	QMap<QString, QString> map;
	map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::MENSTRUATION);
	QString topLast = mUpTextWidget->getString();
	QString downLast = mDownTextWidget->getString();
	map.insert(TempleteTypeEnumeration::MENSTRUATION_TOP, topLast);
	map.insert(TempleteTypeEnumeration::MENSTRUATION_BOTTOM, downLast);
	QMap<QString, QString> beforMap = map;
	bool isDelete = TempleteTypeEnumeration::showDialog(&map, true);
	if (isDelete){
		BaseTemplate::deleteSelf();
		return ;
	}
	if (map.contains(TempleteTypeEnumeration::PROPERTY_IS_CHANG))
	{
		if (map[TempleteTypeEnumeration::PROPERTY_IS_CHANG].compare(TempleteTypeEnumeration::STR_TRUE) == 0)
		{
			map.insert("topLast", topLast);
			map.insert("downLast", downLast);
			QList<ViewObj *>  topBeforObjListBefore;
			QList<ViewObj *> downBeforObjListBefore;
			ViewObj* beforFocus = NULL;
			char dir = 0;
			getFocusObjAndDir(beforFocus, dir);
			SetTxtWidgetElementAction::copyObjListToObjList(mUpTextWidget->mStrObjList, &topBeforObjListBefore, true);
			SetTxtWidgetElementAction::copyObjListToObjList(mDownTextWidget->mStrObjList, &downBeforObjListBefore, true);
			bool isNeedAdd = setTempleteProperty(map);
			if (isNeedAdd)
			{
				map.remove("topLast");
				map.remove("downLast");
				addSetTempletePropertyAction(beforMap, map, topBeforObjListBefore, downBeforObjListBefore,beforFocus,dir);
			}
		}
	}
}
bool MenstruationTemplete::refreshTxtParam(StrContentData* focusData)
{
	int rowWidth = ((RowViewParam*)mContainerObj->getViewParent()->getViewParam())->getInsideW();
	QList<ViewObj *> showContainer;
	//DrawTxtParam* focusTxtData = BaseTemplete::getFocusData();
	StrContentData* focusTxtData = focusData;
	//QFont *  font = focusTxtData->getFont();
	int fontSize = focusTxtData->getPointSize();
	int fontSizeBefor = mFocusTxtData->getPointSize();

	int changeSize = fontSize / mTxtRio;
	focusTxtData->setPointSize(changeSize);
	setChildTxtParam(focusTxtData);
	mUpTextWidget->getWillShowObjList(&showContainer);
	bool isCanSetTop = checkHorizontalWidthThanRow(&showContainer, rowWidth);
	showContainer.clear();
	mDownTextWidget->getWillShowObjList(&showContainer);
	bool isCanSetDown = checkHorizontalWidthThanRow(&showContainer, rowWidth);
	if ((!isCanSetTop) || (!isCanSetDown))
	{
		focusTxtData->setPointSize(fontSize);
		//QFont *  fontBefor = mFocusTxtData->getFont();
		mFocusTxtData->setPointSize(fontSizeBefor / mTxtRio);
		setChildTxtParam(mFocusTxtData);
		return false;
	}
	mFocusTxtData->refreshTxtParamOnSetType(focusTxtData);
	if (focusData->mOprateType != SetTxtType::UnderLineType)
	{
		mLeftTextWidget->getSelfFocusData()->refreshTxtParamOnSetType(focusTxtData);
		mRightTextWidget->getSelfFocusData()->refreshTxtParamOnSetType(focusTxtData);
	}
	mUpTextWidget->getSelfFocusData()->refreshTxtParamOnSetType(focusTxtData);
	mDownTextWidget->getSelfFocusData()->refreshTxtParamOnSetType(focusTxtData);
	mFocusTxtData->setPointSize(fontSize);
	mLeftTextWidget->setTxtPointSize(changeSize);
	mRightTextWidget->setTxtPointSize(changeSize);
	mUpTextWidget->setTxtPointSize(changeSize);
	mDownTextWidget->setTxtPointSize(changeSize);

	focusTxtData->setPointSize(fontSize);
	addTextObjToRowContainer(false);
	return true;
}
void MenstruationTemplete::setChildTxtParam(StrContentData* focusData)
{
	if (focusData->mOprateType != SetTxtType::UnderLineType)
	{
		setListObjTxtParam(mLeftTextWidget->mStrObjList, focusData);
		setListObjTxtParam(mRightTextWidget->mStrObjList, focusData);
	}
	setListObjTxtParam(mUpTextWidget->mStrObjList, focusData);
	setListObjTxtParam(mDownTextWidget->mStrObjList, focusData);
}
void MenstruationTemplete::setListObjTxtParam(QList<ViewObj*>* objList, StrContentData * focusTxtData)
{
	int size = objList->size();
	ViewObj * tempObj;
	for (int i = 0; i < size; i++)
	{
		tempObj = objList->at(i);
		refreshViewTxtParamOnly(focusTxtData, tempObj);
	}
}
BaseParagraph*MenstruationTemplete:: getParagraphTemplete()
{
	return mParagraph;
}
bool MenstruationTemplete::setTempleteProperty(QMap<QString, QString>& setMap)
{
	QString topStr = setMap.value(TempleteTypeEnumeration::MENSTRUATION_TOP);
	if (topStr.length() == 0)
	{
		topStr = " ";
		setMap[TempleteTypeEnumeration::MENSTRUATION_TOP] = topStr;
	}
	QString downStr = setMap.value(TempleteTypeEnumeration::MENSTRUATION_BOTTOM);
	if (downStr.length() == 0)
	{
		downStr = " ";
		setMap[TempleteTypeEnumeration::MENSTRUATION_BOTTOM] = downStr;
	}
	QString topLast = setMap.value("topLast");
	QString downLast = setMap.value("downLast");
	int rowWidth = ((RowViewParam*)mContainerObj->getViewParent()->getViewParam())->getInsideW();
	//setUpAndDownValue(topStr,downStr);
	bool isNeedFresh = false;
	QList<ViewObj *> showContainer;
	if (topLast.compare(topStr) != 0)
	{
		//setChildTxtParam(mUpTextWidget);
		mUpTextWidget->setStrAndGenatateStrObj(topStr);
		mUpTextWidget->getWillShowObjList(&showContainer);
		isNeedFresh = true;
		bool isCanPutInRow = checkHorizontalWidthThanRow(&showContainer, rowWidth);
		if (!isCanPutInRow)
		{
			//把上一步产生的对象删除掉，原始的产生的对象在动作记录栈里
			mUpTextWidget->setStrAndGenatateStrObj(topLast, true);
		}
	}
	if (downLast.compare(downStr) != 0)
	{
		//setChildTxtParam(mUpTextWidget);
		mDownTextWidget->setStrAndGenatateStrObj(downStr);
		showContainer.clear();
		mDownTextWidget->getWillShowObjList(&showContainer);
		isNeedFresh = true;
		bool isCanPutInRow = checkHorizontalWidthThanRow(&showContainer, rowWidth);
		if (!isCanPutInRow)
		{
			mDownTextWidget->setStrAndGenatateStrObj(downLast,true);
		}
	}
	if (isNeedFresh)
	{
		//refreshTxtParam();
		addTextObjToRowContainer();
		/*ViewObj * rowView = mContainerObj->getViewParent();
		if (rowView)
		{
			rowView->getLayout()->judgeIdent();
			rowView->getLayout()->judgeMiner();
			rowView->getLayout()->refreshSelfAndParent(BaseConstants::ReplaceType::REPLACE_ONLY_SELF);
		}*/
		refreshParentWH();
		replaceNeedReplaceList(1);
		return true;
	}
	return false;
}
void MenstruationTemplete::setPropertyOnActionRedoOrUndo(QList<ViewObj *>* topObjList, QList<ViewObj *> *downObjList)
{
	SetTxtWidgetElementAction::copyObjListToObjList(topObjList, mUpTextWidget->mStrObjList, true);
	mUpTextWidget->setOnlyStr(mUpTextWidget->getStrFromStrObjList());
	SetTxtWidgetElementAction::copyObjListToObjList(downObjList, mDownTextWidget->mStrObjList, true);
	mDownTextWidget->setOnlyStr(mDownTextWidget->getStrFromStrObjList());
	addTextObjToRowContainer(false);
	//refreshParentWH();
}
void MenstruationTemplete::addSetTempletePropertyAction(QMap<QString, QString> setBeforMap, QMap<QString, QString> setAfterMap, QList<ViewObj *>& topBeforObjList, QList<ViewObj *> &downBeforObjList, ViewObj* beforFocus, char focusDir)
{
	OperateMgr * operateMgr = getOperateMgr();
	operateMgr->createNewAction();
	SetMenstruationWidgetAction* setAction = new SetMenstruationWidgetAction(operateMgr->getCurStepMgr());
	ViewObj* focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus, dir);
	setAction->initFocus(beforFocus, focus, focusDir, dir);
	setAction->initAfterObjList(mUpTextWidget->mStrObjList, mDownTextWidget->mStrObjList);
	setAction->initBeforObjList(this, &topBeforObjList, &downBeforObjList);
	operateMgr->addToLocalAction(setAction);
}
bool MenstruationTemplete::setDrawTxtParam(StrContentData* focusData)
{
	if (BaseTemplate::isSetFlowType(focusData))
	{
		return true;
	}
	if (focusData->mOprateType == SetTxtType::FontSize)
	{
		return true;
	}
	char flowType = focusData->getFlowType();
	focusData->setFlowType(BaseConstants::NONE);
	if (focusData->mOprateType != NONE)
	{
		focusData->refreshTxtParamExceptSetType(mFocusTxtData, focusData->mOprateType);
	}

	bool isSuccess = refreshTxtParam(focusData);
	focusData->setFlowType(flowType);
	//mFocusTxtData->refreshTxtParamOnSetType(focusData);
	/*if (mLeftTextWidget)
	{
		mLeftTextWidget->setDrawTxtParam(mFocusTxtData);
	}
	if (mRightTextWidget)
	{
		mRightTextWidget->setDrawTxtParam(mFocusTxtData);
	}*/
	//if (mUpTextWidget)
	//{
	//	mUpTextWidget->setDrawTxtParam(mFocusTxtData);
	//}
	//if (mDownTextWidget)
	//{
	//	mDownTextWidget->setDrawTxtParam(mFocusTxtData);
	//}
	return isSuccess;
}
void MenstruationTemplete::getWillShowObjList(QList<ViewObj *> * container)
{
	container->append(mContainerObj);
}
bool MenstruationTemplete::checkHorizontalWidthThanRow(QList<ViewObj *> * container, int rowWidth)
{
	//int tempRowWidth = 0;
	if (mLeftTextWidget)
	{
		mLeftTextWidget->getWillShowObjList(container);
	}
	if (mRightTextWidget)
	{
		mRightTextWidget->getWillShowObjList(container);
	}
	ViewObj * tempObj;
	BaseViewParam * 	tempViewParam;
	int totalWidth = 0;
	int size = container->size();
	for (int i = 0; i < size; i++)
	{
		tempObj = container->at(i);
		tempViewParam = tempObj->getViewParam();
		totalWidth += tempViewParam->getSelfW();
	}
	if (totalWidth>rowWidth)
	{
		return false;
	}
	else
	{
		return true;
	}
}