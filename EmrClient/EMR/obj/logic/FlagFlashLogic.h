#ifndef _FLAG_FLASH_LOGIC_H_
#define _FLAG_FLASH_LOGIC_H_


#include "BaseLogic.h"

class BaseContainer;


// 字符串绘制
class FlagFlashLogic : public BaseLogic
{
public:
	FlagFlashLogic(BaseContainer * viewOwner);
	~FlagFlashLogic();

public:

	void refreshLogic();
	int myUpdate();

private:

};

#endif // _FLAG_FLASH_LOGIC_H_
