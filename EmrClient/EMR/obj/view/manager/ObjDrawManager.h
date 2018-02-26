#ifndef OBJ_DRAW_MANAGER_H_
#define OBJ_DRAW_MANAGER_H_



#include <QPainter>


#include "BaseContainerParam.h"

class ViewObj;
class BaseDraw;

class ObjDrawManager : public BaseContainerParam
{
public:
	ObjDrawManager(ViewObj * owner);
	ObjDrawManager(ViewObj * owner, int classID);
	ObjDrawManager(ViewObj * owner, ObjDrawManager * source);
	~ObjDrawManager();

public:

	void initDraws(BaseDraw * bottom, BaseDraw * top);
	void initDraws(BaseDraw * bottom, BaseDraw * top, int bottomX, int bottomY, int topX, int topY);
	void initDrawsParams(int topW, int topH, int bottomW, int bottomH);
	void setBottomDraw(BaseDraw * bottom);
	BaseDraw *getBottomDraw();


	virtual int draw(QPainter &pPainter, int x, int y, bool isPrint);
private:
	void initDefaultParams();
protected:
	ViewObj * mViewOwner;
	BaseDraw * mDrawBottom;
	BaseDraw * mDrawTop;
	int mDrawTopDisX;
	int mDrawTopDisY;
	int mDrawTopW;
	int mDrawTopH;
	int mDrawBottomDisX;
	int mDrawBottomDisY;
	int mDrawBottomW;
	int mDrawBottomH;
};

#endif
