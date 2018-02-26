#ifndef TXT_WINDOW_MOUSE_H_
#define TXT_WINDOW_MOUSE_H_



#include <QList>

#include "BaseMouse.h"


class BaseContainer;
class ViewObj;

class OperateMgr;
class RightClickMenu;

// 普通页文字录入层鼠标输入处理
class TxtWindowMouse : public BaseMouse
{
public:
	TxtWindowMouse(ViewObj * viewOwner);
	virtual ~TxtWindowMouse();
public:

	virtual int mousePress(int type, int x, int y);
	virtual int mouseRelease(int type, int x, int y);
	virtual int mouseDrag(int type, int x, int y);
	virtual int mouseMove(int type, int x, int y);
	virtual int mouseDoublePress(int type, int x, int y);

private:
	void judgeInitPagePanel();
	ViewObj * getFocusPage(QList<BaseContainer *> * pageList, int &x, int &y);

	int doPress(char type, int x, int y);

	ViewObj * getPagePartTxtLayer(char pressType, ViewObj * page, int &x, int &y);
	ViewObj * judgePart(char pressType, ViewObj * page, int x, int y);
	// 获取聚焦行，没有行输入，聚焦文字就在页这里进行判断
	ViewObj * getPointInsideRow(ViewObj * layer, QList<BaseContainer *> * rowList, int &x, int &y);

	ViewObj * getPointInsideObj(OperateMgr * mgr, QList<BaseContainer *> * objList, int x, int y, char & dir, char pressType);


private:


private:
	ViewObj * mPagePanel;
	QList<BaseContainer *> * mPageList;
	RightClickMenu* mRightMenu;
	int mFocusPartID;
};

#endif // _BASE_TXT_LAYER_MOUSE_H_
