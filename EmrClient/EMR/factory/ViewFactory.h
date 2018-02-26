#ifndef VIEW_FACTORY_H_
#define VIEW_FACTORY_H_


#include <QFont>
#include <QImage>


class BaseContainer;
class ViewObj;
class BaseTemplate;
class WriterManager;
class StrContentData;
class CodeParams;

class ExcelCellData;
class BaseContainerData;
class ExcelTemplete;


class ViewFactory
{
public:
	ViewFactory();
	~ViewFactory();

public:
	static void init();

public:

	static ViewObj * createRoot(WriterManager * writerMgr);
	static ViewObj * createTxtWindow(WriterManager * writerMgr);
	static ViewObj * createPagePanel(WriterManager * writerMgr);
	static ViewObj * createScrollPanel(WriterManager * writerMgr);

	static ViewObj * createNormalPage(WriterManager * writerMgr);
	static ViewObj * createNormalPage(WriterManager * writerMgr, ViewObj * title, ViewObj * bottom);

	static ViewObj * createPageTitle(WriterManager * writerMgr);
	static ViewObj * createPageBottom(WriterManager * writerMgr);
	static ViewObj * createPageContent(WriterManager * writerMgr);
	static ViewObj * createExcelPage(WriterManager * writerMgr, int x, int y, int w, int h, ExcelTemplete* excelTemplate, bool isCreateOnLoad = false);

	static ViewObj * createScrollButtonUp(WriterManager * writerMgr);
	static ViewObj * createScrollButtonMiddle(WriterManager * writerMgr);
	static ViewObj * createScrollButtonDown(WriterManager * writerMgr);

	static ViewObj * createLayer(WriterManager * writerMgr, int layerID, int color, char defaultH);
	static ViewObj * createExcelLayer(WriterManager * writerMgr, int layerID, int color, char defaultH);
	static ViewObj * createRow(WriterManager * writerMgr, BaseContainer * paragraph);
	//excel简单包裹对象，不用绘制自己
	static ViewObj * createExcelRowContainer(ExcelTemplete * excelTemplete, ViewObj * viewParent);

	static ViewObj * createMenstruationRow(WriterManager * writerMgr, BaseContainer * paragraph);
	static ViewObj * createEnter(WriterManager * writerMgr, int userID);
	static ViewObj * createFlag(WriterManager * writerMgr);
	static ViewObj * createHorizontalLine(WriterManager * writerMgr, int userID);
	static ViewObj * createEndPageLine(WriterManager * writerMgr, int userID);
	static ViewObj * createTxt(WriterManager * writerMgr, int userID, QString str);

	static ViewObj * createPageNumber(WriterManager * writerMgr, CodeParams * codeParams, int objId);

	static ViewObj * createTemplateTxt(WriterManager * writerMgr, BaseTemplate* parentTemplate, QString str, StrContentData * data);

	static ViewObj * createImg(WriterManager * writerMgr, CodeParams * codeParams, QString imgPath);
	//创建表格
	static ViewObj * createExcel(BaseTemplate * templateParent, ViewObj * viewParent, int x, int y, int w, int h, int rowNumber, int colNuber, bool isCreateFromLoad);
	static ViewObj * createExcelCell(ViewObj * viewExcel, ExcelCellData* excelCellData);
	static ViewObj * createChoiceBoxRectImg(WriterManager * writerMgr, BaseTemplate * templateParent, int userID);
	static ViewObj * createChoiceBoxRadioImg(WriterManager * writerMgr, BaseTemplate * templateParent, int userID);
	static ViewObj * createChoiceBoxMultipleImg(WriterManager * writerMgr, BaseTemplate * templateParent, int userID);
	static ViewObj * createChoiceBoxContainerView(WriterManager * writerMgr, BaseTemplate * templateParent, int userID);
	static StrContentData * createDefaultStrContentData();

private:
	static void initImgParam(int& w, int& h);
};

#endif
