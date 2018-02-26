#ifndef _SCROLL_PLACE_LAYOUT_
#define _SCROLL_PLACE_LAYOUT_
#pragma once
#include "BaseLayout.h"

class ScrollPlaceLayout :
	public BaseLayout
{
public:
	ScrollPlaceLayout(ViewObj * owner);
	~ScrollPlaceLayout();
public:
	void addChild(ViewObj * child, int idx, bool refreshIdx, char replaceType);
	void replaceChildren();
	void refleshScollPanel(int y, int effectH, int totalH);
};

#endif