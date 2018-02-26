#ifndef BASE_TXT_LAYER_LAYOUT_H_
#define BASE_TXT_LAYER_LAYOUT_H_


#include "VerticalLayout.h"

class ViewObj;
class VerticalViewParam;

#include <QList>

// 坐标布局
class BaseTxtLayerLayout : public VerticalLayout
{
public:
	BaseTxtLayerLayout(ViewObj * viewOwner, int classID);
	virtual ~BaseTxtLayerLayout();


public:
	virtual void replaceChildren();

public:

protected:

};

#endif
