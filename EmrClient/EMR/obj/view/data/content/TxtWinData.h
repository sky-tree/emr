#ifndef TXT_WIN_DATA_H_
#define TXT_WIN_DATA_H_



#include "BaseContentData.h"

class ViewObj;
class FlagParam;

class TxtWinData : public BaseContentData
{
public:
	TxtWinData(BaseContainer * owner);
	~TxtWinData();

public:
	void init();

public:
	ViewObj * getTitleView();
	ViewObj * getBottomView();

	ViewObj * getPagePanel();
	QList<BaseContainer *> * getPageList();
	int getPageListNum();

protected:
	// windowView并不需要存、读档
	void saveSelfData(BaseIOTools *iOTools){};
	void loadSelfData(BaseIOTools *iOTools){};
private:
	ViewObj * mTitle;
	ViewObj * mBottom;

	ViewObj * mPagePanel;
	QList<BaseContainer *> * mPageList;
};

#endif
