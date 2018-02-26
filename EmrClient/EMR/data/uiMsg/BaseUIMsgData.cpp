#include "BaseUIMsgData.h"

#include "WriterManager.h"
#include "OperateMgr.h"

BaseUIMsgData::BaseUIMsgData(WriterManager * writerMgr, int classID) : BaseElement(classID)
, mWriterMgr(writerMgr)
{
	mOperateMgr = mWriterMgr->getOperateMgr();
}

BaseUIMsgData::~BaseUIMsgData()
{
	mWriterMgr = NULL;
	mOperateMgr = NULL;
}