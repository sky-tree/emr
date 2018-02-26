#include "TempParagraphMgrData.h"




#include "BaseIOTools.h"
#include "ParagraphManager.h"
#include "BaseParagraph.h"
#include "ParagraphData.h"

#include "BaseContainer.h"
#include "BaseTemplate.h"



#include "ContentDataFactory.h"
#include "TempleteFactory.h"
#include "ViewFactory.h"
#include "ConstantsID.h"

TempParagraphMgrData::TempParagraphMgrData(BaseContainer * owner, int classID, CodeParams * codeParams) : CodeTempData(owner, classID, codeParams)
, mParagraphMgr(NULL)
{
	init();
}
TempParagraphMgrData::TempParagraphMgrData(BaseContainer * owner, TempParagraphMgrData* paragraphMgrData)
: CodeTempData(owner, paragraphMgrData, ConstantsID::ClassId::TEMP_PARAGRAPH_MGR_DATA)
, mParagraphMgr(NULL)
{
	if (paragraphMgrData)
		mParagraphMgr = new ParagraphManager((BaseTemplate *)mOwner, paragraphMgrData->mParagraphMgr);
}
TempParagraphMgrData::~TempParagraphMgrData()
{
}
void TempParagraphMgrData::saveSelfData(BaseIOTools *iOTools)
{
	if (mOwner->isObj(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER))
	{
		int a = 0;
	}
	QList<BaseParagraph *> * paragraphList = mParagraphMgr->getParagraphList();
	int size = paragraphList->size();
	iOTools->writeInt(size);
	if (size < 1)
	{
		return;
	}
	if (paragraphList->at(0)->getChildren()->size() == 7)
	{
		int a = 0;
	}
	BaseParagraph * paragraph = NULL;
	ParagraphData * data = NULL;
	for (int i = 0; i < size; i++)
	{
		paragraph = paragraphList->at(i);
		data = (ParagraphData *)paragraph->getContentData();
		data->doSave(iOTools);
	}
}
void TempParagraphMgrData::loadSelfData(BaseIOTools *iOTools)
{
	int size = iOTools->readInt();
	if (size < 1)
	{
		return;
	}
	//mParagraphMgr = new ParagraphManager((BaseTemplate *)mOwner);
	BaseParagraph * paragraph = NULL;
	for (int i = 0; i < size; i++)
	{
		paragraph = (BaseParagraph *)ContentDataFactory::loadData(mOwner->getWriterMgr(), iOTools, mOwner);
		mParagraphMgr->addParagraph(paragraph, -1, false);
		//mParagraphDataList->push_back(data);
	}
	mParagraphMgr->refreshChildrenIndex(0);
}
void TempParagraphMgrData::init()
{
	mParagraphMgr = new ParagraphManager((BaseTemplate *)mOwner);
}
ParagraphManager * TempParagraphMgrData::getParagraphMgr()
{
	return mParagraphMgr;
}