#ifndef _SET_PARAGRAPH_OTHER_PARAM_ACTION_H_
#define _SET_PARAGRAPH_OTHER_PARAM_ACTION_H_




class BaseWinData;
class BaseContainer;
class ViewObj; 
class BaseTemplete;
class ParagraphTemplete;

#include "BaseAction.h"

#include <QList>

// 显示相关属性
class SetParagraphOtherParamAction : public BaseAction
{
public:
	SetParagraphOtherParamAction(StepActionManger * stepActonManger);

	virtual ~SetParagraphOtherParamAction();

public:
	void unDo();
	void reDo();

	void setParagraphNumberView(bool pre, bool isView);
	void setPageTitleView(bool pre, bool isView);
	void setPageBottomView(bool pre, bool isView);
	void showTidy(bool pre, bool tidy);

	void setPageTitleNumView(bool pre, bool isView);
	void setPageBottomNumView(bool pre, bool isView);

public:


public:

private:

protected:
	char mType;

	bool mPreParagraphNumberIsView;
	bool mDestParagraphNumberIsView;

	bool mPreTitleIsView;
	bool mDestTitleIsView;

	bool mPreBottomIsView;
	bool mDestBottomIsView;

	bool mPreTidy;
	bool mDestTidy;

	bool mPrePageTitleNumView;
	bool mDestPageTitleNumView;
	bool mPrePageBottomNumView;
	bool mDestPageBottomNumView;
};

#endif
// _SET_PARAGRAPH_OTHER_PARAM_ACTION_H_
