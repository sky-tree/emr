#ifndef LAYOUT_REPLACE_DATA_H_
#define LAYOUT_REPLACE_DATA_H_


#include <QList>

class BaseLayout;

// 基本坐标数据
class LayoutReplaceData
{
public:
	LayoutReplaceData(int layoutID);
	~LayoutReplaceData();

public:
	bool isDestLayout(int id);
	void pushLayout(BaseLayout * layout);
	void clearLayout();
	void replaceLayout();


private:
	int mLayoutID;
	int mCurReplaceIndex;
	QList<BaseLayout *> * mLayoutList;
};

#endif
