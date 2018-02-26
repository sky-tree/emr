#ifndef TEXTWIDGET_H_
#define TEXTWIDGET_H_
#include "BaseTemplate.h"
class InputManager;
class StrContentData;
class InputFilter;
class TextWidget :public BaseTemplate
{
public:
	/*TextWidget(WriterManager * wMgr, BaseTemplete * templeteParent, ViewObj * viewParent, int objId, QString codeId);
	*/
	TextWidget(WriterManager * wMgr, int objId, QString codeId, bool isCreateStrTxtData = false);
	TextWidget(WriterManager * wMgr, int classId, int objId, QString codeId, bool isCreateStrTxtData = false);
	TextWidget(WriterManager * wMgr, BaseTemplate * templeteParent, int classid, int objId, QString codeId, bool isCreateStrTxtData = false);
	TextWidget(WriterManager * wMgr, TextWidget* otherTextWidget, bool canCopyChild);
	TextWidget(WriterManager * wMgr, TextWidget* otherTextWidget);
	virtual~TextWidget();
	void copyStrList(WriterManager * wMgr, QList<ViewObj *>* src, QList<ViewObj *>* dest);
	//void addAllChildToView(BaseWinData * winData, bool needResetFlag);
	void addAllChildToBufList(QList<BaseContainer *> * children, int start, int num, QList<BaseContainer *> * destViewList);
	int handleKeyPress(QString str, BaseElement * obj);
	int handleChoiceTextWidgetKeyPress(QString str, BaseElement * obj);
	int handleKeyRelease(QString str, BaseElement * obj);
	//void setFocusFlagAtObj(ViewObj* focosObj);
	void setTempleteStatus(int status);
	int getTempleteStatus();
	void initWidget(QString defaultString=" ");
	void generateDefaultObj();
	void generateStrObj(QString str,int index);
	void generateStrObjAddToViewParent(QString str, ViewObj* Row,int viewIndex,int templeteIndex);
	void addStr(QString str, int index);
	void addObjToChildShow(ViewObj* showObj,int temptIndex,ViewObj * viewParent,int viewIndex);
	QString filtInput(QString str, int index);
	void deleteStrObj(int startIndex,int endIndex,bool isDeleteInMemory = false);
	void clearDefaultObjListFromShow();

	void insertObjListToView(BaseParagraph* paragrapph, int viewIndex, QList<ViewObj *>* tempObjList, int start, int end);
	void clearStrObjList(bool isDeleteInMemory = false);
	void addDefaultStrObjShow( bool isRefreshNow=true);
	void addStrObjShow( bool isRefreshNow=true);
	void addStrObjToShow(QList<ViewObj *>* tempObjList,bool isRefreshNow=true);
	void ShowObjList(bool isRefreshNow=true);
	virtual void addWidgetToRow(bool isFresh = true);
	void setStr(QString str, bool isAddToChildrenList = true);
	void setStrOnShowTime(QString str);
	void setDefaultStrOnShowTime(QString str);
	void setDefaultStr(QString str,bool isAddToChildrenList = true);
	void setStrAndGenatateStrObj(QString str,bool isDeleteMemory=false);
	void setOnlyStr(QString str);
	ViewObj * getShowObjOnIndex(int index);
	//void addStrObjListToShowList();
	int getAddIndex(ViewObj *focusObj);
	int getChoiceAddIndex(ViewObj *focusObj,char& returnDir);
	void setInputFilter(InputFilter * inputFilter);
	int haveDecimalNumber();
	void setTxtData(StrContentData * focusTxtData);
	void setTxtDataWithValue(StrContentData * focusTxtData, bool needRefreshWH = true);
	void setTxtPointSize(int size);
	StrContentData * getSelfFocusData();
	int getTemptStatus();
	QString getString();
	QString getDefaultString();
	void addStrObjListToStringObjList(QList<ViewObj *> * strObjList);
	void addStrObjListToDefaultStringObjList(QList<ViewObj *> * strObjList);
	void setNumberFilter(int size,int decimalPointLenght);
	virtual void setTitle(QString title, bool isAddToChildrenList=true);
	virtual QString getTitle();
	virtual void checkStrIsAdaptStrListAndChildren(bool isToShow);
	QString getStrFromStrObjList();
	virtual bool setTempleteProperty(QMap<QString, QString>& setMap);
	virtual void setPropertyOnActionRedoOrUndo(QMap<QString, QString>& setMap, QList<ViewObj *> * strObjList, QList<ViewObj *> * defaultObjList);
	void copyObjListAndReplace(QList<ViewObj *> * strObjList, QList<ViewObj *> * defaultObjList);
	virtual bool setDrawTxtParam(StrContentData* focusData);
	void getWillShowObjList(QList<ViewObj *> * container);
	virtual void addSetTempletePropertyAction(QMap<QString, QString>& beforMap, QMap<QString, QString>& map, QList<ViewObj *> * beforDefaultObjList, QList<ViewObj *> * beforStrObjList, ViewObj* beforFocus, char focusDir);
	void addAllStrObjToChild();
	//检查mStr 的内容和mStrObjList里表达的内容是否相同
	//bool checkIsSameBetweenStrAndStrObjList();
	virtual	int mousePress(int type, int x, int y, BaseContainer  * obj);
	virtual	int mouseRelease(int type, int x, int y, BaseContainer  * obj);
	virtual	int mouseMove(int type, int x, int y, BaseContainer  * obj);
	virtual	int mouseDrag(int type, int x, int y, BaseContainer  * obj);
	virtual	int mouseDoublePress(int type, int x, int y, BaseContainer  * obj);
	virtual	int mouseScroll(int angle, BaseContainer  * obj);
	virtual	int keyPress(QString str, BaseContainer  * obj);
	virtual	int keyRelease(QString str, BaseContainer  * obj);
	QString getElementName();
	void SetElementName(QString name);
private:
	void deleteStr(int startIndex, int endIndex);
public:
	// 文字焦点数据
	StrContentData * mFocusTxtData;
	QString mStr;
	QString mDecimal;
	InputFilter * mInputFilter; //输入过滤器
	QString mDefaultString;// 用mDefaultString 空格字符填充
	//QList<ViewObj *> * mShowObjList;//显示列表
	QList<ViewObj *> * mDefaultObjList;//默认显示文字列表
	QList<ViewObj *> * mStrObjList;//默认显示文字列表
	bool mIsShowed;
	int mTemptStatus;
	//名称
	QString mNameStr;
};
#endif

