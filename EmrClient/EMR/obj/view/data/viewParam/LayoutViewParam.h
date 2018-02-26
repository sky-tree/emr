#ifndef LAYOUT_VIEW_PARAM_H_
#define LAYOUT_VIEW_PARAM_H_



#include "BaseViewParam.h"

class LayoutViewParam : public BaseViewParam
{
public:
	LayoutViewParam(ViewObj * owner);
	LayoutViewParam(ViewObj * owner, int classID);
	~LayoutViewParam();


public:
	void initInside(int inL, int inR, int inT, int inB);

	void initMinH(int h);

	int getInsideLeft();
	void setInsideLeft(int insideLeft);
	int getInsideRight();
	void setInsideRight(int insideright);

	void setInsideTop(int value);
	void setInsideBottom(int value);
	int getInsideTop();
	int getInsideBottom();
	int getInsideW();
	int getInsideH();

	void refreshPreInsideW();
	void refreshPreInsideH();
	int getPreInsideW();
	void setPreInsideH(int value);
	int getPreInsideH();

	void refreshInsideW(char childrenOrParentType);
	void refreshInsideH(char childrenOrParentType);
	void setInsideW(int value, char childrenOrParentType);
	void setInsideH(int value, char childrenOrParentType);

	bool judgeCanResetHByChildren(int maxChildrenH);

	virtual void refreshWByChildren(bool circle);
	virtual void refreshHByChildren(bool circle);

	virtual float getChildrenW();
	virtual float getChildrenRowH();
	//void openChildrenW();
	//virtual void refreshChildrenW();

protected:
	int mInsideLeft;
	int mInsideRight;
	int mInsideTop;
	int mInsideBottom;

	int mPreInsideW;
	int mPreInsideH;

	int mMinH;

	//float mChildrenW;
};

#endif
