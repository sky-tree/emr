#include "InputFilter.h"
//#include "BaseTemplete.h"
InputFilter::InputFilter(TextWidget * ownerTemplete, int classID) :BaseElement(classID)
, mOwnerTemplete(ownerTemplete)
{
	mStrMaxSize = INFINITE;
}

InputFilter::~InputFilter()
{
	if (mOwnerTemplete){

		mOwnerTemplete = NULL;
	}
}
/*
fliter 过滤字符串
IsInclude 是 过滤字符串以内的字符允许输入 ，否 过滤字符串以外的字符允许输入
*/
void InputFilter::setFilter(QString fliter, bool IsInclude){
	mFilterStr = fliter;
	mIsIncludeFilter = IsInclude;
}
bool InputFilter::isInFilter(QString str){
	int index = mFilterStr.indexOf(str);
	if (index>=0){
		if (mIsIncludeFilter){
			return true;
		}
		else{
			return false;
		}
		
	}
	else{
		if (mIsIncludeFilter){
			return false;
		}
		else{
			return true;
		}
	}
}
bool InputFilter::isInMaxSize(int size){
	if (mStrMaxSize == INFINITE){
		return true;
	}
	if (size>mStrMaxSize){
		return false;
	}
	else{
		return true;
	}
}
QString InputFilter::filt(QString inputStr, int currentSize, int insertIndex){
	int preSize = currentSize;
	int addSize = inputStr.length();
	int addNum = 1;
	QString returnStr;
	for (int i = 0; i < addSize; i++){
		if (!isInMaxSize(preSize + addNum)){
			break;
		}
		QString tempStr = inputStr.mid(i, 1);
		if (isInFilter(tempStr)){
			returnStr.push_back(tempStr);
			addNum++;
		}

	}
	return returnStr;
}
int InputFilter::getMaxSize(){
	return mStrMaxSize;
}

