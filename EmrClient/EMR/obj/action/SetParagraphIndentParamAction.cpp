#include "SetParagraphIndentParamAction.h"

#include "TempContainerMgr.h"

#include "StepActionManger.h"

#include "OperateMgr.h"
#include "ParagraphManager.h"


#include "BaseParagraph.h"
#include "ParagraphData.h"
#include "ExcelCellData.h"

#include "FlagParam.h"




#include "BaseContainer.h"
#include "ViewObj.h"

#include "RowLayout.h"

#include "ContentDataFactory.h"

#include "BaseTools.h"
#include "BaseSystem.h"
#include "ConstantsID.h"

SetParagraphIndentParamAction::SetParagraphIndentParamAction(StepActionManger * mgr) : BaseAction(mgr
	, ConstantsID::ClassId::SET_PARAGRAPH_INDENT_PARAM_ACTOIN)
{
	mChooseList = new QList<BaseContainer *>();
	mParagraphDataList = new QList<ParagraphData *>();
	mPreParagraphDataListCopy = new QList<ParagraphData *>();
}

SetParagraphIndentParamAction::~SetParagraphIndentParamAction()
{
	mOperateMgr = NULL;
}
void SetParagraphIndentParamAction::doAction(QList<BaseContainer *> * chooseList, int inL, int inR, char indentType, int indentValue
	, ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir, bool isUpsidedown)
{
	mStartFocus = startFocus;
	mStartDir = startDir;
	mEndFocus = endFocus;
	mEndDir = endDir;
	mIsUpsidedown = isUpsidedown;

	mDestInsideLeft = inL;
	mDestInsideRight = inR;
	mDestIndentType = indentType;
	mDestIndentValue = indentValue;
	int size = chooseList->size();
	for (int i = 0; i < size; i ++)
	{
		mChooseList->push_back(chooseList->at(i));
	}

	//BaseParagraph * startParagraph = startFocus->getParagraphParent();
	//BaseParagraph * endParagraph = endFocus->getParagraphParent();
	//if (mIsUpsidedown)
	//{
	//	startParagraph = endFocus->getParagraphParent();
	//	endParagraph = startFocus->getParagraphParent();
	//}
	//ParagraphData * data = (ParagraphData *)startParagraph->getContentData();
	//mParagraphDataList->push_back(data);
	//mPreParagraphDataListCopy->push_back(ContentDataFactory::getParagraphDataCopy(data));
	//while (startParagraph != endParagraph)
	//{
	//	startParagraph = startParagraph->getNextParagraph();
	//	if (!startParagraph)
	//	{
	//		break;
	//	}
	//	data = (ParagraphData *)startParagraph->getContentData();
	//	mParagraphDataList->push_back(data);
	//	mPreParagraphDataListCopy->push_back(ContentDataFactory::getParagraphDataCopy(data));
	//}

	ParagraphData * data = NULL;
	int preValue = 0;

	BaseParagraph * paragraph = NULL;
	BaseParagraph * preParagraph = NULL;
	ParagraphManager * mgr = NULL;
	QList<BaseParagraph *> * paragraphList = NULL;
	if (size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			BaseContainer * obj = mChooseList->at(i);
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
						mPreParagraphDataListCopy->push_back(ContentDataFactory::getParagraphDataCopy(data));
					}
				}
				else
				{
					paragraph = obj->getParagraphParent();
					if (paragraph != preParagraph)
					{
						data = (ParagraphData *)paragraph->getContentData();
						mPreParagraphDataListCopy->push_back(ContentDataFactory::getParagraphDataCopy(data));
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
					mPreParagraphDataListCopy->push_back(ContentDataFactory::getParagraphDataCopy(data));
					preParagraph = paragraph;
				}
			}
			else
			{
				paragraph = obj->findFirstElementFromChildren()->getParagraphParent();
				if (paragraph != preParagraph)
				{
					data = (ParagraphData *)paragraph->getContentData();
					mPreParagraphDataListCopy->push_back(ContentDataFactory::getParagraphDataCopy(data));
					preParagraph = paragraph;
				}
			}
		}
	}
	else
	{
		paragraph = mEndFocus->getParagraphParent();
		data = (ParagraphData *)paragraph->getContentData();
		mPreParagraphDataListCopy->push_back(ContentDataFactory::getParagraphDataCopy(data));
	}
}
void SetParagraphIndentParamAction::unDo()
{
	// 先将光标恢复到设置前
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS, mStartFocus, mStartDir);
	mOperateMgr->setPress(BaseConstants::MousePressType::RELEASE, mEndFocus, mEndDir);
	mOperateMgr->setUpsidedown(mIsUpsidedown);

	//int size = mParagraphDataList->size();
	//ParagraphData * data = NULL;
	//ParagraphData * preData = NULL;

	BaseParagraph * paragraph = NULL;
	BaseContainer * firstObj = NULL;
	ViewObj * firstRow = NULL;
	RowLayout * firstRowLayout = NULL;


	BaseContainer * lastObj = NULL;

	int size = mChooseList->size();
	ParagraphData * data = NULL;
	ParagraphData * preData = NULL;
	if (size < 1)
	{
		preData = mPreParagraphDataListCopy->at(0);
		paragraph = mEndFocus->getParagraphParent();
		data = paragraph->getParagraphData();
		data->setIndentParam(preData->getInsideLeft(), preData->getInsideRight()
				, preData->getIndentType(), preData->getIndentValue(), false);

		firstObj = paragraph->getChild(0);
		firstRow = firstObj->getViewParent();
		firstRowLayout = (RowLayout *)firstRow->getLayout();

		lastObj = paragraph->getLastChild(false);

		BaseContainer * lastRow = lastObj->getViewParent();
		if (lastRow != firstRow)
		{
			BaseLayout * nextLayout = firstRowLayout;
			BaseContainer * tempRow = nextLayout->getOwner();
			BaseLayout * lastLayout = (RowLayout *)lastRow->getChildrenMgr();
			while (nextLayout)
			{
				nextLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
				if (nextLayout == lastLayout)
					break;
				nextLayout = nextLayout->getNextLayout(false);
			}
		}
		else
			firstRowLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
		firstRowLayout->openReplace();
		firstRowLayout->judgeLessThanCircleUntilObj(lastObj);
		firstRowLayout->judgeOverFlowCircleUntilObj(lastObj);
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

			preData = mPreParagraphDataListCopy->at(i);
			data->setIndentParam(preData->getInsideLeft(), preData->getInsideRight()
				, preData->getIndentType(), preData->getIndentValue(), false);

			paragraph = (BaseParagraph *)data->getOwner();

			firstObj = paragraph->getChild(0);
			firstRow = (ViewObj *)firstObj->getViewParent();
			firstRowLayout = (RowLayout *)firstRow->getLayout();

			lastObj = paragraph->getLastChild(false);

			BaseContainer * lastRow = lastObj->getViewParent();
			if (lastRow != firstRow)
			{
				BaseLayout * nextLayout = firstRowLayout;
				BaseContainer * tempRow = nextLayout->getOwner();
				BaseLayout * lastLayout = (RowLayout *)lastRow->getChildrenMgr();
				while (nextLayout)
				{
					nextLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
					if (nextLayout == lastLayout)
						break;
					nextLayout = nextLayout->getNextLayout(false);
				}
			}
			else
				firstRowLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
			firstRowLayout->openReplace();
			firstRowLayout->judgeLessThanCircleUntilObj(lastObj);
			firstRowLayout->judgeOverFlowCircleUntilObj(lastObj);
		}
	}
}
void SetParagraphIndentParamAction::reDo()
{
	mOperateMgr->setPress(BaseConstants::MousePressType::PRESS, mStartFocus, mStartDir);
	mOperateMgr->setPress(BaseConstants::MousePressType::RELEASE, mEndFocus, mEndDir);
	mOperateMgr->refreshChooseList(true, false, true);

	int size = mChooseList->size();

	ParagraphData * data = NULL;
	BaseParagraph * paragraph = NULL;
	BaseContainer * firstObj = NULL;
	ViewObj * firstRow = NULL;
	RowLayout * firstRowLayout = NULL;

	BaseContainer * lastObj = NULL;

	//for (int i = 0; i < size; i++)
	//{
	//	data = mParagraphDataList->at(i);
	//	data->setIndentParam(mDestInsideLeft, mDestInsideRight, mDestIndentType, mDestIndentValue, false);

	//	paragraph = (BaseParagraph *)data->getOwner();
	//	firstObj = paragraph->getChild(0);
	//	firstRow = (ViewObj *)firstObj->getViewParent();
	//	firstRowLayout = (RowLayout *)firstRow->getLayout();

	//	lastObj = paragraph->getLastChild(false);
	//	firstRowLayout->openReplace();
	//	firstRowLayout->judgeLessThanCircleUntilObj(lastObj);
	//	firstRowLayout->judgeOverFlowCircleUntilObj(lastObj);
	//}

	if (size < 1)
	{
		paragraph = mEndFocus->getParagraphParent();
		data = paragraph->getParagraphData();
		data->setIndentParam(mDestInsideLeft, mDestInsideRight, mDestIndentType, mDestIndentValue, false);

		firstObj = paragraph->getChild(0);
		firstRow = (ViewObj *)firstObj->getViewParent();
		firstRowLayout = (RowLayout *)firstRow->getLayout();

		lastObj = paragraph->getLastChild(false);

		BaseContainer * lastRow = lastObj->getViewParent();
		if (lastRow != firstRow)
		{
			BaseLayout * nextLayout = firstRowLayout;
			BaseContainer * tempRow = nextLayout->getOwner();
			BaseLayout * lastLayout = (RowLayout *)lastRow->getChildrenMgr();
			while (nextLayout)
			{
				nextLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
				if (nextLayout == lastLayout)
					break;
				nextLayout = nextLayout->getNextLayout(false);
			}
		}
		else
			firstRowLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
		firstRowLayout->openReplace();
		firstRowLayout->judgeLessThanCircleUntilObj(lastObj);
		firstRowLayout->judgeOverFlowCircleUntilObj(lastObj);
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

			data->setIndentParam(mDestInsideLeft, mDestInsideRight, mDestIndentType, mDestIndentValue, false);

			paragraph = (BaseParagraph *)data->getOwner();

			firstObj = paragraph->getChild(0);
			firstRow = (ViewObj *)firstObj->getViewParent();
			firstRowLayout = (RowLayout *)firstRow->getLayout();

			lastObj = paragraph->getLastChild(false);

			BaseContainer * lastRow = lastObj->getViewParent();
			if (lastRow != firstRow)
			{
				BaseLayout * nextLayout = firstRowLayout;
				BaseContainer * tempRow = nextLayout->getOwner();
				BaseLayout * lastLayout = (RowLayout *)lastRow->getChildrenMgr();
				while (nextLayout)
				{
					nextLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
					if (nextLayout == lastLayout)
						break;
					nextLayout = nextLayout->getNextLayout(false);
				}
			}
			else
				firstRowLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);

			firstRowLayout->openReplace();
			firstRowLayout->judgeLessThanCircleUntilObj(lastObj);
			firstRowLayout->judgeOverFlowCircleUntilObj(lastObj);
		}
	}
}