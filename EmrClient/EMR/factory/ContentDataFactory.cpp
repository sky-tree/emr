#include "ContentDataFactory.h"





#include "WriterManager.h"
#include "OperateMgr.h"
#include "SystemParams.h"

#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseTemplate.h"
#include "BaseParagraph.h"
#include "ParagraphData.h"

#include "BaseContentData.h"
#include "StrContentData.h"
#include "ImgConTentData.h"

#include "CodeParams.h"
#include "NodeParams.h"
#include "ParagraphCodeParams.h"

#include "ParagraphData.h"
#include "TempParagraphMgrData.h"

#include "ImageMouseInput.h"
#include "ObjMouseManager.h"

#include "DrawHorizontalLine.h"

#include "TxtViewParam.h"
#include "ImgViewParam.h"
#include "BaseChildrenManager.h"
#include "TempContainerMgr.h"
#include "NodeMgr.h"
#include "NodeChildrenMgr.h"
#include "ParagraphManager.h"
#include "ObjDrawManager.h"

#include "DrawImage.h"
#include "UnfoldChoiceWidgetTempleteParam.h"
#include "ChoiceWidgetTempleteParam.h"

#include "ExcelTempleteParamData.h"
#include "MenStruationDataParam.h"
//#include "ExcelCellData.h"
#include "MacroTempleteParam.h"
#include "StructurationTextParam.h"
#include "TextWidgetParam.h"

#include "TempleteViewObj.h"

#include "TempleteFactory.h"

#include "ConstantsID.h"
#include "UIConstants.h"
#include "StrConstants.h"
#include "BaseIOTools.h"
#include "BaseScaleParam.h"
#include "BaseEditParam.h"
bool ContentDataFactory::isSaveUnfoldChoicePaddingLeft = false;
ContentDataFactory::ContentDataFactory()
{
	
}

ContentDataFactory::~ContentDataFactory()
{
}
void ContentDataFactory::loadObjID(BaseIOTools * iOTools, QString version, int &classID, int &userID, int &objID
	, int &chooseObjID)
{
	classID = iOTools->readInt();

	if (classID < 1)
		int a = 0;

	//if (version != StrConstants::VERSION)
	//{
	//	userID = -1;
	//}
	//else
	//	userID = iOTools->readInt();



	if (userID==100)
	{
		int a = 0;
	}
	objID = iOTools->readInt();
	chooseObjID = iOTools->readInt();
}
BaseContainer * ContentDataFactory::loadData(WriterManager * mgr, BaseIOTools * iOTools, BaseContainer * tempParent, char addChildTypeAndAddToParent)
{
	BaseContainer * owner = NULL;
	BaseChildrenManager * childrenMgr = NULL;
	BaseContentData * data = NULL;
	ParagraphManager * paragraphMgr = NULL;
	QString version = mgr->getOperateMgr()->getSystemParams()->getVersion();
	int classID = 0;
	int userID = 0;
	int objID = 0;
	int chooseObjId = 0;
	loadObjID(iOTools, version, classID, userID, objID, chooseObjId);
	/*if (objID==ConstantsID::ContainerId::CHOICEWIDGET_TEMPLETE)
	{
		int is = 0;
	}*/
	bool isHHTempObj = true;
	bool needAddToTempParent = true;
	if (addChildTypeAndAddToParent == 2)
		needAddToTempParent = false;
	ImageMouseInput * imgMouseInput = NULL;
	ObjMouseManager * mouseMrg = NULL;
	ViewObj * view = NULL;
	if (objID == ConstantsID::ContainerId::HORIZONTAL_LINE)
	{
		int a = 0;
	}
	switch (objID)
	{
	case ConstantsID::ContainerId::TITLE_TEMP:
	case ConstantsID::ContainerId::BOTTOM_TEMP:
	case ConstantsID::ContainerId::CONTENT_TEMP:
		owner = new BaseTemplate(mgr, objID);
		childrenMgr = new BaseChildrenManager(owner);
		isHHTempObj = false;
		break;
	case ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER:
	case ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER:
	case ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER:
		owner = new BaseTemplate(mgr, objID);
		paragraphMgr = ((TempParagraphMgrData *)tempParent->getContentData())->getParagraphMgr();
		childrenMgr = new NodeMgr(owner, paragraphMgr);
		isHHTempObj = false;
		break; 
	case ConstantsID::ContainerId::NODE_CONTAINER:
		owner = new BaseTemplate(mgr, objID);
		paragraphMgr = ((TempParagraphMgrData *)tempParent->getContentData())->getParagraphMgr();
		childrenMgr = new NodeChildrenMgr(owner, paragraphMgr);
		isHHTempObj = false;
		break;
		// 节点内标题创建
	case ConstantsID::ContainerId::NODE_TITLE:
		//owner = new BaseTemplate(mgr, objID);
		//paragraphMgr = ((TempParagraphMgrData *)tempParent->getContentData())->getParagraphMgr();
		//childrenMgr = new NodeChildrenMgr(owner, paragraphMgr);
		//isHHTempObj = false;
		break;
	case ConstantsID::ContainerId::NORMAL_PARAGRAPH:
		paragraphMgr = ((TempParagraphMgrData *)tempParent->getContentData())->getParagraphMgr();
		owner = new BaseParagraph(mgr, paragraphMgr, objID);
		childrenMgr = new BaseChildrenManager(owner);
		owner->setParent(paragraphMgr->getTempParent());
		//paragraphMgr->addParagraph((BaseParagraph *)owner, -1, true);

		isHHTempObj = false;
		needAddToTempParent = false;
		break;
	case ConstantsID::ContainerId::IMG:
	{
		owner = new ViewObj(mgr, objID);
		view = (ViewObj *)owner;
		ImgViewParam * param = new ImgViewParam(view);
		imgMouseInput = new ImageMouseInput(view);
		mouseMrg = view->getMouseMgr();
		mouseMrg->setMouse(imgMouseInput);
		BaseScaleParam * scaleParam = new BaseScaleParam(view, ConstantsID::ContainerId::IMG);
		param->initScaleParam(scaleParam);
		isHHTempObj = false;
	}
		break;
	case ConstantsID::ContainerId::TXT:
	case ConstantsID::ContainerId::TITLE_PAGE_NUMBER:
	case ConstantsID::ContainerId::BOTTOM_PAGE_NUMBER:
		owner = new ViewObj(mgr, objID);
		new TxtViewParam((ViewObj *)owner);
		isHHTempObj = false;
		break;
	case ConstantsID::ContainerId::TMPLATE_TXT:
		owner = new TempleteViewObj(mgr, (BaseTemplate*)tempParent, objID);
		new TxtViewParam((ViewObj *)owner);
		isHHTempObj = false;
		break;
	case ConstantsID::ContainerId::ENTER:
		owner = new ViewObj(mgr, objID);
		new ImgViewParam((ViewObj *)owner);
		isHHTempObj = false;
		break;
	case ConstantsID::ContainerId::HORIZONTAL_LINE:
	case ConstantsID::ContainerId::END_PAGE_LINE:
		//owner = ViewFactory::createHorizontalLine(mgr, userID);
	{
													  owner = new ViewObj(mgr, objID);
													  BaseViewParam * param = new BaseViewParam((ViewObj *)owner);
													  DrawHorizontalLine * line = new DrawHorizontalLine((ViewObj *)owner, UIConstants::FillRectType::ONLY_FILL);
													  ObjDrawManager * drawMgr = ((ViewObj *)owner)->getDrawMgr();
													  drawMgr->initDraws(line, NULL);

													  param->init(0, 0, 0, 16);
													  param->initDefaultWH(BaseConstants::DefaultWHType::FILL_PARENT_INSIDE, 0);
													  isHHTempObj = false;
	}
		break;
	case ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER:
		owner = new BaseTemplate(mgr, objID);
		childrenMgr = new TempContainerMgr(owner, NULL);
		isHHTempObj = false;
		break;
	}
	if (isHHTempObj)
	{

	}
	else if (needAddToTempParent && tempParent)
	{
		tempParent->getChildrenMgr()->pushChild(owner, true);
	}

	switch (classID)
	{
	case ConstantsID::ClassId::BASE_CONTENT_DATA:
		data = new BaseContentData(owner, classID, NULL);
		break;
	case ConstantsID::ClassId::CODE_TEMP_DATA:
		data = new CodeTempData(owner, classID, NULL);
		break;
	case ConstantsID::ClassId::TEMP_PARAGRAPH_MGR_DATA:
	{
		 data = new TempParagraphMgrData(owner, ConstantsID::ClassId::TEMP_PARAGRAPH_MGR_DATA, NULL);
	     if (objID == ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER)
			 {
				 ParagraphManager * paragraphMgr = ((TempParagraphMgrData*)data)->getParagraphMgr();
				 paragraphMgr->setExcelParagrphManager();
				 ((TempContainerMgr*)childrenMgr)->setParagraphManager(paragraphMgr);
			 }
			 else
			{

			 }
	}
		break;
	case ConstantsID::ClassId::PARAGRAPH_DATA:
		data = new ParagraphData(owner);
		break;
	case ConstantsID::ClassId::STR_CONTENT_DATA:
		data = new StrContentData(owner, NULL);
		break;
	case ConstantsID::ClassId::IMG_CONTENT_DATA:
		data = new ImgConTentData(owner, NULL);
		break;
	case  ConstantsID::ClassId::TEXT_WIDGET_PARAM:
		data = new TextWidgetParam(NULL, NULL);
		if (addChildTypeAndAddToParent == 0)
			data->setNeedLoadSelfChildren(false);
		else
			data->setNeedLoadSelfChildren(true);
		break;
	case  ConstantsID::ClassId::STRUCRATION_TEXT_DATA_PARAM:
		data = new StructurationTextParam(NULL, NULL);
		break;
	case  ConstantsID::ClassId::MENSTRUATION_DATA_PARAM:
		data = new MenStruationDataParam(NULL, NULL);
		break;
	case  ConstantsID::ClassId::MACRO_DATA_PARAM:
		data = new MacroTempleteParam(NULL, NULL);
		break;
	case  ConstantsID::ClassId::CHOICEAA_WIDGET_PARAM:
		data = new ChoiceWidgetTempleteParam(NULL, NULL);
		break;
	case ConstantsID::ClassId::EXCEL_DATA_PARAM:
		data = new ExcelTempleteParamData(NULL, NULL);
		break;
	case ConstantsID::ClassId::UNFOID_CHOICEAA_WIDGET_PARAM:
		data = new UnfoldChoiceWidgetTempleteParam(NULL,NULL);
			break;

	}
	if (isHHTempObj)
	{
		if (data)
		{
			data->setOwnerObjId(objID);
			//data->setUserId(userID);
			data->setChooseObjId(chooseObjId);
			data->loadCodeParams(iOTools);
			data->loadSelfData(iOTools);
			owner = data->createOwner(tempParent, iOTools, needAddToTempParent);
		}

		//data->loadChildrenData(iOTools);
	}
	else
	{
		owner->setChooseObjId(chooseObjId);
		owner->initContentData(data);
		data->doLoadWithoutID(iOTools);
	}
	if (ConstantsID::ContainerId::IMG == objID)
	{
		BaseViewParam * viewParam = view->getViewParam();
		ImgConTentData* imgContentData = (ImgConTentData*)view->getContentData();
		if (imgContentData->getBaseEditParam()->getScaleH() != -1 && 
			imgContentData->getBaseEditParam()->getScaleW() != -1)
		{
			viewParam->setSelfH(imgContentData->getBaseEditParam()->getScaleH());
			viewParam->setSelfW(imgContentData->getBaseEditParam()->getScaleW());
		}
	}
	return owner;
}
CodeParams * ContentDataFactory::loadCodeParams(BaseIOTools * iOTools, BaseContentData * contentData)
{
	CodeParams * codeParams = NULL;
	int classID = iOTools->readInt();
	switch (classID)
	{
	case ConstantsID::ClassId::BASE_CODE_DATA:
		codeParams = new CodeParams();
		break; 
	case ConstantsID::ClassId::NODE_DATA:
		codeParams = new NodeParams();
		break;
	case ConstantsID::ClassId::PARAGRAPH_CODE_DATA:
		codeParams = new ParagraphCodeParams();
		break;
	}
	codeParams->loadSelfData(iOTools);
	return codeParams;
}
ParagraphData * ContentDataFactory::getParagraphDataCopy(ParagraphData * source)
{
	ParagraphData * data = new ParagraphData(NULL, source);
	return data;
}

ParagraphData * ContentDataFactory::createDefaultParagraphData()
{
	ParagraphData * data = new ParagraphData(NULL);
	return data;
}

StrContentData * ContentDataFactory::createDefaultTxtData()
{
	StrContentData * data = new StrContentData(NULL, NULL, "", UIConstants::DefaultFontParams::TXT_SIZE, StrConstants::DEFAULT_FONT_FAMILY
		, UIConstants::DefaultFontParams::FONT_COLOR, UIConstants::DefaultFontParams::BACKGROUND_COLOR, 0, 1, 0, 1, UIConstants::DefaultFontParams::ITALIC
		, UIConstants::DefaultFontParams::BOLD, 0);
	return data;
}