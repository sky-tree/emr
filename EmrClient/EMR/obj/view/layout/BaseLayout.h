#ifndef BASE_LAYOUT_H_
#define BASE_LAYOUT_H_


#include <QList>



#include "BaseChildrenManager.h"

class ViewObj;

// 子对象管理器
class BaseLayout : public BaseChildrenManager
{
public:
	BaseLayout(ViewObj * owner, int classID);
	BaseLayout(ViewObj * owner);
	~BaseLayout();

protected:


private:
public:
	void replaceByType(char replaceType);

	virtual void addChild(BaseContainer * row, int idx, bool refreshIdx, char replaceType);
	virtual void addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx, char needReplace);

	virtual int delChild(BaseContainer * child, bool refreshIdx, char replaceType);
	virtual int delChildren(int start, int num, bool refreshIdx, char replaceType);
	// 对子对象重新排版
	virtual void replaceChildren();

	ViewObj * getViewOwner();
	void openReplace();
	void closeReplace();

	bool isRelativeLayout();

	virtual BaseLayout * getNextLayout(bool createWhenLast);
private:
	void judgeRemovePage(BaseContainer * page);
	void judgeAddPage(BaseContainer * page);
protected:
	bool canReplace();
	virtual BaseLayout * createNextLayout();

private:
	void judgeRefreshPageNum(int preNum);
	void replaceNumber(bool isPlus, ViewObj * pageNum);

private:


public:

protected:
	ViewObj * mViewOwner;
private:
	bool mNeedReplaceChildren;

};

#endif
