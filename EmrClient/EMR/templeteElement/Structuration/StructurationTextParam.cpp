#include "StructurationTextParam.h"
#include "ConstantsID.h"

#include "BaseIOTools.h"
#include "BaseTemplate.h"
#include "TempleteFactory.h"
#include "TextWidget.h"
#include "StrContentData.h"
//#include "BaseIOTools.h"
#include "NumberInputFilter.h"
#include "StructurationTextWidget.h"
#include "ViewObj.h"
#include "WriterManager.h"
#include "BaseChildrenManager.h"
StructurationTextParam::StructurationTextParam(BaseTemplate * templeteOwner, CodeParams * codeParams) : CodeTempData(templeteOwner
	, ConstantsID::ClassId::STRUCRATION_TEXT_DATA_PARAM, codeParams)
, mTempleteOwner((StructurationTextWidget*)templeteOwner)
{
	//setNeedLoadSelfChildren(false);
}


StructurationTextParam::~StructurationTextParam()
{
	if (mTempleteOwner)
	{
		mTempleteOwner = NULL;
	}
}
void StructurationTextParam::setTextTempleteOwner(StructurationTextWidget * templeteOwner)
{
	mTempleteOwner = templeteOwner;
	mOwner = templeteOwner;
}

void StructurationTextParam::saveStrAndDefaultStr(BaseIOTools * ioTools)
{
	QString ttring = mTempleteOwner->getString();
	int sizeOfStr = ttring.length();
	ioTools->writeInt(sizeOfStr);
	if (sizeOfStr >0){
		ioTools->writeString(ttring);
	}
	QString defaultString = mTempleteOwner->getDefaultString();
	int defaultSize = defaultString.length();
	ioTools->writeInt(defaultSize);
	if (defaultSize >0){
		ioTools->writeString(defaultString);
	}
}
void StructurationTextParam::loadStrAndDefaultStr(BaseIOTools * ioTools)
{
	int sizeOfStr = ioTools->readInt();
	if (sizeOfStr>0){
		mStr = ioTools->readString();
	}
	int sizeOfDefault = ioTools->readInt();
	if (sizeOfDefault>0){
		mDefaultString = ioTools->readString();
	}
}
void StructurationTextParam::saveSelfData(BaseIOTools * ioTools)
{
	//return;
	mOwnerObjId = mTempleteOwner->getObjID();
	if (mOwnerObjId == ConstantsID::ContainerId::STRUCTURATION_TEXT_TEMPLETE_OBJ)
	{
		StrContentData * txtData = mTempleteOwner->getSelfFocusData();
		saveFocusData(txtData, ioTools);
		//TextWidget 的属性
		//// mIOTools->writeInt(mTempleteOwner->getTemptStatus());
		saveStrAndDefaultStr(ioTools);
		//时间控件的子对象每一个字状态时相同的，所以删除掉子对象，不用保存单个子对象的属性
		//// mIOTools->writeString(mTempleteOwner->getDefaultString());
		setNeedLoadSelfChildren(false);
		QString valueType = mTempleteOwner->getValueType();
		if (valueType.length()>0)
		{
			ioTools->writeString(valueType);
		}
		QString maxValue = mTempleteOwner->getMaxValue();
		if (maxValue.length()>0)
		{
			ioTools->writeBool(true);
			ioTools->writeString(maxValue);
		}
		else
		{
			ioTools->writeBool(false);
		}
		QString minValue = mTempleteOwner->getMinValue();
		if (minValue.length()>0)
		{
			ioTools->writeBool(true);
			ioTools->writeString(minValue);
		}
		else
		{
			ioTools->writeBool(false);
		}
		
		QString lenght = mTempleteOwner->getLength();
		if (lenght.length()>0)
		{
			ioTools->writeBool(true);
			ioTools->writeString(lenght);
		}
		else
		{
			ioTools->writeBool(false);
		}
		QString dicimalLen = mTempleteOwner->getDicimalLen();
		if (dicimalLen.length()>0)
		{
			ioTools->writeBool(true);
			ioTools->writeString(dicimalLen);
		}
		else
		{
			ioTools->writeBool(false);
		}
		QString timeFormat = mTempleteOwner->getTimeFomat();
		if (timeFormat.length()>0)
		{
			ioTools->writeBool(true);
			ioTools->writeString(timeFormat);
		}
		else
		{
			ioTools->writeBool(false);
		}
		bool hadSetCurrentTime  = mTempleteOwner->getHadGetCurrentTime();
		ioTools->writeBool(hadSetCurrentTime);
		bool isTwentyFourFomat = mTempleteOwner->getIsTwentyFourFomat();
		ioTools->writeBool(isTwentyFourFomat);
	}
	ioTools->writeInt(mTempleteOwner->getTemptStatus());
	if (mTempleteOwner->mNameStr.size()>0)
	{
		ioTools->writeBool(true);
		ioTools->writeString(mTempleteOwner->mNameStr);
	}
	else
	{
		ioTools->writeBool(false);
	}
}

void StructurationTextParam::loadSelfData(BaseIOTools * ioTools)
{
	//return;
	if (mOwnerObjId == ConstantsID::ContainerId::STRUCTURATION_TEXT_TEMPLETE_OBJ)
	{
		mFocusTxtData = loadFocusData(ioTools);
		//TextWidget 的属性
		//mTemptStatus= mIOTools->readInt();
		loadStrAndDefaultStr(ioTools);
		setNeedLoadSelfChildren(false);
		//mDefaultString = mIOTools->readString();
		mValueType = ioTools->readString();
		if (ioTools->readBool())
		{
			mMaxValue = ioTools->readString();
		}
		if (ioTools->readBool())
		{
			mMinValue = ioTools->readString();
		}
		if (ioTools->readBool())
		{
			mLength = ioTools->readString();
		}
		if (ioTools->readBool())
		{
			mDecimalLen = ioTools->readString();
		}
		if (ioTools->readBool())
		{
			mTimeFomat = ioTools->readString();
		}
		mHadSetCurrentTime = ioTools->readBool();
		mIsTwentyFourFomat = ioTools->readBool();
	}
	mTemptStatus = ioTools->readInt();
	if (ioTools->readBool())
	{
		mNameStr = ioTools->readString();
	}
}
BaseContainer * StructurationTextParam::createOwner(BaseContainer * tempParent, BaseIOTools * ioTools, bool isAddToParenTemplate)
{
	StructurationTextWidget * temptTextWidget;
	BaseTemplate* tempParent1 = (BaseTemplate*)tempParent;
	if (mOwnerObjId == ConstantsID::ContainerId::STRUCTURATION_TEXT_TEMPLETE_OBJ)
	{
		temptTextWidget = TempleteFactory::createStructurationTemplete(tempParent1, true);
		temptTextWidget->setTxtData(mFocusTxtData);
		mFocusTxtData->initPainterAndFont(tempParent1);
		temptTextWidget->initWidget(mDefaultString);
		temptTextWidget->setStr(mStr);
		temptTextWidget->initContentData(this);
		temptTextWidget->setTempleteStatus(mTemptStatus);
		setTextTempleteOwner(temptTextWidget);
		mOwner = temptTextWidget;

		temptTextWidget->setValueType(mValueType);
		temptTextWidget->setMaxValue(mMaxValue);
		temptTextWidget->setMinValue(mMinValue);
		temptTextWidget->setLength(mLength);
		temptTextWidget->setDicimalLen(mDecimalLen);
		temptTextWidget->setTimeFomat(mTimeFomat);
		temptTextWidget->setHadGetCurrentTime(mHadSetCurrentTime);

	}
	mTempleteOwner->mNameStr = mNameStr;
	if (temptTextWidget->isCanGetCurrentTime())
	{
		if (!tempParent->canEditTempleteProperty())
		{
			temptTextWidget->getCurrentTime();
		}
	}
	if (mNeedLoadChildren)
	{
		/*for (int i = 0; i < mChildrenDataList->size(); i++)
		{
			BaseContainerData * data = mChildrenDataList->at(i);
			BaseContainer * obj = data->createOwner((BaseTemplate *)mOwner, viewParent);
			((BaseTemplate *)mOwner)->addTempleteChild(obj, -1, false, false);
		}*/
		loadChildrenData(ioTools);
		((TextWidget *)mOwner)->addStrObjListToStringObjList((QList<ViewObj *> *)mOwner->getChildren());
	}
	if (isAddToParenTemplate)
	{
		tempParent->getChildrenMgr()->pushChild(mOwner, true);
	}
	return mOwner;
}

