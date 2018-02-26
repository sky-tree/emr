#include "SetTxtStrParamAction.h"

#include "TempContainerMgr.h"

#include "StepActionManger.h"

#include "OperateMgr.h"
#include "WriterManager.h"
#include "ExcelCellData.h"
#include "BaseTemplate.h"

#include "BaseContentData.h"
#include "StrContentData.h"

#include "FlagParam.h"



#include "BaseContainer.h"
#include "ViewObj.h"
#include "ExcelTemplete.h"
#include "BaseLayout.h"

#include "ContentDataFactory.h"

#include "BaseTools.h"
#include "BaseSystem.h"
#include "ConstantsID.h"

SetTxtStrParamAction::SetTxtStrParamAction(StepActionManger * mgr) : BaseAction(mgr, ConstantsID::ClassId::SET_TXT_STR_PARAM_ACTOIN)
{
	mObjList = new QList<BaseContainer *>();
	//mDataList = new QList<BaseContentData *>();
	mPreValueList = new QList<QList<QString> *>();
}

SetTxtStrParamAction::~SetTxtStrParamAction()
{
	mOperateMgr = NULL;
}
void SetTxtStrParamAction::doAction(QList<BaseContainer *> * list, char type, QString value)
{
	mActionType = type;
	mDestValue = value;
	int size = list->size();
	BaseContainer * obj = NULL;
	BaseContentData * data = NULL;
	BaseTemplate * tempParent = NULL;
	////BaseTools::exchangeListToViewList(list, mViewObjList);
	//for (int i = 0; i < size; i++)
	//{
	//	obj = list->at(i);
	//	mObjList->push_back(obj);
	//	if (obj->isView())
	//	{
	//		if (obj->isTxt())
	//		{
	//			data = obj->getContentData();
	//		}
	//	}
	//	else
	//	{
	//		data = obj->getSelfFocusData();
	//	}
	//	//	mDataList->push_back(NULL);
	//	QString preValue = getPreValue(data);
	//	mPreValueList->push_back(preValue);
	//}
	QString preValue = NULL;
	QList<QString> * tempValue = new QList<QString>();
	QList<BaseContainer *> * tempAllViewChildren = mOperateMgr->getWriterMgr()->getClearTempParagraphViewList();
	for (int i = 0; i < size; i++)
	{
		obj = list->at(i);
		tempValue = new QList<QString>();
		mObjList->push_back(obj);
		if (obj->isView())
		{
			if (obj->isTxt())
			{
				data = (StrContentData *)obj->getContentData();
				preValue = getPreValue(data);
				tempValue->push_back(preValue);
			}
			else if (obj->isObj(ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION))
			{
				tempParent = obj->getTempParent();
				data = tempParent->getSelfFocusData();
				tempValue->push_back(preValue);
			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
			{
				ExcelCellData*	cellData = (ExcelCellData *)obj->getContentData();
				if (cellData)
				{
					BaseTools::exchangeObjToViewList(cellData->mRootTemplete, tempAllViewChildren);
					for (int j = 0; j < tempAllViewChildren->size(); j++)
					{
						BaseContainer * tempObj = tempAllViewChildren->at(j);
						if (tempObj->isTxt())
						{
							data = (StrContentData *)tempObj->getContentData();
						}
						else if (tempObj->isObj(ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION))
						{
							tempParent = tempObj->getTempParent();
							data = tempParent->getSelfFocusData();
						}
						else
						{
							data = obj->getSelfFocusData();
						}
						preValue = getPreValue(data);
						tempValue->push_back(preValue);
					}
				}
			}
		}
		else if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
		{
			BaseTools::getObjListFromExcel(obj, tempAllViewChildren);
			for (int j = 0; j < tempAllViewChildren->size(); j++)
			{
				BaseContainer * tempObj = tempAllViewChildren->at(j);
				if (tempObj->isTxt())
				{
					data = (StrContentData *)tempObj->getContentData();
				}
				else if (tempObj->isObj(ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION))
				{
					tempParent = tempObj->getTempParent();
					data = tempParent->getSelfFocusData();
				}
				else
				{
					data = obj->getSelfFocusData();
				}
				preValue = getPreValue(data);
				tempValue->push_back(preValue);
			}
		}
		else
		{
			BaseTools::exchangeObjToViewList(obj, tempAllViewChildren);
			for (int j = 0; j < tempAllViewChildren->size(); j++)
			{
				BaseContainer * tempObj = tempAllViewChildren->at(j);
				if (tempObj->isTxt())
				{
					data = (StrContentData *)tempObj->getContentData();
				}
				else
				{
					data = obj->getSelfFocusData();
				}
				preValue = getPreValue(data);
				tempValue->push_back(preValue);
			}
		}

		mPreValueList->push_back(tempValue);
		tempAllViewChildren->clear();
	}
}
void SetTxtStrParamAction::unDo()
{
	int size = mObjList->size();
	BaseContainer * obj = NULL;
	BaseContentData * data = NULL;
	BaseTemplate * tempParent = NULL;
	QString preValue = 0;
	StrContentData * tempData = new StrContentData(NULL, NULL);
	QList<BaseContainer *> * tempAllViewChildren = mOperateMgr->getWriterMgr()->getClearTempParagraphViewList();
	for (int i = 0; i < size; i++)
	{
		tempAllViewChildren->clear();
		obj = mObjList->at(i);
		if (obj->isView())
		{
			if (obj->isTxt())
			{
				data = obj->getContentData();
				if (data)
				{
					preValue = mPreValueList->at(i)->at(0);
					setParamByTypeAndFreshLayOut(data, mActionType, preValue,(ViewObj*)obj);
				}
			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
			{
				ExcelCellData *	cellData = (ExcelCellData *)obj->getContentData();
				if (cellData)
				{
					BaseTools::exchangeObjToViewList(cellData->mRootTemplete, tempAllViewChildren);
					for (int j = 0; j < tempAllViewChildren->size(); j++)
					{
						BaseContainer * tempObj = tempAllViewChildren->at(j);
						preValue = mPreValueList->at(i)->at(j);
						if (tempObj->isTxt())
						{
							data = tempObj->getContentData();
							setParamByTypeAndFreshLayOut(data, mActionType, preValue,(ViewObj*)tempObj);
						}
						// 公式
						else if (tempObj->isObj(ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION))
						{
							tempParent = tempObj->getTempParent();
							data = tempParent->getSelfFocusData();
							setParamByTypeAndFreshLayOut((StrContentData *)data, mActionType, preValue, (ViewObj*)tempObj);
							tempParent->setDrawTxtParam((StrContentData *)data);
						}
						else if (tempObj->isEnterImgOrHLine())
						{
						}
						else
						{
							switch (mActionType)
							{
							case BaseConstants::TxtParamActionType::SET_FAMILY:
								tempData->setOperateType(Family);
								tempData->setFamily(preValue);
								break;
							}
							tempObj->setDrawTxtParam(tempData);
						}
					}

					mOperateMgr->judgeListLess(tempAllViewChildren, true);
					mOperateMgr->judgeListOverflow(tempAllViewChildren, false);
				}
			}
		}
		else if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
		{
			BaseTools::getObjListFromExcel(obj, tempAllViewChildren);
			for (int j = 0; j < tempAllViewChildren->size(); j++)
			{
				BaseContainer * tempObj = tempAllViewChildren->at(j);
				preValue = mPreValueList->at(i)->at(j);
				if (tempObj->isView())
				{
					if (tempObj->isTxt())
					{
						data = tempObj->getContentData();
						setParamByTypeAndFreshLayOut(data, mActionType, preValue,(ViewObj*)tempObj);
					}
					// 公式
					else if (tempObj->isObj(ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION))
					{
						tempParent = tempObj->getTempParent();
						data = tempParent->getSelfFocusData();
						setParamByTypeAndFreshLayOut((StrContentData *)data, mActionType, preValue, (ViewObj*)tempObj);
						tempParent->setDrawTxtParam((StrContentData *)data);
					}
				}
				else
				{
					switch (mActionType)
					{
					case BaseConstants::TxtParamActionType::SET_FAMILY:
						tempData->setOperateType(Family);
						tempData->setFamily(preValue);
						break;
					}
					tempObj->setDrawTxtParam(tempData);
				}
			}

			((ExcelTemplete *)obj)->requestLayout(true, true);
			//BaseContainer * excelView = obj->getChild(0);
			//QList<BaseContainer *> * excelPageList = excelView->getChildren();
			//BaseContainer * excelPage = NULL;
			//for (int j = 0; j < excelPageList->size(); j++)
			//{
			//	excelPage = excelPageList->at(j);
			//	BaseTools::getObjListFromExcel(obj, tempAllViewChildren);
			//	mOperateMgr->judgeListLess(tempAllViewChildren, true);
			//	mOperateMgr->judgeListOverflow(tempAllViewChildren, false);
			//}
		}
		else
		{
			BaseTools::exchangeObjToViewList(obj, tempAllViewChildren);
			for (int j = 0; j < tempAllViewChildren->size(); j++)
			{
				BaseContainer * tempObj = tempAllViewChildren->at(j);
				preValue = mPreValueList->at(i)->at(j);
				if (tempObj->isTxt())
				{
					data = tempObj->getContentData();
					setParamByTypeAndFreshLayOut(data, mActionType, preValue, (ViewObj*)tempObj);
				}
				else if (tempObj->isObj(ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION))
				{
					tempParent = tempObj->getTempParent();
					tempData->refreshTxtParamBySourceParam(tempParent->getSelfFocusData(), false);
					setParamByTypeAndFreshLayOut(tempData, mActionType, preValue, (ViewObj *)tempObj);
					tempParent->setDrawTxtParam(tempData);
				}
				else if (tempObj->isEnterImgOrHLine())
				{
				}
				else
				{
					switch (mActionType)
					{
					case BaseConstants::TxtParamActionType::SET_FAMILY:
						tempData->setOperateType(Family);
						tempData->setFamily(preValue);
						break;
					}
					tempObj->setDrawTxtParam(tempData);
				}
			}
		}
	}

	delete tempData;

	mOperateMgr->judgeListLess(mObjList, true);
	mOperateMgr->judgeListOverflow(mObjList, false);

	FlagParam * pressEnd = mOperateMgr->getPressEnd();
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, pressEnd->getFocusObj(), pressEnd->getDirection());
	mOperateMgr->clearChooseList(true);
	mOperateMgr->clearBufferExcelTemp();
}
void SetTxtStrParamAction::reDo()
{
	int size = mObjList->size();
	BaseContentData * data = NULL;
	int preValue = 0;
	BaseContainer * obj = NULL;
	BaseTemplate * tempParent = NULL;
	StrContentData * tempData = new StrContentData(NULL, NULL);
	switch (mActionType)
	{
	case BaseConstants::TxtParamActionType::SET_FAMILY:
		tempData->setOperateType(Family);
		tempData->setFamily(mDestValue);
		break;
	}
	QList<BaseContainer *> * tempAllViewChildren = mOperateMgr->getWriterMgr()->getClearTempParagraphViewList();
	for (int i = 0; i < size; i++)
	{
		obj = mObjList->at(i);
		if (obj->isView())
		{
			if (obj->isTxt())
			{
				data = obj->getContentData();
				if (data)
				{
					setParamByTypeAndFreshLayOut(data, mActionType, mDestValue, (ViewObj*)obj);
				}
			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
			{
				ExcelCellData *	cellData = (ExcelCellData *)obj->getContentData();
				if (cellData)
				{
					BaseTools::exchangeObjToViewList(cellData->mRootTemplete, tempAllViewChildren);
					for (int j = 0; j < tempAllViewChildren->size(); j++)
					{
						BaseContainer * tempObj = tempAllViewChildren->at(j);
						if (tempObj->isTxt())
						{
							data = (StrContentData *)tempObj->getContentData();
							setParamByTypeAndFreshLayOut(data, mActionType, mDestValue, (ViewObj*)tempObj);
						}
						// 公式
						else if (tempObj->isObj(ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION))
						{
							tempParent = tempObj->getTempParent();
							data = tempParent->getSelfFocusData();
							setParamByTypeAndFreshLayOut((StrContentData *)data, mActionType, mDestValue, (ViewObj*)tempObj);
							tempParent->setDrawTxtParam((StrContentData *)data);
						}
						else if (tempObj->isEnterImgOrHLine())
						{
						}
						else
						{
							tempObj->setDrawTxtParam(tempData);
						}
					}
					mOperateMgr->judgeListLess(tempAllViewChildren, true);
					mOperateMgr->judgeListOverflow(tempAllViewChildren, false);
				}
			}
		}
		else if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
		{
			obj->setDrawTxtParam(tempData);
			((ExcelTemplete *)obj)->requestLayout(true, true);
			//BaseContainer * excelView = obj->getChild(0);
			//QList<BaseContainer *> * excelPageList = excelView->getChildren();
			//BaseContainer * excelPage = NULL;
			//for (int j = 0; j < excelPageList->size(); j++)
			//{
			//	excelPage = excelPageList->at(j);
			//	BaseTools::getObjListFromExcel(obj, tempAllViewChildren);
			//	mOperateMgr->judgeListLess(tempAllViewChildren, true);
			//	mOperateMgr->judgeListOverflow(tempAllViewChildren, false);
			//}
		}
		else if (obj->isObj(ConstantsID::ContainerId::MENSTRUATION_TEMPLETE_OBJ))
		{
			//tempParent = tempObj->getTempParent();
			tempData->refreshTxtParamBySourceParam(obj->getSelfFocusData(), false);
			setParamByTypeAndFreshLayOut(tempData, mActionType, mDestValue, obj->getFirstChildView(false));
			obj->setDrawTxtParam(tempData);
		}
		else
		{
			obj->setDrawTxtParam(tempData);
		}
	}

	delete tempData;

	mOperateMgr->judgeListLess(mObjList, true);
	mOperateMgr->judgeListOverflow(mObjList, false);

	FlagParam * pressEnd = mOperateMgr->getPressEnd();
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, pressEnd->getFocusObj(), pressEnd->getDirection());
	mOperateMgr->clearChooseList(true);
	mOperateMgr->clearBufferExcelTemp();
}
QString SetTxtStrParamAction::getPreValue(BaseContentData * source)
{
	if (!source || source->getClassId() != ConstantsID::ClassId::STR_CONTENT_DATA)
	{
		return -1;
	}
	QString preValue = 0;
	switch (mActionType)
	{
	case BaseConstants::TxtParamActionType::SET_FAMILY:
		preValue = ((StrContentData *)source)->getFamily();
		break;
	}
	return preValue;
}
void SetTxtStrParamAction::setParamByType(BaseContentData * source, char type, QString value)
{
	switch (mActionType)
	{
	case BaseConstants::TxtParamActionType::SET_FAMILY:
		StrContentData * tempSource = ((StrContentData *)source);
		tempSource->setFamily(value);
		tempSource->setOperateType(Family);
		//source->getOwner()->getViewParent()->getLayout()->openReplace();
		break;
	}
}
void SetTxtStrParamAction::setParamByTypeAndFreshLayOut(BaseContentData * source, char type, QString value, ViewObj* view)
{
	if (view->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW))
	{
		setParamByType(source, type, value);
		QList<BaseContainer *> * childrenBase = view->getChildren();
		if (childrenBase)
		{
			BaseContainer * tempObj;
			BaseViewParam * viewParam;
			for (int i = 0; i < childrenBase->length(); i++)
			{
				tempObj = childrenBase->at(i);
				StrContentData * txtParam = (StrContentData *)tempObj->getContentData();
				txtParam->refreshTxtParamOnSetType((StrContentData*)source);
				txtParam->refreshTxtWH();
			}
		}
	}
	else
	{
		setParamByType(source, type, value);
	}

	ViewObj* row = view->findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, true);
	if (row)
	{
		row->getLayout()->openReplace();
	}	
}