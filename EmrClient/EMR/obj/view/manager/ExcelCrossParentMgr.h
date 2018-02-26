#ifndef _EXCEL_CROSS_PARENT_MGR_H_
#define _EXCEL_CROSS_PARENT_MGR_H_


#include <QDebug>

class ViewObj;

#include "CrossParentMgr.h"

// Ò³Ã¼Ò³½Å¿ç¶àÒ³¹ÜÀí
class ExcelCrossParentMgr :public CrossParentMgr
{
public:
	ExcelCrossParentMgr(ViewObj * view);
	~ExcelCrossParentMgr();

public:
	void setFirstParam(ViewObj * parent, int y, int spendH);
	void pushParam(ViewObj * parent, int spendH);

	void clearParents(bool needRemoveFromTxtLayer);
	void clearParentsWithoutSource(ViewObj * source);
	void clearParentsAfterSource(ViewObj * source);

	int getDisY(ViewObj * layer);
	int getSpendH(ViewObj * layer);
	int getTotalPreSpendH(ViewObj * layer);

private:
	void clearParams(int start = -1);

private:
	// YÆ«ÒÆ
	//QList<int> * mDisYList;
	int mFirstPageDisY;
	// ÏûºÄH
	QList<int> * mSpendHList;
};

#endif	// _EXCEL_CROSS_PARENT_MGR_H_