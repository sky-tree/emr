#include "BaseTemplate.h"




#include "WriterManager.h"

#include "ObjKeyManager.h"
#include "ObjMouseManager.h"
#include "ObjDrawManager.h"
#include "TxtWinDrawManager.h"
#include "BaseConstants.h"
#include "BaseViewParam.h"
#include "RelativeChildrenParam.h"
#include "ViewObj.h"
#include "BaseLayout.h"
#include "BaseTempleteMouse.h"
#include "BaseTempleteKeyboard.h"
#include "ConstantsID.h"
#include "StrContentData.h"
#include <QTime>
#include "TempleteTypeEnumeration.h"
#include "TempParagraphMgrData.h"
#include "ParagraphManager.h"
#include "OperateMgr.h"
#include "AddTempletWidgetAction.h"
#include "ExcelCellData.h"
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
BaseTemplate::BaseTemplate(WriterManager * writerMgr, int objID) : BaseContainer(writerMgr, ConstantsID::ClassId::BASE_TEMPLATE, objID)
, mExcelCellData(NULL)
, mInputMouse(NULL)
, mInputKeyboard(NULL)
{
}
BaseTemplate::BaseTemplate(WriterManager * writerMgr, int classId, int objId, QString codeId)
: BaseContainer(writerMgr, classId, objId)
, mExcelCellData(NULL)
, mInputMouse(NULL)
, mInputKeyboard(NULL)
//, mIsCanInsert(true)
{
	mCodeId = codeId;
}
BaseTemplate::BaseTemplate(WriterManager * writerMgr, BaseTemplate*  otherTextWidget, bool canCopyChild) : BaseContainer(writerMgr, otherTextWidget)
,mInputMouse(NULL)
, mInputKeyboard(NULL)
, mExcelCellData(NULL)

{
	//mInputMouse->getClassId();
}
BaseTemplate::~BaseTemplate()
{
	if (mInputMouse)
	{
		delete mInputMouse;
		mInputMouse = NULL;
	}
	if (mInputKeyboard)
	{
		delete mInputKeyboard;
		mInputKeyboard = NULL;
	}
	if (mExcelCellData)
	{
		mExcelCellData = NULL;
	}
}
void BaseTemplate::refreshChildrenTempIndex(int start)
{
	QList<BaseContainer *> * children = getChildren();
	BaseContainer * child = NULL;
	int size = children->size();
	for (int i = start; i < size; i ++)
	{
		child = children->at(i);
		child->setTempIndex(i);
	}
}
bool BaseTemplate::canWrite()
{
	if (!mContentData)
	{
		if (mTempParent)
		{
			return mTempParent->canWrite();
		}
		return true;
	}

	if (mContentData->isCodeAndExtendsData())
	{
		if (!((CodeTempData *)mContentData)->canWrite())
			return false;

		if (mTempParent)
		{
			return mTempParent->canWrite();
		}
	}
	else if (mTempParent)
		return mTempParent->canWrite();
	return true;
}
void BaseTemplate::addAllChildToBufList(QList<BaseContainer *> * children, int start, int num, QList<BaseContainer *> * destViewList)
{
	if (isNullVector(children))
		return;
	if (num == -1)
		num = children->size();
	int end = start + num;
	for (int i = start; i < end; i++)
	{
		BaseContainer * obj = children->at(i);

		if (obj->isTxt())
		{
			destViewList->push_back(obj);
		}
		else
		{
			((BaseTemplate *)obj)->addAllChildToBufList(obj->getChildren(), 0, -1, destViewList);
			//((BaseTemplete *)obj)->addAllChildToView(winData, needResetFlag);
		}
	}
}
bool BaseTemplate::isNullVector(QList<BaseContainer *> * children)
{
	if (children)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//int BaseTemplete::addTempleteViewChildren(QList<ViewObj *> * children, int start, int num, int idx, bool needAddToView)
//{
//	if (num == 0)
//		return 0;
//	// 先无条件添加进模板对应位置，之后根据情况判断怎么添加到显示区域
//	BaseContainer::addChildren((QList<BaseContainer *> *)children, start, num, idx, false);
//
//	if (!needAddToView)
//		return BaseConstants::NONE;
//
//	int returnType = BaseConstants::NONE;
//	addChildrenViewToView(children, start, num);
//	return returnType;
//}

int BaseTemplate::mousePress(int type, int x, int y, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		returnType = mInputMouse->mousePress(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	/*if (mTempleteParent)
	{
	returnType = mTempleteParent->mousePress(type, x, y, obj);
	if (returnType != BaseConstants::NONE)
	{
	return returnType;
	}
	}*/
	return returnType;
}
int BaseTemplate::mouseRelease(int type, int x, int y, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		returnType = mInputMouse->mouseRelease(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	/*if (mTempleteParent)
	{
	returnType = mTempleteParent->mouseRelease(type, x, y, obj);
	if (returnType != BaseConstants::NONE)
	{
	return returnType;
	}
	}*/
	return returnType;
}
int BaseTemplate::mouseMove(int type, int x, int y, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		returnType = mInputMouse->mouseMove(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	/*if (mTempleteParent)
	{
	returnType = mTempleteParent->mouseMove(type, x, y, obj);
	if (returnType != BaseConstants::NONE)
	{
	return returnType;
	}
	}*/
	return returnType;
}
int BaseTemplate::mouseDrag(int type, int x, int y, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		returnType = mInputMouse->mouseDrag(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	/*if (mTempleteParent)
	{
	returnType = mTempleteParent->mouseDrag(type, x, y, obj);
	if (returnType != BaseConstants::NONE)
	{
	return returnType;
	}
	}*/
	return returnType;
}
int BaseTemplate::mouseDoublePress(int type, int x, int y, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		returnType = mInputMouse->mouseDoublePress(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	/*if (mTempleteParent)
	{
	returnType = mTempleteParent->mouseDoublePress(type, x, y, obj);
	if (returnType != BaseConstants::NONE)
	{
	return returnType;
	}
	}*/
	return returnType;
}
int BaseTemplate::mouseScroll(int angle, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		returnType = mInputMouse->mouseScroll(angle, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	/*if (mTempleteParent)
	{
	returnType = mTempleteParent->mouseScroll(angle, this);
	if (returnType != BaseConstants::NONE)
	{
	return returnType;
	}
	}*/
	return returnType;
}
int BaseTemplate::keyPress(QString str, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputKeyboard)
	{
		returnType = mInputKeyboard->keyPress(str, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	/*if (mTempleteParent)
	{
	returnType = mTempleteParent->keyPress(str, obj);
	if (returnType != BaseConstants::NONE)
	{
	return returnType;
	}
	}*/
	return returnType;
}
int BaseTemplate::keyRelease(QString str, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputKeyboard)
	{
		returnType = mInputKeyboard->keyRelease(str, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	/*if (mTempleteParent)
	{
	returnType = mTempleteParent->keyRelease(str, obj);
	if (returnType != BaseConstants::NONE)
	{
	return returnType;
	}
	}*/
	return returnType;
}
void BaseTemplate::setKeyboard(BaseTempleteKeyboard * keyboard)
{
	if (!mInputKeyboard)
	{
		mInputKeyboard = keyboard;
	}
	else
	{
	}

}
void BaseTemplate::setMouse(BaseTempleteMouse * mouse)
{
	if (!mInputMouse)
	{
		mInputMouse = mouse;
	}
	else
	{
	}
}

void BaseTemplate::refreshTxtParam()
{
	//if (!canRefreshTxtParam())
	//{
	//	return;
	//}
	/*DrawTxtParam* focusData = getSelfFocusData();
	if (focusData)
	{
	focusData->refreshTxtParamBySourceParam(getFocusData());
	}
	if (haveChildren())
	{
	for (int i = 0; i < mChildren->size(); i++)
	{
	BaseContainer * obj = mChildren->at(i);
	obj->refreshTxtParam();
	}
	}*/
}
void BaseTemplate::setPageRepaint(ViewObj* currenView)
{
	/*if (currenView)
	{
	ViewObj * row = currenView->getParentViewById(ConstantsID::ContainerId::NORMAL_ROW);
	if (row)
	{
	row->setPageRepaint();
	}
	}
	else
	{
	ViewObj * row = getParentViewById(ConstantsID::ContainerId::NORMAL_ROW);
	if (row)
	{
	row->setPageRepaint();
	}
	}*/
}
bool BaseTemplate::addCodeIdToMap(QMap<QString, QString>& map)
{
	if (mCodeId.trimmed().length() > 0)
	{
		map.insert(TempleteTypeEnumeration::CODE_NAME, mCodeId);
	return true;
	}
	return false;
}
bool BaseTemplate::setCodeIdToTemplete(QMap<QString, QString>& map)
{
	if (map.contains(TempleteTypeEnumeration::CODE_NAME))
	{
	QString tempCode = map.value(TempleteTypeEnumeration::CODE_NAME).trimmed();
	if (tempCode.length() > 0)
	{
	if (tempCode.compare(mCodeId) == 0)
	{
	mCodeId = tempCode;
	return true;
	}
	}
	}
	return false;
}
void BaseTemplate::addDeleteSelfAction(bool isCreateAction)
{
	OperateMgr *  operateMgr  = getOperateMgr();
	if (isCreateAction)
	{
		operateMgr->createNewAction();
	}

	AddTempletWidgetAction * addTempleteAction = new AddTempletWidgetAction(operateMgr->getCurStepMgr());
	ViewObj * deleteAfterObj = this->getNextView();
	addTempleteAction->initDeleteAction(this, (BaseTemplate*)getTempParent(), this->getTempIndex(), this->getLastChildView(), BaseConstants::Direction::RIGHT, deleteAfterObj, BaseConstants::Direction::LEFT);
	operateMgr->addToLocalAction(addTempleteAction);
}
/*
addActionIndex =0 时记录的动作加入当前阶段的动作列表，=1时 动作加入第二阶段动作列表，<0 时不记录动作 默认是=1的
*/
void BaseTemplate::replaceNeedReplaceList(int addActionIndex)
{
	/*BaseWinData* winData = this->getWinData();
	ViewObj* excelPageLayer = this->getParentViewById(ConstantsID::ContainerId::EXCEL_PAGE_LAYER_TXT);
	if (excelPageLayer)
	{
	winData->addToReplaceViewList(excelPageLayer);
	}
	winData->replaceNeedReplaceList();
	if (addActionIndex > 0)
	{
	ReplaceNeedReplaceListAction * replaceAction = new ReplaceNeedReplaceListAction(winData);
	winData->addToLocalActionAt(replaceAction, addActionIndex);
	}*/
}

int BaseTemplate::getMaxWidthOfChildView()
{
	QList<BaseContainer *> * mChildren = getChildren();
	if (!mChildren || mChildren->size() == 0)
	{
		return 0;
	}
	int size = mChildren->size();
	BaseContainer * tempChild;
	int maxWidth = 0;
	int tempWidth;
	for (int i = 0; i < size; i++)
	{
		tempChild = mChildren->at(i);
		if (tempChild->isView())
		{
			if (tempChild->isEnter())
			{
				BaseViewParam *viewParam =((ViewObj*)tempChild)->getViewParam();
				tempWidth = viewParam->getSelfW();
			}
			else
			{
				BaseViewParam *viewParam = ((ViewObj*)tempChild)->getViewParam();
				tempWidth = viewParam->getTotalW();
			}
		}
		else
		{
			tempWidth = ((BaseTemplate*)tempChild)->getMaxWidthOfChildView();
		}
		if (tempWidth > maxWidth)
		{
			maxWidth = tempWidth;
		}
	}
	return maxWidth;
}

//bool BaseTemplete::isChoosedThis(BaseWinData* windata, BaseTemplete* templete)
//{
//	if (windata->getChoosedSize() == 1)
//	{
//		return windata->isChoosed(templete);
//	}
//	else
//	{
//		return false;
//	}
//
//}
void BaseTemplate::getViewObjsAtBehind(int indexStart, QList<ViewObj *> * containerList)
{
	/*if (!mChildren)
	{
	return;
	}
	int childSize = mChildren->size();
	if (childSize<=indexStart)
	{
	return;
	}
	BaseContainer* tempContain;
	for (int i = indexStart; i < childSize;i++)
	{
	tempContain = mChildren->at(i);
	if (tempContain->isViewObj())
	{
	containerList->append((ViewObj *)tempContain);
	}
	else
	{
	((BaseTemplete *)tempContain)->getViewObjsAtBehind(0, containerList);
	}
	}*/
}
void BaseTemplate::getWillShowObjList(QList<ViewObj *> * container)
{
	/*if (!mChildren || (mChildren->size()==0))
	{
	return;
	}
	int size = mChildren->size();
	BaseContainer * tempObj;
	for (int i = 0; i < size;i++)
	{
	tempObj = mChildren->at(i);
	if (tempObj->isViewObj())
	{
	container->append((ViewObj *)tempObj);
	}
	else
	{

	((BaseTemplete *)tempObj)->getWillShowObjList(container);
	}
	}*/
}
void BaseTemplate::addTxtLayerAndReplaceNeedReplaceList(ViewObj* tempObj, bool isExcel)
{
}
template<class T> void BaseTemplate::copyObjListToObjList(QList<T *> * srcList, QList<T *> * destList, bool isClearDestList)
{
	if ((!destList) || (!srcList))
	{
		return;
	}
	if (isClearDestList)
	{
		if (destList->size()>0)
		{
			destList->clear();
		}
	}
	int srcSize = srcList->size();
	for (int i = 0; i < srcSize; i++)
	{
		destList->push_back(srcList->at(i));
	}
}
void BaseTemplate::copyObjListToObjList(QList<ViewObj *> * srcList, QList<BaseContainer *> * destList, bool isClearDestList)
{

	if ((!destList) || (!srcList))
	{
		return;
	}
	if (isClearDestList)
	{
		if (destList->size()>0)
		{
			destList->clear();
		}
	}
	int srcSize = srcList->size();
	for (int i = 0; i < srcSize; i++)
	{
		destList->push_back(srcList->at(i));
	}
}
void BaseTemplate::setViewObjColor(ViewObj * obj, int color)
{
	int objId = obj->getObjID();
	if (objId != ConstantsID::ContainerId::TXT && objId != ConstantsID::ContainerId::TMPLATE_TXT)
	{
		return;
	}
	StrContentData * strData = (StrContentData*)obj->getContentData();
	strData->setFontColor(color);
}
bool BaseTemplate::isFocousObj(BaseElement* currentObj)
{
	ViewObj* focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus,dir);
	if (currentObj == focus)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void BaseTemplate::deleteChild(int start, int num, bool refreshIdx, bool isRefreshNow)
{
    QList<BaseContainer *> *  children = getChildren();
	if (!children)
	{
		return ;
	}
	if (num == -1)
		num = children->size() - start;
	if (num < 1)
	{
		return ;
	}
	BaseChildrenManager * childrenMgr = getChildrenMgr();
	if (isObjHaveParagraphMgr(childrenMgr))
	{
		childrenMgr->delChildren(start, 1, refreshIdx);
	}
	else
	{
		deleteViewObjOnShow(getChildren(), start, num, refreshIdx, isRefreshNow);
		childrenMgr->delChildren(start, num, refreshIdx);
	}
	
}
void BaseTemplate::deleteChild(BaseContainer * child, bool refreshIdx, bool isRefreshNow)
{
	int start = getChildren()->indexOf(child);
	if (start>=0)
	{
		BaseChildrenManager * childrenMgr = getChildrenMgr();
		if (isObjHaveParagraphMgr(childrenMgr))
		{
			childrenMgr->delChildren(start, 1, refreshIdx);
		}
		else
		{
			deleteViewObjOnShow(getChildren(), start, 1, refreshIdx, isRefreshNow);
			childrenMgr->delChildren(start, 1, refreshIdx);
		}
	}
}
void BaseTemplate::deleteViewObjOnShow(QList<BaseContainer *> * children, int start, int num, bool refreshIdx, bool isRefreshNow)
{
	ViewObj * focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus, dir);
	ViewObj* focusAtNextView = NULL;
	//ViewObj * deleteFirstView = NULL;
	if (children)
	{
		/*BaseContainer* firstObj = children->at(0);
		firstObj=firstObj->findFirstElementFromChildren();
		if (firstObj&&firstObj->isView())
		{
			deleteFirstView = ((ViewObj *)firstObj);
			deleteFirstView->setFocus(false, false);
		}*/
		if (num==-1)
		{
			num = children->size();
		}
		if (num<=0)
		{
			return;
		}
		int endIndex = start + num - 1;
		if (endIndex>=children->size())
		{
		}
		else
		{
			focusAtNextView = children->at(endIndex)->getNextView();
		}
	
	}else
	{
	}
	if (focus)
	{
		BaseContainer* tempContainer = NULL;
		int tempIdx = -1;
		focus->findNormalTempContainerFromParent(tempContainer, tempIdx);
		if (tempContainer->isObj(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER))
		{
			TempParagraphMgrData * data = (TempParagraphMgrData*)tempContainer->getContentData();
			data->getParagraphMgr()->delChildren(children, start, num, refreshIdx);
		}
		else
		{
			CodeTempData * data = (CodeTempData*)tempContainer->getContentData();
			data->getParentParagraphMgr()->delChildren(children, start, num, refreshIdx);
		}
		/*if (focus)
		{
			if (dir == BaseConstants::Direction::RIGHT)
			{
				focus->setFocus(true,false);
			}
			else
			{
				focus->setFocus(false, false);
			}
		}
		else*/
		if (focusAtNextView)
		{
			ViewObj* parentView = focusAtNextView->getViewParent();
			if (parentView)
			{
				focusAtNextView->setFocus(false);
			}
		}
		else
		{
		}

	}
	
	//data->getParagraphMgr()->delChildren(children, start, num, refreshIdx);
}
void BaseTemplate::addChildren(QList<BaseContainer *> * addList, int index, bool refreshIdx, bool isRefreshNow)
{
	BaseChildrenManager * childrenMgr = getChildrenMgr();
	childrenMgr->addChildren(addList, 0, addList->size(), index, refreshIdx);
	if (isObjHaveParagraphMgr(childrenMgr))
	{
		return;
	}
	addViewObjOnShow(addList, refreshIdx, isRefreshNow);
}
void BaseTemplate::addChild(BaseContainer *child, int index, bool refreshIdx, bool isRefreshNow)
{
	BaseChildrenManager * childrenMgr = getChildrenMgr();
	childrenMgr->addChild(child, index, refreshIdx);
	if (isObjHaveParagraphMgr(childrenMgr))
	{
		return ;
	}
	QList<BaseContainer *>  addList;
	addList.push_back(child);
	addViewObjOnShow(&addList, refreshIdx, isRefreshNow);
}
void BaseTemplate::addViewObjOnShow(QList<BaseContainer *> * addList, bool refreshIdx, bool isRefreshNow)
{
	ViewObj * focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus, dir);
	BaseContainer* tempContainer = NULL;
	int tempIdx = -1;
	focus->findNormalTempContainerFromParent(tempContainer, tempIdx);
	if (tempContainer->isObj(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER))
	{
		TempParagraphMgrData * data = (TempParagraphMgrData*)tempContainer->getContentData();
		data->getParagraphMgr()->addChildren(addList, refreshIdx, NULL, -1, isRefreshNow);
	}
	else
	{
		CodeTempData *data = (CodeTempData*)tempContainer->getContentData();
		data->getParentParagraphMgr()->addChildren(addList, refreshIdx, NULL, -1, isRefreshNow);
	}
	
}
void BaseTemplate::deleteSelf(bool isCreateAction)
{
	ViewObj* lastView = getLastChildView();
	ViewObj *newtView = lastView->getNextView();
	OperateMgr * oprateMgr = getOperateMgr();
	oprateMgr->clearChooseList(true);
	addDeleteSelfAction(isCreateAction);
	//getTempleteParent()->delChild(this, true);
	this->getTempParent()->getChildrenMgr()->delChild(this, true);
	if (newtView)
	{
		newtView->setFocus(false);
	}
	//winData->getFlagObj()->getViewParam()->setViewType(BaseConstants::ViewType::NORMAL);
	replaceNeedReplaceList();
}
bool BaseTemplate::isSetFlowType(StrContentData* focusData)
{
	if (focusData->mOprateType == SetTxtType::FlowType)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void BaseTemplate::setExcelCellData(ExcelCellData * cellData)
{
	mExcelCellData = cellData;
}
ExcelCellData* BaseTemplate::getExcelCellData()
{
	return mExcelCellData;
}
BaseTemplate* BaseTemplate::getTemplateParentInExcelCell()
{
	return mExcelCellData->getViewOwner()->getTempParent();
}
void BaseTemplate::setFirstChildViewFocus(bool isFlagLeft,bool isFreshStrData)
{
	ViewObj* firstView =  getFirstChildView();
	firstView->setFocus(!isFlagLeft, isFreshStrData);
}
QString BaseTemplate::getCodeId()
{
	return mCodeId;
}
bool BaseTemplate::isObjHaveParagraphMgr(BaseChildrenManager* childManger)
{
	if (childManger->mClassID == ConstantsID::ClassId::TEMP_CONTAINER_MGR 
		|| childManger->mClassID == ConstantsID::ClassId::NODE_MGR)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool BaseTemplate::setDrawTxtParam(StrContentData* focusData)
{
	QList<BaseContainer *> * childrenList = getChildren();
	if (childrenList)
	{
		BaseContainer* obj;
		bool isSuccess =true;
		for (int i = 0; i < childrenList->size();i++)
		{
			obj = childrenList->at(i);
			if (!obj->setDrawTxtParam(focusData))
			{
				isSuccess = false;
				break;
			}
			
		}
		return isSuccess;
	}
	return true;
}
BaseParagraph * BaseTemplate::getParagraphParent()
{
	BaseContainer * firstElement =findFirstElementFromChildren(true);
	if (!firstElement)
	{
		return NULL;
	}
	BaseParagraph *paragraph = firstElement->getParagraphParent();
	return paragraph;
}