#include "BaseDraw.h"

#include "ViewObj.h"

#include "RowViewParam.h"


#include "BaseConstants.h"
#include "ConstantsID.h"

BaseDraw::BaseDraw(ViewObj * owner, int classID) : BaseContainerParam(owner, classID)
{
	mViewOwner = (ViewObj *)mOwner;
}
BaseDraw::BaseDraw(ViewObj * owner, BaseDraw *otherDraw) : BaseContainerParam(owner, otherDraw, otherDraw->getClassId())
{
	mViewOwner = (ViewObj *)mOwner;
}
BaseDraw::~BaseDraw()
{
}
RowViewParam * BaseDraw::getRowParam()
{
	ViewObj * row = mViewOwner->findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, true);
	RowViewParam * rowParam = (RowViewParam *)row->getViewParam();
	return rowParam;
}
bool BaseDraw::draw(QPainter &pPainter, int x, int y, int w, int h)
{
	return false;
}
void BaseDraw::setViewOwner(ViewObj * owner)
{
	mViewOwner = owner;
	mOwner = owner;
}