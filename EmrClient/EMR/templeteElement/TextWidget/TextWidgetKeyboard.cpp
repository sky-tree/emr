#include "TextWidgetKeyboard.h"
#include "TextWidget.h"

#include "FlagParam.h"
#include "ConstantsID.h"
#include "StrConstants.h"
#include "ViewObj.h"
#include "BaseContainer.h"
#include "BaseChildrenManager.h"
#include "OperateMgr.h"
#include "ViewFactory.h"
#include "BaseConstants.h"
#include "TempleteTypeEnumeration.h"
//#include "ExcelKeyboard.h"
//#include "BaseWinData.h"
//#include "BaseOperateMgr.h"
TextWidgetKeyboard::TextWidgetKeyboard(BaseTemplate* templeteOwner) :BaseTempleteKeyboard(templeteOwner, ConstantsID::ClassId::TEXT_WIDGET_KEYBOARD)
, mTextWidget((TextWidget*)templeteOwner)
{

}


TextWidgetKeyboard::~TextWidgetKeyboard()
{
	if (mTextWidget){
		mTextWidget = NULL;
	}
}
int TextWidgetKeyboard::keyPress(QString str, BaseElement * obj){
	ViewObj* selectObj = (ViewObj*)obj;
	ViewObj * focus = NULL;
	char  dir = 0;
	mTextWidget->getFocusObjAndDir(focus, dir);
	if (focus != selectObj)
	{
		return 0;
	}
	if (isSystemKey(str)){
		return 0;
	}
	//BaseOperateMgr * opMgr = selectObj->getOperateMgr();
	//BaseTemplete * tempParent = (BaseTemplete *)opMgr->getCanBeChoosenObj();
	//BaseWinData * winData = mTextWidget->getWinData();
	BaseContainer* tempContainer = NULL;
	int tempIdx = -1;
	focus->findNormalTempContainerFromParent(tempContainer, tempIdx); //应为focus是最低层的元素。
	BaseContainer* tempParent = tempContainer->getChildrenMgr()->getChildren()->at(tempIdx);
	OperateMgr * operateMgr = mTextWidget->getOperateMgr();
	if (operateMgr->isChoosed(tempParent) || operateMgr->getChoosedSize()>0)
	{
		if (mTextWidget->getObjID() == ConstantsID::ContainerId::FIX_TEXT_WIDGET_TEMPLETE)
		{
			return 1;
		}
		return 0;
	}
	if (mTextWidget->getObjID() == ConstantsID::ContainerId::MACRO_TEXT_TEMPLETE_OBJ )
	{
		
		ViewObj* firstView = mTextWidget->getFirstChildView();

		//if (mTextWidget->mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS)
		{
			if (firstView == selectObj)
			{
				
				if (dir == BaseConstants::Direction::LEFT)
				{
					if (str.compare(StrConstants::MyKeyBoard::ENTER) == 0)
					{
						return 0;
					}
					if (str.compare(StrConstants::MyKeyBoard::DEL) == 0)
					{

						//mTextWidget->getTempleteParent()->deleteSelf();
						ViewObj* nextView = firstView->getNextView();
						if (nextView)
						{
							nextView->setFocus(false);
						}
						return 1;
					}
					else
					{
						return BaseConstants::NONE;
						
					}
					
				}
				else
				{
					mooveFocus(str, selectObj, dir);
					return 1;
				}
			}
			else
			{
				mooveFocus(str, selectObj, dir);
				return 1;
			}
			
		}
		if (TextWidgetKeyboard::isDeleteOrBackspaceKey(str))
		{
			//BaseWinData * winData = mTextWidget->getWinData();
			if (operateMgr->isChoosed(tempParent) || operateMgr->getChoosedSize()>0)
			{
				return 0;
			}
		}
		if (mTextWidget->mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS || mTextWidget->mTemptStatus == BaseConstants::ObjStatus::NONEDIT_STATUS)
		{
			return 1;

		}
		else
		{
			return	mTextWidget->handleKeyPress(str, obj);
		}
		
	}
	else if (mTextWidget->getObjID() == ConstantsID::ContainerId::CHOICE_TEXT_TEMPLETE_OBJ)
	{
		if (str.compare(StrConstants::MyKeyBoard::ENTER) == 0)
		{
			return 1;
		}
		if (isDeleteOrBackspaceKey(str))
		{
			if (operateMgr->isChoosed(tempParent) || operateMgr->getChoosedSize()>0)
			{
				return 0;
			}
		}
		/*if (mTextWidget->canEditTempleteProperty())
		{
			mooveFocus(str, selectObj, dir);
			return 1;
		}*/
		return mTextWidget->handleChoiceTextWidgetKeyPress(str, obj);
	}
	else if (mTextWidget->getObjID() == ConstantsID::ContainerId::FIX_TEXT_WIDGET_TEMPLETE)
	{
		if (isDeleteOrBackspaceKey(str))
		{
			//BaseWinData* winData = mTextWidget->getWinData();
			if (operateMgr->isChoosed(mTextWidget) || operateMgr->getChoosedSize()>0)
			{
				return 0;
			}
		}
			ViewObj* firstView = mTextWidget->getFirstChildView();
			ViewObj* lastView = mTextWidget->getLastChildView();
			//if (mTextWidget->mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS)
				if (firstView == selectObj)
				{
					
					if (dir == BaseConstants::Direction::LEFT)
					{
						if (str.compare(StrConstants::MyKeyBoard::ENTER) == 0)
						{
							return 0;
						}
						if (str.compare(StrConstants::MyKeyBoard::DEL) == 0)
						{
							//mTextWidget->deleteSelf();
							ViewObj* nextView = firstView->getNextView();
							if (nextView)
							{
								nextView->setFocus(false);
							}
							return 1;
						}
						else if (str.compare(StrConstants::MyKeyBoard::BACK_SPACE) == 0){

							return BaseConstants::NONE;
						}
						else{
							return 0;
							/*BaseTemplete * tempParent = mTextWidget->getTempleteParent();
							int idx = mTextWidget->getTempleteIndexFromParent();
							if (tempParent)
							{
								int size = str.length();
								ViewObj * row = firstView->getViewParent();
								int viewIndex = firstView->getViewIndexFromParent();
								DrawTxtParam * focusData = mTextWidget->getFocusData();
								ViewObj * txt;
								for (int i = 0; i < size; i++)
								{
									DrawTxtParam * txtParam = DrawFactory::createDrawTxtParam(NULL, str.mid(i, 1), focusData);
									txt = ViewFactory::createTxt(tempParent, row, txtParam, ConstantsID::ContainerId::TXT);
									tempParent->addTempleteChild(txt, idx, false, false);
									row->addChild(txt, viewIndex);
									//BaseTemplete::addTempleteChildToViewParent(txt, row, idx, viewIndex);
									row = txt->getViewParent();
									viewIndex = txt->getViewIndexFromParent();
									viewIndex++;
									idx++;
								}
								txt->setFocus(true, false);
							}
							return 1;*/
						}
					}
					else if (dir == BaseConstants::Direction::RIGHT)
					{
						mooveFocus(str, selectObj, dir);
						return 1;
					}
				}
				else if (lastView == selectObj)
				{
					if (dir == BaseConstants::Direction::RIGHT)
					{
						if (str.compare(StrConstants::MyKeyBoard::ENTER) == 0)
						{
							return 0;
						}
						if (str.compare(StrConstants::MyKeyBoard::BACK_SPACE) == 0)
						{
							//mTextWidget->deleteSelf();
							ViewObj* preView = lastView->getPreView();
							if (preView)
							{
								preView->setFocus(true);
							}
							return 1;
						}
						else if (str.compare(StrConstants::MyKeyBoard::DEL) == 0){
							return BaseConstants::NONE;
						}
						else
						{
							return 0;
							/*BaseTemplete * tempParent = mTextWidget->getTempleteParent();
							int idx = mTextWidget->getTempleteIndexFromParent() + 1;
							if (tempParent)
							{
								int size = str.length();
								ViewObj * row = lastView->getViewParent();
								int viewIndex = lastView->getViewIndexFromParent() + 1;
								DrawTxtParam * focusData = mTextWidget->getFocusData();
								ViewObj * txt;
								for (int i = 0; i < size; i++)
								{
									DrawTxtParam * txtParam = DrawFactory::createDrawTxtParam(NULL, str.mid(i, 1), focusData);
									txt = ViewFactory::createTxt(tempParent, row, txtParam, ConstantsID::ContainerId::TXT);
									tempParent->addTempleteChild(txt, idx, false, false);
									row->addChild(txt, viewIndex);
									//BaseTemplete::addTempleteChildToViewParent(txt, row, idx, viewIndex);
									row = txt->getViewParent();
									viewIndex = txt->getViewIndexFromParent();
									viewIndex++;
									idx++;
								}
								txt->setFocus(true, false);
							}
							return 1;*/
						}
					}
					else  if (dir == BaseConstants::Direction::LEFT)
					{
						mooveFocus(str, selectObj, dir);
						return 1;
					}
				}
				else
				{
					mooveFocus(str,selectObj,dir);
					return 1;
				}
				if (isDeleteOrBackspaceKey(str))
				{
					return 0;
				}
			if (mTextWidget->mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS || mTextWidget->mTemptStatus == BaseConstants::ObjStatus::NONEDIT_STATUS)
			{
				return 1;

			}
			else
			{
				return	mTextWidget->handleKeyPress(str, obj);
			}
	}
	else
	{
		return	mTextWidget->handleKeyPress(str, obj);
	}
    return 0;
}
int TextWidgetKeyboard::keyRelease(QString str, BaseElement * obj)
{
	ViewObj * focus = NULL;
	char  dir = 0;
	mTextWidget->getFocusObjAndDir(focus, dir);
	if (focus != (ViewObj*)obj)
	{
		return 0;
	}
	/*if (mTextWidget->mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS){
		return 1;
	}*/
	if (isSystemKey(str))
	{
		return 0;
	}
	return	mTextWidget->handleKeyRelease(str, obj);
}
bool TextWidgetKeyboard::isSystemKey(const QString & keyStr){
	bool isSeystemKeya = false;
	/*if (keyStr.compare(StrConstants::MyKeyBoard::BACK_SPACE) == 0)
	{
		isSeystemKeya = true;
	}
	else if (keyStr.compare(StrConstants::MyKeyBoard::DEL) == 0)
	{
		isSeystemKeya = true;
	}
	else */
	if (keyStr.contains(TempleteTypeEnumeration::SISTEM_SIGN))
	{
		isSeystemKeya = true;
	}
	else if (keyStr.compare(StrConstants::MyKeyBoard::LEFT) == 0)
	{
		isSeystemKeya = true;
	}
	else if (keyStr.compare(StrConstants::MyKeyBoard::RIGHT) == 0)
	{
		isSeystemKeya = true;
	}
	else if (keyStr.compare(StrConstants::MyKeyBoard::UP) == 0)
	{
		isSeystemKeya = true;
	}
	else if (keyStr.compare(StrConstants::MyKeyBoard::DOWN) == 0)
	{
		isSeystemKeya = true;
	}
	/*else if (keyStr.compare(BaseConstants::MyKeyBoard::ENTER) == 0)
	{
		isSeystemKeya = true;
	}*/
	else if (keyStr.compare(StrConstants::MyKeyBoard::ESC) == 0)
	{
		isSeystemKeya = true;
	}
	/*else if (ExcelKeyboard::isSystemExcelKey(keyStr)){
		isSeystemKeya = true;
	}*/
	return isSeystemKeya;
}
bool TextWidgetKeyboard::isDeleteOrBackspaceKey(const QString & keyStr)
{
	bool isSeystemKeya = false;
	if (keyStr.compare(StrConstants::MyKeyBoard::BACK_SPACE) == 0)
	{
	isSeystemKeya = true;
	}
	else if (keyStr.compare(StrConstants::MyKeyBoard::DEL) == 0)
	{
	isSeystemKeya = true;
	}
	return isSeystemKeya;
}
int TextWidgetKeyboard::mooveFocus(const QString& str, ViewObj * focusObj, char dir)
{
	if (str.compare(StrConstants::MyKeyBoard::DEL) == 0)
	{
		ViewObj* nextView = focusObj->getNextView();
		if (nextView)
		{
			if (dir == BaseConstants::Direction::RIGHT)
			{
				nextView->setFocus(true);
			}
			else
			{
				nextView->setFocus(false);

			}
		}
		return 1;
	}
	else if (str.compare(StrConstants::MyKeyBoard::BACK_SPACE) == 0)
	{
		ViewObj* preView = focusObj->getPreView();
		if (preView)
		{
			if (dir == BaseConstants::Direction::RIGHT)
			{
				preView->setFocus(true);
			}
			else
			{
				preView->setFocus(false);
			}
		}
		else
		{
			if (dir == BaseConstants::Direction::RIGHT)
			{
				focusObj->setFocus(false);
			}
			
		}
		return 1;
	}
	return 0;
}