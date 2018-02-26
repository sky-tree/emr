#ifndef _NODE_CHILDREN_MGR_H_
#define _NODE_CHILDREN_MGR_H_


#include <QList>



#include "TempContainerMgr.h"

class ViewObj;
class ParagraphData;
class TempParagraphMgrData;
class ParagraphManager;

// 子对象管理器
class NodeChildrenMgr : public TempContainerMgr
{
public:
	NodeChildrenMgr(BaseContainer * owner, ParagraphManager * paragraphMgr, int classID = ConstantsID::ClassId::NODE_CHILDREN_MGR);
	NodeChildrenMgr(BaseContainer * owner, ParagraphManager * paragraphMgr, NodeChildrenMgr * otherTempContainerMgr);
	~NodeChildrenMgr();

protected:


public:

	virtual bool judgeCanOperate();
	//bool addChild(BaseContainer * child, int idx, bool refreshIdx);
	bool addChild(BaseContainer * child, int idx, bool refreshIdx, QList<ParagraphData *> * useParagraphDataList, int useIndex);
	//void addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdxx);
	void addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx, QList<ParagraphData *> * useParagraphDataList, int useIndex);

	int delChild(BaseContainer * child, bool refreshIdx);
	int delChildren(int start, int num, bool refreshIdx);

	void addAllChildrenToParagraphAndView();
	void addRecordChildrenToParagraphAndView(ViewObj * lastView);
	void delRecordChildrenToParagraphAndView();


	void clearNodeList(bool circle);
private:

private:

protected:
	QList<BaseContainer *> * mNodeList;
};

#endif	//_NODE_CHILDREN_MGR_H_
