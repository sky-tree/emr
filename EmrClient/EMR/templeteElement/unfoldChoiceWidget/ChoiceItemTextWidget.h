#ifndef _CHOICEITEMTEXTWIDGET_H_
#define _CHOICEITEMTEXTWIDGET_H_
#include "TextWidget.h"
class UnfoldChoiceButton;
class ChoiceItemTextWidget :
	public TextWidget
{
public:
	ChoiceItemTextWidget(WriterManager * mgr, BaseContainer * templeteParent,char type, int objId,int paddingLeft, bool isCreateStrTxtData = false);
	ChoiceItemTextWidget(WriterManager * mgr, ChoiceItemTextWidget * otherStructureation);
	~ChoiceItemTextWidget();
	bool setTempleteProperty(QMap<QString, QString>& setMap);
	void setTitle(QString title, bool isAddToChildrenList = true);
	void setChecked(bool checked);
	bool getChecked();
	bool setDrawTxtParam(StrContentData* focusData);
	void setPaddingLeft(int paddingLeft);
	int getPaddingLeft();
	//int mousePress(int type, int x, int y, BaseContainer * obj);
private:
	UnfoldChoiceButton* mChoiceButton;
	//char mChoiceType;
};
#endif