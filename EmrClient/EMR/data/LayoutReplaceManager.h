#ifndef LAYOUT_REPLACE_MANAGER_H_
#define LAYOUT_REPLACE_MANAGER_H_




#include <QList>


class ViewObj;
class BaseLayout;

class LayoutReplaceData;


class LayoutReplaceManager
{
public:
	LayoutReplaceManager();
	~LayoutReplaceManager();

public:
	void pushLayout(BaseLayout * layout);
	//void clearLayout();
	void replaceLayout();

public:

	QList<LayoutReplaceData *> * mReplaceLayoutList;
};

#endif
