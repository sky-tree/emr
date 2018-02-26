#include "ParagraphData.h"




#include "WriterManager.h"

#include "ViewObj.h"
#include "BaseLayout.h"
#include "RowLayout.h"


#include "LayoutReplaceManager.h"

#include "ViewFactory.h"

#include "BaseIOTools.h"
#include "ConstantsID.h"
#include "UIConstants.h"
#include "BaseConstants.h"

ParagraphData::ParagraphData(BaseContainer * owner) : BaseContentData(owner, ConstantsID::ClassId::PARAGRAPH_DATA, NULL)
{
	init();
}
ParagraphData::ParagraphData(BaseContainer * owner, ParagraphData * source) : BaseContentData(owner, ConstantsID::ClassId::PARAGRAPH_DATA, NULL)
{
	init(source);
}

ParagraphData::~ParagraphData()
{
}
void ParagraphData::init()
{
	setNeedLoadSelfChildren(false);

	mRowDisHTimes = 0;
	mParagraphDisTop = 0;
	mParagraphDisBottom = 0;
	setRowDisHTimes(1, false);
	mInsideLeft = 0;
	mInsideRight = 0;
	mIndentValue = 0;
	mIndentType = BaseConstants::NONE;

	mHorizonAlignType = BaseConstants::Direction::LEFT;
}
void ParagraphData::saveSelfData(BaseIOTools *iOTools)
{
	iOTools->writeInt(mParagraphDisTop);
	iOTools->writeInt(mParagraphDisBottom);
	iOTools->writeFloat(mRowDisHTimes);
	iOTools->writeInt(mInsideLeft);
	iOTools->writeInt(mInsideRight);
	iOTools->writeByte(mIndentType);
	iOTools->writeInt(mIndentValue);
	iOTools->writeByte(mHorizonAlignType);
}
void ParagraphData::loadSelfData(BaseIOTools *iOTools)
{
	mParagraphDisTop = iOTools->readInt();
	mParagraphDisBottom = iOTools->readInt();
	float rowDisHTimes = iOTools->readFloat();
	setRowDisHTimes(rowDisHTimes, false);
	mInsideLeft = iOTools->readInt();
	mInsideRight = iOTools->readInt();
	mIndentType = iOTools->readByte();
	mIndentValue = iOTools->readInt();

	mHorizonAlignType = iOTools->readByte();
}

void ParagraphData::init(ParagraphData * source)
{
	setNeedLoadSelfChildren(false);
	mRowDisHTimes = 0;
	if (!source)
	{
		init();
		return;
	}
	mParagraphDisTop = source->mParagraphDisTop;
	mParagraphDisBottom = source->mParagraphDisBottom;
	mInsideLeft = source->mInsideLeft;
	mInsideRight = source->mInsideRight;
	mIndentValue = source->mIndentValue;
	mIndentType = source->mIndentType;

	setRowDisHTimes(source->mRowDisHTimes, false);

	mHorizonAlignType = source->mHorizonAlignType;
}
bool ParagraphData::isSameParagraphParamsExceptAlignType(ParagraphData * source)
{
	if (mIndentType != source->mIndentType || mIndentValue != source->mIndentValue || mInsideLeft != source->mInsideLeft
		|| mInsideRight != source->mInsideRight || mRowDisHTimes != source->mRowDisHTimes
		|| mParagraphDisTop != source->mParagraphDisTop || mParagraphDisBottom != source->mParagraphDisBottom)
	{
		return false;
	}
	return true;
}
void ParagraphData::setParagraphDataWithOutAlignTypeBySource(ParagraphData * source)
{
	mParagraphDisTop = source->mParagraphDisTop;
	mParagraphDisBottom = source->mParagraphDisBottom;
	mRowDisHTimes = source->mRowDisHTimes;
	mInsideLeft = source->mInsideLeft;
	mInsideRight = source->mInsideRight;
	mIndentValue = source->mIndentValue;
	mIndentType = source->mIndentType;
}
float ParagraphData::getParagraphDisTop()
{
	return mParagraphDisTop;
	//return mRowDisHValue / 2;
}
float ParagraphData::getParagraphDisBottom()
{
	return mParagraphDisBottom;
	//return mRowDisHValue / 2;
}
float ParagraphData::getRowDisHTimes()
{
	return mRowDisHTimes;
}
float ParagraphData::getRowDisHValue()
{
	return mRowDisHValue;
}
int ParagraphData::getInsideLeft()
{
	return mInsideLeft;
}
int ParagraphData::getInsideRight()
{
	return mInsideRight;
}
int ParagraphData::getIndentValue()
{
	return mIndentValue;
}
char ParagraphData::getIndentType()
{
	return mIndentType;
}
//bool ParagraphData::setParagraphDisTop(int value)
//{
//	if (mParagraphDisTop == value)
//	{
//		return false;
//	}
//	mParagraphDisTop = value;
//	return true;
//}
//bool ParagraphData::setParagraphDisBottom(int value)
//{
//	if (mParagraphDisBottom == value)
//	{
//		return false;
//	}
//	mParagraphDisBottom = value;
//	return true;
//}
bool ParagraphData::setRowDisHTimes(float times, bool needReplaceNow)
{
	if (mRowDisHTimes == times)
		return false;
	mRowDisHTimes = times;
	mRowDisHValue = UIConstants::DEFAULT_ROW_DIS_H * mRowDisHTimes;
	mParagraphDisTop = mRowDisHValue / 2;
	mParagraphDisBottom = mParagraphDisTop;
	if (!needReplaceNow)
	{
		return true;
	}
	addTxtLayerToReplaceList();
	return true;
}
bool ParagraphData::setInsideLeft(int value, bool needReplaceRow)
{
	if (mInsideLeft == value)
		return false;
	//float pre = mIndentLeft;
	mInsideLeft = value;
	if (!needReplaceRow)
	{
		return true;
	}
	judgeLessOrMoreRowList();
	return true;
	//BaseContainer * firstChild = mOwner->getChild(0);
	//ViewObj * firstRow = firstChild->getViewParent();
	//RowLayout * rowLayout = (RowLayout *)firstRow->getLayout();
	////// 如果左边距增加，有效宽度减小
	////if (pre < value)
	////{
	////	rowLayout->judgeLessThanCircle();
	////}
	////// 如果左边距减小，有效宽度增加
	////else
	////{
	////	rowLayout->judgeOverFlowCircle();
	////}
	//rowLayout->judgeLessThanCircle(true);
	//rowLayout->judgeOverFlowCircle();
}
bool ParagraphData::setInsideRight(int value, bool needReplaceRow)
{
	if (mInsideRight == value)
		return false;
	//float pre = mIndentRight;
	mInsideRight = value;
	if (!needReplaceRow)
	{
		return true;
	}
	judgeLessOrMoreRowList();
	return true;
	//BaseContainer * firstChild = mOwner->getChild(0);
	//ViewObj * firstRow = firstChild->getViewParent();
	//RowLayout * rowLayout = (RowLayout *)firstRow->getLayout();
	//rowLayout->judgeLessThanCircle(true);
	//rowLayout->judgeOverFlowCircle();
}
bool ParagraphData::setIndentParam(int inL, int inR, char type, int value, bool needReplaceRow)
{
	if (mInsideLeft == inL && mInsideRight == inR && mIndentType == type && mIndentValue == value)
	{
		return false;
	}
	mInsideLeft = inL;
	mInsideRight = inR;
	mIndentType = type;
	mIndentValue = value;

	if (!needReplaceRow)
	{
		return true;
	}

	judgeLessOrMoreRowList();
	return true;
	//BaseContainer * firstChild = mOwner->getChild(0);
	//ViewObj * firstRow = firstChild->getViewParent();
	//RowLayout * rowLayout = (RowLayout *)firstRow->getLayout();
	//rowLayout->judgeLessThanCircle(true);
	//rowLayout->judgeOverFlowCircle();

}
char ParagraphData::getHorizonAlignType()
{
	return mHorizonAlignType;
}
bool ParagraphData::setHorizonAlignType(char type, bool needReplaceRow)
{
	if (mHorizonAlignType == type)
		return false;
	mHorizonAlignType = type;
	if (!needReplaceRow)
	{
		return true;
	}
	replaceRowAignType();
	return true;
}
//void ParagraphData::resetParam(ParagraphData * source)
//{
//	// 行排版，但不检测缩进
//	bool needRefreshAlignType = setHorizonAlignType(source->mHorizonAlignType, false);
//	// 文字层添加进排版列表
//	bool needReplaceTxtLayer = setParagraphDisTop(source->mParagraphDisTop);
//	bool temp = false;
//	temp = setParagraphDisTop(source->mParagraphDisTop);
//	if (!needReplaceTxtLayer)
//		needReplaceTxtLayer = temp;
//
//	temp = setParagraphDisBottom(source->mParagraphDisBottom);
//	if (!needReplaceTxtLayer)
//		needReplaceTxtLayer = temp;
//
//	temp = setRowDisHTimes(source->mRowDisHTimes);
//	if (!needReplaceTxtLayer)
//		needReplaceTxtLayer = temp;
//	// 行缩进判断
//	bool needJudgeLessOrMore = setInsideLeft(source->mInsideLeft, false);
//	temp = setInsideRight(source->mInsideLeft, false);
//	if (!needJudgeLessOrMore)
//		needJudgeLessOrMore = temp;
//	temp = setIndentParam(source->mIndentType, source->mIndentValue, false);
//	if (!needJudgeLessOrMore)
//		needJudgeLessOrMore = temp;
//
//
//	// 先判断缩进
//	if (needJudgeLessOrMore)
//	{
//		judgeLessOrMoreRowList();
//	}
//	else
//	{
//		// 判断左中右对齐变化
//		if (needRefreshAlignType)
//		{
//			replaceRowAignType();
//		}
//		// 段落或行间距变化
//		if (needReplaceTxtLayer)
//		{
//			addTxtLayerToReplaceList();
//		}
//	}
//}
void ParagraphData::judgeLessOrMoreRowList()
{
	BaseContainer * firstChild = mOwner->getChild(0);
	ViewObj * firstRow = firstChild->getViewParent();
	RowLayout * rowLayout = (RowLayout *)firstRow->getLayout();

	BaseContainer * lastObj = mOwner->getLastChild(false);

	rowLayout->judgeLessThanCircleUntilObj(lastObj);
	rowLayout->judgeOverFlowCircleUntilObj(lastObj);
	//rowLayout->judgeLessThanCircle(true);
	//rowLayout->judgeOverFlowCircle();
}
void ParagraphData::replaceRowAignType()
{
	BaseContainer * firstChild = mOwner->getChild(0);
	BaseContainer * lastChild = mOwner->getLastChild(false);

	ViewObj * firstRow = firstChild->getViewParent();
	if (!firstRow)
	{
		firstChild = firstChild->getNextView();
		firstRow = firstChild->getViewParent();
	}
	ViewObj * firstLayer = firstRow->getViewParent();
	int firstRowIdx = firstRow->getViewIndex();

	//ViewObj * firstPage = firstRow->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE);
	ViewObj * lastRow = lastChild->getViewParent();
	ViewObj * lastLayer = lastRow->getViewParent();
	int lastRowIdx = lastRow->getViewIndex();
	QList<BaseContainer *> * rowList = firstLayer->getChildren();
	int size = rowList->size();

	ViewObj * row = NULL;
	BaseViewParam * rowParam = NULL;
	BaseLayout * rowLayout = NULL;
	int end = 0;
	// 段落内容在同页
	if (firstLayer == lastLayer)
	{
		end = lastRowIdx + 1;
		for (int i = firstRowIdx; i < end; i++)
		{
			row = (ViewObj *)rowList->at(i);
			rowLayout = row->getLayout();
			rowLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
		}
	}
	// 跨页
	else
	{
		ViewObj * firstPage = firstLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
		int firstPageIdx = firstPage->getViewIndex();
		ViewObj * lastPage = lastLayer->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);
		int lastPageIdx = lastPage->getViewIndex();
		ViewObj * pagePanel = firstPage->getViewParent();

		// 首页行刷新
		end = rowList->size();
		for (int i = firstRowIdx; i < end; i++)
		{
			row = (ViewObj *)rowList->at(i);
			rowLayout = row->getLayout();
			rowLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
		}
		// 中间页刷新
		BaseContainer * page = NULL;
		BaseContainer * layer = NULL;
		QList<BaseContainer *> * pageList = pagePanel->getChildren();
		for (int i = firstPageIdx; i < lastPageIdx; i++)
		{
			page = pageList->at(i);
			layer = page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			rowList = layer->getChildren();
			size = rowList->size();
			for (int j = 0; j < size; j++)
			{
				row = (ViewObj *)rowList->at(j);
				rowLayout = row->getLayout();
				rowLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
			}
		}
		// 尾页刷新
		rowList = lastLayer->getChildren();
		end = lastRowIdx + 1;
		for (int i = 0; i < end; i++)
		{
			row = (ViewObj *)rowList->at(i);
			rowLayout = row->getLayout();
			rowLayout->replaceByType(BaseConstants::ReplaceType::REPLACE_NOW);
		}
	}
}
void ParagraphData::addTxtLayerToReplaceList()
{
	BaseContainer * firstChild = mOwner->getChild(0);
	BaseContainer * lastChild = mOwner->getLastChild(false);
	ViewObj * firstRow = firstChild->getViewParent();
	ViewObj * firstLayer = firstRow->getViewParent();
	BaseLayout * firstLayout = firstLayer->getLayout();
	ViewObj * firstPage = firstRow->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);

	ViewObj * lastRow = lastChild->getViewParent();
	ViewObj * lastLayer = lastRow->getViewParent();
	BaseLayout * lastLayout = lastLayer->getLayout();
	ViewObj * lastPage = lastRow->findViewFromParent(ConstantsID::ContainerId::NORMAL_PAGE, true);

	//OperateMgr * operateMgr = mOwner->getOperateMgr();
	WriterManager * mgr = mOwner->getWriterMgr();
	LayoutReplaceManager * replaceMgr = mgr->getLayoutReplaceMgr();
	if (firstPage != lastPage)
	{
		ViewObj * pagePanel = firstPage->getViewParent();
		QList<BaseContainer *> * pageList = pagePanel->getChildren();
		int firstPageIdx = firstPage->getViewIndex();
		int lastPageIdx = lastPage->getViewIndex();
		ViewObj * tempPage = NULL;
		ViewObj * txtLayer = NULL;
		BaseLayout * layerLayout = NULL;
		for (int i = firstPageIdx; i < lastPageIdx + 1; i ++)
		{
			tempPage = (ViewObj *)pageList->at(i);
			txtLayer = (ViewObj *)tempPage->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false)
				->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			layerLayout = txtLayer->getLayout();
			replaceMgr->pushLayout(layerLayout);
		}
	}
	replaceMgr->pushLayout(firstLayout);
	replaceMgr->pushLayout(lastLayout);
}