#ifndef OBJ_KEY_MANAGER_H_
#define OBJ_KEY_MANAGER_H_


class ViewObj;
class BaseKey;
#include "BaseContainerParam.h"

// 对象的按键管理
class ObjKeyManager :public BaseContainerParam
{
public:
	ObjKeyManager(ViewObj * view);
	ObjKeyManager(ViewObj * view, int classId);
	ObjKeyManager(ViewObj * view, ObjKeyManager * source);
	~ObjKeyManager();
	void setKey(BaseKey* key);

public:
	virtual	int keyPress(QString str);
	virtual	int keyRelease(QString str);
private:
	BaseKey* mKey;

};

#endif
