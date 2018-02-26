#ifndef _MIDDLE_BUTTON_VIEW_PARAM_H_
#define _MIDDLE_BUTTON_VIEW_PARAM_H_

#pragma once
#include "BaseViewParam.h"


class MiddleButtonViewParam :
	public BaseViewParam
{
public:
	MiddleButtonViewParam(ViewObj * owner);
	~MiddleButtonViewParam();
public:
	bool setSelfH(int value, char childrenOrParentType, bool needAddToReplace = false);

};

#endif
