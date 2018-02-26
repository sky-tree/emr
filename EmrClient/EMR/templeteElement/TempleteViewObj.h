#ifndef _TEMPLETEVIEWOBJ_H_
#define _TEMPLETEVIEWOBJ_H_
#include "ViewObj.h"

class BaseTemplate;
class ExcelTemplete;
class TempleteViewObj :
	public ViewObj
{
public:
	TempleteViewObj(WriterManager * mgr, BaseTemplate * templeteParent, int objId);
	TempleteViewObj(WriterManager * mgr, TempleteViewObj* otherObj);
	virtual ~TempleteViewObj();
	bool canEditTempleteProperty();
	bool canEditTemplete();
	virtual bool setDrawTxtParam(StrContentData* focusData);
	//ObjKeyManager * getKeyMgr();
	// ObjMouseManager * getMouseMgr();
private:
	//ObjKeyManager * mKeyMgr;
	//ObjMouseManager * mMouseMgr;
};
#endif

