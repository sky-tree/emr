#include "BracketWidgetKeyboard.h"
#include "BracketWidget.h"
#include "FlagParam.h"
#include "ViewObj.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
//#include "MacroTemplete.h"
//#include "DrawFactory.h"
//#include "ViewFactory.h"
#include "OperateMgr.h"
#include "TextWidgetKeyboard.h"
#include "BaseChildrenManager.h"
#include "StrConstants.h"
#define FIND_TEMPLATE_PARAENT_POSITION(focus) \
	BaseContainer* tempParent = NULL; \
	BaseContainer* tempContainer = NULL; \
	int tempIdx = -1; \
	focus->findNormalTempContainerFromParent(tempContainer, tempIdx); /*应为focus是最低层的元素。*/\
	tempParent = focus->getTempParent(); \
if (tempParent == tempContainer)/*focus是普通文字*/\
{\
if (dir == BaseConstants::Direction::RIGHT)\
{\
	tempIdx++; \
}\
}\
else\
{\
	tempParent = tempContainer->getChildren()->at(tempIdx); \
	BaseContainer* firstView = tempParent->findFirstElementFromChildren(); \
if (firstView == focus&& dir == BaseConstants::Direction::LEFT)\
{\
	\
}\
else\
{\
	tempIdx++; \
}\
	\
}
//#include "BaseOperateMgr.h"
BracketWidgetKeyboard::BracketWidgetKeyboard(BaseTemplate* templeteOwner) :BaseTempleteKeyboard(templeteOwner, ConstantsID::ClassId::TEXT_WIDGET_KEYBOARD)
, mTextWidget((BracketWidget*)templeteOwner)
{
}


BracketWidgetKeyboard::~BracketWidgetKeyboard()
{
	if (mTextWidget)
	{
		mTextWidget = NULL;
	}
}
int BracketWidgetKeyboard::keyPress(QString str, BaseElement * obj){
	ViewObj * focusObj = NULL;
	char dir = 0;
	mTextWidget->getFocusObjAndDir(focusObj,dir);
	if (!focusObj)
		return BaseConstants::NONE;
	
	
	/*if ()
	{

	}*/
	if (focusObj != (ViewObj*)obj)
	{
		return BaseConstants::NONE;
	}
	if (TextWidgetKeyboard::isSystemKey(str)){
		return 0;
	}
	
	//宏元素处理
	int objectId = mTextWidget->getObjID();
	ViewObj * selsectObj = (ViewObj*)obj;
	OperateMgr * operateMgr = selsectObj->getOperateMgr();
	FIND_TEMPLATE_PARAENT_POSITION(selsectObj)
	if (operateMgr->isChoosed(tempParent) || operateMgr->getChoosedSize()>0)
	{
		return BaseConstants::NONE;
	}
	if (objectId == ConstantsID::ContainerId::BRAKETWIDGET_TEMPLETE)
	{
		/*if (TextWidgetKeyboard::isDeleteOrBackspaceKey(str))
		{
			BaseWinData * winData = mTextWidget->getWinData();
			if (winData->isChoosed(mTextWidget->getTempleteParent()))
			{
				return 0;
			}
		}*/
		ViewObj* lastView = mTextWidget->getLastChildView();
		if (lastView == selsectObj){
			if (dir == BaseConstants::Direction::RIGHT)
			{
				/*if (str.compare(BaseConstants::MyKeyBoard::DEL) == 0)
				{
					return BaseConstants::NONE;
				}
				else*/ if (str.compare(StrConstants::MyKeyBoard::BACK_SPACE) == 0){
					ViewObj* preView = lastView->getPreView();
					if (preView)
					{
						preView->setFocus(true);
					}
					//mTextWidget->getTempleteParent()->deleteSelf();
					return 1;
				}
				else{
					return BaseConstants::NONE;
					/*if (str.compare(BaseConstants::MyKeyBoard::ENTER) == 0)
					{
						return 0;
					}
					MacroTemplete * macroTemplete = (MacroTemplete *)mTextWidget->getTempleteParent();
					BaseTemplete * tempParent = macroTemplete->getTempleteParent();
					int idx = macroTemplete->getTempleteIndexFromParent()+1;
					if (tempParent)
					{
						int size = str.length();
						ViewObj * row = lastView->getViewParent();
						int viewIndex = lastView->getViewIndexFromParent()+1;
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
			else
			{
				TextWidgetKeyboard::mooveFocus(str, selsectObj,dir);
				return 1;
			}
			
		}
		TextWidgetKeyboard::mooveFocus(str, selsectObj, dir);
		return 1;
		//if (str == BaseConstants::MyKeyBoard::BACK_SPACE)
		//{
		//	ViewObj * firstObj = mTextWidget->getFirstChildView();
		//	if (selsectObj == firstObj)
		//	{
		//		if (dir == BaseConstants::Direction::RIGHT)
		//		{
		//			return 1;
		//		}

		//	}
		//	else if (selsectObj == mTextWidget->mStrObjList->at(1))
		//	{
		//		if (dir == BaseConstants::Direction::LEFT)
		//		{
		//			return 1;
		//		}
		//	}
		//	else
		//	{
		//		ViewObj * lastView = mTextWidget->getLastChildView();
		//		if (lastView&&lastView == selsectObj)
		//		{
		//			if (dir == BaseConstants::Direction::RIGHT)
		//			{
		//				ViewObj* preView = lastView->getPreView();
		//				if (preView)
		//				{
		//					preView->setFocus(true, false);
		//				}
		//				//mTextWidget->getTempleteParent()->deleteSelf();
		//				return 1;
		//			}
		//		}
		//	}
		//}
		//else if (str == BaseConstants::MyKeyBoard::DEL)
		//{
		//	if (selsectObj == lastView)
		//	{
		//		if (dir == BaseConstants::Direction::LEFT)
		//		{
		//			return 1;
		//		}
		//		if (dir == BaseConstants::Direction::RIGHT)
		//		{
		//			return BaseConstants::NONE;
		//		}
		//	}
		//	else if (selsectObj == mTextWidget->mStrObjList->at(mTextWidget->mStrObjList->size() - 2))
		//	{
		//		if (dir == BaseConstants::Direction::RIGHT)
		//		{
		//			return 1;
		//		}
		//	}
		//}
		//
		//if (mTextWidget->mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS || mTextWidget->mTemptStatus == BaseConstants::ObjStatus::NONEDIT_STATUS){
		//	/*BaseTemplete *parentTemp = mTextWidget->getTempleteParent();
		//	if (parentTemp&&parentTemp->getClassId() != ConstantsID::ClassId::PARAGRAPH_TEMPLETE)
		//	{
		//	int returnValue = parentTemp->keyPress(str,obj);
		//	if (returnValue > BaseConstants::NONE)
		//	{
		//	return returnValue;
		//	}
		//	}*/
		//	return 1;
		//}
	}
	else if ( objectId == ConstantsID::ContainerId::INPUT_TEXT_TEMPLETE){
		/*if (TextWidgetKeyboard::isDeleteOrBackspaceKey(str))
		{
			BaseWinData * winData = mTextWidget->getWinData();
			if (winData->isChoosed(mTextWidget))
			{
				return 0;
			}
		}*/
		ViewObj* firstView = mTextWidget->getFirstChildView();
		ViewObj * lastView = mTextWidget->getLastChildView();
		if (str == StrConstants::MyKeyBoard::BACK_SPACE)
		{
			//ViewObj* secondObj = NULL;
			//if ()
			//{

			//}
			if (selsectObj == firstView)
			{
				if (dir == BaseConstants::Direction::RIGHT)
				{
					TextWidgetKeyboard::mooveFocus(str, selsectObj, dir);
					return 1;
				}
				else if (dir == BaseConstants::Direction::LEFT)
				{
					return 0;
				}

			}
			//else if (selsectObj == mTextWidget->mStrObjList->at(1))
			else if (selsectObj == mTextWidget->getChildren()->at(1))
			{
				if (dir == BaseConstants::Direction::LEFT)
				{
					TextWidgetKeyboard::mooveFocus(str, selsectObj, dir);
					return 1;
				}
			}
			else
			{
				ViewObj * lastView = mTextWidget->getLastChildView();
				if (lastView&&lastView == selsectObj)
				{
					if (dir == BaseConstants::Direction::RIGHT)
					{
						ViewObj* preView = lastView->getPreView();
						if (preView)
						{
							preView->setFocus(true);
						}
						//mTextWidget->deleteSelf();
						return 1;
					}
					else
					{
						if (mTextWidget->mStrObjList->size() == 0)
						{
							TextWidgetKeyboard::mooveFocus(str, selsectObj, dir);
							return 1;
						}
						
					}
				}
			}
			//需要单独处理判断是否只有最后2个表示符号
			int returnValue = mTextWidget->handleKeyPress(str, obj);
			handleIfLeftSign();
			if (returnValue == 0)
			{
				if (mTextWidget->mStrObjList->size() == 0)
				{
					TextWidgetKeyboard::mooveFocus(str, selsectObj, dir);
					return 1;
				}
			}
			return	returnValue;
		}
		else if (str == StrConstants::MyKeyBoard::DEL)
		{
			
			if (selsectObj == lastView)
				{
					if (dir == BaseConstants::Direction::LEFT)
					{
						TextWidgetKeyboard::mooveFocus(str, selsectObj, dir);
						return 1;
					}
					if (dir == BaseConstants::Direction::RIGHT)
					{
						return BaseConstants::NONE;
					}
				}
			else if (selsectObj == mTextWidget->getChildren()->at(mTextWidget->getChildren()->size() - 2))
				{
					if (dir == BaseConstants::Direction::RIGHT)
					{
						TextWidgetKeyboard::mooveFocus(str, selsectObj, dir);
						return 1;
					}
				}
				else
				{
					
					if (firstView&&firstView == selsectObj)
					{
						if (dir == BaseConstants::Direction::LEFT)
						{
							ViewObj* nextView = firstView->getNextView();
							if (nextView)
							{
								nextView->setFocus(false);
							}
							//mTextWidget->deleteSelf();
							return 1;
						}
						else{
							if (mTextWidget->mStrObjList->size() == 0){
								TextWidgetKeyboard::mooveFocus(str, selsectObj, dir);
								return 1;
							}
						}
					}
				}
				//需要单独处理判断是否只有最后2个表示符号
				int returnValue = mTextWidget->handleKeyPress(str, obj);
				handleIfLeftSign();
				if (returnValue == 0)
				{
					if (mTextWidget->mStrObjList->size() == 0)
					{
						TextWidgetKeyboard::mooveFocus(str, selsectObj, dir);
						return 1;
					}
				}
				return	returnValue;

		}
		else{
			if (selsectObj == firstView)
			{
				if (dir == BaseConstants::Direction::LEFT)
				{
					return 0;
					/*if (str.compare(BaseConstants::MyKeyBoard::ENTER) == 0)
					{
						return 0;
					}
					//BaseTemplete * preTemplete = this->getParentTempleteById(ConstantsID::ClassId::PARAGRAPH_TEMPLETE);
					BaseTemplete * tempParent = mTextWidget->getTempleteParent();
					int idx = mTextWidget->getTempleteIndexFromParent();
					if (tempParent)
					{
						int size = str.length();
						ViewObj * row = firstView->getViewParent();
						int viewIndex = firstView->getViewIndexFromParent();
						ViewObj * txt;
						for (int i = 0; i < size; i++)
						{
							DrawTxtParam * txtParam = DrawFactory::createDrawTxtParam(NULL, str.mid(i, 1), mTextWidget->getFocusData());
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
			else if (selsectObj == lastView)
			{
				if (dir == BaseConstants::Direction::RIGHT)
				{
					/*if (str == BaseConstants::MyKeyBoard::ENTER)
					{
						mTextWidget->getWinData()->pressEnter();
						return 1;
					}*/
					return 0;
					/*if (str.compare(BaseConstants::MyKeyBoard::ENTER) == 0)
					{
						return 0;
					}
					BaseTemplete * tempParent = mTextWidget->getTempleteParent();
					int idx = mTextWidget->getTempleteIndexFromParent() + 1;
					if (tempParent)
					{
						int size = str.length();
						ViewObj * row = lastView->getViewParent();
						int viewIndex = lastView->getViewIndexFromParent() + 1;
						ViewObj * txt;
						for (int i = 0; i < size; i++)
						{
							DrawTxtParam * txtParam = DrawFactory::createDrawTxtParam(NULL, str.mid(i, 1), mTextWidget->getFocusData());
							txt = ViewFactory::createTxt(tempParent, row, txtParam, ConstantsID::ContainerId::TXT);
							//BaseTemplete::addTempleteChildToViewParent(txt, row, idx, viewIndex);
							tempParent->addTempleteChild(txt, idx, false, false);
							row->addChild(txt, viewIndex);
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
			if (str.compare(StrConstants::MyKeyBoard::ENTER) == 0)
			{
				return 1;
			}
			if (mTextWidget->canEditTempleteProperty())
			{
				return 1;
			}
			if (mTextWidget->mStrObjList->size() == 0)
			{
				str = mTextWidget->mWidgetSignBeforeOptionItem + str + mTextWidget->mWidgetSignBehindOptionItem;
				mTextWidget->handleKeyPress(str, obj);
				ViewObj* lastTwo = (ViewObj*)mTextWidget->getChildren()->at(mTextWidget->getChildren()->size() - 2);
     				lastTwo->setFocus(true);
				return 1;
			}
			else
			{
				return	mTextWidget->handleKeyPress(str, obj);
			}
			
		}
	}
		//	输入处理后，需要判断
	return	mTextWidget->handleKeyPress(str, obj);
}
void BracketWidgetKeyboard::handleIfLeftSign()
{
	if (mTextWidget->mStrObjList->size()==2)
	{
		ViewObj*  firstStrView = mTextWidget->mStrObjList->at(0);
		ViewObj* row = firstStrView->getViewParent();
		int indexView = firstStrView->getViewIndex();
		firstStrView->setFocus(false);
		mTextWidget->addDefaultStrObjShow();
		mTextWidget->mStr.clear();
		mTextWidget->deleteChild(mTextWidget->mDefaultObjList->size(), 2, true);
		mTextWidget->clearStrObjList();
		//mTextWidget->showObjListWithNoFlag(row,indexView,mTextWidget->mDefaultObjList, 0, mTextWidget->mDefaultObjList->size());
	
		ViewObj* firstView =  mTextWidget->mDefaultObjList->at(0);
		firstView->setFocus(true);
		mTextWidget->addTxtLayerAndReplaceNeedReplaceList(firstView);
	}
}
int BracketWidgetKeyboard::keyRelease(QString str, BaseElement * obj)
{
	ViewObj* focusObj = NULL;
	char dir = 0;
	mTextWidget->getFocusObjAndDir(focusObj,dir);
	if (focusObj != (ViewObj*)obj)
	{
		return BaseConstants::NONE;
	}
	if (mTextWidget->mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS){
		return 1;
	}
	if (TextWidgetKeyboard::isSystemKey(str)){
		return 0;
	}
	return	mTextWidget->handleKeyRelease(str, obj);
}

