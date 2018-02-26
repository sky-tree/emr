#include "VerticalViewParam.h"




#include "ViewObj.h"
#include "RowViewParam.h"

#include "ScrollPlaceLayout.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

VerticalViewParam::VerticalViewParam(ViewObj * owner) : LayoutViewParam(owner, ConstantsID::ClassId::VERTICAL_LAYOUT_VIEW_PARAM)
{
	mChildrenDis = 0;
}

VerticalViewParam::~VerticalViewParam()
{
}
int VerticalViewParam::getChildrenDis()
{
	return mChildrenDis;
}
void VerticalViewParam::setChildrenDis(int dis)
{
	mChildrenDis = dis;
}
float VerticalViewParam::getChildrenRowH()
{
	QList<BaseContainer *> * children = mViewOwner->getChildren();
	if (!children)
	{
		return 0;
	}
	int size = children->size();
	if (size < 1)
	{
		return 0;
	}
	ViewObj * row = NULL;
	RowViewParam * rowParam = NULL;
	int rowH = 0;
	float rowPreDis = 0;
	float curY = 0;
	//float curY = getInsideTop() + getInsideBottom();
	for (int i = 0; i < size; i ++)
	{
		row = (ViewObj *)children->at(i);
		rowParam = (RowViewParam *)row->getViewParam();
		// 除了第0行，其他均要加与上一行的间距
		if (i > 0)
		{
			rowPreDis = rowParam->getDistanceWithPreRow();
			curY += rowPreDis;
		}
		rowH = rowParam->getTotalH();
		//rowParam->setY(curY);
		if (i < size - 1)
			curY += rowH + rowParam->getDistanceWithNextRow();
		else
			curY += rowH;
	}
	return curY;
}
void VerticalViewParam::refreshHByChildren(bool circle)
{
	if (mDefaultH == BaseConstants::DefaultWHType::WRAP_CONTENT_INSIDE)
	{
		if (mViewOwner->haveChildren() < 1)
		{
			return;
		}
		QList<BaseContainer *> * children = mViewOwner->getChildren();
		int size = children->size();
		ViewObj * obj = NULL;
		BaseViewParam * param = NULL;
		ViewObj * lastChild = (ViewObj *)children->at(children->size() - 1);
		BaseViewParam * lastChildParam = lastChild->getViewParam();
		float minY = ((ViewObj *)children->at(0))->getViewParam()->getY();
		float maxY = lastChildParam->getY() + lastChildParam->getTotalH();
		float maxInsideH = maxY - minY;

		if (maxInsideH < mMinH && mMinH >= 0)
		{
			maxInsideH = mMinH;
		}
		if (mSelfH != maxInsideH)
		{
			setInsideH(maxInsideH, 0);


			// 如果为页面板
			if (mViewOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
			{
				ViewObj * window = mViewOwner->getWindow();
				if (window)
				{
					LayoutViewParam * winParam = (LayoutViewParam *)window->getViewParam();
					int winInsideH = winParam->getInsideH();
					ViewObj * scrollPanel = (ViewObj *)window->findChildrenByID(ConstantsID::ContainerId::SCROLL_PANEL, false);
					ScrollPlaceLayout * scrollLayout = (ScrollPlaceLayout *)scrollPanel->getLayout();
					int pagePanelH = getInsideH();
					scrollLayout->refleshScollPanel(mY, winInsideH, pagePanelH);
				}
			}

			if (circle)
			{
				ViewObj * viewParent = mViewOwner->getViewParent();
				if (viewParent)
					viewParent->getViewParam()->refreshHByChildren(circle);
			}
		}
	}
}