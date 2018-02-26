#ifndef FLAG_VIEW_PARAM_H_
#define FLAG_VIEW_PARAM_H_



#include "BaseViewParam.h"

class FlagViewParam : public BaseViewParam
{
public:
	FlagViewParam(ViewObj * owner);
	~FlagViewParam();



public:
	void setFocus(ViewObj * focus, char dir);
	void getFocusAndDirection(ViewObj * &focus, char &dir);
	ViewObj * getFocus();
	char getDirection();

protected:

	ViewObj * mFocusObj;
	char mDirection;


};

#endif
