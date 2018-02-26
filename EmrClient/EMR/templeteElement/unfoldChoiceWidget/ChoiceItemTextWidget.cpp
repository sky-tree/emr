#include "ChoiceItemTextWidget.h"
#include "UnfoldChoiceButton.h"
#include "BaseConstants.h"
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
ChoiceItemTextWidget::ChoiceItemTextWidget(WriterManager * wMgr, BaseContainer * templeteParent, char type, int objId, int paddingLeft, bool isCreateStrTxtData) : TextWidget(wMgr
	, ConstantsID::ClassId::UNFOLD_CHOICE_ITEM_TEMPLETE, objId, BaseConstants::NONE_STR, isCreateStrTxtData)
{
	setTempleteParent(templeteParent);
	setChooseObjId(templeteParent->getObjID());
	mChoiceButton = new UnfoldChoiceButton(wMgr, this, type, ConstantsID::ContainerId::UNFOLD_CHOICE_BUTTON_TEMPLETE, BaseConstants::NONE_STR, paddingLeft);
}
ChoiceItemTextWidget::ChoiceItemTextWidget(WriterManager * wMgr, ChoiceItemTextWidget * otherBracket) : TextWidget(wMgr, otherBracket, false)
{
	//getOperateMgr()->setCanBeChoosenObjId(otherBracket->getOperateMgr()->getCanBeChoosenID());
	//getOperateMgr()->setCanAddObjParentId(otherBracket->getOperateMgr()->getCanAddObjParentId());
	//TextWidgetMouse * textMouse = InputFactory::createTextWidgetMouse(this);
	//setMouse(textMouse);
	//TextWidgetKeyboard* keyboard = InputFactory::createTextWidgetKeyboard(this);
	//setKeyboard(keyboard);
	//mDefaultString = otherBracket->mDefaultString;
	//mStr = otherBracket->mStr;
	//copyStrList(wMgr, otherBracket->mDefaultObjList, mDefaultObjList);
	//copyStrList(wMgr, otherBracket->mStrObjList, mStrObjList);
	//mChildrenMgr = new BaseChildrenManager(this);
	//addAllStrObjToChild();
	////generateDefaultObj();
	////setTitle(otherBracket->getTitle());
	//if (mContentData)
	//{
	//	delete mContentData;
	//	mContentData = TempleteFactory::createTextWidgetParam(this);
	//}
	//else{
	//	mContentData = TempleteFactory::createTextWidgetParam(this);
	//}
}
ChoiceItemTextWidget::~ChoiceItemTextWidget()
{
	//用的父模板的引用
	if (mFocusTxtData!=NULL)
	{
		mFocusTxtData = NULL;
	}
	QList<BaseContainer *> *  childrenList = getChildren();
	//childrenManager里面析构
	if (mChoiceButton)
	{
		childrenList->removeAll(mChoiceButton);
		delete mChoiceButton;
		mChoiceButton = NULL;
	}
}
bool ChoiceItemTextWidget::setTempleteProperty(QMap<QString, QString>& setMap)
{
	/*bool isSetCodeSuccess = setCodeIdToTemplete(setMap);
	QString content = setMap.value(TempleteTypeEnumeration::CONTENT);
	QString fixName = setMap.value(TempleteTypeEnumeration::NAME);
	if (content.length() == 0)
	{
		content = "固定公式";
		setMap[TempleteTypeEnumeration::CONTENT] = content;
	}
	bool isSetNameSuccess = false;
	if (!(fixName.compare(getElementName()) == 0))
	{
		isSetNameSuccess = true;
		SetElementName( fixName);
	}
	if (content.compare(getString())==0)
	{
		if ((!isSetCodeSuccess) && (!isSetNameSuccess))
		{
			return false;
		}
	}
	setFirstChildViewFocus();
	setStrOnShowTime(content);*/
	//setStr(content);
	//getLastChildView()->setFocus(true,false);
	return true;
}
void ChoiceItemTextWidget::setTitle(QString title, bool isAddToChildrenList)
{
	if (isAddToChildrenList)
	{
		TextWidget::setTitle(title, true);
		 QList<BaseContainer *> * childrenList = getChildren();
		if (childrenList)
		{
			childrenList->push_front(mChoiceButton);
		 }
	}
	else
	{
		TextWidget::setTitle(title, false);
	}
	refreshChildrenIndex(0);
}
void ChoiceItemTextWidget::setChecked(bool checked)
{
	mChoiceButton->setChecked(checked);
}
bool ChoiceItemTextWidget::getChecked()
{
	return mChoiceButton->getChecked();
}
bool ChoiceItemTextWidget::setDrawTxtParam(StrContentData* focusData)
{
	//BaseTemplate::setDrawTxtParam(focusData);
	bool isSuccess = mChoiceButton->setDrawTxtParam(focusData);
	if (isSuccess)
	{
		return TextWidget::setDrawTxtParam(focusData);
	}
	else
	{
		return false;
	}
	
}
//int ChoiceItemTextWidget::mousePress(int type, int x, int y, BaseContainer * obj)
//{
//	if (type == Qt::RightButton)
//	{
//		mTempParent->mousePress(type, x, y, obj);
//	}
//	else
//	{
//		if (mChoiceButton->getChecked())
//		{
//
//		}
//	}
//}
void ChoiceItemTextWidget::setPaddingLeft(int paddingLeft)
{
	mChoiceButton->setPaddingLeft(paddingLeft);
}
int ChoiceItemTextWidget::getPaddingLeft()
{
	return mChoiceButton->getPaddingLeft();
}