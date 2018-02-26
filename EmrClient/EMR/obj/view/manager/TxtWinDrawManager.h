#ifndef TXT_WIN_DRAW_MANAGER_H_
#define TXT_WIN_DRAW_MANAGER_H_



#include <QPainter>


#include "ObjDrawManager.h"

class ViewObj;
class BaseDraw;
class BaseViewParam;

class TxtWinDrawManager : public ObjDrawManager
{
public:
	TxtWinDrawManager(ViewObj * owner);
	~TxtWinDrawManager();

public:

	int draw(QPainter &pPainter, int x, int y, bool needJudgeInsideWindow);

private:
	void drawFlag(QPainter &pPainter, int x, int y, bool needJudgeInsideWindow);

private:
	BaseViewParam * mWinViewParam;
	ViewObj * mFlag;
};

#endif
