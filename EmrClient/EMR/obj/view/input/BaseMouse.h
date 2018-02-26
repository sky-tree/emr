#ifndef BASE_MOUSE_H_
#define BASE_MOUSE_H_

#include "BaseContainerParam.h"

class BaseContainer;
class ViewObj;

class BaseMouse : public BaseContainerParam
{
public:
	BaseMouse(ViewObj * owner, int classID);
	~BaseMouse();
	virtual	int mousePress(int type, int x, int y){ return 0; };
	virtual	int mouseRelease(int type, int x, int y){ return 0; };
	virtual	int mouseMove(int type, int x, int y){ return 0; };
	virtual	int mouseDrag(int type, int x, int y){ return 0; };
	virtual	int mouseDoublePress(int type, int x, int y){ return 0; };
	virtual	int mouseScroll(int angle){ return 0; };
	virtual int setMiddleButtonParam(int y, int pageH, int totalH) { return 0; };

	virtual void releaseFocusFromWindow();
	virtual void setScale(float scale) { return; };

protected:
	ViewObj * mViewOwner;
};
#endif