#ifndef _JUDGE_PAGE_PANEL_SCROLL_LOGIC_H_
#define _JUDGE_PAGE_PANEL_SCROLL_LOGIC_H_


#include "BaseLogic.h"

class BaseContainer;
class ViewObj;
class BaseViewParam;

// 字符串绘制
class JudgePagePanelScrollLogic : public BaseLogic
{
public:
	JudgePagePanelScrollLogic(BaseContainer * viewOwner);
	~JudgePagePanelScrollLogic();

public:

	int myUpdate();

private:
	ViewObj * mPagePanel;
	BaseViewParam * mPagePanelParam;
};

#endif // _JUDGE_PAGE_PANEL_SCROLL_LOGIC_H_
