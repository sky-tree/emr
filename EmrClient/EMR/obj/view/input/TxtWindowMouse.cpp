#include "TxtWindowMouse.h"




#include "OperateMgr.h"
#include "CrossParentMgr.h"
#include "ExcelCrossParentMgr.h"


#include "ObjMouseManager.h"
#include "ParagraphManager.h"
#include "TempParagraphMgrData.h"


#include "ViewObj.h"
#include "BaseParagraph.h"


#include "ParagraphData.h"
#include "CodeParams.h"


#include "BaseViewParam.h"
#include "LayoutViewParam.h"
#include "RowViewParam.h"

#include "FlagParam.h"

#include "WriterManager.h"
#include "WriterInputManager.h"
#include "SystemParams.h"

#include "ConstantsID.h"
#include "UIConstants.h"
#include "BaseConstants.h"
#include "RightClickMenu.h"
#include "MessageBox.h"

#include "qdebug.h"

TxtWindowMouse::TxtWindowMouse(ViewObj * viewOwner) : BaseMouse(viewOwner, ConstantsID::ClassId::TXT_WIN_MOUSE)
{
	mPagePanel = NULL;
	mPageList = NULL;
	mFocusPartID = ConstantsID::ContainerId::PAGE_CONTENT;
	//if (mViewOwner->getParentViewById(ConstantsID::ContainerId::PAGE_CONTENT))
	//	mPagePanel = mViewOwner->getParentViewById(ConstantsID::ContainerId::PAGE_PANEL);
	//else
	//	mPagePanel = NULL;
	mRightMenu = new RightClickMenu();
}

TxtWindowMouse::~TxtWindowMouse()
{
	mPageList = NULL;
	mPagePanel = NULL;
	if (mRightMenu)
	{
		delete mRightMenu;
		mRightMenu = NULL;
	}
}
ViewObj * TxtWindowMouse::getFocusPage(QList<BaseContainer *> * pageList, int &x, int &y)
{
	BaseViewParam * panelParam = mPagePanel->getViewParam();
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
		// 如果为首页
		if (i == 0)
		{
			// y小于第0页则判断鼠标在第0页
			if (y <= pageY + pageH)
			{
				x -= pageX;
				y -= pageY;
				break;
			}
		}
		// 如果为最后一页
		else if (i == size - 1)
		{
			//if (y >= pageY)
			{
				x -= pageX;
				y -= pageY;
				break;
			}
		}
		// 中间页，如果在页高度范围内
		else if (y <= pageY + pageH)
		{
			x -= pageX;
			y -= pageY;
			break;
		}
	}
	mViewOwner->getOperateMgr()->setEffectLogicPage(focusPage);
	return focusPage;
}
ViewObj * TxtWindowMouse::judgePart(char pressType, ViewObj * page, int x, int y)
{
	ViewObj * part = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_TITLE, false);
	BaseContainer * title = part;
	int partStatus = part->getStatus();
	BaseViewParam * param = part->getViewParam();
	float partY = 0;
	float partH = 0;
	partY = param->getY();
	partH = param->getTotalH();
	char focusType = 0;
	OperateMgr * operateMgr = mOwner->getOperateMgr();
	SystemParams * sys = operateMgr->getSystemParams();
	// 如果页眉为显示状态
	if (param->isView())
	{
		if (sys->canTitleWriteByViewSide())
		{
			if (pressType == BaseConstants::MousePressType::DRAG || pressType == BaseConstants::MousePressType::RELEASE)
			{
				if (partStatus == BaseConstants::ObjStatus::ACTIVE)
				{
					return part;
				}
			}
			// 如果点中页眉
			if (y <= partY + partH)
			{
				return part;
			}
		}
	}
	// 取消页眉激活状态
	part->changeStatus(BaseConstants::ObjStatus::FREE, false);

	part = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_BOTTOM, false);
	BaseContainer * bottom = part;
	partStatus = part->getStatus();
	param = part->getViewParam();
	partY = param->getY();
	// 如果页脚为显示状态
	if (param->isView())
	{
		if (sys->canBottomWriteByViewSide())
		{
			if (pressType == BaseConstants::MousePressType::DRAG || pressType == BaseConstants::MousePressType::RELEASE)
			{
				if (partStatus == BaseConstants::ObjStatus::ACTIVE)
				{
					return part;
				}
			}
			// 如果落点在页脚内
			if (y >= partY)
			{
				return part;
			}
		}
	}
	part = (ViewObj *)page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false);
	if (pressType == BaseConstants::MousePressType::PRESS || pressType == BaseConstants::MousePressType::RELEASE)
	{
		title->changeStatus(BaseConstants::ObjStatus::FREE, false);
		bottom->changeStatus(BaseConstants::ObjStatus::FREE, false);
	}
	x -= param->getX();
	y -= param->getY();

	ViewObj * layer = NULL;


	//int objID = part->getObjID();
	//OperateMgr * operateMgr = mViewOwner->getOperateMgr();
	//switch (objID)
	//{
	//case ConstantsID::ContainerId::PAGE_TITLE:
	//case ConstantsID::ContainerId::PAGE_BOTTOM:
	//	layer = (ViewObj *)part->findChildrenByID(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER, false);
	//	// 设置页眉页脚当前激活页
	//	part->getCrossParentMgr()->setActiveParent(page);
	//	break;
	//case ConstantsID::ContainerId::PAGE_CONTENT:
	//	layer = (ViewObj *)part->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
	//	break;
	//}
	//BaseViewParam * layerParam = layer->getViewParam();

	//x -= layerParam->getX();
	//y -= layerParam->getY();
	return part;
}
ViewObj * TxtWindowMouse::getPagePartTxtLayer(char pressType, ViewObj * page, int &x, int &y)
{
	ViewObj * part = judgePart(pressType, page, x, y);

	int partStatus = part->getStatus();
	//if (partStatus != BaseConstants::ObjStatus::FREE)
	//{
	//	int a = 0;
	//}
	BaseViewParam * param = part->getViewParam();
	float partY = 0;
	float partH = 0;
	partY = param->getY();
	partH = param->getTotalH();
	OperateMgr * operateMgr = mOwner->getOperateMgr();
	// 如果为双击
	if (pressType == BaseConstants::MousePressType::DOUBLE_PRESS)
	{
		//BaseContainer * temp = NULL;
		//TempParagraphMgrData * data = NULL; 
		if (part->isObj(ConstantsID::ContainerId::PAGE_TITLE))
		{
			//temp = operateMgr->getTitleTemp();
			//data = (TempParagraphMgrData *)temp->getContentData();
			//if (data->canWriteByViewSide())
			{
				if (partStatus == BaseConstants::ObjStatus::FREE)
				{
					part->changeStatus(BaseConstants::ObjStatus::ACTIVE, false);
					BaseContainer * pageBottom = page->findChildrenByID(ConstantsID::ContainerId::PAGE_BOTTOM, false);
					pageBottom->changeStatus(BaseConstants::ObjStatus::FREE, false);
				}
			}
		}
		else if (part->isObj(ConstantsID::ContainerId::PAGE_BOTTOM))
		{
			//temp = operateMgr->getTitleTemp();
			//data = (TempParagraphMgrData *)temp->getContentData();
			//if (data->canWriteByViewSide())
			{
				if (partStatus == BaseConstants::ObjStatus::FREE)
				{
					part->changeStatus(BaseConstants::ObjStatus::ACTIVE, false);
					BaseContainer * pageTitle = page->findChildrenByID(ConstantsID::ContainerId::PAGE_TITLE, false);
					pageTitle->changeStatus(BaseConstants::ObjStatus::FREE, false);
				}
			}
		}
	}
	else
	{
		if (part->isObj(ConstantsID::ContainerId::PAGE_TITLE))
		{
			if (partStatus == BaseConstants::ObjStatus::FREE)
			{
				BaseContainer * pageBottom = page->findChildrenByID(ConstantsID::ContainerId::PAGE_BOTTOM, false);
				pageBottom->changeStatus(BaseConstants::ObjStatus::FREE, false);
				return NULL;
			}
		}
		else if (part->isObj(ConstantsID::ContainerId::PAGE_BOTTOM))
		{
			if (partStatus == BaseConstants::ObjStatus::FREE)
			{
				BaseContainer * pageTitle = page->findChildrenByID(ConstantsID::ContainerId::PAGE_TITLE, false);
				pageTitle->changeStatus(BaseConstants::ObjStatus::FREE, false);
				return NULL;
			}
		}
	}

	//ViewObj * part = (ViewObj *)page->findChildrenByID(mFocusPartID, false);
	if (y < param->getY())
	{
	}

	x -= param->getX();
	y -= param->getY();

	ViewObj * layer = NULL;


	int objID = part->getObjID();
	switch (objID)
	{
	case ConstantsID::ContainerId::PAGE_TITLE:
	case ConstantsID::ContainerId::PAGE_BOTTOM:
		layer = (ViewObj *)part->findChildrenByID(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER, false);
		// 设置页眉页脚当前激活页
		part->getCrossParentMgr()->setActiveParent(page);
		break;
	case ConstantsID::ContainerId::PAGE_CONTENT:
		layer = (ViewObj *)part->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
		break;
	}
	BaseViewParam * layerParam = layer->getViewParam();

	x -= layerParam->getX();
	y -= layerParam->getY();
	return layer;
}
ViewObj * TxtWindowMouse::getPointInsideRow(ViewObj * layer, QList<BaseContainer *> * rowList, int &x, int &y)
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
		if (row->isExcelRow())
		{
			ExcelCrossParentMgr * excelCrossMgr = row->getExcelCrossParentMgr();
			if (excelCrossMgr)
			{
				int disY = excelCrossMgr->getDisY(layer);
				int spendH = excelCrossMgr->getSpendH(layer);
				rowY = disY;
				//rowH = spendH;
			}
		}
		// 如果为首行
		//if (i == 0)
		{
			// y小于第0页则判断鼠标在第0行
			if (y <= rowY + rowH)
			{
				x -= rowX;
				//y -= rowY;
				break;
			}
			// 如果为最后一行
			else if (i == size - 1)
			{
				//if (y >= rowY)
				{
					x -= rowX;
					//y -= rowY;
					//break;
				}
			}
		}
	}
	ViewObj * firstView = row->getFirstChildView(false);
	BaseParagraph * paragraph = firstView->getParagraphParent();
	ParagraphData * paragraphData = paragraph->getParagraphData();
	CodeParams * codeParams = paragraphData->getCodeParams();
	if (codeParams &&  codeParams->judgeAuthority() < 1)
	{
		ParagraphManager * mgr = paragraph->getParagraphMgr();
		int idx = paragraph->getIndexFromParagraphMgr() + 1;
		int size = mgr->getParagraphList()->size();
		while (idx < size)
		{
			paragraph = mgr->getParagrapAt(idx);
			idx++;
			paragraphData = paragraph->getParagraphData();
			codeParams = paragraphData->getCodeParams();
			if (!codeParams || codeParams->judgeAuthority() > 0)
			{
				firstView = (ViewObj *)paragraph->getChild(0);
				row = firstView->getViewParent();
				break;
			}
		}
	}
	return row;
}
ViewObj * TxtWindowMouse::getPointInsideObj(OperateMgr * mgr, QList<BaseContainer *> * objList, int x, int y, char & dir, char pressType)
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
			// x小于第0个对象则判断鼠标在第0个对象
			if (x <= objX + objW)
			{
				// 如果为表格
				if (obj->isObj(ConstantsID::ContainerId::EXCEL_ROW_CONTAINER_VIEW))
				{
					//obj->getMouseMgr()->mousePress(pressType, x, y);
					//return NULL;
				}
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
	mgr->setPress(pressType, obj, dir);
	return obj;
}
int TxtWindowMouse::doPress(char type, int x, int y)
{
	OperateMgr * mgr = mViewOwner->getWriterMgr()->getOperateMgr();
	judgeInitPagePanel();
	mPagePanel->changeStatus(BaseConstants::ObjStatus::FREE, false);
	ViewObj * page = getFocusPage(mPageList, x, y);
	ViewObj * txtLayer = getPagePartTxtLayer(type, page, x, y);
	if (!txtLayer)
	{
		return 1;
	}
	QList<BaseContainer *> * rowList = txtLayer->getChildren();
	ViewObj * row = getPointInsideRow(txtLayer, rowList, x, y);
	QList<BaseContainer *> * objList = row->getChildren();
	char dir = 0;
	ViewObj * obj = getPointInsideObj(mgr, objList, x, y, dir, BaseConstants::MousePressType::PRESS_AND_RELEASE);
	// 如果在表格内部,则由表格自身处理
	if (!obj)
	{
		return 1;
	}
	short status = mViewOwner->getStatus();
	// 窗口处于空闲状态
	if (status == BaseConstants::ObjStatus::FREE)
	{
		//mgr->clearChooseList(true);
		mgr->clearChooseList(true);
		mViewOwner->changeStatus(BaseConstants::ObjStatus::DRAG_CHOOSE, false);
	}
	// 窗口处于拖选后松开状态
	else if (status == BaseConstants::ObjStatus::DRAG_CHOOSE_RELEASE || status == BaseConstants::ObjStatus::DRAG_CHOOSE)
	{
		//short objStatus = obj->getStatus();
		//// 如果点选项未被选中,则重新点选
		//if (objStatus == BaseConstants::ObjStatus::FREE)
		//{
		//	mgr->clearChooseList(true);
		//	mViewOwner->changeStatus(BaseConstants::ObjStatus::DRAG_CHOOSE, false);
		//}
		//// 如果按下点对象已经被拖选
		//else if (objStatus == BaseConstants::ObjStatus::DRAG_CHOOSE)
		//{
		//	mgr->clearChooseList(true);
		//	mViewOwner->changeStatus(BaseConstants::ObjStatus::DRAG_CHOOSE, false);
		//}
		mgr->clearChooseList(true);
		mViewOwner->changeStatus(BaseConstants::ObjStatus::DRAG_CHOOSE, false);
	}
	else if (status == BaseConstants::ObjStatus::EXCEL)
	{
		mgr->clearChooseList(true);
		mViewOwner->changeStatus(BaseConstants::ObjStatus::DRAG_CHOOSE, false);
		//mViewOwner->changeStatus(BaseConstants::ObjStatus::FREE, false);
	}
	return 1;
}
void TxtWindowMouse::judgeInitPagePanel()
{
	if (!mPageList)
	{
		mPagePanel = (ViewObj *)mViewOwner->findChildrenByID(ConstantsID::ContainerId::PAGE_PANEL, false);
		mPageList = mPagePanel->getChildren();
	}
}
int TxtWindowMouse::mousePress(int type, int x, int y)
{
	if (type == Qt::RightButton)
	{
		return BaseConstants::NONE;
	}
	//else
	//{
	//	if (mOwner->getWriterMgr()->isFileReadOnly())
	//	{
	//		//MessageBox::showAboutMessageBox(QStringLiteral("病历状态"), QStringLiteral("病历处于阅读状态，不可编辑"));
	//		return 1;
	//	}
	//}
	return doPress(BaseConstants::MousePressType::PRESS, x, y);
}
int TxtWindowMouse::mouseRelease(int type, int x, int y)
{
	if (type == Qt::RightButton)
	{
		mRightMenu->showMenu(false, NULL);
		return 1;
	}
	//else
	//{
	//	if (mOwner->getWriterMgr()->isFileReadOnly())
	//	{
	//		MessageBox::showAboutMessageBox(QStringLiteral("病历状态"), QStringLiteral("病历处于阅读状态，不可编辑"));
	//		return 1;
	//	}
	//}
	int returnType = BaseConstants::NONE;
	OperateMgr * mgr = mViewOwner->getWriterMgr()->getOperateMgr();
	judgeInitPagePanel();
	mPagePanel->changeStatus(BaseConstants::ObjStatus::FREE, false);
	ViewObj * page = getFocusPage(mPageList, x, y);
	ViewObj * txtLayer = getPagePartTxtLayer(BaseConstants::MousePressType::RELEASE, page, x, y);
	if (!txtLayer)
	{
		return 1;
	}
	QList<BaseContainer *> * rowList = txtLayer->getChildren();
	ViewObj * row = getPointInsideRow(txtLayer, rowList, x, y);
	QList<BaseContainer *> * objList = row->getChildren();
	char dir = 0;
	ViewObj * obj = getPointInsideObj(mgr, objList, x, y, dir, BaseConstants::MousePressType::RELEASE);
	// 如果在表格内部,则由表格自身处理
	if (!obj)
	{
		return 1;
	}
	short status = mViewOwner->getStatus();
	if (status == BaseConstants::ObjStatus::DRAG_CHOOSE)
	{
		//mgr->clearChooseList(true);
		mgr->refreshChooseList(true, true, true);
		mViewOwner->changeStatus(BaseConstants::ObjStatus::DRAG_CHOOSE_RELEASE, false);
	}
	return returnType;
}
int TxtWindowMouse::mouseDrag(int type, int x, int y)
{
	if (type == Qt::RightButton)
		return BaseConstants::NONE;
	//else
	//{
	//	if (mOwner->getWriterMgr()->isFileReadOnly())
	//	{
	//		return 1;
	//	}
	//}
	int returnType = BaseConstants::NONE;
	int preY = y;
	OperateMgr * mgr = mViewOwner->getWriterMgr()->getOperateMgr();
	judgeInitPagePanel();
	LayoutViewParam * winParam = (LayoutViewParam *)mViewOwner->getViewParam();
	ViewObj * page = getFocusPage(mPageList, x, y);
	ViewObj * txtLayer = getPagePartTxtLayer(BaseConstants::MousePressType::DRAG, page, x, y);
	if (!txtLayer)
	{
		return 1;
	}
	QList<BaseContainer *> * rowList = txtLayer->getChildren();
	ViewObj * row = getPointInsideRow(txtLayer, rowList, x, y);
	QList<BaseContainer *> * objList = row->getChildren();
	char dir = 0;
	ViewObj * obj = getPointInsideObj(mgr, objList, x, y, dir, BaseConstants::MousePressType::DRAG);
	// 如果在表格内部,则由表格自身处理
	if (!obj)
	{
		return 1;
	}
	short status = mViewOwner->getStatus();
	if (status == BaseConstants::ObjStatus::DRAG_CHOOSE)
	{
		OperateMgr * operateMgr = mViewOwner->getOperateMgr();
		mgr->refreshChooseList(true, true, true);
		int insideT = winParam->getInsideTop();
		int insideH = winParam->getInsideH();
		int insideBottom = insideT + insideH;
		ViewObj * pagePanel = operateMgr->getPagePanel();
		BaseViewParam * pagePanelParam = pagePanel->getViewParam();
		if (preY <= insideT)
		{
			mPagePanel->changeStatus(BaseConstants::ObjStatus::PAGE_PANEL_MOVE_DOWN, false);
		}
		else if (preY >= insideBottom - UIConstants::DEFAULT_PAGE_BOTTOM_DRAG_EFFECT_AREA)
		{
			mPagePanel->changeStatus(BaseConstants::ObjStatus::PAGE_PANEL_MOVE_UP, false);
		}
		else
		{
			mPagePanel->changeStatus(BaseConstants::ObjStatus::FREE, false);
		}
	}
	return returnType;
}
int TxtWindowMouse::mouseMove(int type, int x, int y)
{
	return BaseConstants::NONE;
}
int TxtWindowMouse::mouseDoublePress(int type, int x, int y)
{
	if (type == Qt::RightButton)
		return BaseConstants::NONE;
	int returnType = BaseConstants::NONE;
	return doPress(BaseConstants::MousePressType::DOUBLE_PRESS, x, y);
}