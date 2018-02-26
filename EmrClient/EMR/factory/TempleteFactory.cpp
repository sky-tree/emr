#include "TempleteFactory.h"




#include "qdebug.h"

#include "BaseIOTools.h"
#include "WriterManager.h"
#include "OperateMgr.h"

#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseTemplate.h"
#include "BaseParagraph.h"



#include "TempParagraphMgrData.h"

#include "ParagraphData.h"
#include "CodeTempData.h"
#include "NodeParams.h"

#include "BaseChildrenManager.h"
#include "TempContainerMgr.h"
#include "NodeMgr.h"
#include "NodeChildrenMgr.h"
#include "ParagraphManager.h"

#include "NoCopyTextWidget.h"

#include "InputFactory.h"
#include "ViewFactory.h"
#include "TempleteFactory.h"
#include "ContentDataFactory.h"
#include "ConstantsID.h"

#include "TextWidgetMouse.h"


#include "TextWidget.h"
#include "TextWidgetKeyboard.h"
#include "StructurationTextWidget.h"
#include "TextWidgetParam.h"
#include "StructurationTextParam.h"
#include "BracketWidgetKeyboard.h"
#include "MenstruationTemplete.h"
#include "MenStruationDataParam.h"
#include "MacroTempleteParam.h"
#include "MacroTemplete.h"
#include "ChoiceWidgetTemplete.h"
#include "ChoiceWidgetTempleteParam.h"
#include "ExcelTemplete.h"
#include "ExcelTempleteParamData.h"
#include "ExcelMouseInputManager.h"
#include "ExcelKeyboard.h"
#include "ChoiceItemTextWidget.h"
#include "UnfoldChoiceWidgetTemplete.h"
#include "UnfoldChoiceWidgetTempleteParam.h"
TempleteFactory::TempleteFactory()
{
}

TempleteFactory::~TempleteFactory()
{
}

BaseTemplate * TempleteFactory::createRootTemp(WriterManager * mgr, BaseIOTools * ioTools)
{
	BaseTemplate * rootTemp = new BaseTemplate(mgr, ConstantsID::ContainerId::ROOT_TEMP);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(rootTemp);
	//rootTemp->initChildrenMgr(childrenMgr);
	BaseContainer * title = NULL;
	BaseContainer * bottom = NULL;
	BaseContainer * content = NULL;
	if (ioTools)
	{
		title = ContentDataFactory::loadData(mgr, ioTools, rootTemp);
		bottom = ContentDataFactory::loadData(mgr, ioTools, rootTemp);
		content = ContentDataFactory::loadData(mgr, ioTools, rootTemp);
		//content = createPartTemp(mgr, ConstantsID::ContainerId::CONTENT_TEMP);
		//childrenMgr->pushChild(content, true);
	}
	else
	{
		title = createPartTemp(mgr, ConstantsID::ContainerId::TITLE_TEMP);
		bottom = createPartTemp(mgr, ConstantsID::ContainerId::BOTTOM_TEMP);
		content = createPartTemp(mgr, ConstantsID::ContainerId::CONTENT_TEMP);
		childrenMgr->pushChild(title, true);
		childrenMgr->pushChild(bottom, true);
		childrenMgr->pushChild(content, true);
	}
	return rootTemp;
}
BaseTemplate * TempleteFactory::createPartTemp(WriterManager * mgr, int objId)
{
	BaseTemplate * temp = new BaseTemplate(mgr, objId);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(temp);
	CodeParams * p = NULL;
	TempParagraphMgrData * data = new TempParagraphMgrData(temp, ConstantsID::ClassId::TEMP_PARAGRAPH_MGR_DATA, NULL);
	temp->initContentData(data);
	ParagraphManager * paragraphMgr = data->getParagraphMgr();
	BaseTemplate * child = NULL;
	if (objId == ConstantsID::ContainerId::TITLE_TEMP)
	{
		child = createTempContainer(mgr, ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER, paragraphMgr);
	}
	else if (objId == ConstantsID::ContainerId::BOTTOM_TEMP)
	{
		child = createTempContainer(mgr, ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER, paragraphMgr);
	}
	else 
	{
		child = createTempContainer(mgr, ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER, paragraphMgr);
	}
	childrenMgr->pushChild(child, true);
	return temp;
}
BaseTemplate * TempleteFactory::createTempContainer(WriterManager * mgr, int objId, ParagraphManager * paragraphMgr)
{
	BaseTemplate * temp = new BaseTemplate(mgr, objId);
	CodeTempData * data = new CodeTempData(temp, ConstantsID::ClassId::CODE_TEMP_DATA, NULL);
	temp->initContentData(data);
	NodeMgr * childrenMgr = new NodeMgr(temp, paragraphMgr);
	return temp;
}
BaseTemplate * TempleteFactory::createNode(WriterManager * mgr, ParagraphManager * paragraphMgr, CodeParams * nodeParams, CodeParams * titleParams)
{
	BaseTemplate * temp = new BaseTemplate(mgr, ConstantsID::ContainerId::NODE_CONTAINER);
	CodeTempData * data = new CodeTempData(temp, ConstantsID::ClassId::CODE_TEMP_DATA, NULL);
	data->initCodeParams(nodeParams);
	temp->initContentData(data);

	NodeChildrenMgr * childrenMgr = new NodeChildrenMgr(temp, paragraphMgr);
	if (titleParams)
	{
		BaseContainer * title = mgr->getOperateMgr()->createNodeTile(titleParams);
		childrenMgr->pushChild(title, true);
	}
	ViewObj * enter = ViewFactory::createEnter(mgr, -1);
	childrenMgr->pushChild(enter, true);
	return temp;
}
BaseParagraph * TempleteFactory::createParagraph(WriterManager * mgr, ParagraphManager * paragraphMgr, ParagraphData * source)
{
	BaseParagraph * paragraph = new BaseParagraph(mgr, paragraphMgr, ConstantsID::ContainerId::NORMAL_PARAGRAPH);
	ParagraphData * data = new ParagraphData(paragraph, source);
	paragraph->initContentData(data);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(paragraph);
	//paragraph->initChildrenMgr(childrenMgr);
	if (paragraphMgr)
	{
		paragraph->setParent(paragraphMgr->getTempParent());
	}
	

	return paragraph;
}
TextWidget * TempleteFactory::createNoEditTextWidgetTemplete(BaseContainer * templeteParent, ViewObj * viewParent, int chooseID
	, bool isCreateFromLoad, int objId, QString defaultStr, bool isCreateStrTxtData)
{
	TextWidget * txt = new NoCopyTextWidget(templeteParent->getWriterMgr(), templeteParent, objId, isCreateStrTxtData);
	if (chooseID >= 0)
		txt->setChooseObjId(chooseID);
	TextWidgetMouse * textMouse = InputFactory::createTextWidgetMouse(txt);
	txt->setMouse((BaseTempleteMouse*)textMouse);
	TextWidgetKeyboard* keyboard = InputFactory::createTextWidgetKeyboard(txt);
	txt->setKeyboard(keyboard);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(txt);
	//excel->initChildrenMgr(childrenMgr);
	if (!isCreateFromLoad)
	{
		txt->setDefaultStr(defaultStr);
		txt->setStr(defaultStr);
		TextWidgetParam * textWidgetParam = TempleteFactory::createTextWidgetParam(txt);
		txt->initContentData(textWidgetParam);
	}
	txt->setTempleteStatus(BaseConstants::ObjStatus::NONEDIT_STATUS);
	return txt;
}
StructurationTextWidget * TempleteFactory::createStructurationTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad)
{
	StructurationTextWidget * textWidget = new StructurationTextWidget(templeteParent->getWriterMgr(), templeteParent
		, ConstantsID::ContainerId::STRUCTURATION_TEXT_TEMPLETE_OBJ);
	//textWidget->getOperateMgr()->setCanAddObjParentId(templeteParent->getObjId());//用于拖拽加元素时使用
	TextWidgetMouse * textMouse = InputFactory::createTextWidgetMouse(textWidget);
	textWidget->setMouse((BaseTempleteMouse*)textMouse);
	TextWidgetKeyboard* keyboard = InputFactory::createTextWidgetKeyboard(textWidget);
	textWidget->setKeyboard(keyboard);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(textWidget);
	//textWidget->initChildrenMgr(childrenMgr);
	if (!isCreateFromLoad){
		StructurationTextParam * textWidgetParam = TempleteFactory::createStructurationParam(textWidget);
		textWidget->initContentData(textWidgetParam);
		textWidget->setTitle("格式化录入");
	}

	return textWidget;
}
BracketWidget * TempleteFactory::createInputTextTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad)
{
	BracketWidget * textWidget = new BracketWidget(templeteParent->getWriterMgr(), templeteParent
		, ConstantsID::ContainerId::INPUT_TEXT_TEMPLETE);
	TextWidgetMouse * textMouse = InputFactory::createTextWidgetMouse(textWidget);
	textWidget->setMouse((BaseTempleteMouse*)textMouse);
	BracketWidgetKeyboard* keyboard = InputFactory::createBrackWidgetKeyboard(textWidget);
	textWidget->setKeyboard(keyboard);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(textWidget);
	//textWidget->initChildrenMgr(childrenMgr);
	//textWidget->getOperateMgr()->setCanBeChoosenObjId(textWidget->getObjId());
	//textWidget->getOperateMgr()->setCanAddObjParentId(templeteParent->getObjId());
	if (!isCreateFromLoad){
		TextWidgetParam * textWidgetParam = TempleteFactory::createTextWidgetParam(textWidget);
		textWidget->initContentData(textWidgetParam);
		//textWidget->initWidget();
	}
	return textWidget;
}
MenstruationTemplete * TempleteFactory::createMenstruationTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad){
	MenstruationTemplete * excel = new MenstruationTemplete(templeteParent->getWriterMgr(), templeteParent
		, ConstantsID::ContainerId::MENSTRUATION_TEMPLETE_OBJ, BaseConstants::NONE, isCreateFromLoad);
	if (!isCreateFromLoad){
		MenStruationDataParam * textWidgetParam = TempleteFactory::createMenstruationParam(excel);
		excel->initContentData(textWidgetParam);
		excel->initWidget();
	}
	return excel;
}
MacroTemplete * TempleteFactory::createMacroTemplete(BaseTemplate * templeteParent, int userID, bool isCreateFromLoad)
{
	MacroTemplete * excel = new MacroTemplete(templeteParent->getWriterMgr(), templeteParent, ConstantsID::ContainerId::MACRO_TEMPLETE_OBJ
		, BaseConstants::NONE, isCreateFromLoad);
	if (!isCreateFromLoad)
	{
		MacroTempleteParam * textWidgetParam = TempleteFactory::createMacronParam(excel);
		excel->initContentData(textWidgetParam);
	}
	return excel;
}
TextWidget * TempleteFactory::createMacroTextWidgetTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad){
	//int userID = templeteParent->getUserID();
	TextWidget * textWidget = new TextWidget(templeteParent->getWriterMgr(), ConstantsID::ContainerId::MACRO_TEXT_TEMPLETE_OBJ, BaseConstants::NONE);
	textWidget->setTempleteParent(templeteParent);
	TextWidgetKeyboard* keyboard = InputFactory::createTextWidgetKeyboard(textWidget);
	textWidget->setKeyboard(keyboard);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(textWidget);
	int choosenObjid = templeteParent->getObjID();
	textWidget->setChooseObjId(choosenObjid);
	//textWidget->initChildrenMgr(childrenMgr);
	/*BaseContainer *  beChooseObj = templeteParent->getOperateMgr()->getCanBeChoosenObj();
	int choosenObjid = BaseConstants::NONE;
	if (beChooseObj)
	{
		choosenObjid = beChooseObj->getObjId();
	}
	textWidget->getOperateMgr()->setCanBeChoosenObjId(choosenObjid);
	textWidget->getOperateMgr()->setCanAddObjParentId(templeteParent->getTempleteParent()->getObjId());*/
	if (!isCreateFromLoad){
		TextWidgetParam * textWidgetParam = TempleteFactory::createTextWidgetParam(textWidget);
		textWidget->initContentData(textWidgetParam);
		textWidget->initWidget();
	}
	return textWidget;
}
BracketWidget * TempleteFactory::createBracketTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad)
{
	BracketWidget * textWidget = new BracketWidget(templeteParent->getWriterMgr(), templeteParent, ConstantsID::ContainerId::BRAKETWIDGET_TEMPLETE);
	TextWidgetMouse * textMouse = InputFactory::createTextWidgetMouse(textWidget);
	textWidget->setMouse((BaseTempleteMouse*)textMouse);
	BracketWidgetKeyboard* keyboard = InputFactory::createBrackWidgetKeyboard(textWidget);
	textWidget->setKeyboard(keyboard);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(textWidget);
	//textWidget->initChildrenMgr(childrenMgr);
	/*BaseContainer *  beChooseObj = templeteParent->getOperateMgr()->getCanBeChoosenObj();
	int choosenObjid = BaseConstants::NONE;
	if (beChooseObj)
	{
		choosenObjid = beChooseObj->getObjId();
	}
	textWidget->getOperateMgr()->setCanBeChoosenObjId(choosenObjid);
	textWidget->getOperateMgr()->setCanAddObjParentId(templeteParent->getTempleteParent()->getObjId());*/
	int choosenObjid = templeteParent->getObjID();
		textWidget->setChooseObjId(choosenObjid);
	
	if (!isCreateFromLoad)
	{
		TextWidgetParam * textWidgetParam = TempleteFactory::createTextWidgetParam(textWidget);
		textWidget->initContentData(textWidgetParam);
		textWidget->initWidget();
	}

	return textWidget;
}
ChoiceWidgetTemplete * TempleteFactory::createChoiceWidgetTemplete(BaseTemplate * templeteParent, char widgetType, bool isCreateFromLoad)
{
	ChoiceWidgetTemplete * excel = new ChoiceWidgetTemplete(templeteParent->getWriterMgr(), widgetType, templeteParent
		, ConstantsID::ContainerId::CHOICEWIDGET_TEMPLETE, BaseConstants::NONE, isCreateFromLoad);
	if (!isCreateFromLoad)
	{
		ChoiceWidgetTempleteParam * choiceWidgetParam = TempleteFactory::createChoiceWidgetParam(excel, NULL);
		excel->initContentData(choiceWidgetParam);
		//excel->initWidget();
	}
	else{
		excel->setCreateFromLoad();
	}
	return excel;

}
TextWidget *TempleteFactory::createChoiceTextWidgetTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad)
{
	TextWidget * textWidget = TempleteFactory::createTextWidgetTemplete(templeteParent, ConstantsID::ContainerId::CHOICE_TEXT_TEMPLETE_OBJ, !isCreateFromLoad);
	//textWidget->setDrawTxtParam();
	textWidget->setChooseObjId(templeteParent->getObjID());
	//textWidget->getOperateMgr()->setCanAddObjParentId(templeteParent->getTempleteParent()->getObjId());
	//textWidget->getOperateMgr()->setCanBeChoosenObjId(templeteParent->getObjId());
	if (!isCreateFromLoad)
	{
		TextWidgetParam * textWidgetParam = TempleteFactory::createTextWidgetParam(textWidget);
		textWidget->initContentData(textWidgetParam);
		//textWidget->initWidget();
	}

	return textWidget;
}
TextWidget * TempleteFactory::createTextWidgetTemplete(BaseTemplate * templeteParent, int objID, bool isCreateStrData){
	TextWidget * excel = new TextWidget(templeteParent->getWriterMgr(), objID, BaseConstants::NONE, isCreateStrData);
	excel->setTempleteParent(templeteParent);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(excel);
	excel->initChildrenMgr(childrenMgr);
	TextWidgetMouse * textMouse = InputFactory::createTextWidgetMouse(excel);
	excel->setMouse((BaseTempleteMouse*)textMouse);
	TextWidgetKeyboard* keyboard = InputFactory::createTextWidgetKeyboard(excel);
	excel->setKeyboard(keyboard);
	return excel;

}
ExcelTemplete * TempleteFactory::createExcelTemplete(BaseTemplate * templeteParent, bool isCreateFromLoad)
{
	ExcelTemplete * excel = new ExcelTemplete(templeteParent->getWriterMgr(), templeteParent
		, ConstantsID::ContainerId::EXCEL_TEMPLETE, BaseConstants::NONE);
	/*ExcelMouseInputManager* mouseManager = new ExcelMouseInputManager(excel);
	excel->setMouse(mouseManager);
	ExcelKeyboard * keyboard = new ExcelKeyboard(excel);
	excel->setKeyboard(keyboard);*/
	if (!isCreateFromLoad)
	{
		ExcelTempleteParamData * excelData = new ExcelTempleteParamData(excel, NULL);
		excel->initContentData(excelData);
	}
	return excel;

}
BaseTemplate* TempleteFactory::createExcelCellTemplate(BaseTemplate * templeteParent)
{
	//int userID = templeteParent->getUserID();
	BaseTemplate * temp = new BaseTemplate(templeteParent->getWriterMgr(), ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER);
	TempParagraphMgrData * data = new TempParagraphMgrData(temp, ConstantsID::ClassId::TEMP_PARAGRAPH_MGR_DATA,NULL);
	temp->initContentData(data);
	ParagraphManager * paragraphMgr = data->getParagraphMgr();
	paragraphMgr->setExcelParagrphManager();
	TempContainerMgr * childrenMgr = new TempContainerMgr(temp, paragraphMgr);
	return temp;
}
ChoiceItemTextWidget* TempleteFactory::crateUnfoldChoiceItemTextWidget(BaseTemplate * templeteParent, char type, int paddingLeft, bool isCreateFromLoad)
{
	ChoiceItemTextWidget* choiceItemText = new ChoiceItemTextWidget(templeteParent->getWriterMgr(), templeteParent, type,
		ConstantsID::ContainerId::UNFOLD_CHOICE_TEXT_TEMPLETE_OBJ, paddingLeft, isCreateFromLoad);
	choiceItemText->setTempleteParent(templeteParent);
	BaseChildrenManager * childrenMgr = new BaseChildrenManager(choiceItemText);
	choiceItemText->initChildrenMgr(childrenMgr);
	choiceItemText->setChooseObjId(templeteParent->getObjID());
	/*if (!isCreateFromLoad)
	{
		TextWidgetParam * textWidgetParam = TempleteFactory::createTextWidgetParam(choiceItemText);
		choiceItemText->initContentData(textWidgetParam);
	}*/
	return choiceItemText;
}
UnfoldChoiceWidgetTemplete * TempleteFactory::createUnfoldChoiceWidget(BaseTemplate * templeteParent, char type, bool isCreateFromLoad)
{
	UnfoldChoiceWidgetTemplete * excel = new UnfoldChoiceWidgetTemplete(templeteParent->getWriterMgr(), type, templeteParent
		, ConstantsID::ContainerId::UNFOLD_CHOICE_TEMPLETE, BaseConstants::NONE, isCreateFromLoad);
	if (!isCreateFromLoad)
	{
		UnfoldChoiceWidgetTempleteParam * choiceWidgetParam = TempleteFactory::createUnfoldChoiceWidgetParam(excel, NULL);
		excel->initContentData(choiceWidgetParam);
	}
	return excel;
}
MacroTempleteParam * TempleteFactory::createMacronParam(BaseTemplate * templeteOwner)
{
	MacroTempleteParam * textWidgetParam = new  MacroTempleteParam(templeteOwner, NULL);
	return textWidgetParam;
}
MenStruationDataParam * TempleteFactory::createMenstruationParam(BaseTemplate * templeteOwner)
{
	MenStruationDataParam * textWidgetParam = new  MenStruationDataParam(templeteOwner, NULL);
	return textWidgetParam;
}
TextWidgetParam * TempleteFactory::createTextWidgetParam(BaseTemplate * templeteOwner){
	TextWidgetParam * textWidgetParam = new  TextWidgetParam(templeteOwner, NULL);
	return textWidgetParam;
}
StructurationTextParam * TempleteFactory::createStructurationParam(BaseTemplate * templeteOwner){
	StructurationTextParam * textWidgetParam = new  StructurationTextParam(templeteOwner, NULL);
	return textWidgetParam;
}
ChoiceWidgetTempleteParam * TempleteFactory::createChoiceWidgetParam(BaseTemplate * templeteOwner, CodeParams * codeParams)
{
	ChoiceWidgetTempleteParam * choiceWidgetParam = new  ChoiceWidgetTempleteParam(templeteOwner, codeParams);
	return choiceWidgetParam;
}
UnfoldChoiceWidgetTempleteParam * TempleteFactory::createUnfoldChoiceWidgetParam(BaseTemplate * templeteOwner, CodeParams * codeParams)
{
	UnfoldChoiceWidgetTempleteParam * choiceWidgetParam = new  UnfoldChoiceWidgetTempleteParam(templeteOwner, codeParams);
	return choiceWidgetParam;
}
