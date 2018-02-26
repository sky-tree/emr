#ifndef _EXCELTEMPLETE_H_
#define _EXCELTEMPLETE_H_
#include "BaseTemplate.h"
#include "qrect.h"
class ExcelLayout;
class ExcelCellData;
class ExcelMouseInputManager;
class ExcelTemplete :
	public BaseTemplate
{
public:
	ExcelTemplete(WriterManager * wMgr, BaseTemplate * templeteParent, int objId, QString codeId);
	ExcelTemplete(WriterManager * wMgr, ExcelTemplete* otherExcelTemplete);
	~ExcelTemplete();
	void initExcelTemplete(int row,int col,int w,int h,ViewObj *initRow,bool isShowBorder);
	void initExcelTemplete(bool isShowBorder,bool isFromCreate);
	void setInitOver();
	void requestLayoutByCell(ViewObj * currentRow,int index);
	void addContainerToPage(bool isNeedAddToShow);
	void requestLayout(bool needResetFlag = true, bool isFreshFromInnerChange=false);
	void layoutAgent(ViewObj *row, int index, ViewObj *preFocus, char direction, ViewObj *&preFocus2, char& direction2, bool isFreshFromInnerChange);
	void addWidgetToChildMgr();
	virtual	int mousePress(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseRelease(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseMove(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseDrag(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseDoublePress(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseScroll(int angle, BaseContainer * obj);
	virtual	int keyPress(QString str, BaseContainer * obj);
	virtual	int keyRelease(QString str, BaseContainer * obj);
	ViewObj* getPageLayer();
	ViewObj * getWindowObj();
	ViewObj * getRowObj(int index);
	void refreshRowObj();
	bool getIsShowBorader();
	ViewObj * getStartRow();
	//void addAllChildToView(BaseWinData * winData, bool needResetFlag);
	//void addAllChildToBufList(QList<BaseContainer *> * viewList);
	//void addAllChildToView(BaseWinData * winData, bool needResetFlag);
	//void addAllChildToBufList(QList<BaseContainer *> * children, int start, int num, QList<BaseContainer *> * destViewList);
	//void getRowAndIndexAtuo(ViewObj*&row, int& viewIndex);
	void showTidy();
	void addExcelDragLineToContainer(ViewObj* line);
	void removeExcelDragLineFromContainer(ViewObj* line);
	void resetExcelRect();
	void resizeRect(int l,int t,int r,int b);
	static bool isMyCell(QList<BaseContainer *> *  & chooseListT, ExcelTemplete * excelTemp);
	static bool isMyCell(QList<ExcelCellData *>* & chooseListT, ExcelTemplete * excelTemp);
    void getWillShowObjList(QList<ViewObj *> * container);
	int getCellSize();
	QList<ExcelCellData *>* getCellList();
	ExcelLayout *getExcelLayout();
	void sortLine();
	ViewObj* getVirtulExcel();
	void setFocusOnSelf();
	ViewObj * mVirtulExcel;
	ExcelLayout * mExcelLayout;

	QList<ViewObj *>* mLayoutRowObjList;//包装的行对象列表
	//ExcelMouseInputManager * mInput
	ViewObj* mTemptViewFocus;
	bool getIsPressOnExcel();
	void setIsPressOnExcel(bool isPressOnExcel);
	void setMouse(BaseTempleteMouse * mouse);
	bool setDrawTxtParam(StrContentData* focusData);
	static bool setExcelTemplateVerticalType(QList<int> positionList, QList<ExcelCellData *>* changeCells);
	int getVerticalLayoutType();
	void getCellPageRootTemplate(QList<BaseContainer*>& cellRootContainerList);

	// 根据传过来的当前表格有效高度，返回<=curH的整行的有效高度
	int getWholeRowCrossPageH(int curH);
	ViewObj * getPressPage();
	void setPressPage(ViewObj * page);
	void setWriteManager(WriterManager* writeManger, bool isCircle = true);
private:
	bool isInit;
	bool mIsOnLayout;
	ExcelMouseInputManager* mExcelInputMouseManager;
	ViewObj * mStartRow;
public :
	bool mIsShowBorder;

	int mLayoutRowObjIndex;
	int mThroughRowIndex;
	QRect mExcelRect;
	bool mCanDragExcel;//是否可以拖动表格
	bool mCanOperateExcelCell;//是否可以合并，拆分等菜单操作
};
#endif
