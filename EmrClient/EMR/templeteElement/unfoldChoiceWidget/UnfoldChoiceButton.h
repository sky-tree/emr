#ifndef _UNFOLDCHOICEBUTTON_H_
#define _UNFOLDCHOICEBUTTON_H_
#include "BaseTemplate.h"
class ViewObj;
class StrContentData;
class UnfoldChoiceWidgetTemplete;
class UnfoldChoiceButton :
	public BaseTemplate
{
public:
	//ChoiceWidgetTemplete(WriterManager * wMgr, BaseTemplate * templeteParent, int objId, QString codeId);
	UnfoldChoiceButton(WriterManager * wMgr, BaseTemplate * templeteParent,char type, int objId, QString codeId,int leftPadding);
	UnfoldChoiceButton(WriterManager * wMgr, UnfoldChoiceButton* otherChoiceWidget);
	~UnfoldChoiceButton();
	void setChecked(bool checked);
	bool getChecked();
	bool setDrawTxtParam(StrContentData* focusData);

	int mousePress(int type, int x, int y, BaseContainer * obj);
	int mouseRelease(int type, int x, int y, BaseContainer * obj);
	int mouseMove(int type, int x, int y, BaseContainer * obj);
	int mouseDrag(int type, int x, int y, BaseContainer * obj);
	int mouseDoublePress(int type, int x, int y, BaseContainer * obj);
	int mouseScroll(int angle, BaseContainer * obj);
	int keyPress(QString str, BaseContainer * obj);
	int keyRelease(QString str, BaseContainer * obj);
	void setPaddingLeft(int leftPadding);
	int getPaddingLeft();
private:
	void initCreateImageView(WriterManager * writeMgr, char type);
	void showMarkView();
	void invisibleMarkView();
	void refreshMarkPosition();
private:
	ViewObj * mContainerObj; //最外层
	ViewObj* mMarkOfSelected;
	ViewObj* mRectView;
	char mChoiceType;
	bool mIsChecked;
	StrContentData*mUnfoldChoiceTxtData;
	UnfoldChoiceWidgetTemplete* mUnfoldChoiceTemplate;
	int mPaddingLeft;
public:
	static const char RADIO = 0;//单选
	static const char MULTIPLE_CHOICE = 2;//多选
};
#endif

