#include "BracketWidget.h"
#include "TextWidget.h"
#include "TempleteFactory.h"
//#include "BaseOperateMgr.h"
#include "ConstantsID.h"
#include "TextWidgetMouse.h"
#include "InputFactory.h"
#include "BracketWidgetKeyboard.h"
#include "TextWidgetParam.h"
#include "ViewObj.h"
#include "StrContentData.h"
#include "UIConstants.h"
#include "TempleteTypeEnumeration.h"
#include "BaseChildrenManager.h"
//#include "AddTempletWidgetAction.h"
//#include "SetTxtWidgetElementAction.h"
BracketWidget::BracketWidget(WriterManager * wMgr, BaseTemplate * templeteParent, int objId) : TextWidget(wMgr, templeteParent
	, ConstantsID::ClassId::BRAKETWIDGET_TEMPLETE, objId, BaseConstants::NONE_STR, true)
	, mParentViewOfSignBeforView(NULL)
	, mParentViewOfSignBehindView(NULL)
//, mSignBeforeOptionObj(NULL)
//, mSignBehindOptionObj(NULL)
{
	mNeedCopyChildrenWhenPast = false;
}
BracketWidget::BracketWidget(WriterManager * wMgr, BaseTemplate * templeteParent, int classid, int objId, QString codeId, bool needCreateTxtData)
: TextWidget(wMgr, templeteParent,
classid, objId, BaseConstants::NONE_STR, needCreateTxtData)
	, mParentViewOfSignBeforView(NULL)
	, mParentViewOfSignBehindView(NULL)
{

}
BracketWidget::BracketWidget(WriterManager * wMgr, BracketWidget * otherBracket, bool canCopyChild) : TextWidget(wMgr, otherBracket, canCopyChild)
, mParentViewOfSignBeforView(NULL)
, mParentViewOfSignBehindView(NULL)
{

}
BracketWidget::BracketWidget(WriterManager * wMgr, BracketWidget * otherBracket) : TextWidget(wMgr, otherBracket, false)
, mParentViewOfSignBeforView(NULL)
, mParentViewOfSignBehindView(NULL)
{
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
	//getOperateMgr()->setCanBeChoosenObjId(otherBracket->getOperateMgr()->getCanBeChoosenID());
	//getOperateMgr()->setCanAddObjParentId(otherBracket->getOperateMgr()->getCanAddObjParentId());
	TextWidgetMouse * textMouse = InputFactory::createTextWidgetMouse(this);
	setMouse(textMouse);
	BracketWidgetKeyboard* keyboard = InputFactory::createBrackWidgetKeyboard(this);
	setKeyboard(keyboard);
	mDefaultString = otherBracket->mDefaultString;
	mStr = otherBracket->mStr;
	copyStrList(wMgr, otherBracket->mDefaultObjList, mDefaultObjList);
	copyStrList(wMgr, otherBracket->mStrObjList, mStrObjList);
	addAllStrObjToChild();
	//if (mStrObjList->size()>0)
	//{
		//mSignBeforeOptionObj = mStrObjList->at(0);
		//mSignBehindOptionObj = mStrObjList->at(mStrObjList->size() - 1);
	//}
	//else
	//{
		//mSignBeforeOptionObj = mDefaultObjList->at(0);
		//mSignBehindOptionObj = mDefaultObjList->at(mStrObjList->size() - 1);
	//}
	//generateDefaultObj();
	//setTitle(otherBracket->getTitle());
	if (mContentData)
	{
		delete mContentData;
		mContentData = TempleteFactory::createTextWidgetParam(this);
	}
	else{
		mContentData = TempleteFactory::createTextWidgetParam(this);
	}
}

BracketWidget::~BracketWidget()
{
	if (mParentViewOfSignBeforView)
	{
		mParentViewOfSignBeforView = NULL;
	}
	if (mParentViewOfSignBehindView)
	{
		mParentViewOfSignBehindView = NULL;
	}
	/*if (mSignBeforeOptionObj)
	{
		mSignBeforeOptionObj = NULL;
	}*/
	/*if (mSignBehindOptionObj)
	{
		mSignBehindOptionObj = NULL;
	}*/
}
//void BracketWidget::setTempleteStatus(int status){
//	mTemptStatus = status;
//	mContentTemplete->setTempleteStatus(status);
//}
//int BracketWidget::getTempleteStatus(){
//	return mTemptStatus;
//}
void BracketWidget::setTitle(QString str, bool isAddToChildRENlIST)
{
	
	TextWidget::setStr(mWidgetSignBeforeOptionItem + str + mWidgetSignBehindOptionItem, isAddToChildRENlIST);
	//mSignBeforeOptionObj = mStrObjList->at(0);
	//mSignBehindOptionObj = mStrObjList->at(mStrObjList->size()-1);
}
void BracketWidget::setTitleOnShow(QString title)
{
	setFirstChildViewFocus();
	deleteChild(0, -1, true);
	setTitle(title, false);

	ShowObjList();
}
QString BracketWidget::getTitle()
{
	QString tempStr = TextWidget::getString();
	tempStr.remove(0,1);
	tempStr.remove(tempStr.length()-1, 1);
	return tempStr;
}
void BracketWidget::setHint(QString title, bool isAddToChildrenList)
{
	//int fontColor = mFocusTxtData->getFontColor();
	//mFocusTxtData->setFontColor(UIConstants::MyColor::GRAYER);
	
	TextWidget::setDefaultStr(mWidgetSignBeforeOptionItem + title + mWidgetSignBehindOptionItem,isAddToChildrenList);
	int endIndex = mDefaultObjList->size() - 1;
	ViewObj * temptView=NULL;
	for (int i = 1; i <endIndex; i++)
	{
		temptView = mDefaultObjList->at(i);
		StrContentData * txtData= (StrContentData*)temptView->getContentData();
		txtData->setNeedGray(true);
	}
	//ViewObj * fistView = mDefaultObjList->at(0);
	//ViewObj * lastView = mDefaultObjList->at(mDefaultObjList->size()-1);
	//setViewObjColor(fistView, fontColor);
	//setViewObjColor(lastView, fontColor);
	//mFocusTxtData->setFontColor(fontColor);
}
QString BracketWidget::getHint()
{
	QString tempStr=TextWidget::getDefaultString();
	tempStr.remove(0, 1);
	tempStr.remove(tempStr.length() - 1, 1);
	return tempStr;
}
//void BracketWidget::setContent(QString title)
//{
//	TextWidget::setStr(mWidgetSignBeforeOptionItem + str + mWidgetSignBehindOptionItem);
//}
//QString BracketWidget::getContent()
//{
//	QString tempStr = mWidgetSignBeforeOptionItem + TextWidget::getString() + mWidgetSignBehindOptionItem;
//	return tempStr;
//}
void BracketWidget::showTidy()
{
	//if (mIsShowed)
	{
		if (!mIsShowTidy)
		{
			mIsShowTidy = true;
			ViewObj* firstView;
			ViewObj* lastView;
			int size = mStrObjList->size();
			if (size>0){
				firstView = mStrObjList->at(0);
				lastView = mStrObjList->at(size-1);
			}
			else{
				firstView = mDefaultObjList->at(0);
				lastView = mDefaultObjList->at(mDefaultObjList->size() - 1);
			}
			
			if (mStrObjList->size()==0)
			{
				//clearDefaultObjListFromShow();
				setFirstChildViewFocus();
				deleteChild(0, -1, true, false);
			}
			else
			{
				firstView->setFocus(false);
				deleteChild(firstView, true, false);
				lastView->setFocus(false);
				deleteChild(lastView, true, false);
			}
		}
		else{
			mIsShowTidy = false;
			ViewObj* addFirstView;
			ViewObj* addLastView;
			int size = mStrObjList->size();
			if (size>0){
				addFirstView = mStrObjList->at(0);
				addLastView = mStrObjList->at(size - 1);
			}
			else{
				addFirstView = mDefaultObjList->at(0);
				addLastView = mDefaultObjList->at(mDefaultObjList->size() - 1);
			}
			//ViewObj* row = NULL;
			//int viewIndex = 0;
		
				ViewObj* preView = BaseContainer:: getPreView();
				/*ViewObj* firstView = getFirstChildView();
				ViewObj* row = firstView->getViewParent();
				int viewIndex = firstView->getViewIndexFromParent();*/
				bool hasFind = false;
				if (preView&&!preView->isEnter()){
					//row = preView->getViewParent();
					//viewIndex = preView->getViewIndexFromParent() + 1;
					preView->setFocus(true);
					hasFind = true;
				}
				if (!hasFind)
				{
					ViewObj* firstView = getFirstChildView();
					if (firstView)
					{
						firstView->setFocus(false);
						//row = firstView->getViewParent();
						//viewIndex = firstView->getViewIndexFromParent();
						hasFind = true;
					}
				}
				if (!hasFind)
				{
					ViewObj* nextView = BaseContainer::getNextView();
					nextView->setFocus(false);
					//row = nextView->getViewParent();
					//viewIndex = nextView->getViewIndexFromParent() ;
				}
				if (mStrObjList->size() == 0)
				{
					ShowObjList();
					//showObjListWithNoFlag(row, viewIndex, mDefaultObjList, 0, mDefaultObjList->size());
				}
				else
				{
					//addTempleteChildToViewParent(addFirstView, row, 0, viewIndex, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF);
					addChild(addFirstView,0,true,true);
					ViewObj* lastView = getLastChildView();
					lastView->setFocus(true);
					addChild(addLastView, getChildren()->size(), true, true);
					//row = lastView->getViewParent();
					//viewIndex = lastView->getViewIndexFromParent() + 1;
					//addTempleteChildToViewParent(addLastView, row, -1, viewIndex, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF);
				}
			
		}
	}
}
bool BracketWidget::setTempleteProperty(QMap<QString, QString>& setMap)
{
	QString hint = setMap.value(TempleteTypeEnumeration::INFO);
	if (hint.size()==0)
	{
		hint = " ";
	}
	bool setHintIsSuccess = true;
	if (hint.compare(getHint())==0)
	{
		setHintIsSuccess =  false;
	}
	QString elementName = setMap.value(TempleteTypeEnumeration::NAME);
	bool setElementNameIsSuccess = true;
	if (elementName.compare(getElementName()) == 0)
	{
		setElementNameIsSuccess = false;
	}
	else
	{
		SetElementName(elementName);
	}
	if ((!setElementNameIsSuccess)&&(!setHintIsSuccess))
	{
		return false;
	}
	setCodeIdToTemplete(setMap);
	//QString name = map.value(TempleteTypeEnumeration::NAME);
	//StrContentData* focusData = this->getSelfFocusData();
	//int color = mFocusTxtData->getFontColor();
	//QFont* font = focusData->getFont();
	
	setFirstChildViewFocus();
	deleteChild(0, -1, true);
	setHint(hint,false);
	
	ShowObjList();
	ViewObj* lastObj =getLastChildView();
	if (lastObj){
		lastObj->setFocus(true, false);
	}
	
	//focusData->setFontColor(color);
	return true;
}
void BracketWidget::setPropertyOnActionRedoOrUndo(QMap<QString, QString>& setMap, QList<ViewObj *> * strObjList, QList<ViewObj *> * defaultObjList)
{
	setCodeIdToTemplete(setMap);
	StrContentData* focusData = this->getSelfFocusData();
	int color = mFocusTxtData->getFontColor();
	//QFont* font = focusData->getFont();
	QString hint = setMap.value(TempleteTypeEnumeration::INFO);
	mDefaultString = hint;
	mNameStr = setMap.value(TempleteTypeEnumeration::NAME);
	copyObjListAndReplace(strObjList, defaultObjList);
}