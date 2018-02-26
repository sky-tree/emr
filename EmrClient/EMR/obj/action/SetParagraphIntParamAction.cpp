#include "SetParagraphIntParamAction.h"

#include "TempContainerMgr.h"

#include "StepActionManger.h"

#include "OperateMgr.h"
#include "ParagraphManager.h"

#include "BaseParagraph.h"
#include "ParagraphData.h"
#include "ViewObj.h"
#include "ExcelCellData.h"

#include "FlagParam.h"



#include "BaseContainer.h"

#include "ContentDataFactory.h"

#include "BaseTools.h"
#include "BaseSystem.h"
#include "ConstantsID.h"

SetParagraphIntParamAction::SetParagraphIntParamAction(StepActionManger * mgr) : BaseAction(mgr, ConstantsID::ClassId::SET_PARAGRAPH_INT_PARAM_ACTOIN)
{
	mChooseList = new QList<BaseContainer *>();
	mPreParagraphValueList = new QList<int>();
}

SetParagraphIntParamAction::~SetParagraphIntParamAction()
{
	mOperateMgr = NULL;
}
void SetParagraphIntParamAction::doAction(QList<BaseContainer *> * chooseList, char type, int value, ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir, bool isUpsidedown)
{
	mType = type;
	mDestValue = value;
	mStartFocus = startFocus;
	mStartDir = startDir;
	mEndFocus = endFocus;
	mEndDir = endDir;
	mIsUpsidedown = isUpsidedown;
 
	for (int i = 0; i < chooseList->size(); i ++)
	{
		mChooseList->push_back(chooseList->at(i));
	}

	ParagraphData * data = NULL;
	int preValue = 0;

	int size = chooseList->size();
	BaseParagraph * paragraph = NULL;
	BaseParagraph * preParagraph = NULL;
	ParagraphManager * mgr = NULL;
	QList<BaseParagraph *> * paragraphList = NULL;
	if (size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			BaseContainer * obj = chooseList->at(i);
			if (obj->isView())
			{
				if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
				{
					int a = 0;
					ExcelCellData *	cellData = (ExcelCellData *)obj->getContentData();
					mgr = cellData->getParagraphMgr();
					paragraphList = mgr->getParagraphList();
					for (int j = 0; j < paragraphList->size(); j++)
					{
						paragraph = paragraphList->at(j);
						data = paragraph->getParagraphData();
						//mParagraphDataList->push_back(data);
						preValue = getValue(data, mType);
						mPreParagraphValueList->push_back(preValue);
					}
				}
				else
				{
					paragraph = obj->getParagraphParent();
					if (paragraph != preParagraph)
					{
						data = (ParagraphData *)paragraph->getContentData();
						//mParagraphDataList->push_back(data);
						preValue = getValue(data, mType);
						mPreParagraphValueList->push_back(preValue);
						preParagraph = paragraph;
					}
				}
			}
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
			{
				paragraph = obj->getFirstChildView()->getParagraphParent();
				if (paragraph != preParagraph)
				{
					data = (ParagraphData *)paragraph->getContentData();
					//mParagraphDataList->push_back(data);
					preValue = getValue(data, mType);
					mPreParagraphValueList->push_back(preValue);
					preParagraph = paragraph;
				}
			}
			else
			{
				paragraph = obj->findFirstElementFromChildren()->getParagraphParent();
				if (paragraph != preParagraph)
				{
					data = (ParagraphData *)paragraph->getContentData();
					//mParagraphDataList->push_back(data);
					preValue = getValue(data, mType);
					mPreParagraphValueList->push_back(preValue);
					preParagraph = paragraph;
				}
			}
		}
	}
	else
	{
		paragraph = mEndFocus->getParagraphParent();
		data = (ParagraphData *)paragraph->getContentData();
		preValue = getValue(data, mType);
		mPreParagraphValueList->push_back(preValue);
	}
}
int SetParagraphIntParamAction::getValue(ParagraphData * source, char type)
{
	int preValue = -1;
	switch (type)
	{
	case BaseConstants::UIInputType::SET_PARAGRAPH_HORIZON_ALIGN_TYPE:
		preValue = source->getHorizonAlignType();
		break;
	}
	return preValue;
}
void SetParagraphIntParamAction::unDo()
{
	// 先将光标恢复到设置前
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS, mStartFocus, mStartDir);
	mOperateMgr->setPress(BaseConstants::MousePressType::RELEASE, mEndFocus, mEndDir);

	mOperateMgr->setUpsidedown(mIsUpsidedown);
	int size = mChooseList->size();
	ParagraphData * data = NULL;
	int preValue = -1;
	if (size < 1)
	{
		preValue = mPreParagraphValueList->at(0);
		data = mEndFocus->getParagraphParent()->getParagraphData();

		switch (mType)
		{
		case BaseConstants::UIInputType::SET_PARAGRAPH_HORIZON_ALIGN_TYPE:
			data->setHorizonAlignType(preValue, true);
			break;
		}
	}
	else
	{

		BaseContainer * obj = NULL;
		ParagraphManager * mgr = NULL;
		BaseParagraph * paragraph = NULL;
		BaseParagraph * preParagraph = NULL;
		QList<BaseContainer * > * paragraphList = BaseSystem::getInstance()->getClearTempContainerList();
		for (int i = 0; i < size; i++)
		{
			obj = mChooseList->at(i);
			if (obj->isView())
			{
				// 选中表格页
				if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
				{
					ExcelCellData *	cellData = (ExcelCellData *)obj->getContentData();
					mgr = cellData->getParagraphMgr();
					QList<BaseParagraph *> * tempParagraphList = mgr->getParagraphList();
					for (int j = 0; j < tempParagraphList->size(); j++)
					{
						paragraph = tempParagraphList->at(j);
						paragraphList->push_back(paragraph);
					}
				}
				else
				{
					paragraph = obj->getFirstView()->getParagraphParent();
					if (preParagraph != paragraph)
					{
						preParagraph = paragraph;
						paragraphList->push_back(paragraph);
					}
				}
			}
			// 选中整个表格
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
			{
				paragraph = obj->getFirstView()->getParagraphParent();
				if (preParagraph != paragraph)
				{
					preParagraph = paragraph;
					paragraphList->push_back(paragraph);
				}
			}
			// 其他模板
			else
			{
				paragraph = obj->getFirstView()->getParagraphParent();
				if (preParagraph != paragraph)
				{
					preParagraph = paragraph;
					paragraphList->push_back(paragraph);
				}
			}
		}

		for (int i = 0; i < paragraphList->size(); i++)
		{
			paragraph = (BaseParagraph *)paragraphList->at(i);
			data = paragraph->getParagraphData();
			preValue = mPreParagraphValueList->at(i);
			switch (mType)
			{
			case BaseConstants::UIInputType::SET_PARAGRAPH_HORIZON_ALIGN_TYPE:
				data->setHorizonAlignType(preValue, true);
				break;
			}
			data->replaceRowAignType();
		}
	}
	mOperateMgr->refreshToolbarParams();
}
void SetParagraphIntParamAction::reDo()
{
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS, mStartFocus, mStartDir);
	mOperateMgr->setPress(BaseConstants::MousePressType::RELEASE, mEndFocus, mEndDir);
	mOperateMgr->refreshChooseList(true, false, true);

	int size = mChooseList->size();
	ParagraphData * data = NULL;
	ParagraphData * preData = NULL;
	if (size < 1)
	{
		data = mEndFocus->getParagraphParent()->getParagraphData();

		switch (mType)
		{
		case BaseConstants::UIInputType::SET_PARAGRAPH_HORIZON_ALIGN_TYPE:
			data->setHorizonAlignType(mDestValue, true);
			break;
		}
	}
	else
	{
		BaseContainer * obj = NULL;
		ParagraphManager * mgr = NULL;
		BaseParagraph * paragraph = NULL;
		BaseParagraph * preParagraph = NULL;
		QList<BaseContainer * > * paragraphList = BaseSystem::getInstance()->getClearTempContainerList();
		for (int i = 0; i < size; i++)
		{
			obj = mChooseList->at(i);
			if (obj->isView())
			{
				// 选中表格页
				if (obj->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
				{
					ExcelCellData *	cellData = (ExcelCellData *)obj->getContentData();
					mgr = cellData->getParagraphMgr();
					QList<BaseParagraph *> * tempParagraphList = mgr->getParagraphList();
					for (int j = 0; j < tempParagraphList->size(); j++)
					{
						paragraph = tempParagraphList->at(j);
						paragraphList->push_back(paragraph);
					}
				}
				else
				{
					paragraph = obj->getFirstView()->getParagraphParent();
					if (preParagraph != paragraph)
					{
						preParagraph = paragraph;
						paragraphList->push_back(paragraph);
					}
				}
			}
			// 选中整个表格
			else if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
			{
				paragraph = obj->getFirstView()->getParagraphParent();
				if (preParagraph != paragraph)
				{
					preParagraph = paragraph;
					paragraphList->push_back(paragraph);
				}
			}
			// 其他模板
			else
			{
				paragraph = obj->getFirstView()->getParagraphParent();
				if (preParagraph != paragraph)
				{
					preParagraph = paragraph;
					paragraphList->push_back(paragraph);
				}
			}
		}

		for (int i = 0; i < paragraphList->size(); i++)
		{
			paragraph = (BaseParagraph *)paragraphList->at(i);
			data = paragraph->getParagraphData();
			switch (mType)
			{
			case BaseConstants::UIInputType::SET_PARAGRAPH_HORIZON_ALIGN_TYPE:
				data->setHorizonAlignType(mDestValue, true);
				break;
			}
			data->replaceRowAignType();
		}
	}
	mOperateMgr->refreshToolbarParams();
}