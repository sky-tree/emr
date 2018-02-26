#ifndef TEMPLETE_FACTORY_H_
#define TEMPLETE_FACTORY_H_


#include "qstring.h"

class BaseIOTools;
class BaseContainer;
class BaseTemplate;
class BaseParagraph;
class WriterManager;
class ParagraphManager;
class ParagraphData;
class TextWidget;
class ViewObj;
class StructurationTextWidget;
class StructurationTextParam;
class TextWidgetParam;
class BracketWidget;
class MenstruationTemplete;
class MenStruationDataParam;
class MacroTemplete;
class MacroTempleteParam;
class ChoiceWidgetTemplete;
class ChoiceWidgetTempleteParam;
class ExcelTemplete;
class CodeParams;
class ChoiceItemTextWidget;
class UnfoldChoiceWidgetTemplete;
class UnfoldChoiceWidgetTempleteParam;
class TempleteFactory
{
public:
	TempleteFactory();
	~TempleteFactory();

public:
	static BaseTemplate * createRootTemp(WriterManager * mgr, BaseIOTools * ioTools);
	static BaseParagraph * createParagraph(WriterManager * mgr, ParagraphManager * paragraphMgr, ParagraphData * data);

	static BaseTemplate * createNode(WriterManager * mgr, ParagraphManager * paragraphMgr
		, CodeParams * nodeParams, CodeParams * titleParams);

	static TextWidget * createNoEditTextWidgetTemplete(BaseContainer * templeteParent, ViewObj * viewParent, int chooseID
		, bool isCreateFromLoad, int objId, QString defaultStr, bool isCreateStrTxtData = false);
	static StructurationTextWidget * createStructurationTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad);
	static BracketWidget * createInputTextTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad);
	static MenstruationTemplete * createMenstruationTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad);
	static MacroTemplete * createMacroTemplete(BaseTemplate * templeteParent, int userID, bool isCreateFromLoad);
	static TextWidget * createMacroTextWidgetTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad);
	static BracketWidget * createBracketTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad);
	static ChoiceWidgetTemplete * createChoiceWidgetTemplete(BaseTemplate * templeteParent, char widgetType, bool isCreateFromLoad);
	static TextWidget * createChoiceTextWidgetTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad);
	static TextWidget * createTextWidgetTemplete(BaseTemplate * templeteParent, int objID,bool isCreateStrData= false);
	static ExcelTemplete * createExcelTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad);
	static BaseTemplate* createExcelCellTemplate(BaseTemplate * templeteParent);
	static ChoiceItemTextWidget* crateUnfoldChoiceItemTextWidget(BaseTemplate * templeteParent, char type,int paddingLeft, bool isCreateFromLoad);
	static UnfoldChoiceWidgetTemplete * createUnfoldChoiceWidget(BaseTemplate * templeteParent, char type, bool isCreateFromLoad);

	static MenStruationDataParam * createMenstruationParam(BaseTemplate * templeteOwner);
	static StructurationTextParam *createStructurationParam(BaseTemplate * templeteOwner);
	static TextWidgetParam * createTextWidgetParam(BaseTemplate * templeteOwner);
	static MacroTempleteParam * createMacronParam(BaseTemplate * templeteOwner);
	static ChoiceWidgetTempleteParam * createChoiceWidgetParam(BaseTemplate * templeteOwner, CodeParams * codeParams);
	static UnfoldChoiceWidgetTempleteParam * createUnfoldChoiceWidgetParam(BaseTemplate * templeteOwner, CodeParams * codeParams);

private:
	static BaseTemplate * createPartTemp(WriterManager * mgr, int objId);
	// 创建普通模板容器：页眉、页脚下一级子对象
	static BaseTemplate * createTempContainer(WriterManager * mgr, int objId, ParagraphManager * paragraphMgr);
};

#endif
