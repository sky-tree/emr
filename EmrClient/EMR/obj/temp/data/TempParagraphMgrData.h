#ifndef _TEMP_PARAGRAPH_MGR_DATA_H_
#define _TEMP_PARAGRAPH_MGR_DATA_H_


#include <QList>


#include "CodeTempData.h"


class ParagraphData;
class ParagraphManager;


class TempParagraphMgrData : public CodeTempData
{
public:
	TempParagraphMgrData(BaseContainer * owner, int classID, CodeParams * codeParams);
	TempParagraphMgrData(BaseContainer * owner, TempParagraphMgrData* paragraphMgrData);
	~TempParagraphMgrData();

public:
	virtual void init();

	ParagraphManager * getParagraphMgr();
protected:
	virtual void saveSelfData(BaseIOTools *iOTools);
	virtual void loadSelfData(BaseIOTools *iOTools);
private:
	ParagraphManager * mParagraphMgr;
};

#endif
