#include "MenStruationDataParam.h"
#include "ConstantsID.h"
#include "BaseTemplate.h"
#include "BaseIOTools.h"
#include "MenstruationTemplete.h"
#include "TextWidget.h"
#include "TempleteFactory.h"
#include "ContentDataFactory.h"
#include "StrContentData.h"
#include "BaseChildrenManager.h"
MenStruationDataParam::MenStruationDataParam(BaseTemplate * templeteOwner, CodeParams * codeParams) : CodeTempData(templeteOwner
	, ConstantsID::ClassId::MENSTRUATION_DATA_PARAM, codeParams)
, mTempleteOwner((MenstruationTemplete*)templeteOwner)
{
	setNeedLoadSelfChildren(false);
}


MenStruationDataParam::~MenStruationDataParam()
{
	if (mTempleteOwner)
	{
		mTempleteOwner = NULL;
	}
}
void MenStruationDataParam::setMenstruationTempleteOwner(MenstruationTemplete * templeteOwner)
{
	mTempleteOwner = templeteOwner;
	mOwner = templeteOwner;
}
void MenStruationDataParam::saveSelfData(BaseIOTools * ioTools)
{
	//return;
	mOwnerObjId = mTempleteOwner->getObjID();
	if (mOwnerObjId == ConstantsID::ContainerId::MENSTRUATION_TEMPLETE_OBJ)
	{
		setNeedLoadSelfChildren(false);
		StrContentData * txtData = mTempleteOwner->getSelfFocusData();
		saveFocusData(txtData, ioTools);

		mTempleteOwner->getLeftTextWidget()->getContentData()->doSave(ioTools);
		mTempleteOwner->getRightTTextWidget()->getContentData()->doSave(ioTools);
		mTempleteOwner->getUpTextWidget()->getContentData()->doSave(ioTools);
		mTempleteOwner->getDownTextWidget()->getContentData()->doSave(ioTools);
	}

}

void MenStruationDataParam::loadSelfData(BaseIOTools * ioTools)
{
	//return;
	if (mOwnerObjId == ConstantsID::ContainerId::MENSTRUATION_TEMPLETE_OBJ)
	{
		setNeedLoadSelfChildren(false);
		//loadFocusData();
		mFocusTxtData = loadFocusData(ioTools);
		
	}

}
BaseContainer * MenStruationDataParam::createOwner(BaseContainer * tempParent, BaseIOTools *iOTools, bool isAddToParenTemplate)
{
	BaseTemplate* tempParent1 = (BaseTemplate*)tempParent;
	MenstruationTemplete * menStruationWidget;
	if (mOwnerObjId == ConstantsID::ContainerId::MENSTRUATION_TEMPLETE_OBJ)
	{
		
		menStruationWidget = TempleteFactory::createMenstruationTemplete(tempParent1, true);
		
		menStruationWidget->initContentData(this);
		menStruationWidget->setFocusData(mFocusTxtData);
		mFocusTxtData->initPainterAndFont(tempParent1);
		setMenstruationTempleteOwner(menStruationWidget);
		StrContentData* focusData = mFocusTxtData;
		/*QFont *  font = focusData->getFont();
		int fontSize = font->pointSize();*/
		int fontSize = focusData->getPointSize();
		int sizeChange = fontSize / MenstruationTemplete::mTxtRio;
		focusData->setPointSize(sizeChange);
		//choiceWidget->initWidget();
		ContentDataFactory::loadData(menStruationWidget->getWriterMgr(), iOTools, menStruationWidget);
		ContentDataFactory::loadData(menStruationWidget->getWriterMgr(), iOTools, menStruationWidget);
		ContentDataFactory::loadData(menStruationWidget->getWriterMgr(), iOTools, menStruationWidget);
		ContentDataFactory::loadData(menStruationWidget->getWriterMgr(), iOTools, menStruationWidget);
		QList<BaseContainer *> * children =   menStruationWidget->getChildren();
		TextWidget*  leftTemplete = (TextWidget*)children->at(1);
		TextWidget*  rightTemplete = (TextWidget*)children->at(2);
		TextWidget*  upTemplete = (TextWidget*)children->at(3);
		TextWidget*  downTemplete = (TextWidget*)children->at(4);

		menStruationWidget->getChildrenMgr()->delChildren(1,4,true);

		
		leftTemplete->setTxtPointSize(sizeChange);
		rightTemplete->setTxtPointSize(sizeChange);
		upTemplete->setTxtPointSize(sizeChange);
		downTemplete->setTxtPointSize(sizeChange);
		//TextWidget* leftTemplete = (TextWidget*)(mLeftTextWidgetData)->createOwner(menStruationWidget, viewParent);
		//menStruationWidget->delChild(leftTemplete, false);
		//leftTemplete->setTempleteParent(menStruationWidget->getParagraphTemplete());
		////leftTemplete->setTxtData(mFocusTxtData);

		//TextWidget* rightTemplete = (TextWidget*)(mRightTextWidgetData)->createOwner(menStruationWidget, viewParent);
		//menStruationWidget->delChild(rightTemplete, false);
		//rightTemplete->setTempleteParent(menStruationWidget->getParagraphTemplete());
		////rightTemplete->setTxtData(mFocusTxtData);

		//TextWidget* upTemplete = (TextWidget*)(mUpTextWidgetData)->createOwner(menStruationWidget, viewParent);
		//menStruationWidget->delChild(upTemplete, false);
		//upTemplete->setTempleteParent(menStruationWidget->getParagraphTemplete());
		////upTemplete->setTxtData(mFocusTxtData);

		//TextWidget* downTemplete = (TextWidget*)(mDownTextWidgetData)->createOwner(menStruationWidget, viewParent);
		//menStruationWidget->delChild(downTemplete, false);
		//downTemplete->setTempleteParent(menStruationWidget->getParagraphTemplete());
		////downTemplete->setTxtData(mFocusTxtData);
		
		menStruationWidget->setLeftTextWidget(leftTemplete);
		menStruationWidget->setRightTTextWidget(rightTemplete);
		menStruationWidget->setUpTextWidget(upTemplete);
		menStruationWidget->setDownTextWidget(downTemplete);
		menStruationWidget->initTxtWidgetChooseId();
		menStruationWidget->setChildTxtParam(focusData);
		focusData->setPointSize(fontSize);
		menStruationWidget->addTextObjToRowContainer(false);
		if (isAddToParenTemplate)
		{
			tempParent->getChildrenMgr()->pushChild(menStruationWidget, true);
		}
		return mOwner;
	}
	return NULL;
}
