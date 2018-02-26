#include "NodeParams.h"




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

NodeParams::NodeParams(int classID) : CodeParams(classID)
{
	mLevel = 0;
}
NodeParams::NodeParams(QString id, QString name, int level, int classID) : CodeParams(id, name, classID)
{
	mLevel = level;
}
NodeParams::~NodeParams()
{
}
void NodeParams::saveSelfData(BaseIOTools *ioTools)
{
	CodeParams::saveSelfData(ioTools);
	ioTools->writeInt(mLevel);
}
void NodeParams::loadSelfData(BaseIOTools *ioTools)
{
	CodeParams::loadSelfData(ioTools);
	mLevel = ioTools->readInt();
}
int NodeParams::getLevel()
{
	return mLevel;
}