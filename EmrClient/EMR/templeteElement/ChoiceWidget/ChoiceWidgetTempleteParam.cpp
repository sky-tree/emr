#include "ChoiceWidgetTempleteParam.h"
#include "ChoiceWidgetTemplete.h"
#include "TempleteFactory.h"
#include "StrContentData.h"
#include "BaseIOTools.h"
#include "ContentDataFactory.h"
#include "TextWidgetParam.h"
#include "TextWidget.h"
#include "ConstantsID.h"
#include "BaseChildrenManager.h"
ChoiceWidgetTempleteParam::ChoiceWidgetTempleteParam(BaseTemplate * templeteOwner, CodeParams * codeParams) : CodeTempData(templeteOwner
	, ConstantsID::ClassId::CHOICEAA_WIDGET_PARAM, codeParams)
, mTempleteOwner((ChoiceWidgetTemplete*)templeteOwner)
//, mBeforSelectData(NULL)
//, mBeforNotSelectData(NULL)
{
	//setNeedLoadSelfChildren(false);
}


ChoiceWidgetTempleteParam::~ChoiceWidgetTempleteParam()
{
	if (mTempleteOwner)
	{
		mTempleteOwner = NULL;
	}
}
void ChoiceWidgetTempleteParam::setChoiceTempleteOwner(ChoiceWidgetTemplete * templeteOwner)
{
	mTempleteOwner = templeteOwner;
	mOwner = templeteOwner;
}
void ChoiceWidgetTempleteParam::saveSelfData(BaseIOTools * ioTools)
{
	//return;
	mOwnerObjId = mTempleteOwner->getObjID();
	if (mOwnerObjId == ConstantsID::ContainerId::CHOICEWIDGET_TEMPLETE)
	{
		bool canInputOnKeyboard = mTempleteOwner->getCanInputOnKeyBoard();
		ioTools->writeBool(canInputOnKeyboard);
		StrContentData * txtData = mTempleteOwner->getFocusData();
		saveFocusData(txtData, ioTools);
		int wedgitType = mTempleteOwner->getWidgetType();
		ioTools->writeInt(wedgitType);
		if (wedgitType == ChoiceWidgetTemplete::HAVE_CHOICE){
			BaseTemplate* beforSelect = mTempleteOwner->getSignBeforSelect();
			BaseContentData * beforeSelectContentData = beforSelect->getContentData();
			beforeSelectContentData->setNeedLoadSelfChildren(false);
			beforeSelectContentData->doSave(ioTools);
			beforeSelectContentData->setNeedLoadSelfChildren(true);
			BaseTemplate* signBeforNotSelect = mTempleteOwner->getSignBeforNotSelect();
			BaseContentData * beforeNotSelectContentData = signBeforNotSelect->getContentData();
			beforeNotSelectContentData->setNeedLoadSelfChildren(false);
			beforeNotSelectContentData->doSave(ioTools);
			beforeNotSelectContentData->setNeedLoadSelfChildren(true);
		}
		QList<int> chooseList = mTempleteOwner->getSelectList();
		int sizeChoose = chooseList.size();
		ioTools->writeInt(sizeChoose);
		for (int i = 0; i < sizeChoose; i++)
		{
			ioTools->writeInt(chooseList.at(i));
		}
		ioTools->writeInt(mTempleteOwner->getTempleteStatus());
		char titlePotion = mTempleteOwner->getTitlePosition();
		ioTools->writeInt(titlePotion);
		//if (titlePotion == ChoiceWidgetTemplete::DISVISABLE)
		{
			BaseTemplate* title = mTempleteOwner->getTiltle();
			BaseContentData * titleContentData = title->getContentData();
			titleContentData->setNeedLoadSelfChildren(false);
			titleContentData->doSave(ioTools);
			titleContentData->setNeedLoadSelfChildren(true);
		}
		QList<TextWidget*>* optionList = mTempleteOwner->getOptionList();
		int optionSize = optionList->size();
		ioTools->writeInt(optionSize);
		TextWidget* tempOption;
		for (int i = 0; i < optionSize; i++)
		{
			tempOption = optionList->at(i);
			BaseContentData * optionContentData = tempOption->getContentData();
			optionContentData->setNeedLoadSelfChildren(false);
			optionContentData->doSave(ioTools);
			optionContentData->setNeedLoadSelfChildren(true);
		}
	}

}

void ChoiceWidgetTempleteParam::loadSelfData(BaseIOTools * ioTools)
{
	//return;
	if (mOwnerObjId == ConstantsID::ContainerId::CHOICEWIDGET_TEMPLETE)
	{
		mCanInputOnKeyBoard = ioTools->readBool();
		mFocusTxtData = loadFocusData(ioTools);
		mWidgetType = ioTools->readInt();
		
	}

}
BaseContainer * ChoiceWidgetTempleteParam::createOwner(BaseContainer * tempParent1, BaseIOTools *iOTools, bool isAddToParenTemplate)
{
	BaseTemplate* tempParent = (BaseTemplate*)tempParent1;
	//CodeParams * codeParams = 
	//int userID = tempParent1->getUserID();
	ChoiceWidgetTemplete * choiceWidget;
	if (mOwnerObjId == ConstantsID::ContainerId::CHOICEWIDGET_TEMPLETE)
	{
		choiceWidget = TempleteFactory::createChoiceWidgetTemplete(tempParent, mWidgetType, true);
		choiceWidget->setTempleteStatus(mTemptStatus);
		
		//choiceWidget->setWidgetType(mWidgetType);
		choiceWidget->setCanInputOnKeyBoard(mCanInputOnKeyBoard);
		choiceWidget->setFocusData(mFocusTxtData);
		mFocusTxtData->initPainterAndFont(tempParent);
		//choiceWidget->initWidget();
		BaseChildrenManager *  childMgr = choiceWidget->getChildrenMgr();
		//读取数据
		if (mWidgetType == ChoiceWidgetTemplete::HAVE_CHOICE)
		{
			TextWidget* beforSelectTemplete = (TextWidget*)ContentDataFactory::loadData(choiceWidget->getWriterMgr(), iOTools, choiceWidget, 0);
			childMgr-> delChild(beforSelectTemplete, false);
			choiceWidget->setSignBeforSelectNotShow(beforSelectTemplete);
			TextWidget* beforNotSelectTemplete = (TextWidget*)ContentDataFactory::loadData(choiceWidget->getWriterMgr(), iOTools, choiceWidget, 0);
			childMgr->delChild(beforNotSelectTemplete, false);
			choiceWidget->setSignBeforNotSelectNotShow(beforNotSelectTemplete);
		}
		else
		{
			choiceWidget->setSignBeforSelectNotShow(NULL);
			choiceWidget->setSignBeforNotSelectNotShow(NULL);
		}

		int sizechoose = iOTools->readInt();
		for (int i = 0; i < sizechoose; i++)
		{
			mChooseList.push_back(iOTools->readInt());
		}
		choiceWidget->setSelectItem(mChooseList);
		mTemptStatus = iOTools->readInt();
		mTitlePosition = iOTools->readInt();
		choiceWidget->setTitlePosition(mTitlePosition);
		//if (mTitlePosition == ChoiceWidgetTemplete::DISVISABLE)
		{
			TextWidget* titleTemplete = (TextWidget*)ContentDataFactory::loadData(choiceWidget->getWriterMgr(), iOTools, choiceWidget, 0);
			childMgr->delChild(titleTemplete, false);
			choiceWidget->setTitleOnNotShow(titleTemplete);
			choiceWidget->generatSignHaveOrNotViewObj(true);

		}
		int optionSize = iOTools->readInt();
		QList<TextWidget*>* optionTempleteList = new QList<TextWidget*>();
		
		for (int i = 0; i < optionSize; i++)
		{
			TextWidget * option = (TextWidget*)ContentDataFactory::loadData(choiceWidget->getWriterMgr(), iOTools, choiceWidget, 0);
			childMgr->delChild(option, false);
			optionTempleteList->push_back(option);
		}
		choiceWidget->setOptionsOnNotShow(optionTempleteList);
		choiceWidget->initContentData(this);
		choiceWidget->setIsShowed(true);
		setChoiceTempleteOwner(choiceWidget);
		//----------------------------------------------------------------------	
		
	}

	if (mNeedLoadChildren)
	{
		loadChildrenData(iOTools);
		BaseContainer * fistObj = choiceWidget->getChildren()->at(0);
		BaseContainer * lastObj = choiceWidget->getChildren()->at(choiceWidget->getChildren()->size()-1);
		choiceWidget->setSignBefor((ViewObj*)fistObj);
		choiceWidget->setSignBehind((ViewObj*)lastObj);
	}
	if (isAddToParenTemplate)
	{
		tempParent->getChildrenMgr()->pushChild(mOwner, true);
	}
	return mOwner;
}
