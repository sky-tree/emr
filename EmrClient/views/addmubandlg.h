#ifndef ADDMUBANDLG_H
#define ADDMUBANDLG_H

#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
  
#include <QDialog>
#include <QFile>
#include <QTextStream>
class QLabel;
class QComboBox;
class QLineEdit;
class QRadioButton;
class QTextEdit;
class QCheckBox;
class QSqlQueryModel;
class DataBaseControl;
#include"qmap.h"
#include"dataModel/DBDataModel/DBManager.h"
class AddMuBanDlg : public QDialog
{
    Q_OBJECT
public:
    explicit AddMuBanDlg(QWidget *parent = 0);
	~AddMuBanDlg();
	void init(QMap<QString, QString> &map);
	void setRadioBtnEnable();
public:
	inline	QMap<QString, QString> getData() const
	{
		return *map;
	}
private:
	//int getType();
	int getTemplateType();
	inline	QString getCheckBoxIsChecked(QCheckBox *checkBox);
	QString m_dctDepat;//医生科室
signals:

private slots:
    void onPushBtnClicked();
    void onSecondContainerBtnsClicked(bool checked);
private:
    void createWidgets();
    void loadComboxData();
	void setComboxValue();
private:
    QLabel *nameLabel;
    QLabel *departmentLabel;
    QLabel *businessLabel;

    QComboBox *departmentComboBox;
    QComboBox *businessComboBox;
    QLineEdit *lineEdit;
    QRadioButton *publicRB;
    QRadioButton *departmentRB;
    QRadioButton *personalRB;

    QPushButton *saveButton;
    QPushButton *quitButton;
	QMap<QString, QString> *map;
	QSqlQueryModel* model;
	DataBaseControl *m_DataBaseControl;
	DBManager *m_DbManager;
	QMap<QString, QString> dataMap;
private:
	void debuglogs(QString logs);
	QFile*		 m_pLogFile;
	QTextStream* m_pTxtOutput;
};

#endif // ADDMUBANDLG_H
