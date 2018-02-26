#include "ChoiceWidgetTemplete.h"
//#include "TempleteInputManager.h"
//#include "BaseKeyboard.h"
#include "BaseMouse.h"
#include "ViewFactory.h"
#include "StrContentData.h"
//#include "ViewParamFactory.h"
#include "ViewObj.h"
//#include "BaseWinData.h"
#include "FlagParam.h"
#include "qdebug.h"
//#include "DrawManager.h"
#include "DrawStr.h"
//#include "DrawTxtParam.h"
#include "TextWidget.h"
#include "TempleteFactory.h"
#include "ViewObj.h"
//#include "InputManager.h"
#include "TempleteTypeEnumeration.h"
#include "StrConstants.h"
//#include "BaseOperateMgr.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "TextWidgetKeyboard.h"
#include "ChoiceWidgetTempleteParam.h"
#include "TempleteViewObj.h"
#include "BaseChildrenManager.h"
#include "BaseTempleteMouse.h"
#include "OperateMgr.h"
#include "SelectionPopup.h"
#include "SetChoiceWidgetAction.h"
#include "SetTxtWidgetElementAction.h"
//ChoiceWidgetTemplete::ChoiceWidgetTemplete(WriterManager * wMgr, BaseTemplate * templeteParent, int objId, QString codeId)
//: BaseTemplate(wMgr, ConstantsID::ClassId::CHOICE_WIDGET_TEMPLETE, objId, codeId)
//, mSignBeforeOptionObj(NULL)
//, mSignBehindOptionObj(NULL)
//, mSeparatorObjList(NULL)
//, mFocusTxtData(NULL)
//, mWidgetTitle(NULL)
//, mSignBeforSelect(NULL)
//, mSignBeforNotSelect(NULL)
//, mSignCommaObj(NULL)
//{
//	setTempleteParent(templeteParent);
//	isCreateFromLoad = false;
//	mWidgetType = MULTIPLE_CHOICE;
//	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
//	this->initChildrenMgr(childrenMgr);
//	//isShowTitle = true;
//	mIsShowed = false;
//	mTemptStatus = BaseConstants::ObjStatus::NEW_STATUS;
//	mSeparatorObjList = new QList<ViewObj *>();
//	StrContentData* tempFocusData = templeteParent->getFocusStrData();
//	mFocusTxtData = new StrContentData(this, tempFocusData->getString(), tempFocusData);
//	mOptionTempleteList = new  QList<TextWidget *>();
//	initWidget();
//	//generateTitleTempleteForType();
//}
ChoiceWidgetTemplete::ChoiceWidgetTemplete(WriterManager * wMgr, char widgetType, BaseTemplate * templeteParent, int objId, QString codeId, bool isFromLoad)
: BaseTemplate(wMgr, ConstantsID::ClassId::CHOICE_WIDGET_TEMPLETE, objId, codeId)
, mSignBeforeOptionObj(NULL)
, mSignBehindOptionObj(NULL)
, mSeparatorObjList(NULL)
, mOptionTempleteList(NULL)
, mFocusTxtData(NULL)
, mWidgetTitle(NULL)
, mSignBeforSelect(NULL)
, mSignBeforNotSelect(NULL)
, mSignCommaObj(NULL)
, mIsCanInputOnKeyboard(true)
{
	mIsCheckTitleShow = true;
	setTempleteParent(templeteParent);
	isCreateFromLoad = false;
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
	//isShowTitle = true;
	mWidgetType = widgetType ;
	mTemptStatus = BaseConstants::ObjStatus::NEW_STATUS;
	mSeparatorObjList = new QList<ViewObj *>();
	
	if (!isFromLoad)
	{
		mOptionTempleteList = new  QList<TextWidget *>();
		mFocusTxtData = new StrContentData(this, NULL, templeteParent->getFocusStrData()->getString(), templeteParent->getFocusStrData());
		initWidget();
	}
	else
	{
		mIsShowed = false;
	}
	//generateTitleTempleteForType();
}
ChoiceWidgetTemplete::ChoiceWidgetTemplete(WriterManager * wMgr, ChoiceWidgetTemplete* otherChoiceWidget) : BaseTemplate(wMgr, otherChoiceWidget, true)
, mSignBeforeOptionObj(NULL)
, mSignBehindOptionObj(NULL)
, mSignBeforSelect(NULL)
, mSignBeforNotSelect(NULL)
, mFocusTxtData(NULL)
, mSignCommaObj(NULL)
{
	mIsCheckTitleShow = true;
	isCreateFromLoad = false;
	mIsCanInputOnKeyboard = otherChoiceWidget->mIsCanInputOnKeyboard;
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
	mTitlePosition = otherChoiceWidget->mTitlePosition;
	StrContentData* temp = otherChoiceWidget->getSelfFocusData();
	mFocusTxtData = new StrContentData(this, NULL, temp->getString(), temp);
	//为了下面setSelectItem（）； 时，只设置内容，不排版
	mIsShowed = false;
	setSelectItem(otherChoiceWidget->mChooseList);
	mTemptStatus = otherChoiceWidget->mTemptStatus;
	mWidgetType = otherChoiceWidget->mWidgetType;
	mSeparatorObjList = new QList<ViewObj *>();
	mOptionTempleteList = new  QList<TextWidget *>();
	mSignBeforeOptionObj = new TempleteViewObj(wMgr, (TempleteViewObj*)otherChoiceWidget->mSignBeforeOptionObj);
	mSignBehindOptionObj = new TempleteViewObj(wMgr, (TempleteViewObj*)otherChoiceWidget->mSignBehindOptionObj);
	if (otherChoiceWidget->mWidgetTitle)
	{
		mWidgetTitle = new TextWidget(wMgr, otherChoiceWidget->mWidgetTitle);
		mWidgetTitle->setTempleteParent(this);
	}
	if (otherChoiceWidget->mSignBeforSelect)
	{
		mSignBeforSelect = new TextWidget(wMgr, otherChoiceWidget->mSignBeforSelect);
		mSignBeforSelect->setTempleteParent(this);
	}
	if (otherChoiceWidget->mSignBeforNotSelect)
	{
		mSignBeforNotSelect = new TextWidget(wMgr, otherChoiceWidget->mSignBeforNotSelect);
		mSignBeforNotSelect->setTempleteParent(this);
	}
	if (otherChoiceWidget->mSignCommaObj)
	{
		mSignCommaObj = new TempleteViewObj(wMgr, (TempleteViewObj*)otherChoiceWidget->mSignCommaObj);
		mSignCommaObj->setTempleteParent(this);
	}
	int size = otherChoiceWidget->mOptionTempleteList->size();
	for (int i = 0; i < size;i++)
	{
		TextWidget* tempOpion = new TextWidget(wMgr, otherChoiceWidget->mOptionTempleteList->at(i));
		tempOpion->setTempleteParent(this);
		mOptionTempleteList->push_back(tempOpion);
	}
	mContentData = TempleteFactory::createChoiceWidgetParam(this, NULL);
	addAllChildToChildrenMgr();
	//剪切时mChildren 的内容被删除，特此注释
	/*BaseChildrenManager * childMgr  = getChildrenMgr();
	if (childMgr)
	{
		childMgr->delChildren(0,-1,true);
	}

	mIsShowed = false;*/
}
ChoiceWidgetTemplete::~ChoiceWidgetTemplete()
{
	if (mFocusTxtData)
	{
		delete mFocusTxtData;
		mFocusTxtData = NULL;
	}
	if (mOptionTempleteList)
	{
		if (mWidgetType == HAVE_CHOICE)
		{

		}
		else
		{
			int sizeList = mOptionTempleteList->size();
			BaseTemplate * tempList;
			ViewObj * tempObj = NULL;
			for (int i = 0; i < sizeList; i++)
			{
				tempList = mOptionTempleteList->at(i);
				if (!isSelectedOnItem(i))
				{//删除没选中项的obj
					//tempList->setIsJudgeViewParenOnDecontruction(mIsJudgeViewParenOnDecontruction);
					delete tempList;
					tempList = NULL;
				}
			}
		}
		mOptionTempleteList->clear();
		delete mOptionTempleteList;
		mOptionTempleteList = NULL;
	}
	if (mWidgetTitle)
	{
		if (mTitlePosition == DISVISABLE)
		{
			//mWidgetTitle->setIsJudgeViewParenOnDecontruction(mIsJudgeViewParenOnDecontruction);
			delete mWidgetTitle;
			mWidgetTitle = NULL;
		}
		mWidgetTitle = NULL;
	}
	if (mWidgetType != HAVE_CHOICE)
	{
		
		if (mSignBeforSelect)
		{
			//mSignBeforSelect->setIsJudgeViewParenOnDecontruction(mIsJudgeViewParenOnDecontruction);
			delete mSignBeforSelect;
			mSignBeforSelect = NULL;
		}
		if (mSignBeforNotSelect)
		{
			//mSignBeforNotSelect->setIsJudgeViewParenOnDecontruction(mIsJudgeViewParenOnDecontruction);
			delete mSignBeforNotSelect;
			mSignBeforNotSelect = NULL;
		}
	}
	QList<BaseContainer *> * children = getChildren();
	if (mSignBeforeOptionObj){
		children->removeAll(mSignBeforeOptionObj);
		//mSignBeforeOptionObj->setIsJudgeViewParenOnDecontruction(mIsJudgeViewParenOnDecontruction);
		//mSignBeforeOptionObj->clearViewParent();
		delete mSignBeforeOptionObj;
		mSignBeforeOptionObj = NULL;
	}
	if (mSignBehindOptionObj){
		children->removeAll(mSignBehindOptionObj);
		//mSignBehindOptionObj->setIsJudgeViewParenOnDecontruction(mIsJudgeViewParenOnDecontruction);
		//mSignBehindOptionObj->clearViewParent();
		delete mSignBehindOptionObj;
		mSignBehindOptionObj = NULL;
	}
	if (mSignCommaObj){
		children->removeAll(mSignCommaObj);
		//mSignCommaObj->setIsJudgeViewParenOnDecontruction(mIsJudgeViewParenOnDecontruction);
		//mSignCommaObj->clearViewParent();
		delete mSignCommaObj;
		mSignCommaObj = NULL;
	}
	if (mSeparatorObjList){
		int size = mSeparatorObjList->size();
		ViewObj * tempObj = NULL;
		for (int i = 0; i < size;i++){
			tempObj = mSeparatorObjList->at(i);
			children->removeAll(tempObj);
			//tempObj->setIsJudgeViewParenOnDecontruction(mIsJudgeViewParenOnDecontruction);
			//tempObj->clearViewParent();
			delete tempObj;
			tempObj = NULL;
		}
		delete mSeparatorObjList;
		mSeparatorObjList = NULL;
	}
	/*if (mParentViewOfSignBeforView)
	{
		mParentViewOfSignBeforView = NULL;
	}
	if (mParentViewOfSignBehindView)
	{
		mParentViewOfSignBehindView = NULL;
	}*/
}
bool ChoiceWidgetTemplete::isSelectedOnItem(int itemIndex){
	int size = mChooseList.size();
	for (int i = 0; i < size; i++){
		if (mChooseList.at(i) == itemIndex){
			return true;
		}
	}
	return false;
}
void ChoiceWidgetTemplete::setTitle(TextWidget* title){
	if (mWidgetTitle == title){
		return ;
	}
	mWidgetTitle = title;
	deleteContentChild();
	addContentChild();
	
}

void ChoiceWidgetTemplete::setTitleOnNotShow(TextWidget* title)
{
	mWidgetTitle = title;
}
void ChoiceWidgetTemplete::setSignBeforSelectNotShow(TextWidget* title)
{
	mSignBeforSelect = title;
}
void ChoiceWidgetTemplete::setSignBeforNotSelectNotShow(TextWidget* title)
{
	mSignBeforNotSelect = title;
}
void ChoiceWidgetTemplete::setTitle(QString str)
{
	try{
		TextWidget * title = (TextWidget*)mWidgetTitle;
			title->setStr(str);
	}
	catch (...){
		TextWidget* textWidget = TempleteFactory::createChoiceTextWidgetTemplete(this,  false);
		textWidget->setTempleteStatus(mTemptStatus);
		setTitle(textWidget);
	}
	
}
void ChoiceWidgetTemplete::setTitleHintName(QString str)
{
	TextWidget * title = (TextWidget*)mWidgetTitle;
	title->setDefaultStr(str);
}
BaseTemplate* ChoiceWidgetTemplete::getTiltle(){
	return mWidgetTitle;
}
BaseTemplate* ChoiceWidgetTemplete::getSignBeforSelect()
{
	return mSignBeforSelect;
}
BaseTemplate* ChoiceWidgetTemplete::getSignBeforNotSelect()
{
	return mSignBeforNotSelect;
}
QList<TextWidget*>* ChoiceWidgetTemplete::getOptionList()
{
	return mOptionTempleteList;
}
/*
*如果选择项与以前的相同，返回ture
*/
void ChoiceWidgetTemplete::setOptions(QList<TextWidget*>* optionTempleteList)
{
	//bool isSame =true;
	if (optionTempleteList == mOptionTempleteList){
		return;
	}
	//if (isCreateFromLoad)
	{
	//	deleteLoadChild();
		//加载标题
	}
	//deleteOptions();
	//setOptionsDisVisable();
	//clearSeparateObj();
	deleteContentChild();
	if (optionTempleteList)
	{
		mOptionTempleteList = optionTempleteList;
	}
	//addOptionsToChileView();
	addContentChild();
}
void ChoiceWidgetTemplete::setOptionsOnNotShow(QList<TextWidget*>* optionTempleteList)
{
	if (optionTempleteList == mOptionTempleteList){
		return;
	}
	//deleteOptions();
	mOptionTempleteList = optionTempleteList;
}
void ChoiceWidgetTemplete::setOptions(QList<QString> strList)
{
	QList<TextWidget*>* optionTempleteList = new QList<TextWidget*>();
	int size = strList.length();
	for (int i = 0; i < size;i++){
		TextWidget * textWidget = TempleteFactory::createChoiceTextWidgetTemplete(this, false);
		textWidget->setTxtData(mFocusTxtData);
		textWidget->setTempleteStatus(mTemptStatus);
		textWidget->setStr(strList.at(i));
		optionTempleteList->push_back(textWidget);
	}
	mChooseList.clear();
	mChooseList.push_back(0);
	setOptions(optionTempleteList);
}
bool ChoiceWidgetTemplete::isSameOptions(QList<QString> strList)
{
	int optionSize = mOptionTempleteList->size();
	int strSize = strList.size();
	if (optionSize != strSize)
	{
		return false;
	}
	TextWidget * textWidget;
	for (int i = 0; i < strSize; i++)
	{
		textWidget = mOptionTempleteList->at(i);
		if (textWidget->mStr.compare(strList.at(i)) != 0)
		{
			return false;
		}
	}
	return true;
}
void ChoiceWidgetTemplete::setWedgetType(char type){
	mWidgetType = type;
}
char ChoiceWidgetTemplete::getWedgetType(){
	return mWidgetType;
}
void ChoiceWidgetTemplete::setTempleteStatus(int status){
	mTemptStatus = status;
	QList<BaseContainer *> * children = getChildren();
	if (!children)
	{
		return;
	}
	int chileSize = children->size() - 1;
	BaseContainer* tempContainer;
	for (int i = 1; i < chileSize; i++)
	{
		tempContainer = children->at(i);
		if (tempContainer->getClassId() == ConstantsID::ClassId::TXT_WIDGET)
		{
			TextWidget* textWidget = (TextWidget*)tempContainer;
			textWidget->setTempleteStatus(mTemptStatus);
		}
			
		
	}
}
int ChoiceWidgetTemplete::getTempleteStatus(){
	return mTemptStatus;
}
//void ChoiceWidgetTemplete::setIsShowTitle(bool titileShowable){
//	isShowTitle = titileShowable;
//}
//bool ChoiceWidgetTemplete::getIsShowTitle(){
//	return isShowTitle;
//}
void ChoiceWidgetTemplete::setTitleAndOptions(TextWidget* title, QList<TextWidget*>*  options){
	//if (mTemptStatus == BaseConstants::ObjStatus::EDIT_STATUS)
	{
		setTitleOnNotShow(title);
		setOptionsOnNotShow(options);
			 deleteContentChild();
			 addContentChild();
	 }
}
void ChoiceWidgetTemplete::setTitleAndOptions(QString title, QList<QString> strList){
	//if (mTemptStatus == BaseConstants::ObjStatus::EDIT_STATUS)
	{
		setTitle(title);
		mChooseList.clear();
		mChooseList.push_back(0);
		setOptions(strList);
	}
}
void ChoiceWidgetTemplete::setSelectItem(QList<int> chooseList){
	mChooseList = chooseList;
	if (!mIsShowed)
	{
		return;
	}
		deleteContentChild();
		addContentChild();
}
void ChoiceWidgetTemplete::addAllChildToBufList(QList<BaseContainer *> * children, int start, int num, QList<BaseContainer *> * destViewList)
{
	if (mIsShowed)
	{
		if (isCreateFromLoad)
		{
			BaseTemplate::addAllChildToBufList(children,start,num,destViewList);
		}
		else
		{
			/*mIsShowed = false;
			deSelfTempleteViewChildrenAndRefresh();
			addTempleteToView();*/
			//打开已经保存的模板
			addAllContentAndShowList(children,destViewList);
			isCreateFromLoad = false;
		}
	}
	else
	{
		/*addTempleteToView();*/
		addAllContentAndShowList(children, destViewList);
		mIsShowed = true;
	}
}
void ChoiceWidgetTemplete::generateViewObjOnNewStatus(){
	if (mTemptStatus != BaseConstants::ObjStatus::NEW_STATUS){
	}
	generatSignBeforeOptionViewObj();
	generatSignBehindOptionViewObj();
}
void ChoiceWidgetTemplete::addOptionsToChileView(){
	if (!mIsShowed)
	{
		return;
	}
	ViewObj *row = NULL;
	int indexView = 0;
	int indexTemplete = 0;
	if (mTitlePosition == DISVISABLE || mTitlePosition == RIGHT){
		indexTemplete = 1;
	}
	else if (mTitlePosition == LEFT){
		indexTemplete = 2;
	}
	/*else if (mTitlePosition == RIGHT){
		indexTemplete = 1;
		indexView = mSignBeforeOptionObj->getViewIndexFromParent() + 1;
		mSignBeforeOptionObj->setFocus(true);
		row = mSignBeforeOptionObj->getViewParent();
	}*/
	BaseChildrenManager * childrenMgr = getChildrenMgr();
	int optionListSize = mOptionTempleteList->size();
	//int optionSize;
	int choiceNumber = 0;
	int separateSignNumber = 0;
	ViewObj* separate=NULL;
	ViewObj * lastObj=	NULL;
	BaseTemplate * firstChoosedOptionObjList = NULL;
	BaseTemplate * lastChoosedOptionObjList = NULL;
	for (int i = 0; i < optionListSize; i++){
		if (!isSelectedOnItem(i)){
			continue;
		}
		choiceNumber++;
		if ((mWidgetType == RADIO)&&choiceNumber>1){
			break;
		}
		BaseTemplate * optionObjList = mOptionTempleteList->at(i);
		//addTempleteChild(optionObjList, indexTemplete, false, false);
		childrenMgr->addChild(optionObjList, indexTemplete,true);
		if (!firstChoosedOptionObjList)//记录第一个选中项
		{
			firstChoosedOptionObjList = optionObjList;
		}
		lastChoosedOptionObjList = optionObjList;
		//optionObjList->addWidgetToRow(row, indexView);
		optionObjList->checkStrIsAdaptStrListAndChildren(false);
		//optionObjList->addWidgetToRow(false);
		separate = getSeparatorObj(i);
		separateSignNumber = i;
		
		indexTemplete++;
		//addViewObjToChildShow(separate, indexTemplete, row, indexView);
		childrenMgr->addChild(separate, indexTemplete, true);
		indexTemplete++;
	}
	if (optionListSize>0 && separate)
	{
		//处理有无选择:加有字，加逗号
		if (mWidgetType == HAVE_CHOICE)
		{
			//ViewObj* fistChooseObj = firstChoosedOptionObjList->getFirstChildView();
			
			indexTemplete = firstChoosedOptionObjList->getTempIndex();
//<<<<<<< HEAD
//			addTempleteChildToViewParent(mSignHaveObj, row, indexTemplete, indexView, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF);
//=======
			//addTempleteChildToViewParent(mSignHaveObj, row, indexTemplete, indexView);
			childrenMgr->addChild(mSignBeforSelect, indexTemplete, true);
			mSignBeforSelect->checkStrIsAdaptStrListAndChildren(false);
//>>>>>>> 2d5eafa1b43c08cd967f9da5d56267cc2269503d
		}
		childrenMgr->delChild(separate,true);//删除顿号
	}
	if (choiceNumber == optionListSize)
	{
	
	}
	else
	{
		if (mWidgetType == HAVE_CHOICE)//加入无选择项
		{
			//
			if (choiceNumber>0){
				if (lastChoosedOptionObjList)
				{
				//	ViewObj* lastChildOflastChoosedOpion = lastChoosedOptionObjList->getLastChildView();
					
					indexTemplete = lastChoosedOptionObjList->getTempIndex() + 1;
					childrenMgr->addChild(mSignCommaObj, indexTemplete, true);
					//addTempleteChildToViewParent(mSignCommaObj, row, indexTemplete, indexView, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF);
				}
				else
				{
					childrenMgr->addChild(mSignCommaObj, indexTemplete, true);
					//addTempleteChildToViewParent(mSignCommaObj, row, indexTemplete, indexView, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF);
				}


				indexTemplete = mSignCommaObj->getTempIndex() + 1;
				
			}

			//<<<<<<< HEAD

			//		addTempleteChildToViewParent(mSignNotHaveObj, row, indexTemplete, indexView, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF);
			//		mSignNotHaveObj->setFocus(true,false);
			//=======
			childrenMgr->addChild(mSignBeforNotSelect, indexTemplete, true);
			mSignBeforNotSelect->checkStrIsAdaptStrListAndChildren(false);
			//addTempleteChildToViewParent(mSignNotHaveObj, row, indexTemplete, indexView);
			//mSignNotHaveObj->setFocus(true,false);
			//>>>>>>> 2d5eafa1b43c08cd967f9da5d56267cc2269503d
			separateSignNumber++;
			int notChoiceNumber = 0;
			//indexTemplete = mSignNotHaveObj->getTempleteIndexFromParent()+1;
			indexTemplete = mSignBeforNotSelect->getTempIndex() + 1;

			for (int i = 0; i < optionListSize; i++)
			{
				if (isSelectedOnItem(i)){
					continue;
				}
				notChoiceNumber++;
				BaseTemplate * optionObjList = mOptionTempleteList->at(i);
				childrenMgr->addChild(optionObjList, indexTemplete, true);
				optionObjList->checkStrIsAdaptStrListAndChildren(false);
				separate = getSeparatorObj(separateSignNumber);
				separateSignNumber++;
				
				indexTemplete++;
				//addViewObjToChildShow(separate, indexTemplete, row, indexView);
				childrenMgr->addChild(separate, indexTemplete, true);
				indexTemplete++;
			}
			if (notChoiceNumber>0)
			{
				childrenMgr->delChild(separate,true);//删除顿号
			}
		}
	}
	checkCanShowTitle(false,false);
	
//	addTxtLayerAndReplaceNeedReplaceList(NULL);
}
void ChoiceWidgetTemplete::addOptionsToChildrenAndShowList(QList<BaseContainer *> * children, QList<BaseContainer *> * destViewList)
{
	/*int indexTemplete = 0;
	if (mTitlePosition == DISVISABLE || mTitlePosition == RIGHT)
	{
		indexTemplete = 1;
	}
	else if (mTitlePosition == LEFT)
	{
		indexTemplete = 2;
	}*/
	int optionListSize = mOptionTempleteList->size();
	//int optionSize;
	int choiceNumber = 0;
	int separateSignNumber = 0;
	ViewObj* separate = NULL;
	ViewObj * lastObj = NULL;
	for (int i = 0; i < optionListSize; i++)
	{
		if (!isSelectedOnItem(i))
		{
			continue;
		}
		choiceNumber++;
		if ((mWidgetType == RADIO) && choiceNumber>1)
		{
			break;
		}
		BaseTemplate * optionObjList = mOptionTempleteList->at(i);
		if (mWidgetType == HAVE_CHOICE&&choiceNumber==1)
		{
			mSignBeforSelect->setTempleteParent(this);
			children->push_back(mSignBeforSelect);
			mSignBeforSelect->addAllChildToBufList(mSignBeforSelect->getChildren(), 0, -1, destViewList);
		}
		optionObjList->setTempleteParent(this);
		children->push_back(optionObjList);
		optionObjList->addAllChildToBufList(optionObjList->getChildren(),0,-1,destViewList);
		separate = getSeparatorObj(i);
		separateSignNumber = i;
		//addViewObjToChildShow(separate, indexTemplete, row, indexView);
		children->push_back(separate);
		destViewList->push_back(separate);
	}
	if (optionListSize>0 && separate)
	{
		children->removeLast();
		destViewList->removeLast(); //移除最后一个顿号
	}
	if (choiceNumber == optionListSize)
	{

	}
	else
	{
		if (mWidgetType == HAVE_CHOICE)//加入无选择项
		{
			mSignCommaObj->setTempleteParent(this);
			children->push_back(mSignCommaObj);
			destViewList->push_back(mSignCommaObj);
			mSignBeforNotSelect->setTempleteParent(this);
			children->push_back(mSignBeforNotSelect);
			mSignBeforNotSelect->addAllChildToBufList(mSignBeforNotSelect->getChildren(), 0, -1, destViewList);
			
			separateSignNumber++;
			int notChoiceNumber = 0;
		
			for (int i = 0; i < optionListSize; i++)
			{
				if (isSelectedOnItem(i))
				{
					continue;
				}
				notChoiceNumber++;
				BaseTemplate * optionObjList = mOptionTempleteList->at(i);
				optionObjList->setTempleteParent(this);
				children->push_back(optionObjList);
				optionObjList->addAllChildToBufList(optionObjList->getChildren(), 0, -1, destViewList);
				separate = getSeparatorObj(separateSignNumber);
				separateSignNumber++;
				children->push_back(separate);
				destViewList->push_back(separate);
		
			}
			if (notChoiceNumber>0)
			{
				children->removeLast();
				destViewList->removeLast(); //移除最后一个顿号
			}
		}
	}
}
void ChoiceWidgetTemplete::addAllContentAndShowList(QList<BaseContainer *> * children, QList<BaseContainer *> * destViewList)
{
	mSignBeforeOptionObj->setTempleteParent(this);
	children->push_back(mSignBeforeOptionObj);
	destViewList->push_back(mSignBeforeOptionObj);
	if (mTitlePosition == DISVISABLE)
	{
		//lastObj = mSignBeforeOptionObj;
		addOptionsToChildrenAndShowList(getChildren(), destViewList);
	}
	else if (mTitlePosition == LEFT)
	{
		mWidgetTitle->setTempleteParent(this);
		children->push_back(mWidgetTitle);
		mWidgetTitle->addAllChildToBufList(mWidgetTitle->getChildren(), 0, -1, destViewList);
		addOptionsToChildrenAndShowList(getChildren(), destViewList);
	}
	else if (mTitlePosition == RIGHT)
	{
		addOptionsToChildrenAndShowList(getChildren(), destViewList);
		mWidgetTitle->setTempleteParent(this);
		children->push_back(mWidgetTitle);
		mWidgetTitle->addAllChildToBufList(mWidgetTitle->getChildren(), 0, -1, destViewList);
	}
	mSignBehindOptionObj->setTempleteParent(this);
	children->push_back(mSignBehindOptionObj);
	destViewList->push_back(mSignBehindOptionObj);
}
void ChoiceWidgetTemplete::generateTitleTempleteForType()
{
	TextWidget * textWidget = TempleteFactory::createChoiceTextWidgetTemplete(this, false);
	textWidget->setTempleteStatus(BaseConstants::ObjStatus::EDIT_STATUS);
	mWidgetTitle = textWidget;
	if (mWidgetType == RADIO){
		textWidget->setStr("单选");
	}
	else if (mWidgetType == HAVE_CHOICE){
		textWidget->setStr("有无选择");
	}
	else if (mWidgetType == MULTIPLE_CHOICE){
		textWidget->setStr("多选");
	}
	//isShowTitle = true;
	mTitlePosition = LEFT;
}
void ChoiceWidgetTemplete::initWidget()
{
	if (!mFocusTxtData){
		//if (mTemptStatus == BaseConstants::ObjStatus::NONEDIT_STATUS){
		//	mFocusTxtData = ViewParamFactory::createFocusTxtData();
		//}
		//else

		/*{
			mFocusTxtData = BaseTemplate::getFocusData();
		}*/
	}
	generateViewObjOnNewStatus();
	generatSignHaveOrNotViewObj(false);
	generateTitleTempleteForType();
	addAllChildToChildrenMgr();
}
void ChoiceWidgetTemplete::addAllChildToChildrenMgr()
{
	BaseChildrenManager * childMgr = getChildrenMgr();
	childMgr->addChild(mSignBeforeOptionObj, -1, true);
	childMgr->addChild(mSignBehindOptionObj, -1, true);
	mIsShowed = true;
	addContentChild(false);
}
void ChoiceWidgetTemplete::generatSignBeforeOptionViewObj()
{
	mSignBeforeOptionObj = ViewFactory::createTemplateTxt(mWriterMgr, this, mWidgetSignBeforeOptionItem, mFocusTxtData);
	//mSignBeforeOptionObj->getOperateMgr()->setCanBeChoosenObjId(getObjId());
	//mSignBeforeOptionObj->getOperateMgr()->setCanAddObjParentId(getTempleteParent()->getObjId());
	setViewObjColor(mSignBeforeOptionObj, COLOR_OPTION_SIGN);
	mFocusTxtData->setFontColor(COLOR_NORMAL);
}
void ChoiceWidgetTemplete::generatSignBehindOptionViewObj()
{
	mSignBehindOptionObj = ViewFactory::createTemplateTxt(mWriterMgr, this, mWidgetSignBehindOptionItem, mFocusTxtData);
	//mSignBehindOptionObj->getOperateMgr()->setCanBeChoosenObjId(getObjId());
	//mSignBehindOptionObj->getOperateMgr()->setCanAddObjParentId(getTempleteParent()->getObjId());
	setViewObjColor(mSignBehindOptionObj, COLOR_OPTION_SIGN);
	mFocusTxtData->setFontColor(COLOR_NORMAL);
}
void ChoiceWidgetTemplete::generatSignHaveOrNotViewObj(bool isFormLoad)
{
	if (!isFormLoad){
		mSignBeforSelect = TempleteFactory::createChoiceTextWidgetTemplete(this, false);
		mSignBeforSelect->setStr("有");
		//mSignHaveObj = ViewFactory::createTempleteTxt(this, NULL, "有", mFocusTxtData);
		//mSignHaveObj->getOperateMgr()->setCanBeChoosenObjId(getObjId());
		//mSignHaveObj->getOperateMgr()->setCanAddObjParentId(getTempleteParent()->getObjId());
		mSignBeforNotSelect = TempleteFactory::createChoiceTextWidgetTemplete(this, false);
		mSignBeforNotSelect->setStr("无");
	}
	//mSignNotHaveObj = ViewFactory::createTempleteTxt(this, NULL, "无", mFocusTxtData);
	//mSignNotHaveObj->getOperateMgr()->setCanBeChoosenObjId(getObjId());
	//mSignNotHaveObj->getOperateMgr()->setCanAddObjParentId(getTempleteParent()->getObjId());
	mSignCommaObj = ViewFactory::createTemplateTxt(mWriterMgr, this, "，", mFocusTxtData);
	//mSignCommaObj->getOperateMgr()->setCanBeChoosenObjId(getObjId());
	//mSignCommaObj->getOperateMgr()->setCanAddObjParentId(getTempleteParent()->getObjId());
}
ViewObj* ChoiceWidgetTemplete::getSeparatorObj(int index)
{
	if (!mFocusTxtData)
	{
		mFocusTxtData = getFocusStrData();
	}
	int size = mSeparatorObjList->size();
	if (index >= size){
		for (int i = size ; i <= index;i++)
		{
			ViewObj * txt = ViewFactory::createTemplateTxt(mWriterMgr, this, mListSeparator, mFocusTxtData);
			mSeparatorObjList->push_back(txt);
		}
	}
	ViewObj * tempObj = mSeparatorObjList->at(index);

	deleteChild(tempObj, true);
	/*if (tempObj->getViewParent())
	{
		tempObj->getViewParent()->deletec(tempObj, true);
	}*/
	return tempObj;
}
//void ChoiceWidgetTemplete::setOptionsDisVisable()
//{
//	int size = mOptionTempleteList->size();
//	if (size>0){
//		BaseTemplate* temptObj;
//		for (int i = 0; i < size;i++){
//			temptObj = mOptionTempleteList->at(i);
//			//delChild(temptTemplete,true);
//			newDelChild(temptObj, true, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF, true, true);
//		}
//	}
//}
//void ChoiceWidgetTemplete::deleteOptions(){
//	int size = mOptionTempleteList->size();
//	if (size>0){
//		BaseTemplate* temptObj;
//		for (int i = 0; i < size; i++)
//		{
//			temptObj = mOptionTempleteList->at(i);
//			//delChild(temptTemplete, true);
//			newDelChild(temptObj, true, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF, true, true);
//		}
//	}
//	mOptionTempleteList->clear();
//	delete mOptionTempleteList;
//	mOptionTempleteList = NULL;
//}
//void ChoiceWidgetTemplete::clearSeparateObj(){
//	int size = mSeparatorObjList->size();
//	if (size>0){
//		ViewObj* temptObj;
//		for (int i = 0; i < size; i++){
//			temptObj = mSeparatorObjList->at(i);
//			//delChild(temptObj, true);
//			newDelChild(temptObj, true, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF, true, true);
//		}
//	}
//}

void ChoiceWidgetTemplete::setFocusColor(){
	//int size = mShowObjList->size();
	//ViewObj * obj;
	//for (int i = 0; i < size;i++){
	//	obj = mShowObjList->at(i);
	//	setViewObjColor(obj, COLOR_FOCUS);
	//}
	//setViewObjColor(mSignBeforeOptionObj, COLOR_FOCUS);
	//setViewObjColor(mSignBehindOptionObj, COLOR_FOCUS);
}
void ChoiceWidgetTemplete::setNormalColor(){
	//int size = mShowObjList->size();
	//ViewObj * obj;
	//for (int i = 0; i < size; i++){
	//	obj = mShowObjList->at(i);
	//	setViewObjColor(obj, COLOR_NORMAL);
	//}
	//setViewObjColor(mBehindTitleObj, COLOR_BEHIND_TITLE_SIGN);
	setViewObjColor(mSignBeforeOptionObj, COLOR_OPTION_SIGN);
	setViewObjColor(mSignBehindOptionObj, COLOR_OPTION_SIGN);
}
int ChoiceWidgetTemplete::mousePress(int type, int x, int y, BaseContainer  * obj)
{
	if (type == Qt::RightButton)
	{
		QMap<QString, QString> map;
		if (mWidgetType == RADIO)
		{
			map.insert(TempleteTypeEnumeration::TYPE,TempleteTypeEnumeration::RADIO);
			map.insert(TempleteTypeEnumeration::NAME,((TextWidget*)mWidgetTitle)->getString());
		}
		else if (mWidgetType == HAVE_CHOICE)
		{
			map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::HAVE_CHOICE);
			map.insert(TempleteTypeEnumeration::NAME, ((TextWidget*)mWidgetTitle)->getString());
			map.insert(TempleteTypeEnumeration::SIGN_BEFOR_SELECT, ((TextWidget*)mSignBeforSelect)->getString());
			map.insert(TempleteTypeEnumeration::SIGN_BEFOR_NOT_SELECT, ((TextWidget*)mSignBeforNotSelect)->getString());
		}
		else if (mWidgetType == MULTIPLE_CHOICE)
		{
			map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::MULTIPLE_CHOICE);
			map.insert(TempleteTypeEnumeration::NAME, ((TextWidget*)mWidgetTitle)->getString());
		}
		QString tempCanEdit;
		if (mIsCanInputOnKeyboard)
		{
			tempCanEdit = "false";
		}
		else
		{
			tempCanEdit = "true";
		}
		map.insert(TempleteTypeEnumeration::ISEDIT, tempCanEdit);
		//map.insert(TempleteTypeEnumeration::TITLE_POSTION, TempleteTypeEnumeration::TITLE_DIVISABLE);
		if (mTitlePosition == DISVISABLE)
		{
			map.insert(TempleteTypeEnumeration::TITLE_POSTION, TempleteTypeEnumeration::TITLE_DIVISABLE);
		}
		else if (mTitlePosition == LEFT)
		{
			map.insert(TempleteTypeEnumeration::TITLE_POSTION, TempleteTypeEnumeration::TITLE_LEFT);
		}
		else if (mTitlePosition == RIGHT)
		{
			map.insert(TempleteTypeEnumeration::TITLE_POSTION, TempleteTypeEnumeration::TITLE_RIGHT);
		}
		
		QString sendOptions = getOptions();
		map.insert(TempleteTypeEnumeration::OPTIONS,sendOptions);
		addCodeIdToMap(map);
		QMap<QString, QString> beforMap = map;
		bool isDelete = TempleteTypeEnumeration::showDialog(&map, true);
		if (isDelete){
			BaseTemplate::deleteSelf();
			return 1;
		}
		if (map.contains(TempleteTypeEnumeration::PROPERTY_IS_CHANG))
		{
			if (map[TempleteTypeEnumeration::PROPERTY_IS_CHANG].compare(TempleteTypeEnumeration::STR_TRUE) == 0)
			{
				QList<int>  beforChooseList = mChooseList;

				
				QList<ViewObj *>  beforDefaultTitleList;
				QList<ViewObj *>  beforTitleStrList;
				QList<TextWidget*>* beforOptionList = mOptionTempleteList;
				SetTxtWidgetElementAction::copyObjListToObjList(mWidgetTitle->mStrObjList, &beforTitleStrList, true);
				SetTxtWidgetElementAction::copyObjListToObjList(mWidgetTitle->mDefaultObjList, &beforDefaultTitleList, true);
				QList<ViewObj *>  beforSignHaveList;
				QList<ViewObj *>  beforSignNotHaveList;
				
				ViewObj* beforFocus = NULL;
				char dir = 0;
				getFocusObjAndDir(beforFocus,dir);
				if (mSignBeforSelect)
				{
					SetTxtWidgetElementAction::copyObjListToObjList(mSignBeforSelect->mStrObjList, &beforSignHaveList, true);
				}
				if (mSignBeforNotSelect)
				{
					SetTxtWidgetElementAction::copyObjListToObjList(mSignBeforNotSelect->mDefaultObjList, &beforSignNotHaveList, true);
				}

				bool isSetSuccess = setTempleteProperty(map);
				if (isSetSuccess)
				{
					addSetTempletePropertyAction(beforMap, map, beforChooseList, beforDefaultTitleList, beforTitleStrList, beforOptionList, beforSignHaveList, beforSignNotHaveList
						,beforFocus,dir);
				}
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}
	/*else if (type == Qt::LeftButton)
	{
		return showSelectDialog();
	}*/
	/*if (obj == mSignBeforeOptionObj || obj == mSignBehindOptionObj)
	{
		setFocusColor();
	}*/
	int returnType = BaseConstants::NONE;
	if (mInputMouse){
		return mInputMouse->mousePress(type, x, y, obj);
	} 
	return returnType;
}
int ChoiceWidgetTemplete::mouseRelease(int type, int x, int y, BaseContainer * obj)
{
	/*if (obj == mSignBeforeOptionObj || obj == mSignBehindOptionObj)
	{
		setNormalColor();
	}*/
	int returnType = BaseConstants::NONE;
	setNormalColor();
	if (mInputMouse){
		return mInputMouse->mouseRelease(type, x, y, obj);
	}
	return BaseConstants::NONE;
}
int ChoiceWidgetTemplete::mouseMove(int type, int x, int y, BaseContainer * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse){
		return mInputMouse->mouseMove(type, x, y, obj);
	}
	return returnType;
}
int ChoiceWidgetTemplete::mouseDrag(int type, int x, int y, BaseContainer * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse){
		return mInputMouse->mouseDrag(type, x, y, obj);
	}
	return returnType;
}
int ChoiceWidgetTemplete::mouseDoublePress(int type, int x, int y, BaseContainer * obj)
{
	if (type == Qt::LeftButton)
	{
		return showSelectDialog();
	}
	int returnType = BaseConstants::NONE;
	if (mInputMouse){
		return mInputMouse->mouseDoublePress(type, x, y, obj);
	}
	return returnType;
	/*int returnType = BaseConstants::NONE;
	if (mInputManager){
	return mInputManager->mouseDoublePress(type, x, y, obj);
	}
	return returnType;*/
}
int ChoiceWidgetTemplete::showSelectDialog()
{
	QString  sendType;
	if (mWidgetType == RADIO)
	{
		sendType = TempleteTypeEnumeration::RADIO;
	}
	else if (mWidgetType == HAVE_CHOICE)
	{
		sendType = TempleteTypeEnumeration::HAVE_CHOICE;
	}
	else if (mWidgetType == MULTIPLE_CHOICE)
	{
		sendType = TempleteTypeEnumeration::MULTIPLE_CHOICE;
	}
	QList<DataType*> sendOptions;
	QStringList words = getOptions().split(TempleteTypeEnumeration::SPLIT_STR);

	int optionListSize = mOptionTempleteList->size();

	for (int i = 0; i < optionListSize; i++)
	{
		DataType* tempDataType = new DataType();
		if (!isSelectedOnItem(i)){
			tempDataType->isChecked = false;
		}
		else{
			tempDataType->isChecked = true;
		}

		if (words[i].isEmpty() || words[i] == "") continue;

		tempDataType->name = words.at(i);
		sendOptions.push_back(tempDataType);
	}

	SelectionPopup::getSelection(sendType, sendOptions);
	QList<int>  chooseList;
	for (int i = 0; i < optionListSize; i++)
	{

		DataType* tempDataType = sendOptions.at(i);
		if (tempDataType->isChecked){
			chooseList.push_back(i);
		}
	}
	setSelectItem(chooseList);
	int optionSize = sendOptions.size();
	for (int i = 0; i < optionSize;i++)
	{
		delete sendOptions.at(i);
	}
	return 1;
}
bool ChoiceWidgetTemplete::deleteObjOnBackpressKey(ViewObj * focusObj, char dir)
{
	if (focusObj->getTempParent() == this)
	{
		ViewObj* preTempObj = focusObj->getPreView();;
		if (dir == BaseConstants::Direction::LEFT)
		{
			focusObj = preTempObj;
			preTempObj = focusObj->getPreView();
		}
		//QList<BaseContainer *>  deleteList;
		//deleteList.push_back(focusObj);
		//deleteViewObjOnShow(&deleteList, 0, deleteList.size(),true);
		//delChild(focusObj, true);
		deleteChild(focusObj,true,true);
		preTempObj->setFocus(true);
		checkCanShowTitle(true);
		return true;
	}
	return false;
}
bool ChoiceWidgetTemplete::deleteObjOnDeleteKey(ViewObj * focusObj, char dir)
{
	if (focusObj->getTempParent() == this)
	{
		ViewObj* nextTempObj = focusObj->getNextView();
		if (dir == BaseConstants::Direction::RIGHT)//如果是在右边，删除的是下一位所以需要移动一位
		{
			focusObj = nextTempObj;
			nextTempObj = focusObj->getNextView();
		}
		//QList<BaseContainer *>  deleteList;
		//deleteList.push_back(focusObj);
		//deleteViewObjOnShow(&deleteList, 0, deleteList.size(), true);
		delChild(focusObj, true);
		nextTempObj->setFocus(false, false);
		checkCanShowTitle(true);
		return true;
	}
	return false;
}
int ChoiceWidgetTemplete::mouseScroll(int angle, BaseContainer * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		return mInputMouse->mouseScroll(angle, obj);
	}
	return returnType;
}
int ChoiceWidgetTemplete::keyPress(QString str, BaseContainer * obj)
{
	//refreshChildrenIndex(0);
		if (isFocousObj(obj))
		{
			
		}
		else
		{
			return 0;
		}
		OperateMgr * operateMgr = getOperateMgr();
		if (operateMgr->isChoosed(this) || operateMgr->getChoosedSize()>0)
		{
			return BaseConstants::NONE;
		}
		ViewObj* selectObj = (ViewObj*)obj;
		ViewObj* focus = NULL;
		char dir = 0;
		getFocusObjAndDir(focus,dir);
	//在选中项 前后符号处，按删除或者delete  需要特殊处理
		if (str.compare(StrConstants::MyKeyBoard::BACK_SPACE) == 0)
	{
		if (TextWidgetKeyboard::isDeleteOrBackspaceKey(str))
		{
			
			if (operateMgr->isChoosed(this) || operateMgr->getChoosedSize()>0)
			{
				return 0;
			}
		}
		if (obj == mSignBeforeOptionObj)
		{
			if (dir == BaseConstants::Direction::RIGHT)
			{
				TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
				return 1;
			}
			return BaseConstants::NONE;
		}
		else if (obj == mSignBehindOptionObj)
		{
			if (dir == BaseConstants::Direction::RIGHT)
			{
				//deleteSelf();
				ViewObj* preView= mSignBehindOptionObj->getPreView();
				if (preView)
				{
					preView->setFocus(true, false);
				}
	
				return 1;
			}
			else
			{
				if (this->getChildren()->size()==2)
				{
					TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
					return 1;
				}
				else
				{
					/*if (canEditTempleteProperty())
					{
						TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
					}
					else*/
					if (mIsCanInputOnKeyboard)
					{
						deleteLastViewObj(mSignBehindOptionObj->getTempIndex());
					}
					else
					{
						TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
						return 1;
					}
				
					return 1;
				}
			}
			return BaseConstants::NONE;
		}
		else if (obj == mSignBeforeOptionObj->getNextView())
		{
			if (dir == BaseConstants::Direction::LEFT)
			{
				TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
				return 1;
			}
			else
			{
				/*if (canEditTempleteProperty())
				{
					TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
					return 1;
				}
				else*/
				if (mIsCanInputOnKeyboard)
				{
					ViewObj* tempObj = (ViewObj*)obj;
					if (deleteObjOnBackpressKey(tempObj, dir))
					{
						return 1;
					}
				}
				else
				{
					TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
					return 1;
				}
				
			}
		}
		else
		{
			/*if (canEditTempleteProperty())
			{
				TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
				return 1;
			}
			else*/
			if (mIsCanInputOnKeyboard)
			{
				//函数开始判断了，obj是view
				ViewObj* tempObj = (ViewObj*)obj;
				if (deleteObjOnBackpressKey(tempObj, dir))
				{
					return 1;
				}


			}
			else
			{
				TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
				return 1;
			}
			
		}
	}
		else if (str.compare(StrConstants::MyKeyBoard::DEL) == 0)
	{
		if (TextWidgetKeyboard::isDeleteOrBackspaceKey(str))
		{
			//BaseWinData * windata = getWinData();
			if (operateMgr->isChoosed(this) || operateMgr->getChoosedSize()>0)
			{
				return 0;
			}
		}
		if (obj == mSignBeforeOptionObj )
		{
			if (dir == BaseConstants::Direction::LEFT)
			{
				//deleteSelf();
				ViewObj* nextView = mSignBeforeOptionObj->getNextView();
				if (nextView)
				{
					nextView->setFocus(false, false);
				}
				return 1;
			}
			else
			{
				if (this->getChildren()->size() == 2)
				{
					TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
					return 1;
				}
				else
				{
					/*if (canEditTempleteProperty())
					{
						TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
						return 1;
					}
					else*/
					if (mIsCanInputOnKeyboard)
					{
						deleteNextViewObj(0);
						return 1;
					}
					else
					{
						TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
						return 1;
					}
					
				}
			}
			return BaseConstants::NONE;
		}
		else if (obj == mSignBehindOptionObj)
		{
			if (dir == BaseConstants::Direction::LEFT)
			{
				TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
				return 1;
			}
			else
			{
				return BaseConstants::NONE;
			}
		}
		else if (obj==mSignBehindOptionObj->getPreView())
		{
			if (dir == BaseConstants::Direction::RIGHT)
			{
				TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
				return 1;
			}
			else
			{
				/*if (canEditTempleteProperty())
				{
					TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
					return 1;
				}
				else*/
				if (mIsCanInputOnKeyboard)
				{
					ViewObj* tempObj = (ViewObj*)obj;
					if (deleteObjOnDeleteKey(tempObj, dir))
					{
						return 1;
					}
				}
				else
				{
					TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
					return 1;
				}
			}
		}
		else
		{
			/*if (canEditTempleteProperty())
			{
				TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
				return 1;
			}
			else*/
			if (mIsCanInputOnKeyboard)
			{
				//函数开始判断了，obj是view
				ViewObj* tempObj = (ViewObj*)obj;
				if (deleteObjOnDeleteKey(tempObj, dir))
				{
					return 1;
				}
			}
			else
			{
				TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
				return 1;
			}
		}
	}
	else
	{

		if (obj == mSignBeforeOptionObj)
		{
			if (TextWidgetKeyboard::isSystemKey(str))
			{
				return BaseConstants::NONE;
			}
			if (dir == BaseConstants::Direction::LEFT)
			{
				return 0;
				/*if (str.compare(BaseConstants::MyKeyBoard::ENTER) == 0)
				{
					return 0;
				}
				//BaseTemplete * preTemplete = this->getParentTempleteById(ConstantsID::ClassId::PARAGRAPH_TEMPLETE);
				BaseTemplete * tempParent = this->getTempleteParent();
				int idx = getTempleteIndexFromParent();
				if (tempParent)
				{
					int size = str.length();
					ViewObj * row = mSignBeforeOptionObj->getViewParent();
					int viewIndex = mSignBeforeOptionObj->getViewIndexFromParent();
					ViewObj * txt;
					for (int i = 0; i < size; i++)
					{
						DrawTxtParam * txtParam = DrawFactory::createDrawTxtParam(NULL, str.mid(i, 1), mFocusTxtData);
						 txt = ViewFactory::createTxt(tempParent, row, txtParam, ConstantsID::ContainerId::TXT);
						 tempParent->addTempleteChild(txt, idx, false, false);
						 row->addChild(txt, viewIndex);
						//BaseTemplete::addTempleteChildToViewParent(txt, row, idx, viewIndex);
						row = txt->getViewParent();
						viewIndex = txt->getViewIndexFromParent();
						viewIndex++;
						idx++;
					}
					txt->setFocus(true,false);
				}
				return 1;*/
			}
			else if (dir == BaseConstants::Direction::RIGHT)
			{
				/*if (canEditTempleteProperty())
				{
					TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
					return 1;
				}*/
				//BaseTemplete * preTemplete = this->getParentTempleteById(ConstantsID::ClassId::PARAGRAPH_TEMPLETE);
				if (str.compare(StrConstants::MyKeyBoard::ENTER) == 0)
				{
					return 1;
				}
				if (mIsCanInputOnKeyboard)
				{
					BaseTemplate * tempParent = this;
					int idx = mSignBeforeOptionObj->getTempIndex() + 1;
					mSignBeforeOptionObj->setFocus(true);
					if (tempParent)
					{
						int size = str.length();
						//ViewObj * row = mSignBeforeOptionObj->getViewParent();
						//int viewIndex = mSignBeforeOptionObj->getViewIndex()+1;
						QList<BaseContainer *> addList;
						ViewObj * txt;
						for (int i = 0; i < size; i++)
						{
							/*DrawTxtParam * txtParam = DrawFactory::createDrawTxtParam(NULL, str.mid(i, 1), mFocusTxtData);
							txt = ViewFactory::createTxt(tempParent, row, txtParam, ConstantsID::ContainerId::TXT);*/
							txt = ViewFactory::createTemplateTxt(mWriterMgr, tempParent, str.mid(i, 1), mFocusTxtData);
							//txt->getOperateMgr()->setCanBeChoosenObjId(getObjId());
							//txt->getOperateMgr()->setCanAddObjParentId(getTempleteParent()->getObjId());
							addList.push_back(txt);
							/*tempParent->addTempleteChild(txt, idx, false, false);
							row->addChild(txt, viewIndex, false);
							row = txt->getViewParent();
							viewIndex = txt->getViewIndex();
							viewIndex++;
							idx++;*/
						}
						addChildren(&addList, idx, true);
						txt->setFocus(true, false);
					}
				}
				
				return 1;
			}
		}
		else if (obj == mSignBehindOptionObj)
		{
			if (TextWidgetKeyboard::isSystemKey(str))
			{
				return BaseConstants::NONE;
			}
			if (dir == BaseConstants::Direction::RIGHT)
			{
				return 0;
				/*if (str.compare(BaseConstants::MyKeyBoard::ENTER) == 0)
				{
					return 0;
				}
				BaseTemplete * tempParent = this->getTempleteParent();
				int idx = getTempleteIndexFromParent()+1;
				if (tempParent)
				{
					int size = str.length();
					ViewObj * row = mSignBehindOptionObj->getViewParent();
					int viewIndex = mSignBehindOptionObj->getViewIndexFromParent() + 1;
					ViewObj * txt;
					for (int i = 0; i < size; i++)
					{
						DrawTxtParam * txtParam = DrawFactory::createDrawTxtParam(NULL, str.mid(i, 1), mFocusTxtData);
						txt = ViewFactory::createTxt(tempParent, row, txtParam, ConstantsID::ContainerId::TXT);
						//BaseTemplete::addTempleteChildToViewParent(txt, row, idx, viewIndex);
						tempParent->addTempleteChild(txt, idx, false, false);
						row->addChild(txt, viewIndex);
						row = txt->getViewParent();
						viewIndex = txt->getViewIndexFromParent();
						viewIndex++;
						idx++;
					}
					txt->setFocus(true, false);
				}
				return 1;*/
			}
			else if (dir == BaseConstants::Direction::LEFT)
			{
				/*if (canEditTempleteProperty())
				{
					TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
					return 1;
				}*/
				if (str.compare(StrConstants::MyKeyBoard::ENTER) == 0)
				{
					return 1;
				}
				if (mIsCanInputOnKeyboard)
				{
					BaseTemplate * tempParent = this;
					int idx = mSignBehindOptionObj->getTempIndex();
					mSignBehindOptionObj->setFocus(false);
					if (tempParent)
					{
						int size = str.length();
						//ViewObj * row = mSignBehindOptionObj->getViewParent();
						//	int viewIndex = mSignBehindOptionObj->getViewIndex();
						ViewObj * txt;
						QList<BaseContainer *> addList;
						for (int i = 0; i < size; i++)
						{
							/*DrawTxtParam * txtParam = DrawFactory::createDrawTxtParam(NULL, str.mid(i, 1), mFocusTxtData);
							txt = ViewFactory::createTxt(tempParent, row, txtParam, ConstantsID::ContainerId::TXT);*/
							txt = ViewFactory::createTemplateTxt(mWriterMgr, tempParent, str.mid(i, 1), mFocusTxtData);
							addList.push_back(txt);
							//txt->getOperateMgr()->setCanBeChoosenObjId(getObjId());
							//txt->getOperateMgr()->setCanAddObjParentId(getTempleteParent()->getObjId());
							//BaseTemplete::addTempleteChildToViewParent(txt, row, idx, viewIndex);
							/*tempParent->addTempleteChild(txt, idx, false, false);
							row->addChild(txt, viewIndex, false);
							row = txt->getViewParent();
							viewIndex = txt->getViewIndex();
							viewIndex++;
							idx++;*/
						}
						addChildren(&addList, idx, true);
						txt->setFocus(true, false);
					}
				}
				
				return 1;
			}
		}
		else
		{

			if (TextWidgetKeyboard::isSystemKey(str))
			{
				return BaseConstants::NONE;
			}
			/*if (canEditTempleteProperty())
			{
				TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
				return 1;
			}*/
			if (str.compare(StrConstants::MyKeyBoard::ENTER) == 0)
			{
				return 1;
			}

			ViewObj* tempObj = (ViewObj*)obj;
			if (tempObj->getTempParent() == this)
			{
				if (mIsCanInputOnKeyboard)
				{
					BaseTemplate * tempParent = this;
					int idx = tempObj->getTempIndex();
					if (tempParent)
					{
						int size = str.length();
						//ViewObj * row = tempObj->getViewParent();
						//int viewIndex = tempObj->getViewIndex();
						if (dir == BaseConstants::Direction::RIGHT){
							//viewIndex++;
							idx++;
							tempObj->setFocus(true);
						}
						else
						{
							tempObj->setFocus(false);
						}
						ViewObj * txt;
						QList<BaseContainer *> addList;
						for (int i = 0; i < size; i++)
						{
							/*DrawTxtParam * txtParam = DrawFactory::createDrawTxtParam(NULL, str.mid(i, 1), mFocusTxtData);
							txt = ViewFactory::createTxt(tempParent, row, txtParam, ConstantsID::ContainerId::TXT);*/
							txt = ViewFactory::createTemplateTxt(mWriterMgr, tempParent, str.mid(i, 1), mFocusTxtData);
							addList.push_back(txt);
							//txt->getOperateMgr()->setCanBeChoosenObjId(getObjId());
							//txt->getOperateMgr()->setCanAddObjParentId(getTempleteParent()->getObjId());
							//BaseTemplete::addTempleteChildToViewParent(txt, row, idx, viewIndex);
							/*tempParent->addTempleteChild(txt, idx, false, false);
							row->addChild(txt, viewIndex, false);
							row = txt->getViewParent();
							viewIndex = txt->getViewIndex();
							viewIndex++;
							idx++;*/
						}
						addChildren(&addList, idx, true);
						txt->setFocus(true);
					}
				}
				
				return 1;
			}
		}
	}
	
	int returnType = BaseConstants::NONE;
	if (mInputKeyboard){
		return mInputKeyboard->keyPress(str, obj);
	}
	return returnType;
}
int ChoiceWidgetTemplete::keyRelease(QString str, BaseContainer * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputKeyboard){
		return mInputKeyboard->keyRelease(str, obj);
	}
	return returnType;
}
bool ChoiceWidgetTemplete::setTitlePosition(char position)
{
	if (mTitlePosition == position)
	{
		return false;
	}
	mTitlePosition = position;
	return true;
}
char ChoiceWidgetTemplete::getTitlePosition()
{
	return mTitlePosition;
}
void ChoiceWidgetTemplete::setFocusData(StrContentData* focusData)
{
	mFocusTxtData = focusData;
}
StrContentData*ChoiceWidgetTemplete::getFocusData()
{
	return mFocusTxtData;
}
char ChoiceWidgetTemplete::getWidgetType()
{
	return mWidgetType;
}
void ChoiceWidgetTemplete::setWidgetType(int type)
{
	 mWidgetType = type;
}
void ChoiceWidgetTemplete::setTitlePosition(int type)
{
	mWidgetType = type;
}
const QList<int> ChoiceWidgetTemplete::getSelectList()
{
	return mChooseList;
}
void ChoiceWidgetTemplete::setSignBefor(ViewObj * signObj)
{
	mSignBeforeOptionObj = signObj;
}
void ChoiceWidgetTemplete::setSignBehind(ViewObj * signObj)
{
	mSignBehindOptionObj = signObj;
}
void ChoiceWidgetTemplete::setCreateFromLoad()
{
	isCreateFromLoad = true;
}
void ChoiceWidgetTemplete::deleteLoadChild()
{
	if (!isCreateFromLoad){
		return;
	}
	isCreateFromLoad = false;
	//
	QList<BaseContainer *> * children = getChildren();
	if (!children){
		return;
	}
	//children->removeFirst();
	//children->removeLast();
	deleteChild(1,children->size()-2,true);
	/*while (children->size() - 2>0)
	{
		delChild(mChildren->size() - 2,true);
	}*/
}
void ChoiceWidgetTemplete::deleteContentChild()
{
	QList<BaseContainer *> * children = getChildren();
	if (!children){
		return;
	}
	mSignBeforeOptionObj->setFocus(true);
	deleteLoadChild();
	mSignBeforeOptionObj->setFocus(true);
	deleteChild(1, children->size() - 2, true);
	QList<BaseContainer *> * childrenOfTitle = mWidgetTitle->getChildren();
	if (childrenOfTitle)
	{
		childrenOfTitle->clear();
	}
	/*while (children->size() - 2>0)
	{
		newDelChild(mChildren->size() - 2, true, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF, true, true);
	}*/
}
void ChoiceWidgetTemplete::addContentChild(bool isAddToShow)
{
	if (!mIsShowed)
	{
		return;
	}
	//ViewObj* lastObj = NULL;

	QList<BaseContainer *> * children = getChildren();
	BaseChildrenManager * childrenMgr= getChildrenMgr();
	if (mTitlePosition == DISVISABLE)
	{
		//lastObj = mSignBeforeOptionObj;
	}
	else
	{
		childrenMgr->addChild(mWidgetTitle,1, true);
		//children->insert(1, mWidgetTitle);
		//mWidgetTitle->setTempleteParent(this);
		//addTempleteChild(mWidgetTitle, 1, false, false);
		//mWidgetTitle->addWidgetToRow(mSignBeforeOptionObj->getViewParent(), mSignBeforeOptionObj->getViewIndexFromParent() + 1);
		mWidgetTitle->checkStrIsAdaptStrListAndChildren(false);
		//mWidgetTitle->addWidgetToRow();
	}
	//clearSeparateObj();
	addOptionsToChileView();
	refreshChildrenIndex(0);
	if (isAddToShow)
	{
		mSignBeforeOptionObj->setFocus(true);
		QList<BaseContainer *> addViewList;
		QList<BaseContainer *> * children = getChildren();
		int endIndex = children->size() - 1;
		BaseContainer* tempObj;
		for (int i = 1; i < endIndex; i++)
		{
			tempObj = children->at(i);
			addViewList.push_back(tempObj);
		}
		addViewObjOnShow(&addViewList, true);
		mSignBehindOptionObj->setFocus(true);
	}

}
QString ChoiceWidgetTemplete::getOptions()
{
	int size = mOptionTempleteList->size();
	BaseTemplate * temp;
	QString optionsStr;
	for (int i = 0; i < size;i++)
	{
		temp = mOptionTempleteList->at(i);
		try
		{
			TextWidget* textChiled = (TextWidget*)temp;
			optionsStr += textChiled->getString() + TempleteTypeEnumeration::SPLIT_STR;
		}
		catch (...)
		{
			continue;
		}
	}
	///int lastIndex = optionsStr.lastIndexOf(TempleteTypeEnumeration::SPLIT_STR);
	//optionsStr.remove(lastIndex, 2);
	return optionsStr;
}
void ChoiceWidgetTemplete::setIsShowed(bool isShowed)
{
	mIsShowed = isShowed;
}
void ChoiceWidgetTemplete::showTidy()
{
	if (mIsShowed)
	{
		if (!mIsShowTidy)
		{
			mIsShowTidy = true;
			mSignBeforeOptionObj->setFocus(false);
			/*mParentViewOfSignBeforView = mSignBeforeOptionObj->getViewParent();
			indexOfSignBeforViewInParentView = mSignBeforeOptionObj->getViewIndexFromParent();
			mParentViewOfSignBehindView = mSignBehindOptionObj->getViewParent();
			indexOfSignBehindViewInParentView = mSignBehindOptionObj->getViewIndexFromParent();*/
			deleteChild(mSignBeforeOptionObj->getTempIndex(),1,true,false);
			mSignBehindOptionObj->setFocus(false);
			deleteChild(mSignBehindOptionObj->getTempIndex(), 1, true, false);
			//delChild(mSignBeforeOptionObj,true);
			//delChild(mSignBehindOptionObj, true);
		}
		else{
			mIsShowTidy = false;
			/*ViewObj* firstView  = getFirstChildView();
			
			ViewObj* row = firstView->getViewParent();
			int viewIndex = firstView->getViewIndexFromParent();*/
			ViewObj* markView = this->getPreView();
			char direct = BaseConstants::Direction::RIGHT;
			
			if ((!markView)||markView->isEnter())
			//if (!markView)
			{
				markView = this->getFirstChildView();
				direct = BaseConstants::Direction::LEFT;
				if (!markView){
					markView = this->getNextView();
				}
			}
			//ViewObj* row = markView->getViewParent();
			//int viewIndex = markView->getViewIndex();
			if (direct == BaseConstants::Direction::RIGHT){
				markView->setFocus(true);
			}
			else{
				markView->setFocus(false);
			}
			addChild(mSignBeforeOptionObj, 0, true,true);
			//addTempleteChildToViewParent(mSignBeforeOptionObj, row, 0, viewIndex, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF);
			ViewObj* lastView = getLastChildView();
			lastView->setFocus(true);
			/*row = lastView->getViewParent();
			viewIndex = lastView->getViewIndex()+1;*/
			addChild(mSignBehindOptionObj, getChildrenMgr()->getChildrenSize(), true, true);
			//addTempleteChildToViewParent(mSignBehindOptionObj, row, -1, viewIndex, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF);
		/*	mParentViewOfSignBeforView = NULL;
			indexOfSignBeforViewInParentView = 0;
			mParentViewOfSignBehindView = NULL;
			indexOfSignBehindViewInParentView = 0;*/
		}
	}
}
void ChoiceWidgetTemplete::deleteNextViewObj(int currentTempleteIndex)
{
	ViewObj * nextView = ChoiceWidgetTemplete::findNextViewObj( currentTempleteIndex);
	if (nextView == mSignBehindOptionObj)
	{
		nextView->setFocus(false,false);
	}
	else
	{
		ViewObj* focus = nextView->getNextView();
		BaseTemplate *  templateParent = (BaseTemplate *)nextView->getTempParent();
		templateParent->deleteChild(nextView->getTempIndex(),1, true);
		QList<BaseContainer *> * tempChaildren = templateParent->getChildren();
		if (tempChaildren && (tempChaildren->size() == 0))
		{
			((BaseTemplate*)templateParent->getTempParent())->deleteChild(templateParent->getTempIndex(), 1, false);
		}
		checkCanShowTitle(false);
		focus->setFocus(false);
	}
}
ViewObj* ChoiceWidgetTemplete::findNextViewObj(int currentTempleteIndex)
{
	QList<BaseContainer *> * children = getChildren();
	int childSize = children->size();
	BaseContainer * temptObj;
	ViewObj* returnViewObj = NULL;
	for (int i = currentTempleteIndex + 1; i <childSize; i++)
	{
		temptObj = children->at(i);
		if (temptObj->isView())
		{
			returnViewObj = (ViewObj*)temptObj;
			break;
		}
		else
		{
			//temptTemplete = (BaseTemplete*)temptObj;
			if (temptObj->getChildren()->size() == 0)
			{
				continue;
			}
			else
			{
				returnViewObj = temptObj->getFirstChildView();
				break;
			}
		}
	}
	return returnViewObj;
}
void ChoiceWidgetTemplete::deleteLastViewObj(int currentTempleteIndex)
{
	ViewObj * preView = ChoiceWidgetTemplete::findPreViewObj(currentTempleteIndex);
	if (preView == mSignBeforeOptionObj)
	{
		preView->setFocus(true);
	}
	else
	{
		ViewObj* focus = preView->getPreView();
		BaseTemplate *  templateParent = (BaseTemplate *)preView->getTempParent();
		templateParent->deleteChild(preView->getTempIndex(), 1,true);
		QList<BaseContainer *> * tempChaildren = templateParent->getChildren();
		if (tempChaildren && (tempChaildren->size() == 0))
		{
			((BaseTemplate*)templateParent->getTempParent())->deleteChild(templateParent->getTempIndex(), 1, true);
		}
		checkCanShowTitle(true);
		focus->setFocus(true);
	}
}
ViewObj* ChoiceWidgetTemplete::findPreViewObj(int currentTempleteIndex)
{
	QList<BaseContainer *> * children = getChildren();
	BaseContainer * temptObj;
	ViewObj* returnViewObj = NULL;
	for (int i = currentTempleteIndex -1; i >=0; i--)
	{
		temptObj = children->at(i);
		if (temptObj->isView())
		{
			returnViewObj = (ViewObj*)temptObj;
			break;
		}
		else
		{
			//temptTemplete = (BaseTemplete*)temptObj;
			if (temptObj->getChildren()->size() == 0)
			{
				continue;
			}
			else
			{
				returnViewObj = temptObj->getLastChildView();
				break;
			}
		}
	}
	return returnViewObj;
}
bool ChoiceWidgetTemplete::setTempleteProperty(QMap<QString, QString>& setMap)
{
	isSetCodeSuccess = setCodeIdToTemplete(setMap);

	setCanEdit(setMap);
	QString type = setMap.value(TempleteTypeEnumeration::TYPE);
	QString name = setMap.value(TempleteTypeEnumeration::NAME);
	if (name.length() == 0)
	{
		name = " ";
		setMap[TempleteTypeEnumeration::NAME] = name;
	}
	QString options = setMap.value(TempleteTypeEnumeration::OPTIONS);
	QStringList words = options.split(TempleteTypeEnumeration::SPLIT_STR);
	for (int i = 0; i < words.length();)
	{
		QString tempStr = words.at(i);
		if (tempStr.trimmed().isEmpty())
		{
			words.removeAt(i);
		}
		else
		{
			i++;
		}
	}
	QString titlePostion = setMap.value(TempleteTypeEnumeration::TITLE_POSTION);
	 isSetTitlepostionSuccess = false;
	if (titlePostion == TempleteTypeEnumeration::TITLE_DIVISABLE)
	{
		isSetTitlepostionSuccess = setTitlePosition(DISVISABLE);
	}
	else if (titlePostion == TempleteTypeEnumeration::TITLE_LEFT)
	{
		isSetTitlepostionSuccess = setTitlePosition(LEFT);
	}
	else if (titlePostion == TempleteTypeEnumeration::TITLE_RIGHT)
	{
		isSetTitlepostionSuccess = setTitlePosition(RIGHT);
	}
	 isSetTitleSuccess = false;
	if (mWidgetTitle->getTitle().compare(name) == 0)
	{

	}
	else
	{
		try
		{
			TextWidget * title = (TextWidget*)mWidgetTitle;
			title->deleteChild(0,-1,false,false);
			title->setStrAndGenatateStrObj(name);
			isSetTitleSuccess = true;
		}
		catch (...)
		{

		}
	}
	 isSetSignBeforSuccess = false;
	 isSetSignNotBeforSuccess = false;
	if (mWidgetType == HAVE_CHOICE)
	{
		QString signBeforSelect = setMap.value(TempleteTypeEnumeration::SIGN_BEFOR_SELECT);
		if (signBeforSelect.size()==0)
		{
			signBeforSelect = "有";
			setMap[TempleteTypeEnumeration::SIGN_BEFOR_SELECT] = signBeforSelect;
		}
		QString signBeforNotSelect = setMap.value(TempleteTypeEnumeration::SIGN_BEFOR_NOT_SELECT);
		if (signBeforNotSelect.size() == 0)
		{
			signBeforNotSelect = "无";
			setMap[TempleteTypeEnumeration::SIGN_BEFOR_SELECT] = signBeforNotSelect;
		}
		if (signBeforSelect.compare(mSignBeforSelect->getString()) != 0)
		{
			isSetSignBeforSuccess = true;
			mSignBeforSelect->setStrAndGenatateStrObj(signBeforSelect);
		}
		if (signBeforNotSelect.compare(mSignBeforNotSelect->getString()) != 0)
		{
			isSetSignNotBeforSuccess = true;
			mSignBeforNotSelect->setStrAndGenatateStrObj(signBeforNotSelect);
		}
	}
	isSameOptionSuccess = isSameOptions(words);
	if ((!isSetCodeSuccess) && (!isSetTitlepostionSuccess) && (!isSetTitleSuccess) && (!isSetSignNotBeforSuccess) && (!isSetSignBeforSuccess) && (isSameOptionSuccess))
	{
		return false;
	}
	//如果选项为0，需要显示提示
	if (words.size() == 0 && titlePostion == TempleteTypeEnumeration::TITLE_DIVISABLE)
	{
		setTitlePosition(LEFT);
	}
	if (isSameOptionSuccess)
	{
		deleteContentChild();
		addContentChild();
	}
	else
	{
		mChooseList.clear();
		mChooseList.push_back(0);
		setOptions(words);
	}
	
	return true;
}
void ChoiceWidgetTemplete::addSetTempletePropertyAction(QMap<QString, QString>& beforMap, QMap<QString, QString>& map, QList<int> beforChooseList
	, QList<ViewObj *> & beforDefaultTitleList, QList<ViewObj *>& beforTitleStrList, QList<TextWidget*>* beforeOptionTempleteList
	, QList<ViewObj *> & beforHaveList, QList<ViewObj *> & beforNotHaveList, ViewObj* beforFocus, char focusDir)
{
	OperateMgr * operateMgr = getOperateMgr();
	operateMgr->createNewAction();
	SetChoiceWidgetAction* setAction = new SetChoiceWidgetAction(operateMgr->getCurStepMgr());
	ViewObj* focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus, dir);
	setAction->initFocus(beforFocus, focus, focusDir, dir);
	setAction->initAction(this, beforMap, map, beforChooseList,mChooseList);

	
	setAction->initAfterObjList(mWidgetTitle->mDefaultObjList, (isSetTitleSuccess ? mWidgetTitle->mStrObjList : mWidgetTitle->mStrObjList), (isSameOptionSuccess ? NULL : mOptionTempleteList));
	setAction->initBeforObjList(mWidgetTitle->mDefaultObjList, (isSetTitleSuccess ? &beforTitleStrList : &beforTitleStrList), (isSameOptionSuccess ? NULL : beforeOptionTempleteList));
	if (mSignBeforSelect&&mSignBeforNotSelect)
	{
		setAction->initHaveAndNotHaveObjList((isSetSignBeforSuccess ? &beforHaveList : &beforHaveList), (isSetSignNotBeforSuccess ? &beforNotHaveList : &beforNotHaveList), (isSetSignBeforSuccess ? mSignBeforSelect->mStrObjList : mSignBeforSelect->mStrObjList), (isSetSignNotBeforSuccess ? mSignBeforNotSelect->mStrObjList : mSignBeforNotSelect->mStrObjList));
	}
	//setAction->initHaveAndNotHaveObjList((isSetSignBeforSuccess ? &beforHaveList : &beforHaveList), (isSetSignNotBeforSuccess ? &beforNotHaveList : &beforNotHaveList), (isSetSignBeforSuccess ? mSignBeforSelect->mStrObjList : mSignBeforSelect->mStrObjList), (isSetSignNotBeforSuccess ? mSignBeforNotSelect->mStrObjList : mSignBeforNotSelect->mStrObjList));
	operateMgr->addToLocalAction(setAction);
}
void ChoiceWidgetTemplete::setPropertyOnActionRedoOrUndo(QMap<QString, QString>& setMap, QList<ViewObj *> * strObjList, QList<ViewObj *> * defaultObjList,
	QList<ViewObj *> * strHaveList, QList<ViewObj *> * strNotHaveList,
	QList<TextWidget*>* optionTempleteList, QList<int> cChooseList)
{
	setCodeIdToTemplete(setMap);
	QString type = setMap.value(TempleteTypeEnumeration::TYPE);
	QString name = setMap.value(TempleteTypeEnumeration::NAME);
	QString options = setMap.value(TempleteTypeEnumeration::OPTIONS);
	QString titlePostion = setMap.value(TempleteTypeEnumeration::TITLE_POSTION);
	QStringList words = options.split(TempleteTypeEnumeration::SPLIT_STR);
	setCanEdit(setMap);
	if (titlePostion == TempleteTypeEnumeration::TITLE_DIVISABLE)
	{
		setTitlePosition(DISVISABLE);
	}
	else if (titlePostion == TempleteTypeEnumeration::TITLE_LEFT)
	{
		setTitlePosition(LEFT);
	}
	else if (titlePostion == TempleteTypeEnumeration::TITLE_RIGHT)
	{
		setTitlePosition(RIGHT);
	}
	SetTxtWidgetElementAction::copyObjListToObjList(strObjList, mWidgetTitle->mStrObjList, true);
	SetTxtWidgetElementAction::copyObjListToObjList(defaultObjList, mWidgetTitle->mDefaultObjList, true);
	mWidgetTitle->setOnlyStr(name);
	if (mWidgetType == HAVE_CHOICE)
	{
		QString signBeforSelect = setMap.value(TempleteTypeEnumeration::SIGN_BEFOR_SELECT);
		QString signBeforNotSelect = setMap.value(TempleteTypeEnumeration::SIGN_BEFOR_NOT_SELECT);
		if (signBeforSelect.compare(mSignBeforSelect->getString()) != 0){
			SetTxtWidgetElementAction::copyObjListToObjList(strHaveList, mSignBeforSelect->mStrObjList, true);
			mSignBeforSelect->setOnlyStr(signBeforSelect);
		}
		if (signBeforNotSelect.compare(mSignBeforNotSelect->getString()) != 0){
			SetTxtWidgetElementAction::copyObjListToObjList(strNotHaveList, mSignBeforNotSelect->mStrObjList, true);
			mSignBeforNotSelect->setOnlyStr(signBeforNotSelect);
		}
	}
	//如果选项为0，需要显示提示
	if (words.size() == 0 && titlePostion == TempleteTypeEnumeration::TITLE_DIVISABLE)
	{
		setTitlePosition(LEFT);
	}
	mChooseList = cChooseList;
		setOptions(optionTempleteList);
}
void ChoiceWidgetTemplete::setCanEdit(QMap<QString, QString>& setMap)
{
	QString canEdit = setMap.value(TempleteTypeEnumeration::ISEDIT);
	QVariant tempValue = canEdit;
	bool isOnlyChoose = tempValue.toBool();
	if (isOnlyChoose)
	{
		mIsCanInputOnKeyboard = false;
	}
	else
	{
		mIsCanInputOnKeyboard = true;
	}
	
}
bool ChoiceWidgetTemplete::setDrawTxtParam(StrContentData* focusData)
{
		if (BaseTemplate::isSetFlowType(focusData))
		{
			return true;
		}
		StrContentData* preData = new StrContentData(NULL, NULL);
		preData->refreshTxtParamBySourceParam(mFocusTxtData, false);
		preData->setOperateType(focusData->mOprateType);
		mFocusTxtData->refreshTxtParamOnSetType(focusData);
		//ViewObj*	viewParent = mSignBeforeOptionObj->getViewParent();
		
		//StrContentData * txtParam = (StrContentData *)mSignBeforeOptionObj->getContentData();
		//txtParam->refreshTxtParamOnSetType(mFocusTxtData);
		//txtParam->refreshTxtWH();
		bool isSuccess = true;
		isSuccess = mSignBeforeOptionObj->setDrawTxtParam(focusData);
		//ViewObj*	viewParent1 = mSignBehindOptionObj->getViewParent();
		
		//StrContentData * txtParam1 = (StrContentData *)mSignBehindOptionObj->getContentData();
		//txtParam1->refreshTxtParamOnSetType(mFocusTxtData);
		//txtParam1->refreshTxtWH();
		if (isSuccess)
		{
			isSuccess = mSignBehindOptionObj->setDrawTxtParam(focusData);
		}
		
		if (mWidgetType == HAVE_CHOICE)
		{
			if (mSignBeforSelect)
			{
				if (isSuccess)
				{
					isSuccess = mSignBeforSelect->setDrawTxtParam(mFocusTxtData);
				}
	
			}
			if (mSignBeforNotSelect)
			{
				if (isSuccess)
				{
					isSuccess = mSignBeforNotSelect->setDrawTxtParam(mFocusTxtData);
				}
				
			}
			if (mSignCommaObj)
			{
				/*ViewObj*	viewParent = mSignCommaObj->getViewParent();
				if (viewParent)
					viewParent->setPageRepaint();*/

				//StrContentData * txtParam = (StrContentData *)mSignCommaObj->getContentData();
				//txtParam->refreshTxtParamOnSetType(mFocusTxtData);
				//txtParam->refreshTxtWH();
				if (isSuccess)
				{
					isSuccess = mSignCommaObj->setDrawTxtParam(mFocusTxtData);
				}
				
			}
		}
		if (isSuccess)
		{
			isSuccess = mWidgetTitle->setDrawTxtParam(mFocusTxtData);
		}
	
		
		if (isSuccess)
		{
			int optionSize = mOptionTempleteList->size();
			TextWidget* option;
			int successIndex = 0;
			for (successIndex; successIndex < optionSize; successIndex++)
			{
				option = mOptionTempleteList->at(successIndex);
				if (!option->setDrawTxtParam(mFocusTxtData))
				{
					isSuccess = false;
					break;
				}
			}
			if (!isSuccess)
			{
				for (int i = 0; i <= successIndex; i++)
				{
					option = mOptionTempleteList->at(i);
					option->setDrawTxtParam(preData);
				}
			}
		}
		if (isSuccess)
		{
			QList<BaseContainer *> * children = getChildren();
			int showSize = children->size();
			ViewObj* showChild;
			int successIndex = 0;
			for (successIndex; successIndex < showSize; successIndex++)
			{
				showChild = (ViewObj*)children->at(successIndex);
				//if (showChild->isTxt())
				//{
				//	ViewObj*	viewParent = showChild->getViewParent();
				//	/*if (viewParent)
				//		viewParent->setPageRepaint();*/
				//	StrContentData * txtParam = (StrContentData *)showChild->getContentData();
				//	txtParam->refreshTxtParamOnSetType(mFocusTxtData);
				//	txtParam->refreshTxtWH();
				//}
				//else
				//{
				//	showChild->setDrawTxtParam(focusData);
				//}
				if (!showChild->setDrawTxtParam(focusData))
				{
					isSuccess = false;
					break;
				}
			}
			if (!isSuccess)
			{
				for (int i = 0; i <= successIndex; i++)
				{
					showChild = (ViewObj*)children->at(successIndex);
					showChild->setDrawTxtParam(preData);
				}
			}
		}
		if (isSuccess)
		{
			delete preData;
			preData = NULL;
			return true;
		}
		else
		{
			mFocusTxtData->refreshTxtParamOnSetType(preData);
			setBackTxtParam(preData);
			delete preData;
			preData = NULL;
			return false;
		}
		
}
void ChoiceWidgetTemplete::setBackTxtParam(StrContentData* focusData)
{
	bool isSuccess = true;
	isSuccess = mSignBeforeOptionObj->setDrawTxtParam(focusData);
	//ViewObj*	viewParent1 = mSignBehindOptionObj->getViewParent();

	//StrContentData * txtParam1 = (StrContentData *)mSignBehindOptionObj->getContentData();
	//txtParam1->refreshTxtParamOnSetType(mFocusTxtData);
	//txtParam1->refreshTxtWH();
	if (isSuccess)
	{
		isSuccess = mSignBehindOptionObj->setDrawTxtParam(focusData);
	}

	if (mWidgetType == HAVE_CHOICE)
	{
		if (mSignBeforSelect)
		{
			if (isSuccess)
			{
				isSuccess = mSignBeforSelect->setDrawTxtParam(mFocusTxtData);
			}

		}
		if (mSignBeforNotSelect)
		{
			if (isSuccess)
			{
				isSuccess = mSignBeforNotSelect->setDrawTxtParam(mFocusTxtData);
			}

		}
		if (mSignCommaObj)
		{
			/*ViewObj*	viewParent = mSignCommaObj->getViewParent();
			if (viewParent)
			viewParent->setPageRepaint();*/

			//StrContentData * txtParam = (StrContentData *)mSignCommaObj->getContentData();
			//txtParam->refreshTxtParamOnSetType(mFocusTxtData);
			//txtParam->refreshTxtWH();
			if (isSuccess)
			{
				isSuccess = mSignCommaObj->setDrawTxtParam(mFocusTxtData);
			}

		}
	}
	if (isSuccess)
	{
		isSuccess = mWidgetTitle->setDrawTxtParam(mFocusTxtData);
	}


	if (isSuccess)
	{
		int optionSize = mOptionTempleteList->size();
		TextWidget* option;
		int successIndex = 0;
		for (successIndex; successIndex < optionSize; successIndex++)
		{
			option = mOptionTempleteList->at(successIndex);
			if (!option->setDrawTxtParam(mFocusTxtData))
			{
				isSuccess = false;
				break;
			}
		}
	}
	if (isSuccess)
	{
		QList<BaseContainer *> * children = getChildren();
		int showSize = children->size();
		ViewObj* showChild;
		int successIndex = 0;
		for (successIndex; successIndex < showSize; successIndex++)
		{
			showChild = (ViewObj*)children->at(successIndex);
			//if (showChild->isTxt())
			//{
			//	ViewObj*	viewParent = showChild->getViewParent();
			//	/*if (viewParent)
			//		viewParent->setPageRepaint();*/
			//	StrContentData * txtParam = (StrContentData *)showChild->getContentData();
			//	txtParam->refreshTxtParamOnSetType(mFocusTxtData);
			//	txtParam->refreshTxtWH();
			//}
			//else
			//{
			//	showChild->setDrawTxtParam(focusData);
			//}
			if (!showChild->setDrawTxtParam(focusData))
			{
				isSuccess = false;
				break;
			}
		}
	}
}
StrContentData*ChoiceWidgetTemplete::getSelfFocusData()
{
	return mFocusTxtData;
}
void ChoiceWidgetTemplete::getWillShowObjList(QList<ViewObj *> * container)
{
	container->append(mSignBeforeOptionObj);
	if (mWidgetType == HAVE_CHOICE)
	{
		container->append(mSignCommaObj);
		mSignBeforSelect->getWillShowObjList(container);
		mSignBeforNotSelect->getWillShowObjList(container);
	}
	if (mTitlePosition != DISVISABLE)
	{
		mWidgetTitle->getWillShowObjList(container);
	}

	int size = mOptionTempleteList->size();
	TextWidget * tempWidget;
	for (int i = 0; i < size; i++)
	{
		tempWidget = mOptionTempleteList->at(i);
		tempWidget->getWillShowObjList(container);
	}
	container->append(mSignBehindOptionObj);
}
void ChoiceWidgetTemplete::checkCanShowTitle(bool isSetFocusFront, bool isAddToShow)
{
	if (!mIsCheckTitleShow)
	{
		return;
	}
	BaseChildrenManager * childrenMgr = getChildrenMgr();
	if (childrenMgr->getChildrenSize() <= 2)
	{
		mSignBeforeOptionObj->setFocus(true);
		mWidgetTitle->checkStrIsAdaptStrListAndChildren(false);
		if (isAddToShow)
		{
			addChild(mWidgetTitle, 1, true, true);
		}
		else
		{
			childrenMgr->addChild(mWidgetTitle, 1, true);
		}
		//addTempleteChild(mWidgetTitle, 1, false, false);
		//mWidgetTitle->addWidgetToRow(mSignBeforeOptionObj->getViewParent(), mSignBeforeOptionObj->getViewIndexFromParent() + 1);
		
		if (isSetFocusFront)
		{
			mSignBeforeOptionObj->setFocus(true, false);
		}
		//mSignBehindOptionObj->setFocus(true, false);
	}
}
bool ChoiceWidgetTemplete::getCanInputOnKeyBoard()
{
	return mIsCanInputOnKeyboard;
}
void ChoiceWidgetTemplete::setCanInputOnKeyBoard(bool canInputOnKeyBoard)
{
	mIsCanInputOnKeyboard = canInputOnKeyBoard;
}
void ChoiceWidgetTemplete::setIsCheckTitleShow(bool isCheckTitle)
{
	mIsCheckTitleShow = isCheckTitle;
}