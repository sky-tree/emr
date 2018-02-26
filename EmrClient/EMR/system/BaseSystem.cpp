#include "BaseSystem.h"

#include "WriterManager.h"

#include "OperateMgr.h"

#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseTemplate.h"
#include "BaseParagraph.h"

#include "ObjKeyManager.h"
#include "ObjMouseManager.h"
#include "ObjDrawManager.h"
#include "BaseMouse.h"
#include "ImageMouseInput.h"

#include "BaseDraw.h"
#include "DrawStr.h"
#include "DrawImage.h"
#include "DrawHorizontalLine.h"


#include "BaseViewParam.h"
#include "TxtViewParam.h"
#include "ImgViewParam.h"

#include "BaseContentData.h"
#include "StrContentData.h"
#include "ImgConTentData.h"

#include "ParagraphData.h"


#include "EMR\data\authority\AuthorityViewLineAndStr.h"

#include "GlobalImgData.h"

#include "BaseIOTools.h"
#include "BaseTools.h"
#include "ViewFactory.h"
#include "ContentDataFactory.h"
#include "UIConstants.h"
#include "BaseConstants.h"


#include "NoCopyTextWidget.h"
#include "StructurationTextWidget.h"
#include "BracketWidget.h"
#include "MenstruationTemplete.h"
#include "MacroTemplete.h"
#include "ChoiceWidgetTemplete.h"
#include "ExcelTemplete.h"
#include "QSharedMemory.h"
#include "QBuffer.h"
#include "QApplication.h"
#include "QClipboard.h"
#include "QMimeData.h"
#include <windows.h> 
#include "UnfoldChoiceWidgetTemplete.h"
BaseSystem * BaseSystem::mSystem = NULL;
//QList<GlobalImgData *> * BaseSystem::mImgDataList = NULL;


#include "GlobalTemplate.h"


BaseSystem::BaseSystem() 
{
	mSystem = this;
	mTempIntList = new QList<int>();
	mTempBaseContainerList = new QList<BaseContainer *>();
	mImgDataList = new QList<GlobalImgData *>();

	mFont = new QFont();

	mDefaultStrData = ViewFactory::createDefaultStrContentData();
	mDefaultParagraphData = ContentDataFactory::createDefaultParagraphData();
	mCopyList = new QList<BaseContainer *>();

	mBufferParagraphDataForCopy = new QList<ParagraphData *>();


	mTotalAuthorityViewLineAndStrList = new QList<AuthorityViewLineAndStr *>();

	QClipboard *clipboard = QApplication::clipboard();
		//connect(clipboard, SIGNAL(dataChanged()), this, SLOT(deleteMimeData()));
	//mCopyList = new QList<BaseContainer *>();
	//mDrawParamList = new QList<DrawTxtParam *>();
	//LAYOUT_REPLACE_MAX_TYPE = new int[]
	//{
	//	10000100, 10000500, 10000600, 10000700, 10000701, 10000702, 10000800, 10000900
	//};
	mSystemId = GetCurrentProcessId();//当前进程id
	static CGarbo Garbo;
}

BaseSystem::~BaseSystem()
{
	if (mSystem)
	{
		mSystem = NULL;
	}
	if (mImgDataList)
	{
		deleteVector(mImgDataList);
		mImgDataList = NULL;
	}
	if (mTempBaseContainerList)
	{
		//deleteVector(mTempBaseContainerList);
		mTempBaseContainerList->clear();
		mTempBaseContainerList = NULL;
	}
	if (mTempIntList)
	{
		mTempIntList->clear();
		delete mTempIntList;
		mTempIntList = NULL;
	}
	//if (mBufferParagraphData)
	//{
	//	deleteVector(mBufferParagraphData);
	//	mBufferParagraphData = NULL;
	//}
	if (mBufferParagraphDataForCopy)
	{
		ParagraphData * pre = NULL;
		ParagraphData * cur = NULL;
		// 清楚重复的复制段落
		for (int i = 0; i < mBufferParagraphDataForCopy->size(); i++)
		{
			cur = mBufferParagraphDataForCopy->at(i);
			if (pre == cur)
			{
				mBufferParagraphDataForCopy->removeAt(i);
				i--;
				continue;
			}
			pre = cur;
		}
		//deleteVector(mBufferParagraphDataForCopy);
		mBufferParagraphDataForCopy = NULL;
	}

	if (mCopyList)
	{
		//deleteVector(mCopyList);
		BaseContainer* tempContainer;
		int size = mCopyList->size();
		for (int i = 0; i < size; i++)
		{
			tempContainer = mCopyList->at(i);
			tempContainer->setWriteManager(NULL);
			delete tempContainer;
		}
		mCopyList->clear();
		delete mCopyList;
		mCopyList = NULL;
	}
	//if (mMimeData)
	//{
	//	delete mMimeData;
	//	mMimeData = NULL;
	//}
}

BaseSystem * BaseSystem::getInstance()
{
	if (mSystem)
	{
		return mSystem;
	}
	return new BaseSystem();
}
//int * BaseSystem::getLayoutReplaceMaxType()
//{
//	return LAYOUT_REPLACE_MAX_TYPE;
//}
QFont * BaseSystem::getFont()
{
	return mFont;
}
StrContentData * BaseSystem::getDefaultStrData()
{
	return mDefaultStrData;
}
ParagraphData * BaseSystem::getDefaultParagraphData()
{
	return mDefaultParagraphData;
}
QList<int> * BaseSystem::getTempIntList()
{
	return mTempIntList;
}
QList<BaseContainer *> * BaseSystem::getClearTempContainerList()
{
	mTempBaseContainerList->clear();
	return mTempBaseContainerList;
}
void BaseSystem::clearCopyList()
{
	int size = mCopyList->size();
	if (size > 0)
	{
		BaseContainer * obj = NULL;
		for (int i = 0; i < size; i++)
		{
			obj = mCopyList->at(i);
			delete obj;
		}
		mCopyList->clear();
	}
}
QList<BaseContainer *> * BaseSystem::getCopyList()
{
	return mCopyList;
}
//void BaseSystem::clearBufferParagraphDataIndex()
//{
//	mBufferParagraphDataIndex = 0;
//}
//int BaseSystem::getBufParagraphDataIndex()
//{
//	return mBufferParagraphDataIndex;
//}
QList<ParagraphData *> * BaseSystem::getBufParagraphDataForCopyList()
{
	return mBufferParagraphDataForCopy;
}
ParagraphData * BaseSystem::useBufferParagraphData(QList<ParagraphData *> * useParagraphDataList, int& useIndex)
{
	if (useParagraphDataList->size() < 1)
	{
		return NULL;
	}
	ParagraphData * data = useParagraphDataList->at(useIndex);
	useIndex++;
	return data;
}
void BaseSystem::setCopyList(WriterManager * writerMgr, QList<BaseContainer *> * list, bool isAbs, bool needWriteToOtherThread
	, QList<ParagraphData *> * paragraphDataList)
{
	int size = list->size();
	if (size < 1)
	{
		if (isAbs)
		{
			clearCopyList();
		}
		return;
	}
	clearCopyList();
	BaseContainer * obj = NULL;
	BaseContainer * copyObj = NULL;
	int count = 0;
	QList<ParagraphData *> * tempParagraphDataList = NULL;
	BaseParagraph * paragraph = NULL;
	BaseParagraph * preParagraph = NULL;
	ParagraphData * paragraphData = NULL;
	mBufferParagraphDataForCopy->clear();
	BaseContentData * data = NULL;

	BaseIOTools * ioTools = writerMgr->getIOTools();
	if (needWriteToOtherThread)
	{
		tempParagraphDataList = new QList<ParagraphData *>();
		ioTools->writeFile();

		for (int i = 0; i < size; i++)
		{
			obj = list->at(i);
			paragraph = obj->getParagraphParent();
			if (paragraph != preParagraph)
			{
				preParagraph = paragraph;
				tempParagraphDataList->push_back(paragraph->getParagraphData());
			}
		}
		int paragraphSize = tempParagraphDataList->size();

		ioTools->writeInt(paragraphSize);
		ioTools->writeInt(BaseSystem::getInstance()->getSystemId());
		if (paragraphSize > 0)
		{
			for (int i = 0; i < paragraphSize; i ++)
			{
				tempParagraphDataList->at(i)->doSave(ioTools);
			}

			ioTools->writeInt(size);
		}
		tempParagraphDataList->clear();
		delete tempParagraphDataList;
	}


	for (int i = 0; i < size; i++)
	{
		obj = list->at(i);
		if (needWriteToOtherThread)
		{
			data = obj->getContentData();
			data->doSave(ioTools);
		}

		copyObj = getContainerCopy(writerMgr, obj);
		mCopyList->push_back(copyObj);
		paragraph = obj->getParagraphParent();
		if (paragraph)
		{
			paragraphData = (ParagraphData *)paragraph->getContentData();
			if (preParagraph != paragraph)
			{
				mBufferParagraphDataForCopy->push_back(paragraphData);
			}
			// 如果最后一项为回车，则再添加一次
			if (obj->isEnter() && i == size - 1 && mBufferParagraphDataForCopy->size() > 0)
			{
				mBufferParagraphDataForCopy->push_back(paragraphData);
			}
		}
		else
		{
			for (int j = 0; j < paragraphDataList->size() - 1; j++)
			{
				paragraphData = (ParagraphData *)paragraphDataList->at(j);
				mBufferParagraphDataForCopy->push_back(paragraphData);
			}
			if (obj->isEnter() && i == size - 1 && mBufferParagraphDataForCopy->size() > 0)
			{
				paragraphData = (ParagraphData *)paragraphDataList->at(paragraphDataList->size() - 1);
				mBufferParagraphDataForCopy->push_back(paragraphData);
			}
		}
		preParagraph = paragraph;
	}


	if (needWriteToOtherThread)
	{
		BaseSystem::getInstance()->saveToClipboard(&ioTools->getByteArray());
		ioTools->close();
	}
}
void BaseSystem::CopyListToList(QList<BaseContainer *> * srclist, WriterManager * destWriterMgr, QList<BaseContainer *> * dstlist)
{
	BaseContainer * obj = NULL;
	BaseContainer * copyObj = NULL;
	int size = srclist->size();
	for (int i = 0; i < size; i++)
	{
		obj = srclist->at(i);
		copyObj = getContainerCopy(destWriterMgr, obj);
		dstlist->push_back(copyObj);
	}
}
QList<BaseContainer *> * BaseSystem::createCopyFromList(WriterManager * writerMgr, QList<BaseContainer *> * list)
{
	int size = list->size();
	if (size < 1)
	{
		return NULL;
	}
	QList<BaseContainer *> * dest = new QList<BaseContainer *>();
	BaseContainer * obj = NULL;
	BaseContainer * copyObj = NULL;
	for (int i = 0; i < size; i++)
	{
		obj = list->at(i);
		copyObj = getContainerCopy(writerMgr, obj);
		dest->push_back(copyObj);
	}
	return dest;
}
BaseContainer * BaseSystem::getContainerCopy(WriterManager * writerMgr, BaseContainer * source)
{
	BaseContainer * dest = NULL;
	int classID = source->getClassId();
	int userID = writerMgr->getOperateMgr()->getUserID();
	switch (classID)
	{
	case ConstantsID::ClassId::VIEW_OBJ:
		dest = new ViewObj(writerMgr, (ViewObj *)source);
		break;
	case ConstantsID::ClassId::BASE_TEMPLATE:
		break;
	case ConstantsID::ClassId::NOCOPYTEXTWIDGET:
		dest = new NoCopyTextWidget(writerMgr, (NoCopyTextWidget *)source);
		break;
	case ConstantsID::ClassId::STRUCTURATION_TEXT_TEMPLETE:
		dest = new StructurationTextWidget(writerMgr, (StructurationTextWidget *)source);
		break;
	case ConstantsID::ClassId::BRAKETWIDGET_TEMPLETE:
		dest = new BracketWidget(writerMgr, (BracketWidget *)source);
		break;
	case ConstantsID::ClassId::MENSTRUATION_TEMPLETE:
		dest = new MenstruationTemplete(writerMgr, (MenstruationTemplete *)source);
		break;
	case ConstantsID::ClassId::MACRO_WIDGET_TEMPLETE:
		dest = new MacroTemplete(writerMgr, (MacroTemplete *)source);
		break;
	case ConstantsID::ClassId::CHOICE_WIDGET_TEMPLETE:
		dest = new ChoiceWidgetTemplete(writerMgr, (ChoiceWidgetTemplete *)source);
		break;
	case ConstantsID::ClassId::UNFOLD_CHOICE_WIDGET_TEMPLETE:
		dest = new UnfoldChoiceWidgetTemplete(writerMgr, (UnfoldChoiceWidgetTemplete *)source);
		break;
	case ConstantsID::ClassId::EXCEL_TEMPLETE:
		dest = new ExcelTemplete(writerMgr, (ExcelTemplete *)source);
		break;

	}
	return dest;
}
BaseViewParam * BaseSystem::getViewParamCopy(ViewObj * owner, BaseViewParam * source)
{
	BaseViewParam * dest = NULL;
	int classID = source->getClassId();
	switch (classID)
	{
	case ConstantsID::ClassId::BASE_VIEW_PARAM:
		dest = new BaseViewParam(owner, (TxtViewParam *)source);
		break;
	case ConstantsID::ClassId::TXT_VIEW_PARAM:
		dest = new TxtViewParam(owner, (TxtViewParam *)source);
		break;
	case ConstantsID::ClassId::IMG_VIEW_PARAM:
		dest = new ImgViewParam(owner, (ImgViewParam *)source);
		break;
	}
	return dest;
}
ObjDrawManager * BaseSystem::getDrawMgrCopy(ViewObj * owner, ObjDrawManager * source)
{
	ObjDrawManager * dest = NULL;
	int classID = source->getClassId();
	switch (classID)
	{
	case ConstantsID::ClassId::OBJ_DRAW_MANAGER:
		dest = new ObjDrawManager(owner, source);
		break;
	}
	return dest;
}
BaseDraw * BaseSystem::getDrawCopy(ViewObj * owner, BaseDraw * source)
{
	BaseDraw * dest = NULL;
	int classID = source->getClassId();
	switch (classID)
	{
	case ConstantsID::ClassId::DRAW_TXT:
		dest = new DrawStr(owner);
		break;
	case ConstantsID::ClassId::DRAW_IMAGE:
		dest = new DrawImage(owner, (ImgConTentData *)owner->getContentData());
		break;
	case ConstantsID::ClassId::DRAW_HORIZONTAL_LINE:
		dest = new DrawHorizontalLine(owner, (DrawHorizontalLine *)source);
		break;
	}
	return dest;
}
BaseContentData * BaseSystem::getContentDataCopy(BaseContainer * owner, BaseContentData * source)
{
	BaseContentData * data = NULL;
	int classID = source->getClassId();
	switch (classID)
	{
	case ConstantsID::ClassId::BASE_CONTENT_DATA:
		data = new BaseContentData(owner, ConstantsID::ClassId::BASE_CONTENT_DATA, NULL);
		break;
	case ConstantsID::ClassId::STR_CONTENT_DATA:
		data = new StrContentData(owner, NULL, ((StrContentData *)source)->getString(), (StrContentData *)source);
		break;
	case ConstantsID::ClassId::IMG_CONTENT_DATA:
		data = new ImgConTentData(owner, NULL, (ImgConTentData *)source);
		break;
	}
	return data;
}
ObjKeyManager * BaseSystem::getKeyMgrCopy(ViewObj * owner, ObjKeyManager * source)
{
	ObjKeyManager * mgr = NULL;
	int classID = source->getClassId();
	switch (classID)
	{
	case ConstantsID::ClassId::OBJ_KEY_MANAGER:
		mgr = new ObjKeyManager(owner, source);
		break;
	}
	return mgr;
}
ObjMouseManager * BaseSystem::getMouseMgrCopy(ViewObj * owner, ObjMouseManager * source)
{
	ObjMouseManager * mgr = NULL;
	int classID = source->getClassId();
	switch (classID)
	{
	case ConstantsID::ClassId::OBJ_MOUSE_MANAGER:
		mgr = new ObjMouseManager(owner, source);
		break;
	}
	return mgr;
}
BaseMouse * BaseSystem::getMouseInputCopy(ViewObj * owner, BaseMouse * source)
{
	if (source)
	{
		BaseMouse * mouse = NULL;
		int classID = source->getClassId();
		switch (classID)
		{
		case ConstantsID::ClassId::BUTTON_MOUSE:
			mouse = new ImageMouseInput(owner, source);
			break;
		}
		return mouse;
	}
	else
	{
		return NULL;
	}


}
GlobalImgData * BaseSystem::getImg(QString imgName)
{
	int size = mImgDataList->size();
	for (int i = 0; i < size; i++)
	{
		GlobalImgData * imgData = mImgDataList->at(i);
		if (imgData->isSameImg(imgName))
		{
			imgData->addNum();
			return imgData;
		}
	}
	GlobalImgData * data = new GlobalImgData(imgName);
	mImgDataList->push_back(data);
	return data;
}
void BaseSystem::refreshPainterFont(QPainter &pPainter, StrContentData * data)
{
	mFont->setFamily(data->getFamily());

	mFont->setBold(data->getBold());
	mFont->setItalic(data->getItalic());
	int fontSize = data->getPointSize();
	if (data->getFlowType() != BaseConstants::NONE)
	{
		mFont->setPointSize(fontSize / 2);
	}
	else
	{
		mFont->setPointSize(fontSize);
	}
	pPainter.setFont(*mFont);
}
void BaseSystem::loadFromClipboard(QByteArray* byteArray)
{
	QClipboard *clipboard = QApplication::clipboard();
	const QMimeData * mimeData = clipboard->mimeData();
	if (mimeData)
	{
		QByteArray byteAr = mimeData->data(mMIMETypes);
		*byteArray = byteAr;
	}
	
}
void BaseSystem::saveToClipboard(QByteArray* byteArray)
{
	QClipboard *clipboard = QApplication::clipboard();
	QMimeData * mimeData = new QMimeData();
	//if (mimeData)
	{
		mimeData->setData(mMIMETypes, *byteArray);
	}
	clipboard->setMimeData(mimeData);
}
//void BaseSystem::deleteMimeData()
//{
//	QClipboard *clipboard = QApplication::clipboard();
//	const QMimeData * mimeData = clipboard->mimeData();
//	//if (mMimeData)
//	//{
//	//	if (mimeData != mMimeData)
//	//	{
//	//		delete mMimeData;
//	//		mMimeData = NULL;
//	//	}
//	//	
//	//}
//}
long BaseSystem::getSystemId()
{
	return mSystemId;
}
void BaseSystem::setCopyListWriteManger(WriterManager* writeManger, bool isCircle)
{
	if (mCopyList)
	{
		//deleteVector(mCopyList);
		BaseContainer* tempContainer;
		int size = mCopyList->size();
		for (int i = 0; i < size; i++)
		{
			tempContainer = mCopyList->at(i);
			tempContainer->setWriteManager(writeManger,isCircle);
		}
	}
}