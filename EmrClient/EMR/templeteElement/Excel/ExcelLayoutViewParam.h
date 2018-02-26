#ifndef _EXCELLAYOUTVIEWPARAM_H_
#define _EXCELLAYOUTVIEWPARAM_H_
#include "BaseViewParam.h"
class ExcelLayoutViewParam : public BaseViewParam
{
public:
	ExcelLayoutViewParam(ViewObj * viewOwner);
	~ExcelLayoutViewParam();
public:
	void setInsideLeft(int insideLeft);
	void setInsideRight(int insideLeft);
	void setInsideTop(int insideTop);
	void setInsideBottom(int insideBottom);
	int getInsideLeft();
	int getInsideRight();
	int getInsideTop();
	int getInsideBottom();

protected:

	// 自身内部左、右边距：用于控制子对象x向范围
	int mInsideLeft;
	int mInsideRight;
	// 自身内部上、下边距：用于控制子对象y向范围
	int mInsideTop;
	int mInsideBottom;
};
#endif

