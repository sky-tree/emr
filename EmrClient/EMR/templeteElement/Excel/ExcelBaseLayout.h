#ifndef _EXCELBASETEMPLETE_H_
#define _EXCELBASETEMPLETE_H_
#include "BaseContainer.h"
#include "MeasureWH.h"
class ExcelBaseLayout 
{
public:
	ExcelBaseLayout();
	~ExcelBaseLayout();
	
	virtual void onMeasure(MeasureWH * measureWH, bool isFreshFromInnerChange) = 0;
	virtual void onLayout(bool isChanged,int l,int t,int r,int b) = 0;
protected :
	void setMeasureWH(int width, int height,int contentHeight){
		mMeasureWidth = width;
		mMeasureHeight = height;
		mContentHeight = contentHeight;
	}
	//virtual void composing() = 0;
public:
	//char mComposingWidthType; //宽度排版类型
	//char mComposingHeightType;//高度排版类型
	int mMeasureWidth;
	int mMeasureHeight;
	int mContentHeight;
	int mX;
	int mY;
};
#endif

