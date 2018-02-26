#ifndef _CROSS_PARENT_MGR_H_
#define _CROSS_PARENT_MGR_H_


#include <QDebug>

class ViewObj;
class BaseKey;
#include "BaseContainerParam.h"

// 页眉页脚跨多页管理
class CrossParentMgr :public BaseContainerParam
{
public:
	CrossParentMgr(ViewObj * view);
	CrossParentMgr(ViewObj * view, int classId);
	~CrossParentMgr();

private:
	void init();

public:
	void setActiveParent(ViewObj * parent);
	ViewObj * getActiveParent();

	//QList<ViewObj *> * getCrosssParents();
	bool haveCrossParent();

	void addParent(ViewObj * parent);
	void removeParent(ViewObj * parent);

	ViewObj * getFirstCrossParent();
	ViewObj * getLastCrossParent();
	QList<ViewObj *> * getCrossParents();

private:
	//void refindActiveParent();
protected:
	ViewObj * mViewOwner;
	// 当前激活父对象
	ViewObj * mActiveParent;
	// 所跨父对象列表
	QList<ViewObj *> * mCrossParents;

};

#endif
