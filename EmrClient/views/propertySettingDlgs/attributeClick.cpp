#include "attributeClick.h"
attributeClick::attributeClick() : m_cellPropertyDlg(NULL), m_inputPromptDlg(NULL)
, m_keyWordDlg(NULL), m_PageSettingDlg(NULL), m_SelectionPropertyDlg(NULL)
, m_structuringDlg(NULL), m_tablePropertyDlg(NULL), m_MacroPropertyPopupDlg(NULL), m_formulaDlg(NULL), m_paragraphSettingDlg(NULL)
{
}
#define DELETEPOINT(m_Point)\
if (m_Point != NULL){ delete m_Point; m_Point = NULL; }
attributeClick::~attributeClick()
{
	//DELETEPOINT(m_map)
	DELETEPOINT(m_cellPropertyDlg)
	DELETEPOINT(m_keyWordDlg)
	DELETEPOINT(m_PageSettingDlg)
	DELETEPOINT(m_SelectionPropertyDlg)
	DELETEPOINT(m_structuringDlg)
	DELETEPOINT(m_tablePropertyDlg)
	DELETEPOINT(m_inputPromptDlg)
	DELETEPOINT(m_formulaDlg)
	DELETEPOINT(m_paragraphSettingDlg)
}
#define CREATION(m_Name,ClassName,map) \
if (m_Name == NULL){ m_Name = new ClassName(); } \
	m_Name->InitData(map);\
	m_Name->show();\
if (m_Name->exec() == QDialog::Accepted){}\
 
void attributeClick::show(QMap<QString, QString> &map)
{
	QString typeStr = map["dlgType"];
	int type = 0;
	if (typeStr.isEmpty() == false)
	{
		type = typeStr.toInt();
	}
	switch (type)
	{
	case cellPropertyDlg:
		CREATION(m_cellPropertyDlg, CellPropertyDlg, map)
		break;
	case inputPromptDlg:
		CREATION(m_inputPromptDlg, InputPromptDlg, map)
		break;
	case keyWordDlg:
		CREATION(m_keyWordDlg, KeyWordDlg, map)
		m_keyWordDlg->setWindowTitle("关键词属性设置");
		break;
// 	case pageSettingDlg:
// 		CREATION(m_PageSettingDlg, PageSettingDlg, map)
// 		break;
	case selectionPropertyDlg:
		//CREATION(m_SelectionPropertyDlg, SelectionPropertyDlg, map)
		setSelectionPropertyDlg(1, map);
		break;
	case selectionPropertyDlg_multity:
		setSelectionPropertyDlg(2, map);
		break;
	case selectionPropertyDlg_bool:
		setSelectionPropertyDlg(3, map);
		break;
	case structuringDlg:
		CREATION(m_structuringDlg, StructuringDlg, map)
		break;
	case tablePropertyDlg:
		CREATION(m_tablePropertyDlg, TablePropertyDlg, map)
		break;
	case macroPropertyPopupDlg:
		CREATION(m_MacroPropertyPopupDlg, MacroPropertyPopup, map)
		break;
	case changelessText:
		CREATION(m_keyWordDlg, KeyWordDlg, map)
		m_keyWordDlg->setWindowTitle("固定元素属性设置");
		break;
	case formula:
		CREATION(m_formulaDlg, FormulaDlg, map)
		break;
	case paragraphSettingDlg:
		CREATION(m_paragraphSettingDlg, ParagraphSettingPopup,map)
		break;
	case unfoldRedioDlg:
		setSelectionPropertyDlg(1, map);
		break;
	case unfoldMultpleChoiceDlg:
		setSelectionPropertyDlg(1, map);
		break;
	default:
		break;
	}
}
void attributeClick::setSelectionPropertyDlg(int type,QMap<QString, QString> &map)
{
	if (m_SelectionPropertyDlg == NULL)
	{
		m_SelectionPropertyDlg = new SelectionPropertyDlg();

	}
	//m_SelectionPropertyDlg->setFlags(type);
	m_SelectionPropertyDlg->InitData(map);
	m_SelectionPropertyDlg->show();
	if (m_SelectionPropertyDlg->exec() == QDialog::Accepted)
	{
		//int a = 1;
	}
}