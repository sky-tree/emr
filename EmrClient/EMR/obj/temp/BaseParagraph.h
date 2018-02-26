#ifndef _BASE_PARAGRAPH_H_
#define _BASE_PARAGRAPH_H_



#include <QPainter>


#include "BaseContainer.h"

class ParagraphManager;


class BaseViewParam;

class BaseParagraph : public BaseContainer
{
public:
	BaseParagraph(WriterManager * writerMgr, ParagraphManager * mgr,int objID);
	BaseParagraph(WriterManager * writerMgr, ParagraphManager * mgr, BaseParagraph* otherBaseParagraph);
	~BaseParagraph();

public:
	void initParagraphMgr(ParagraphManager * mgr);
	//void addChildren(QList<BaseContainer *> * children, int paragraphIdx, int viewIdx, bool judgeRepeat, bool refreshIdx);
	ParagraphManager * getParagraphMgr();
	void refreshChildrenParagraphIndex(int start);

	bool isFirstParagraph();
	bool isLastParagraph();

	int getIndexFromParagraphMgr();
	void setIndexFromParagraphMgr(int idx);
	BaseParagraph * getNextParagraph();

private:
	//void seperateList(QList<QList<ViewObj *> *> * seperateAllViewChildren, QList<ViewObj *> * source);

public:

private:
	ParagraphManager * mParagraphMgr;

	int mIndexFromParagraphMgr;
};

#endif	//_BASE_PARAGRAPH_H_
