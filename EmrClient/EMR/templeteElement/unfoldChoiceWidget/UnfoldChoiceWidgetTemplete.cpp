#include "UnfoldChoiceWidgetTemplete.h"
//#include "TempleteInputManager.h"
#include "ChoiceItemTextWidget.h"
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
#include "UnfoldChoiceWidgetTempleteParam.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "TextWidgetKeyboard.h"
#include "TempleteViewObj.h"
#include "BaseChildrenManager.h"
#include "BaseTempleteMouse.h"
#include "OperateMgr.h"
#include "SelectionPopup.h"
#include "SetUnfoldChoiceWidgetAction.h"
#include "SetTxtWidgetElementAction.h"
#include "RowLayout.h"
UnfoldChoiceWidgetTemplete::UnfoldChoiceWidgetTemplete(WriterManager * wMgr, char widgetType, BaseTemplate * templeteParent, int objId, QString codeId, bool isFromLoad)
: BaseTemplate(wMgr, ConstantsID::ClassId::UNFOLD_CHOICE_WIDGET_TEMPLETE, objId, codeId)
, mOptionTempleteList(NULL)
, mFocusTxtData(NULL)
, mWidgetTitle(NULL)
, mColonViewObj(NULL)
{
	mPaddingLeft = 0;
	mChoiceType = widgetType;
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
	
	if (!isFromLoad)
	{
		mOptionTempleteList = new  QList<ChoiceItemTextWidget *>();
		mFocusTxtData = new StrContentData(this, NULL, templeteParent->getFocusStrData()->getString(), templeteParent->getFocusStrData());
		mColonViewObj = ViewFactory::createTemplateTxt(mWriterMgr, this, mColonStr, mFocusTxtData);
		initWidget();
	}
	//generateTitleTempleteForType();
}
UnfoldChoiceWidgetTemplete::UnfoldChoiceWidgetTemplete(WriterManager * wMgr, UnfoldChoiceWidgetTemplete* otherChoiceWidget) : BaseTemplate(wMgr, otherChoiceWidget, false)
, mOptionTempleteList(NULL)
, mFocusTxtData(NULL)
, mWidgetTitle(NULL)
, mColonViewObj(NULL)
{
	mPaddingLeft = otherChoiceWidget->mPaddingLeft;
	mChoiceType = otherChoiceWidget->getChoiceType();
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
	mFocusTxtData = new StrContentData(this, NULL, otherChoiceWidget->getSelfFocusData()->getString(), otherChoiceWidget->getSelfFocusData());
	mColonViewObj = ViewFactory::createTemplateTxt(mWriterMgr, this, mColonStr, mFocusTxtData);
	setTitlePosition(otherChoiceWidget->getTitlePosition());
	createTitle(otherChoiceWidget->getTitleStr());
	QList<ChoiceItemTextWidget*>* optionList = otherChoiceWidget->getOptionList();
	int optionSize = optionList->size();
	ChoiceItemTextWidget* tempOption;
	QList<QString> strList;
	QList<int> paddLeftList;
	for (int i = 0; i < optionSize; i++)
	{
		tempOption = optionList->at(i);
		paddLeftList.push_back( tempOption->getPaddingLeft());
		strList.push_back(tempOption->getTitle());
	}
	mChooseList = otherChoiceWidget->mChooseList;
	setOptionsOnCreate(strList, paddLeftList);
	UnfoldChoiceWidgetTempleteParam * choiceWidgetParam = TempleteFactory::createUnfoldChoiceWidgetParam(this, NULL);
	this->initContentData(choiceWidgetParam);
	addContentChild(false);

}
UnfoldChoiceWidgetTemplete::~UnfoldChoiceWidgetTemplete()
{
	QList<BaseContainer *> *  childrenList = getChildren();
	if (mWidgetTitle)
	{
		childrenList->removeAll(mWidgetTitle);
		delete mWidgetTitle;
		mWidgetTitle = NULL;
	}
	if (mColonViewObj)
	{
		childrenList->removeAll(mColonViewObj);
		delete mColonViewObj;
		mColonViewObj = NULL;
	}
	
	if (mOptionTempleteList)
	{
		QList<ChoiceItemTextWidget*>* optionList = mOptionTempleteList;
		int optionSize = optionList->size();
		ChoiceItemTextWidget* tempOption;
		for (int i = 0; i < optionSize; i++)
		{
			tempOption = optionList->at(i);
			childrenList->removeAll(tempOption);
			delete tempOption;
		}
		delete mOptionTempleteList;
		mOptionTempleteList = NULL;
	}
	if (mFocusTxtData)
	{
		delete mFocusTxtData;
		mFocusTxtData = NULL;
	}
}
void UnfoldChoiceWidgetTemplete::initWidget()
{
	if (!mFocusTxtData)
	{
		
	}
	
	generateTitleTempleteForType();
	addAllChildToChildrenMgr();
}
void UnfoldChoiceWidgetTemplete::generateTitleTempleteForType()
{
	mWidgetTitle = new TextWidget(this->getWriterMgr(),ConstantsID::ContainerId:: UNFOLD_CHOICE_TITLE_TEMPLETE_OBJ, BaseConstants::NONE, true);
	mWidgetTitle->setTempleteParent(this);
	mWidgetTitle->setChooseObjId(getObjID());
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(mWidgetTitle);
	mWidgetTitle->initChildrenMgr(childrenMgr);
	mWidgetTitle->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
	if (mChoiceType == RADIO){
		mWidgetTitle->setStr("单选");
	}
	else if (mChoiceType == MULTIPLE_CHOICE){
		mWidgetTitle->setStr("多选");
	}
	//isShowTitle = true;
	mTitlePosition = LEFT;
}
void UnfoldChoiceWidgetTemplete::createTitle(QString& title)
{
	mWidgetTitle = new TextWidget(this->getWriterMgr(), ConstantsID::ContainerId::UNFOLD_CHOICE_TITLE_TEMPLETE_OBJ, BaseConstants::NONE, true);
	mWidgetTitle->setTempleteParent(this);
	mWidgetTitle->setChooseObjId(getObjID());
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(mWidgetTitle);
	mWidgetTitle->initChildrenMgr(childrenMgr);
	mWidgetTitle->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
	mWidgetTitle->setTxtDataWithValue(mFocusTxtData,false);
	mWidgetTitle->setStr(title);
}
void UnfoldChoiceWidgetTemplete::addAllChildToChildrenMgr()
{
	
	addContentChild(false);
}
void UnfoldChoiceWidgetTemplete::addContentChild(bool isAddToShow)
{
	BaseChildrenManager * childrenMgr = getChildrenMgr();
	if (mTitlePosition == DISVISABLE)
	{
		//lastObj = mSignBeforeOptionObj;
	}
	else
	{
		childrenMgr->addChild(mWidgetTitle, 0, true);
		mWidgetTitle->checkStrIsAdaptStrListAndChildren(false);
		if (!(mTitlePosition == RIGHT))
		{
			childrenMgr->addChild(mColonViewObj, 1, true);
		}
	}
	//clearSeparateObj();
	addOptionsToChildrenManager();
	checkNeedToAddTitleWidget();
	refreshChildrenIndex(0);
	if (isAddToShow)
	{
		QList<BaseContainer *> addViewList;
		QList<BaseContainer *> * children = getChildren();
		int endIndex = children->size();
		BaseContainer* tempObj;
		for (int i = 0; i < endIndex; i++)
		{
			tempObj = children->at(i);
			addViewList.push_back(tempObj);
		}

		addViewObjOnShow(&addViewList, true);
		ViewObj * lastView = getLastChildView();
		if (lastView)
		{
			lastView->setFocus(true, true, BaseConstants::MousePressType::PRESS_AND_RELEASE);
		}
	}
}
void UnfoldChoiceWidgetTemplete::addOptionsToChildrenManager()
{
	int indexTemplete = 0;
	if (mTitlePosition == DISVISABLE || mTitlePosition == RIGHT){
		indexTemplete = 0;
	}
	else if (mTitlePosition == LEFT){
		indexTemplete = 2;
	}
	checkRadioChoiceCheckedList();
	if (!mOptionTempleteList)
	{
		return;
	}
	BaseChildrenManager * childrenMgr = getChildrenMgr();
	int optionListSize = mOptionTempleteList->size();
	//int optionSize;
	int choiceNumber = 0;
	int separateSignNumber = 0;
	BaseTemplate * firstChoosedOptionObjList = NULL;
	BaseTemplate * lastChoosedOptionObjList = NULL;
	for (int i = 0; i < optionListSize; i++)
	{
		ChoiceItemTextWidget * optionObjList = mOptionTempleteList->at(i);
		if (isSelectedOnItem(i))
		{
			optionObjList->setChecked(true);
		}
		else
		{
			optionObjList->setChecked(false);
		}
		choiceNumber++;
		childrenMgr->addChild(optionObjList, indexTemplete, true);

		indexTemplete++;
	}
}
bool UnfoldChoiceWidgetTemplete::isSelectedOnItem(int itemIndex){
	int size = mChooseList.size();
	for (int i = 0; i < size; i++){
		if (mChooseList.at(i) == itemIndex){
			return true;
		}
	}
	return false;
}
void UnfoldChoiceWidgetTemplete::checkRadioChoiceCheckedList()
{
	int size = mChooseList.size();
	int choiceNumber = 0;
	if ((mChoiceType == RADIO) && size>2)
	{
		for (int i = 1; i < mChooseList.size();)
		{
			mChooseList.removeAt(i);
		}
	}
}
void UnfoldChoiceWidgetTemplete::checkNeedToAddTitleWidget()
{
	if (mTitlePosition==DISVISABLE)
	{
		if (mOptionTempleteList->size() == 0)
		{
			BaseChildrenManager * childrenMgr = getChildrenMgr();
			childrenMgr->addChild(mWidgetTitle, 0, true);
			childrenMgr->delChild(mColonViewObj, true);
		}	
	}
	else if (mTitlePosition == RIGHT)
	{
		BaseChildrenManager * childrenMgr = getChildrenMgr();
		childrenMgr->delChild(mColonViewObj, true);
	}
}
void UnfoldChoiceWidgetTemplete::deleteContentChild()
{
	QList<BaseContainer *> * children = getChildren();
	if (!children)
	{
		return;
	}
	ViewObj * firstView = getFirstView();
	if (firstView)
	{
		firstView->setFocus(BaseConstants::Direction::RIGHT, false,BaseConstants::MousePressType::PRESS_AND_RELEASE);
		deleteChild(0, -1, true);
	}
	else
	{
		qDebug() << "UnfoldChoiceWidgetTemplete deleteContentChild firstView = NULL ";
	}
}
/*
*如果选择项与以前的相同，返回ture
*/
void UnfoldChoiceWidgetTemplete::setOptions(QList<ChoiceItemTextWidget*>* optionTempleteList)
{
	//bool isSame =true;

	if (optionTempleteList == NULL || optionTempleteList == mOptionTempleteList)
	{
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
void UnfoldChoiceWidgetTemplete::setOptionsOnNotShow(QList<ChoiceItemTextWidget*>* optionTempleteList)
{
	if (optionTempleteList == mOptionTempleteList)
	{
		return;
	}
	//deleteOptions();
	mOptionTempleteList = optionTempleteList;
}
QList<ChoiceItemTextWidget*>* UnfoldChoiceWidgetTemplete::createOptionsByStrList(QList<QString>& strList, QList<int>& paddingLeft)
{
		QList<ChoiceItemTextWidget*>* optionTempleteList = new QList<ChoiceItemTextWidget*>();
	for (int i = strList.size() - 1; i >= 0; i--)
	{
		if (strList.at(i).size() <= 0)
		{
			strList.removeAt(i);
		}
	}
	int size = strList.length();
	int temptPaddingLeft = 0;
	for (int i = 0; i < size; i++)
	{
		temptPaddingLeft = paddingLeft.at(i);
		ChoiceItemTextWidget * textWidget = TempleteFactory::crateUnfoldChoiceItemTextWidget(this, mChoiceType, temptPaddingLeft, false);
		textWidget->setTxtData(mFocusTxtData);
		textWidget->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
		textWidget->setTitle(strList.at(i));
		optionTempleteList->push_back(textWidget);
	}
	return optionTempleteList;
}
void UnfoldChoiceWidgetTemplete::setOptions(QList<QString>& strList, QList<int>& paddingLeft)
{
	QList<ChoiceItemTextWidget*>* optionTempleteList = createOptionsByStrList(strList, paddingLeft);
	mChooseList.clear();
	mChooseList.push_back(0);
	setOptions(optionTempleteList);
}
void UnfoldChoiceWidgetTemplete::setOptionsOnCreate(QList<QString>& strList, QList<int>& paddingLeft)
{
	QList<ChoiceItemTextWidget*>* optionTempleteList = createOptionsByStrList(strList, paddingLeft);
	mOptionTempleteList = optionTempleteList;
}
bool UnfoldChoiceWidgetTemplete::isSameOptions(QList<QString> strList)
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
void UnfoldChoiceWidgetTemplete::setTitleHintName(QString str)
{
	TextWidget * title = (TextWidget*)mWidgetTitle;
	title->setDefaultStr(str);
}
StrContentData* UnfoldChoiceWidgetTemplete::getSelfFocusData()
{
	return mFocusTxtData;
}
void UnfoldChoiceWidgetTemplete::setTxtData(StrContentData * focusTxtData)
{
	mFocusTxtData = focusTxtData;
}
bool UnfoldChoiceWidgetTemplete::setDrawTxtParam(StrContentData* focusData)
{
	if (BaseTemplate::isSetFlowType(focusData))
	{
		return true;
	}
	StrContentData* preData = new StrContentData(NULL, NULL);
	preData->refreshTxtParamBySourceParam(mFocusTxtData, false);
	preData->setOperateType(focusData->mOprateType);
	mFocusTxtData->refreshTxtParamOnSetType(focusData);
	bool isSuccess = BaseTemplate::setDrawTxtParam(focusData);
	if (isSuccess)
	{
		delete preData;
		preData = NULL;
		return true;
	}
	else
	{
		mFocusTxtData->refreshTxtParamOnSetType(preData);
		delete preData;
		preData = NULL;
		return false;
	}
}
int UnfoldChoiceWidgetTemplete::mousePress(int type, int x, int y, BaseContainer  * obj)
{
	if (type == Qt::RightButton)
	{
		QMap<QString, QString> map;

		//map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::UNFOLD_RADIO);
		if (mChoiceType == RADIO)
		{
			map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::UNFOLD_RADIO);
			map.insert(TempleteTypeEnumeration::NAME, ((TextWidget*)mWidgetTitle)->getString());
		}
		else if (mChoiceType == MULTIPLE_CHOICE)
		{
			map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::UNFOLD_MULTIPLE_CHOICE);
			map.insert(TempleteTypeEnumeration::NAME, ((TextWidget*)mWidgetTitle)->getString());
		}
		QString tempCanEdit;
		tempCanEdit = "true";

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
		map.insert(TempleteTypeEnumeration::OPTIONS, sendOptions);
		int paddLeft = mPaddingLeft;
		map.insert(TempleteTypeEnumeration::PADDINGLEFT, QString::number(paddLeft));
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
				QList<ChoiceItemTextWidget*>* beforOptionList = mOptionTempleteList;
				SetTxtWidgetElementAction::copyObjListToObjList(mWidgetTitle->mStrObjList, &beforTitleStrList, true);
				SetTxtWidgetElementAction::copyObjListToObjList(mWidgetTitle->mDefaultObjList, &beforDefaultTitleList, true);
				//QList<ViewObj *>  beforSignHaveList;
				//QList<ViewObj *>  beforSignNotHaveList;

				ViewObj* beforFocus = NULL;
				char dir = 0;
				getFocusObjAndDir(beforFocus, dir);
				/*if (mSignBeforSelect)
				{
					SetTxtWidgetElementAction::copyObjListToObjList(mSignBeforSelect->mStrObjList, &beforSignHaveList, true);
				}
				if (mSignBeforNotSelect)
				{
					SetTxtWidgetElementAction::copyObjListToObjList(mSignBeforNotSelect->mDefaultObjList, &beforSignNotHaveList, true);
				}*/

				bool isSetSuccess = setTempleteProperty(map);
				if (isSetSuccess)
				{
					addSetTempletePropertyAction(beforMap, map, beforChooseList, beforDefaultTitleList, beforTitleStrList, beforOptionList,
					 beforFocus, dir);
				}
				return 1;
			}
		}
		return 1;
	}
	else
	{
		
	}
	return 0;
}
void UnfoldChoiceWidgetTemplete::clicked(BaseTemplate* item, bool ischecked)
{
	ChoiceItemTextWidget* checkedItem = (ChoiceItemTextWidget*)item;
	if (mChoiceType==RADIO)
	{
		int optionSize = mOptionTempleteList->size();
		mChooseList.clear();
		mChooseList.push_back(mOptionTempleteList->indexOf(checkedItem));
		ChoiceItemTextWidget* temptItem;
		for (int i = 0; i < optionSize;i++)
		{
			temptItem =mOptionTempleteList->at(i);
			if (temptItem != item)
			{
				temptItem->setChecked(false);
			}
		}
	}
	else
	{
		int indexOfCurrentCheck = mOptionTempleteList->indexOf(checkedItem);
		if (ischecked)
		{
			if (mChooseList.indexOf(indexOfCurrentCheck) < 0)
			{
				mChooseList.push_back(indexOfCurrentCheck);
			}
		}
		else
		{
			if (mChooseList.indexOf(indexOfCurrentCheck) >=0)
			{
				mChooseList.removeAll(indexOfCurrentCheck);
			}
		}
	}
}
QString UnfoldChoiceWidgetTemplete::getOptions()
{
	int size = mOptionTempleteList->size();
	BaseTemplate * temp;
	QString optionsStr;
	for (int i = 0; i < size; i++)
	{
		temp = mOptionTempleteList->at(i);
		try
		{
			ChoiceItemTextWidget* textChiled = (ChoiceItemTextWidget*)temp;
			optionsStr += textChiled->getString() + TempleteTypeEnumeration::SPLIT_STR;
		}
		catch (...)
		{
			continue;
		}
	}
	return optionsStr;
}
QList<ChoiceItemTextWidget*>*UnfoldChoiceWidgetTemplete:: getOptionList()
{
	return mOptionTempleteList;
}
bool UnfoldChoiceWidgetTemplete::setTempleteProperty(QMap<QString, QString>& setMap)
{
	isSetCodeSuccess = setCodeIdToTemplete(setMap);
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
	int paddingleft = setMap.value(TempleteTypeEnumeration::PADDINGLEFT).toInt();
	isSamePaddingLeft = false;
	if (words.size()==mOptionTempleteList->size())
	{
		if (words.size()>0)
		{
			if (paddingleft == mPaddingLeft)
			{
				isSamePaddingLeft = true;
			}
		}
		else
		{
			if (words.size() == 0)
			{
				isSamePaddingLeft = true;
			}
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
			//title->deleteChild(0, -1, false, false);
			title->setStrAndGenatateStrObj(name);
			
			isSetTitleSuccess = true;
		}
		catch (...)
		{

		}
	}
	isSameOptionSuccess = isSameOptions(words);
	if ((!isSetCodeSuccess) && (!isSetTitlepostionSuccess) && (!isSetTitleSuccess) && (isSameOptionSuccess) && (isSamePaddingLeft))
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
		if (!isSamePaddingLeft)
		{
			setOptionPaddingLeft(paddingleft, false);
		}
		deleteContentChild();
		addContentChild();
	
	}
	else
	{
		mChooseList.clear();
		mChooseList.push_back(0);
		QList<int> paddingleftList;
		for (int i = 0; i < words.size();i++)
		{
			if (i == 0)
			{
				paddingleftList.push_back(0);
			}
			else
			{
				paddingleftList.push_back(paddingleft);
			}

		}
		setOptions(words, paddingleftList);
	}

	return true;
}
bool UnfoldChoiceWidgetTemplete::setTitlePosition(char position)
{
	if (mTitlePosition == position)
	{
		return false;
	}
	mTitlePosition = position;
	return true;
}
char UnfoldChoiceWidgetTemplete::getTitlePosition()
{
	return mTitlePosition;
}
void UnfoldChoiceWidgetTemplete::addSetTempletePropertyAction(QMap<QString, QString>& beforMap, QMap<QString, QString>& map, QList<int> beforChooseList
	, QList<ViewObj *> & beforDefaultTitleList, QList<ViewObj *>& beforTitleStrList, QList<ChoiceItemTextWidget*>* beforeOptionTempleteList,
	ViewObj* beforFocus, char focusDir)
{
	OperateMgr * operateMgr = getOperateMgr();
	operateMgr->createNewAction();
	SetUnfoldChoiceWidgetAction* setAction = new SetUnfoldChoiceWidgetAction(operateMgr->getCurStepMgr());
	ViewObj* focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus, dir);
	setAction->initFocus(beforFocus, focus, focusDir, dir);
	setAction->initAction(this, beforMap, map, beforChooseList, mChooseList);


	setAction->initAfterObjList(mWidgetTitle->mDefaultObjList, (isSetTitleSuccess ? mWidgetTitle->mStrObjList : mWidgetTitle->mStrObjList), (isSameOptionSuccess ? NULL : mOptionTempleteList));
	setAction->initBeforObjList(mWidgetTitle->mDefaultObjList, (isSetTitleSuccess ? &beforTitleStrList : &beforTitleStrList), (isSameOptionSuccess ? NULL : beforeOptionTempleteList));

	//setAction->initHaveAndNotHaveObjList((isSetSignBeforSuccess ? &beforHaveList : NULL), (isSetSignNotBeforSuccess ? &beforNotHaveList : NULL), (isSetSignBeforSuccess ? mSignBeforSelect->mStrObjList : NULL), (isSetSignNotBeforSuccess ? mSignBeforNotSelect->mStrObjList : NULL));
	operateMgr->addToLocalAction(setAction);
}
void UnfoldChoiceWidgetTemplete::setPropertyOnActionRedoOrUndo(QMap<QString, QString>& setMap, QList<ViewObj *> * strObjList, QList<ViewObj *> * defaultObjList,
	QList<ChoiceItemTextWidget*>* optionTempleteList, QList<int> chooseList)
{
	setCodeIdToTemplete(setMap);
	QString type = setMap.value(TempleteTypeEnumeration::TYPE);
	QString name = setMap.value(TempleteTypeEnumeration::NAME);
	QString options = setMap.value(TempleteTypeEnumeration::OPTIONS);
	QString titlePostion = setMap.value(TempleteTypeEnumeration::TITLE_POSTION);
	QStringList words = options.split(TempleteTypeEnumeration::SPLIT_STR);
	int paddingLeft = setMap.value(TempleteTypeEnumeration::PADDINGLEFT).toInt();
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
	//如果选项为0，需要显示提示
	if (words.size() == 0 && titlePostion == TempleteTypeEnumeration::TITLE_DIVISABLE)
	{
		setTitlePosition(LEFT);
	}
	mChooseList = chooseList;
	int currentPaddingLeft = getCurrentnPaddingLeftOptions();

	if (optionTempleteList)
	{
		setOptions(optionTempleteList);
	}
	else
	{
		if (currentPaddingLeft != paddingLeft)
		{
			setOptionPaddingLeft(paddingLeft, false);
		}
		deleteContentChild();
		addContentChild();
	}

}
void UnfoldChoiceWidgetTemplete::setFocusData(StrContentData* focusData)
{
	mFocusTxtData = focusData;
}
StrContentData*UnfoldChoiceWidgetTemplete::getFocusData()
{
	return mFocusTxtData;
}
char UnfoldChoiceWidgetTemplete::getChoiceType()
{
	return mChoiceType;
}
//void UnfoldChoiceWidgetTemplete::setChoiceType(char type)
//{
//	mChoiceType = type;
//}
QList<int> UnfoldChoiceWidgetTemplete::getSelectList()
{
	return mChooseList;
}
void UnfoldChoiceWidgetTemplete::setSelectList(QList<int> select)
{
	mChooseList = select;
}
QString UnfoldChoiceWidgetTemplete::getTitleStr()
{
	return mWidgetTitle->getTitle();
}
void UnfoldChoiceWidgetTemplete::initWedgitOnLoad(BaseContentData * data, char titlePositon, QString & titlestr, QList<QString>& strList
	, QList<int>& paddingLeftList, int wedgitPaddingLeft)
{
	mColonViewObj = ViewFactory::createTemplateTxt(mWriterMgr, this, mColonStr, mFocusTxtData);
	this->initContentData(data);
	setTitlePosition(titlePositon);
	createTitle(titlestr);
	mPaddingLeft = wedgitPaddingLeft;
	setOptionsOnCreate(strList, paddingLeftList);
	addContentChild(false);
}
int UnfoldChoiceWidgetTemplete::keyPress(QString str, BaseContainer * obj)
{
	if (TextWidgetKeyboard::isSystemKey(str))
	{
		return 0;
	}
	OperateMgr * operateMgr = getOperateMgr();
	if (operateMgr->isChoosed(this) || operateMgr->getChoosedSize()>0)
	{
		return 0;
	}
	ViewObj* focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus, dir);
	if (focus == obj)
	{
		ViewObj* firstView = getFirstChildView();
		ViewObj* lastView = getLastView();
		if (firstView&&firstView == obj&&dir == BaseConstants::Direction::LEFT)
		{
			return 0;
		}
		if (lastView&&lastView == obj&&dir == BaseConstants::Direction::RIGHT)
		{
			return 0;
		}
		return 1;
	}
	return 0;
}
int UnfoldChoiceWidgetTemplete::keyRelease(QString str, BaseContainer * obj)
{
	ViewObj* focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus, dir);
	if (focus == obj)
	{
		ViewObj* firstView = getFirstChildView();
		ViewObj* lastView = getLastView();
		if (firstView&&firstView == obj&&dir == BaseConstants::Direction::LEFT)
		{
			return 0;
		}
		if (lastView&&lastView == obj&&dir == BaseConstants::Direction::RIGHT)
		{
			return 0;
		}
		return 1;
	}
	return 0;
}
void UnfoldChoiceWidgetTemplete::setOptionPaddingLeft(int paddingLeft, bool isFreshRow)
{
	int isOverFlow = 0;  // -1 表示less，1表示more，0表示不变
	if (paddingLeft>mPaddingLeft)
	{
		isOverFlow = 1;
	}
	else if (paddingLeft == mPaddingLeft)
	{
		isOverFlow = 0;
	}
	else
	{
		isOverFlow = -1;
	}
	mPaddingLeft = paddingLeft;
	int temptPaddingLeft = 0;
	if (mOptionTempleteList)
	{
		ChoiceItemTextWidget* tempItem = NULL;
		for (int i = 0; i<mOptionTempleteList->size(); i++)
		{
			tempItem = mOptionTempleteList->at(i);
			if (i == 0)
			{
				temptPaddingLeft = 0;
			}
			else
			{
				temptPaddingLeft = paddingLeft;
			}
			tempItem->setPaddingLeft(temptPaddingLeft);
		}
		
	}
	if (isFreshRow)
	{
		ViewObj*  lastChild = (ViewObj*)mOptionTempleteList->at(mOptionTempleteList->size() - 1)->getLastView();
		ViewObj* firstRow = (ViewObj*)mOptionTempleteList->at(0)->getFirstView()->getViewParent();
		RowLayout*rowLayout = (RowLayout*)firstRow->getLayout();
		rowLayout->openReplace();
		if (isOverFlow==1)
		{
			rowLayout->judgeOverFlowCircleUntilObj(lastChild);
		}
		else if (isOverFlow == -1)
		{
			rowLayout->judgeLessThanCircleUntilObj(lastChild);
		}
			
		
	}
}
int UnfoldChoiceWidgetTemplete::getCurrentnPaddingLeftOptions()
{
	//int paddingLeft = 0;
	//if (mOptionTempleteList)
	//{
	//	ChoiceItemTextWidget* tempItem;
	//	//for (int i = 0; i<mOptionTempleteList->size(); i++)
	//	if (mOptionTempleteList->size()>0)
	//	{
	//		tempItem = mOptionTempleteList->at(0);
	//		paddingLeft = tempItem->getPaddingLeft();
	//	}
	//}
	//return paddingLeft;
	return mPaddingLeft;
}
int UnfoldChoiceWidgetTemplete::getPaddingLeft()
{
	return mPaddingLeft;
}