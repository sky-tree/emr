#ifndef SELECTMUBANDLG_H
#define SELECTMUBANDLG_H

#include <QDialog>
#include "dataModel/BaseDataType.h"
#include"dataModel/DBDataModel/DBManager.h"
class QRadioButton;
class QVerticalLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableView;
class DataBaseControl;
class QSqlQueryModel;
// 代码一定要是: UTF-8(BOM)
//qt版本高于等于qt5，VS版本高于或等于VS2010
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
class SelectMuBanDlg : public QDialog
{
    Q_OBJECT
public:
    SelectMuBanDlg(QString pType,QWidget* parent = 0);
	~SelectMuBanDlg();
    virtual void closeEvent(QCloseEvent *e);
private:
    QRadioButton * allBtn;
    QRadioButton * publicBtn;
    QRadioButton * departmentBtn;
    QRadioButton * personalBtn;

    QLabel * codeLabel;
	QLineEdit * inputEdit;
    QPushButton * searchBtn;
    QTableView * table;
	DataBaseControl *m_DataBaseControl;
	DBManager *m_DbManager;
	QSqlQueryModel* model;
	QString type;
signals:

private slots:
    void onRadioBtnToggled();
    void doubleClicked(const QModelIndex &index);
	void onSearchBtnClicked();

	//测试
public:
	MRTemplate selectedData;
	MRTemplate& getData() { return selectedData; }
	void updateTable();
	//void setType(QString pType){ type = pType; }
};

#endif // SELECTMUBANDLG_H
