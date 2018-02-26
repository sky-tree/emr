#ifndef ROW_VIEW_PARAM_H_
#define ROW_VIEW_PARAM_H_



#include "LayoutViewParam.h"

class RowViewParam : public LayoutViewParam
{
public:
	RowViewParam(ViewObj * owner);
	~RowViewParam();



private:
	bool judgeChangeParagraph();
public:
	//int getAscent();
	float getCrossY(ViewObj * layer);
	int getDescent();
	void refreshVerticalParamByChildren();
	void refreshHByChildren(bool circle);
	void refreshWByChildren(bool circle);
	float getChildrenW();

	int getEffectL(ViewObj * child = NULL);
	int getEffectW(ViewObj * child = NULL);

	float getDistanceWithPreRow();
	float getDistanceWithNextRow();
protected:
	//int mAscent;
	int mDescent;
	int mChildrenH;
	//float mUnderLineH;
	//float mUpH;用insideTop代替
	//float mDownH;用insideBottom代替
	int mPreRowDistance;
	int mNextRowDistance;

	ViewObj * mPreFirstChild;

};

#endif
