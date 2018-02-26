#include "MacroTemplete.h"
//#include "ViewParamFactory.h"
#include "OperateMgr.h"
#include "TempleteFactory.h"
#include "TextWidget.h"
#include "BracketWidget.h"
#include "qdebug.h"
#include "TempleteTypeEnumeration.h"
#include "ConstantsID.h"
#include "UIConstants.h"
#include "FlagParam.h"
#include "ViewObj.h"
#include "WriterManager.h"
#include "MacroTempleteParam.h"
#include "StrContentData.h"
#include "StrConstants.h"
#include "BaseChildrenManager.h"
#include "SetTxtWidgetElementAction.h"
//#include "AddTempletWidgetAction.h"
#include "SetMacroWidgetAction.h"
MacroTemplete::MacroTemplete(WriterManager * wMgr, BaseTemplate * templeteParent, int objId, QString codeId, bool isCreateOnLoad)
: BaseTemplate(wMgr, ConstantsID::ClassId::MACRO_WIDGET_TEMPLETE, objId, codeId)
//=======
//MacroTemplete::MacroTemplete(WriterManager * wMgr, BaseTemplete * templeteParent, ViewObj * viewParent, int objId) : BaseTemplete(wMgr, ConstantsID::ClassId::MACRO_WIDGET_TEMPLETE, templeteParent, viewParent, objId)
//>>>>>>> 5577721ac74ae7078ab5c2da0561d8f922d7d8a3
, mWidgetTitle(NULL)
, mContent(NULL)
{
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
	if (wMgr){
		wMgr->addMacro(this);
	}
	mId = -1;
	/*WriterManager* writeManget = templeteParent->getParentWindow()->getViewParent()->getWriterManager();
	if (writeManget){
		writeManget->addMacro(this);
	}*/
	if (!isCreateOnLoad)
	{
		initWidget();
	}
}
MacroTemplete::MacroTemplete(WriterManager * wMgr, MacroTemplete * otherMacroTemplete) : BaseTemplate(wMgr, otherMacroTemplete, false)
, mWidgetTitle(NULL)
, mContent(NULL)
{
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
	if (wMgr){
		wMgr->addMacro(this);
	}
	mId = otherMacroTemplete->mId;
	mWidgetTitle = new TextWidget(wMgr, otherMacroTemplete->mWidgetTitle);
	mContent = new  BracketWidget(wMgr, (BracketWidget*)otherMacroTemplete->mContent);
	childrenMgr->addChild(mWidgetTitle, -1, false);
	childrenMgr->addChild(mContent, -1, false);
	refreshChildrenIndex(0);
	mContentData = TempleteFactory::createMacronParam(this);
}
MacroTemplete::~MacroTemplete()
{
	/*ViewObj * window = getParentWindow();
	if (window)
	{
		WriterManager* writeManget = window->getViewParent()->getWriterManager();
		if (writeManget){
			writeManget->deleteMacro(this);
		}
	}*/
	if (mWidgetTitle)
	{
		delete mWidgetTitle;
		mWidgetTitle = NULL;
	}
	if (mContent)
	{
		delete mContent;
		mContent = NULL;
	}
	if (mWriterMgr){
		mWriterMgr->deleteMacro(this);
	}
	getChildren()->clear();
}
void MacroTemplete::initWidget()
{
	//这个是窗口的字体
	StrContentData * windTxtData = getFocusStrData();
	int fontColor = windTxtData->getFontColor();
	int sizeOfFont = windTxtData->getPointSize();
	bool isBold = windTxtData->getBold();
	//这个是宏元素前面  标题的字体
	/*FocusTxtData* titleTxtData =  ViewParamFactory::createFocusTxtData();
	titleTxtData->setFontColor(fontColor);
	titleTxtData->setPointSize(sizeOfFont);
	titleTxtData->setBold(isBold);*/

	//这个是宏元素后面  内容的字体
	//FocusTxtData* contentTxtData = ViewParamFactory::createFocusTxtData();
	////contentTxtData->setFontColor(UIConstants::MyColor::RED);
	//contentTxtData->setFontColor(fontColor);
	//contentTxtData->setPointSize(sizeOfFont);
	//contentTxtData->setBold(isBold);
	TextWidget* textWidget = TempleteFactory::createMacroTextWidgetTemplete(this, false);
	//textWidget->setChooseObjId(mObjID);
	textWidget->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
	if (!textWidget->getSelfFocusData())
	{
		StrContentData* textWidgetFocusTxtData = new StrContentData(this, NULL, windTxtData->getString(), windTxtData);
		textWidget->setTxtData(textWidgetFocusTxtData);
	}
	mWidgetTitle = textWidget;
	BracketWidget* textWidget1 = TempleteFactory::createBracketTemplete(this, false);
	//textWidget1->setChooseObjId(mObjID);
	textWidget1->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
	if (!textWidget1->getSelfFocusData())
	{
		StrContentData* textWidget1FocusTxtData = new StrContentData(this, NULL, windTxtData->getString(), windTxtData);
		textWidget1->setTxtData(textWidget1FocusTxtData);
	}

	textWidget1->setTitle("内容");
	mContent = textWidget1;
	
	BaseChildrenManager * childrenMgr =  getChildrenMgr();
	childrenMgr->addChild(mWidgetTitle, -1, false);
	childrenMgr->addChild(mContent, -1, false);
	refreshChildrenIndex(0);
	//contentTxtData->setFontColor(fontColor);
	//windTxtData->setFontColor(UIConstants::MyColor::BLACK);
}
void MacroTemplete::setTitle(QString str)
{
	mWidgetTitle->setTitle(str);
}
QString MacroTemplete::getTitle()
{
	return mWidgetTitle->getTitle();
}
void MacroTemplete::setContent(QString str, bool needDeleteAndAddShow, bool isFreshStrData)
{
	if (str.size()==0)
	{
		str = " ";
	}
	if (str.compare(mContent->getTitle()) != 0)
	{
		if (needDeleteAndAddShow)
		{
			setFirstChildViewFocus(true, isFreshStrData);
			mContent->deleteChild(0, -1, true);
			mContent->setTitle(str, false);
			mContent->ShowObjList();
		}
		else
		{
			mContent->setTitle(str, true);
		}
		
	}
}
QString MacroTemplete::getContent()
{
	return mContent->getTitle();
}
void MacroTemplete::setTitle(TextWidget* title)
{
	mWidgetTitle = title;
}
void MacroTemplete::setContent(BracketWidget*content)
{
	mContent = content;
}
void MacroTemplete::setId(int id)
{
	mId = id;
}
int MacroTemplete::getId(){
	return mId;
}
void MacroTemplete::addWidgetToRow(bool isFresh )
{

}
int MacroTemplete::mousePress(int type, int x, int y, BaseContainer  * obj)
{
	if (type == Qt::RightButton)
	{
		QMap<QString, QString> map;
		QMap<QString, QString> beforMap;
		map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::MACRO);
		map.insert(TempleteTypeEnumeration::NAME, ((TextWidget*)mWidgetTitle)->getString());
		map.insert(TempleteTypeEnumeration::MACRO_VALUSE,mContent->getTitle());
		addCodeIdToMap(map);
		beforMap = map;
		bool isDelete =  TempleteTypeEnumeration::showDialog(&map, true);
		if (isDelete)
		{
			BaseTemplate::deleteSelf();
		}
		QList<ViewObj *>  contentDefaultObjListBefore;
		QList<ViewObj *> contentStrObjListBefore;
		QList<ViewObj *>  titleDefaultObjListBefore;
		QList<ViewObj *> titleStrObjListBefore;
		ViewObj* beforFocus = NULL;
		char dir = 0;
		getFocusObjAndDir(beforFocus,dir);
		SetTxtWidgetElementAction::copyObjListToObjList(mContent->mDefaultObjList, &contentDefaultObjListBefore, true);
		SetTxtWidgetElementAction::copyObjListToObjList(mContent->mStrObjList, &contentStrObjListBefore, true);
		SetTxtWidgetElementAction::copyObjListToObjList(mWidgetTitle->mDefaultObjList, &titleDefaultObjListBefore, true);
		SetTxtWidgetElementAction::copyObjListToObjList(mWidgetTitle->mStrObjList, &titleStrObjListBefore, true);
		bool isSetSeccess = setTempleteProperty(map);
		if (isSetSeccess)
		{
			addSetTempletePropertyAction(beforMap, map, &contentStrObjListBefore, &contentDefaultObjListBefore, &titleStrObjListBefore, &titleDefaultObjListBefore, beforFocus, dir);
		}
		return 1;
	}
	
	return BaseConstants::NONE;
}
bool MacroTemplete::setTempleteProperty(QMap<QString, QString>& setMap)
{
	QString type = setMap.value(TempleteTypeEnumeration::TYPE);
	//QString name = map.value(TempleteTypeEnumeration::NAME);
	QString contentStr = setMap.value(TempleteTypeEnumeration::MACRO_VALUSE);
	if (contentStr.length() == 0)
	{
		contentStr = " ";
		setMap[TempleteTypeEnumeration::MACRO_VALUSE] = contentStr;
	}
	bool isContentChangeD = false;
	if (contentStr.compare(mContent->getTitle()) == 0)
	{
		
	}
	else
	{
		isContentChangeD = true;
	}
		bool isTitleChangeD = false;
	QString titleStr = setMap.value(TempleteTypeEnumeration::NAME);
	if (titleStr.length() == 0)
	{
		titleStr = " ";
		setMap[TempleteTypeEnumeration::NAME] = titleStr;
	}
	if (titleStr.compare(mWidgetTitle->getTitle()) == 0)
	{

	}
	else
	{
		isTitleChangeD = true;
	}
	if ((!isContentChangeD) && (!isTitleChangeD))
	{
		return false;
	}
	setCodeIdToTemplete(setMap);
	//setTitle(name);
	if (isTitleChangeD)
	{
		mWidgetTitle->deleteChild(0, -1, true);
		mWidgetTitle->setTitle(titleStr,false);
		mWidgetTitle->ShowObjList();
	}
	if (isContentChangeD)
	{
		mContent->deleteChild(0, -1, true);
		mContent->setTitle(contentStr,false);
		mContent->ShowObjList();
	}
	OperateMgr * operateMgr = getOperateMgr();
	operateMgr->clearChooseList(true);
	getLastChildView()->setFocus(true,false);
	replaceNeedReplaceList(1);
	return true;
}
void MacroTemplete::addSetTempletePropertyAction(QMap<QString, QString>& beforMap, QMap<QString, QString>& map, QList<ViewObj *> * beforContentStrObjList, QList<ViewObj *> * beforDefaultContentObjList, QList<ViewObj *> * beforTitleStrObjList, QList<ViewObj *> * beforDefaultTitleObjList, ViewObj* beforFocus, char focusDir)
{
	OperateMgr * operateMgr = getOperateMgr();
	operateMgr->createNewAction();
	SetMacroWidgetAction* setAction = new SetMacroWidgetAction(operateMgr->getCurStepMgr());
	ViewObj* focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus, dir);
	setAction->initFocus(beforFocus, focus, focusDir, dir);
	setAction->initAction(this, beforMap, map);
	setAction->initAfterObjList(mContent->mDefaultObjList, mContent->mStrObjList, mWidgetTitle->mDefaultObjList, mWidgetTitle->mStrObjList);
	setAction->initBeforObjList(beforDefaultContentObjList, beforContentStrObjList, beforDefaultTitleObjList,beforTitleStrObjList);
	operateMgr->addToLocalAction(setAction);
}
void MacroTemplete::setPropertyOnActionRedoOrUndo(QMap<QString, QString>& setMap, QList<ViewObj *> * contentStrObjList, QList<ViewObj *> * contentDefaultObjList
	, QList<ViewObj *> * titleStrObjList, QList<ViewObj *> * titleDefaultObjList)
{
	QString type = setMap.value(TempleteTypeEnumeration::TYPE);
	//QString name = map.value(TempleteTypeEnumeration::NAME);
	QString contentStr = setMap.value(TempleteTypeEnumeration::MACRO_VALUSE);
	bool isContentChanged = false;
	if (contentStr.compare(mContent->getTitle()) == 0)
	{
		
	}
	else
	{
		isContentChanged = true;
	}
	QString titleStr = setMap.value(TempleteTypeEnumeration::NAME);
	bool isTitleChanged = false;
	if (titleStr.compare(mWidgetTitle->getTitle()) == 0)
	{

	}
	else
	{
		isTitleChanged = true;
	}
	if ((!isContentChanged) && (!isTitleChanged))
	{
		return;
	}
	setCodeIdToTemplete(setMap);
	if (isContentChanged)
	{
		mContent->setOnlyStr(contentStr);
		mContent->copyObjListAndReplace(contentStrObjList, contentDefaultObjList);
	}
	if (isTitleChanged)
	{
		mWidgetTitle->setOnlyStr(titleStr);
		mWidgetTitle->copyObjListAndReplace(titleStrObjList, titleDefaultObjList);
	}
	/*SetTxtWidgetElementAction::copyObjListToObjList(strObjList, mContent->mStrObjList, true);
	SetTxtWidgetElementAction::copyObjListToObjList(defaultObjList, mContent->mDefaultObjList, true);*/
	
	
	//setTitle(name);
}
int MacroTemplete::mouseRelease(int type, int x, int y, BaseContainer * obj)
{
	/*if (obj == mSignBeforeOptionObj || obj == mSignBehindOptionObj)
	{
		setNormalColor();
	}
	int returnType = BaseConstants::NONE;
	setNormalColor();
	if (mInputManager){
		return mInputManager->mouseRelease(type, x, y, obj);
	}*/
	return BaseConstants::NONE;
}
int MacroTemplete::mouseMove(int type, int x, int y, BaseContainer * obj)
{
	int returnType = BaseConstants::NONE;
	/*if (mInputManager){
		return mInputManager->mouseMove(type, x, y, obj);
	}*/
	return returnType;
}
int MacroTemplete::mouseDrag(int type, int x, int y, BaseContainer * obj)
{
	int returnType = BaseConstants::NONE;
	/*if (mInputManager){
		return mInputManager->mouseDrag(type, x, y, obj);
	}*/
	return returnType;
}
int MacroTemplete::mouseDoublePress(int type, int x, int y, BaseContainer * obj)
{
	/*QString  sendType;
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
	QList<DataType*> *sendOptions = new QList<DataType*>();
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
		sendOptions->push_back(tempDataType);
	}

	SelectionPopup::getSelection(sendType, *sendOptions);
	QList<int>  chooseList;
	for (int i = 0; i < optionListSize; i++)
	{

		DataType* tempDataType = sendOptions->at(i);
		if (tempDataType->isChecked){
			chooseList.push_back(i);
		}
	}
	setSelectItem(chooseList);*/
	/*int returnType = BaseConstants::NONE;
	if (mInputManager){
		return mInputManager->mouseDoublePress(type, x, y, obj);
	}*/
	return BaseConstants::NONE;
}
int MacroTemplete::mouseScroll(int angle, BaseContainer * obj)
{
	int returnType = BaseConstants::NONE;
	/*if (mInputManager){
		return mInputManager->mouseScroll(angle, obj);
	}*/
	return returnType;
}
int MacroTemplete::keyPress(QString str, BaseContainer * obj)
{
	ViewObj* focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus,dir);
	if (focus==obj)
		{

		}
		else{
			return 0;
		}
		if (str.compare(StrConstants::MyKeyBoard::BACK_SPACE) == 0)
		{
			OperateMgr * operateMgr = getOperateMgr();
			if (operateMgr->getChoosedSize()>0)
			{
				return 0;
			}
			ViewObj* lastView = getLastChildView();
			if (lastView == (ViewObj*)obj)
			{
				if (dir == BaseConstants::Direction::RIGHT)
				{
					//deleteSelf();
					ViewObj* preView = lastView->getPreView();
					if (preView)
					{
						preView->setFocus(true, false);
					}
					return 1;
				}
			}

		}
		else if (str.compare(StrConstants::MyKeyBoard::DEL) == 0)
		{
			OperateMgr * operateMgr = getOperateMgr();
			if (operateMgr->getChoosedSize()>0)
			{
				return 0;
			}
			ViewObj* firstView = getFirstChildView();
			if (firstView == (ViewObj*)obj)
			{
				if (dir == BaseConstants::Direction::LEFT)
				{
					//deleteSelf();
					ViewObj* nextView = firstView->getNextView();
					if (nextView)
					{
						nextView->setFocus(false, false);
					}
					return 1;
				}
			}


		}
	return  BaseConstants::NONE;
}
int MacroTemplete::keyRelease(QString str, BaseContainer * obj)
{
	if (isFocousObj(obj))
	{

	}
	else{
		return 0;
	}
	int returnType = BaseConstants::NONE;
	
	return returnType;
}
bool MacroTemplete::setDrawTxtParam(StrContentData* focusData)
{
	if (BaseTemplate::isSetFlowType(focusData))
	{
		return true;
	}
	bool isSuccess = mWidgetTitle->setDrawTxtParam(focusData);
	if (isSuccess)
	{
		return mContent->setDrawTxtParam(focusData);
	}
	else
	{
		return false;
	}

}
StrContentData*MacroTemplete::getSelfFocusData()
{
	return mWidgetTitle->getSelfFocusData();
}
void MacroTemplete::getWillShowObjList(QList<ViewObj *> * container)
{
	mWidgetTitle->getWillShowObjList(container);
	mContent->getWillShowObjList(container);
}
void MacroTemplete::haveDeleteFromParent(BaseContainer * parent)
{
	if (mWriterMgr)
	{
		mWriterMgr->deleteMacro(this);
	}

}
void MacroTemplete::haveAddToParent(BaseContainer * parent)
{
	if (mWriterMgr)
	{
		mWriterMgr->addMacro(this);
	}

}