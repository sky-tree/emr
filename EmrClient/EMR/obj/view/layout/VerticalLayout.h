#ifndef VERTICAL_LAYOUT_H_
#define VERTICAL_LAYOUT_H_


#include "BaseLayout.h"

class ViewObj;
class VerticalViewParam;

#include <QList>

// 坐标布局
class VerticalLayout : public BaseLayout
{
public:
	VerticalLayout(ViewObj * viewOwner);
	VerticalLayout(ViewObj * viewOwner, int classID);
	virtual ~VerticalLayout();


public:
	virtual void replaceChildren();

public:

protected:
	VerticalViewParam * mViewParam;

};

#endif
