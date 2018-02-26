#ifndef BASE_LOGIC_H_
#define BASE_LOGIC_H_

#include "BaseContainerParam.h"

class BaseContainer;

class BaseLogic : public BaseContainerParam
{
public:
	BaseLogic(BaseContainer * owner, int classID);
	virtual ~BaseLogic();


public:
	int getTime();
	void addTime();
	void clearTime();
	void setBlock(bool value);
	bool isBlock();

	virtual void refreshLogic();
public:
	virtual int myUpdate() = 0;

private:
	bool mIsBlock;
	// 时间
	int mTime;

};

#endif
