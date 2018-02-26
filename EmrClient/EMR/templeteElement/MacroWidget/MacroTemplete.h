#ifndef _MACROTEMPLETE_H_
#define _MACROTEMPLETE_H_
#include "BaseTemplate.h"
//qt版本高于等于qt5，VS版本高于或等于VS2010
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class BracketWidget;
class TextWidget;
class MacroTemplete :
	public BaseTemplate
{
public:
	MacroTemplete(WriterManager * wMgr, BaseTemplate * templeteParent, int objId, QString codeId,bool isCreateOnLoad = false);
	MacroTemplete(WriterManager * wMgr, MacroTemplete * otherMacroTemplete);
	~MacroTemplete();
	void initWidget();
	void setTitle(QString str);
	QString getTitle();
	void setContent(QString str, bool needDeleteAndAddShow = false, bool isFreshStrData=true);
	QString getContent();
	void setTitle(TextWidget* title);
	void setContent(BracketWidget*content);
	void setId(int id);
	int getId();
	virtual	int mousePress(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseRelease(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseMove(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseDrag(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseDoublePress(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseScroll(int angle, BaseContainer * obj);
	virtual	int keyPress(QString str, BaseContainer * obj);
	virtual	int keyRelease(QString str, BaseContainer * obj);
	virtual void addWidgetToRow(bool isFresh = true);
	//void addAllChildToBufList(QList<BaseContainer *> * children, int start, int num, QList<BaseContainer *> * destViewList);
	bool setDrawTxtParam(StrContentData* focusData);
	StrContentData*getSelfFocusData();
	//void addAllChildToView(bool needResetFlag);
	//void addAllChildToView(BaseWinData * winData, bool needResetFlag);
	bool setTempleteProperty(QMap<QString, QString>& setMap);
	void addSetTempletePropertyAction(QMap<QString, QString>& beforMap, QMap<QString, QString>& map, QList<ViewObj *> * beforContentStrObjList, QList<ViewObj *> * beforDefaultContentObjList,  QList<ViewObj *> * beforTitleStrObjList, QList<ViewObj *> * beforDefaultTitleObjList, ViewObj* beforFocus, char focusDir);
	void setPropertyOnActionRedoOrUndo(QMap<QString, QString>& setMap, QList<ViewObj *> * contantStrObjList, QList<ViewObj *> * contantDefaultObjList, QList<ViewObj *> * titleStrObjList, QList<ViewObj *> * titleDefaultObjList);
	void getWillShowObjList(QList<ViewObj *> * container);
	void haveDeleteFromParent(BaseContainer * parent);
	void haveAddToParent(BaseContainer * parent);
private:
	TextWidget* mWidgetTitle;
	BracketWidget* mContent;
	int mId;
};
#endif
