#include "CodeParams.h"




#include "OperateMgr.h"
#include "SystemParams.h"
#include "BaseTemplate.h"
#include "ParagraphManager.h"
#include "TempParagraphMgrData.h"
#include "StrContentData.h"

#include "ConstantsID.h"
#include "AuthorityConstants.h"
#include "BaseIOTools.h"
#include "ViewFactory.h"

CodeParams::CodeParams(int classID) : BaseElement(classID)
{
	mID = "";
	mName = "";
}
CodeParams::CodeParams(QString id, QString name, int classID) : BaseElement(classID)
{
	mID = id;
	mName = name;
}
CodeParams::~CodeParams()
{
}
void CodeParams::saveID(BaseIOTools *ioTools)
{
	ioTools->writeInt(mClassID);
}
void CodeParams::saveSelfData(BaseIOTools *ioTools)
{
	ioTools->writeString(mID);
	ioTools->writeString(mName);
}
void CodeParams::loadSelfData(BaseIOTools *ioTools)
{
	mID = ioTools->readString();
	mName = ioTools->readString();
}
QString CodeParams::getID()
{
	return mID;
}
QString CodeParams::getName()
{
	return mName;
}
int CodeParams::judgeAuthority()
{
	return 1;
}