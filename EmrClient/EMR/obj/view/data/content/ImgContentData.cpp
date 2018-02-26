#include "ImgConTentData.h"




#include "ViewObj.h"

#include "BaseViewParam.h"
#include "DrawImage.h"
#include "ObjDrawManager.h"
#include "GlobalImgData.h"

#include "StrConstants.h"
#include "ViewFactory.h"
#include "ConstantsID.h"
#include "BaseIOTools.h"
#include "BaseSystem.h"
#include "BaseEditParam.h"
#include "WriterManager.h"
#include "MyAppEntrance.h"

ImgConTentData::ImgConTentData(BaseContainer * owner, CodeParams * codeParams)
: BaseContentData(owner, ConstantsID::ClassId::IMG_CONTENT_DATA, codeParams)
	, mBaseEditParam(NULL)
{
	mViewOwner = (ViewObj *)owner;
	mImgData = NULL;
	init();
}
ImgConTentData::ImgConTentData(BaseContainer * owner, CodeParams * codeParams, QString imgName)
: BaseContentData(owner, ConstantsID::ClassId::IMG_CONTENT_DATA, codeParams)
	, mBaseEditParam(NULL)
{
	mViewOwner = (ViewObj *)owner;
	mImgData = BaseSystem::getInstance()->getImg(imgName);
	init();
}
ImgConTentData::ImgConTentData(BaseContainer * owner, CodeParams * codeParams, ImgConTentData * source)
: BaseContentData(owner, source->getClassId(), codeParams)
	, mBaseEditParam(NULL),mSnap(NULL),mScaleW(1),mScaleH(1)
{
	mViewOwner = (ViewObj *)owner;
	mImgData = source->mImgData;
	mImgData->addNum();
	mBaseEditParam = new BaseEditParam(mViewOwner, ConstantsID::ClassId::IMG_EDIT_PARAM,
		source->mBaseEditParam);
	if (source->mSnap)
	{
		mSnap = new QImage(*source->mSnap);
	}
}

ImgConTentData::~ImgConTentData()
{
	destroy();
}
void ImgConTentData::saveSelfData(BaseIOTools *iOTools)
{
	iOTools->writeString(mImgData->getImgName());

	if (mViewOwner->isObj(ConstantsID::ContainerId::IMG))
	{
		iOTools->writeInt(mBaseEditParam->getScaleH());
		iOTools->writeInt(mBaseEditParam->getScaleW());
	}

	QList<StrItemData> items = mBaseEditParam->getItem();
	iOTools->writeInt(items.size());

	for (int i = 0; i < items.size(); i++)
	{
		iOTools->wirteStrItemData(items[i]);
	}
}
void ImgConTentData::loadSelfData(BaseIOTools *iOTools)
{
	QString str = iOTools->readString();

	if (mViewOwner->isObj(ConstantsID::ContainerId::IMG))
	{
		mBaseEditParam->setScaleH(iOTools->readInt());
		mBaseEditParam->setScaleW(iOTools->readInt());
	}

	int num = iOTools->readInt();

	QList<StrItemData> items;
	for (int i = 0; i < num; i++)
	{
		items.push_back(iOTools->readStrItemData());
	}
	mBaseEditParam->setItem(items);

	mImgData = BaseSystem::getInstance()->getImg(str);
	DrawImage * img = new DrawImage(mViewOwner, this);
	ObjDrawManager * drawMgr = mViewOwner->getDrawMgr();
	drawMgr->initDraws(img, NULL);
	int w = getImg()->width();
	int h = getImg()->height();
	mViewOwner->getViewParam()->init(0, 0, w, h);

	if (mViewOwner->getObjID() == ConstantsID::ContainerId::IMG)
	{
		WriterManager * wirteMgr = mViewOwner->getWriterMgr();
		MyAppEntrance * app = wirteMgr->getAppEntrance();
		app->getImageSnap(this);
	}
	
}
void ImgConTentData::init()
{
	mScaleW = 1;
	mScaleH = 1;
	mSnap = NULL;
	if (mBaseEditParam == NULL)
	{
		mBaseEditParam = new BaseEditParam(mViewOwner, ConstantsID::ClassId::IMG_EDIT_PARAM);
	}
}
void ImgConTentData::destroy()
{
	if (mBaseEditParam != NULL)
	{
		delete mBaseEditParam;
		mBaseEditParam = NULL;
	}
	if (mSnap != NULL)
	{
		delete mSnap;
		mSnap = NULL;
	}
}

QImage * ImgConTentData::getSnap()
{
	return mSnap;
}
QImage * ImgConTentData::getImg()
{
	return mImgData->getImg();
}

QString ImgConTentData::getImgName()
{
	return mImgData->getImgName();
}


BaseEditParam * ImgConTentData::getBaseEditParam()
{
	return mBaseEditParam;
}


void ImgConTentData::setSnap(QImage * snap)
{
	mSnap = snap;
}

void ImgConTentData::setWightAndHeight(int w, int h)
{
	mScaleW = w;
	mScaleH = h;
	if (mBaseEditParam != NULL)
	{
		mBaseEditParam->setScaleH(h);
		mBaseEditParam->setScaleW(w);
	}
}
