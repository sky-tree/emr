#ifndef _CHOICEWIDGETTEMPLETE_H_
#define _CHOICEWIDGETTEMPLETE_H_
#include "BaseTemplate.h"
//qt版本高于等于qt5，VS版本高于或等于VS2010
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class InputManager;
class BaseKeyboard;
class BaseMouse;
class DrawTxtParam;
class TextWidget;
class ChoiceWidgetTemplete :
	public BaseTemplate
{
public:
	//ChoiceWidgetTemplete(WriterManager * wMgr, BaseTemplate * templeteParent, int objId, QString codeId);
	ChoiceWidgetTemplete(WriterManager * wMgr, char widgetType, BaseTemplate * templeteParent, int objId, QString codeId,bool isFromLoad = false);
	ChoiceWidgetTemplete(WriterManager * wMgr, ChoiceWidgetTemplete* otherChoiceWidget);
	~ChoiceWidgetTemplete();
	//void addAllChildToView(BaseWinData * winData, bool needResetFlag);
	void addAllChildToBufList(QList<BaseContainer *> * children, int start, int num, QList<BaseContainer *> * destViewList);
	bool isSelectedOnItem(int itemIndex);
	void setTitleAndOptions(TextWidget* title, QList<TextWidget*>*  options);
	void setTitleAndOptions(QString title, QList<QString> strList);
	void setSelectItem(QList<int> chooseList);
	void setTitle(TextWidget* title);
	void setTitleOnNotShow(TextWidget* title);
	void setSignBeforSelectNotShow(TextWidget* title);
	void setSignBeforNotSelectNotShow(TextWidget* title);
	void setTitle(QString str);
	void setTitleHintName(QString str);
	BaseTemplate* getTiltle();
	BaseTemplate* getSignBeforSelect();
	BaseTemplate* getSignBeforNotSelect();
	QList<TextWidget*>* getOptionList();
	void setOptions(QList<TextWidget*>* optionTempleteList);
	void setOptionsOnNotShow(QList<TextWidget*>* optionTempleteList);
	void setOptions(QList<QString> strList);
	bool isSameOptions(QList<QString> strList);
	void setWedgetType(char type);
	char getWedgetType();
	void setTempleteStatus(int status);
	int getTempleteStatus();
	//void setIsShowTitle(bool titileShowable);
	//bool getIsShowTitle();
	//void addWidgetToRow(bool isfresh = false);
	void generateViewObjOnNewStatus();
	void addOptionsToChileView();
	void addOptionsToChildrenAndShowList(QList<BaseContainer *> * children, QList<BaseContainer *> * destViewList);
	void addAllContentAndShowList(QList<BaseContainer *> * children, QList<BaseContainer *> * destViewList);
	void addViewObjToShowListOnNomalStatus();
	void generateTitleTempleteForType();
	void initWidget();
	void addAllChildToChildrenMgr();
	void generatSignBeforeOptionViewObj();
	void generatSignBehindOptionViewObj();
	void generatSignHaveOrNotViewObj(bool isFormLoad);
	ViewObj* getSeparatorObj(int index);
	//void setOptionsDisVisable();
	//void deleteOptions();
	//void clearSeparateObj();
	
	void setFocusColor();
	void setNormalColor();
	bool setTitlePosition(char postion);
	char getTitlePosition();
	virtual	int mousePress(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseRelease(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseMove(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseDrag(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseDoublePress(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseScroll(int angle, BaseContainer * obj);
	virtual	int keyPress(QString str, BaseContainer * obj);
	virtual	int keyRelease(QString str, BaseContainer * obj);
	bool deleteObjOnBackpressKey(ViewObj * focusObj,char dir);
	bool deleteObjOnDeleteKey(ViewObj * focusObj, char dir);
	void setFocusData(StrContentData* focusData);
	StrContentData*getFocusData();
	char getWidgetType();
	void setWidgetType(int type);
	void setTitlePosition(int type);
	const QList<int> getSelectList();
	void setSignBefor(ViewObj * signObj);
	void setSignBehind(ViewObj * signObj);
	void setCreateFromLoad();
	void deleteContentChild();
	void addContentChild(bool isAddToShow = true);
	void deleteLoadChild();
	QString getOptions();
	void setIsShowed(bool isShowed);
	virtual void showTidy();
	void deleteNextViewObj(int currentTempleteIndex);
	void deleteLastViewObj(int currentTempleteIndex);
	ViewObj* findNextViewObj(int currentTempleteIndex);
	ViewObj* findPreViewObj(int currentTempleteIndex);
	int  showSelectDialog();
	bool setTempleteProperty(QMap<QString, QString>& setMap);
	void addSetTempletePropertyAction(QMap<QString, QString>& beforMap, QMap<QString, QString>& map, QList<int> beforChooseList
		, QList<ViewObj *> & beforDefaultTitleList, QList<ViewObj *>& beforTitleStrList, QList<TextWidget*>* beforeOptionTempleteList,
		QList<ViewObj *> & beforHaveList, QList<ViewObj *> & beforNotHaveList, ViewObj* beforFocus, char focusDir);
	void setPropertyOnActionRedoOrUndo(QMap<QString, QString>& setMap, QList<ViewObj *> * strObjList, QList<ViewObj *> * defaultObjList,
		QList<ViewObj *> * strHaveList, QList<ViewObj *> * strNotHaveList,
		QList<TextWidget*>* optionTempleteList, QList<int> chooseList);
	void setCanEdit(QMap<QString, QString>& setMap);
	bool setDrawTxtParam(StrContentData* focusData);
	void setBackTxtParam(StrContentData* focusData);
	StrContentData*getSelfFocusData();
	void getWillShowObjList(QList<ViewObj *> * container);
	void checkCanShowTitle(bool isSetFocusFront,bool isAddToShow = true);
	bool getCanInputOnKeyBoard();
	void setCanInputOnKeyBoard(bool canInputOnKeyBoard);
	void setIsCheckTitleShow(bool isCheckTitle);
protected:
	TextWidget* mWidgetTitle ;
	//QList<ViewObj *> * mTitleObjList;
	QList<TextWidget*>* mOptionTempleteList;
	//QList<QList<ViewObj *> *> * mOptionObjList;
	QString mWidgetSignBeforeOptionItem="{";
	ViewObj * mSignBeforeOptionObj;
	QString mWidgetSignBehindOptionItem = "}";
	ViewObj * mSignBehindOptionObj;
	QString mListSeparator="、";
	QList<ViewObj *> * mSeparatorObjList;//选项分割号列表
	QList<int>  mChooseList;    //选中列表
	char mWidgetType; //0 表示单选，1 表示多选
	int mTemptStatus;
	bool mIsShowed;//是否展开过。
	//bool isShowTitle;//是否显示标题
	char mTitlePosition;//标题位置
	// 文字焦点数据
	StrContentData * mFocusTxtData;
	//是否是从文档加载出来的。如果是，需要在选择控件操作中把除了前后选项（mSignBeforeOptionObj、mSignBehindOptionObj）标示符以外的所有对象栓除掉
	bool isCreateFromLoad;
	//有无选择专用
	TextWidget * mSignBeforSelect;
	TextWidget * mSignBeforNotSelect;
	ViewObj * mSignCommaObj;
	//整洁显示时记录前后两个标示符位置

	//记录属性设置时的动作
	bool isSetCodeSuccess;
	bool isSetTitlepostionSuccess;
	bool isSetTitleSuccess;
	bool isSetSignNotBeforSuccess;
	bool isSetSignBeforSuccess;
	bool isSameOptionSuccess;
	//是否允许键盘输入
	bool mIsCanInputOnKeyboard;

	//是否需要检验空时，显示标题
	bool mIsCheckTitleShow;
public:
	static const int COLOR_BEHIND_TITLE_SIGN = 0xffff0000;
	static const int COLOR_OPTION_SIGN = 0xff222222;
	static const int COLOR_FOCUS = 0xff0000ff;
	static const int COLOR_NORMAL = 0xff000000;
	static const char RADIO = 0;//单选
	static const char HAVE_CHOICE = 1; //有无选择
	static const char MULTIPLE_CHOICE = 2;//多选
	
	static const char LEFT = -1;
	static const char RIGHT = 1;
	static const char DISVISABLE = 0;
};
#endif

