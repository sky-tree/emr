#include "TempleteViewObjMouseManager.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "BaseViewParam.h"
#include "ViewObj.h"
#include "BaseMath.h"
#include "BaseMouse.h"
#include "BaseTemplate.h"
TempleteViewObjMouseManager::TempleteViewObjMouseManager(ViewObj * view) :ObjMouseManager(view, ConstantsID::ClassId::TEMP_VIEWOBJ_MOUSE_MANAGER)
//, mMouse(NULL)
{
}


TempleteViewObjMouseManager::~TempleteViewObjMouseManager()
{
}
int TempleteViewObjMouseManager::mousePress(int type, int x, int y)
{
	if (mOwner->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
	{
		int i = 0;
	}
	int objId = mOwner->getObjID();
	if (objId == ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW)
	{
		int i = 0;
	}
	bool isHaveO = true;
	if (type == Qt::RightButton)
	{
		if (!(mOwner->isObj(ConstantsID::ContainerId::EXCEL_PAGE) || mOwner->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW)))
		{
			if (!mOwner->canEditTempleteProperty())
			{
				isHaveO = false;
			}
		}
		
	}
	int returnType = BaseConstants::NONE;
	if (isHaveO)
	{
		int preX = x;
		int preY = y;
		BaseViewParam * viewParam = ((ViewObj *)mOwner)->getViewParam();
		x -= viewParam->getX();
		if (mOwner->isObj(ConstantsID::ContainerId::TMPLATE_TXT))
		{
			y =  1;
		}
		else
		{
			y -= viewParam->getY();
		}
		
		int totalW = viewParam->getTotalW();
		int totalH = viewParam->getTotalH();
		//当时焦点对象时，不判断范围
		ViewObj* windowView = (ViewObj*)mOwner->getParentViewById(ConstantsID::ContainerId::TXT_WIN_VIEW);
		ViewObj* inputFocus = NULL;
		bool needCharge = true;
		if (windowView)
		{
			inputFocus = windowView->getMouseMgr()->getFocus();
			if (inputFocus&&inputFocus == mOwner)
			{
				needCharge = false;
			}
		}
		if (needCharge)
		{
			// 如果超出自身范围则不处理
			if (!BaseMath::isPointerInsizeRect(x, y, 0, 0, totalW, totalH))
			{
				return returnType;
			}
		}
		
		if (mMouse)
		{
			returnType = mMouse->mousePress(type, x, y);
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}

		BaseContainer * tempParent = mOwner->getTempParent();
		if (tempParent)
		{
			if (tempParent->isTemplateElement())
			{
				returnType = ((BaseTemplate*)tempParent)->mousePress(type, x, y, mOwner);
			}
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}
	}
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			returnType = child->getMouseMgr()->mousePress(type, x, y);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}

	if (returnType > BaseConstants::NONE)
		return returnType;
	return  BaseConstants::NONE;
}
int TempleteViewObjMouseManager::mouseRelease(int type, int x, int y)
{
	//开放鼠标点击
	/*bool isHaveO = false;
	if (type == Qt::RightButton){
		if (mOwner->canEditTempleteProperty()){
			isHaveO = true;
		}
	}
	else{
		if (mOwner->canEditTemplete()){
			isHaveO = true;
		}
	}*/
	bool isHaveO = true;
	if (type == Qt::RightButton)
	{
		if (!(mOwner->isObj(ConstantsID::ContainerId::EXCEL_PAGE) || mOwner->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW)))
		{
			if (!mOwner->canEditTempleteProperty())
			{
				isHaveO = false;
			}
		}

	}
	int returnType = BaseConstants::NONE;
	if (isHaveO)
	{
	
		BaseViewParam * viewParam = ((ViewObj *)mOwner)->getViewParam();
		x -= viewParam->getX();
		y -= viewParam->getY();
		int totalW = viewParam->getTotalW();
		int totalH = viewParam->getTotalH();

		//当时焦点对象时，不判断范围
		ViewObj* windowView = (ViewObj*)mOwner->getParentViewById(ConstantsID::ContainerId::TXT_WIN_VIEW);
		ViewObj* inputFocus = NULL;
		bool needCharge = true;
		if (windowView)
		{
			//BaseViewParam *  viewParam1 = mTemptViewFocus->getViewParam();
			//returnValue = mVirtulExcel->getInputManager()->mouseRelease(type, viewParam1->getX() + x, viewParam1->getY() + y);
			inputFocus = windowView->getMouseMgr()->getFocus();
			if (inputFocus&&inputFocus == mOwner)
			{
				needCharge = false;
			}
		}
		if (needCharge)
		{
			// 如果超出自身范围则不处理
			if (!BaseMath::isPointerInsizeRect(x, y, 0, 0, totalW, totalH))
			{
				/*if (mObjId == ConstantsID::ContainerId::NORMAL_PAGE)
				int a = 0;
				if (isBlock(BaseConstants::BlockType::ONLY_MOUSE_OUT_SIDE))
				return BaseConstants::ReturnType::RETURN_ONE_LEVEL;*/
				return returnType;
			}
		}
		
		
		BaseContainer * tempParent = mOwner->getTempParent();
		if (tempParent)
		{
			if (tempParent->isTemplateElement())
			{
				returnType = ((BaseTemplate*)tempParent)->mouseRelease(type, x, y, mOwner);
			}
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}
		if (mMouse)
		{
			returnType = mMouse->mouseRelease(type, x, y);
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}

		
	}
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			returnType = child->getMouseMgr()->mouseRelease(type, x, y);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}

	return returnType;
}
int TempleteViewObjMouseManager::mouseMove(int type, int x, int y)
{
	int returnType = BaseConstants::NONE;
	if (mOwner->canEditTemplete())
	{
		BaseViewParam * viewParam = ((ViewObj *)mOwner)->getViewParam();
		x -= viewParam->getX();
		y -= viewParam->getY();
		int totalW = viewParam->getTotalW();
		int totalH = viewParam->getTotalH();
		/*bool isExcelRowContainer = mOwner->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW);
		ExcelTemplete excelTemp = NULL;
		if (isExcelRowContainer)
		{
			mOwner->getTempParent();
		}*/
		// 如果超出自身范围则不处理
		if (!BaseMath::isPointerInsizeRect(x, y, 0, 0, totalW, totalH))
		{
			/*if (mObjId == ConstantsID::ContainerId::NORMAL_PAGE)
			int a = 0;
			if (isBlock(BaseConstants::BlockType::ONLY_MOUSE_OUT_SIDE))
			return BaseConstants::ReturnType::RETURN_ONE_LEVEL;*/
			
			return returnType;
		}
		/*if (mOwner->getObjID() == ConstantsID::ContainerId::EXCEL_PAGE)
		{
			int a = 0;
		}*/
		if (mMouse)
		{
			returnType = mMouse->mouseMove(type, x, y);
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}

		BaseContainer * tempParent = mOwner->getTempParent();
		if (tempParent)
		{
			if (tempParent->isTemplateElement())
			{
				returnType = ((BaseTemplate*)tempParent)->mouseMove(type, x, y, mOwner);
			}
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}
	}
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			returnType = child->getMouseMgr()->mouseMove(type, x, y);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}
	
	return returnType;
}
int TempleteViewObjMouseManager::mouseDrag(int type, int x, int y)
{
	int returnType = BaseConstants::NONE;
	/*bool isHaveO = false;
	if (type == Qt::RightButton){
		if (mOwner->canEditTempleteProperty()){
			isHaveO = true;
		}
	}
	else{
		if (mOwner->canEditTemplete()){
			isHaveO = true;
		}
	}
	if (isHaveO)*/
	{
		BaseViewParam * viewParam = ((ViewObj *)mOwner)->getViewParam();
		x -= viewParam->getX();
		if (mOwner->isObj(ConstantsID::ContainerId::TMPLATE_TXT))
		{
			y = 1;
		}
		else
		{
			y -= viewParam->getY();
		}
		int totalW = viewParam->getTotalW();
		int totalH = viewParam->getTotalH();

		//当时焦点对象时，不判断范围
		ViewObj* windowView = (ViewObj*)mOwner->getParentViewById(ConstantsID::ContainerId::TXT_WIN_VIEW);
		ViewObj* inputFocus = NULL;
		bool needCharge = true;
		if (windowView)
		{
			//BaseViewParam *  viewParam1 = mTemptViewFocus->getViewParam();
			//returnValue = mVirtulExcel->getInputManager()->mouseRelease(type, viewParam1->getX() + x, viewParam1->getY() + y);
			inputFocus = windowView->getMouseMgr()->getFocus();
			if (inputFocus&&inputFocus == mOwner)
			{
				needCharge = false;
			}
		}
		if (needCharge)
		{
			// 如果超出自身范围则不处理
			if (!BaseMath::isPointerInsizeRect(x, y, 0, 0, totalW, totalH))
			{
				/*if (mObjId == ConstantsID::ContainerId::NORMAL_PAGE)
				int a = 0;
				if (isBlock(BaseConstants::BlockType::ONLY_MOUSE_OUT_SIDE))
				return BaseConstants::ReturnType::RETURN_ONE_LEVEL;*/
				return returnType;
			}
		}
		
	
		BaseContainer * tempParent = mOwner->getTempParent();
		if (tempParent)
		{
			if (tempParent->isTemplateElement())
			{
				returnType = ((BaseTemplate*)tempParent)->mouseDrag(type, x, y, mOwner);
			}
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}
		if (mMouse)
		{
			returnType = mMouse->mouseDrag(type, x, y);
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}

		
	}
	
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			returnType = child->getMouseMgr()->mouseDrag(type, x, y);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}
	return returnType;
}
int TempleteViewObjMouseManager::mouseDoublePress(int type, int x, int y)
{
	int returnType = BaseConstants::NONE;
	if (mOwner->canEditTemplete())
	{
		BaseViewParam * viewParam = ((ViewObj *)mOwner)->getViewParam();
		x -= viewParam->getX();
		if (mOwner->isObj(ConstantsID::ContainerId::TMPLATE_TXT))
		{
			y = 1;
		}
		else
		{
			y -= viewParam->getY();
		}
		int totalW = viewParam->getTotalW();
		int totalH = viewParam->getTotalH();
		// 如果超出自身范围则不处理
		if (!BaseMath::isPointerInsizeRect(x, y, 0, 0, totalW, totalH))
		{
			/*if (mObjId == ConstantsID::ContainerId::NORMAL_PAGE)
			int a = 0;
			if (isBlock(BaseConstants::BlockType::ONLY_MOUSE_OUT_SIDE))
			return BaseConstants::ReturnType::RETURN_ONE_LEVEL;*/
			return returnType;
		}

		if (mOwner->haveChildren() > 0)
		{
			QList<BaseContainer *> * children = mOwner->getChildren();
			int start = children->size() - 1;
			ViewObj * child = NULL;
			for (int i = start; i >= 0; i--)
			{
				child = (ViewObj *)children->at(i);
				returnType = child->getMouseMgr()->mouseDoublePress(type, x, y);
				if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
				{
					return returnType;
				}
			}
		}

		if (mMouse)
		{
			returnType = mMouse->mouseDoublePress(type, x, y);
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}

		BaseContainer * tempParent = mOwner->getTempParent();
		if (tempParent)
		{
			if (tempParent->isTemplateElement())
			{
				returnType = ((BaseTemplate*)tempParent)->mouseDoublePress(type, x, y, mOwner);
			}
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}

	}
	return returnType;
}
int TempleteViewObjMouseManager::mouseScroll(int angle)
{
	int returnType = BaseConstants::NONE;
	if (mMouse)
	{
		returnType = mMouse->mouseScroll(angle);
		if (returnType > BaseConstants::NONE)
		{
			return returnType;
		}
	}
	
	BaseContainer * tempParent = mOwner->getTempParent();
	if (tempParent)
	{
		if (tempParent->isTemplateElement())
		{
			returnType = ((BaseTemplate*)tempParent)->mouseScroll(angle, mOwner);

		}
		if (returnType > BaseConstants::NONE)
		{
			return returnType;
		}
	}
	
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			returnType = child->getMouseMgr()->mouseScroll(angle);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}
	
	return returnType;
}