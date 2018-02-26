#ifndef TEMP_CONTAINER_MGR_H_
#define TEMP_CONTAINER_MGR_H_


#include <QList>



#include "BaseChildrenManager.h"

class ViewObj;
class ParagraphData;
class TempParagraphMgrData;
class ParagraphManager;

// 子对象管理器
class TempContainerMgr : public BaseChildrenManager
{
public:
	TempContainerMgr(BaseContainer * owner, ParagraphManager * paragraphMgr, int classID = ConstantsID::ClassId::TEMP_CONTAINER_MGR);
	TempContainerMgr(BaseContainer * owner, ParagraphManager * paragraphMgr, TempContainerMgr* otherTempContainerMgr);
	~TempContainerMgr();

protected:


public:
	ParagraphManager * getParagraphMgr();

	virtual bool judgeCanOperate();
	virtual bool addChild(BaseContainer * child, int idx, bool refreshIdx);
	virtual bool addChild(BaseContainer * child, int idx, bool refreshIdx, QList<ParagraphData *> * useParagraphDataList, int useIndex);
	virtual void addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdxx);
	virtual void addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx, QList<ParagraphData *> * useParagraphDataList, int useIndex);

	int delChild(BaseContainer * child, bool refreshIdx);
	int delChildren(int start, int num, bool refreshIdx);

	void addAllChildrenToParagraphAndView(bool needAddToParagraph = true);
	void addRecordChildrenToParagraphAndView(ViewObj * lastView);
	void delRecordChildrenToParagraphAndView();
	void setParagraphManager(ParagraphManager * paragraphMgr);
private:

private:

protected:
	// 循环向上找第一个paragraphMgr
	ParagraphManager * mParagraphMgr;
};

#endif
