#include "StructurationTextWidget.h"
#include "TextWidget.h"
#include "TempleteFactory.h"
#include "ConstantsID.h"
#include "FlagParam.h"
#include "ViewObj.h"
//#include <qcalendarwidget.h>
#include "TempleteTypeEnumeration.h"
#include "OperateMgr.h"
#include "StructurationTextParam.h"
//#include "DrawTxtParam.h"
#include "BaseChildrenManager.h"
#include "ViewFactory.h"
#include "TextWidgetKeyboard.h"
#include "../../../../../views/propertySettingDlgs/StructuringPopup.h"
#include "StrConstants.h"
#include "SetTxtWidgetElementAction.h"
#include "QDateTime.h"
StructurationTextWidget::StructurationTextWidget(WriterManager * mgr, BaseTemplate * templeteParent, int objId) : BracketWidget(mgr, templeteParent
	, ConstantsID::ClassId::STRUCTURATION_TEXT_TEMPLETE, objId, BaseConstants::NONE_STR, true)
//	, mCaledar(NULL)
{
	//getOperateMgr()->setCanBeChoosenObjId(getObjId());
	//getOperateMgr()->setCanAddObjParentId(getTempleteParent()->getObjId());
	mValueType = "true";
	mIsTwentyFourFomat = true;
	initNumercial();
//	mCaledar = new  QCalendarWidget();
//	mCaledar->setWindowFlags(Qt::Popup);
//	connect(mCaledar, SIGNAL(clicked(QDate)), this, SLOT(setDate()));
}
StructurationTextWidget::StructurationTextWidget(WriterManager * mgr, StructurationTextWidget * otherStructureation)
:BracketWidget(mgr, otherStructureation, false)
{
	//getOperateMgr()->setCanBeChoosenObjId(otherStructureation->getOperateMgr()->getCanBeChoosenID());
	//getOperateMgr()->setCanAddObjParentId(otherStructureation->getOperateMgr()->getCanAddObjParentId());
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
	mValueType = otherStructureation->mValueType;
	mMaxValue = otherStructureation->mMaxValue;
	mMinValue = otherStructureation->mMinValue;
	mLength = otherStructureation->mLength;
	mDecimalLen = otherStructureation->mDecimalLen;

	mTimeFomat = otherStructureation->mTimeFomat;
	mIsTwentyFourFomat = otherStructureation->mIsTwentyFourFomat;
	mHadGetCurrentTime = otherStructureation->mHadGetCurrentTime;
	copyStrList(mgr, otherStructureation->mDefaultObjList, mDefaultObjList);
	copyStrList(mgr, otherStructureation->mStrObjList, mStrObjList);
	addAllStrObjToChild();
//	mCaledar = new  QCalendarWidget();
//	mCaledar->setWindowFlags(Qt::Popup);
//	connect(mCaledar, SIGNAL(clicked(QDate)), this, SLOT(setDate()));
	if (!mContentData)
	{
		mContentData = TempleteFactory::createStructurationParam(this);
	}
	
}

StructurationTextWidget::~StructurationTextWidget()
{
	//if (mCaledar)
	//{
	//	delete mCaledar;
	//	mCaledar = NULL;
	//}
}
void StructurationTextWidget::initNumercial()
{
	mLength = QString::number(5);
	mDecimalLen = QString::number(2);
	mMaxValue = QString::number(99.99);
	mMinValue = QString::number(0);
}
int StructurationTextWidget::mousePress(int type, int x, int y, BaseContainer  * obj)
{
	if (type == Qt::RightButton)
	{
		QMap<QString, QString> map;

		map.insert(TempleteTypeEnumeration::TYPE, TempleteTypeEnumeration::STRUCTURE);
		map.insert(TempleteTypeEnumeration::VALUETYPE, mValueType);
		map.insert(TempleteTypeEnumeration::NAME, getElementName());
		if (mValueType.compare(TempleteTypeEnumeration::STR_TRUE) == 0)
		{
			//数值类型
			QString titleStr = getTitle();
			if (titleStr.compare(QStringLiteral("格式化录入")) != 0)
			{
				map.insert(TempleteTypeEnumeration::CONTENT, titleStr);
			}
			map.insert(TempleteTypeEnumeration::LENGTH, mLength);
			map.insert(TempleteTypeEnumeration::DECIMALEN, mDecimalLen);
			map.insert(TempleteTypeEnumeration::MAXVALUE, mMaxValue);
			map.insert(TempleteTypeEnumeration::MINVALUE, mMinValue);
		}
		else
		{
			//日期类型
			map.insert(TempleteTypeEnumeration::CONTENT, getTitle());
			map.insert(TempleteTypeEnumeration::NORMAL_TIME_FORMAT, mTimeFomat);
		}
		addCodeIdToMap(map);
		QMap<QString, QString> beforMap = map;
		char dir = 0;
		ViewObj* beforFocus;
		getFocusObjAndDir(beforFocus, dir);
		bool isDelete = TempleteTypeEnumeration::showDialog(&map, true);
		if (isDelete){
			BaseTemplate::deleteSelf();
			return 1;
		}
		if (map.contains(TempleteTypeEnumeration::PROPERTY_IS_CHANG))
		{
			if (map[TempleteTypeEnumeration::PROPERTY_IS_CHANG].compare(TempleteTypeEnumeration::STR_TRUE) == 0)
			{
				QList<ViewObj *>  beforObjListBefore;
				SetTxtWidgetElementAction::copyObjListToObjList(mStrObjList, &beforObjListBefore, true);
				setTempleteProperty(map,NULL,NULL);
				ViewObj* lastObj = getLastChildView();
				lastObj->setFocus(true);
				addSetTempletePropertyAction(beforMap, map, beforObjListBefore,beforFocus,dir);
				replaceNeedReplaceList();
				return 1;
			}
		}
		
	}
	
	return BaseConstants::NONE;
}
int StructurationTextWidget::mouseRelease(int type, int x, int y, BaseContainer * obj)
{
	
	return BaseConstants::NONE;
}
int StructurationTextWidget::mouseMove(int type, int x, int y, BaseContainer * obj)
{
	int returnType = BaseConstants::NONE;
	/*if (mInputManager){
	return mInputManager->mouseMove(type, x, y, obj);
	}*/
	return returnType;
}
int StructurationTextWidget::mouseDrag(int type, int x, int y, BaseContainer * obj)
{
	if (isFocousObj(obj))
	{
		if (mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS){
			return 1;
		}
		if (mTemptStatus == BaseConstants::ObjStatus::NONEDIT_STATUS){
			return 1;
		}
	}
	else{
		return 0;
	}
	return  BaseConstants::NONE;
}
int StructurationTextWidget::mouseDoublePress(int type, int x, int y, BaseContainer * obj)
{
	if (!(mValueType.compare(TempleteTypeEnumeration::STR_TRUE) == 0))
	{
		
		//mCaledar->show();
		//mCaledar->move(QCursor::pos().x(), QCursor::pos().y());
		QString date;
		if (mTimeFomat.size()>0)
		{
			date = StructuringPopup::getDate(getTitle(),mTimeFomat);
		}
		else
		{
			date = StructuringPopup::getDate(getTitle());
		}
		if (date.compare(getTitle()) == 0)
		{
			return 1;
		}
		deleteChild(0, -1, true);
		setTitle(date,false);
		ShowObjList();
		ViewObj* lastObj = getLastChildView();
		lastObj->setFocus(true);
		replaceNeedReplaceList(0);
		return 1;
	}
	else
	{
		QString text = StructuringPopup::getInputNumber(getTitle(), mLength.toInt(), mDecimalLen.toInt(), mMinValue.toDouble(), mMaxValue.toDouble());
		if (text.compare(getTitle())==0)
		{
			return 1;
		}
		deleteChild(0,-1,true);
		setTitle(text,false);
		ShowObjList();
		replaceNeedReplaceList(0);
		ViewObj* lastObj = getLastChildView();
		lastObj->setFocus(true);
		return 1;
	}
	
	return BaseConstants::NONE;
}
int StructurationTextWidget::mouseScroll(int angle, BaseContainer * obj)
{
	int returnType = BaseConstants::NONE;

	return returnType;
}
int StructurationTextWidget::keyPress(QString str, BaseContainer * obj)
{
	if (isFocousObj(obj))
	{
		
	}
	else{
		return 0;
	}
	if (TextWidgetKeyboard::isSystemKey(str)){
		return 0;
	}
	/*if (TextWidgetKeyboard::isDeleteOrBackspaceKey(str))
	{
		BaseWinData* winData = getWinData();
		if (winData->isChoosed(this))
		{
			return 0;
		}
	}*/
	OperateMgr * operateMgr = getOperateMgr();
	if (operateMgr->isChoosed(this) || operateMgr->getChoosedSize()>0)
	{
		return BaseConstants::NONE;
	}
	ViewObj* selectObj = (ViewObj*)obj;
	char dir = 0;
	ViewObj * focusObj = NULL;
	getFocusObjAndDir(focusObj,dir);
	ViewObj* lastView = getLastChildView();
	if (lastView == selectObj){
		if (dir == BaseConstants::Direction::RIGHT)
		{
			if (str.compare(StrConstants::MyKeyBoard::BACK_SPACE) == 0)
			{
				//deleteSelf();
				ViewObj* preView = lastView->getPreView();
				if (preView)
				{
					preView->setFocus(true);
				}
				return 1;
			}
			else
			{
				return BaseConstants::NONE;
				/*BaseTemplete * tempParent = getTempleteParent();
				int idx = getTempleteIndexFromParent()+1;
				if (tempParent)
				{
					int size = str.length();
					ViewObj * row = lastView->getViewParent();
					int viewIndex = lastView->getViewIndexFromParent()+1;
					DrawTxtParam * focusData = getFocusData();
					ViewObj * txt;
					for (int i = 0; i < size; i++)
					{
						DrawTxtParam * txtParam = DrawFactory::createDrawTxtParam(NULL, str.mid(i, 1), focusData);
						txt = ViewFactory::createTxt(tempParent, row, txtParam, ConstantsID::ContainerId::TXT);
						tempParent->addTempleteChild(txt, idx, false, false);
						row->addChild(txt, viewIndex);
						//BaseTemplete::addTempleteChildToViewParent(txt, row, idx, viewIndex);
						row = txt->getViewParent();
						viewIndex = txt->getViewIndexFromParent();
						viewIndex++;
						idx++;
					}
					txt->setFocus(true, false);
				}
				return 1;*/
			}
		}
		else
		{
			TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
			return 1;
		}
		
	}
	ViewObj* firstView = getFirstChildView();
	if (firstView == selectObj)
	{
		if (dir == BaseConstants::Direction::LEFT)
		{
			if (str.compare(StrConstants::MyKeyBoard::DEL) == 0){
				//deleteSelf();
				ViewObj* nextView = firstView->getNextView();
				if (nextView)
				{
					nextView->setFocus(false);
				}
				return 1;
			}
			else{
				return BaseConstants::NONE;
				/*BaseTemplete * tempParent = getTempleteParent();
				int idx = getTempleteIndexFromParent();
				if (tempParent)
				{
					int size = str.length();
					ViewObj * row = firstView->getViewParent();
					int viewIndex = firstView->getViewIndexFromParent();
					DrawTxtParam * focusData = getFocusData();
					ViewObj * txt;
					for (int i = 0; i < size; i++)
					{
						DrawTxtParam * txtParam = DrawFactory::createDrawTxtParam(NULL, str.mid(i, 1), focusData);
						txt = ViewFactory::createTxt(tempParent, row, txtParam, ConstantsID::ContainerId::TXT);
						tempParent->addTempleteChild(txt, idx, false, false);
						row->addChild(txt, viewIndex);
						//BaseTemplete::addTempleteChildToViewParent(txt, row, idx, viewIndex);
						row = txt->getViewParent();
						viewIndex = txt->getViewIndexFromParent();
						viewIndex++;
						idx++;
					}
					txt->setFocus(true, false);
				}
				return 1;*/
			}
			
		}
		else{
			TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
			return 1;
		}
	}
	 TextWidgetKeyboard::mooveFocus(str, selectObj, dir);
	return 1;
	/*if (mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS){
		return 1;
	}
	if (mTemptStatus == BaseConstants::ObjStatus::NONEDIT_STATUS){
		return 1;
	}
	return  BaseConstants::NONE;*/
}
int StructurationTextWidget::keyRelease(QString str, BaseContainer * obj)
{
	if (isFocousObj(obj))
	{
		if (mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS){
			return 1;
		}
		if (mTemptStatus == BaseConstants::ObjStatus::NONEDIT_STATUS){
			return 1;
		}
	}
	else{
		return 0;
	}
	return  BaseConstants::NONE;
}
void StructurationTextWidget::setValueType(QString dlgType)
{
	mValueType = dlgType;
}
QString StructurationTextWidget::getValueType()
{
	return mValueType;
}
//void StructurationTextWidget::setDataTime(QString dlgType)
//{
//
//}
//QString StructurationTextWidget::getDataTime()
//{
//
//}
void StructurationTextWidget::setMaxValue(QString dlgType)
{
	mMaxValue = dlgType;
}
QString StructurationTextWidget::getMaxValue()
{
	return mMaxValue;
}
void StructurationTextWidget::setMinValue(QString dlgType)
{
	mMinValue = dlgType;
}
QString StructurationTextWidget::getMinValue()
{
	return mMinValue;
}
void StructurationTextWidget::setLength(QString dlgType)
{
	mLength = dlgType;
}
QString StructurationTextWidget::getLength()
{
	return mLength;
}
void StructurationTextWidget::setDicimalLen(QString dlgType)
{
	mDecimalLen = dlgType;
}
QString StructurationTextWidget::getDicimalLen()
{
	return mDecimalLen;
}
void StructurationTextWidget::setTimeFomat(QString timeFormat)
{
	mTimeFomat = timeFormat;
}
QString StructurationTextWidget::getTimeFomat()
{
	return mTimeFomat;
}
void StructurationTextWidget::setHadGetCurrentTime(bool hadSet)
{
	mHadGetCurrentTime = hadSet;
}
bool StructurationTextWidget::getHadGetCurrentTime()
{
	return mHadGetCurrentTime;
}
void StructurationTextWidget::setIsTwentyFourFomat(bool isTwentyFour)
{
	mIsTwentyFourFomat = isTwentyFour;
}
bool StructurationTextWidget::getIsTwentyFourFomat()
{
	return mIsTwentyFourFomat;
}
void StructurationTextWidget::getCurrentTime()
{
	if (!(mValueType.compare(TempleteTypeEnumeration::STR_TRUE) == 0))
	{
		if (mHadGetCurrentTime)
		{
			return;
		}
		mHadGetCurrentTime = true;
		QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
		QString timeStr;
		if (mTimeFomat.size()>0)
		{
			timeStr = time.toString(mTimeFomat); 
		}
		else
		{
			timeStr = time.toString("yyyy-MM-dd hh:mm:ss"); 
		}
		if (timeStr.compare(mStr) != 0)
		{
			setTitle(timeStr);
		}
	}
}
bool StructurationTextWidget::isCanGetCurrentTime()
{
	if (mValueType.compare(TempleteTypeEnumeration::STR_TRUE) == 0)
	{
		return false;
	}
	else
	{
		if (mHadGetCurrentTime)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}
bool StructurationTextWidget::setTempleteProperty(QMap<QString, QString>& setMap, QList<ViewObj *>* strObjList, QList<ViewObj *>* defaultObjList)
{
	setCodeIdToTemplete(setMap);
	mValueType = setMap.value(TempleteTypeEnumeration::VALUETYPE);
	mNameStr = setMap.value(TempleteTypeEnumeration::NAME);
	if (mValueType.compare(TempleteTypeEnumeration::STR_TRUE) == 0)
	{
		//数值类型
		if (strObjList)
		{
			//证明是redo 或者 undo调用
			copyObjListAndReplace(strObjList, defaultObjList);
			//SetTxtWidgetElementAction::copyObjListToObjList(strObjList, mStrObjList, true);
		}
		else
		{
			QString content = setMap.value(TempleteTypeEnumeration::CONTENT);
			if (content.size()==0)
			{
				content = "0";
			}
			if (content.compare(getTitle()) != 0)
			{
				deleteChild(0, -1, true);
				setTitle(content,false);
				ShowObjList();
			}
		}
		
		mLength = setMap.value(TempleteTypeEnumeration::LENGTH);
		mDecimalLen = setMap.value(TempleteTypeEnumeration::DECIMALEN);
		mMaxValue = setMap.value(TempleteTypeEnumeration::MAXVALUE);
		mMinValue = setMap.value(TempleteTypeEnumeration::MINVALUE);
		mTimeFomat.clear();
		mHadGetCurrentTime = false;
	}
	else
	{
		//日期类型
		if (strObjList)
		{
			//证明是redo 或者 undo调用
			copyObjListAndReplace(strObjList, defaultObjList);
			//SetTxtWidgetElementAction::copyObjListToObjList(strObjList, mStrObjList, true);
		}
		else
		{
			//QString datatime = setMap.value(TempleteTypeEnumeration::DATATIME);
			mTimeFomat = setMap.value(TempleteTypeEnumeration::DATATIME);
			//if (datatime.size() == 0)
			//{
			//	mTimeFomat = datatime;
			//}
			QString content = setMap.value(TempleteTypeEnumeration::CONTENT);
			if (content.length() == 0)
			{
				content = mTimeFomat;
			}
			if (content.size() == 0)
			{
				content = " ";
			}
			if (content.compare(getTitle()) != 0)
			{
				deleteChild(0, -1, true);
				setTitle(content,false);
				ShowObjList();
			}
			/*if (datatime.compare(mStr) != 0)
			{
				setTitle(datatime);
			}*/
		}
		mMaxValue.clear();
		mMinValue.clear();
		mLength.clear();
		mDecimalLen.clear();
		mHadGetCurrentTime = false;
	}
	return true;
}
void StructurationTextWidget::setPropertyOnActionRedoOrUndo(QMap<QString, QString>& setMap, QList<ViewObj *> * strObjList, QList<ViewObj *> * defaultObjList)
{
	setTempleteProperty(setMap, strObjList, defaultObjList);
}
void StructurationTextWidget::addSetTempletePropertyAction(QMap<QString, QString>& beforMap, QMap<QString, QString>& map, QList<ViewObj *>& beforStrObjList, ViewObj* beforFocus, char beforFocusDir)
{
	OperateMgr * operateMgr  = getOperateMgr();
	operateMgr->createNewAction();
	SetTxtWidgetElementAction* setAction = new SetTxtWidgetElementAction(operateMgr->getCurStepMgr());
	ViewObj* focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus,dir);

	setAction->initFocus(beforFocus, focus, beforFocusDir, dir);
	setAction->initAction(this, beforMap, map);
	setAction->initAfterObjList(NULL, mStrObjList);
	setAction->initBeforObjList(NULL, &beforStrObjList);
	operateMgr->addToLocalAction(setAction);
}