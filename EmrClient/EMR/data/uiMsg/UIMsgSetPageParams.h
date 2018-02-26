#ifndef _UI_MSG_SET_PAGE_PARAMS_H_
#define _UI_MSG_SET_PAGE_PARAMS_H_


#include "BaseUIMsgData.h"

// UI 文字int属性设置事件（需要事件类型，以及int参数,包括char、short、int）
class UIMsgSetPageParams : public BaseUIMsgData
{
public:
	UIMsgSetPageParams(WriterManager * writerMgr, bool isHorizontal, short paperType, int pageInsideTop, int pageInsideBottom, int pageInsideLeft, int pageInsideRight, int titleMaxH, int bottomMaxH);
	~UIMsgSetPageParams();

public:
	void doAction();

protected:
	bool mIsHorizontal;
	short mPaperType;
	int mPageInsideTop;
	int mPageInsideBottom;
	int mPageInsideLeft;
	int mPageInsideRight;
	int mTitleMaxH;
	int mBottomMaxH;
};

#endif	// _UI_MSG_SET_PAGE_PARAMS_H_