#ifndef _MENSTRUATIONTEMPLETE_H_
#define _MENSTRUSTIONTEMPLETE_H_
#include"BaseTemplate.h"
class ViewObj;
class TextWidget;
class ParagraphTemplete;
class DrawTxtParam;
class MenstruationTemplete :public BaseTemplate
{
public:
	MenstruationTemplete(WriterManager * wMgr, BaseTemplate * templeteParent, int objId, QString codeId, bool createFromLoad);
	MenstruationTemplete(WriterManager * wMgr, MenstruationTemplete* otherMenstruation);
	~MenstruationTemplete();
	virtual	int mousePress(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseRelease(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseMove(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseDrag(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseDoublePress(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseScroll(int angle, BaseContainer * obj);
	virtual	int keyPress(QString str, BaseContainer * obj);
	virtual	int keyRelease(QString str, BaseContainer * obj);
	void initTxtWidgetChooseId();
	//void addWidgetToRow(bool isFresh = true);
	void addAllChildToView( bool needResetFlag);
	void addAllChildToBufList(QList<BaseContainer *> * children, int start, int num, QList<BaseContainer *> * destViewList);
	void creatLine();
	void initWidget();
	void setValue(QString left,QString right,QString up,QString down);
	void setUpAndDownValue(QString up, QString down, bool setContainerFocus = true);
	void addTextObjToRowContainer(bool setContainerFocus = true);
	void setPostion(bool setContainerFocus);
	void refreshParentWH();
	int getWidth();
	void addObjListToObjView(QList<ViewObj*>* objList,ViewObj* container);
	void setFocusData(StrContentData* focusData);
	StrContentData*getSelfFocusData();
	void setLeftTextWidget(TextWidget* textWidget);
	void setRightTTextWidget(TextWidget* textWidget);
	void setUpTextWidget(TextWidget* textWidget);
	void setDownTextWidget(TextWidget* textWidget);
	TextWidget* getLeftTextWidget();
	TextWidget* getRightTTextWidget();
	TextWidget* getUpTextWidget();
	TextWidget* getDownTextWidget();
	void HandeRightMousePress();
	bool refreshTxtParam(StrContentData* focusData);
	void setChildTxtParam(StrContentData* focusData);
	void setListObjTxtParam(QList<ViewObj*>* objList, StrContentData * focusTxtData);
	BaseParagraph* getParagraphTemplete();
	bool setTempleteProperty(QMap<QString, QString>& setMap);
	void setPropertyOnActionRedoOrUndo(QList<ViewObj *>* topObjList, QList<ViewObj *> *downObjList);
	void addSetTempletePropertyAction(QMap<QString, QString> setBeforMap, QMap<QString, QString> setAfterMap, QList<ViewObj *>& topBeforObjList, QList<ViewObj *> &downBeforObjList, ViewObj* beforFocus, char focusDir);
	bool setDrawTxtParam(StrContentData* focusData);
	void getWillShowObjList(QList<ViewObj *> * container);
	bool checkHorizontalWidthThanRow(QList<ViewObj *> * container,int rowWidth);
private:
	TextWidget* mLeftTextWidget;//左
	TextWidget* mRightTextWidget;
	TextWidget* mUpTextWidget;
	TextWidget* mDownTextWidget;
	ViewObj * mContainerObj; //自动排列行
	BaseParagraph * mParagraph;
	ViewObj * mLeftRow;
	ViewObj * mRightRow;
	ViewObj * mUpRow;
	ViewObj * mDownRow;
	ViewObj * mMiddleLine;
	// 文字焦点数据
	StrContentData * mFocusTxtData;
	bool mIsShowed;
public:
	static const float mTxtRio;
};
#endif