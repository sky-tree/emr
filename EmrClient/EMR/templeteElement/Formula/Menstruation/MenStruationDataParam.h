#ifndef _MENSTRUATIONDATAPARAM_H_
#define _MENSTRUATIONDATAPARAM_H_
#include "CodeTempData.h"
class MenstruationTemplete;
class BaseTemplate;
class StrContentData;
class MenStruationDataParam :
	public CodeTempData
{
public:
	MenStruationDataParam(BaseTemplate * templeteOwner, CodeParams * codeParams);
	~MenStruationDataParam();
	MenstruationTemplete * mTempleteOwner;
	void setMenstruationTempleteOwner(MenstruationTemplete * templeteOwner);
public:
	void saveSelfData(BaseIOTools * ioTools);
	void loadSelfData(BaseIOTools * ioTools);
	BaseContainer * createOwner(BaseContainer * tempParent, BaseIOTools *iOTools, bool isAddToParenTemplate);
private:
	/*BaseContainerData* mLeftTextWidgetData;
	BaseContainerData* mRightTextWidgetData;
	BaseContainerData* mUpTextWidgetData;
	BaseContainerData* mDownTextWidgetData;*/
	StrContentData * mFocusTxtData;
};
#endif

