#ifndef IMG_VIEW_PARAM_H_
#define IMG_VIEW_PARAM_H_


#include "BaseViewParam.h"
class ViewObj;
class NormalRowParam;

#include <QDebug>

// 显示相关属性
class ImgViewParam : public BaseViewParam
{
public:
	ImgViewParam(ViewObj * viewOwner);
	ImgViewParam(ViewObj * viewOwner, ImgViewParam * source);
	~ImgViewParam();

public:

public:

public:

	void setSourceW(int w);
	void setSourceH(int h);

	float getSourceW();
	float getSourceH();

protected:

	int mSourceW;
	int mSourceH;

protected:

};

#endif
// _IMG_VIEW_PARAM_H_
