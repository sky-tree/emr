#ifndef BASE_TEMPLATE_H_
#define BASE_TEMPLATE_H_



#include <QPainter>

#include <qmap.h>
#include "BaseContainer.h"
class BaseTempleteKeyboard;
class BaseTempleteMouse;
class ExcelCellData;
class BaseTemplate : public BaseContainer
{
public:
	BaseTemplate(WriterManager * writerMgr, int objID);
	BaseTemplate(WriterManager * writerMgr, int classId, int objId, QString codeId);
	BaseTemplate(WriterManager * writerMgr, BaseTemplate*  otherTextWidget, bool canCopyChild);
	virtual~BaseTemplate();

public:
	void refreshChildrenTempIndex(int start);
	bool canWrite();
public:
	virtual void addAllChildToBufList(QList<BaseContainer *> * children, int start, int num, QList<BaseContainer *> * destViewList);
	bool isNullVector(QList<BaseContainer *> * children);
	void setKeyboard(BaseTempleteKeyboard * keyboard);
	virtual void setMouse(BaseTempleteMouse * mouse);
	virtual	int mousePress(int type, int x, int y, BaseContainer  * obj);
	virtual	int mouseRelease(int type, int x, int y, BaseContainer  * obj);
	virtual	int mouseMove(int type, int x, int y, BaseContainer  * obj);
	virtual	int mouseDrag(int type, int x, int y, BaseContainer  * obj);
	virtual	int mouseDoublePress(int type, int x, int y, BaseContainer  * obj);
	virtual	int mouseScroll(int angle, BaseContainer  * obj);
	virtual	int keyPress(QString str, BaseContainer  * obj);
	virtual	int keyRelease(QString str, BaseContainer  * obj);

	//void setCanInsert(bool isCanInsert){ mIsCanInsert = isCanInsert; }
	//bool getCanInsert(){ return mIsCanInsert; }
	virtual bool setTempleteProperty(QMap<QString, QString>& setBeforMap){ return false; };
	virtual void addWidgetToRow(bool isFresh = true){};
	//void addViewObjToChildShow(BaseContainer* showObj, int tempIdx, ViewObj * viewParent, int viewIndex);
	void addTempleteToView();
	void setPreObjFocus(ViewObj* focusObj, bool isRight);
	bool isFocousObj(BaseElement* currentObj);
	virtual void setTempleteStatus(int status){}
	virtual int getTempleteStatus(){ return 0; }
	virtual void setTitle(QString title){}
	virtual QString getTitle(){ return NULL; }
	virtual void checkStrIsAdaptStrListAndChildren(bool isToShow){};
	void setViewObjColor(ViewObj * obj, int color);
	void deleteSelf(bool isCreateAction=true);
	
	void refreshTxtParam();
	virtual	StrContentData*getSelfFocusData(){ return NULL; };
	void setPageRepaint(ViewObj* currenView);
	bool addCodeIdToMap(QMap<QString, QString>& map);
	bool setCodeIdToTemplete(QMap<QString, QString>& map);
	void addDeleteSelfAction(bool isCreateAction = true);
	void replaceNeedReplaceList(int addActionIndex = 1);
	virtual bool setDrawTxtParam(StrContentData* focusData);
	bool isSetFlowType(StrContentData* focusData);
	int getMaxWidthOfChildView();

	//static bool isChoosedThis(BaseWinData* windata, BaseTemplete* templete);
	void getViewObjsAtBehind(int indexStart, QList<ViewObj *> * containerList);
	virtual  void getWillShowObjList(QList<ViewObj *> * container);
	void addTxtLayerAndReplaceNeedReplaceList(ViewObj* tempView, bool isExcel = false);
	template<class T>void copyObjListToObjList(QList<T *> * srcList, QList<T *> * destList, bool isClearDestList);
	void copyObjListToObjList(QList<ViewObj *> * srcList, QList<BaseContainer *> * destList, bool isClearDestList);
	void deleteChild(int start, int num, bool refreshIdx, bool isRefreshNow = true);
	void deleteChild(BaseContainer * child, bool refreshIdx, bool isRefreshNow = true);
	void deleteViewObjOnShow(QList<BaseContainer *> * children, int start, int num, bool refreshIdx,bool isRefreshNow = true);
	void addChildren(QList<BaseContainer *> * addList, int index, bool refreshIdx, bool isRefreshNow = true);
	void addChild(BaseContainer *child, int index, bool refreshIdx, bool isRefreshNow = true);
	void addViewObjOnShow(QList<BaseContainer *> * addList, bool refreshIdx, bool isRefreshNow = true);
	void setExcelCellData(ExcelCellData * cellData);
	ExcelCellData* getExcelCellData();
	BaseTemplate* getTemplateParentInExcelCell();
	void setFirstChildViewFocus(bool isFlagLeft = true, bool isFreshStrData = true);
	QString getCodeId();
	bool isObjHaveParagraphMgr(BaseChildrenManager* childManger);
    virtual	BaseParagraph * getParagraphParent();
protected:
	// 输入管理器
	BaseTempleteMouse * mInputMouse;
	BaseTempleteKeyboard* mInputKeyboard;

	QString mCodeId;
	//专门为表格单元格添加，持有ExcelCellData 对象
	ExcelCellData* mExcelCellData;
public:

private:
};

#endif
