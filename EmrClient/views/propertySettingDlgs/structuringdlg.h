#ifndef STRUCTURINGDLG_H
#define STRUCTURINGDLG_H

#include <QWidget>
#include "customDelegate/MSC_VER.h"
#include"qdialog.h"
#include<QMap>
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QTextEdit;
class QCheckBox;
class QRadioButton;
class QDateTimeEdit;

class StructuringDlg : public QDialog
{
    Q_OBJECT

public:
    explicit StructuringDlg(QWidget *parent = 0);
    ~StructuringDlg();

	void createUI();
	void initUI();
	void InitData(QMap<QString, QString> &map);
private:
	void connectSlots();
	void resetMap();
	void va_listRemove(QMap<QString, QString> *qMap, int argc, ...);
	inline void initTextEdit(QMap<QString, QString> &map, QLineEdit *edit, QString str);
	void setRadioBtnEnable(bool isEnable);
	bool checkData();
	double maxData();
private slots:
	void onConfirmBtnClicked();
	void onCancelBtnClicked();
	void onRadioBtnClicked();
	void dealNumber();
	void setNumRange();
private:
//	QCheckBox *calculatePropertyCheckBox;张磊注释2017.8.1
	QCheckBox *mustClickCheckBox;
	QComboBox *codeSysComboBox;
	QComboBox *timeComboBox;
//	QLabel *scriptNameLabel;
/*	QLabel *formatLabel_2;*/
	QLabel *codeSysLabel;
	QLabel *formatLabel;
	QLabel *decimalLenLabel;
	QLabel *currentContentLabel;
	QLabel *totalLenLabel;
/*	QLabel *dateFormatLabel;*/
	QLabel *codeLabel;
	QLabel *nameLabel;
	QLabel *boundValueLabel;
	QLineEdit *nameEdit;
	QLineEdit *codeEdit;
//	QLineEdit *scriptNameEdit;张磊注释2017.8.1
	QLineEdit *decimalLenEdit;
	QLineEdit *totalLenEdit;
	QLineEdit *maxEdit;
	QLineEdit *minEdit;
/*	QLineEdit *formatEdit;*/
	QLineEdit *currentContentEdit;
	QPushButton *cancelBtn;
	QPushButton *confirmBtn;
	QRadioButton *dateRadioButton;
	QRadioButton *numValueRadioButton;
	QMap<QString, QString> *m_map;
};

#endif // STRUCTURINGDLG_H
