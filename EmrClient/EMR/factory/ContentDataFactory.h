#ifndef _CONTENT_DATA_FACTORY_H_
#define _CONTENT_DATA_FACTORY_H_


#include <QString>

class WriterManager;
class BaseContainer;
class BaseContentData;
class StrContentData;
class ParagraphData;
class CodeParams;
class BaseIOTools;

class ContentDataFactory
{
public:
	ContentDataFactory();
	~ContentDataFactory();

public:
	static void loadObjID(BaseIOTools * iOTools, QString version, int &classID, int &userID, int &objID, int &chooseObjID);
	static BaseContainer * loadData(WriterManager * mgr, BaseIOTools * iOTools, BaseContainer * tempParent, char addChildTypeAndAddToParent = 1);
	static CodeParams * loadCodeParams(BaseIOTools * iOTools, BaseContentData * contentData);

	static ParagraphData * getParagraphDataCopy(ParagraphData * source);

	static ParagraphData * createDefaultParagraphData();

	static StrContentData * createDefaultTxtData();
	static bool isSaveUnfoldChoicePaddingLeft;
	
};
#endif
