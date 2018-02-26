#ifndef TABLEPROPERTYDLG_H
#define TABLEPROPERTYDLG_H

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
class QFrame;

class TablePropertyDlg : public QDialog
{
    Q_OBJECT

public:
	explicit TablePropertyDlg(QWidget *parent = 0);
	~TablePropertyDlg();
	void InitData(QMap<QString, QString> &map);	
private:
	void createUI();
	void initUI();
	void connectSlots();
	void resetMap();
	void va_listRemove(QMap<QString, QString> *qMap, int argc, ...);
private:
	QRadioButton *noFrameRadioBtn;
	QRadioButton *haveFrameRadioBtn;

	QWidget *alignmentRadioBtnContainer;
	QWidget *frameRadioBtnContainer;

	QCheckBox * mCanDragLine;

	QRadioButton *leftRadioBtn;
	QRadioButton *centerRadioBtn;
	QRadioButton *rightRadioBtn;

	QFrame *line;

	QLabel *desLabel;
	QLabel *cellMarginLabel;
	QLabel *mmUnitLabel;
	QLabel *borderWidthLabel;

	QLineEdit *cellMarginEdit;
	QLineEdit *borderWidthEdit;

	QPushButton *confirmBtn;
	QPushButton *cancelBtn;


	QMap<QString, QString> *m_map;
private slots:
	void onConfirmBtnClicked();
	void onCancelBtnClicked();
	void borderContainerBtnsClicked();

};

#endif // TABLEPROPERTYDLG_H
