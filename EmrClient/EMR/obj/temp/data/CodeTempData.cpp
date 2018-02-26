#include "CodeTempData.h"




#include "OperateMgr.h"
#include "SystemParams.h"
#include "BaseTemplate.h"
#include "ParagraphManager.h"
#include "TempParagraphMgrData.h"
#include "StrContentData.h"

#include "ConstantsID.h"
#include "BaseConstants.h"
#include "AuthorityConstants.h"

#include "BaseIOTools.h"
#include "ViewFactory.h"


CodeTempData::CodeTempData(BaseContainer * owner, int classID, CodeParams * codeParams) : BaseContentData(owner, classID, codeParams)
{
	//mAllViewChildren = NULL;
	mWriteAuthority = AuthorityConstants::Authority::WRITE_ALL;
	//mWriteAuthority = AuthorityConstants::Authority::ONLY_READ;
}
CodeTempData::CodeTempData(BaseContainer * owner, CodeTempData * otherCodeTempData, int classID) : BaseContentData(owner, otherCodeTempData, classID)
{
	if (otherCodeTempData)
	mCodeID = otherCodeTempData->mCodeID;
	//mAllViewChildren = NULL;
	mWriteAuthority = AuthorityConstants::Authority::WRITE_ALL;
	//mWriteAuthority = AuthorityConstants::Authority::ONLY_READ;
}
CodeTempData::~CodeTempData()
{
}
void CodeTempData::init()
{
}
void CodeTempData::saveSelfData(BaseIOTools *ioTools)
{
	if (mOwner->isObj(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER) || mOwner->isObj(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER))
	{
		int a = 0;
	}
	ioTools->writeInt(mWriteAuthority);
	ioTools->writeString(mCodeID);
}
void CodeTempData::loadSelfData(BaseIOTools *ioTools)
{
	mWriteAuthority = ioTools->readInt();
	mCodeID = ioTools->readString();
}
void CodeTempData::saveChildrenData(BaseIOTools *ioTools)
{
	if (!mOwner->isObj(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER) && !mOwner->isObj(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER))
	{
		BaseContentData::saveChildrenData(ioTools);
	}
	else
	{
		bool normalSave = true;
		SystemParams * sys = mOwner->getOperateMgr()->getSystemParams();
		if (mOwner->isObj(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER))
		{
			if (sys->getTitleNumViewType() == BaseConstants::NONE)
			{
				BaseContentData::saveChildrenData(ioTools);
				return;
			}
		}
		else if (mOwner->isObj(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER))
		{
			if (sys->getBottomNumViewType() == BaseConstants::NONE)
			{
				BaseContentData::saveChildrenData(ioTools);
				return;
			}
		}
		if (!mNeedLoadChildren)
			return;
		int size = 0;
		QList<BaseContainer *> * children = mOwner->getChildren();
		if (children)
			size = children->size();
		// 子对象数量
		ioTools->writeInt(size - 1);
		if (mOwner->isObj(ConstantsID::ContainerId::NORMAL_PARAGRAPH))
		{
			int a = 0;
		}
		// 遍历调用每个子数据的存档
		for (int i = 0; i < size; i++)
		{
			BaseContainer * child = children->at(i);
			if (child->isPageNumber())
			{
				continue;
			}
			BaseContentData * data = child->getContentData();
			if (data)
			{
				data->doSave(ioTools);
			}
			else
			{
				int a = 0;
			}
		}
	}
}
BaseContainer * CodeTempData::createOwner(BaseContainer* templateParent)
{
	return NULL;
}
bool CodeTempData::canWrite()
{
	return mWriteAuthority >= AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT;
}
void CodeTempData::setAuthority(int authority)
{
	mWriteAuthority = authority;
}

void CodeTempData::setCodeId(QString codeId)
{
	mCodeID = codeId;
}

QString CodeTempData::getCodeId()
{
	return mCodeID;
}
ParagraphManager * CodeTempData::getParentParagraphMgr()
{
	BaseTemplate * tempParent = (BaseTemplate *)mOwner->getTempParent();
	BaseContentData * data = tempParent->getContentData();
	while (!data->isParagraphMgrData())
	{
		tempParent = (BaseTemplate *)tempParent->getTempParent();
		data = tempParent->getContentData();
	}
	ParagraphManager * mgr = ((TempParagraphMgrData *)data)->getParagraphMgr();
	return mgr;
}
//void CodeTempData::refreshAllViewChildren()
//{
//	if (!mAllViewChildren)
//		mAllViewChildren = new QList<ViewObj *>();
//	else
//		mAllViewChildren->clear();
//	QList<BaseContainer *> * children = mOwner->getChildren();
//	int size = children->size();
//	for (int i = 0; i < size; i++)
//	{
//		BaseContainer * obj = children->at(i);
//		if (obj->isView())
//		{
//			mAllViewChildren->push_back((ViewObj *)obj);
//		}
//		else
//		{
//			CodeTempData * data = (CodeTempData *)obj->getContentData();
//			data->addAllViewToList();
//		}
//	}
//}
//QList<ViewObj *> * CodeTempData::getAllViewChildren()
//{
//	return mAllViewChildren;
//}
//void CodeTempData::addAllViewToList()
//{
//	if (mOwner->haveChildren() > 0)
//	{
//		return;
//	}
//
//	QList<BaseContainer *> * children = mOwner->getChildren();
//	if (children)
//	{
//		int size = children->size();
//		for (int i = 0; i < size; i++)
//		{
//			BaseContainer * obj = children->at(i);
//			if (obj->isView())
//			{
//				mAllViewChildren->push_back((ViewObj *)obj);
//			}
//			else
//			{
//				CodeTempData * data = (CodeTempData *)obj->getContentData();
//				data->addAllViewToList();
//			}
//		}
//	}
//}
void CodeTempData::saveFocusData(StrContentData * txtData, BaseIOTools *iOTools)
{
	//QFont * font = txtData->getFont();

	// 文字大小
	//int	fontSize = font->pointSize();
	int	fontSize = txtData->getPointSize();
	//QString	fontType = font->family();
	QString	fontType = txtData->getFamily();
	//int fontWeight = font->weight();
	bool fontItalic = txtData->getItalic();
	bool fontBold = txtData->getBold();
	//文字大小
	iOTools->writeInt(fontSize);
	iOTools->writeString(fontType);
	// �ִ�
	//iOTools->writeInt(fontWeight);
	// 
	iOTools->writeBool(fontItalic);
	iOTools->writeBool(fontBold);

	// 文字颜色
	iOTools->writeInt(txtData->getFontColor());
	// 背景色
	iOTools->writeInt(txtData->getBackgroundColor());
	// 下划线
	iOTools->writeByte(txtData->getUnderLineType());

	iOTools->writeShort(txtData->getUnderLineThick());
	iOTools->writeByte(txtData->getFlowType());
	iOTools->writeByte(txtData->getDelLineType());
	iOTools->writeShort(txtData->getDelLineThick());
}
StrContentData * CodeTempData::loadFocusData(BaseIOTools *iOTools)
{
	//QFont * font = new QFont();
	int	fontSize = iOTools->readInt();

	QString	fontType = iOTools->readString();
	//int fontWeight = iOTools->readInt();
	bool fontItalic = iOTools->readBool();
	bool fontBold = iOTools->readBool();
	// 文字类型
	//font->setFamily(fontType);
	// 文字大小
	//font->setPointSize(fontSize);
	// 可能是文字粗细吧（都是QFont的参数，我没有具体看）
	//font->setWeight(fontWeight);
	// 好像是是否倾斜（都是QFont的参数，我没有具体看）
	//font->setItalic(fontItalic);
	// 文字颜色
	int strColor = iOTools->readInt();
	// 背景色
	int backgroundColor = iOTools->readInt();
	// 下划线类型
	char underlineType = iOTools->readByte();

	short underLineThick = iOTools->readShort();
	char flowType = iOTools->readByte();
	char delType = iOTools->readByte();
	short delThick = iOTools->readShort();


	StrContentData* focusTxtData = new StrContentData(NULL, NULL, ""
		, fontSize, fontType, strColor, backgroundColor, underlineType, underLineThick, delType, delThick, fontItalic, fontBold, flowType);
	return focusTxtData;
}