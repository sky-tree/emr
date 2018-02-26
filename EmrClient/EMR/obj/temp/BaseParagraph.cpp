#include "BaseParagraph.h"

#include <qDebug>

#include "WriterManager.h"
#include "OperateMgr.h"

#include "ViewObj.h"

#include "BaseContentData.h"
#include "CodeParams.h"

#include "FlagParam.h"

#include "RowLayout.h"
#include "PageContentTxtLayerLayout.h"


#include "ParagraphManager.h"


#include "ConstantsID.h"
#include "BaseConstants.h"

#include "TempleteFactory.h"
#include "ViewFactory.h"

#include "BaseTools.h"

BaseParagraph::BaseParagraph(WriterManager * writerMgr, ParagraphManager * mgr, int objID) : BaseContainer(writerMgr, ConstantsID::ClassId::PARAGRAPH_OBJ, objID)
, mParagraphMgr(mgr)
{
}
BaseParagraph::BaseParagraph(WriterManager * writerMgr, ParagraphManager * mgr, BaseParagraph* otherBaseParagraph) : BaseContainer(writerMgr, otherBaseParagraph)
, mParagraphMgr(mgr)
, mIndexFromParagraphMgr(otherBaseParagraph->mIndexFromParagraphMgr)
{

}
BaseParagraph::~BaseParagraph()
{
	mParagraphMgr = NULL;
}
void BaseParagraph::initParagraphMgr(ParagraphManager * mgr)
{
	if (mParagraphMgr != mgr)
	{
		mParagraphMgr = mgr;
	}
}
ParagraphManager * BaseParagraph::getParagraphMgr()
{
	return mParagraphMgr;
}
bool BaseParagraph::isFirstParagraph()
{
	return getIndexFromParagraphMgr() == 0;
}
bool BaseParagraph::isLastParagraph()
{
	QList<BaseParagraph *> * children = mParagraphMgr->getParagraphList();
	if (getIndexFromParagraphMgr() == children->size() - 1)
	{
		return true;
	}
	return false;
}
void BaseParagraph::refreshChildrenParagraphIndex(int start)
{
	QList<BaseContainer *> * children = getChildren();
	int size = children->size();
	ViewObj * child = NULL;
	for (int i = start; i < size; i++)
	{
		child = (ViewObj *)children->at(i);
		child->setParagraphIndex(i);
	}
}
int BaseParagraph::getIndexFromParagraphMgr()
{
	//if (!mParagraphMgr)
	//{
	//	int a = 0;
	//}
	QList<BaseParagraph *> * children = mParagraphMgr->getParagraphList();
	return mIndexFromParagraphMgr;
}
void BaseParagraph::setIndexFromParagraphMgr(int idx)
{
	mIndexFromParagraphMgr = idx;
}
BaseParagraph * BaseParagraph::getNextParagraph()
{
	int idx = getIndexFromParagraphMgr();
	if (idx < 0)
	{
		return NULL;
	}
	QList<BaseParagraph *> * list = (QList<BaseParagraph *> *)mParagraphMgr->getParagraphList();
	if (idx < list->size() - 1)
	{
		return list->at(idx + 1);
	}
	return NULL;
}