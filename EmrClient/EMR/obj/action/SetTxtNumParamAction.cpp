#include "SetTxtNumParamAction.h"

#include "TempContainerMgr.h"

#include "StepActionManger.h"

#include "OperateMgr.h"
#include "WriterManager.h"
#include "ExcelCellData.h"
#include "BaseTemplate.h"
#include "ExcelTemplete.h"

#include "StrContentData.h"

#include "FlagParam.h"



#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseLayout.h"

#include "ContentDataFactory.h"

#include "BaseTools.h"
#include "BaseSystem.h"
#include "ConstantsID.h"

SetTxtNumParamAction::SetTxtNumParamAction(StepActionManger * mgr) : BaseAction(mgr, ConstantsID::ClassId::SET_TXT_NUM_PARAM_ACTOIN)
{
	mObjList = new QList<BaseContainer *>();
	mPreValueList = new QList<QList<int> *>();
}

SetTxtNumParamAction::~SetTxtNumParamAction()
{
	mOperateMgr = NULL;
}
void SetTxtNumParamAction::doAction(QList<BaseContainer *> * list, char type, int value)
{
	mActionType = type;
	mDestValue = value;
	int size = list->size();
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;
	BaseTemplate * tempParent = NULL;

	int preValue = -1;
	QList<int> * tempValue = new QList<int>();
	QList<BaseContainer *> * tempAllViewChildren = mOperateMgr->getWriterMgr()->getClearTempParagraphViewList();
	if (size < 1)
	{
		data = mOperateMgr->getFocusTxtData();
		mFocusPreValue = getPreValue(data);
	}
	else
		mFocusPreValue = 0;
	for (int i = 0; i < size; i++)
	{
		obj = list->at(i);
		tempValue = new QList<int>();
		mObjList->push_back(obj);
		if (obj->isView())
		{
			if (obj->isTxt())
			{
				data = (StrContentData *)obj->getContentData();
				preValue = getPreValue(data);
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
void SetTxtNumParamAction::unDo()
{
	int size = mObjList->size();
	int preValue = 0;
	bool needReplaceLayout = false;
	//for (int i = 0; i < size; i++)
	//{
	//	data = mDataList->at(i);
	//	if (data)
	//	{
	//		preValue = mPreValueList->at(i);
	//		if (preValue != mDestValue)
	//			needReplaceLayout = setParamByType(data, mActionType, preValue);
	//	}
	//}
	BaseContainer * obj = NULL;
	BaseContentData * data = NULL;
	StrContentData * tempData = new StrContentData(NULL, NULL);
	bool tempReplace = false;
	BaseTemplate * tempParent = NULL;

	QList<BaseContainer *> * tempAllViewChildren = mOperateMgr->getWriterMgr()->getClearTempParagraphViewList();
	for (int i = 0; i < size; i++)
	{
		tempReplace = false;
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
					setParamByType((StrContentData *)data, mActionType, preValue, needReplaceLayout);
					if (obj->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW))
					{
						obj->getTempParent()->setDrawTxtParam(tempData);
					}
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
							setParamByType((StrContentData *)data, mActionType, preValue, tempReplace);
							if (tempObj->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW))
							{
								tempObj->getTempParent()->setDrawTxtParam((StrContentData *)data);
							}
						}
						// 公式
						else if (tempObj->isObj(ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION))
						{
							tempParent = tempObj->getTempParent();
							data = tempParent->getSelfFocusData();
							setParamByType((StrContentData *)data, mActionType, preValue, tempReplace);
							tempParent->setDrawTxtParam((StrContentData *)data);
						}
						else if (tempObj->isEnterImgOrHLine())
						{
						}
						else
						{
							setParamByType(tempData, mActionType, preValue, tempReplace);
							tempObj->setDrawTxtParam(tempData);
						}
					}
					if (tempReplace)
					{
						mOperateMgr->judgeListLess(tempAllViewChildren, true);
						mOperateMgr->judgeListOverflow(tempAllViewChildren, false);
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
				preValue = mPreValueList->at(i)->at(j);
				if (tempObj->isView())
				{
					if (tempObj->isTxt())
					{
						data = tempObj->getContentData();
						setParamByType((StrContentData *)data, mActionType, preValue, tempReplace);
						if (tempObj->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW))
						{
							tempObj->getTempParent()->setDrawTxtParam((StrContentData *)data);
						}
					}
					// 公式
					else if (tempObj->isObj(ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION))
					{
						tempParent = tempObj->getTempParent();
						data = tempParent->getSelfFocusData();
						setParamByType((StrContentData *)data, mActionType, preValue, tempReplace);
						tempParent->setDrawTxtParam((StrContentData *)data);
					}
				}
				else
				{
					setParamByType((StrContentData *)data, mActionType, preValue, tempReplace);
					tempObj->setDrawTxtParam(tempData);
				}
			}
			if (tempReplace)
			{
				((ExcelTemplete *)obj)->requestLayout(true, true);
			}
		}
		else if (obj->isObj(ConstantsID::ContainerId::MENSTRUATION_TEMPLETE_OBJ))
		{
			preValue = mPreValueList->at(i)->at(0);
			tempData->refreshTxtParamBySourceParam(obj->getSelfFocusData(), false);
			setParamByType(tempData, mActionType, preValue, tempReplace);
			obj->setDrawTxtParam(tempData);
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
					if (tempObj->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW))
					{
						setParamByType((StrContentData *)data, mActionType, preValue, needReplaceLayout);
						tempObj->getTempParent()->setDrawTxtParam((StrContentData *)data);
					}
					else
						setParamByType((StrContentData *)data, mActionType, preValue, needReplaceLayout);
				}
				else if (tempObj->isEnterImgOrHLine())
				{
				}
				else
				{
					setParamByType((StrContentData *)tempData, mActionType, preValue, needReplaceLayout);
					tempObj->setDrawTxtParam(tempData);
				}
			}
		}
	}

	delete tempData;
	if (needReplaceLayout)
	{
		mOperateMgr->judgeListLess(mObjList, true);
		mOperateMgr->judgeListOverflow(mObjList, false);
	}
	FlagParam * pressEnd = mOperateMgr->getPressEnd();
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, pressEnd->getFocusObj(), pressEnd->getDirection());
	mOperateMgr->clearChooseList(true);
	mOperateMgr->clearBufferExcelTemp();
	//if (mObjList->size() < 1)
	//{
	//	data = mOperateMgr->getFocusTxtData();
	//	setParamByType((StrContentData *)data, mActionType, mFocusPreValue, tempReplace);
	//	mOperateMgr->refreshToolbarParams();
	//}
}
void SetTxtNumParamAction::reDo()
{
	int size = mObjList->size();
	int curValue = 0;
	bool needReplaceLayout = false;
	BaseContainer * obj = NULL;
	BaseContentData * data = NULL;
	BaseTemplate * tempParent = NULL;
	StrContentData * tempData = new StrContentData(NULL, NULL);
	setParamByType(tempData, mActionType, mDestValue, needReplaceLayout);
	bool isTxt = false;
	bool tempReplace = false;
	QList<BaseContainer *> * tempAllViewChildren = mOperateMgr->getWriterMgr()->getClearTempParagraphViewList();
	for (int i = 0; i < size; i++)
	{
		tempAllViewChildren->clear();
		tempReplace = false;
		obj = mObjList->at(i);
		if (obj->isView())
		{
			if (obj->isTxt())
			{
				data = obj->getContentData();
				if (data)
				{
					setParamByType((StrContentData *)data, mActionType, mDestValue, needReplaceLayout);
					if (obj->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW))
					{
						obj->getTempParent()->setDrawTxtParam(tempData);
					}
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
							setParamByType((StrContentData *)data, mActionType, mDestValue, tempReplace);
							if (tempObj->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW))
							{
								tempObj->getTempParent()->setDrawTxtParam((StrContentData *)data);
							}
						}
						// 公式
						else if (tempObj->isObj(ConstantsID::ContainerId::TEXT_WIDGET_OF_MENSTRUATION))
						{
							tempParent = tempObj->getTempParent();
							data = tempParent->getSelfFocusData();
							setParamByType((StrContentData *)data, mActionType, mDestValue, tempReplace);
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
					if (tempReplace)
					{
						mOperateMgr->judgeListLess(tempAllViewChildren, true);
						mOperateMgr->judgeListOverflow(tempAllViewChildren, false);
					}
				}
			}
		}
		else if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
		{

			obj->setDrawTxtParam(tempData);
			if (needReplaceLayout)
			{
				((ExcelTemplete *)obj)->requestLayout(true, true);
				//BaseContainer * excelView = obj->getChild(0);
				//QList<BaseContainer *> * excelPageList = excelView->getChildren();
				//BaseContainer * excelPage = NULL;
				//for (int j = 0; j < excelPageList->size(); j++)
				//{
				//	tempAllViewChildren->clear();
				//	excelPage = excelPageList->at(j);
				//	ExcelCellData * excelData = (ExcelCellData *)excelPage->getContentData();
				//	BaseTools::getObjListFromExcel(obj, tempAllViewChildren);
				//	mOperateMgr->judgeListLess(tempAllViewChildren, true);
				//	mOperateMgr->judgeListOverflow(tempAllViewChildren, false);
				//}
			}
		}
		else if (obj->isObj(ConstantsID::ContainerId::MENSTRUATION_TEMPLETE_OBJ))
		{
			tempData->refreshTxtParamBySourceParam(obj->getSelfFocusData(), false);
			setParamByType(tempData, mActionType, mDestValue, tempReplace);
			obj->setDrawTxtParam(tempData);
		}
		else
		{
			if (obj->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW))
			{
				setParamByType(tempData, mActionType, mDestValue, needReplaceLayout);
				obj->getTempParent()->setDrawTxtParam(tempData);
			}
			else
			{
				obj->setDrawTxtParam(tempData);
				BaseTools::openAllRowReplace(obj);
			}
		}
	}

	delete tempData;
	if (needReplaceLayout)
	{
		mOperateMgr->judgeListLess(mObjList, true);
		mOperateMgr->judgeListOverflow(mObjList, false);
	}
	FlagParam * pressEnd = mOperateMgr->getPressEnd();
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, pressEnd->getFocusObj(), pressEnd->getDirection());
	mOperateMgr->clearChooseList(true);
	mOperateMgr->clearBufferExcelTemp();
	//if (mObjList->size() < 1)
	//{
	//	data = mOperateMgr->getFocusTxtData();
	//	setParamByType((StrContentData *)data, mActionType, mDestValue, tempReplace);
	//	mOperateMgr->refreshToolbarParams();
	//}
}
int SetTxtNumParamAction::getPreValue(StrContentData * source)
{
	if (!source)
	{
		return -1;
	}
	int preValue = 0;
	switch (mActionType)
	{
	case BaseConstants::TxtParamActionType::SET_BOLD:
		if (source->getBold())
		{
			preValue = 1;
		}
		else
			preValue = 0;
		break;
	case BaseConstants::TxtParamActionType::SET_ITALIC:
		if (source->getItalic())
		{
			preValue = 1;
		}
		else
			preValue = 0;
		break;
	case BaseConstants::TxtParamActionType::SET_FONT_SIZE:
		preValue = source->getPointSize();
		break;
	case BaseConstants::TxtParamActionType::SET_FONT_COLOR:
		preValue = source->getFontColor();
		break;
	case BaseConstants::TxtParamActionType::SET_BACKGROUND__COLOR:
		preValue = source->getBackgroundColor();
		break;
	case BaseConstants::TxtParamActionType::SET_FLOW_TYPE:
		preValue = source->getFlowType();
		break;
	}
	return preValue;
}
void SetTxtNumParamAction::setParamByType(StrContentData * source, char type, int value, bool & needReplace)
{
	switch (mActionType)
	{
	case BaseConstants::TxtParamActionType::SET_BOLD:
		if (value == 0)
			source->setBold(false);
		else
			source->setBold(true);
		source->setOperateType(Bold);
		needReplace = true;
		break;
	case BaseConstants::TxtParamActionType::SET_ITALIC:
		if (value == 0)
			source->setItalic(false);
		else
			source->setItalic(true);
		source->setOperateType(FontItalic);
		break;
	case BaseConstants::TxtParamActionType::SET_FONT_SIZE:
		source->setPointSize(value);
		source->setOperateType(FontSize);
		needReplace = true;
		break;
	case BaseConstants::TxtParamActionType::SET_FONT_COLOR:
		source->setFontColor(value);
		source->setOperateType(FontColor);
		break;
	case BaseConstants::TxtParamActionType::SET_BACKGROUND__COLOR:
		source->setBackgroundColor(value);
		source->setOperateType(BackgroundColor);
		break;
	case BaseConstants::TxtParamActionType::SET_FLOW_TYPE:
		source->setFlowType(value);
		needReplace = true;
		source->setOperateType(FlowType);
		break;
	}
	if (needReplace)
	{
		BaseContainer * owner = source->getOwner();
		if (owner)
		{
			if (!owner->isTemp())
				owner->getViewParent()->getLayout()->openReplace();
		}
	}
}