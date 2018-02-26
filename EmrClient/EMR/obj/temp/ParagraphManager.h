#ifndef PARAGRAPH_MANAGER_H_
#define PARAGRAPH_MANAGER_H_


#include <QList>



#include "BaseElement.h"


class BaseContainer;
class ViewObj;
class BaseParagraph;
class BaseTemplate;

class ParagraphData;

class WriterManager;


// 子对象管理器
class ParagraphManager : public BaseElement
{
public:
	ParagraphManager(BaseTemplate * owner);
	ParagraphManager(BaseTemplate * owner, ParagraphManager* otherParagraphMgr);
	~ParagraphManager();

protected:


public:

	BaseTemplate * getTempParent();

	void addNode(BaseContainer * child, bool refreshIdx, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow = true);
	void addChild(BaseContainer * child, bool refreshIdx, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow = true);
	void addChildren(QList<BaseContainer *> * children, bool refreshIdx
		, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow = true);
	void addChildren(QList<BaseContainer *> * children, int start, int num, bool refreshIdx
		, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow = true);
	void addChildren(QList<BaseContainer *> * children, int start, int num, bool refreshIdx
		, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow, bool needAddToParagraph);

	void addRecordChildren(QList<BaseContainer *> * children, ViewObj * lastView);
	void delRecordChildren(QList<BaseContainer *> * children);

	void delChild(BaseContainer * child, bool refreshIdx, bool needReplaceRow = true);
	void delChildren(QList<BaseContainer *> * children, int start, int num, bool refreshIdx, bool needReplaceRow = true);

	void addParagraph(BaseParagraph * paragraph, int idx, bool refreshIdx);
	void delParagraph(BaseParagraph * paragraph, bool refreshIdx);

	ViewObj * getFirstLayer();
	void setExcelParagrphManager();
	QList<BaseParagraph *> * getParagraphList();
	BaseParagraph * getParagrapAt(int index);

	void refreshChildrenIndex(int start);
private:
	void addExchangeRecordChildren(QList<QList<BaseContainer *> *> * seperateAllViewChildren, QList<BaseContainer *> * tempAllViewChildren, ViewObj * lastView);
	void delExchangeRecordChildren(QList<QList<BaseContainer *> *> * seperateAllViewChildren, QList<BaseContainer *> * tempAllViewChildren);
	//void seperateList(QList<QList<BaseContainer *> *> * seperateAllViewChildren, QList<BaseContainer *> * source);

	void addExchangeNode(QList<QList<BaseContainer *> *> * seperateAllViewChildren
		, QList<BaseContainer *> * tempAllViewChildren, bool refreshIdx
		, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow);
	void addExchangeChildren(QList<QList<BaseContainer *> *> * seperateAllViewChildren, QList<BaseContainer *> * tempAllViewChildren, bool refreshIdx
		, QList<ParagraphData *> * useParagraphDataList, int useIndex, bool needReplaceRow, bool needAddToParagraph);

	void delExchangeChildren(QList<QList<BaseContainer *> *> * seperateAllViewChildren
		, QList<BaseContainer *> * tempAllViewChildren, bool refreshIdx, bool needReplaceRow);

	//void resetParagraphWhenNotSameAsEnter(int startParagraphIdx, int seperSize);
	void createParagraphWhenNotCorrect(int startParagraphIdx);

private:
	WriterManager * mWriterMgr;
	BaseTemplate * mTempParent;
	QList<BaseParagraph *> * mParagraphList;
	// 当前使用paragraph的索引
	int mCurUseIndex;
	bool mIsExcelParagraphManger;
};

#endif
