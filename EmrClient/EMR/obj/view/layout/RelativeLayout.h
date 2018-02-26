#ifndef RELATIVE_LAYOUT_H_
#define RELATIVE_LAYOUT_H_





#include "BaseLayout.h"

class ViewObj;
class RelativeChildrenParam;

// 子对象管理器
class RelativeLayout : public BaseLayout
{
public:
	RelativeLayout(ViewObj * owner);
	~RelativeLayout();

public:
	void replaceChildren();

public:
	void addChild(ViewObj * child, int idx, bool refreshIdx, char replaceType, RelativeChildrenParam * relativeParams);


protected:
	QList<RelativeChildrenParam *> * mRelativeParams;
};

#endif
