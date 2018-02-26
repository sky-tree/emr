#ifndef _SETTXTWIDGETELEMENTACTION_H_
#define _SETTXTWIDGETELEMENTACTION_H_
#define delete_before_List(list,isSameObjInList) \
if (list)\
{\
if (isUseBefor)\
	{\
	list->clear(); \
	}\
	else\
	{\
if (isSameObjInList)\
		{\
		list->clear(); \
		}\
		else\
		{\
		deleteVector(list); \
		}\
	}\
	delete list; \
	list = NULL; \
}
#define delete_after_list(list,isSameObjInList) \
if (list)\
{\
if (isUseBefor)\
	{\
if (isSameObjInList)\
		{\
		list->clear(); \
		}\
		else\
		{\
		deleteVector(list); \
		}\
	}\
	else\
	{\
	list->clear(); \
	}\
	delete list; \
	list = NULL; \
}
#include "SetWidgetElementPropertyAction.h"
#include "qmap.h"
class TextWidget;
class ViewObj;
class BaseContainer;
class SetTxtWidgetElementAction :
	public SetWidgetElementPropertyAction
{
public:
	SetTxtWidgetElementAction(StepActionManger * mgr);
	~SetTxtWidgetElementAction();
	void unDo();
	void reDo();
	void initAction(TextWidget * setTemplete, QMap<QString, QString> setBeforMap, QMap<QString, QString> setAfterMap);
	void initBeforObjList(QList<ViewObj *> * beforDefaultList, QList<ViewObj *> * beforStrList);
	void initAfterObjList(QList<ViewObj *> * afterDefaultList, QList<ViewObj *> * afterStrList);
	static void copyObjListToObjList(QList<ViewObj *> * srcList, QList<ViewObj *> * destList,bool isClearDestList);
	static bool isSameObjInList(QList<ViewObj *> * srcList, QList<ViewObj *> * destList);
	static bool isSameObjInTwoList(QList<BaseContainer *> * srcList, QList<ViewObj *> * destList);
	TextWidget * mSetTemplete;
	QMap<QString, QString> mSetBeforMap;
	QMap<QString, QString> mSetAfterMap;
	QList<ViewObj *> * mBeforDefaultObjList;//默认显示文字列表
	QList<ViewObj *> * mBeforStrObjList;//默认显示文字列表
	QList<ViewObj *> * mAfterDefaultObjList;//默认显示文字列表
	QList<ViewObj *> * mAfterStrObjList;//默认显示文字列表
	bool isUseBefor;
};
#endif
