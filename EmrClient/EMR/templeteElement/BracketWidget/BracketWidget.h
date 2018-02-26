#ifndef _BRACKETWIDGET_H_
#define _BRACKETWIDGET_H_
#include "TextWidget.h"
//qt版本高于等于qt5，VS版本高于或等于VS2010
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class BracketWidget :
	public TextWidget
{
public:
	BracketWidget(WriterManager * wMgr, BaseTemplate * templeteParent, int objId);
	BracketWidget(WriterManager * wMgr, BaseTemplate * templeteParent, int classid, int objId, QString codeId, bool needCreateTxtData = false);
	BracketWidget(WriterManager * wMgr, BracketWidget * otherBracket);
	BracketWidget(WriterManager * wMgr, BracketWidget * otherBracket, bool canCopyChild);
    virtual	bool setTempleteProperty(QMap<QString, QString>& setMap);
	virtual void setPropertyOnActionRedoOrUndo(QMap<QString, QString>& setMap, QList<ViewObj *> * strObjList, QList<ViewObj *> * defaultObjList);
	virtual ~BracketWidget();
//	void setTempleteStatus(int status);
//	int getTempleteStatus();
	void setTitle(QString title,bool isAddToChildRENlIST = true);
	void setTitleOnShow(QString title);
	QString getTitle();
	void setHint(QString title,bool isAddToChildrenList= true);
	QString getHint();
	void showTidy();
	/*void setContent(QString title);
	QString getContent();*/
	//整洁显示时记录前后两个标示符位置
	ViewObj* mParentViewOfSignBeforView;
	int indexOfSignBeforViewInParentView;
	ViewObj* mParentViewOfSignBehindView;
	int indexOfSignBehindViewInParentView;
public:
	QString mWidgetSignBeforeOptionItem = "{";
	//ViewObj * mSignBeforeOptionObj;
	QString mWidgetSignBehindOptionItem = "}";
	//ViewObj * mSignBehindOptionObj;
	//friend	BracketWidgetKeyboard;
	BaseTemplate* mContentTemplete;
//	int mTemptStatus;
	
};
#endif
