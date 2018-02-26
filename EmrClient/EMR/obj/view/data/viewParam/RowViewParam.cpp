#include "RowViewParam.h"




#include "ViewObj.h"

#include "BaseParagraph.h"


#include "ExcelCrossParentMgr.h"

#include "TxtViewParam.h"
#include "ParagraphData.h"



#include "BaseTxtLayerLayout.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

RowViewParam::RowViewParam(ViewObj * owner) : LayoutViewParam(owner, ConstantsID::ClassId::ROW_VIEW_PARAM)
, mPreFirstChild(NULL)
{
	//mAscent = 0;
	mDescent = 0;
	mPreRowDistance = 0;
	mNextRowDistance = 0;
	mChildrenH = 0;
	mMinH = 0;
}

RowViewParam::~RowViewParam()
{
}
//int RowViewParam::getAscent()
//{
//	return mAscent;
//}
float RowViewParam::getCrossY(ViewObj * layer)
{
	if (!mViewOwner->isExcelRow())
	{
		return mY;
	}
	ExcelCrossParentMgr * mgr = mViewOwner->getExcelCrossParentMgr();
	if (mgr && mgr->haveCrossParent())
	{
		return mgr->getDisY(layer);
	}
	return mY;
}
int RowViewParam::getDescent()
{
	return mDescent;
}
bool RowViewParam::judgeChangeParagraph()
{
	ViewObj * curFirst = (ViewObj *)mViewOwner->getChild(0);
	if (mPreFirstChild == curFirst)
		return false;
	return true;
}
int RowViewParam::getEffectL(ViewObj * child)
{
	if (mViewOwner->haveChildren() < 1)
	{
		return getInsideLeft();
	}
	ViewObj * first = NULL;
	if (mViewOwner->haveChildren() < 1)
	{
		first = child;
	}
	else
		first = (ViewObj *)mViewOwner->getChild(0);
	BaseParagraph * paragraph = first->getParagraphParent();
	ParagraphData * paragraphData = (ParagraphData *)paragraph->getContentData();
	BaseContainer * firstFromParagraph = paragraph->getChild(0);
	int paragraphLeft = paragraphData->getInsideLeft();

	int insideL = getInsideLeft();

	char indentType = paragraphData->getIndentType();
	int indentValue = 0;
	switch (indentType)
	{
		// 首行缩进
	case BaseConstants::IndentType::INDENT_FIRST_ROW:
		// 此行为首行
		if (first == firstFromParagraph)
			indentValue = paragraphData->getIndentValue();
		break;
		// 后续行缩进
	case BaseConstants::IndentType::INDENT_SECOND_ROW:
		// 此行不为首行
		if (first != firstFromParagraph)
			indentValue = paragraphData->getIndentValue();
		break;
	}
	return insideL + paragraphLeft + indentValue;
}
int RowViewParam::getEffectW(ViewObj * child)
{
	if (mViewOwner->haveChildren() < 1)
	{
		return getInsideW();
	}
	ViewObj * first = NULL;
	if (mViewOwner->haveChildren() < 1)
	{
		first = child;
	}
	else
		first = (ViewObj *)mViewOwner->getChild(0);
	BaseParagraph * paragraph = first->getParagraphParent();
	ParagraphData * paragraphData = (ParagraphData *)paragraph->getContentData();
	//int paragraphLeft = paragraphData->getInsideLeft();
	int effectLeft = getEffectL(first);
	int paragraphRight = paragraphData->getInsideRight();

	float insideW = getInsideW();
	return insideW - effectLeft - paragraphRight;
}
float RowViewParam::getChildrenW()
{
	float childrenW = 0;
	QList<BaseContainer *> * children = mViewOwner->getChildren();
	int size = children->size();
	ViewObj * obj = NULL;
	BaseViewParam * param = NULL;
	for (int i = 0; i < size; i ++)
	{
		obj = (ViewObj *)children->at(i);
		if (obj->isEnter())
		{
			continue;
		}

		param = obj->getViewParam();
		childrenW += param->getTotalW();
	}
	return childrenW;
}
float RowViewParam::getDistanceWithPreRow()
{
	float distance = 0;

	ViewObj * first = (ViewObj *)mViewOwner->getChild(0);
	BaseParagraph * paragraph = first->getParagraphParent();
	ParagraphData * paragraphData = (ParagraphData *)paragraph->getContentData();
	// 如果本行为段落首行,则设置于上一行偏差加上本段disTop,否则为0
	if (first == paragraph->getChild(0))
	{
		distance = paragraphData->getParagraphDisTop();
	}

	return distance;
}
float RowViewParam::getDistanceWithNextRow()
{
	float distance = 0;

	ViewObj * last = (ViewObj *)mViewOwner->getLastChild(false);
	BaseParagraph * paragraph = last->getParagraphParent();
	ParagraphData * paragraphData = (ParagraphData *)paragraph->getContentData();
	// 如果本行为段落最后行
	if (last == paragraph->getLastChild(false))
	{
		distance = paragraphData->getParagraphDisBottom();
	}
	else
	{
		distance = paragraphData->getRowDisHValue();
	}

	return distance;
}
void RowViewParam::refreshVerticalParamByChildren()
{
	QList<BaseContainer *> * children = mViewOwner->getChildren();
	ViewObj * child = NULL;
	int size = children->size();
	float curH = 0;
	float ascent = 0;
	float descent = 0;
	for (int i = 0; i < size; i ++)
	{
		child = (ViewObj *)children->at(i);
		BaseViewParam * param = child->getViewParam();
		curH = param->getTotalH();
		if (curH > mChildrenH)
		{
			mChildrenH = curH;
			if (child->isTxt())
			{
				TxtViewParam * txtParam = (TxtViewParam *)param;
				descent = txtParam->getDescent();
				if (descent > mDescent)
				{
					mDescent = descent;
				}
			}
		}
	}
}
void RowViewParam::refreshHByChildren(bool circle)
{
	if (mViewOwner->haveChildren() < 1)
	{
		return;
	}
	mDescent = 0;
	QList<BaseContainer *> * children = mViewOwner->getChildren();
	int size = children->size();
	ViewObj * obj = NULL;
	BaseViewParam * param = NULL;
	ViewObj * lastChild = (ViewObj *)children->at(children->size() - 1);
	BaseViewParam * lastChildParam = lastChild->getViewParam();
	ViewObj * firstChild = (ViewObj *)children->at(0);
	float minY = firstChild->getViewParam()->getY();
	float maxY = lastChildParam->getY() + lastChildParam->getTotalH();
	float maxInsideH = maxY - minY;
	float h = 0;
	float descent = 0;
	for (int i = 0; i < size; i++)
	{
		obj = (ViewObj *)children->at(i);
		param = obj->getViewParam();
		h = param->getTotalH();
		if (h > maxInsideH)
		{
			maxInsideH = h;
		}
		if (obj->isTxt())
		{
			TxtViewParam * txtParam = (TxtViewParam *)param;
			descent = txtParam->getDescent();
			if (descent > mDescent)
			{
				mDescent = descent;
			}
		}
	}
	if (maxInsideH < mMinH && mMinH >= 0)
	{
		maxInsideH = mMinH;
	}
	if (mSelfH != maxInsideH)
	{
		setInsideH(maxInsideH, 0);
		if (circle)
		{
			ViewObj * txtLayer = mViewOwner->getViewParent();
			if (txtLayer)
			{
				bool haveCross = false;
				if (mViewOwner->isExcelRow())
				{
					ExcelCrossParentMgr * crossMgr = mViewOwner->getExcelCrossParentMgr();
					if (crossMgr && crossMgr->haveCrossParent())
					{
						QList<ViewObj *> * parents = crossMgr->getCrossParents();
						int size = parents->size();
						for (int i = 0; i <size; i++)
						{
							txtLayer = parents->at(i);
							BaseTxtLayerLayout * txtLayerLayout = (BaseTxtLayerLayout *)txtLayer->getLayout();
							txtLayerLayout->replaceByType(BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
						}
						haveCross = true;
					}
				}
				if (!haveCross)
				{
					BaseTxtLayerLayout * txtLayerLayout = (BaseTxtLayerLayout *)txtLayer->getLayout();
					txtLayerLayout->replaceByType(BaseConstants::ReplaceType::ADD_TO_REPLACE_LIST);
				}
			}
			//if (txtLayer)
			//	txtLayer->getViewParam()->refreshHByChildren(circle);
		}
	}
}
void RowViewParam::refreshWByChildren(bool circle)
{
	if (mDefaultW == BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE)
	{
		if (mViewOwner->haveChildren() < 1)
		{
			return;
		}
		int childrenW = getChildrenW();
		if (mSelfW != childrenW)
		{
			setInsideW(childrenW, 0);
			if (circle)
			{
				ViewObj * viewParent = mViewOwner->getViewParent();
				if (viewParent)
					viewParent->getViewParam()->refreshWByChildren(circle);
			}
		}
	}
	else if (mDefaultW == BaseConstants::DefaultWHType::FILL_PARENT_INSIDE)
	{
		LayoutViewParam * parentViewParam = (LayoutViewParam *)mViewOwner->getViewParent()->getViewParam();
		setSelfW(parentViewParam->getInsideW());
	}
	else if (mDefaultW == BaseConstants::DefaultWHType::FILL_PARENT_TOTAL)
	{
		LayoutViewParam * parentViewParam = (LayoutViewParam *)mViewOwner->getViewParent()->getViewParam();
		setSelfW(parentViewParam->getTotalW());
	}
}