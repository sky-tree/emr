#include "NumberInputFilter.h"
#include "TextWidget.h"

NumberInputFilter::NumberInputFilter(TextWidget *ownerTemplete, int classID) :InputFilter(ownerTemplete, classID)
{
	mStrMaxSize = INFINITE;
	mDecimalPointLenght = INFINITE;
	mFilterStr = "1234567890.";
	mDecimal = ".";
}


NumberInputFilter::~NumberInputFilter()
{
}
void NumberInputFilter :: initFilter(int maxLeght, int decimalPointLenght){
	mStrMaxSize = maxLeght;
	mDecimalPointLenght = decimalPointLenght;
}
QString NumberInputFilter::filt(QString inputStr, int currentSize,  int insertIndex){
	int preSize = currentSize;
	int addSize = inputStr.length();
	int addNum = 1;
	QString returnStr;
	int decimalNum = mOwnerTemplete->haveDecimalNumber();
	int decimalIndex = preSize-1 - decimalNum;
	for (int i = 0; i < addSize; i++){
		if (!isInMaxSize(preSize + addNum)){
			break;
		}
		QString tempStr = inputStr.mid(i, 1);
		if (mDecimal.compare(tempStr)==0){
			if (decimalNum>=0){
				continue;
			}
		}

			if (isInFilter(tempStr)){
				if (insertIndex>decimalIndex){
					if (mDecimalPointLenght==INFINITE){
						decimalNum++;
					}
					else{
						if (decimalNum >= mDecimalPointLenght){
							continue;
						}
						else{
							decimalNum++;
						}
					}
					
				}
				returnStr.push_back(tempStr);
				addNum++;
			}


	}
	return returnStr;
}
int NumberInputFilter::getDecimalPointLenght()
{
	return mDecimalPointLenght;
}
