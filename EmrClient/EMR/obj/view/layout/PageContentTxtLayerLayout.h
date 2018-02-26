#ifndef PAGE_CONTENT_TXT_LAYER_LAYOUT_H_
#define PAGE_CONTENT_TXT_LAYER_LAYOUT_H_


#include "VerticalLayout.h"

class ViewObj;
class VerticalViewParam;
class ExcelCrossParentMgr;

#include <QList>

// 坐标布局
class PageContentTxtLayerLayout : public VerticalLayout
{
public:
	PageContentTxtLayerLayout(ViewObj * viewOwner);
	virtual ~PageContentTxtLayerLayout();


public:
	void addChild(BaseContainer * row, int idx, bool refreshIdx, char replaceType);
	void addChildren(QList<BaseContainer *> * rowList, int start, int num, int idx, bool refreshIdx, char replaceType);
	void replaceChildren();

	int delChild(BaseContainer * child, bool refreshIdx, char replaceType);
	int delChildren(int start, int num, bool refreshIdx, char replaceType);

	void delChildWithOutClearCrossParents(BaseContainer * child, bool refreshIdx, char replaceType);
	void delChildrenWithOutClearCrossParents(int start, int num, bool refreshIdx, char replaceType);

	PageContentTxtLayerLayout * getPreLayout();
private:
	void replaceTitleBottomLayer();
	void replaceContentLayer();
	void replaceExcelLayer();

	bool isExcelRowFirstPage(ViewObj * page, int rowIdx, ExcelCrossParentMgr * crossMgr);

	int judgeCanHaveNum();
	int judgeNextPageIndent(int freeH);

	BaseLayout * getNextLayout(bool createWhenLast);
	BaseLayout * createNextLayout();

protected:
	// 剩余H,最后一行与页面最低端之间所消耗的高度,用于两页之间行间距衔接
	int mRestH;
	ViewObj * mPagePanel;
};

#endif
