#ifndef _UNFOLDCHOICEWIDGERTEMPLATEPARAM_H_
#define _UNFOLDCHOICEWIDGERTEMPLATEPARAM_H_
#include "CodeTempData.h"
#include <qlist.h>
class UnfoldChoiceWidgetTemplete;
class BaseTemplate;
class UnfoldChoiceWidgetTempleteParam :
	public CodeTempData
{
public:
	UnfoldChoiceWidgetTempleteParam(BaseTemplate * templeteOwner, CodeParams * codeParams);
	~UnfoldChoiceWidgetTempleteParam();
	UnfoldChoiceWidgetTemplete * mTempleteOwner;
	void setChoiceTempleteOwner(UnfoldChoiceWidgetTemplete * templeteOwner);
public:
	void saveSelfData(BaseIOTools * ioTools);
	void loadSelfData(BaseIOTools * ioTools);
	BaseContainer * createOwner(BaseContainer * tempParent, BaseIOTools *iOTools, bool isAddToParenTemplate);
	//QList<BaseContentData *> * mOptionDataList;
	//BaseContentData* mTitleData;
	//BaseContentData* mBeforSelectData;
	//BaseContentData* mBeforNotSelectData;
	char mTitlePosition;
	//QList<int>  mChooseList;    //选中列表
	char mWidgetType; //0 表示单选，1 表示多选
	int mTemptStatus;
	StrContentData * mFocusTxtData;
	bool mCanInputOnKeyBoard;
};
#endif
