#include "BaseContentData.h"
#include "QList.h"



#include "CodeParams.h"
#include "NodeParams.h"
#include "ParagraphCodeParams.h"
#include "BaseIOTools.h"
#include "ContentDataFactory.h"

BaseContentData::BaseContentData(BaseContainer * owner, int classID, CodeParams * codeParams) : BaseContainerParam(owner, classID)
//, mUserID(userId)
, mChooseObjId(-1)
, mCodeParams(codeParams)
{
	mNeedLoadChildren = true;
}
BaseContentData::BaseContentData(BaseContainer * owner, BaseContentData * otherBaseContainerData, int classID)
: BaseContainerParam(owner, otherBaseContainerData, classID)
{
	if (otherBaseContainerData)
	{
		mNeedLoadChildren = otherBaseContainerData->mNeedLoadChildren;
		mOwnerObjId = otherBaseContainerData->mOwnerObjId;
		mChooseObjId = otherBaseContainerData->mChooseObjId;
		mCodeParams = otherBaseContainerData->mCodeParams;
	}
	else
	{
		mNeedLoadChildren = true;
		mChooseObjId = -1;
		if (owner)
			mOwnerObjId = owner->getObjID();
		else
			mOwnerObjId = -1;
		mCodeParams = NULL;
	}
}
BaseContentData::~BaseContentData()
{
	/*if (mChildrenDataList)
	{
		mChildrenDataList->clear();
		delete mChildrenDataList;
		mChildrenDataList = NULL;
	}*/
	if (mCodeParams)
	{
		delete mCodeParams;
		mCodeParams = NULL;
	}
	mOwner = NULL;
}
void BaseContentData::saveCodeParams(BaseIOTools *iOTools)
{
	if (mCodeParams)
	{
		iOTools->writeByte(1);
		mCodeParams->saveID(iOTools);
		mCodeParams->saveSelfData(iOTools);
	}
	else
		iOTools->writeByte(0);
}
void BaseContentData::loadCodeParams(BaseIOTools *iOTools)
{
	char haveCodeParams = iOTools->readByte();
	// 如果有codeParams
	if (haveCodeParams == 1)
	{
		mCodeParams = ContentDataFactory::loadCodeParams(iOTools, this);
	}
}
void BaseContentData::doSave(BaseIOTools *iOTools)
{
	saveID(iOTools);
	saveCodeParams(iOTools);
	saveSelfData(iOTools);
	saveChildrenData(iOTools);
}
void BaseContentData::doLoadWithoutID(BaseIOTools *iOTools)
{
	loadCodeParams(iOTools);
	loadSelfData(iOTools);
	loadChildrenData(iOTools);
}
void BaseContentData::setNeedLoadSelfChildren(bool needLoad)
{
	/*if (needLoad)
	{
		mChildrenDataList = new QList<BaseContentData *>();
	}*/
	mNeedLoadChildren = needLoad;

}
//void BaseContentData::saveCodeParams(BaseIOTools *iOTools)
//{
//}
//void BaseContentData::loadCodeParams(BaseIOTools *iOTools)
//{
//}
void BaseContentData::saveChildrenData(BaseIOTools *iOTools)
{
	if (!mNeedLoadChildren)
		return;
	if (mOwner->getObjID()==202)
	{
		int a = 0;
	}
	int size = 0;
	QList<BaseContainer *> * children = mOwner->getChildren();
	if (children)
		size = children->size();
	// 子对象数量
	iOTools->writeInt(size);
	if (mOwner->isObj(ConstantsID::ContainerId::NORMAL_PARAGRAPH))
	{
		int a = 0;
	}
	// 遍历调用每个子数据的存档
	for (int i = 0; i < size; i++)
	{
		BaseContainer * child = children->at(i);
		BaseContentData * data = child->getContentData();
		if (data)
		{
			data->doSave(iOTools);
		}
		else
		{
			int a = 0;
		}
	}
}
void BaseContentData::loadChildrenData(BaseIOTools *iOTools)
{
	if (!mNeedLoadChildren)
		return;
	int size = iOTools->readInt();
	if (size < 1)
		return;

	//mChildrenDataList = new QList<BaseContentData *>();

	if (mOwnerObjId == ConstantsID::ContainerId::NORMAL_PARAGRAPH)
	{
		int a = 0;
	}
	WriterManager * mgr = mOwner->getWriterMgr();
	if (mOwner->getObjID() == ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER)
	{
		int a = 0;
	}
	if (mOwner->getObjID() == ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER)
	{
		int a = 0;
	}
	for (int i = 0; i < size; i++)
	{
		if (size == 48 && i == 45)
		{
			int a = 0;
		}
		ContentDataFactory::loadData(mgr, iOTools, mOwner);
	}
	/*if (mOwner)
	{
		WriterManager * mgr = mOwner->getWriterMgr();
		for (int i = 0; i < size; i++)
		{

			ContentDataFactory::loadData(mgr, iOTools, mOwner);
		}
	}
	else
	{
		for (int i = 0; i < size; i++)
		{

			ContentDataFactory::loadData(NULL, iOTools, NULL);
		}
	}*/
	
}
void BaseContentData::saveID(BaseIOTools *iOTools)
{
	/*if (mClassID ==ConstantsID::ClassId::CHOICEAA_WIDGET_PARAM)
	{
		int a = 0;
	}*/
	iOTools->writeInt(mClassID);
	iOTools->writeInt(mOwner->getObjID());
	iOTools->writeInt(mOwner->getChooseObjId());

	//iOTools->writeInt(mUserID);
}
bool BaseContentData::isParagraphMgrData()
{
	if (mClassID == ConstantsID::ClassId::TEMP_PARAGRAPH_MGR_DATA)
		return true;
	return false;
}
bool BaseContentData::isTempContainerData()
{
	if (mClassID == ConstantsID::ClassId::CODE_TEMP_DATA)
		return true;
	return false;
}
bool BaseContentData::isCodeAndExtendsData()
{
	if (mClassID == ConstantsID::ClassId::CODE_TEMP_DATA
		|| mClassID == ConstantsID::ClassId::TEMP_PARAGRAPH_MGR_DATA)
		return true;
	return false;
}
BaseContainer*BaseContentData::createOwner(BaseContainer* templateParent, BaseIOTools *iOTools, bool isAddToParenTemplate)
{
	return NULL;
}
void BaseContentData::setOwnerObjId(int objId)
{
	mOwnerObjId = objId;
}
void BaseContentData::setChooseObjId(int chooseObjId)
{
	mChooseObjId = chooseObjId;
}
void BaseContentData::initCodeParams(CodeParams * params)
{
	mCodeParams = params;
}
CodeParams * BaseContentData::getCodeParams()
{
	return mCodeParams;
}
NodeParams * BaseContentData::getNodeParams()
{
	return (NodeParams *)mCodeParams;
}
ParagraphCodeParams * BaseContentData::getParagraphCodeParams()
{
	return (ParagraphCodeParams *)mCodeParams;
}
//void BaseContentData::loadID(BaseIOTools *iOTools)
//{
//	mOwnerObjId = iOTools->readInt();
//}
//int BaseContentData::getUuseId()
//{
//	return mUserID;
//}