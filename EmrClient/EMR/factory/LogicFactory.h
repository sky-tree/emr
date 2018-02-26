#ifndef _LOGIC_FACTORY_H_
#define _LOGIC_FACTORY_H_


class ViewObj;

class BaseLogic;

class LogicFactory
{
public:
	LogicFactory();
	~LogicFactory();

public:

	static BaseLogic * createLogic(ViewObj * viewOwner);
};

#endif // _LOGIC_FACTORY_H_
