#ifndef TEMPLETE_VIEW_OBJ_MOUSE_MANAGER_H_
#define TEMPLETE_VIEW_OBJ_MOUSE_MANAGER_H_
#include "ObjMouseManager.h"
class TempleteViewObjMouseManager :
	public ObjMouseManager
{
public:
	TempleteViewObjMouseManager(ViewObj * view);
	~TempleteViewObjMouseManager();
public:
	virtual	int mousePress(int type, int x, int y);
	virtual	int mouseRelease(int type, int x, int y);
	virtual	int mouseMove(int type, int x, int y);
	virtual	int mouseDrag(int type, int x, int y);
	virtual	int mouseDoublePress(int type, int x, int y);
	virtual	int mouseScroll(int angle);
	//void setMouse(BaseMouse * mouse);
private:
	//BaseMouse* mMouse;
};
#endif
