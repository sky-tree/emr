#ifndef BASE_CONTENT_DATA_H_
#define BASE_CONTENT_DATA_H_


#include "BaseContainerParam.h"

class CodeParams;
class NodeParams;
class ParagraphCodeParams;

class BaseIOTools;

class BaseContentData : public BaseContainerParam
{
public:
	BaseContentData(BaseContainer * owner, int classID, CodeParams * codeParams);
	BaseContentData(BaseContainer * owner, BaseContentData * otherBaseContainerData, int classID);
	virtual~BaseContentData();
	void setNeedLoadSelfChildren(bool needLoad);

public:
	virtual void init(){};
public:
	void doSave(BaseIOTools *iOTools);
	void doLoadWithoutID(BaseIOTools *iOTools);

public:
	bool isParagraphMgrData();
	bool isTempContainerData();
	bool isCodeAndExtendsData();
public:
	virtual void saveSelfData(BaseIOTools *iOTools){};
	virtual void loadSelfData(BaseIOTools *iOTools){};
	virtual void saveCodeParams(BaseIOTools *iOTools);
	virtual void loadCodeParams(BaseIOTools *iOTools);

	virtual void saveChildrenData(BaseIOTools *iOTools);
	void loadChildrenData(BaseIOTools *iOTools);
	void saveID(BaseIOTools *iOTools);
	void setOwnerObjId(int objId);
	void setChooseObjId(int chooseObjId);

	void initCodeParams(CodeParams * params);
	CodeParams * getCodeParams();
	NodeParams * getNodeParams();
	ParagraphCodeParams * getParagraphCodeParams();
	//void loadID(BaseIOTools *iOTools);
	//inline	int getUserId(){	return mUserID;};
	//inline void setUserId(int useId){ mUserID = useId; };
public:
	virtual BaseContainer* createOwner(BaseContainer* templateParent, BaseIOTools *iOTools,bool isAddToParenTemplate);
protected:
	bool mNeedLoadChildren;
	//BaseIOTools * mIOTools;
	//QList<BaseContentData *> * mChildrenDataList;
	int mOwnerObjId;
	int mChooseObjId;
	//int mUserID;


	// id、name相关基本参数
	CodeParams * mCodeParams;
};

#endif
