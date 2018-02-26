#pragma once
#ifndef ATTRIBUTECLICK_H
#define ATTRIBUTECLICK_H
#include<QMap>
#include<QString>
#include"views/propertySettingDlgs/cellpropertydlg.h"
#include"views/propertySettingDlgs/inputpromptdlg.h"
#include"views/propertySettingDlgs/keyworddlg.h"
#include"views/propertySettingDlgs/pagesettingdlg.h"
#include"views/propertySettingDlgs/selectionpropertydlg.h"
#include"views/propertySettingDlgs/structuringdlg.h"
#include"views/propertySettingDlgs/tablepropertydlg.h"
#include"views/propertySettingDlgs/FormulaDlg.h"
#include"views/propertySettingDlgs/MacroPropertyPopup.h"
#include"views/propertySettingDlgs/ParagraphSettingPopup.h"
//#include"views/globalvariable.h"
class attributeClick
{
public:
	attributeClick();
	~attributeClick();
	void show(QMap<QString,QString> &map);

private:
	//QMap<QString, QString> *m_map;
	CellPropertyDlg *m_cellPropertyDlg;
	InputPromptDlg  *m_inputPromptDlg;
	KeyWordDlg      *m_keyWordDlg;
	PageSettingDlg  *m_PageSettingDlg;
	SelectionPropertyDlg *m_SelectionPropertyDlg;
	StructuringDlg  *m_structuringDlg;
	TablePropertyDlg *m_tablePropertyDlg;
	FormulaDlg     *m_formulaDlg;
	MacroPropertyPopup *m_MacroPropertyPopupDlg;
	ParagraphSettingPopup *m_paragraphSettingDlg;
	//enum  dlgType
	//{
	// cellPropertyDlg=1, inputPromptDlg, keyWordDlg, pageSettingDlg, selectionPropertyDlg,selectionPropertyDlg_multity,selectionPropertyDlg_bool, structuringDlg, tablePropertyDlg
	//};
private:
	void setSelectionPropertyDlg(int type,QMap<QString,QString> &map);
};
#endif
