#ifndef _UNFOLDCHOICEWIDGETTEMPLETE_H_
#define _UNFOLDCHOICEWIDGETTEMPLETE_H_
#include "BaseTemplate.h"
//qt版本高于等于qt5，VS版本高于或等于VS2010
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class InputManager;
class BaseKeyboard;
class BaseMouse;
class StrContentData;
class ChoiceItemTextWidget;
class TextWidget;
class WriterManager;
class UnfoldChoiceWidgetTemplete :
	public BaseTemplate
{
public:
	UnfoldChoiceWidgetTemplete(WriterManager * wMgr, char widgetType, BaseTemplate * templeteParent, int objId, QString codeId, bool isFromLoad = false);
	UnfoldChoiceWidgetTemplete(WriterManager * wMgr, UnfoldChoiceWidgetTemplete* otherChoiceWidget);
	~UnfoldChoiceWidgetTemplete();

	void setTitleHintName(QString str);
	void setOptions(QList<ChoiceItemTextWidget*>* optionTempleteList);
	void setOptionsOnNotShow(QList<ChoiceItemTextWidget*>* optionTempleteList);
	QList<ChoiceItemTextWidget*>* createOptionsByStrList(QList<QString>& strList, QList<int>& paddingLeft);
	void setOptions(QList<QString>& strList, QList<int>& paddingLeft);
	void setOptionsOnCreate(QList<QString> &strList, QList<int>& paddingLeft);
	StrContentData* getSelfFocusData();
	void setTxtData(StrContentData * focusTxtData);
	bool setDrawTxtParam(StrContentData* focusData);
	int mousePress(int type, int x, int y, BaseContainer * obj);
	int keyPress(QString str, BaseContainer * obj);
	int keyRelease(QString str, BaseContainer * obj);
	void clicked(BaseTemplate* item,bool ischecked);
	QString getOptions();
	QList<ChoiceItemTextWidget*>* getOptionList();
	bool setTempleteProperty(QMap<QString, QString>& setMap);
	void addSetTempletePropertyAction(QMap<QString, QString>& beforMap, QMap<QString, QString>& map, QList<int> beforChooseList
		, QList<ViewObj *> & beforDefaultTitleList, QList<ViewObj *>& beforTitleStrList, QList<ChoiceItemTextWidget*>* beforeOptionTempleteList,
		 ViewObj* beforFocus, char focusDir);
	void setPropertyOnActionRedoOrUndo(QMap<QString, QString>& setMap, QList<ViewObj *> * strObjList, QList<ViewObj *> * defaultObjList,
		QList<ChoiceItemTextWidget*>* optionTempleteList, QList<int> chooseList);
	void setFocusData(StrContentData* focusData);
	StrContentData*getFocusData();
	char getChoiceType();
	//void setChoiceType(char type);
	bool setTitlePosition(char postion);
	char getTitlePosition();
	QList<int> getSelectList();
	void setSelectList(QList<int> select);
	void createTitle(QString& title);
	QString getTitleStr();
	void addContentChild(bool isAddToShow = true);
	void initWedgitOnLoad(BaseContentData * data, char titlePositon, QString & titlestr, QList<QString>& strList, QList<int>& paddingLeft,int wedgitPaddingLeft);
	int getPaddingLeft();
private:
	void initWidget();
	void generateTitleTempleteForType();
	void addAllChildToChildrenMgr();
	
	void addOptionsToChildrenManager();
	bool isSelectedOnItem(int itemIndex);
	void checkRadioChoiceCheckedList();
	void checkNeedToAddTitleWidget();
	void deleteContentChild();

	
	bool isSameOptions(QList<QString> strList);
	void setOptionPaddingLeft(int paddingLeft,bool isFreshRow);
	int getCurrentnPaddingLeftOptions();
protected:
	QString mColonStr = ":";
	ViewObj* mColonViewObj;
	char mChoiceType;
	TextWidget* mWidgetTitle;
	QList<ChoiceItemTextWidget*>* mOptionTempleteList;
	// 文字焦点数据
	StrContentData * mFocusTxtData;
	char mTitlePosition;//标题位置
	QList<int>  mChooseList;    //选中列表
	// 选项左边距宽度
	int mPaddingLeft;
	//记录属性设置时的动作
	bool isSetCodeSuccess;
	bool isSetTitlepostionSuccess;
	bool isSetTitleSuccess;
	bool isSameOptionSuccess;
	bool isSamePaddingLeft;
public:
	static const char RADIO = 0;//单选
	static const char HAVE_CHOICE = 1; //有无选择
	static const char MULTIPLE_CHOICE = 2;//多选
	
	static const char LEFT = -1;
	static const char RIGHT = 1;
	static const char DISVISABLE = 0;
};
#endif

