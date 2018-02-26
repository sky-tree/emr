#ifndef _NODE_PARAMS_H_
#define _NODE_PARAMS_H_


#include <QString>


#include "CodeParams.h"


#include "ConstantsID.h"

class BaseIOTools;

class NodeParams : public CodeParams
{
public:
	NodeParams(int classID = ConstantsID::ClassId::NODE_DATA);
	NodeParams(QString id, QString name, int level, int classID = ConstantsID::ClassId::NODE_DATA);
	~NodeParams();

public:
	virtual void saveSelfData(BaseIOTools *ioTools);
	virtual void loadSelfData(BaseIOTools *ioTools);

public:

	int getLevel();

protected:

private:
	int mLevel;
};

#endif	//_NODE_PARAMS_H_
