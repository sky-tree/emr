#include "TextWidgetParam.h"
#include "ConstantsID.h"


#include "TempleteFactory.h"
#include "TextWidget.h"
#include "BaseTemplate.h"
#include "BaseIOTools.h"
#include "NumberInputFilter.h"
#include "BaseChildrenManager.h"
#include "StrContentData.h"
#include "ViewObj.h"
#include "UIConstants.h"
#include "BracketWidget.h"
TextWidgetParam::TextWidgetParam(BaseTemplate * templeteOwner, CodeParams * codeParams) : CodeTempData(templeteOwner, ConstantsID::ClassId::TEXT_WIDGET_PARAM, codeParams)
, mTempleteOwner((TextWidget*)templeteOwner)
, mFocusTxtData(NULL)
{
	//setNeedLoadSelfChildren(false);
}


TextWidgetParam::~TextWidgetParam()
{
	if (mTempleteOwner)
	{
		mTempleteOwner = NULL;
	}
	if (mFocusTxtData)
	{
		mFocusTxtData = NULL;
	}
}
void TextWidgetParam::setTextTempleteOwner(TextWidget * templeteOwner)
{
	mTempleteOwner = templeteOwner;
	mOwner = templeteOwner;
}

void TextWidgetParam::saveStrAndDefaultStr(BaseIOTools * ioTools)
{
	QString ttring = mTempleteOwner->getString();
	int sizeOfStr = ttring.length();
	ioTools->writeInt(sizeOfStr);
	if (sizeOfStr >0){
		ioTools->writeString(ttring);
	}
	QString defaultString = mTempleteOwner->getDefaultString();
	int defaultSize = defaultString.length();
	ioTools->writeInt(defaultSize);
	if (defaultSize >0){
		ioTools->writeString(defaultString);
	}
}
void TextWidgetParam::loadStrAndDefaultStr(BaseIOTools * ioTools)
{
	int sizeOfStr = ioTools->readInt();
	if (sizeOfStr>0){
		mStr = ioTools->readString();
	}
	int sizeOfDefault = ioTools->readInt();
	if (sizeOfDefault>0){
		mDefaultString = ioTools->readString();
	}
}
void TextWidgetParam::saveSelfData(BaseIOTools * ioTools)
{
	//return;
	mOwnerObjId = mTempleteOwner->getObjID();
	if (mOwnerObjId == ConstantsID::ContainerId::TIME_TEXT_WIDGET_TEMPLETE || mOwnerObjId == ConstantsID::ContainerId::FIX_TEXT_WIDGET_TEMPLETE
		 || mOwnerObjId == ConstantsID::ContainerId::MACRO_TEXT_TEMPLETE_OBJ)
	{
		StrContentData * txtData = mTempleteOwner->getSelfFocusData();
		saveFocusData(txtData, ioTools);
	 
		saveStrAndDefaultStr(ioTools);
	// //时间控件的子对象每一个字状态时相同的，所以删除掉子对象，不用保存单个子对象的属性
	 setNeedLoadSelfChildren(false);
	
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::MENSTRUATION_TEXT_TEMPLETE_OBJ)
	{
		saveStrAndDefaultStr(ioTools);
		setNeedLoadSelfChildren(false);
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::TEXT_WIDGET_TEMPLETE)
	{
		StrContentData * txtData = mTempleteOwner->getSelfFocusData();
		saveFocusData(txtData, ioTools);
		QString ttring = mTempleteOwner->getString();
		int sizeOfStr = ttring.length();
		ioTools->writeInt(sizeOfStr);
		if (sizeOfStr >0)
		{
			ioTools->writeString(ttring);
		}
		else
		{
			setNeedLoadSelfChildren(false);
		}

		QString defaultString = mTempleteOwner->getDefaultString();
		int defaultSize = defaultString.length();
		ioTools->writeInt(defaultSize);
		if (defaultSize >0)
		{
			ioTools->writeString(defaultString);
		}
	}
	else if ( mOwnerObjId == ConstantsID::ContainerId::CHOICE_TEXT_TEMPLETE_OBJ)
	{
		
		QString ttring = mTempleteOwner->getString();
		int sizeOfStr = ttring.length();
		ioTools->writeInt(sizeOfStr);
		if (sizeOfStr >0)
		{
			ioTools->writeString(ttring);
		}
		else
		{
			setNeedLoadSelfChildren(false);
		}

		QString defaultString = mTempleteOwner->getDefaultString();
		int defaultSize = defaultString.length();
		ioTools->writeInt(defaultSize);
		if (defaultSize >0)
		{
			ioTools->writeString(defaultString);
		}
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::TEXT_WIDGET_AND_NUMBER_FILTER_TEMPLETE)
	{
		StrContentData * txtData = mTempleteOwner->getSelfFocusData();
		saveFocusData(txtData, ioTools);
		saveStrAndDefaultStr(ioTools);
		NumberInputFilter * numberFitlter = (NumberInputFilter*)mTempleteOwner->mInputFilter;
		ioTools->writeInt(numberFitlter->getMaxSize());
		ioTools->writeInt(numberFitlter->getDecimalPointLenght());
		setNeedLoadSelfChildren(false);
	}
	else if ( mOwnerObjId == ConstantsID::ContainerId::INPUT_TEXT_TEMPLETE)
	{
		StrContentData * txtData = mTempleteOwner->getSelfFocusData();
		saveFocusData(txtData, ioTools);
		
		saveStrAndDefaultStr(ioTools);
		
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::BRAKETWIDGET_TEMPLETE)
	{
		StrContentData * txtData = mTempleteOwner->getSelfFocusData();
		saveFocusData(txtData, ioTools);
		saveStrAndDefaultStr(ioTools);
		setNeedLoadSelfChildren(false);
	}
	ioTools->writeInt(mTempleteOwner->getTemptStatus());
	if (mTempleteOwner->mNameStr.size()>0)
	{
		ioTools->writeBool(true);
		ioTools->writeString(mTempleteOwner->mNameStr);
	}
	else
	{
		ioTools->writeBool(false);
	}
}

void TextWidgetParam::loadSelfData(BaseIOTools * ioTools)
{
	//return;
	if (mOwnerObjId == ConstantsID::ContainerId::TIME_TEXT_WIDGET_TEMPLETE || mOwnerObjId == ConstantsID::ContainerId::FIX_TEXT_WIDGET_TEMPLETE
		|| mOwnerObjId == ConstantsID::ContainerId::MACRO_TEXT_TEMPLETE_OBJ)
	{
		mFocusTxtData = loadFocusData(ioTools);
		
		loadStrAndDefaultStr(ioTools);
		setNeedLoadSelfChildren(false);
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::MENSTRUATION_TEXT_TEMPLETE_OBJ)
	{
		loadStrAndDefaultStr(ioTools);
		setNeedLoadSelfChildren(false);
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::TEXT_WIDGET_TEMPLETE )
	{
		mFocusTxtData = loadFocusData(ioTools);
		int sizeOfStr = ioTools->readInt();
		if (sizeOfStr>0){
			mStr = ioTools->readString();
		}
		else{
			setNeedLoadSelfChildren(false);
		}
		int sizeOfDefault = ioTools->readInt();
		if (sizeOfDefault>0){
			mDefaultString = ioTools->readString();
		}
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::CHOICE_TEXT_TEMPLETE_OBJ)
	{
		int sizeOfStr = ioTools->readInt();
		if (sizeOfStr>0){
			mStr = ioTools->readString();
		}
		else{
			setNeedLoadSelfChildren(false);
		}
		int sizeOfDefault = ioTools->readInt();
		if (sizeOfDefault>0){
			mDefaultString = ioTools->readString();
		}
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::TEXT_WIDGET_AND_NUMBER_FILTER_TEMPLETE)
	{
		mFocusTxtData = loadFocusData(ioTools);
		loadStrAndDefaultStr(ioTools);
		mStrMaxSize = ioTools->readInt();
		mDecimalPointLenght = ioTools->readInt();
		setNeedLoadSelfChildren(false);
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::INPUT_TEXT_TEMPLETE)
	{
		mFocusTxtData = loadFocusData(ioTools);
		
		loadStrAndDefaultStr(ioTools);
		setNeedLoadSelfChildren(true);
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::BRAKETWIDGET_TEMPLETE)
	{
		mFocusTxtData = loadFocusData(ioTools);
		loadStrAndDefaultStr(ioTools);
		setNeedLoadSelfChildren(false);
	}
	mTemptStatus = ioTools->readInt();
	if (ioTools->readBool())
	{
		mNameStr = ioTools->readString();
	}
}
BaseContainer * TextWidgetParam::createOwner(BaseContainer * tempParent, BaseIOTools *iOTools, bool isAddToParenTemplate)
{
	BaseTemplate*	tempParent1 = (BaseTemplate*)tempParent;
	TextWidget * temptTextWidget;
	if (mFocusTxtData)
	{
		mFocusTxtData->initPainterAndFont(tempParent1);
	}
	bool isNeedAddChildrenToStrList = true;
	if (mOwnerObjId == ConstantsID::ContainerId::TIME_TEXT_WIDGET_TEMPLETE || mOwnerObjId == ConstantsID::ContainerId::MENSTRUATION_TEXT_TEMPLETE_OBJ)
	{
		if (mOwnerObjId == ConstantsID::ContainerId::TIME_TEXT_WIDGET_TEMPLETE)
		{
			temptTextWidget = TempleteFactory::createNoEditTextWidgetTemplete(tempParent1, NULL, -1, true, ConstantsID::ContainerId::TIME_TEXT_WIDGET_TEMPLETE, "7");
			temptTextWidget->setTxtData(mFocusTxtData);
		}
		else if (mOwnerObjId == ConstantsID::ContainerId::MENSTRUATION_TEXT_TEMPLETE_OBJ)
		{
			temptTextWidget = TempleteFactory::createNoEditTextWidgetTemplete(tempParent1, NULL, -1, true, ConstantsID::ContainerId::MENSTRUATION_TEXT_TEMPLETE_OBJ, "9", true);
			temptTextWidget->getSelfFocusData()->refreshTxtParamBySourceParam(tempParent1->getSelfFocusData(),false);
		}
		temptTextWidget->setChooseObjId(mChooseObjId);
	
		temptTextWidget->initWidget(mDefaultString);
		temptTextWidget->setStr(mStr);
		temptTextWidget->initContentData(this);
		temptTextWidget->setTempleteStatus(mTemptStatus);
		setTextTempleteOwner(temptTextWidget);
		mOwner = temptTextWidget;
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::FIX_TEXT_WIDGET_TEMPLETE || mOwnerObjId == ConstantsID::ContainerId::MACRO_TEXT_TEMPLETE_OBJ)
	{
		if (mOwnerObjId == ConstantsID::ContainerId::FIX_TEXT_WIDGET_TEMPLETE)
		{
			temptTextWidget = TempleteFactory::createNoEditTextWidgetTemplete(tempParent1, NULL, -1, true, ConstantsID::ContainerId::FIX_TEXT_WIDGET_TEMPLETE, "");
		}
		else if (mOwnerObjId == ConstantsID::ContainerId::MACRO_TEXT_TEMPLETE_OBJ)
		{
			temptTextWidget = TempleteFactory::createMacroTextWidgetTemplete(tempParent1, true);
		}
		temptTextWidget->setChooseObjId(mChooseObjId);
		temptTextWidget->setTxtData(mFocusTxtData);
		temptTextWidget->initWidget(mDefaultString);
		temptTextWidget->setStr(mStr);
		temptTextWidget->initContentData(this);
		temptTextWidget->setTempleteStatus(mTemptStatus);
		setTextTempleteOwner(temptTextWidget);
		mOwner = temptTextWidget;
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::TEXT_WIDGET_TEMPLETE )
	{
		if (mOwnerObjId == ConstantsID::ContainerId::TEXT_WIDGET_TEMPLETE)
		{
			temptTextWidget = TempleteFactory::createTextWidgetTemplete(tempParent1, true);
			temptTextWidget->setChooseObjId(mChooseObjId);
			temptTextWidget->setTxtData(mFocusTxtData);
		}
		
		temptTextWidget->initWidget(mDefaultString);
		temptTextWidget->setOnlyStr(mStr);
		temptTextWidget->initContentData(this);
		temptTextWidget->setTempleteStatus(mTemptStatus);
		setTextTempleteOwner(temptTextWidget);
		mOwner = temptTextWidget;
	}
	else if ( mOwnerObjId == ConstantsID::ContainerId::CHOICE_TEXT_TEMPLETE_OBJ)
	{
		isNeedAddChildrenToStrList = false;
			temptTextWidget = TempleteFactory::createChoiceTextWidgetTemplete(tempParent1, true);
			temptTextWidget->setChooseObjId(mChooseObjId);
			StrContentData* focusData = tempParent1->getSelfFocusData();
			StrContentData* textWidgetFocusTxtData = new StrContentData(temptTextWidget, NULL, focusData->getString(), focusData);
			temptTextWidget->setTxtData(textWidgetFocusTxtData);
	
		temptTextWidget->initWidget(mDefaultString);
		temptTextWidget->setOnlyStr(mStr);
		temptTextWidget->initContentData(this);
		temptTextWidget->setTempleteStatus(mTemptStatus);
		setTextTempleteOwner(temptTextWidget);
		mOwner = temptTextWidget;
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::TEXT_WIDGET_AND_NUMBER_FILTER_TEMPLETE)
	{
		/*temptTextWidget = TempleteFactory::createNumberAndFilterWidgetTemplete(tempParent1, viewParent, true);
		NumberInputFilter * numberFitlter = (NumberInputFilter*)temptTextWidget->mInputFilter;
		numberFitlter->initFilter(mStrMaxSize, mDecimalPointLenght);
		temptTextWidget->setTxtData(mFocusTxtData);
		temptTextWidget->initWidget(mDefaultString);
		temptTextWidget->setStr(mStr);
		temptTextWidget->setContentData(this);
		temptTextWidget->setTempleteStatus(mTemptStatus);
		setTextTempleteOwner(temptTextWidget);
		mOwner = temptTextWidget;*/
	}
	else if ( mOwnerObjId == ConstantsID::ContainerId::INPUT_TEXT_TEMPLETE)
	{
		BracketWidget * temptTextWidget1;
		/*if (mOwnerObjId == ConstantsID::ContainerId::BRAKETWIDGET_TEMPLETE)
		{
			temptTextWidget1 = TempleteFactory::createBracketTemplete(tempParent1, true);
		}
		else*/
		{
			temptTextWidget1 = TempleteFactory::createInputTextTemplete(tempParent1, true);
			temptTextWidget1->setChooseObjId(mChooseObjId);
		}
		
		temptTextWidget1->setTxtData(mFocusTxtData);

		if (mStr >0)
		{
			mDefaultString.remove(0, 1);
			mDefaultString.remove(mDefaultString.length() - 1, 1);
			temptTextWidget1->setHint(mDefaultString,false);
		}
		else
		{
			temptTextWidget1->mDefaultString = mDefaultString;
		}
	
		temptTextWidget1->setOnlyStr(mStr);
		temptTextWidget1->setTempleteStatus(mTemptStatus);
		temptTextWidget1->initContentData(this);
		setTextTempleteOwner(temptTextWidget1);
		mOwner = temptTextWidget1;
	}
	else if (mOwnerObjId == ConstantsID::ContainerId::BRAKETWIDGET_TEMPLETE)
	{
		BracketWidget * temptTextWidget1;
			temptTextWidget1 = TempleteFactory::createBracketTemplete(tempParent1, true);
			temptTextWidget1->setChooseObjId(mChooseObjId);
		temptTextWidget1->setTxtData(mFocusTxtData);
		temptTextWidget1->setStr(mStr);
		temptTextWidget1->setTempleteStatus(mTemptStatus);
		temptTextWidget1->initContentData(this);
		setTextTempleteOwner(temptTextWidget1);
		mOwner = temptTextWidget1;
	}
	if (mNameStr.size()>0)
	{
		mTempleteOwner->mNameStr = mNameStr;
	}
	if (mNeedLoadChildren)
	{
		/*for (int i = 0; i < mChildrenDataList->size(); i++)
		{
			BaseContainerData * data = mChildrenDataList->at(i);
			BaseContainer * obj = data->createOwner((BaseTemplete *)mOwner, viewParent);
			
		}*/
		loadChildrenData(iOTools);
		if (mStr.size()>0)
		{
			//if (isNeedAddChildrenToStrList)
			{
				QList<ViewObj *> * strList = (QList<ViewObj *> *)mOwner->getChildren();
				if (strList&&strList->size()>0)
				{
					((TextWidget *)mOwner)->addStrObjListToStringObjList(strList);
				}
				
			}
		}
		else
		{
			QList<ViewObj *> * children = (QList<ViewObj *> *)mOwner->getChildren();
			((TextWidget *)mOwner)->addStrObjListToDefaultStringObjList(children);
			if (mOwnerObjId == ConstantsID::ContainerId::INPUT_TEXT_TEMPLETE)
			{
				ViewObj * obj;
				//mFocusTxtData->mOprateType = SetTxtType::FontColor;
				//int fontColor = mFocusTxtData->getFontColor();
				//mFocusTxtData->setFontColor(UIConstants::MyColor::GRAYER);
				int size = children->size() - 1;
				for (int i = 1; i < size; i++)
				{
					obj = children->at(i);
					StrContentData * txtParam = (StrContentData *)obj->getContentData();
					txtParam->setNeedGray(true);
				}
				//mFocusTxtData->setFontColor(fontColor);
			}
		}
		
	}
	if (isAddToParenTemplate)
	{
		tempParent1->getChildrenMgr()->pushChild(mOwner, true);
	}
	return mOwner;
}
