#ifndef INSPECTION_H
#define INSPECTION_H

#include <QWidget>
#include "ui_inspection.h"
#include<QDialog>
#include<QSqlQueryModel>
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class DataBaseControl;
class DBManager;
class QSqlQueryModel;
class Inspection : public QDialog
{
	Q_OBJECT

public:
	Inspection(QWidget *parent = 0);
	~Inspection();
	static QString getData();
private:
	void createConnect();
	void initSql();
	void initData();
	bool initModel();
	QString getReturnData() const;
	void initTableView();
	void initInspectView();
	void initInspectMicroView();
	void initExmineView();
	QWidget* createCheckBoxofWidget();
	void constructWidgetData(QStringList listData, QTableWidget *widget);
	void setTableHeaderWidth(QTableWidget *widget);
	void setReturnData();
	QString getExmineData();
	QString getInspectData();
	QString getInspectMicroData();
private slots:
     void ClickResult();
	void tabChanged(int index);
private:
	Ui::Inspection ui;
	QString returnData;
	DBManager* m_DbManager;
	DataBaseControl* m_DataBaseControl;
	QSqlQueryModel*  examineResultModel;
	QSqlQueryModel*  inspectResultModel;
	QSqlQueryModel*  inspectMicroResultModel;
	QString        exmineResultSql;
	QString        inspectResultSql;
	QString        inspectMicroResultSql;
};
#endif // INSPECTION_H
