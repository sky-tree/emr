
#include "StrContentData.h"


#include "BaseIOTools.h"

#include "OperateMgr.h"

#include "ViewObj.h"

#include "TxtWinData.h"

#include "BaseLayout.h"
#include "RowLayout.h"


#include "TxtViewParam.h"

#include "DrawStr.h"

#include "ObjDrawManager.h"

#include "SystemParams.h"

#include "ViewFactory.h"

#include "BaseMath.h"
#include "BaseTools.h"
#include "BaseSystem.h"

#include "StrConstants.h"
#include "UIConstants.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "WriterManager.h"
#include "MyAppEntrance.h"
#include "BaseTemplate.h"
StrContentData::StrContentData(BaseContainer * owner, CodeParams * codeParams)
: BaseContentData(owner, ConstantsID::ClassId::STR_CONTENT_DATA, codeParams)
, mFontColorObj(NULL)
, mBackgroundColorObj(NULL)
, mString("")
, mFontFamily("")
, mUnderLineType(0)
, mUnderLineThick(0)
, mFlowType(0)
, mFontItalic(0)
, mBold(0)
, mDelLineType(0)
, mDelLineThick(0)
, mFontSize(0)
, mOprateType(NONE)
, mTxtViewParam(NULL)
, mNeedGray(false)
{
}
StrContentData::StrContentData(BaseContainer * owner, CodeParams * codeParams, QString str, int pointSize, QString family, int fontColor, int backgroundColor
	, char underLineType, short underLineThick, char delType, short delThick, bool italic, bool bold, char flowType)
	: BaseContentData(owner, ConstantsID::ClassId::STR_CONTENT_DATA, codeParams)
	, mFontColorObj(NULL)
	, mBackgroundColorObj(NULL)
	, mString(str)
	, mFontFamily(family)
	, mUnderLineType(underLineType)
	, mUnderLineThick(underLineThick)
	, mFlowType(flowType)
	, mFontItalic(italic)
	, mBold(bold)
	, mDelLineType(delType)
	, mDelLineThick(delThick)
	, mFontSize(pointSize)
	, mOprateType(NONE)
	, mTxtViewParam(NULL)
	, mNeedGray(false)
{
	setFontColor(fontColor);
	setBackgroundColor(backgroundColor);
	if (mOwner)
	{
		if (owner->isTxt())
		{
			mTxtViewParam = (TxtViewParam *)((ViewObj*)owner)->getViewParam();
			refreshTxtWH();
		}
		else
		{
			mTxtViewParam = NULL;
		}
	}
	
}
StrContentData::StrContentData(BaseContainer * owner, CodeParams * params, QString str, StrContentData * data)
: BaseContentData(owner, data, ConstantsID::ClassId::STR_CONTENT_DATA)
, mFontColorObj(NULL)
, mBackgroundColorObj(NULL)
, mString(str)
, mFontFamily(data->mFontFamily)
, mUnderLineType(data->mUnderLineType)
, mUnderLineThick(data->mUnderLineThick)
, mFlowType(data->mFlowType)
, mFontItalic(data->mFontItalic)
, mBold(data->mBold)
, mDelLineType(data->mDelLineType)
, mDelLineThick(data->mDelLineThick)
, mFontSize(data->mFontSize)
, mNeedGray(data->mNeedGray)
, mOprateType(NONE)
, mTxtViewParam(NULL)
{
	setFontColor(data->mFontColor);
	setBackgroundColor(data->mBackgroundColor);
	if (mOwner)
	{
		if (owner->isTxt())
		{
			mTxtViewParam = (TxtViewParam *)((ViewObj *)mOwner)->getViewParam();
			refreshTxtWH();
		}
		else
		{
			mTxtViewParam = NULL;
		}
	}
}
StrContentData::~StrContentData()
{
	if (mFontColorObj)
	{
		delete mFontColorObj;
		mFontColorObj = NULL;
	}
	if (mBackgroundColorObj)
	{
		delete mBackgroundColorObj;
		mBackgroundColorObj = NULL;
	}
	if (mPainter)
	{
		mPainter = NULL;
	}
	//if (mFont)
	//{
	//	mFont = NULL;
	//}
}
void StrContentData::init()
{
}
void StrContentData::saveSelfData(BaseIOTools *iOTools)
{
	iOTools->writeString(mString);
	iOTools->writeString(mFontFamily);
	iOTools->writeBool(mBold);
	iOTools->writeBool(mFontItalic);
	iOTools->writeInt(mFontSize);
	iOTools->writeInt(mFontColor);
	iOTools->writeInt(mBackgroundColor);

	iOTools->writeByte(mUnderLineType);
	iOTools->writeShort(mUnderLineThick);

	iOTools->writeByte(mDelLineType);
	iOTools->writeShort(mDelLineThick);

	iOTools->writeByte(mFlowType);
}
void StrContentData::loadSelfData(BaseIOTools *iOTools)
{
	mString = iOTools->readString();
	mFontFamily = iOTools->readString();
	mBold = iOTools->readBool();
	mFontItalic = iOTools->readBool();

	mFontSize = iOTools->readInt();

	int fontColor = iOTools->readInt();
	setFontColor(fontColor);
	int backGroundColor = iOTools->readInt();
	setBackgroundColor(backGroundColor);

	mUnderLineType = iOTools->readByte();
	mUnderLineThick = iOTools->readShort();

	mDelLineType = iOTools->readByte();
	mDelLineThick = iOTools->readShort();

	mFlowType = iOTools->readByte();


	ViewObj * viewOwner = (ViewObj *)mOwner;
	mTxtViewParam = (TxtViewParam *)viewOwner->getViewParam();
	DrawStr * draw = new DrawStr(viewOwner);
	ObjDrawManager * drawMgr = viewOwner->getDrawMgr();
	drawMgr->initDraws(draw, NULL);

	refreshTxtWH();
}
void StrContentData::refreshTxtParam(int dir, bool needReplaceRow)
{
	//TxtViewParam * txtViewParam = (TxtViewParam *)mViewOwner->getViewParam();
	//if (mViewOwner->isTxt())
	//{
	//	txtViewParam->setSelfW(mStrW);
	//}

	//txtViewParam->setSelfH(mStrH);
	//txtViewParam->initAscent(mAscent, mDisAscent);
	//if (needReplaceRow && mViewOwner)
	//{
	//	ViewObj * viewParent = mViewOwner->getViewParent();
	//	if (viewParent && viewParent->isRow())
	//	{
	//		RowLayout * layout = (RowLayout *)viewParent->getLayout();
	//		//layout->refreshSelfAndParent(dir, BaseConstants::ReplaceType::REPLACE_ONLY_SELF);
	//		//viewParent->getLayout()->refreshChildrenPlace();
	//	}
	//}
}
void StrContentData::setOperateType(SetTxtType type)
{
	mOprateType = type;
}
void StrContentData::setFontColor(int color)
{
	mFontColor = color;
	if (!mFontColorObj)
		mFontColorObj = BaseMath::createColor(color);
	else
	{
		mFontColorObj->setRgba(QRgb(color));
	}
}
int StrContentData::getFontColor()
{
	return mFontColor;
}
void StrContentData::setBackgroundColor(int color)
{
	mBackgroundColor = color;
	if (!mBackgroundColorObj)
		mBackgroundColorObj = BaseMath::createColor(color);
	else
	{
		mBackgroundColorObj->setRgba(QRgb(color));
	}
}
void StrContentData::setFontColor(QColor color)
{
	mFontColor = BaseMath::exchangeQColorToColor(color);
	if (!mFontColorObj)
		mFontColorObj = new QColor(color);
	else
	{
		mFontColorObj->setRgba(color.rgba());
	}
}
QColor * StrContentData::getFontColorObj()
{
	return mFontColorObj;
}
void StrContentData::setBackgroundColor(QColor color)
{
	mBackgroundColor = BaseMath::exchangeQColorToColor(color);
	if (!mBackgroundColorObj)
		mBackgroundColorObj = new QColor(color);
	else
	{
		mBackgroundColorObj->setRgba(color.rgba());
	}
}
int StrContentData::getBackgroundColor()
{
	return mBackgroundColor;
}
QColor * StrContentData::getBackgroundColorObj()
{
	return mBackgroundColorObj;
}
char StrContentData::getUnderLineType()
{
	return mUnderLineType;
}
short StrContentData::getUnderLineThick()
{
	return mUnderLineThick;
}
char StrContentData::getDelLineType()
{
	return mDelLineType;
}
short StrContentData::getDelLineThick()
{
	return mDelLineThick;
}
int StrContentData::getPointSize()
{
	return mFontSize;
}
void StrContentData::setPointSize(int pointSize)
{
	if (pointSize == mFontSize)
	{
		return;
	}
	mFontSize = pointSize;
	if (mTxtViewParam)
	{
		refreshTxtWH();
	}
}
void StrContentData::setUnderLine(char type, short thick)
{
	mUnderLineType = type;
	mUnderLineThick = thick;
}
void StrContentData::setDelLine(char type, short thick)
{
	mDelLineType = type;
	mDelLineThick = thick;
}
void StrContentData::setNeedGray(bool needGray)
{
	mNeedGray = needGray;
}
bool StrContentData::getNeedGray()
{
	return mNeedGray;
}
void StrContentData::refreshTxtParamBySourceParam(StrContentData * data, bool needRefreshWH)
{
	if (!data)
	{
		return;
	}
	mOprateType = data->mOprateType;
	//mFont = new QFont(*data->mFont);
	//mGraphics = NULL;
	setFontColor(data->mFontColor);
	setBackgroundColor(data->mBackgroundColor);

	// 下划线类型
	mUnderLineType = data->mUnderLineType;
	mUnderLineThick = data->mUnderLineThick;

	mFontSize = data->mFontSize;
	// 删除线类型
	mDelLineType = data->mDelLineType;
	mDelLineThick = data->mDelLineThick;

	// 浮动类型：上下标
	mFlowType = data->mFlowType;
	mFontItalic = data->mFontItalic;
	mBold = data->mBold;
	mFontFamily = data->mFontFamily;

	if (needRefreshWH)
		refreshTxtWH();
	//setFont(data->mFont);
}
bool StrContentData::refreshTxtParamOnSetType(StrContentData * data)
{
	bool isNeedFresh = false;
	mOprateType = data->mOprateType;
	switch (data->mOprateType)
	{
	case SetTxtType::FontColor:
		setFontColor(data->mFontColor);
		setBackgroundColor(data->mBackgroundColor);
		break;
	case SetTxtType::UnderLineType:
		// 下划线类型
		mUnderLineType = data->mUnderLineType;
		mUnderLineThick = data->mUnderLineThick;
		break;
	case SetTxtType::DelLineType:
		// 删除线类型
		mDelLineType = data->mDelLineType;
		mDelLineThick = data->mDelLineThick;
		break;
	case SetTxtType::FlowType:
		// 浮动类型：上下标
		if (mFlowType != data->mFlowType)
		{
			mFlowType = data->mFlowType;
			isNeedFresh = true;
		}		
		break;
	case SetTxtType::FontItalic:
		mFontItalic = data->mFontItalic;
		break;
	case SetTxtType::Family:
		setFamily(data->getFamily());
		break;
	case SetTxtType::FontSize:
		if (mFontSize != data->getPointSize())
		{
			mFontSize = data->getPointSize();
			isNeedFresh = true;
		}
		//mFont->setPointSize(data->getPointSize()); //setPointSize(data->mfontSize);
		break;
	case SetTxtType::Bold:
		setBold(data->mBold);
		break;
	//case SetTxtType::FontSize:
	//	setFont(data->mFont);
	//	break;
	}
	return isNeedFresh;
}
void StrContentData::refreshTxtParamExceptSetType(StrContentData * data, SetTxtType operateType)
{
	if (operateType != SetTxtType::FontColor)
	{
		setFontColor(data->mFontColor);
		setBackgroundColor(data->mBackgroundColor);
	}
	if (operateType != SetTxtType::UnderLineType)
	{
		// 下划线类型
		mUnderLineType = data->mUnderLineType;
		mUnderLineThick = data->mUnderLineThick;
	}
	if (operateType != SetTxtType::DelLineType)
	{
		// 删除线类型
		mDelLineType = data->mDelLineType;
		mDelLineThick = data->mDelLineThick;
	}
	if (operateType != SetTxtType::FlowType)
	{
		// 浮动类型：上下标
		mFlowType = data->mFlowType;
	}
	if (operateType != SetTxtType::FontItalic)
	{
		mFontItalic = data->mFontItalic;
	}
	if (operateType != SetTxtType::Family)
	{
		setFamily(data->getFamily());
	}
	if (operateType != SetTxtType::FontSize)
	{
		mFontSize = data->getPointSize();
	}
	if (operateType != SetTxtType::Bold)
	{
		setBold(data->mBold);
	}
}
void StrContentData::refreshTxtWH()
{
	if (!mOwner)
		return;
	QFont * font = BaseSystem::getInstance()->getFont();
	font->setFamily(mFontFamily);
	font->setBold(mBold);
	font->setItalic(mFontItalic);
	if (mFlowType != BaseConstants::NONE)
	{
		font->setPointSize(mFontSize / 2);
	}
	else
	{
		font->setPointSize(mFontSize);
	}
	if (!mTxtViewParam)
	{
		return;
	}
	QFontMetrics  graphics(*font);
	char dir = 0;
	float preW = mTxtViewParam->getSelfW();
	int w = 0; 
	int h = graphics.height();
	// 如果为页码，则以当前最大页数来计算宽度
	if (mOwner->isPageNumber())
	{
		OperateMgr * operate = mOwner->getOperateMgr();
		SystemParams * sysParam = operate->getSystemParams();
		char type = 0;
		if (mOwner->isObj(ConstantsID::ContainerId::TITLE_PAGE_NUMBER))
		{
			type = sysParam->getTitleNumViewType();
		}
		else
		{
			type = sysParam->getBottomNumViewType();
		}
		TxtWinData * winData = operate->getWinData();
		if (winData)
		{
			w = BaseTools::getMaxPageNumberWidth(winData, graphics, type);
		}
		else
		{
			QChar  tChar = mString[0];
			w = graphics.width(tChar);
			//w = graphics.charWidth(mString, 0);
		}
		//int pageNum = winData->getPageListNum();
		//if ()
		//{
		//}
		//w = graphics.width(mString);
	}
	else
	{
		QChar  tChar = mString[0];
		//w = graphics.width(tChar);
		w = graphics.charWidth(mString, 0);
	}
	mTxtViewParam->setSelfW(w);
	mTxtViewParam->setSelfH(h);

	if (w > preW)
	{
		dir = BaseConstants::Direction::LEFT;
	}
	else if (w < preW)
	{
		dir = BaseConstants::Direction::RIGHT;
	}

	//if (mFlowType != BaseConstants::NONE)
	//{
	//    font->setPointSize(mFontSize);
	//	QFontMetrics fontMetr(*font);
	//	mSourceStrH = fontMetr.height();
	//}


	int ascent = graphics.ascent();
	int descent = graphics.descent();
	mTxtViewParam->setAscent(ascent);
	mTxtViewParam->setDescent(descent);
//	mDisAscent = graphics.descent();

	if (!mOwner)
		return;
	//// 如果可操作参数为空，代表对象还未初始化完
	//if (mViewOwner->getOperateMgr() == NULL)
	//{
	//	return;
	//}
	//refreshTxtParam(dir, needReplaceRow);
}
void StrContentData::setTxtParamW(int w)
{
	if (mTxtViewParam)
	{
		mTxtViewParam->setSelfW(w);
	}

}
void StrContentData::setBold(bool enable)
{
	if (enable == mBold)
	{
		return;
	}
	mBold = enable;
	refreshTxtWH();
}
bool StrContentData::getBold()
{
	return mBold;
}

void StrContentData::setItalic(bool italic)
{
	mFontItalic = italic;
}
bool StrContentData::getItalic()
{
	return mFontItalic;
}
void StrContentData::setFamily(QString family)
{
	if (mFontFamily.compare(family.trimmed()) == 0)
	{
		return;
	}
	mFontFamily = family;
	refreshTxtWH();
}
QString StrContentData::getFamily()
{
	return mFontFamily;
}
QString &StrContentData::getString()
{
	return mString;
}
void StrContentData::setString(QString str)
{
	mString = str;
	refreshTxtWH();

	//mViewOwner->getViewParent()->getLayout()->refreshChildrenPlace();
}

char StrContentData::getFlowType()
{
	return mFlowType;
}
void StrContentData::setFlowType(char type)
{
	char preType = mFlowType;
	if (preType == type)
	{
		return;
	}
	mFlowType = type;
	// 设置前后有不浮动和浮动
	if (type == BaseConstants::NONE || preType == BaseConstants::NONE)
	{
		//// 如果设置为上下标
		//if (type != BaseConstants::NONE)
		//{
		//	font->setPointSize(mFontSize / 2);
		//}
		//// 从上下标恢复
		//else
		//{
		//	font->setPointSize(mFontSize);
		//}
		////mFontSize = font->pointSize();
		//clearGraphics();
		//createGraphics();
		refreshTxtWH();
	}
}
bool StrContentData::isSameTxtType(StrContentData * source)
{
	if (mOwner->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW) || source->getOwner()->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW))
		return false;
	if (mNeedGray != source->mNeedGray)
		return false;
	if (mFontItalic != source->mFontItalic)
		return false;
	if (mFontSize != source->mFontSize)
		return false;
	if (mFontFamily != source->mFontFamily)
		return false;
	if (mBold != source->mBold)
		return false;
	if (mFontColor != source->mFontColor)
		return false;
	if (mBackgroundColor != source->mBackgroundColor)
		return false;
	if (mUnderLineType != source->mUnderLineType)
		return false;
	if (mDelLineType != source->mDelLineType)
		return false;

	if (mFlowType != source->mFlowType)
		return false;

	if (mUnderLineThick != source->mUnderLineThick)
		return false;
	if (mDelLineThick != source->mDelLineThick)
		return false;

	return true;
}
void StrContentData::initPainterAndFont(BaseTemplate * tempTemplate)
{
	WriterManager * writer = tempTemplate->getWriterMgr();
	if (writer)
	{
		mPainter = writer->getPainter();
	}
}