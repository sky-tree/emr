#include "ObjDrawManager.h"




#include "OperateMgr.h"

#include "ExcelCrossParentMgr.h"

#include "BaseViewParam.h"

#include "BaseDraw.h"
#include "ViewObj.h"

#include "BaseChildrenManager.h"
#include "StrContentData.h"
#include "DrawStr.h"
#include "ExcelCellData.h"
#include "BaseSystem.h"
#include "BaseMath.h"

#include "BaseConstants.h"
#include "ConstantsID.h"
#include "ConstantsNumber.h"
#include "BaseTools.h"

ObjDrawManager::ObjDrawManager(ViewObj * owner) : BaseContainerParam(owner, ConstantsID::ClassId::OBJ_DRAW_MANAGER)
, mViewOwner(owner), mDrawBottom(NULL), mDrawTop(NULL)
, mDrawTopDisX(0), mDrawTopDisY(0), mDrawBottomDisX(0), mDrawBottomDisY(0)
{
	owner->initDrawMgr(this);
	initDefaultParams();
}
ObjDrawManager::ObjDrawManager(ViewObj * owner, int classID) : BaseContainerParam(owner, classID)
, mViewOwner(owner), mDrawBottom(NULL), mDrawTop(NULL)
, mDrawTopDisX(0), mDrawTopDisY(0), mDrawBottomDisX(0), mDrawBottomDisY(0)
{
	owner->initDrawMgr(this);
	initDefaultParams();
}
ObjDrawManager::ObjDrawManager(ViewObj * owner, ObjDrawManager * source) : BaseContainerParam(owner, source->getClassId())
, mViewOwner(owner)
, mDrawTopDisX(source->mDrawTopDisX)
, mDrawTopDisY(source->mDrawTopDisY)
, mDrawBottomDisX(source->mDrawBottomDisX)
, mDrawBottomDisY(source->mDrawBottomDisY)
, mDrawTopW(source->mDrawTopW)
, mDrawTopH(source->mDrawTopH)
, mDrawBottomW(source->mDrawBottomW)
, mDrawBottomH(source->mDrawBottomH)
{
	if (source->mDrawBottom)
		mDrawBottom = BaseSystem::getInstance()->getDrawCopy(mViewOwner, source->mDrawBottom);
	else
		mDrawBottom = NULL;
	if (source->mDrawTop)
		mDrawTop = BaseSystem::getInstance()->getDrawCopy(mViewOwner, source->mDrawTop);
	else
		mDrawTop = NULL;

	owner->initDrawMgr(this);
}

ObjDrawManager::~ObjDrawManager()
{
	if (mDrawBottom)
	{
		delete mDrawBottom;
		mDrawBottom = NULL;
	}
	if (mDrawTop)
	{
		delete mDrawTop;
		mDrawTop = NULL;
	}
	mViewOwner = NULL;
}

void ObjDrawManager::initDraws(BaseDraw * bottom, BaseDraw * top)
{
	mDrawBottom = bottom;
	mDrawTop = top;
}
void ObjDrawManager::initDefaultParams()
{
	mDrawTopW = ConstantsNumber::MIN_CHAR;
	mDrawTopH = ConstantsNumber::MIN_CHAR;
	mDrawBottomW = ConstantsNumber::MIN_CHAR;
	mDrawBottomH = ConstantsNumber::MIN_CHAR;
}

void ObjDrawManager::initDraws(BaseDraw * bottom, BaseDraw * top, int bottomX, int bottomY, int topX, int topY)
{
	mDrawBottom = bottom;
	mDrawTop = top;
	mDrawTopDisX = topX;
	mDrawTopDisY = topY;
	mDrawBottomDisX = bottomX;
	mDrawBottomDisY = bottomY;
}

void ObjDrawManager::initDrawsParams(int topW, int topH, int bottomW, int bottomH)
{
	mDrawTopW = topW;
	mDrawTopH = topH;
	mDrawBottomW = bottomW;
	mDrawBottomH = bottomH;
}
void ObjDrawManager::setBottomDraw(BaseDraw * bottom)
{
	if (mDrawBottom)
	{
		delete mDrawBottom;
	}
	mDrawBottom = bottom;
}
BaseDraw *ObjDrawManager::getBottomDraw()
{
	return mDrawBottom;
}
int ObjDrawManager::draw(QPainter &pPainter, int x, int y, bool isPrint)
{
	//long startT = BaseTools::refreshStartTime();
	if (mViewOwner->getObjID() == ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW
		|| mViewOwner->getObjID() == ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_RECT_VIEW)
	{
		int a = 0;
	}
	if (mViewOwner->getObjID() == ConstantsID::ContainerId::EXCEL_PAGE)
	{
		if (isPrint)
		{
			ExcelCellData* cellData = (ExcelCellData*)mViewOwner->getContentData();
			if (!cellData->getCanPrint())
			{
				return 0;
			}
		}
	}
	BaseViewParam * viewParam = mViewOwner->getViewParam();
	if (viewParam->getViewType() < BaseConstants::ViewType::NORMAL)
	{
		return 0;
	}
	OperateMgr * operateMgr = mOwner->getOperateMgr();
	int selfTotalW = viewParam->getTotalW();
	int selfTotalH = viewParam->getTotalH();

	int returnType = BaseConstants::NONE;

	bool isCrossPage = false;
	if (mViewOwner->isExcelRow())
	{
		ExcelCrossParentMgr * crossMgr = mViewOwner->getExcelCrossParentMgr();
		if (crossMgr && crossMgr->haveCrossParent())
		{
			isCrossPage = true;
			ViewObj * page = mOwner->getOperateMgr()->getEffectLoopPage();
			BaseContainer * pageContent = page->findChildrenByID(ConstantsID::ContainerId::PAGE_CONTENT, false);
			ViewObj * layer = (ViewObj *)pageContent->findChildrenByID(ConstantsID::ContainerId::CONTENT_TXT_LAYER, false);
			int disY = crossMgr->getDisY(layer);
			int spendH = crossMgr->getSpendH(layer);

			x += viewParam->getX();
			y += disY;
		}
	}
	if (!isCrossPage)
	{
		x += viewParam->getX();
		y += viewParam->getY();
	}
	int * totalClip = operateMgr->getTotalClip();
	ViewObj * viewParent = mViewOwner->getViewParent();
	// 如果父对象为行，则仅判断横向显示范围
	if (viewParent && viewParent->isRow())
	{
		if (x < totalClip[0]
			|| x > totalClip[0] + totalClip[2]
			)
		{
			return 0;
		}
	}
	else if (!BaseMath::isRectCrossWH(x, y, selfTotalW, selfTotalH, totalClip[0], totalClip[1], totalClip[2] - totalClip[0], totalClip[3] - totalClip[1]))
	{
		return 0;
	}
	//if (mViewOwner->getObjID() == ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW)
	//{
	//	int i = 0;
	//}
	//if (mViewOwner->getObjID() == ConstantsID::ContainerId::CONTENT_TXT_LAYER)
	//{
	//	int i = 0;
	//}
	//if (mViewOwner->getObjID() == ConstantsID::ContainerId::EXCEL_TXT_LAYER)
	//{
	//	int i = 0;
	//}
	if (mDrawBottom)
	{
		if (!isPrint || !mViewOwner->isObj(ConstantsID::ContainerId::NORMAL_PAGE))
			mDrawBottom->draw(pPainter, x + mDrawBottomDisX, y + mDrawBottomDisY, mDrawBottomW
			, mDrawBottomH);
	}
	// 如果需要设置显示范围
	int needSetClipBack = -2;
	QVector<QRect> rectList;
	int * clip = viewParam->getClip();
	/*仅显示自身范围*/
	if (clip)
	{
		rectList = BaseTools::getCurClip(pPainter);
		if (rectList.size() >= 0)
			needSetClipBack = -1;
		for (int i = 0; i < rectList.size(); i++)
		{
			needSetClipBack = i;
		}
		if (mViewOwner->isObj(ConstantsID::ContainerId::CONTENT_TXT_LAYER))
		{
			int a = 0;
		}
		int clipX = x + clip[0];
		int clipY = y + clip[1];
		int destW = selfTotalW - (clip[0] + clip[2]);
		int destH = selfTotalH - (clip[1] + clip[3]);

		QRect rect;
		if (needSetClipBack >= 0)
		{
			/* 如果不为页层*/
			//if (mObjId != ConstantsID::ContainerId::LAYER_TXT)
			{
				rect = rectList.at(needSetClipBack);
				int dis = clipX - rect.x();
				if (dis < 0)
				{
					clipX = rect.x();
					destW += dis;
				}
				if (clipX + destW > rect.right())
				{
					destW = rect.right() - clipX;
				}
				dis = clipY - rect.y();
				//if (clipY < rect.y())
				//{
				//	//clipY = rect.y();
				//	//destH += dis;
				//}
				if (destH < 0)
				{
					clip = NULL;
					return 0;
				}
				//else if (clipY + destH > rect.bottom())
				//{
				//	//destH = rect.bottom() - clipY;
				//}
			}
		}
		if (destH > 0)
		{
			operateMgr->refreshTotalClipInside(clipX, clipY, destW, destH);
			pPainter.setClipRect(clipX, clipY, destW, destH);
		}
		else
			return 0;
	}
	//BaseTools::printSpendTime(startT, "ObjDrawMgr::draw 111", mOwner->getObjID());
	if (mViewOwner->isObj(ConstantsID::ContainerId::EXCEL_PAGE))
	{
		int maskColor = mViewOwner->getMaskColor();
		if (maskColor)
		{
			QColor * colorObj = BaseMath::createColor(maskColor);
			pPainter.fillRect(x, y, selfTotalW, selfTotalH, *colorObj);
		}
	}

	bool needDrawChildren = true;

	QList<BaseContainer *> * children = mOwner->getChildren();
	int size = 0;
	if (children)
	{
		size = children->size();
	}
	// 如果为页眉页脚图层，则判断如果内容仅有一行，且仅有一个回车，则不绘子对象
	if (mViewOwner->isObj(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER))
	{
		if (size == 1)
		{
			BaseContainer * firstRow = mViewOwner->getChild(0);
			// 如果仅有一个回车，同时自身父对象不处于被激活状态
			if (firstRow->getChild(0)->isEnter() && mViewOwner->getViewParent()->getStatus() != BaseConstants::ObjStatus::ACTIVE)
			{
				needDrawChildren = false;
			}
		}
	}
	//BaseTools::printSpendTime(startT, "ObjDrawMgr::draw 222", mOwner->getObjID());
	if (needDrawChildren && size > 0)
	{
		int size = children->size();
		ViewObj * child = NULL;
		OperateMgr * operateMgr = mOwner->getOperateMgr();
		if (mViewOwner->isRow())
		{
			StrContentData * sourceDrawStrParam = NULL;
			QList<StrContentData *> * drawParamList = mViewOwner->getOperateMgr()->getBufDrawParamList();
			BaseViewParam * childViewParam = NULL;
			BaseChildrenManager * childMgr = mViewOwner->getChildrenMgr();
			for (int i = 0; i < size; i++)
			{
				int preI = i;
				ViewObj * view = (ViewObj *)childMgr->getChild(i);
				if (view->isTxt())
				{
					//if (view->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW))
					//{
					//	int a = 0;
					//}
					drawParamList->clear();
					sourceDrawStrParam = (StrContentData *)view->getContentData();
					BaseContainer * curTxt = NULL;
					for (int j = i + 1; j < size; j++)
					{
						curTxt = childMgr->getChild(j);
						if (curTxt->isTxt())
						{
							if (!curTxt->isView())
							{
								break;
							}
							StrContentData * curDrawStrParam = (StrContentData *)curTxt->getContentData();
							/*如果两个字属性相同，则可以一次性绘制*/
							if (!view->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW) && sourceDrawStrParam->isSameTxtType(curDrawStrParam)
								&& !curTxt->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW))
							{
								drawParamList->push_back(curDrawStrParam);
							}
							else
								break;
							i++;
						}
						else
							break;
					}
					DrawStr * drawStr = (DrawStr *)view->getDrawMgr()->getBottomDraw();
					if (drawParamList->size() > 0)
					{
						drawStr->drawStrList(pPainter, x, y, drawParamList);
						drawParamList->clear();
					}
					else
					{
						view->getDrawMgr()->draw(pPainter, x, y, isPrint);
					}
				}
				else
				{
					if (!isPrint || !view->isEnter())
						view->getDrawMgr()->draw(pPainter, x, y, isPrint);
				}
			}
		}
		else
		{
			for (int i = 0; i < size; i++)
			{
				child = (ViewObj *)children->at(i);
				// 将当前页设置为活动页
				if (mOwner->isObj(ConstantsID::ContainerId::PAGE_PANEL))
				{
					operateMgr->setEffectLoopPage(child);
				}
				//if (mOwner->isObj(ConstantsID::ContainerId::NORMAL_PAGE) && i> 1)
				//{
				//	int a = 0;
				//}
				returnType = child->getDrawMgr()->draw(pPainter, x, y, isPrint);
				if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
				{
					return returnType;
				}
			}
		}
	}

	//BaseTools::printSpendTime(startT, "ObjDrawMgr::draw 333", mOwner->getObjID());
	if (mDrawTop)
	{
		mDrawTop->draw(pPainter, x + mDrawTopDisX, y + mDrawTopDisY, mDrawTopW
			, mDrawTopH);
	}
	//BaseTools::printSpendTime(startT, "ObjDrawMgr::draw 555", mOwner->getObjID());

	// 恢复范围*/
	if (clip)
	{
		ViewObj * rootView = mViewOwner->getParentViewById(ConstantsID::ContainerId::ROOT_VIEW);
		BaseViewParam * rootViewParam = rootView->getViewParam();
		int w = 0;
		int h = 0;
		if (needSetClipBack == -1)
		{
			w = rootViewParam->getTotalW();
			h = rootViewParam->getTotalH();
			pPainter.setClipRect(0, 0, w, h, Qt::ReplaceClip);
			operateMgr->refreshTotalClipInside(0, 0, w, h);
		}
		else if (needSetClipBack >= 0)
		{
			QRect rect = rectList.at(needSetClipBack);
			int tx = rect.x();
			int ty = rect.y();
			w = rect.width();
			h = rect.height();
			pPainter.setClipRect(tx, ty, w, h, Qt::ReplaceClip);
			operateMgr->refreshTotalClipOutside(tx, ty, w, h);
		}
	}
	//BaseTools::printSpendTime(startT, "ObjDrawMgr::draw 666", mOwner->getObjID());
	return returnType;
}