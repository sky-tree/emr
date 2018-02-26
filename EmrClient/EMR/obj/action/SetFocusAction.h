#ifndef _SETFOCUSACTION_H_
#define _SETFOCUSACTION_H_
#include "BaseAction.h"
class ViewObj;
class SetFocusAction :
	public BaseAction
{
public:
	SetFocusAction(StepActionManger * stepActonManger);
	~SetFocusAction();
	void initAction(ViewObj * preFocus, char preDir, ViewObj * afterFocus,char afterDir);
	void unDo();
	void reDo();
	ViewObj * mPreFocus;
	char mPreDir;
	ViewObj * mAfterFocus;
	char mAfterDir;
};

#endif