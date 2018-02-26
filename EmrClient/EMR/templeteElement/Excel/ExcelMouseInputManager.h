#ifndef _EXCELMOUSEINPUTMANAGER_H_
#define _EXCELMAOUSEINPUTMANAGER_H_
#include "BaseTempleteMouse.h"
#include "ExcelLayout.h"
#include "SelectView.h"
class ExcelMouseInputManager :public BaseTempleteMouse
{
public:
	ExcelMouseInputManager(ExcelTemplete* templeteOwner);
	~ExcelMouseInputManager();
	void init();
public:
	virtual int mousePress(int type, int x, int y, BaseElement * obj);
	virtual int mouseRelease(int type, int x, int y, BaseElement * obj);
	virtual int mouseMove(int type, int x, int y, BaseElement * obj);
	virtual int mouseDrag(int type, int x, int y, BaseElement * obj);
	virtual int mouseDoublePress(int type, int x, int y, BaseElement * obj);
	virtual int mouseScroll(int scroll, BaseElement * obj);
	bool getIsPreeOnExcel();
	void setIsPressOnExcel(bool isPressOnExcel);
	void setPressPage(ViewObj * page);
	ViewObj * getPressPage();
private:
	bool isClickOnSelectCellLeftOrRight(ViewObj * viewObj,int y);
	bool findSelectCellLAndRCellData(ExcelCellData * contentCellData);
	void handleDragLine(int type, int x, int y);
	void handleDragOnSelectCellLOrR(int type, int x, int y);
	void handleReleaseDragLine(int type, int x, int y);
	void handleReleaseDragOnSelectCellLOrR(int type, int x, int y);
	void handleReleaseDragOnSelectCellLOrRNew(int type, int x, int y);
	void selectCellLOrRChangeLine(OperateMgr * operateMgr, Line* changeLine);
	void setDragLineShow(bool isShow);
	bool caculateMarquee(int type, int x, int y); // 超过一个单元格范围，返回值为真。其余的为假
	bool isSelectOnChooseCellLeftOrRight(QList<BaseContainer *> *  chooseList, ExcelCellData* resultCell,int y);
	bool isAbleToMooveCellLOrR(ExcelCellData * chooseCellData,Line * selectedLine);
	
public:
	enum InputType{Nomal,SelectLine,SelectOnCellLOrR};
	ExcelLayout * mOwnerViewLayout;
	ExcelTemplete * mOwnerExcelTemplete;
	static const char SLOT_CHANGE_DISTANCE = 3;
	static const char DRAG_CHANGE_MIN = 4;
	static const int SELECT_CELL_COLOR = 0x440000ff;
	SelectView* mSelectLineObj;
	InputType mInputType;
	bool isPressOnCellLeft; //辅助上一个变量
	bool isInOneCell;
	QPoint mMouseDown;
	int mSelectLineDragPosition;
	ExcelCellData * mSelectLineLeftCellData;//处理拖动单元格左右边时用
	ExcelCellData * mSelectLineRightLeftData; //处理拖动单元格左右边时用
	QList<ViewObj *> * mTemptViewObjList;
	ViewObj * mViewOwner; // 虚拟表格

private:
	bool canMousePressInTitleBottom();
private:
	bool mIsPressOnExcel;
	bool mIsPressMove;
	ViewObj* mPressPage;
};
#endif

