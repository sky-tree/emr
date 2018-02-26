#ifndef CODE_TEMP_DATA_H_
#define CODE_TEMP_DATA_H_


#include <QString>


#include "BaseContentData.h"


class ParagraphManager;


class CodeTempData : public BaseContentData
{
public:
	CodeTempData(BaseContainer * owner, int classID, CodeParams * codeParamsD);
	CodeTempData(BaseContainer * owner, CodeTempData * otherCodeTempData, int classID);
	~CodeTempData();

public:
	void init();
	virtual void saveSelfData(BaseIOTools *ioTools);
	virtual void loadSelfData(BaseIOTools *ioTools);
	void saveChildrenData(BaseIOTools *ioTools);
	virtual BaseContainer* createOwner(BaseContainer* templateParent) ;
public:
	//void refreshAllViewChildren();
	//QList<ViewObj *> * getAllViewChildren();

	//void addAllViewToList();

	ParagraphManager * getParentParagraphMgr();

	bool canWrite();
	void setAuthority(int authority);
	void setCodeId(QString codeId);
	QString getCodeId();

protected:
	void saveFocusData(StrContentData * txtData, BaseIOTools *iOTools);
	StrContentData *  loadFocusData(BaseIOTools *iOTools);
private:
	QString mCodeID;

	int mWriteAuthority;

	//QList<ViewObj *> * mAllViewChildren;
	//BaseIOTools* mIOTools;
};

#endif
