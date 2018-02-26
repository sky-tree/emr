#ifndef _SETPAGEPROPERTYACTION_H_
#define _SETPAGEPROPERTYACTION_H_


#include "qmap.h"

#include "BaseAction.h"

//class WriterManager;

class SetPagePropertyAction : public BaseAction
{
public:
	SetPagePropertyAction(StepActionManger * stepActonManger);
	~SetPagePropertyAction();
	//void initAction(WriterManager* writeMgr, QMap<QString, QString> preMap, QMap<QString, QString> setMap);
	void doAction(bool isHorizontal, short paperType, int pageInsideT, int pageInsideB, int pageInsideL, int pageInsideR, int titleMaxH, int bottomMaxH);
	void unDo();
	void reDo();

private:
	bool mIsHorizontal;
	short mPaperType;
	int mPageInsideTop;
	int mPageInsideBottom;
	int mPageInsideLeft;
	int mPageInsideRight;
	int mTitleMaxH;
	int mBottomMaxH;

	bool mPreIsHorizontal;
	short mPrePaperType;
	int mPrePageInsideTop;
	int mPrePageInsideBottom;
	int mPrePageInsideLeft;
	int mPrePageInsideRight;
	int mPreTitleMaxH;
	int mPreBottomMaxH;
	//QMap<QString, QString> mPreMap;
	//QMap<QString, QString> mSetMap;
	//WriterManager* mWriteMgr;
};
#endif