#ifndef BASE_DRAW_H_
#define BASE_DRAW_H_


#include<QPainter>
#include<QColor>


#include "BaseContainerParam.h"

class ViewObj;
class RowViewParam;
class BaseContainerParam;


// 绘制基类
class BaseDraw : public BaseContainerParam
{
public:
	BaseDraw(ViewObj * owner, int classID);
	BaseDraw(ViewObj * owner, BaseDraw *otherDraw);
	virtual ~BaseDraw();

public:

	RowViewParam * getRowParam();
	void setViewOwner(ViewObj * owner);

public:

	virtual bool draw(QPainter &pPainter, int x, int y, int w, int h);

protected:
	ViewObj * mViewOwner;

};

#endif
