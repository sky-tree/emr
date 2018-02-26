#ifndef EXCEL_CELL_PAGET_MOUSE_H_
#define EXCEL_CELL_PAGET_MOUSE_H_



#include <QList>

#include "BaseMouse.h"


class BaseContainer;
class ViewObj;

class OperateMgr;


// 普通页文字录入层鼠标输入处理
class  ExcelCellPageMouse : public BaseMouse
{
public:
	ExcelCellPageMouse(ViewObj * viewOwner);
	virtual ~ExcelCellPageMouse();
public:

	virtual int mousePress(int type, int x, int y);
	virtual int mouseRelease(int type, int x, int y);
	virtual int mouseDrag(int type, int x, int y);
	virtual int mouseMove(int type, int x, int y);
	virtual int mouseDoublePress(int type, int x, int y);

private:
	//ViewObj * getFocusPage(QList<BaseContainer *> * pageList, int &x, int &y);

	int doPress(char type, int x, int y);

	ViewObj * getPagePartTxtLayer(char pressType, ViewObj * page, int &x, int &y);
	// 获取聚焦行，没有行输入，聚焦文字就在页这里进行判断
	ViewObj * getPointInsideRow(QList<BaseContainer *> * rowList, int &x, int &y);

	ViewObj * getPointInsideObj(OperateMgr * mgr, QList<BaseContainer *> * objList, int x, char & dir, char pressType);

	bool getCanEdit();
private:


private:
	ViewObj * mPagePanel;
	QList<BaseContainer *> * mPageList;

	int mFocusPartID;
};

#endif // _BASE_TXT_LAYER_MOUSE_H_
