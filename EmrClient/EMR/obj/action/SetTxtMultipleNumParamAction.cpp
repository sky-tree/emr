#include "SetTxtMultipleNumParamAction.h"

#include "TempContainerMgr.h"

#include "StepActionManger.h"

#include "OperateMgr.h"
#include "WriterManager.h"

#include "StrContentData.h"

#include "FlagParam.h"



#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseTemplate.h"
#include "ExcelCellData.h"

#include "ContentDataFactory.h"

#include "BaseTools.h"
#include "BaseSystem.h"
#include "ConstantsID.h"

SetTxtMultipleNumParamAction::SetTxtMultipleNumParamAction(StepActionManger * mgr) : BaseAction(mgr, ConstantsID::ClassId::SET_TXT_MULTIPLE_NUM_PARAM_ACTOIN)
{
	mObjList = new QList<BaseContainer *>();
	mPreValueList = new QList<QList<int *> *>();
}

SetTxtMultipleNumParamAction::~SetTxtMultipleNumParamAction()
{
	mOperateMgr = NULL;
}
void SetTxtMultipleNumParamAction::doAction(QList<BaseContainer *> * list, char type, int valueNum, int * value)
{
	mActionType = type;
	mDestValue = value;

	mValueNum = valueNum;
	int size = list->size();
	BaseContainer * obj = NULL;
	StrContentData * data = NULL;
	QList<int *> * tempValue = NULL;
	QList<BaseContainer *> * tempAllViewChildren = mOperateMgr->getWriterMgr()->getClearTempParagraphViewList();
	for (int i = 0; i < size; i++)
	{
		obj = list->at(i);
		int * preValue = NULL;
		tempValue = new QList<int *>();
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
void SetTxtMultipleNumParamAction::unDo()
{
	int size = mObjList->size();
	int * preValue = 0;
	BaseContainer * obj = NULL;
	BaseContentData * data = NULL;
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
					setParamByType((StrContentData *)data, preValue);
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
							setParamByType((StrContentData *)data, preValue);
						}
						else if (tempObj->isEnterImgOrHLine())
						{
						}
						else
						{
							switch (mActionType)
							{
							case BaseConstants::TxtParamActionType::SET_UNDERLINE:
								tempData->setOperateType(UnderLineType);
								tempData->setUnderLine(preValue[0], preValue[1]);
								break;
							case BaseConstants::TxtParamActionType::SET_DEL_LINE:
								tempData->setOperateType(DelLineType);
								tempData->setDelLine(preValue[0], preValue[1]);
								break;
							}
							tempObj->setDrawTxtParam(tempData);
						}
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
						setParamByType((StrContentData *)data, preValue);
					}
				}
				else
				{
					switch (mActionType)
					{
					case BaseConstants::TxtParamActionType::SET_UNDERLINE:
						tempData->setOperateType(UnderLineType);
						tempData->setUnderLine(preValue[0], preValue[1]);
						break;
					case BaseConstants::TxtParamActionType::SET_DEL_LINE:
						tempData->setOperateType(DelLineType);
						tempData->setDelLine(preValue[0], preValue[1]);
						break;
					}
					tempObj->setDrawTxtParam(tempData);
				}
			}
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
					setParamByType((StrContentData *)data, preValue);
				}
				else if (tempObj->isEnterImgOrHLine())
				{
				}
				else
				{
					switch (mActionType)
					{
					case BaseConstants::TxtParamActionType::SET_UNDERLINE:
						tempData->setOperateType(UnderLineType);
						tempData->setUnderLine(preValue[0], preValue[1]);
						break;
					case BaseConstants::TxtParamActionType::SET_DEL_LINE:
						tempData->setOperateType(DelLineType);
						tempData->setDelLine(preValue[0], preValue[1]);
						break;
					}
					tempObj->setDrawTxtParam(tempData);
				}
			}
		}
	}

	delete tempData;

	FlagParam * pressEnd = mOperateMgr->getPressEnd();
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, pressEnd->getFocusObj(), pressEnd->getDirection());
	mOperateMgr->clearChooseList(true);
	mOperateMgr->clearBufferExcelTemp();
}
void SetTxtMultipleNumParamAction::reDo()
{
	int size = mObjList->size();
	int preValue = 0;
	BaseContainer * obj = NULL;
	BaseContentData * data = NULL;
	StrContentData * tempData = new StrContentData(NULL, NULL);
	switch (mActionType)
	{
	case BaseConstants::TxtParamActionType::SET_UNDERLINE:
		tempData->setOperateType(UnderLineType);
		tempData->setUnderLine(mDestValue[0], mDestValue[1]);
		break;
	case BaseConstants::TxtParamActionType::SET_DEL_LINE:
		tempData->setOperateType(DelLineType);
		tempData->setDelLine(mDestValue[0], mDestValue[1]);
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
					setParamByType((StrContentData *)data, mDestValue);
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
							setParamByType((StrContentData *)data, mDestValue);
						}
						else if (tempObj->isEnterImgOrHLine())
						{
						}
						else
						{
							tempObj->setDrawTxtParam(tempData);
						}
					}
				}
			}
		}
		else
		{
			obj->setDrawTxtParam(tempData);
		}
	}

	delete tempData;
	FlagParam * pressEnd = mOperateMgr->getPressEnd();
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, pressEnd->getFocusObj(), pressEnd->getDirection());
	mOperateMgr->clearChooseList(true);
	mOperateMgr->clearBufferExcelTemp();
}
int * SetTxtMultipleNumParamAction::getPreValue(StrContentData * source)
{
	if (!source)
	{
		return NULL;
	}
	int * preValue = new int[mValueNum];
	switch (mActionType)
	{
	case BaseConstants::TxtParamActionType::SET_UNDERLINE:
		preValue[0] = source->getUnderLineType();
		preValue[1] = source->getUnderLineThick();
		break;
	case BaseConstants::TxtParamActionType::SET_DEL_LINE:
		preValue[0] = source->getDelLineType();
		preValue[1] = source->getDelLineThick();
		break;
	}
	return preValue;
}
void SetTxtMultipleNumParamAction::setParamByType(StrContentData * source, int * value)
{
	switch (mActionType)
	{
	case BaseConstants::TxtParamActionType::SET_UNDERLINE:
		source->setUnderLine(value[0], value[1]);
		break;
	case BaseConstants::TxtParamActionType::SET_DEL_LINE:
		source->setDelLine(value[0], value[1]);
		break;
	}
}