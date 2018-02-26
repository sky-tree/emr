#include "ObjMouseManager.h"
#pragma execution_character_set("utf-8")


#include "BaseMouse.h"

#include "ExcelCrossParentMgr.h"
#include "OperateMgr.h"
#include "WriterManager.h"

#include "ViewObj.h"
#include "BaseViewParam.h"
#include "RowViewParam.h"


#include "MessageBox.h"
#include "BaseMath.h"
#include "BaseConstants.h"
#include "ConstantsID.h"
#include "BaseSystem.h"


ObjMouseManager::ObjMouseManager(ViewObj * view) : BaseContainerParam(view, ConstantsID::ClassId::OBJ_MOUSE_MANAGER)
, mInputFocus(NULL)
, mMouse(NULL)
{
}
ObjMouseManager::ObjMouseManager(ViewObj * view, int classID) : BaseContainerParam(view, classID)
, mInputFocus(NULL)
, mMouse(NULL)
{

}
ObjMouseManager::ObjMouseManager(ViewObj * view, ObjMouseManager * source) : BaseContainerParam(view, source->getClassId())
, mInputFocus(NULL)
, mMouse(NULL)
{
	mMouse = BaseSystem::getInstance()->getMouseInputCopy(view, source->getMouse());
}
ObjMouseManager::~ObjMouseManager()
{
	if (mMouse)
	{
		delete mMouse;
		mMouse = NULL;
	}
}
void ObjMouseManager::setMouse(BaseMouse * mouse)
{
	if (mMouse)
	{
		delete mMouse;
	}
	mMouse = mouse;
}
BaseMouse * ObjMouseManager::getMouse()
{
	return mMouse;
}
void ObjMouseManager::setFocus(ViewObj * focus)
{
	if (mInputFocus && mInputFocus != focus)
	{
		mInputFocus->releaseFromWindow();
	}
	mInputFocus = focus;
}
ViewObj* ObjMouseManager::getFocus()
{
	return mInputFocus;
}
int ObjMouseManager::mousePress(int type, int x, int y)
{
	BaseViewParam * viewParam = ((ViewObj *)mOwner)->getViewParam();
	if (!viewParam->isView())
		return BaseConstants::NONE;
	//// ���Ϊֻ��ģʽ��������pagePanel�ϵ��������룬�ұ߽�����������ſ�
	//if (mOwner->getWriterMgr()->isFileReadOnly())
	//{
	//	if (mOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
	//	{
	//		return 1;
	//	}
	//}
	int returnType = BaseConstants::NONE;

	int preX = x;
	int preY = y;
	x -= viewParam->getX();
	int selfY = 0;
	if (mOwner->isExcelRow())
	{
		OperateMgr * operateMgr = mOwner->getOperateMgr();
		operateMgr->getEffectLogicPage();
		ViewObj * excelRow = (ViewObj *)mOwner;
		ExcelCrossParentMgr * crossMgr = excelRow->getExcelCrossParentMgr();
		if (crossMgr && crossMgr->haveCrossParent())
		{
			ViewObj * loopPage = operateMgr->getEffectLoopPage();
			operateMgr->setEffectLogicPage(loopPage);
			ViewObj * layer = (ViewObj *)loopPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			selfY = crossMgr->getDisY(layer);
		}
		else
			selfY = viewParam->getY();
	}
	else
		selfY = viewParam->getY();
	y -= selfY;

	//bool needBlock = isBlock(BaseConstants::BlockType::ONLY_MOUSE_ALL_SIDE);

	int totalW = viewParam->getTotalW();
	int totalH = viewParam->getTotalH();
	// �����������Χ�򲻴���
	if (!BaseMath::isPointerInsizeRect(x, y, 0, 0, totalW, totalH))
	{
		/*if (mObjId == ConstantsID::ContainerId::NORMAL_PAGE)
			int a = 0;
			if (isBlock(BaseConstants::BlockType::ONLY_MOUSE_OUT_SIDE))
			return BaseConstants::ReturnType::RETURN_ONE_LEVEL;*/
		return returnType;
	}

	// ����н�����������ȴ�����������룬���Ҳ��ж����뷶Χ
	if (mInputFocus)
	{
		BaseViewParam * focusViewParan = mInputFocus->getViewParam();
		if (!focusViewParan->isView())
		{
			return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		}
		ViewObj * viewOwner = (ViewObj *)mOwner;
		BaseViewParam * focusParentViewParan = mInputFocus->getViewParent()->getViewParam();
		int parentAbsX = focusParentViewParan->getAbsX(viewOwner);
		int parentAbsY = focusParentViewParan->getAbsY(viewOwner);
		y += selfY;
		x -= parentAbsX + focusViewParan->getX();
		y -= parentAbsY + focusViewParan->getY();
		BaseMouse *  tempMouse = mInputFocus->getMouseMgr()->getMouse();
		if (tempMouse)
		{
			returnType = tempMouse->mousePress(type, x, y);
		}
		else
		{
			returnType = mInputFocus->getMouseMgr()->mousePress(type, x, y);
		}
		if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
		{
			return returnType;
		}
	}
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		OperateMgr * operateMgr = mOwner->getOperateMgr();
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			// ����ǰҳ����Ϊ�ҳ
			if (mOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
			{
				operateMgr->setEffectLoopPage(child);
			}
			returnType = child->getMouseMgr()->mousePress(type, x, y);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}
	if (mMouse)
	{
		return	mMouse->mousePress(type, x, y);
	}
	//if (!mInputManager)
	//{
	//	if (needBlock)
	//		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	//	return returnType;
	//}
	//returnType = mInputManager->mousePress(type, x, y);
	if (returnType > BaseConstants::NONE)
		return returnType;
	//if (needBlock)
	//	return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	return returnType;
}
int ObjMouseManager::mouseRelease(int type, int x, int y)
{
	BaseViewParam * viewParam = ((ViewObj *)mOwner)->getViewParam();
	if (!viewParam->isView())
		return BaseConstants::NONE;
	//// ��������Ҽ������ж�
	//if (type != Qt::RightButton)
	//{
	//	// ���Ϊֻ��ģʽ��������pagePanel�ϵ��������룬�ұ߽�����������ſ�
	//	if (mOwner->getWriterMgr()->isFileReadOnly())
	//	{
	//		if (mOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
	//		{
	//			MessageBox::showAboutMessageBox(QStringLiteral("����״̬"), QStringLiteral("���������Ķ�״̬�����ɱ༭"));
	//			return 1;
	//		}
	//	}
	//}
	OperateMgr * operateMgr = mOwner->getOperateMgr();
	int returnType = BaseConstants::NONE;
	int preX = x;
	int preY = y;
	x -= viewParam->getX();
	int selfY = 0;
	if (mOwner->isExcelRow())
	{
		ViewObj * excelRow = (ViewObj *)mOwner;
		ExcelCrossParentMgr * crossMgr = excelRow->getExcelCrossParentMgr();
		if (crossMgr && crossMgr->haveCrossParent())
		{
			ViewObj * loopPage = operateMgr->getEffectLoopPage();
			ViewObj * layer = (ViewObj *)loopPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			selfY = crossMgr->getDisY(layer);
		}
		else
			selfY = viewParam->getY();
	}
	else
		selfY = viewParam->getY();
	y -= selfY;
	int totalW = viewParam->getTotalW();
	int totalH = viewParam->getTotalH();
	// �����������Χ�򲻴���
	if (!BaseMath::isPointerInsizeRect(x, y, 0, 0, totalW, totalH))
	{
		/*if (mObjId == ConstantsID::ContainerId::NORMAL_PAGE)
		int a = 0;
		if (isBlock(BaseConstants::BlockType::ONLY_MOUSE_OUT_SIDE))
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;*/
		return returnType;
	}

	// ����н�����������ȴ�����������룬���Ҳ��ж����뷶Χ
	if (mInputFocus)
	{
		BaseViewParam * focusViewParan = mInputFocus->getViewParam();
		if (!focusViewParan->isView())
		{
			return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		}

		exchangeCrossPageExcelXYWhenFocusInput(focusViewParan, x, y);

		BaseMouse *  tempMouse = mInputFocus->getMouseMgr()->getMouse();
		if (tempMouse)
		{
			tempMouse->mouseRelease(type, x, y);
		}
		else
		{
			mInputFocus->getMouseMgr()->mouseRelease(type, x, y);
		}
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			// ����ǰҳ����Ϊ�ҳ
			if (mOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
			{
				operateMgr->setEffectLoopPage(child);
			}
			returnType = child->getMouseMgr()->mouseRelease(type, x, y);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}
	if (mMouse)
	{
		return	mMouse->mouseRelease(type, x, y);
	}
	return returnType;
}
int ObjMouseManager::mouseMove(int type, int x, int y)
{
	BaseViewParam * viewParam = ((ViewObj *)mOwner)->getViewParam();
	if (!viewParam->isView())
		return BaseConstants::NONE;
	//// ���Ϊֻ��ģʽ��������pagePanel�ϵ��������룬�ұ߽�����������ſ�
	//if (mOwner->getWriterMgr()->isFileReadOnly())
	//{
	//	if (mOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
	//	{
	//		return 1;
	//	}
	//}
	OperateMgr * operateMgr = mOwner->getOperateMgr();
	int returnType = BaseConstants::NONE;
	int preX = x;
	int preY = y;

	x -= viewParam->getX();
	int selfY = 0;
	if (mOwner->isExcelRow())
	{
		ViewObj * excelRow = (ViewObj *)mOwner;
		ExcelCrossParentMgr * crossMgr = excelRow->getExcelCrossParentMgr();
		if (crossMgr && crossMgr->haveCrossParent())
		{
			ViewObj * loopPage = operateMgr->getEffectLoopPage();
			ViewObj * layer = (ViewObj *)loopPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			selfY = crossMgr->getDisY(layer);
		}
		else
			selfY = viewParam->getY();
	}
	else
		selfY = viewParam->getY();
	y -= selfY;

	int totalW = viewParam->getTotalW();
	int totalH = viewParam->getTotalH();
	// �����������Χ�򲻴���
	if (!BaseMath::isPointerInsizeRect(x, y, 0, 0, totalW, totalH))
	{
		/*if (mObjId == ConstantsID::ContainerId::NORMAL_PAGE)
		int a = 0;
		if (isBlock(BaseConstants::BlockType::ONLY_MOUSE_OUT_SIDE))
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;*/
		return returnType;
	}

	// ����н�����������ȴ�����������룬���Ҳ��ж����뷶Χ
	if (mInputFocus)
	{
		BaseViewParam * focusViewParan = mInputFocus->getViewParam();
		if (!focusViewParan->isView())
		{
			return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		}
		//ViewObj * viewOwner = (ViewObj *)mOwner;
		//BaseViewParam * focusParentViewParan = mInputFocus->getViewParent()->getViewParam();
		//int parentAbsX = focusParentViewParan->getAbsX(viewOwner);
		//int parentAbsY = focusParentViewParan->getAbsY(viewOwner);
		//x -= parentAbsX + focusViewParan->getX();
		//y -= parentAbsY + focusViewParan->getY();
		exchangeCrossPageExcelXYWhenFocusInput(focusViewParan, x, y);

		BaseMouse *  tempMouse = mInputFocus->getMouseMgr()->getMouse();
		if (tempMouse)
		{
			tempMouse->mouseMove(type, x, y);
		}
		else
		{
			mInputFocus->getMouseMgr()->mouseMove(type, x, y);
		}
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			// ����ǰҳ����Ϊ�ҳ
			if (mOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
			{
				operateMgr->setEffectLoopPage(child);
			}
			returnType = child->getMouseMgr()->mouseMove(type, x, y);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}
	if (mMouse)
	{
		return	mMouse->mouseMove(type, x, y);
	}
	return returnType;
}
int ObjMouseManager::mouseDrag(int type, int x, int y)
{
	BaseViewParam * viewParam = ((ViewObj *)mOwner)->getViewParam();
	if (!viewParam->isView())
		return BaseConstants::NONE;
	OperateMgr * operateMgr = mOwner->getOperateMgr();
	//// ���Ϊֻ��ģʽ��������pagePanel�ϵ��������룬�ұ߽�����������ſ�
	//if (mOwner->getWriterMgr()->isFileReadOnly())
	//{
	//	if (mOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
	//	{
	//		return 1;
	//	}
	//}
	int returnType = BaseConstants::NONE;
	int preX = x;
	int preY = y;
	x -= viewParam->getX();
	int selfY = 0;
	if (mOwner->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
	{
		int a = 0;
	}
	if (mOwner->isExcelRow())
	{
		ViewObj * excelRow = (ViewObj *)mOwner;
		ExcelCrossParentMgr * crossMgr = excelRow->getExcelCrossParentMgr();
		if (crossMgr && crossMgr->haveCrossParent())
		{
			ViewObj * loopPage = operateMgr->getEffectLoopPage();
			ViewObj * layer = (ViewObj *)loopPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			selfY = crossMgr->getDisY(layer);
		}
		else
			selfY = viewParam->getY();
	}
	else
		selfY = viewParam->getY();
	y -= selfY;
	int totalW = viewParam->getTotalW();
	int totalH = viewParam->getTotalH();
	// �����������Χ�򲻴���
	if (!BaseMath::isPointerInsizeRect(x, y, 0, 0, totalW, totalH))
	{
		return returnType;
	}

	// ����н�����������ȴ�����������룬���Ҳ��ж����뷶Χ
	if (mInputFocus)
	{
		BaseViewParam * focusViewParan = mInputFocus->getViewParam();
		if (!focusViewParan->isView())
		{
			return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		}
		exchangeCrossPageExcelXYWhenFocusInput(focusViewParan, x, y);
		BaseMouse *  tempMouse = mInputFocus->getMouseMgr()->getMouse();
		if (tempMouse)
		{
			tempMouse->mouseDrag(type, x, y);
		}
		else
		{
			mInputFocus->getMouseMgr()->mouseDrag(type, x, y);
		}
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			if (child->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
			{
				int a = 0;
			}
			// ����ǰҳ����Ϊ�ҳ
			if (mOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
			{
				operateMgr->setEffectLoopPage(child);
			}
			returnType = child->getMouseMgr()->mouseDrag(type, x, y);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}
	if (mMouse)
	{
		return	mMouse->mouseDrag(type, x, y);
	}
	return returnType;
}
int ObjMouseManager::mouseDoublePress(int type, int x, int y)
{
	BaseViewParam * viewParam = ((ViewObj *)mOwner)->getViewParam();
	if (!viewParam->isView())
		return BaseConstants::NONE;
	//// ���Ϊֻ��ģʽ��������pagePanel�ϵ��������룬�ұ߽�����������ſ�
	//if (mOwner->getWriterMgr()->isFileReadOnly())
	//{
	//	if (mOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
	//	{
	//		return 1;
	//	}
	//}
	OperateMgr * operateMgr = mOwner->getOperateMgr();
	int returnType = BaseConstants::NONE;
	int preX = x;
	int preY = y;
	x -= viewParam->getX();
	int selfY = 0;
	if (mOwner->isExcelRow())
	{
		ViewObj * excelRow = (ViewObj *)mOwner;
		ExcelCrossParentMgr * crossMgr = excelRow->getExcelCrossParentMgr();
		if (crossMgr && crossMgr->haveCrossParent())
		{
			ViewObj * loopPage = operateMgr->getEffectLoopPage();
			ViewObj * layer = (ViewObj *)loopPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			selfY = crossMgr->getDisY(layer);
		}
		else
			selfY = viewParam->getY();
	}
	else
		selfY = viewParam->getY();
	y -= selfY;
	int totalW = viewParam->getTotalW();
	int totalH = viewParam->getTotalH();
	// �����������Χ�򲻴���
	if (!BaseMath::isPointerInsizeRect(x, y, 0, 0, totalW, totalH))
	{
		/*if (mObjId == ConstantsID::ContainerId::NORMAL_PAGE)
		int a = 0;
		if (isBlock(BaseConstants::BlockType::ONLY_MOUSE_OUT_SIDE))
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;*/
		return returnType;
	}

	// ����н�����������ȴ�����������룬���Ҳ��ж����뷶Χ
	if (mInputFocus)
	{
		BaseViewParam * focusViewParan = mInputFocus->getViewParam();
		if (!focusViewParan->isView())
		{
			return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		}
		exchangeCrossPageExcelXYWhenFocusInput(focusViewParan, x, y);
		BaseMouse *  tempMouse = mInputFocus->getMouseMgr()->getMouse();
		if (tempMouse)
		{
			tempMouse->mouseDoublePress(type, x, y);
		}
		else
		{
			mInputFocus->getMouseMgr()->mouseDoublePress(type, x, y);
		}
		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			// ����ǰҳ����Ϊ�ҳ
			if (mOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
			{
				operateMgr->setEffectLoopPage(child);
			}
			returnType = child->getMouseMgr()->mouseDoublePress(type, x, y);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}
	if (mMouse)
	{
		return	mMouse->mouseDoublePress(type, x, y);
	}
	return returnType;
}
int ObjMouseManager::mouseScroll(int angle)
{
	BaseViewParam * viewParam = ((ViewObj *)mOwner)->getViewParam();
	if (viewParam->getViewType() < BaseConstants::ViewType::NORMAL)
		return BaseConstants::NONE;
	int returnType = BaseConstants::NONE;

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
	if (mMouse)
	{
		return	mMouse->mouseScroll(angle);
	}
	return returnType;
}

int ObjMouseManager::setMiddleButtonParam(int y, int pageH, int totalH)
{
	if (mMouse)
		return	mMouse->setMiddleButtonParam(y, pageH, totalH);
	return 0;
}
ViewObj * ObjMouseManager::getFocusPage(ViewObj * pagePanel, int x, int y)
{
	QList<BaseContainer *> * pageList = pagePanel->getChildren();
	BaseViewParam * panelParam = pagePanel->getViewParam();
	x -= panelParam->getX();
	y -= panelParam->getY();

	ViewObj * focusPage = NULL;
	LayoutViewParam * param = NULL;
	int size = pageList->size();
	float pageX = 0;
	float pageY = 0;
	float pageH = 0;
	for (int i = 0; i < pageList->size(); i++)
	{
		focusPage = (ViewObj *)pageList->at(i);
		param = (LayoutViewParam *)focusPage->getViewParam();
		pageX = param->getX();
		pageY = param->getY();
		pageH = param->getTotalH();
		// ���Ϊ��ҳ
		if (i == 0)
		{
			// yС�ڵ�0ҳ���ж�����ڵ�0ҳ
			if (y <= pageY + pageH)
			{
				break;
			}
		}
		// ���Ϊ���һҳ
		else if (i == size - 1)
		{
			if (y >= pageY)
			{
				break;
			}
		}
		// �м�ҳ�������ҳ�߶ȷ�Χ��
		else if (y >= pageY && y <= pageY + pageH)
		{
			break;
		}
	}
	mOwner->getOperateMgr()->setEffectLogicPage(focusPage);
	return focusPage;
}
void ObjMouseManager::exchangeCrossPageExcelXYWhenFocusInput(BaseViewParam * focusViewParan, int & x, int & y)
{
	ViewObj * viewOwner = (ViewObj *)mOwner;
	OperateMgr * operateMgr = mOwner->getOperateMgr();
	int parentAbsX = 0;
	int parentAbsY = 0;
	int disY = focusViewParan->getY();
	bool isCrossExcel = false;
	ViewObj * parentRow = mInputFocus->getViewParent();
	BaseViewParam * parentRowViewParan = parentRow->getViewParam();
	parentAbsX = parentRowViewParan->getAbsX(viewOwner);
	int layerY = 0;
	int spendH = 0;
	int preY = y;
	if (parentRow->isExcelRow())
	{
		ExcelCrossParentMgr * crossMgr = parentRow->getExcelCrossParentMgr();
		// ��������δ��ҳ
		if (crossMgr && crossMgr->haveCrossParent())
		{
			isCrossExcel = true;
			ViewObj * pagePanel = (ViewObj *)mOwner->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
			getFocusPage(pagePanel, x, y);
			ViewObj * logicPage = operateMgr->getEffectLogicPage();
			ViewObj * layer = (ViewObj *)logicPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			disY += crossMgr->getDisY(layer);
			BaseViewParam * layerParam = layer->getViewParam();
			layerY = layerParam->getAbsY(viewOwner);
			if (y < layerY)
			{
				y = layerY;
			}
			else
			{
				spendH = crossMgr->getSpendH(layer);
				if (layer == crossMgr->getFirstCrossParent())
				{
					layerY += crossMgr->getDisY(layer);
				}
				layerY += spendH;
				if (y > layerY)
				{
					y = layerY;
				}
			}
			parentAbsY = layerParam->getAbsY(viewOwner);
		}
	}
	if (!isCrossExcel)
	{
		parentAbsY = parentRowViewParan->getAbsY(viewOwner);
	}
	x -= parentAbsX;// +focusViewParan->getX();
	y -= parentAbsY + disY;
}
