#include "AuthorityData.h"

#include "ViewObj.h"


#include "BaseIOTools.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

AuthorityData::AuthorityData()
: mLevel(-1)
, mAuthorityViewIndex(-1)
{
}
AuthorityData::AuthorityData(int level, QString job, int authorityViewIdx)
: mLevel(level)
, mJobName(job)
, mAuthorityViewIndex(authorityViewIdx)
{
}

AuthorityData::~AuthorityData()
{
}

void AuthorityData::doSave(BaseIOTools * ioTools)
{
	ioTools->writeInt(mLevel);
	ioTools->writeString(mJobName);
	ioTools->writeInt(mAuthorityViewIndex);
}
void AuthorityData::doLoad(BaseIOTools * ioTools)
{
	mLevel = ioTools->readInt();
	mJobName = ioTools->readString();
	mAuthorityViewIndex = ioTools->readInt();
}
int AuthorityData::getLevel()
{
	return mLevel;
}
QString AuthorityData::getJobName()
{
	return mJobName;
}