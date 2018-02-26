#ifndef ROW_LAYOUT_H_
#define ROW_LAYOUT_H_


#include "BaseLayout.h"

class ViewObj;
class VerticalViewParam;
class RowViewParam;

#include <QList>

// 坐标布局
class RowLayout : public BaseLayout
{
public:
	RowLayout(ViewObj * viewOwner);
	virtual ~RowLayout();


public:
	BaseParagraph * getParagraph();
public:
	int judgeCanHaveNumAfterAdd(QList<BaseContainer *> * children, int start, int num, int idx);
	void judgeAddChildren(QList<BaseContainer *> * children, int start, int num, int idx, char needReplace);

	void addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx, char needReplace);
	int delChildren(int start, int num, bool refreshIdx, bool needReplace);

	bool judgeLessThanCircle(bool needJudgeEnter);
	void judgeOverFlowCircle();
	bool judgeLessThanCircleUntilObj(BaseContainer * last);
	void judgeOverFlowCircleUntilObj(BaseContainer * last);

	void replaceChildren();

	BaseLayout * getPreLayout(bool createWhenLast);
	BaseLayout * getNextLayout(bool createWhenLast);
private:
	BaseLayout * createPreLayout();
	BaseLayout * createNextLayout();

	// 判断溢出
	RowLayout * judgeOverflow();

	// 判断不足
	RowLayout * judgeLessThan(bool & haveIndent, bool needJudgeEnter);

protected:
	RowViewParam * mRowParam;

};

#endif
