#include "MacroTempleteParam.h"
#include "ConstantsID.h"
#include "BaseTemplate.h"
#include "MacroTemplete.h"
#include "TempleteFactory.h"
#include "BaseIOTools.h"
#include "ViewObj.h"
#include "WriterManager.h"
#include "BaseChildrenManager.h"
MacroTempleteParam::MacroTempleteParam(BaseTemplate * templeteOwner, CodeParams * codeParams) : CodeTempData(templeteOwner, ConstantsID::ClassId::MACRO_DATA_PARAM, codeParams)
, mTempleteOwner((MacroTemplete*)templeteOwner)
{
	mId = -1;
}
MacroTempleteParam::~MacroTempleteParam()
{
	if (mTempleteOwner)
	{
		mTempleteOwner = NULL;
	}
}
void MacroTempleteParam::setMcrodTempleteOwner(MacroTemplete * templeteOwner)
{
	mTempleteOwner = templeteOwner;
	mOwner = templeteOwner;
}
void MacroTempleteParam::saveSelfData(BaseIOTools * ioTools)
{
	//return;
	mOwnerObjId = mTempleteOwner->getObjID();
	if (mOwnerObjId == ConstantsID::ContainerId::MACRO_TEMPLETE_OBJ)
	{
		ioTools->writeInt(mTempleteOwner->getId());
	}

}

void MacroTempleteParam::loadSelfData(BaseIOTools * ioTools)
{
	//return;
	int i = 200;
	if (mOwnerObjId == ConstantsID::ContainerId::MACRO_TEMPLETE_OBJ)
	{
		mId = ioTools->readInt();
	}

}
BaseContainer * MacroTempleteParam::createOwner(BaseContainer * tempParent1, BaseIOTools *iOTools, bool isAddToParenTemplate)
{
	BaseTemplate* tempParent = (BaseTemplate*) tempParent1;
	//int userID = tempParent1->getUserID();
	MacroTemplete * menStruationWidget;
	if (mOwnerObjId == ConstantsID::ContainerId::MACRO_TEMPLETE_OBJ)
	{

		menStruationWidget = TempleteFactory::createMacroTemplete(tempParent, NULL, true);
		menStruationWidget->initContentData(this);
		setMcrodTempleteOwner(menStruationWidget);
		//choiceWidget->initWidget();
		menStruationWidget->setId(mId);
	
		if (mNeedLoadChildren)
		{
			//for (int i = 0; i < mChildrenDataList->size(); i++)
			//{
			//	BaseContainerData * data = mChildrenDataList->at(i);
			//	BaseContainer * obj = data->createOwner((BaseTemplete *)mOwner, viewParent);
			//	//((BaseTemplete *)mOwner)->addTempleteChild(obj, -1, false, false);
			//}
			loadChildrenData(iOTools);
			BaseContainer * fistContainer = mOwner->getChild(0);
			BaseContainer * secondContainer = mOwner->getChild(1);
			menStruationWidget->setTitle((TextWidget*)fistContainer);
			menStruationWidget->setContent((BracketWidget*)secondContainer);
			WriterManager* writeManget = tempParent->getWriterMgr();
			if (writeManget){
				writeManget->assignmentMacro(menStruationWidget,true,false);
			}
		}
		if (isAddToParenTemplate)
		{
			tempParent->getChildrenMgr()->pushChild(menStruationWidget, true);
		}

		return mOwner;
	}
	return NULL;
}
