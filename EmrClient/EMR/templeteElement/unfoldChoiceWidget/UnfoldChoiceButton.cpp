#include "UnfoldChoiceButton.h"
#include "ViewFactory.h"
#include "BaseChildrenManager.h"
#include "ViewObj.h"
#include "QMessageBox.h"
#include "BaseViewParam.h"
#include "UnfoldChoiceWidgetTemplete.h"
#include "StrContentData.h"
#include "TempleteViewObjMouseManager.h"
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif

UnfoldChoiceButton::UnfoldChoiceButton(WriterManager * wMgr, BaseTemplate * templeteParent, char type, int objId, QString codeId, int leftPadding)
:BaseTemplate(wMgr, ConstantsID::ClassId::UNFOLD_CHOICE_BUTTON_TEMPLETE, objId, codeId)
, mUnfoldChoiceTemplate(NULL)
, mIsChecked(false)
, mRectView(NULL)
, mMarkOfSelected(NULL)
, mContainerObj(NULL)
, mUnfoldChoiceTxtData(NULL)
{
	mPaddingLeft = leftPadding;
	setTempleteParent(templeteParent);
	mUnfoldChoiceTemplate = (UnfoldChoiceWidgetTemplete*)templeteParent->getTempParent();
	setChooseObjId(templeteParent->getChooseObjId());
	initCreateImageView(wMgr,type);
}
UnfoldChoiceButton::~UnfoldChoiceButton()
{
	if (mMarkOfSelected != NULL)
	{
		mMarkOfSelected = NULL;
	}
	if (mUnfoldChoiceTemplate != NULL)
	{
		mUnfoldChoiceTemplate = NULL;
	}
	if (mContainerObj != NULL)
	{
		mContainerObj->initContentData(NULL);
		getChildren()->removeAll(mContainerObj);
		delete mContainerObj;
		mContainerObj = NULL;
	}
	if (mUnfoldChoiceTxtData != NULL)
	{
		mUnfoldChoiceTxtData = NULL;
	}
	
}
void UnfoldChoiceButton::initCreateImageView(WriterManager * writeMgr, char type)
{
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
	mContainerObj = ViewFactory::createChoiceBoxContainerView(writeMgr, this, -1);
	
	childrenMgr->addChild(mContainerObj, -1, true);
	mChoiceType = type;
	QString markStr;
	markStr = "√";
	if (mChoiceType == RADIO)
	{
		//mMarkOfSelected = ViewFactory::createChoiceBoxRadioImg(writeMgr, this, -1);
		
	}
	else if (mChoiceType == MULTIPLE_CHOICE)
	{
		//mMarkOfSelected = ViewFactory::createChoiceBoxMultipleImg(writeMgr, this, -1);
	}
	else
	{
		QMessageBox::about(NULL, "错误", "<font color='red'>UnfoldChoiceButton type error!</font>");
	}
	UnfoldChoiceWidgetTemplete* parentItem = dynamic_cast<UnfoldChoiceWidgetTemplete*>(findTempFromParent(ConstantsID::ContainerId::UNFOLD_CHOICE_TEMPLETE, true));
	mUnfoldChoiceTxtData = parentItem->getSelfFocusData();
	mContainerObj->initContentData(mUnfoldChoiceTxtData);
	mMarkOfSelected = ViewFactory::createTemplateTxt(mWriterMgr, this, markStr, mUnfoldChoiceTxtData);
	//mRectView = ViewFactory::createChoiceBoxRectImg(writeMgr, this, -1);
	QString rectStr="□";
	mRectView = ViewFactory::createTemplateTxt(mWriterMgr, this, rectStr, mUnfoldChoiceTxtData);
	mUnfoldChoiceTxtData->setString(rectStr);
	BaseChildrenManager * rectChildManger = mContainerObj->getChildrenMgr();
	rectChildManger->addChild(mRectView, -1, true);
	setPaddingLeft(mPaddingLeft);
	BaseViewParam * rectViewParam = mRectView->getViewParam();
	BaseViewParam * containerViewParam = mContainerObj->getViewParam();
	containerViewParam->setSelfH(rectViewParam->getTotalH());
}
void UnfoldChoiceButton::showMarkView()
{
	BaseChildrenManager * childManger = mContainerObj->getChildrenMgr();
	childManger->addChild(mMarkOfSelected, -1, true);
	refreshMarkPosition();
}
void UnfoldChoiceButton::refreshMarkPosition()
{
	BaseViewParam * rectViewParam = mRectView->getViewParam();
	int rectW = rectViewParam->getTotalW();
	int rectH = rectViewParam->getTotalH();
	BaseViewParam * markViewParam = mMarkOfSelected->getViewParam();
	int markW = markViewParam->getTotalW();
	int offSetX = markW / 6;
	int markH = markViewParam->getTotalH();
	int markX = rectViewParam->getX();
	markX += ((rectW - markW) >> 1) - offSetX;
	markViewParam->setX(markX);
	markViewParam->setY((rectH - markH) >> 1);

}
void UnfoldChoiceButton::invisibleMarkView()
{
	BaseChildrenManager * childManger = mContainerObj->getChildrenMgr();
	childManger->delChild(mMarkOfSelected,true);
}
void UnfoldChoiceButton::setChecked(bool checked)
{
	if (checked == mIsChecked)
	{
		return ;
	}
	else
	{
		mIsChecked = checked;
	}

	if (mIsChecked)
	{
		showMarkView();
	}
	else
	{
		invisibleMarkView();
	}
}
bool UnfoldChoiceButton::getChecked()
{
	return mIsChecked;
}
bool UnfoldChoiceButton::setDrawTxtParam(StrContentData* focusData)
{
	if (BaseTemplate::isSetFlowType(focusData))
	{
		return true;
	}
	
	ViewObj* viewParent = NULL;
	BaseContainer * obj = mRectView;
	bool isSuccess = true;
		if (obj->isView())
		{
			//viewParent = obj->getViewParent();
			/*if (viewParent)
			{
			viewParent->setPageRepaint();
			}*/
			ViewObj * tempViewObj = (ViewObj *)obj;
			if (tempViewObj->isTxt())
			{
				/*StrContentData * txtParam = (StrContentData *)obj->getContentData();
				txtParam->refreshTxtParamOnSetType(mUnfoldChoiceTxtData);
				txtParam->refreshTxtWH();*/
				isSuccess = tempViewObj->setDrawTxtParam(mUnfoldChoiceTxtData);
			}
		}
	
		 obj = mMarkOfSelected;
		 if (obj->isView() && isSuccess)
			{
				//viewParent = obj->getViewParent();
				/*if (viewParent)
				{
				viewParent->setPageRepaint();
				}*/
			 ViewObj * tempViewObj = (ViewObj *)obj;
			 if ((tempViewObj)->isTxt())
			 if (tempViewObj->isTxt())
				{
					/*StrContentData * txtParam = (StrContentData *)obj->getContentData();
					txtParam->refreshTxtParamOnSetType(mUnfoldChoiceTxtData);
					txtParam->refreshTxtWH();*/
				 isSuccess = tempViewObj->setDrawTxtParam(mUnfoldChoiceTxtData);
				}
			}
			refreshMarkPosition();
			BaseViewParam * rectViewParam = mRectView->getViewParam();
			int containerW = mPaddingLeft + rectViewParam->getTotalW();
			mContainerObj->getViewParam()->setSelfW(containerW);
			return isSuccess;
	//replaceNeedReplaceList();
}
int UnfoldChoiceButton::mousePress(int type, int x, int y, BaseContainer  * obj)
{
	if (type == Qt::RightButton)
	{
		return mUnfoldChoiceTemplate->mousePress(type, x, y, obj);
	}
	else
	{
		/*if (mChoiceType == RADIO)
		{
			if (mIsChecked)
			{

			}
			else
			{
				setChecked(true);
				mUnfoldChoiceTemplate->clicked(mTempParent,mIsChecked);
			}
		}
		else if (mChoiceType == MULTIPLE_CHOICE)
		{
			if (mIsChecked)
			{
				setChecked(false);
			}
			else
			{
				setChecked(true);
			}
			mUnfoldChoiceTemplate->clicked(mTempParent, mIsChecked);
		}*/
		return 1;
	}
}
int UnfoldChoiceButton::mouseRelease(int type, int x, int y, BaseContainer * obj)
{

	return BaseConstants::NONE;
}
int UnfoldChoiceButton::mouseMove(int type, int x, int y, BaseContainer * obj)
{
	int returnType = BaseConstants::NONE;
	/*if (mInputManager){
	return mInputManager->mouseMove(type, x, y, obj);
	}*/
	return returnType;
}
int UnfoldChoiceButton::mouseDrag(int type, int x, int y, BaseContainer * obj)
{
	return  BaseConstants::NONE;
}
int UnfoldChoiceButton::mouseDoublePress(int type, int x, int y, BaseContainer * obj)
{
	if (mChoiceType == RADIO)
	{
		if (mIsChecked)
		{

		}
		else
		{
			setChecked(true);
			mUnfoldChoiceTemplate->clicked(mTempParent, mIsChecked);
		}
	}
	else if (mChoiceType == MULTIPLE_CHOICE)
	{
		if (mIsChecked)
		{
			setChecked(false);
		}
		else
		{
			setChecked(true);
		}
		mUnfoldChoiceTemplate->clicked(mTempParent, mIsChecked);
	}
	return 1;
	return 1;
}
int UnfoldChoiceButton::mouseScroll(int angle, BaseContainer * obj)
{
	int returnType = BaseConstants::NONE;

	return returnType;
}
int UnfoldChoiceButton::keyPress(QString str, BaseContainer * obj)
{
	return mUnfoldChoiceTemplate->keyPress(str, obj);
}
int UnfoldChoiceButton::keyRelease(QString str, BaseContainer * obj)
{
	return mUnfoldChoiceTemplate->keyRelease(str, obj);
}
void UnfoldChoiceButton::setPaddingLeft(int paddingLeft)
{
	mPaddingLeft = paddingLeft;
	BaseViewParam * rectViewParam = mRectView->getViewParam();
	rectViewParam->setX(mPaddingLeft);
	int containerW = mPaddingLeft + rectViewParam->getTotalW();
	BaseViewParam * containerViewParam = mContainerObj->getViewParam();
	containerViewParam->setSelfW(containerW);
	mUnfoldChoiceTxtData->setTxtParamW(containerW);
	if (mIsChecked)
	{
		refreshMarkPosition();
	}
}
int UnfoldChoiceButton::getPaddingLeft()
{
	return mPaddingLeft;
}