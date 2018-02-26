#ifndef VERTICAL_VIEW_PARAM_H_
#define VERTICAL_VIEW_PARAM_H_



#include "LayoutViewParam.h"

class VerticalViewParam : public LayoutViewParam
{
public:
	VerticalViewParam(ViewObj * owner);
	~VerticalViewParam();


public:
	int getChildrenDis();
	void setChildrenDis(int dis);
	float getChildrenRowH();

	void refreshHByChildren(bool circle);
protected:
	// �Ӷ���������
	int mChildrenDis;

};

#endif
