#ifndef _MACROTEMPLETEPARAM_H_
#define _MACROTEMPLETEPARAM_H_
#include "CodeTempData.h"
class MacroTemplete;
class BaseTemplate;
class MacroTempleteParam :public CodeTempData
{
public:
	MacroTempleteParam(BaseTemplate * templeteOwner, CodeParams * codeParams);
	~MacroTempleteParam();
	void setMcrodTempleteOwner(MacroTemplete * templeteOwner);
public:
	void saveSelfData(BaseIOTools * ioTools);
	void loadSelfData(BaseIOTools * ioTools);
	BaseContainer * createOwner(BaseContainer * tempParent, BaseIOTools *iOTools, bool isAddToParenTemplate);
	MacroTemplete * mTempleteOwner;
	int mId;
};
#endif