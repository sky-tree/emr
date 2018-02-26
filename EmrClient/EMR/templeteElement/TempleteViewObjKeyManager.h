#ifndef TEMPLETE_VIEW_OBJ_KEY_MANAGER_H_
#define TEMPLETE_VIEW_OBJ_KEY_MANAGER_H_
#include "ObjKeyManager.h"
class TempleteViewObjKeyManager :
	public ObjKeyManager
{
public:
	TempleteViewObjKeyManager(ViewObj * view);
	~TempleteViewObjKeyManager();
	void setKey(BaseKey* key);

public:
		int keyPress(QString str);
		int keyRelease(QString str);
private:
	BaseKey* mKey;
};
#endif
