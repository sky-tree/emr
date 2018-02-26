#ifndef TXT_VIEW_PARAM_H_
#define TXT_VIEW_PARAM_H_


#include "BaseViewParam.h"

class ViewObj;
class RowViewParam;

#include <QFontMetrics>

// 显示相关属性
class TxtViewParam : public BaseViewParam
{
public:
	TxtViewParam(ViewObj * viewOwner);
	TxtViewParam(ViewObj * viewOwner, TxtViewParam * source);
	~TxtViewParam();

public:
	//void initAscent(float ascent, float disAscent);

public:

	//void refresh();
	//void refreshRowParam();

public:

	int getAscent();
	void setAscent(int value);
	int getDescent();
	void setDescent(int value);
	//float getDisAscent();

	//NormalRowParam * getRowParam();

protected:

	int mAscent;
	int mDescent;
	//float mDisAscent;

	//NormalRowParam * mRowParam;

protected:

};

#endif
