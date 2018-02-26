#ifndef OBJ_MOUSE_MANAGER_H_
#define OBJ_MOUSE_MANAGER_H_



#include <qDebug>

#include "BaseContainerParam.h"

class ViewObj;
class BaseViewParam;
class BaseMouse;


class ObjMouseManager : public BaseContainerParam
{
public:
	ObjMouseManager(ViewObj * view);
	ObjMouseManager(ViewObj * view, int classID);
	ObjMouseManager(ViewObj * view, ObjMouseManager * source);
	~ObjMouseManager();
public:
	virtual int setMiddleButtonParam(int y, int pageH, int totalH);
	virtual	int mousePress(int type, int x, int y);
	virtual	int mouseRelease(int type, int x, int y);
	virtual	int mouseMove(int type, int x, int y);
	virtual	int mouseDrag(int type, int x, int y);
	virtual	int mouseDoublePress(int type, int x, int y);
	virtual	int mouseScroll(int angle);

	void setMouse(BaseMouse * mouse);
	BaseMouse * getMouse();
	void setFocus(ViewObj * focus);
	ViewObj* getFocus();

private:
	ViewObj * getFocusPage(ViewObj * pagePanel, int x, int y);
	void exchangeCrossPageExcelXYWhenFocusInput(BaseViewParam * focusViewParan, int & x, int & y);


private:
	QList<BaseContainer *> * mChildren;
protected:
	BaseMouse * mMouse;
	ViewObj * mInputFocus;
};

#endif
