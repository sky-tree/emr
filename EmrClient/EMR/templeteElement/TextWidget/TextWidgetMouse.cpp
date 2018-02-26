#include "TextWidgetMouse.h"
#include "TextWidget.h"
#include "BracketWidget.h"
#include "ViewObj.h"
#include "qDebug.h"
#include "TempleteTypeEnumeration.h"
#include "FlagParam.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "StrContentData.h"
#include "QFont.h"
//#include "BaseWinData.h"
//#include "ReplaceNeedReplaceListAction.h"
#include "SetTxtWidgetElementAction.h"
TextWidgetMouse::TextWidgetMouse(BaseTemplate* templeteOwner, int classid) :BaseTempleteMouse(templeteOwner, classid)
, mTempleteOwner((TextWidget*)templeteOwner)
{
}


TextWidgetMouse::~TextWidgetMouse()
{
}
int TextWidgetMouse::mousePress(int type, int x, int y, BaseElement * obj)
{
	if (type == Qt::RightButton)
	{
		if (mTempleteOwner->getObjID() == ConstantsID::ContainerId::FIX_TEXT_WIDGET_TEMPLETE)
		{
			fixTextHandleRightMouse();
			return 1;
		}
		else if (mTempleteOwner->getObjID() == ConstantsID::ContainerId::TIME_TEXT_WIDGET_TEMPLETE)
		{
			timeTextHandeRightMouse();
			return 1;
		}
		else if (mTempleteOwner->getObjID() == ConstantsID::ContainerId::TEXT_WIDGET_AND_NUMBER_FILTER_TEMPLETE)
		{
			numberFIlterHandleRightMouse();
			return 1;
		}
		else if (mTempleteOwner->getObjID() == ConstantsID::ContainerId::INPUT_TEXT_TEMPLETE)
		{
			inputTextHandleRightMouse();
			return 1;
		}
		/*else if (mTempleteOwner->getObjID() == ConstantsID::ContainerId::MACRO_TEXT_TEMPLETE_OBJ)
		{
			mTempleteOwner->getTempParent();
			return 1;
		}*/
		return BaseConstants::NONE;
	}
	int returnType = BaseConstants::NONE;
	
	
	return 0;
}
int TextWidgetMouse::mouseRelease(int type, int x, int y, BaseElement * obj)
{
	//int returnType = BaseConstants::NONE;
	//if (mTempleteOwner->mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS){
	//	return 1;
	//}
	//int ownerClassId = mTempleteOwner->getClassId();
	//if (ownerClassId == ConstantsID::ClassId::TEXT_WIDGET_TEMPLETE)
	//{
	//	if (mTempleteOwner->mStrObjList->size() == 0){
	//		ViewObj *obj = mTempleteOwner->mDefaultObjList->at(0);
	//		obj->setFocus(false, false);
	//		return 1;
	//	}
	//}
	//else if (ownerClassId == ConstantsID::ClassId::BRAKETWIDGET_TEMPLETE)
	//{
	//	BracketWidget * OwnertempleteReal = (BracketWidget*)mTempleteOwner;
	//	ViewObj*focusObj = (ViewObj*)obj;
	//	ViewObj *firstObj = OwnertempleteReal->getFirstChildView();
	//	ViewObj *lastObj = OwnertempleteReal->getLastChildView();
	//	if (mTempleteOwner->mStrObjList->size() == 0)
	//	{
	//		bool isNeedFocus = true;
	//		FlagParam  *mPressEnd = OwnertempleteReal->getPressEnd();
	//		char dir = mPressEnd->getDirection();
	//		if (focusObj == firstObj)
	//		{
	//			//if (dir == BaseConstants::Direction::LEFT)
	//			{
	//				isNeedFocus = false;
	//			}
	//		}
	//		else if (focusObj == lastObj)
	//		{
	//			if (dir == BaseConstants::Direction::RIGHT)
	//			{
	//				isNeedFocus = false;
	//			}
	//		}
	//		if (isNeedFocus){
	//			if (mTempleteOwner->mDefaultObjList->size()>0)
	//			{
	//				ViewObj *obj = mTempleteOwner->mDefaultObjList->at(0);
	//				obj->setFocus(true, false);
	//				//mTempleteOwner->setFocusFlagAtObj(obj);
	//				return 1;
	//			}

	//		}
	//	}
	//	else
	//	{


	//		/*if (focusObj == firstObj)
	//		{
	//			ViewObj* nextObj = focusObj->getNextView();
	//			nextObj->setFocus(false, true);
	//			return 1;
	//		}
	//		else if (focusObj == lastObj)
	//		{
	//			ViewObj* preView = focusObj->getPreView();
	//			preView->setFocus(true, true);
	//			return 1;
	//		}*/
	//	}
	//}
	//((ViewObj*)obj)->setFocus(true, false);
	
	/*if (mHadHandled){
		return 1;
	}
	else{
		return 0;
	}*/
	return 0;
}
int TextWidgetMouse::mouseMove(int type, int x, int y, BaseElement * obj)
{
	int returnType = BaseConstants::NONE;
	/*if (mTempleteOwner->mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS){
		return 1;
	}*/
	return returnType;
}
int TextWidgetMouse::mouseDrag(int type, int x, int y, BaseElement * obj)
{
	/*int returnType = BaseConstants::NONE;
	if ( mTempleteOwner->mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS){
		return 1;
	}
	return returnType;*/
	return 0;
}
int TextWidgetMouse::mouseDoublePress(int type, int x, int y, BaseElement * obj)
{
	int returnType = BaseConstants::NONE;
	/*if (mTempleteOwner->mTemptStatus == BaseConstants::ObjStatus::NONEDIT_STATUS){
		return returnType;
	}*/

	return returnType;
}
int TextWidgetMouse::mouseScroll(int angle, BaseElement * obj)
{
	int returnType = BaseConstants::NONE;

	return returnType;
}
void TextWidgetMouse::fixTextHandleRightMouse()
{
	QMap<QString, QString> map;
	map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::FIXTEXT);
	map.insert(TempleteTypeEnumeration::NAME, mTempleteOwner->getElementName());
	map.insert(TempleteTypeEnumeration::CONTENT, mTempleteOwner->getString());
	mTempleteOwner->addCodeIdToMap(map);
	QMap<QString, QString> beforMap=map;
	bool isDelete = TempleteTypeEnumeration::showDialog(&map, true);
	if (isDelete){
		mTempleteOwner->deleteSelf();
		return ;
	}
	QList<ViewObj *>  defaultObjListBefore;
	QList<ViewObj *> strObjListBefore;
	
	ViewObj* beforFocus = NULL;
	char dir = 0;
	mTempleteOwner->getFocusObjAndDir(beforFocus,dir);
	SetTxtWidgetElementAction::copyObjListToObjList(mTempleteOwner->mDefaultObjList,&defaultObjListBefore, true);
	SetTxtWidgetElementAction::copyObjListToObjList(mTempleteOwner->mStrObjList, &strObjListBefore, true);
	bool isSucceed= mTempleteOwner->setTempleteProperty(map);
	if (isSucceed)
	{
		mTempleteOwner->addSetTempletePropertyAction(beforMap, map, &defaultObjListBefore, &strObjListBefore,beforFocus,dir);
	}
	mTempleteOwner->getLastChildView()->setFocus(true,false);
	mTempleteOwner->replaceNeedReplaceList(1);
}
void TextWidgetMouse::timeTextHandeRightMouse()
{
	QMap<QString, QString> map;
	map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::TIME);
	map.insert(TempleteTypeEnumeration::NAME, mTempleteOwner->getString());
	bool isDelete = TempleteTypeEnumeration::showDialog(&map, true);
	if (isDelete){
		//mTempleteOwner->deleteSelf();
		return;
	}
	QString name = map.value(TempleteTypeEnumeration::NAME);
	mTempleteOwner->setStr(name);
}
void TextWidgetMouse::numberFIlterHandleRightMouse()
{
	QMap<QString, QString> map;
	map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::NUMBER_FILTER);
	map.insert(TempleteTypeEnumeration::NAME, mTempleteOwner->getString());
	bool isDelete = TempleteTypeEnumeration::showDialog(&map, true);
	if (isDelete){
		//mTempleteOwner->deleteSelf();
		return;
	}
	QString name = map.value(TempleteTypeEnumeration::NAME);
	mTempleteOwner->setStr(name);
}
void TextWidgetMouse::inputTextHandleRightMouse()
{
	QMap<QString, QString> map;
	BracketWidget* tempOwner = (BracketWidget*)mTempleteOwner;
	map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::INPUT_TEXT);
	map.insert(TempleteTypeEnumeration::INFO, tempOwner->getHint());
	map.insert(TempleteTypeEnumeration::NAME, tempOwner->getElementName());
	mTempleteOwner->addCodeIdToMap(map);
	QMap<QString, QString> beforMap = map;
	bool isDelete = TempleteTypeEnumeration::showDialog(&map, true);
	if (isDelete)
	{
		//StrContentData* selffocusData = mTempleteOwner->getSelfFocusData();
		//StrContentData* focusData = mTempleteOwner->getFocusStrData();
		//int color = selffocusData->getFontColor();
		mTempleteOwner->deleteSelf();
		//focusData->setFontColor(color);
		return;
	}
	QList<ViewObj *>  defaultObjListBefore;
	QList<ViewObj *> strObjListBefore;


	ViewObj* beforFocus = NULL;
	char dir = 0;
	mTempleteOwner->getFocusObjAndDir(beforFocus,dir);
	SetTxtWidgetElementAction::copyObjListToObjList(mTempleteOwner->mDefaultObjList, &defaultObjListBefore, true);
	SetTxtWidgetElementAction::copyObjListToObjList(mTempleteOwner->mStrObjList, &strObjListBefore, true);
	bool isSucceed = mTempleteOwner->setTempleteProperty(map);
	if (isSucceed)
	{
		mTempleteOwner->addSetTempletePropertyAction(beforMap, map, &defaultObjListBefore, &strObjListBefore, beforFocus, dir);
	}
	mTempleteOwner->replaceNeedReplaceList(1);
	
}
