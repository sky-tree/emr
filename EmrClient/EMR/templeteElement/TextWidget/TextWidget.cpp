#include "TextWidget.h"
//#include "TempleteInputManager.h"
#include "ViewFactory.h"
//#include "ViewParamFactory.h"
#include "qdebug.h"
#include "ViewObj.h"
//#include "DrawTxtParam.h"
#include "InputFactory.h"
#include "FlagParam.h"
#include "InputFilter.h"
#include "TempleteFactory.h"
#include "TextWidgetParam.h"
#include "OperateMgr.h"
#include "NumberInputFilter.h"
#include "TempContainerMgr.h"
#include "TempleteViewObj.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "ChoiceWidgetTemplete.h"
#include "TempleteTypeEnumeration.h"
#include "TextWidgetMouse.h"
#include "BaseSystem.h"
#include "TextWidgetKeyboard.h"
#include "StrContentData.h"
#include "StrConstants.h"
#include "BaseParagraph.h"
#include "WriterManager.h"
#include "SetTxtWidgetElementAction.h"
#define Show_ObjList_With_NoFlag(viewList) \
ViewObj* row;\
int viewIndex;\
getRowAndViewIndex(this, row, viewIndex);\
showObjListWithNoFlag(row, viewIndex, viewList, 0, viewList->size());
TextWidget::TextWidget(WriterManager * wMgr, int objId, QString codeId, bool isCreateStrTxtData)
: BaseTemplate(wMgr, ConstantsID::ClassId::TXT_WIDGET, objId, codeId)
, mFocusTxtData(NULL)
,mDefaultObjList(NULL)
, mStrObjList(NULL)
, mInputFilter(NULL)
{
	mIsShowed = false;
	//mTemptStatus = BaseConstants::ObjStatus::EDIT_STATUS;
	if (isCreateStrTxtData)
	{
		mFocusTxtData = new StrContentData(this, NULL, "", wMgr->getFocusTxtData());
	}
	mDefaultObjList = new  QList<ViewObj *>();
	mStrObjList = new  QList<ViewObj *>();	//initWidget();
	mDecimal = ".";
}
TextWidget::TextWidget(WriterManager * wMgr, int classId, int objId, QString codeId, bool isCreateStrTxtData)
: BaseTemplate(wMgr, classId, objId, codeId)
, mFocusTxtData(NULL)
, mDefaultObjList(NULL)
, mStrObjList(NULL)
, mInputFilter(NULL)
{
	mIsShowed = false;
	//mTemptStatus = BaseConstants::ObjStatus::EDIT_STATUS;
	if (isCreateStrTxtData)
	{
		mFocusTxtData = new StrContentData(this, NULL, "", wMgr->getFocusTxtData());
	}
	mDefaultObjList = new  QList<ViewObj *>();
	mStrObjList = new  QList<ViewObj *>();	//initWidget();
	mDecimal = ".";
}
TextWidget::TextWidget(WriterManager * wMgr, BaseTemplate * templeteParent, int classid, int objId, QString codeId, bool isCreateStrTxtData)
: BaseTemplate(wMgr, classid, objId, codeId)
, mFocusTxtData(NULL)
, mDefaultObjList(NULL)
, mStrObjList(NULL)
, mInputFilter(NULL)
{
	mTempParent = templeteParent;
	mIsShowed = false;
	mTemptStatus = BaseConstants::ObjStatus::EDIT_STATUS;
	if (isCreateStrTxtData)
	{
		mFocusTxtData = new StrContentData(this, NULL, templeteParent->getFocusStrData()->getString(), templeteParent->getFocusStrData());
	}
	mDefaultObjList = new  QList<ViewObj *>();
	mStrObjList = new  QList<ViewObj *>();	//initWidget();
	mDecimal = ".";
}
TextWidget::TextWidget(WriterManager * wMgr, TextWidget* otherTextWidget, bool canCopyChild) :BaseTemplate(wMgr, otherTextWidget, canCopyChild)
, mFocusTxtData(NULL)
, mDecimal(otherTextWidget->mDecimal)
, mInputFilter(NULL)
, mDefaultString(otherTextWidget->getDefaultString())
, mIsShowed(false)
, mStr(otherTextWidget->mStr)
, mTemptStatus(otherTextWidget->getTempleteStatus())
{
	mFocusTxtData = new StrContentData(this, NULL, otherTextWidget->getSelfFocusData()->getString(), otherTextWidget->getSelfFocusData());
	mDefaultObjList = new  QList<ViewObj *>();
	mStrObjList = new  QList<ViewObj *>();	//initWidget();

	/*generateDefaultObj();
	setStr(otherTextWidget->getString());
	if (mContentData)
	{
		delete mContentData;
		mContentData = TempleteFactory::createTextWidgetParam(this);
	}*/
}
TextWidget::TextWidget(WriterManager * wMgr, TextWidget* otherTextWidget) :BaseTemplate(wMgr, otherTextWidget, false)
, mFocusTxtData(NULL)
, mDecimal(otherTextWidget->mDecimal)
, mStr(otherTextWidget->mStr)
, mInputFilter(NULL)
, mDefaultString(otherTextWidget->getDefaultString())
, mIsShowed(false)
{
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(this);
	mDefaultObjList = new  QList<ViewObj *>();
	mStrObjList = new  QList<ViewObj *>();	//initWidget();
	mFocusTxtData = new StrContentData(this, NULL, otherTextWidget->getSelfFocusData()->getString(), otherTextWidget->getSelfFocusData());
	copyStrList(wMgr, otherTextWidget->mDefaultObjList, mDefaultObjList);
	copyStrList(wMgr, otherTextWidget->mStrObjList, mStrObjList);
	addAllStrObjToChild();
	//generateDefaultObj();
	//setStr(otherTextWidget->getString());
	if (mContentData)
	{
		delete mContentData;
		mContentData = TempleteFactory::createTextWidgetParam(this);
	}
	else{
		mContentData = TempleteFactory::createTextWidgetParam(this);
	}
	TextWidgetMouse * textMouse = InputFactory::createTextWidgetMouse(this);
	this->setMouse((BaseTempleteMouse*)textMouse);
	TextWidgetKeyboard* keyboard = InputFactory::createTextWidgetKeyboard(this);
	this->setKeyboard(keyboard);
}
void TextWidget::copyStrList(WriterManager * wMgr, QList<ViewObj *>* src, QList<ViewObj *>* dest)
{
	int defaultSize = src->size();
	TempleteViewObj * tempViewObj = NULL;
	for (int i = 0; i < defaultSize; i++)
	{
		tempViewObj = (TempleteViewObj*)src->at(i);
		tempViewObj = new TempleteViewObj(wMgr, tempViewObj);
		tempViewObj->setTempleteParent(this);
		dest->push_back(tempViewObj);

	}
}
TextWidget::~TextWidget()
{
	if (mInputFilter){
		delete mInputFilter;
		mInputFilter = NULL;
	}
	if (mDefaultObjList){
		int strSize = mStrObjList->size();
		if (strSize>0)
		{
		int	defaultShowSize = mDefaultObjList->size()-1;
			ViewObj * tempObj = NULL;
			for (int i = defaultShowSize; i >=0; i--)
			{
				tempObj = mDefaultObjList->at(i);
				//tempObj->setIsJudgeViewParenOnDecontruction(mIsJudgeViewParenOnDecontruction);
				delete tempObj;
				tempObj = NULL;
			}
			mDefaultObjList->clear();
		}
		delete mDefaultObjList;
		mDefaultObjList = NULL;
	}
	if (mStrObjList){
		mStrObjList->clear();
		delete mStrObjList;
		mStrObjList = NULL;
	}
	if (mFocusTxtData)
	{
		delete mFocusTxtData;
		mFocusTxtData = NULL;
	}
	
}

/*void TextWidget::setFocusFlagAtObj(ViewObj* focosObj)
{
	BaseWinData * winData = focosObj->getParentWindowData();
	winData->setPressStartOrEnd(focosObj, BaseConstants::Direction::LEFT, false);
}*/
int TextWidget::handleKeyPress(QString str, BaseElement * obj)
{
	int returnType = BaseConstants::NONE;
	if (mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS)
	{
		return 1;
	}
	int index = getAddIndex((ViewObj*)obj);
	if (index<0)
	{
		return returnType;
	}
	
	if (mTemptStatus == BaseConstants::ObjStatus::NONEDIT_STATUS )
	{
		return 1;
	}
	
	if (str == StrConstants::MyKeyBoard::BACK_SPACE || str == StrConstants::MyKeyBoard::DEL)
	{
		int start =0 ;
		int end = 0;
		if (str == StrConstants::MyKeyBoard::BACK_SPACE)
		{
			start = index - 1;
			end = start + 1;
		}
		else
		{
			start = index;
			end = start + 1;
		}
		if (start<0 || start >= mStrObjList->size())
		{
			return returnType;
		}
		ViewObj* row;
		ViewObj* temptObj;
		int viewIndex = -1;
		temptObj = mStrObjList->at(start);
		row = temptObj->getViewParent();
		viewIndex = temptObj->getTempIndex();
		
		//光标处理
		if (start - 1<0)
		{
			temptObj = mStrObjList->at(start);
			//BaseWinData * winData = temptObj->getParentWindowData();
			temptObj->setFocus(false);
			//winData->setPressStartOrEnd(temptObj, BaseConstants::Direction::LEFT, false);
		}
		else
		{
			temptObj = mStrObjList->at(start - 1);
			//BaseWinData * winData = temptObj->getParentWindowData();
			temptObj->setFocus(true);
			//winData->setPressStartOrEnd(temptObj, BaseConstants::Direction::RIGHT, false);
		}
		deleteChild(start,end-start,true);
		//deleteViewObjOnShow(getChildren(), start,end-start,true);
		deleteStrObj(start, end, true);
		//getChildrenMgr()->delChildren( start, end - start, true);
		//delChild(start, true);
		if (mStrObjList->size()==0)
		{
			//showObjList(row, viewIndex,mDefaultObjList,0,mDefaultObjList->size());
			//insertObjListToTemplete(row, viewIndex, mDefaultObjList, 0, mDefaultObjList->size());
			//showObjList( mDefaultObjList, 0, mDefaultObjList->size());
			addAllStrObjToChild();
			ViewObj *obj = mDefaultObjList->at(0);
			obj->setFocus(false);
		}
		else{
			
			
			
		}

		returnType = 1;
	}
	else{
		if (mInputFilter){
			str = mInputFilter->filt(str, mStr.size(), index);
		}
		if (str.length() == 0){
			return 1;
		}
		
		//showObjList(row, viewIndex, mStrObjList, index, endIndex);
		QList<BaseContainer *> addViewList;
			ViewObj* firtViewObj;
			//bool isNeedClearDefaultStrObj = false;
			
			if (mStrObjList->size() == 0)
			{
				firtViewObj = mDefaultObjList->at(0);
				firtViewObj->setFocus(false);
				//isNeedClearDefaultStrObj = true;
				deleteChild(0, mDefaultObjList->size(),true);
			
				
			}
			else
			{
				if (index == mStrObjList->size())
				{
					firtViewObj = mStrObjList->at(index - 1);
					//row = firtViewObj->getViewParent();
					//viewIndex = firtViewObj->getTempIndex() + 1;
				}
				else
				{
					firtViewObj = mStrObjList->at(index);
					//row = firtViewObj->getViewParent();
					//viewIndex = firtViewObj->getTempIndex();
				}
				firtViewObj->setFocus(false);
			}
			
			addStr(str, index);
			int endIndex = index + str.length();
			ViewObj * addObj = mStrObjList->at(endIndex - 1);
			addViewList.clear();
			int addSize = str.size();
			int startAdd = index;
			int endAdd = addSize + startAdd;
			for (; startAdd < endAdd; startAdd++)
			{
				addObj = mStrObjList->at(startAdd);
				addViewList.push_back(addObj);
			}
			addChildren(&addViewList, index,true);
			
			/*if (isNeedClearDefaultStrObj)
			{
				clearDefaultObjListFromShow();
			}*/
	
			addObj->setFocus(true);
			returnType = 1;
			replaceNeedReplaceList(0);
	}

	return returnType;
}
int TextWidget::handleChoiceTextWidgetKeyPress(QString str, BaseElement * obj)
{
	/*if (mWriterMgr->getFileType()==)
	{
		return 1;
	}*/
	//BaseWinData * winData = ((ViewObj *)obj)->getParentWindowData();
	int returnType = BaseConstants::NONE;
	if (mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS)
	{
		return 1;
	}
	ViewObj* selectObj = (ViewObj*)obj;
	char focousDir=0;
	int index = getChoiceAddIndex(selectObj, focousDir);
	if (index<0)
	{
		return returnType;
	}

	if (mTemptStatus == BaseConstants::ObjStatus::NONEDIT_STATUS)
	{
		return 1;
	}
	bool isBackSpace = false;
	if (str == StrConstants::MyKeyBoard::BACK_SPACE || str == StrConstants::MyKeyBoard::DEL)
	{
		ChoiceWidgetTemplete* parentTemplete = (ChoiceWidgetTemplete*)getTempParent();
		if (parentTemplete->getCanInputOnKeyBoard())
		{
			int start = 0;
			int end = 0;
			if (str == StrConstants::MyKeyBoard::BACK_SPACE)
			{
				start = index - 1;
				end = start + 1;
				isBackSpace = true;
			}
			else
			{
				start = index;
				end = start + 1;
			}
			if (start<0)
			{

				parentTemplete->deleteLastViewObj(this->getTempIndex());
				return 1;
			}

			//如果删除的是第0个对象。就需要判断一下当前选项是否还有显示的文字。
			bool hasHandled = false;
			QList<BaseContainer *> * mChildren = getChildren();
			if (start == 0)
			{
				if (mChildren->size() == 0)
				{

				}
				else
				{
					ViewObj * nextViewObj = getPreView();
					nextViewObj->setFocus(true);
				}
			}
			else
			{
				if (start == mChildren->size())
				{
					ChoiceWidgetTemplete* parentTemplete = (ChoiceWidgetTemplete*)getTempParent();
					parentTemplete->deleteNextViewObj(this->getTempIndex());

					return 1;
				}
				ViewObj * lastViewObj = (ViewObj*)mChildren->at(start - 1);
				lastViewObj->setFocus(true);
			}
			deleteChild(start, 1, true);
			ChoiceWidgetTemplete* parentTemplete = (ChoiceWidgetTemplete*)getTempParent();
			if (mChildren && (mChildren->size() == 0))
			{
				parentTemplete->deleteChild(this, true);
			}
			parentTemplete->checkCanShowTitle(isBackSpace); //是回退键，时光标设置到前面去
		}
		else
		{
			TextWidgetKeyboard::mooveFocus(str, selectObj, focousDir);
		}
		
		//delChild(start, true);
		returnType = 1;
	}
	else{
		ChoiceWidgetTemplete* parentTemplete = (ChoiceWidgetTemplete*)getTempParent();
		if (parentTemplete->getCanInputOnKeyBoard())
		{
			if (mInputFilter){
				str = mInputFilter->filt(str, mStr.size(), index);
			}
			if (str.length() == 0){
				return 1;
			}
			ViewObj* row;

			int viewIndex = -1;
			int totalChild = getChildren()->size();
			int addTempleteIndex = -1;
			if (index >= totalChild)
			{
				addTempleteIndex = totalChild - 1;
			}
			else{
				addTempleteIndex = index;
			}


			ViewObj *addObj = (ViewObj *)getChild(addTempleteIndex);
			row = addObj->getViewParent();
			viewIndex = addObj->getViewIndex();
			if (index >= totalChild)
			{
				viewIndex++;
			}
			generateStrObjAddToViewParent(str, row, viewIndex, index);
		}
		
		returnType = 1;
		replaceNeedReplaceList(0);
	}
	return returnType;
}
int TextWidget::handleKeyRelease(QString str, BaseElement * obj)
{
	/*int returnType = BaseConstants::NONE;
	if (mTemptStatus == BaseConstants::ObjStatus::NONEDIT_STATUS || mTemptStatus == BaseConstants::ObjStatus::NO_FOCOUS_INSIDE_STATUS){
		return 1;
	}
	return returnType;*/
	return 0;
} 
void TextWidget::initWidget( QString defaultString ){
	if (!mFocusTxtData){
		//if (mTemptStatus == BaseConstants::ObjStatus::NONEDIT_STATUS){
		//	mFocusTxtData = ViewParamFactory::createFocusTxtData();
		//}
		//else
		{
			mFocusTxtData = new StrContentData(this, NULL, NULL, BaseTemplate::getFocusStrData());
		}
	}
	
	mDefaultString = defaultString;
	generateDefaultObj();
	
}
void TextWidget::generateDefaultObj()
{
	if (!mFocusTxtData)
	{
		mFocusTxtData = new StrContentData(NULL, NULL, "", BaseSystem::getInstance()->getDefaultStrData());
	}
	if (mDefaultObjList->size() > 0)
	{
		mDefaultObjList->clear();
	}
	int size = mDefaultString.size();
	int generateNum = 0;
	if (size>0)
	{
		do{
			ViewObj * txt = ViewFactory::createTemplateTxt(mWriterMgr, this, mDefaultString.mid(generateNum, 1), mFocusTxtData);
			mDefaultObjList->push_back(txt);
			generateNum++;
		} while (generateNum<size);
	}
}
void TextWidget::generateStrObj(QString str, int index)
{
	if (!mFocusTxtData)
	{
		mFocusTxtData = new StrContentData(NULL, NULL, "", BaseSystem::getInstance()->getDefaultStrData());
	}
	
	int size = str.length();
	int startIndex = index;
	for (int i = 0; i < size; i++)
	{

		ViewObj * txt = ViewFactory::createTemplateTxt(mWriterMgr, this, str.mid(i, 1), mFocusTxtData);
		if (startIndex == mStrObjList->size()){
			mStrObjList->push_back(txt);
		}
		else{
			mStrObjList->insert(startIndex, txt);
		}
		startIndex++;
	}
}
void TextWidget::generateStrObjAddToViewParent(QString str, ViewObj* row, int viewIndex, int templeteIndex)
{
	int size = str.length();
	ViewObj * tempRow = row;
	ViewObj * txt;
	for (int i = 0; i < size; i++)
	{
		txt = ViewFactory::createTemplateTxt(mWriterMgr, this, str.mid(i, 1), mFocusTxtData);
		tempRow = txt->getViewParent();
		addChild(txt, templeteIndex,true);
		//viewIndex = txt->getViewIndexFromParent()+1;
		templeteIndex++;
	}
	txt->setFocus(true,false);
}
void TextWidget::addStr(QString str, int index)
{
//	QString filtStr = filtInput(str,index);
	int size = str.length();
	if (index == mStr.size()){
		mStr.push_back(str);
		}
		else{
			mStr.insert(index, str);
		}
		generateStrObj(str, index);
}
QString TextWidget::filtInput(QString str, int index){
	if (mInputFilter){
		return mInputFilter->filt(str,mStr.size(),index);
	}
	return str;
}
void TextWidget::deleteStrObj(int startIndex, int endIndex, bool isDeleteInMemory)
{
	int strSize= mStrObjList->size();
	if (startIndex>strSize){
		return;
	}
	if (endIndex>strSize){
		endIndex = strSize;
	}
	
	ViewObj* tempView;
	for (int i = startIndex; i < endIndex;i++)
	{
		tempView = mStrObjList->at(i);
		mStrObjList->removeAt(startIndex);
		if (isDeleteInMemory)
		{
			getChildrenMgr()->delChild(tempView,false);
			delete tempView;
		}
	}
	deleteStr(startIndex, endIndex);
	refreshChildrenIndex(0);
}
void TextWidget::deleteStr(int startIndex, int endIndex){
	mStr.remove(startIndex,endIndex-startIndex);
}
void TextWidget::clearDefaultObjListFromShow(){

	ViewObj * window = NULL;
	int size = mDefaultObjList->size();
	int startIndex =getChildren()->indexOf( mDefaultObjList->at(0));
	int endIndex = startIndex + size;
	QList<BaseContainer *> addViewList;
	
	ViewObj * txt;
	for (int i = 0; i < size; i++)
	{
		txt = mDefaultObjList->at(i);	
			bool needDelViewSide = true;
			if (txt->getViewParent() == NULL)
			{
				needDelViewSide = false;
			}
			else
			{
				addViewList.push_back(txt);
			}
	}
	deleteViewObjOnShow(&addViewList, 0, size, true);
	if (startIndex>=0)
	{
		getChildrenMgr()->delChildren(startIndex, endIndex, false);
	}
	refreshChildrenIndex(0);
}

void TextWidget::ShowObjList(bool isRefreshNow)
{
	if (mStrObjList->size()>0)
	{
		addStrObjShow(isRefreshNow);
	}
	else
	{
		addDefaultStrObjShow(isRefreshNow);
	}
}
void TextWidget::insertObjListToView(BaseParagraph* paragrapph, int viewIndex, QList<ViewObj *>* tempObjList, int start, int end)
{

}
void TextWidget::clearStrObjList( bool isDeleteInMemory)
{
	ViewObj * txt;
	int optionListSize = mStrObjList->size();
	for (int m = 0; m < optionListSize; m++){
		txt = mStrObjList->at(m);
		/*ViewObj * viewParent = txt->getViewParent();
		if (viewParent){
			viewParent->delChild(txt);
		}*/
		if (mIsShowed){
			//newDelChild(txt, true, true, BaseConstants::ReplaceType::REPLACE_ONLY_SELF, true, true);
		}
		if (isDeleteInMemory)
		{
			delete txt;
		}
		txt = NULL;
	}
	mStrObjList->clear();
}
void TextWidget::addDefaultStrObjShow(bool isRefreshNow)
{
	addStrObjToShow(mDefaultObjList,isRefreshNow);
}
void TextWidget::addStrObjShow(bool isRefreshNow)
{
	addStrObjToShow(mStrObjList, isRefreshNow);
}
void TextWidget::addStrObjToShow(QList<ViewObj *>* tempObjList, bool isRefreshNow)
{
	QList<BaseContainer *>  addList;
	int size = tempObjList->size();
	ViewObj* addObj;
	for (int i = 0; i < size; i++)
	{
		addObj = tempObjList->at(i);
		addList.push_back(addObj);
	}
	addChildren(&addList, 0, true, isRefreshNow);
}
void TextWidget::addWidgetToRow(bool isFresh)
{
	
}
void TextWidget::addAllChildToBufList(QList<BaseContainer *> * children, int start, int num, QList<BaseContainer *> * destViewList)
{
	
}
void TextWidget::setOnlyStr(QString str)
{
	mStr= str;
}
void TextWidget::setStr(QString str, bool isAddToChildrenList)
{
	if (mInputFilter){
		str = mInputFilter->filt(str, mStr.length(), 0);
	}
	
	if (!mIsShowed){
		mStr.clear();
		clearStrObjList();
		addStr(str, 0);
	}
	else {
		mStr.clear();
		clearStrObjList();
		addStr(str, 0);
	}
	if (isAddToChildrenList)
	{
		addAllStrObjToChild();
	}
}
void TextWidget::setStrOnShowTime(QString str)
{
	deleteChild(0,-1,true);
	setStr(str,false);
	ShowObjList();
}
void TextWidget::setDefaultStrOnShowTime(QString str)
{
	deleteChild(0, -1, true);
	setStr(str, false);
	ShowObjList();
}
void TextWidget::setDefaultStr(QString str, bool isAddToChildrenList)
{
	mDefaultString.clear();
	mDefaultString = str;
	if (!mIsShowed){
		generateDefaultObj();
	}
	else {
		if (mStrObjList->size()>0)
		{

		}
		else{
			clearDefaultObjListFromShow();
		}
		generateDefaultObj();
	}
	if (isAddToChildrenList)
	{
		addAllStrObjToChild();
	}
}
void TextWidget::setStrAndGenatateStrObj(QString str, bool isDeleteMemory)
{
	if (mInputFilter){
		str = mInputFilter->filt(str, mStr.length(), 0);
	}
	mStr.clear();
	clearStrObjList(isDeleteMemory);
	addStr(str, 0);
}
ViewObj * TextWidget::getShowObjOnIndex(int index){
	if (index==0){
		if (mStrObjList->size() == 0){
			return mDefaultObjList->at(0);
		}
		else{
			return mStrObjList->at(0);
		}
	}
	else{
		return mStrObjList->at(index);
	}
	
}
int TextWidget::getAddIndex(ViewObj *focusObj){
	
	ViewObj * focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus,dir);
	if (focus != focusObj){
		return -1;
	}
	int disIdx = 0;
	if (dir == BaseConstants::Direction::RIGHT)
		disIdx = 1;
	int idx = focus->getTempIndex() + disIdx;
	int sizeStr = mStrObjList->size();
	if (idx >= sizeStr)
	{
		idx = sizeStr;
	}
	return idx;
}
int TextWidget::getChoiceAddIndex(ViewObj *focusObj, char& returnDir)
{
	
	ViewObj * focus = NULL;
	char dir = 0;
	getFocusObjAndDir(focus,dir);
	returnDir = dir;
	if (focus != focusObj){
		return -1;
	}
	int disIdx = 0;
	if (dir == BaseConstants::Direction::RIGHT)
		disIdx = 1;
	int idx = focus->getTempIndex() + disIdx;
	int sizeStr = mStrObjList->size();
	return idx;
}
void TextWidget::setInputFilter(InputFilter * inputFilter){
	mInputFilter = inputFilter;
}
int TextWidget::haveDecimalNumber(){
	int indexDecimal = mStr.indexOf(mDecimal);
	if (indexDecimal<0){
		return -1;
	}
	int maxIndex = mStr.size() - 1;
	return maxIndex - indexDecimal;
}
void TextWidget::setTempleteStatus(int status){
	mTemptStatus = status;
}
int TextWidget::getTempleteStatus(){
	return mTemptStatus;
}
/*
设置颜色要在initWidget（）之前调用
*/
void TextWidget::setTxtData(StrContentData * focusTxtData)
{
	mFocusTxtData = focusTxtData;
}
void TextWidget::setTxtDataWithValue(StrContentData * focusTxtData, bool needRefreshWH)
{
	mFocusTxtData->refreshTxtParamBySourceParam(focusTxtData, needRefreshWH);
}
void TextWidget:: setTxtPointSize(int size)
{
	mFocusTxtData->setPointSize(size);
}
StrContentData * TextWidget::getSelfFocusData()
{
	return mFocusTxtData;
}
int TextWidget::getTemptStatus(){
	return mTemptStatus;
}
QString TextWidget::getString(){
	return mStr;
}
QString TextWidget::getDefaultString(){
	return mDefaultString;
}
void TextWidget::addStrObjListToStringObjList(QList<ViewObj *> * strObjList){
	int size = strObjList->size();
	ViewObj* tempObj;
	for (int i = 0; i < size; i++){
		tempObj = strObjList->at(i);
		mStrObjList->push_back(tempObj);
	}
}
void TextWidget::addStrObjListToDefaultStringObjList(QList<ViewObj *> * strObjList)
{
	if (mDefaultObjList->size() > 0)
	{
		mDefaultObjList->clear();
	}
	int size = strObjList->size();
	ViewObj* tempObj;
	for (int i = 0; i < size; i++){
		tempObj = strObjList->at(i);
		mDefaultObjList->push_back(tempObj);
	}
}
void TextWidget::setNumberFilter(int size, int decimalPointLenght){
	NumberInputFilter * numberFitlter = (NumberInputFilter*)mInputFilter;
	numberFitlter->initFilter(size, decimalPointLenght);
}
void TextWidget::setTitle(QString str, bool isAddToChildrenList)
{

	setStr(str, isAddToChildrenList);
}

QString TextWidget::getTitle()
{
	return mStr;
}
void TextWidget::checkStrIsAdaptStrListAndChildren(bool isToShow)
{
	if (mStr.compare(getStrFromStrObjList()) != 0)
	{
		setStrAndGenatateStrObj(mStr);
	}
	bool isSameStrObjList = SetTxtWidgetElementAction::isSameObjInTwoList(getChildren(), mStrObjList);
	if (!isSameStrObjList)
	{
		QList<BaseContainer *> * childrenList = getChildren();
		if (childrenList)
		{
			bool isNeedDeleteFormShow = false;
			int childSize = childrenList->size();
			if (childSize>0)
			{
				bool isShow = BaseContainer::isShow(this);
				//必须判断是否在显示
				if (isShow)
				{
					isNeedDeleteFormShow = true;
				}
			}
			if (isNeedDeleteFormShow)
			{
				setFirstChildViewFocus();
				deleteChild(0, -1, false, false);
			}
			else
			{
				childrenList->clear();
			}
		}
		
		if (isToShow)
		{
			addStrObjShow(false);
		}
		else
		{
			addAllStrObjToChild();
		}
		
	}
}
QString TextWidget::getStrFromStrObjList()
{
	int size = mStrObjList->size();
	QString showStr;
	ViewObj * tempObj;
	StrContentData * viewParm;
	for (int i = 0; i < size; i++)
	{
		tempObj = mStrObjList->at(i);
		viewParm = (StrContentData *)tempObj->getContentData();
		showStr.push_back(viewParm->getString());
	}
	return showStr;
}
bool TextWidget::setTempleteProperty(QMap<QString, QString>& setMap)
{
	QString content = setMap.value(TempleteTypeEnumeration::CONTENT);
	if (content.compare(getString()) == 0)
	{
		return false;
	}
	setFirstChildViewFocus();
	setStrOnShowTime(content);
	ViewObj* lastObj = getLastChildView();
	if (lastObj)
	{
		lastObj->setFocus(true, false);
	}
	setCodeIdToTemplete(setMap);
	return true;
}
void TextWidget::setPropertyOnActionRedoOrUndo(QMap<QString, QString>& setMap, QList<ViewObj *> * strObjList, QList<ViewObj *> * defaultObjList)
{
	setCodeIdToTemplete(setMap);
	QString content = setMap.value(TempleteTypeEnumeration::CONTENT);
	mStr = content;
	mNameStr = setMap.value(TempleteTypeEnumeration::NAME);
	copyObjListAndReplace(strObjList,defaultObjList);
}
void TextWidget::copyObjListAndReplace(QList<ViewObj *> * strObjList, QList<ViewObj *> * defaultObjList)
{
	bool isNeedReplace = false;
	bool isSameStrObjList = SetTxtWidgetElementAction::isSameObjInList(strObjList, mStrObjList);
	if (!isSameStrObjList)
	{
		if (mStrObjList->size() == 0)
		{

		}
		else
		{
			deleteChild(0, -1, true);
			SetTxtWidgetElementAction::copyObjListToObjList(strObjList, mStrObjList, true);
			isNeedReplace = true;
		}

	}

	bool isSameDefaultObjList = SetTxtWidgetElementAction::isSameObjInList(defaultObjList, mDefaultObjList);
	if (!isSameDefaultObjList)
	{
		if (mDefaultObjList->size() == 0)
		{

		}
		else
		{
			
			if (mStrObjList->size() == 0)
			{
				deleteChild(0, -1, true);
				isNeedReplace = true;
			}
			SetTxtWidgetElementAction::copyObjListToObjList(defaultObjList, mDefaultObjList, true);
		}

	}
	if (isNeedReplace)
	{
		ShowObjList();
		
		//addTxtLayerAndReplaceNeedReplaceList(NULL);
	}
}
bool TextWidget::setDrawTxtParam(StrContentData* focusData)
{
	if (BaseTemplate::isSetFlowType(focusData))
	{
		return true;
	}
	int size = mStrObjList->size();
	StrContentData* preData = new StrContentData(NULL, NULL);
	/*if (size==0)
	{
		if (focusData->mOprateType==SetTxtType::UnderLineType)
		{
			mFocusTxtData->refreshTxtParamOnSetType(focusData);
		}
	}
	else*/
	{
		
		preData->refreshTxtParamBySourceParam(mFocusTxtData, false);
		preData->setOperateType(focusData->mOprateType);
		mFocusTxtData->refreshTxtParamOnSetType(focusData);
	}
	bool isSuccess = true;
	ViewObj* viewParent = NULL;
	for (int i = 0; i < size; i++)
	{
		BaseContainer * obj = mStrObjList->at(i);
		if (obj->isView())
		{
			//viewParent = obj->getViewParent();
			/*if (viewParent)
			{
				viewParent->setPageRepaint();
			}*/
			/*if (((ViewObj *)obj)->isTxt())
			{
				StrContentData * txtParam = (StrContentData *)obj->getContentData();
				txtParam->refreshTxtParamOnSetType(mFocusTxtData);
				txtParam->refreshTxtWH();
			}*/
			if (!obj->setDrawTxtParam(mFocusTxtData))
			{
				isSuccess =  false;
				break;
			}
		}
	}
	if (isSuccess)
	{
		if (size == 0)
		{
			size = mDefaultObjList->size();
			for (int i = 0; i < size; i++)
			{
				BaseContainer * obj = mDefaultObjList->at(i);
				if (obj->isView())
				{
					//viewParent = obj->getViewParent();
					/*if (viewParent)
					{
					viewParent->setPageRepaint();
					}*/
					/*if (((ViewObj *)obj)->isTxt())
					{
					StrContentData * txtParam = (StrContentData *)obj->getContentData();
					txtParam->refreshTxtParamOnSetType(mFocusTxtData);
					txtParam->refreshTxtWH();
					}*/
					if (!obj->setDrawTxtParam(mFocusTxtData))
					{
						isSuccess = false;
						break;
					}
				}
			}
		}
	}
	if (isSuccess)
	{
		delete preData;
		preData = NULL;
		return true;
	}
	else
	{
		mFocusTxtData->refreshTxtParamOnSetType(preData);
		delete preData;
		preData = NULL;
		return false;
	}

	//replaceNeedReplaceList();
}
void TextWidget::getWillShowObjList(QList<ViewObj *> * container)
{
	QList<ViewObj *> * tempList;
	if (mStrObjList->size()>0)
	{
		tempList = mStrObjList;
	}
	else
	{
		tempList = mDefaultObjList;
	}
	ViewObj * tempView;
	for (int i = 0; i < tempList->size();i++)
	{
		tempView = tempList->at(i);
		container->append(tempView);
	}
}
void TextWidget::addSetTempletePropertyAction(QMap<QString, QString>& beforMap, QMap<QString, QString>& map, QList<ViewObj *> * beforDefaultObjList, QList<ViewObj *> * beforStrObjList, ViewObj* beforFocus, char beforFocusDir)
{
	OperateMgr * operateMgr = this->getOperateMgr();
	operateMgr->createNewAction();
	ViewObj* afterFocus = NULL;
	char afterDir = 0;
	operateMgr->getFocusAndDirection(afterFocus, afterDir);
	SetTxtWidgetElementAction* setAction = new SetTxtWidgetElementAction(operateMgr->getCurStepMgr());

	setAction->initFocus(beforFocus, afterFocus, beforFocusDir, afterDir);
		setAction->initAction(this, beforMap, map);
		setAction->initAfterObjList(mDefaultObjList, mStrObjList);
		setAction->initBeforObjList(beforDefaultObjList, beforStrObjList);
		operateMgr->addToLocalAction(setAction);
}
void TextWidget::addAllStrObjToChild()
{
	BaseChildrenManager * tempMgr = (BaseChildrenManager *)getChildrenMgr();
	tempMgr->delChildren(0, -1, true);
	int strSize = mStrObjList->size();
	if (strSize > 0)
	{
		ViewObj* tempObj;
		for (int i = 0; i < strSize; i++)
		{
			tempObj = mStrObjList->at(i);
			tempMgr->addChild(tempObj, -1, false);
		}
		refreshChildrenIndex(0);
		//tempMgr->addChildren(mStrObjList, 0, size, tempIdx, true);
	}
	else
	{
		int strSize = mDefaultObjList->size();
		if (strSize > 0)
		{
			ViewObj* tempObj;
			for (int i = 0; i < strSize; i++)
			{
				tempObj = mDefaultObjList->at(i);
				tempMgr->addChild(tempObj, -1, false);
			}
			refreshChildrenIndex(0);
		}
	}
}
#define MACRO_IS_CONTINUE \
	int objId = getObjID();\
bool canNoticeParent = false;\
if (objId == ConstantsID::ContainerId::CHOICE_TEXT_TEMPLETE_OBJ)\
{\
	canNoticeParent = true;\
}\
else if (objId == ConstantsID::ContainerId::MACRO_TEXT_TEMPLETE_OBJ)\
{\
	canNoticeParent = true; \
}\
else if (objId == ConstantsID::ContainerId::BRAKETWIDGET_TEMPLETE)\
{\
canNoticeParent = true; \
}\
else if (objId == ConstantsID::ContainerId::UNFOLD_CHOICE_TEXT_TEMPLETE_OBJ)\
{\
	canNoticeParent = true; \
}\
else if (objId == ConstantsID::ContainerId::UNFOLD_CHOICE_TITLE_TEMPLETE_OBJ)\
{\
	canNoticeParent = true; \
}\
if (!canNoticeParent)\
{\
	return returnType;\
}
int TextWidget::mousePress(int type, int x, int y, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		returnType = mInputMouse->mousePress(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	MACRO_IS_CONTINUE
	if (mTempParent)
	{
		returnType = ((BaseTemplate*)mTempParent)->mousePress(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	return returnType;
}
int TextWidget::mouseRelease(int type, int x, int y, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		returnType = mInputMouse->mouseRelease(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	MACRO_IS_CONTINUE
	if (mTempParent)
	{
		returnType = ((BaseTemplate*)mTempParent)->mouseRelease(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	return returnType;
}
int TextWidget::mouseMove(int type, int x, int y, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		returnType = mInputMouse->mouseMove(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	MACRO_IS_CONTINUE
	if (mTempParent)
	{
		returnType = ((BaseTemplate*)mTempParent)->mouseMove(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	return returnType;
}
int TextWidget::mouseDrag(int type, int x, int y, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		returnType = mInputMouse->mouseDrag(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	MACRO_IS_CONTINUE
	if (mTempParent)
	{
		returnType = ((BaseTemplate*)mTempParent)->mouseDrag(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	return returnType;
}
int TextWidget::mouseDoublePress(int type, int x, int y, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		returnType = mInputMouse->mouseDoublePress(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	MACRO_IS_CONTINUE
	if (mTempParent)
	{
		returnType = ((BaseTemplate*)mTempParent)->mouseDoublePress(type, x, y, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	return returnType;
}
int TextWidget::mouseScroll(int angle, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputMouse)
	{
		returnType = mInputMouse->mouseScroll(angle, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	MACRO_IS_CONTINUE
	if (mTempParent)
	{
		returnType = ((BaseTemplate*)mTempParent)->mouseScroll(angle, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	return returnType;
}
int TextWidget::keyPress(QString str, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputKeyboard)
	{
		returnType = mInputKeyboard->keyPress(str, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	MACRO_IS_CONTINUE
	if (mTempParent)
	{
		returnType = ((BaseTemplate*)mTempParent)->keyPress(str, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	return returnType;
}
int TextWidget::keyRelease(QString str, BaseContainer  * obj)
{
	int returnType = BaseConstants::NONE;
	if (mInputKeyboard)
	{
		returnType = mInputKeyboard->keyRelease(str, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	MACRO_IS_CONTINUE
	if (mTempParent)
	{
		returnType = ((BaseTemplate*)mTempParent)->keyRelease(str, obj);
		if (returnType != BaseConstants::NONE)
		{
			return returnType;
		}
	}
	return returnType;
}
QString TextWidget::getElementName()
{
	return mNameStr;
}
void TextWidget::SetElementName(QString name)
{
	mNameStr = name;
}