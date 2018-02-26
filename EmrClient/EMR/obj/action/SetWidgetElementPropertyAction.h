#ifndef _SETWIDGETELEMENTPROPERTYACTION_H_
#define _SETWIDGETELEMENTPROPERTYACTION_H_
#include "BaseAction.h"
class ViewObj;
class SetWidgetElementPropertyAction :
	public BaseAction
{
public:
	SetWidgetElementPropertyAction(StepActionManger * stepActonManger, int classId);
	virtual ~SetWidgetElementPropertyAction();
	void initFocus(ViewObj * beforFocus, ViewObj * aferFocus, char beforDir, char afterDir);
	ViewObj* mBeforFocus;
	ViewObj* mAfterFocus;
	char mBeforDir;
	char mAfterDir;
};
#endif
