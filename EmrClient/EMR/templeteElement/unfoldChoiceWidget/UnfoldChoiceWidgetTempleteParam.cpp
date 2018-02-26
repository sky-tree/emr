#include "UnfoldChoiceWidgetTempleteParam.h"
#include "UnfoldChoiceWidgetTemplete.h"
#include "TempleteFactory.h"
#include "StrContentData.h"
#include "BaseIOTools.h"
#include "ContentDataFactory.h"
#include "TextWidgetParam.h"
#include "ChoiceItemTextWidget.h"
#include "ConstantsID.h"
#include "BaseChildrenManager.h"
#include "OperateMgr.h"
UnfoldChoiceWidgetTempleteParam::UnfoldChoiceWidgetTempleteParam(BaseTemplate * templeteOwner, CodeParams * codeParams) : CodeTempData(templeteOwner
	, ConstantsID::ClassId::UNFOID_CHOICEAA_WIDGET_PARAM, codeParams)
	, mTempleteOwner((UnfoldChoiceWidgetTemplete*)templeteOwner)
//, mBeforSelectData(NULL)
//, mBeforNotSelectData(NULL)
{
	setNeedLoadSelfChildren(false);
}


UnfoldChoiceWidgetTempleteParam::~UnfoldChoiceWidgetTempleteParam()
{
	if (mTempleteOwner)
	{
		mTempleteOwner = NULL;
	}
}
void UnfoldChoiceWidgetTempleteParam::setChoiceTempleteOwner(UnfoldChoiceWidgetTemplete * templeteOwner)
{
	mTempleteOwner = templeteOwner;
	mOwner = templeteOwner;
}
void UnfoldChoiceWidgetTempleteParam::saveSelfData(BaseIOTools * ioTools)
{
	//return;
	mOwnerObjId = mTempleteOwner->getObjID();
	if (mOwnerObjId == ConstantsID::ContainerId::UNFOLD_CHOICE_TEMPLETE)
	{
		bool canInputOnKeyboard = false;
		ioTools->writeBool(canInputOnKeyboard);
		StrContentData * txtData = mTempleteOwner->getFocusData();
		saveFocusData(txtData, ioTools);
		int wedgitType = mTempleteOwner->getChoiceType();
		ioTools->writeInt(wedgitType);
		
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
		QString titleStr = mTempleteOwner->getTitleStr();
		ioTools->writeString(titleStr);
		QList<ChoiceItemTextWidget*>* optionList = mTempleteOwner->getOptionList();
		int optionSize = 0;
		if (optionList)
		{
			optionSize=optionList->size();
		}
		ioTools->writeInt(optionSize);
		ChoiceItemTextWidget* tempOption;
		for (int i = 0; i < optionSize; i++)
		{
			tempOption = optionList->at(i);
			ioTools->writeInt(tempOption->getPaddingLeft());
			ioTools->writeString(tempOption->getTitle());
		}
		ioTools->writeInt(mTempleteOwner->getPaddingLeft());
	}

}

void UnfoldChoiceWidgetTempleteParam::loadSelfData(BaseIOTools * ioTools)
{
	//return;
	if (mOwnerObjId == ConstantsID::ContainerId::UNFOLD_CHOICE_TEMPLETE)
	{
		mCanInputOnKeyBoard = ioTools->readBool();
		mFocusTxtData = loadFocusData(ioTools);
		mWidgetType = ioTools->readInt();
		
	}

}
BaseContainer * UnfoldChoiceWidgetTempleteParam::createOwner(BaseContainer * tempParent1, BaseIOTools *iOTools, bool isAddToParenTemplate)
{
	BaseTemplate* tempParent = (BaseTemplate*)tempParent1;
	UnfoldChoiceWidgetTemplete * choiceWidget;
	if (mOwnerObjId == ConstantsID::ContainerId::UNFOLD_CHOICE_TEMPLETE)
	{
		choiceWidget = TempleteFactory::createUnfoldChoiceWidget(tempParent, mWidgetType, true);
		choiceWidget->setTempleteStatus(mTemptStatus);
		
		//choiceWidget->setWidgetType(mWidgetType);
		//choiceWidget->setCanInputOnKeyBoard(mCanInputOnKeyBoard);
		choiceWidget->setFocusData(mFocusTxtData);
		mFocusTxtData->initOwner(tempParent);
		mFocusTxtData->initPainterAndFont(tempParent);
		//choiceWidget->initWidget();
		BaseChildrenManager *  childMgr = choiceWidget->getChildrenMgr();
		//读取数据

		int sizechoose = iOTools->readInt();
		QList<int>  chooseList;
		for (int i = 0; i < sizechoose; i++)
		{
			chooseList.push_back(iOTools->readInt());
		}
		choiceWidget->setSelectList(chooseList);
		mTemptStatus = iOTools->readInt();
		mTitlePosition = iOTools->readInt();
		
		QString titleStr = iOTools->readString();

		int optionSize = iOTools->readInt();
		QList<QString> strList;
		QList<int> paddingLeftList;
		for (int i = 0; i < optionSize; i++)
		{
		
			if (ContentDataFactory::isSaveUnfoldChoicePaddingLeft)
			{
				paddingLeftList.push_back(iOTools->readInt());
			}
			else
			{
				paddingLeftList.push_back(0);
			}
			strList.push_back(iOTools->readString());
		}
		int wedgitPaddingLeft = 0;
		if (tempParent1->getOperateMgr()->getVersionInt()>=4)
		{
			wedgitPaddingLeft = iOTools->readInt();
		}
		
		setChoiceTempleteOwner(choiceWidget);

		choiceWidget->initWedgitOnLoad(this, mTitlePosition, titleStr, strList, paddingLeftList, wedgitPaddingLeft);
		//----------------------------------------------------------------------	
		
	}


	if (isAddToParenTemplate)
	{
		tempParent->getChildrenMgr()->pushChild(mOwner, true);
	}
	return mOwner;
}
