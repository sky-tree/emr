#ifndef VIEW_OBJ_H_
#define VIEW_OBJ_H_



#include <QPainter>

#include "BaseContainer.h"
#include "BaseConstants.h"

class WriterManager;
class CrossParentMgr;
class ExcelCrossParentMgr;
class ObjKeyManager;
class ObjMouseManager;
class ObjDrawManager;

class BaseLayout;
class TxtWinData;
class RelativeChildrenParam;

class BaseViewParam;

class ViewObj : public BaseContainer
{
public:
	ViewObj(WriterManager * writerMgr, int objID);
	ViewObj(WriterManager * writerMgr, int classId, int objID, bool isNeedNewInputManager);//isNeedNewInputManager�Ƿ���Ҫnew  ����������

	ViewObj(WriterManager * writerMgr, ViewObj * source);
	virtual ~ViewObj();

public:
	void initViewParam(BaseViewParam * param);
	void initDrawMgr(ObjDrawManager * drawMgr);
	void initCrossParentMgr(CrossParentMgr * mgr);
public:

	virtual ObjKeyManager * getKeyMgr();
	virtual ObjMouseManager * getMouseMgr();
	ObjDrawManager * getDrawMgr();
	CrossParentMgr * getCrossParentMgr();
	ExcelCrossParentMgr * getExcelCrossParentMgr();
	ViewObj * getViewParent();
	ViewObj * getParentTxtLayer(bool circle);


	void setViewIndex(int idx);

	void refreshChildrenViewIndex(int idx);

	BaseViewParam * getViewParam();

	void setRelativeParam(RelativeChildrenParam * source);
	RelativeChildrenParam * getRelativeParam();

	BaseLayout * getLayout();
	ViewObj * getParentPage();
	ViewObj * getParentPagePanel();

	ViewObj * getWindow();
	TxtWinData * getWinData();

	int getViewIndex();
	int getParagraphIndex();
	void setParagraphIndex(int idx);

	ViewObj * getPreViewFromViewParent();
	ViewObj * getNextViewFromViewParent();

	ViewObj * getPreViewFromParagraphParent();
	ViewObj * getNextViewFromParagraphParent();


	bool isInDest(int objID);

	void setFocus(bool isRight, bool isFreshStrData=true, char pressType = BaseConstants::MousePressType::PRESS_AND_RELEASE);
	void setFocus1(char direction, bool isFreshStrData = true, char pressType = BaseConstants::MousePressType::PRESS_AND_RELEASE);
	void setFocusByX(int pressX, char pressType);
public:
	void setViewType(char viewType);
	void releaseFromWindow();
	virtual bool setDrawTxtParam(StrContentData* focusData);
private:
	ViewObj * mWindow;
	TxtWinData * mWinData;

	ObjDrawManager * mDrawMgr;
protected:
	ObjKeyManager * mKeyMgr;
	ObjMouseManager * mMouseMgr;
private:
	// �縸���������
	CrossParentMgr * mCrossMgr;

	BaseViewParam * mViewParam;
	// ���������RelativeLayout��ʱ�������Լ���ӵ�еĹ�ϵ����
	RelativeChildrenParam * mRelativeParam;



	int mViewIndex;
	int mParagraphIndex;
};

#endif
