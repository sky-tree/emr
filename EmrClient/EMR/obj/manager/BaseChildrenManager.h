#ifndef BASE_CHILDREN_MANAGER_H_
#define BASE_CHILDREN_MANAGER_H_



#include <QList>

#include "BaseContainerParam.h"

// �Ӷ��������
class BaseChildrenManager : public BaseContainerParam
{
public:
	BaseChildrenManager(BaseContainer * owner);
	BaseChildrenManager(BaseContainer * owner, int classID);
	BaseChildrenManager(BaseContainer * owner, BaseChildrenManager* otherChildrenMgr);
	virtual~BaseChildrenManager();

protected:
	virtual void recycleChildren();

public:
	// -1��û���Ӷ����б�0�����Ӷ����б���û���Ӷ���1���Ӷ�������>0;
	char haveChildren();
	void setChildWriteManger(WriterManager * writeManger);
	void swapChild(BaseContainer * source, BaseContainer * dest);

protected:
	void createChildren();

private:
	void directAddChild(BaseContainer * child, int idx, bool refreshIdx);
	void directAddChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx);

	int directDelChild(int idx, bool refreshIdx);
	int directDelChild(BaseContainer * child, bool refreshIdx);
	int directDelChildren(int start, int num, bool refreshIdx);

private:

	int swapChild(BaseContainer *child, int idx);
	bool swapChildren(QList<BaseContainer *> * children, int start, int num, int idx, QList<int> * tempList);

public:
	void pushChild(BaseContainer * child, bool refreshIdx);
	virtual bool addChild(BaseContainer * child, int idx, bool refreshIdx);
	virtual void addChildren(QList<BaseContainer *> * children, int start, int num, int idx, bool refreshIdx);

	virtual int delChild(int idx, bool refreshIdx);
	virtual int delChild(BaseContainer * child, bool refreshIdx);
	virtual int delChildren(int start, int num, bool refreshIdx);
public:
	BaseContainer * getChild(int idx);
	BaseContainer * getFirstChild();
	BaseContainer * getLastChild();
	QList<BaseContainer *> * getChildren();
	int getChildrenSize();
	//QList<ViewObj *> * getViewChildren();

protected:
	// �Ӷ����б�
	QList<BaseContainer *> * mChildren;

	//QList<ViewObj *> * mViewChildren;
};

#endif
