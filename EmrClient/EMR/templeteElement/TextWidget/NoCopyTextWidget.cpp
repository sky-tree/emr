#include "NoCopyTextWidget.h"
#include "TextWidget.h"
#include "TempleteFactory.h"
#include "BaseChildrenManager.h"
#include "ConstantsID.h"
#include "TextWidgetMouse.h"
#include "InputFactory.h"
#include "TextWidgetKeyboard.h"
#include "TextWidgetParam.h"
#include "TempleteTypeEnumeration.h"
#include "ViewObj.h"
#include "BaseConstants.h"
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
NoCopyTextWidget::NoCopyTextWidget(WriterManager * wMgr, BaseContainer * templeteParent, int objId, bool isCreateStrTxtData) : TextWidget(wMgr
	, ConstantsID::ClassId::NOCOPYTEXTWIDGET, objId, BaseConstants::NONE_STR, isCreateStrTxtData)
{
	
}
NoCopyTextWidget::NoCopyTextWidget(WriterManager * wMgr, NoCopyTextWidget * otherBracket) : TextWidget(wMgr, otherBracket, false)
{
	//getOperateMgr()->setCanBeChoosenObjId(otherBracket->getOperateMgr()->getCanBeChoosenID());
	//getOperateMgr()->setCanAddObjParentId(otherBracket->getOperateMgr()->getCanAddObjParentId());
	TextWidgetMouse * textMouse = InputFactory::createTextWidgetMouse(this);
	setMouse(textMouse);
	TextWidgetKeyboard* keyboard = InputFactory::createTextWidgetKeyboard(this);
	setKeyboard(keyboard);
	mDefaultString = otherBracket->mDefaultString;
	mStr = otherBracket->mStr;
	copyStrList(wMgr, otherBracket->mDefaultObjList, mDefaultObjList);
	copyStrList(wMgr, otherBracket->mStrObjList, mStrObjList);
	mChildrenMgr = new BaseChildrenManager(this);
	addAllStrObjToChild();
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
bool NoCopyTextWidget::setTempleteProperty(QMap<QString, QString>& setMap)
{
	bool isSetCodeSuccess = setCodeIdToTemplete(setMap);
	QString content = setMap.value(TempleteTypeEnumeration::CONTENT);
	QString fixName = setMap.value(TempleteTypeEnumeration::NAME);
	if (content.length() == 0)
	{
		content = "¹Ì¶¨¹«Ê½";
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
	setStrOnShowTime(content);
	//setStr(content);
	//getLastChildView()->setFocus(true,false);
	return true;
}