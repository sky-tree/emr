#include "ParagraphCodeParams.h"




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

ParagraphCodeParams::ParagraphCodeParams(int classID) : CodeParams(classID)
{
	mWriteAuthority = 1;
	mLevel = 0;
}
ParagraphCodeParams::ParagraphCodeParams(QString id, QString name, int authority, int level, int classID) : CodeParams(id, name, classID)
{
	mWriteAuthority = authority;
	mLevel = level;
}
ParagraphCodeParams::~ParagraphCodeParams()
{
}
void ParagraphCodeParams::saveSelfData(BaseIOTools *ioTools)
{
	CodeParams::saveSelfData(ioTools);
	ioTools->writeInt(mWriteAuthority);
	ioTools->writeInt(mLevel);
}
void ParagraphCodeParams::loadSelfData(BaseIOTools *ioTools)
{
	CodeParams::loadSelfData(ioTools);
	mWriteAuthority = ioTools->readInt();
	mLevel = ioTools->readInt();
}
int ParagraphCodeParams::getLevel()
{
	return mLevel;
}
void ParagraphCodeParams::setAuthority(int authority)
{
	mWriteAuthority = authority;
}
int ParagraphCodeParams::getAuthority()
{
	return mWriteAuthority;
}
int ParagraphCodeParams::judgeAuthority()
{
	return mWriteAuthority;
}