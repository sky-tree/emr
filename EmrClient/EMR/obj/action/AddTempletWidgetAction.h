#ifndef _ADD_TEMPLET_WIDGET_ACTION_H_
#define _ADD_TEMPLET_WIDGET_ACTION_H_
#include "BaseAction.h"
class ViewObj;
class BaseTemplate;
class AddTempletWidgetAction : public BaseAction
{
public:
	AddTempletWidgetAction(StepActionManger * stepActonManger);
	 ~AddTempletWidgetAction();
public:
	void unDo();
	void reDo();
	void deleteTemplete(ViewObj * deleteFocusObj, char focusDir);
	void addTemplete(ViewObj * addFocusObj, char focusDir, ViewObj * addAfterObj, char addAfterObjDir);
	void initAddAction(BaseTemplate*addTempleteWedgit, BaseTemplate * addObjTempParent, int addIndex, ViewObj * addBeforeFocus, char addBeforeDir, ViewObj * addAfterFocus, char addAfterDir);
	void initDeleteAction(BaseTemplate*deleteTempleteWedgit, BaseTemplate * deleteObjTempParent, int deleteIndex, ViewObj * deleteBeforeFocus, char deleteBeforeDir, ViewObj * deleteAfterFocus, char deleteAfterDir);
private:
	bool isAddAction;
	bool mIsUse;//是否正使用加载的元素
	BaseTemplate * mAddTempleteWedgit;
	BaseTemplate * mAddObjTempParent;
	int mAddIndex;
	ViewObj * mAddBeforeFocus;
	char mAddBeforeDir;
	ViewObj * mAddAfterFocus;
	char mAddAfterDir;
};
#endif
