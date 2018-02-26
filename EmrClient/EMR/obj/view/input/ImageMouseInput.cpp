#include <QDebug>

#include "ImageMouseInput.h"
#include "ViewObj.h"
#include "BaseViewParam.h"
#include "BaseScaleParam.h"
#include "MyAppEntrance.h"
#include "WriterManager.h"
#include "ObjMouseManager.h"
#include "ImgConTentData.h"

#include "BaseTools.h"
#include "UIConstants.h"
#include "OperateMgr.h"
#include "BaseTemplate.h"

ImageMouseInput::ImageMouseInput(ViewObj * owner) :BaseMouse(owner, ConstantsID::ClassId::BUTTON_MOUSE)
{
}
ImageMouseInput::ImageMouseInput(ViewObj * owner, BaseMouse * scoure)
	:BaseMouse(owner, ConstantsID::ClassId::BUTTON_MOUSE)
{
}


ImageMouseInput::~ImageMouseInput()
{
}


int ImageMouseInput::mousePress(int type, int x, int y)
{
	if (type != Qt::LeftButton)
		return 0;
	BaseTemplate * temp = (BaseTemplate*)mViewOwner->findTempFromParent(ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER, true);
	if (temp == NULL)
	{
		temp = (BaseTemplate*)mViewOwner->findTempFromParent(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, true);
		if (temp == NULL)
		{
			temp = (BaseTemplate*)mViewOwner->findTempFromParent(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, true);
		}
	}
	if (temp != NULL)
	{
		if (!temp->canWrite())
			return 0;
	}
	if (mViewOwner->getStatus() == BaseConstants::ObjStatus::FREE)
	{
		OperateMgr * operateMgr = mViewOwner->getOperateMgr();
		operateMgr->clearChooseList(true);
		operateMgr->addToChooseList(mViewOwner);
		mViewOwner->changeStatus(BaseConstants::ObjStatus::CLICK_CHOOSE, true);

		if (x > (mViewOwner->getViewParam()->getSelfW()/2))
		{
			mViewOwner->getOperateMgr()->setPress(BaseConstants::MousePressType::RELEASE
				, mViewOwner, BaseConstants::Direction::RIGHT);
		}
		else
		{
			mViewOwner->getOperateMgr()->setPress(BaseConstants::MousePressType::RELEASE
				, mViewOwner, BaseConstants::Direction::LEFT);
		}
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	if (mViewOwner->getStatus() == BaseConstants::ObjStatus::CLICK_CHOOSE)
	{
		BaseViewParam * viewParam = mViewOwner->getViewParam();
		BaseScaleParam * scaleParam = viewParam->getBaseScaleParam();
		scaleParam->judgeStart(x, y);
		mViewOwner->changeStatus(BaseConstants::ObjStatus::DRAG_SCALE, true);
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	return 0;
}

int ImageMouseInput::mouseRelease(int type, int x, int y)
{
	if (type != Qt::LeftButton)
		return false;
	if (mViewOwner->getStatus() == BaseConstants::ObjStatus::FREE)
	{
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	else if (mViewOwner->getStatus() == BaseConstants::ObjStatus::CLICK_CHOOSE)
	{
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	else if (mViewOwner->getStatus() == BaseConstants::ObjStatus::DRAG_SCALE)
	{
		mViewOwner->changeStatus(BaseConstants::ObjStatus::FREE, true);
		BaseViewParam * viewParam = mViewOwner->getViewParam();
		BaseScaleParam * scaleParam = viewParam->getBaseScaleParam();
		scaleParam->judgeStop(x, y);
		OperateMgr * operateMgr = mViewOwner->getOperateMgr();
		operateMgr->clearChooseList(true);
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	return false;
}

int ImageMouseInput::mouseDrag(int type, int x, int y)
{
	if (type != Qt::LeftButton)
		return false;
	if (mViewOwner->getStatus() == BaseConstants::ObjStatus::DRAG_SCALE)
	{
		BaseViewParam * viewParam = mViewOwner->getViewParam();
		BaseScaleParam * scaleParam = viewParam->getBaseScaleParam();
		x -= viewParam->getX();
		scaleParam->doScale(x, y);
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	return false;
}

int ImageMouseInput::mouseDoublePress(int type, int x, int y)
{
	if (type != Qt::LeftButton)
		return false;
	doDoublePress(false);
	return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
}

void ImageMouseInput::doDoublePress(bool isAction)
{
	BaseTemplate * temp =(BaseTemplate*)mViewOwner->findTempFromParent(ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER, true);
	if (temp == NULL)
	{
		temp = (BaseTemplate*)mViewOwner->findTempFromParent(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, true);
		if (temp == NULL)
		{
			temp = (BaseTemplate*)mViewOwner->findTempFromParent(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, true);
		}
	}
	if (temp != NULL)
	{
		if (!temp->canWrite())
			return;
	}
	ImgConTentData * imgData= (ImgConTentData*)mViewOwner->getContentData();
	if (imgData)
	{
		WriterManager * wirteMgr = mViewOwner->getWriterMgr();
		if (wirteMgr)
		{
			MyAppEntrance * app = wirteMgr->getAppEntrance();
			if (app)
			{
				// 取消当前托选项
				OperateMgr * operateMgr = mViewOwner->getOperateMgr();
				operateMgr->clearChooseList(true);

				// 取消txt中自身作为输入焦点
				ViewObj * viewObj = mViewOwner->findViewFromParent(ConstantsID::ContainerId::TXT_WIN_VIEW, true);
				ObjMouseManager * mouseMgr = viewObj->getMouseMgr();
				mouseMgr->setFocus(NULL);
				
				app->OpenEidtImageWindow(imgData);
			}
		}
	}	
}
