#include "ExcelCellPageMouse.h"




#include "OperateMgr.h"
#include "CrossParentMgr.h"

#include "ViewObj.h"

#include "BaseViewParam.h"
#include "LayoutViewParam.h"
#include "RowViewParam.h"

#include "FlagParam.h"

#include "WriterManager.h"
#include "WriterInputManager.h"
#include "ObjMouseManager.h"
#include "ExcelCellData.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "ExcelTemplete.h"
#include "qdebug.h"
#include "BaseMath.h"
#include "ExcelMouseInputManager.h"
#include "VerticalViewParam.h"
#include "SystemParams.h"
ExcelCellPageMouse::ExcelCellPageMouse(ViewObj * viewOwner) : BaseMouse(viewOwner, ConstantsID::ClassId::EXCEL_PAGE_MOUSE)
{
	mPagePanel = NULL;
	mPageList = NULL;
	mFocusPartID = ConstantsID::ContainerId::PAGE_CONTENT;
	//if (mViewOwner->getParentViewById(ConstantsID::ContainerId::PAGE_CONTENT))
	//	mPagePanel = mViewOwner->getParentViewById(ConstantsID::ContainerId::PAGE_PANEL);
	//else
	//	mPagePanel = NULL;
}

ExcelCellPageMouse::~ExcelCellPageMouse()
{
	mPageList = NULL;
	mPagePanel = NULL;
}
//ViewObj * ExcelCellPageMouse::getFocusPage(QList<BaseContainer *> * pageList, int &x, int &y)
//{
//	BaseViewParam * panelParam = mPagePanel->getViewParam();
//	x -= panelParam->getX();
//	y -= panelParam->getY();
//
//	ViewObj * focusPage = NULL;
//	LayoutViewParam * param = NULL;
//	int size = pageList->size();
//	float pageX = 0;
//	float pageY = 0;
//	float pageH = 0;
//	for (int i = 0; i < pageList->size(); i++)
//	{
//		focusPage = (ViewObj *)pageList->at(i);
//		param = (LayoutViewParam *)focusPage->getViewParam();
//		pageX = param->getX();
//		pageY = param->getY();
//		pageH = param->getTotalH();
//		// 如果为首页
//		if (i == 0)
//		{
//			// y小于第0页则判断鼠标在第0页
//			if (y <= pageY + pageH)
//			{
//				x -= pageX;
//				y -= pageY;
//				break;
//			}
//		}
//		// 如果为最后一页
//		else if (i == size - 1)
//		{
//			if (y >= pageY)
//			{
//				x -= pageX;
//				y -= pageY;
//				break;
//			}
//		}
//		// 中间页，如果在页高度范围内
//		else if (y >= pageY && y <= pageY + pageH)
//		{
//			x -= pageX;
//			y -= pageY;
//			break;
//		}
//	}
//	mViewOwner->getOperateMgr()->setEffectLogicPage(focusPage);
//	return focusPage;
//}
ViewObj * ExcelCellPageMouse::getPagePartTxtLayer(char pressType, ViewObj * page, int &x, int &y)
{

	ViewObj * layer = NULL;


	
	layer = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::EXCEL_TXT_LAYER, false);
	BaseViewParam * layerParam = layer->getViewParam();

	x -= layerParam->getX();
	y -= layerParam->getY();
	return layer;
}
ViewObj * ExcelCellPageMouse::getPointInsideRow(QList<BaseContainer *> * rowList, int &x, int &y)
{
	ViewObj * row = NULL;
	RowViewParam * param = NULL;
	int size = rowList->size();
	float rowX = 0;
	float rowY = 0;
	float rowH = 0;
	for (int i = 0; i < rowList->size(); i++)
	{
		row = (ViewObj *)rowList->at(i);
		param = (RowViewParam *)row->getViewParam();
		rowX = param->getX();
		rowY = param->getY();
		rowH = param->getTotalH();
		// 如果为首页
		//if (i == 0)
		{
			// y小于第0页则判断鼠标在第0页
			if (y <= rowY + rowH)
			{
				x -= rowX;
				y -= rowY;
				break;
			}
			// 如果为最后一行
			else if (i == size - 1)
			{
				//if (y >= rowY)
				{
					x -= rowX;
					y -= rowY;
					//break;
				}
			}
		}
		//// 如果为最后一行
		//else if (i == size - 1)
		//{
		//	//if (y >= rowY)
		//	{
		//		x -= rowX;
		//		y -= rowY;
		//		break;
		//	}
		//}
		//// 中间页，如果在页高度范围内
		//else if (y <= rowY + rowH)
		//{
		//	x -= rowX;
		//	y -= rowY;
		//	break;
		//}
	}
	return row;
}
ViewObj * ExcelCellPageMouse::getPointInsideObj(OperateMgr * mgr, QList<BaseContainer *> * objList, int x, char & dir, char pressType)
{
	int size = objList->size();
	ViewObj * obj = NULL;
	BaseViewParam * param = NULL;
	float objX = 0;
	int objW = 0;

	for (int i = 0; i < size; i++)
	{
		obj = (ViewObj *)objList->at(i);
		param = obj->getViewParam();
		objX = param->getX();
		objW = param->getTotalW();
		// 如果为首页
		if (i == 0)
		{
			// y小于第0页则判断鼠标在第0页
			if (x <= objX + objW)
			{
				if (x > objX + objW / 2)
				{
					dir = BaseConstants::Direction::RIGHT;
				}
				else
					dir = BaseConstants::Direction::LEFT;
				break;
			}
			// 本行只有一个
			else if (i == size - 1)
			{
				if (obj->isEnter())
				{
					dir = BaseConstants::Direction::LEFT;
				}
				else
				{
					dir = BaseConstants::Direction::RIGHT;
				}
				break;
			}
		}
		// 如果为最后一页
		else if (i == size - 1)
		{
			if (x >= objX)
			{
				if (x < objX + objW / 2)
					dir = BaseConstants::Direction::LEFT;
				else
					dir = BaseConstants::Direction::RIGHT;
				break;
			}
		}
		// 中间页，如果在页高度范围内
		else if (x >= objX && x <= objX + objW)
		{
			if (x < objX + objW / 2)
				dir = BaseConstants::Direction::LEFT;
			else
				dir = BaseConstants::Direction::RIGHT;
			break;
		}
	}
	if (pressType == BaseConstants::MousePressType::PRESS || pressType == BaseConstants::MousePressType::DOUBLE_PRESS)
	{
		mgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, obj, dir);
	}
	else
		mgr->setPress(pressType, obj, dir);
	return obj;
}
int ExcelCellPageMouse::doPress(char type, int x, int y)
{
	int preX = x;
	int preY = y;
	OperateMgr * mgr = mViewOwner->getWriterMgr()->getOperateMgr();
	mgr->setEffectLogicPage(mgr->getEffectLoopPage());
	ViewObj * titleBottom = mViewOwner->findViewFromParent(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER, true);
	if (titleBottom)
	{
		SystemParams * sys = mViewOwner->getOperateMgr()->getSystemParams();
		if (sys->canTitleWriteByViewSide())
		{
			ViewObj * part = titleBottom->getViewParent();
			int status = part->getStatus();
			if (status == BaseConstants::ObjStatus::FREE)
			{
				if (type == BaseConstants::MousePressType::DOUBLE_PRESS)
				{
					part->changeStatus(BaseConstants::ObjStatus::ACTIVE, false);
				}
				else
					return 0;
			}
		}
		else
		{
			return 0;
		}
		
	}
	/*if (!mPageList)
	{
		mPagePanel = (ViewObj *)mViewOwner->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
		mPageList = mPagePanel->getChildren();
	}*/
	//ViewObj * page = getFocusPage(mPageList, x, y);
	ViewObj * page = mViewOwner;
	ViewObj * txtLayer = getPagePartTxtLayer(type, page, x, y);
	QList<BaseContainer *> * rowList = txtLayer->getChildren();
	ViewObj * row = getPointInsideRow(rowList, x, y);
	QList<BaseContainer *> * objList = row->getChildren();
	char dir = 0;
	ViewObj * obj = getPointInsideObj(mgr, objList, x, dir, BaseConstants::MousePressType::PRESS);
	short status = mViewOwner->getStatus();
	// 表格处于空闲状态
	if (status == BaseConstants::ObjStatus::FREE)
	{
		//mgr->clearChooseList(true);
		mgr->clearChooseList(true);
		mViewOwner->changeStatus(BaseConstants::ObjStatus::DRAG_CHOOSE, false);
	}
	// 窗口处于拖选后松开状态
	else if (status == BaseConstants::ObjStatus::DRAG_CHOOSE_RELEASE || status == BaseConstants::ObjStatus::DRAG_CHOOSE)
	{
		mgr->clearChooseList(true);
		mViewOwner->changeStatus(BaseConstants::ObjStatus::DRAG_CHOOSE, false);
	}
	// 如果为双击，同时聚焦在图片上
	if (type == BaseConstants::MousePressType::DOUBLE_PRESS && obj->isObj(ConstantsID::ContainerId::IMG))
	{
		return 0;
	}
	return 1;
}
int ExcelCellPageMouse::mousePress(int type, int x, int y)
{
	if (type == Qt::RightButton)
		return BaseConstants::NONE;
	
	if (!getCanEdit())
	{
		return 1;
	}
	return doPress(BaseConstants::MousePressType::PRESS, x, y);
}
int ExcelCellPageMouse::mouseRelease(int type, int x, int y)
{
	if (type == Qt::RightButton)
		return BaseConstants::NONE;
	if (!getCanEdit())
	{
		return 1;
	}
	ExcelTemplete *excelTemp = (ExcelTemplete*)mViewOwner->getTempParent();
	if (excelTemp->getPressPage() == mViewOwner)
	{
		int returnType = BaseConstants::NONE;
		OperateMgr * mgr = mViewOwner->getWriterMgr()->getOperateMgr();
		ViewObj * titleBottom = mViewOwner->findViewFromParent(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER, true);
		if (titleBottom)
		{
			ViewObj * part = titleBottom->getViewParent();
			int status = part->getStatus();
			if (status == BaseConstants::ObjStatus::FREE)
			{
				return 0;
			}
		}
		ViewObj * page = mViewOwner;
		ViewObj * txtLayer = getPagePartTxtLayer(BaseConstants::MousePressType::RELEASE, page, x, y);
		QList<BaseContainer *> * rowList = txtLayer->getChildren();
		ViewObj * row = getPointInsideRow(rowList, x, y);
		QList<BaseContainer *> * objList = row->getChildren();
		char dir = 0;
		ViewObj * obj = getPointInsideObj(mgr, objList, x, dir, BaseConstants::MousePressType::RELEASE);
		short status = mViewOwner->getStatus();
		if (status == BaseConstants::ObjStatus::DRAG_CHOOSE)
		{
			mgr->refreshChooseList(true, true, true);
			mViewOwner->changeStatus(BaseConstants::ObjStatus::DRAG_CHOOSE_RELEASE, false);
			return 1;
		}
		else
		{
			int a = 0;
		}
		excelTemp->setPressPage(NULL);
		return returnType;
	}
	else
	{
		excelTemp->setPressPage(NULL);
		return 0;
	}
	
}
int ExcelCellPageMouse::mouseDrag(int type, int x, int y)
{
	
	if (type == Qt::RightButton)
		return BaseConstants::NONE;
	ViewObj * titleBottom = mViewOwner->findViewFromParent(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER, true);
	if (titleBottom)
	{
		ViewObj * part = titleBottom->getViewParent();
		int status = part->getStatus();
		if (status == BaseConstants::ObjStatus::FREE)
		{
			return 0;
		}
	}
	ExcelTemplete *excelTemp = (ExcelTemplete*)mViewOwner->getTempParent();
	if (!excelTemp->getIsPressOnExcel())
	{
		return 1;
	}
	if (excelTemp->getPressPage() == mViewOwner)
	{
		int returnType = BaseConstants::NONE;
		int preY = y;
		OperateMgr * mgr = mViewOwner->getWriterMgr()->getOperateMgr();
		ViewObj * page = mViewOwner;
		ViewObj * txtLayer = getPagePartTxtLayer(BaseConstants::MousePressType::DRAG, page, x, y);
		QList<BaseContainer *> * rowList = txtLayer->getChildren();
		ViewObj * row = getPointInsideRow(rowList, x, y);
		QList<BaseContainer *> * objList = row->getChildren();
		char dir = 0;
		ViewObj * obj = getPointInsideObj(mgr, objList, x, dir, BaseConstants::MousePressType::DRAG);
		short cellPageStatus = mViewOwner->getStatus();
		ViewObj* windObj = mViewOwner->findViewFromParent(ConstantsID::ContainerId::TXT_WIN_VIEW, true);
		if (windObj)
		{
			short winStatus = windObj->getStatus();
			if (winStatus == BaseConstants::ObjStatus::EXCEL)
			{
				return returnType;
			}
		}
		if (cellPageStatus == BaseConstants::ObjStatus::DRAG_CHOOSE)
		{
			if (!mgr->hadExcelCellInChooseList())
			{
				mgr->refreshChooseList(true, true, true);
			}

		}
		return returnType;
	}
	else
	{
		return  BaseConstants::NONE;
	}
	
}
int ExcelCellPageMouse::mouseMove(int type, int x, int y)
{
	return BaseConstants::NONE;
}
int ExcelCellPageMouse::mouseDoublePress(int type, int x, int y)
{
	if (!getCanEdit())
	{
		return 1;
	}
	return doPress(BaseConstants::MousePressType::DOUBLE_PRESS, x, y);
}
bool ExcelCellPageMouse::getCanEdit()
{
	ExcelCellData* cellData = (ExcelCellData*)mViewOwner->getContentData();
	return cellData->getCanEdit();
}