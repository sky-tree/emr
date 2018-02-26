#include "ButtonInput.h"

#include "GlobalImgData.h"
#include "BaseContainer.h"
#include "ConstantsNumber.h"
#include "ConstantsID.h"
#include "ImgContentData.h"
#include "DrawButton.h"
#include "BaseViewParam.h"
#include "ObjDrawManager.h"
#include "ObjMouseManager.h"
#include "DrawRect.h"
#include "BaseConstants.h"
#include "UIConstants.h"
#include "OperateMgr.h"
#include "QDebug"

ButtonInput::ButtonInput(ViewObj * owner) :BaseMouse(owner, ConstantsID::ClassId::BUTTON_MOUSE)
{
	mScale = 1.0;
}
ButtonInput::ButtonInput(ViewObj * owner, WriterManager * writerMgr, QString resRelease, QString resPress) : BaseMouse(owner, ConstantsID::ClassId::BUTTON_MOUSE)
{
	mWriterMgr = writerMgr;
	mResRelease = resRelease;
	mResPress = resPress;
	mImage = NULL;
	mType = ConstantsID::ContainerId::SCROLL_BUTTON_UP;
	mLastY = -1;
	mScale = 1.0;
}

ButtonInput::~ButtonInput()
{
	if (mImage)
	{
		delete mImage;
	}
	mImage = NULL;
}

void ButtonInput::init(int type)
{
	BaseViewParam * viewParam = new BaseViewParam(mViewOwner);
	if (!mImage)
	{
		mImage = new DrawButton(mViewOwner);
	}
	mImage->init(mResRelease, mResPress);
	ObjDrawManager * drawMgr = mViewOwner->getDrawMgr();
	drawMgr->initDraws(mImage, NULL);

	if (type != ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE)
	{
		viewParam->init(0, 0, 16, 16);
	}
	else
	{
	 	viewParam->init(0, 0, 16, 100);
	}
}

int ButtonInput::setMiddleButtonParam(int y, int pageH, int totalH)
{
//	mMiddleY = y / totalH * h;//显示Y坐标/总高度*滚动条高度
//	mMiddleH = socrllH / totalH * socrllH; //显示页面高度/总高度*滚动条高度
	if (mOwner->getObjID() == ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE_BAR && totalH != 0)
	{
		ViewObj * viewObj = mOwner->getViewParent();
		BaseViewParam * viewParam = viewObj->getViewParam();
		float barH = viewParam->getSelfH();
		mScale = barH / (totalH * 1.0);
		syncScale();
		int selfH = pageH * mScale;
		if (selfH == 0)
		{
			return false;
		}
		else if(selfH > barH)
		{
			selfH = barH;
		}
		int settingY = y * mScale;
		if (settingY < 0)
		{
			settingY *= -1;
		}
		viewParam = mViewOwner->getViewParam();
		viewParam->setSelfH(selfH);
		viewParam->setY(settingY);
		return true;
	}
	ViewObj * viewObj = mOwner->getViewParent();
	BaseViewParam * viewParam = viewObj->getViewParam();
	float barH = viewParam->getSelfH();
	mScale = barH / (totalH * 1.0);
	return false;
}


int ButtonInput::mousePress(int type, int x, int y)
{
	if (type != Qt::LeftButton)
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	ViewObj * window = mViewOwner->getWindow();
	if (window)
	{
		window->getMouseMgr()->setFocus(mViewOwner);
		ViewObj * mPagePanel = (ViewObj *)window->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, true);
		mPagePanel->changeStatus(BaseConstants::ObjStatus::PAGE_PANEL_SCROLL_DRAG, false);
	}
	mImage->setIndex(BaseConstants::MousePressType::PRESS);
	if (mOwner->getObjID() == ConstantsID::ContainerId::SCROLL_BUTTON_UP)
	{
		changeMiddleButtonY(ConstantsID::ContainerId::SCROLL_BUTTON_UP);
	}
	else if (mOwner->getObjID() == ConstantsID::ContainerId::SCROLL_BUTTON_DOWN)
	{
		changeMiddleButtonY(ConstantsID::ContainerId::SCROLL_BUTTON_DOWN);
	}
	return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
}
int ButtonInput::mouseRelease(int type, int x, int y)
{
	if (type != Qt::LeftButton)
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	mImage->setIndex(BaseConstants::MousePressType::RELEASE);
	ViewObj * window = mViewOwner->getWindow();
	if (window)
	{
		window->getMouseMgr()->setFocus(NULL);
		ViewObj * mPagePanel = (ViewObj *)window->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, true);
		//mPagePanel->changeStatus(BaseConstants::ObjStatus::FREE, false);
	}

	if (mOwner->getObjID() == ConstantsID::ContainerId::SCROLL_BUTTON_UP)
	{

	}
	else if (mOwner->getObjID() == ConstantsID::ContainerId::SCROLL_BUTTON_DOWN)
	{

	}
	if (mLastY != -1)
		mLastY = -1;
	return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
}
int ButtonInput::mouseMove(int type, int x, int y)
{
	return 0;
}
int ButtonInput::mouseDrag(int type, int x, int y)
{
	if (type != Qt::LeftButton)
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;

	if (mOwner->getObjID() == ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE_BAR)
	{
		BaseViewParam * viewParam = mViewOwner->getViewParam();
		float disY = viewParam->getY();
		if (mLastY != -1)
		{
			float settingY = mLastY - disY - y;
			settingY = disY - settingY;
			if (true == isBeyondScrollPanel(settingY, disY))
				return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
			viewParam->setY(settingY);
			refleshPagePanel(settingY);
		}
		mLastY = disY + y;
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	return 0;
}
int ButtonInput::mouseDoublePress(int type, int x, int y)
{
	if (type != Qt::LeftButton)
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	return 0;
}
int ButtonInput::mouseScroll(int angle)
{
	if (mOwner->getObjID() == ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE_BAR)
	{
		BaseViewParam * viewParam = mViewOwner->getViewParam();
		float settingY = viewParam->getY();
		int i = 0;
		float stepY = angle * mScale / 2.0;
//		while (i++ < 5)
		{
			settingY = settingY - stepY;
			if (true == isBeyondScrollPanel(settingY, settingY + stepY))
				return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
			viewParam->setY(settingY);
			refleshPagePanel(settingY);
		}
		mOwner->getOperateMgr()->setNeedRefreshPagePanelYByFlag(false);
	}
	if (mLastY != -1)
		mLastY = -1;
	return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
}

bool ButtonInput::changeMiddleButtonY(float type)
{
	ViewObj * viewParent = mOwner->getViewParent();
	if (viewParent)
	{
		ViewObj * middleButtonBarViewObj = (ViewObj*)viewParent->findChildrenByID(ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE_BAR, true);
		if (middleButtonBarViewObj)
		{
			BaseViewParam * middleButtonBarViewParam = middleButtonBarViewObj->getViewParam();
			if (middleButtonBarViewParam)
			{
				float disY = middleButtonBarViewParam->getY();
				float settingY = 0;
				if (type == ConstantsID::ContainerId::SCROLL_BUTTON_UP)
				{
					float stepY = -600 * mScale;
					settingY = disY + stepY;
					if (true == isBeyondScrollPanel(settingY, disY))
						return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
					middleButtonBarViewParam->setY(settingY);
					refleshPagePanel(settingY);
				}
				else
				{
					float stepY = 600 * mScale;
					settingY = disY + stepY;
					if (true == isBeyondScrollPanel(settingY, disY))
						return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
					middleButtonBarViewParam->setY(settingY);
					refleshPagePanel(settingY);
				}
			}
		}
	}
	return false;
}

bool ButtonInput::isBeyondScrollPanel(float& settingY, float lastY)
{
	int scrollH = 0, middleH = 0;
	if (mOwner->getObjID() == ConstantsID::ContainerId::SCROLL_BUTTON_UP || mOwner->getObjID() == ConstantsID::ContainerId::SCROLL_BUTTON_DOWN)
	{
		ViewObj * viewParent = mOwner->getViewParent();
		ViewObj * middleButtonViewObj = (ViewObj*)viewParent->findChildrenByID(ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE, true);
		if (middleButtonViewObj)
		{
			BaseViewParam* viewParam = middleButtonViewObj->getViewParam();
			scrollH = viewParam->getTotalH();
			ViewObj * middleBarViewobj = (ViewObj*)middleButtonViewObj->findChildrenByID(ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE_BAR, true);
			if (middleBarViewobj)
			{
				viewParam = middleBarViewobj->getViewParam();
				middleH = viewParam->getTotalH();
			}
		}
	}
	else if (mOwner->getObjID() == ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE_BAR)
	{
		BaseViewParam * viewParam = mViewOwner->getViewParam();
		middleH = viewParam->getTotalH();

		ViewObj * viewParent = mOwner->getViewParent();
		viewParam = viewParent->getViewParam();
		scrollH = viewParam->getTotalH();
	}

	if (settingY + middleH > scrollH)
	{
		if (lastY < scrollH)
		{
			settingY = scrollH - middleH;
			return false;
		}
		return true;
	}
	else if (settingY < 0)
	{
		if (lastY > 0)
		{
			settingY = 0;
			return false;
		}
		return true;
	}
	
	return false;
}

void ButtonInput::refleshPagePanel(float settingY)
{ 
	OperateMgr * operateMgr = mViewOwner->getOperateMgr();
	ViewObj * viewObj = operateMgr->getPagePanel();
	BaseViewParam * viewParam = viewObj->getViewParam();
	float dY = viewParam->getY();
	float totalH = viewParam->getTotalH();

	ViewObj * parentViewObj = mViewOwner->getViewParent();
	BaseViewParam * parentViewParam = NULL;
	if (!parentViewObj->isObj(ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE))
	{
		ViewObj * middleViewObj = (ViewObj*)parentViewObj->findChildrenByID(ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE, true);
		parentViewParam = middleViewObj->getViewParam();
	}
	else
	{
		parentViewParam = parentViewObj->getViewParam();
	}

	float scrollTotalH = parentViewParam->getTotalH();

	float y = totalH * settingY / scrollTotalH;
	viewParam->setY( -y);
}
void ButtonInput::releaseFocusFromWindow()
{
	//ViewObj * window = mViewOwner->getWindow();
	//if (window)
	//{
	//	window->getMouseMgr()->setFocus(NULL);
	//}
}

void ButtonInput::syncScale()
{
//	ViewObj * viewObj = mOwner->getViewParent();
	ViewObj * viewObj = mOwner->findViewFromParent(ConstantsID::ContainerId::SCROLL_PANEL, true);
	QList<BaseContainer *> * childrenList = viewObj->getChildren();
	for (int i = 0; i < childrenList->size(); i++)
	{
		ViewObj * p = (ViewObj*) childrenList->at(i);
		if (!p->isObj(ConstantsID::ContainerId::SCROLL_BUTTON_MIDDLE))
		{
			ObjMouseManager * muoseMgr = p->getMouseMgr();
			muoseMgr->getMouse()->setScale(mScale);
		}
	}
}

void ButtonInput::setScale(float scale)
{
	mScale = scale;
}