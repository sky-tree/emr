#ifndef PAGESETTINGDLG_H
#define PAGESETTINGDLG_H

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

class PageSettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PageSettingDlg(QWidget *parent = 0);
    ~PageSettingDlg();

	void createUI();
	void initUI();
	void InitData();
private:
	void connectSlots();
	private slots:
	void onConfirmBtnClicked();
	void onCancelBtnClicked();
	void onComboxActivated(QString name);
	void onRadioBtnClicked();

private:
	QLabel *pageTypeLabel;
	QLabel *upLabel;
//	QLabel *widthTitleLabel;
	QLabel *downLabel;
	QLabel *leftLabel;
//	QLabel *marginUnitTitleLabel;
	QLabel *rightLabel;
	QLabel *headerLabel;
	QLabel *heightLabel;
	QLabel *widthLabel;
//	QLabel *marginTitleLabel;
//	QLabel *pageTitleLabel;
	QLabel *footerLabel;
//	QLabel *pageUnitTitleLabel;
	QLabel *directionLabel;

 	QComboBox *pageUnitComBox;
// 	QComboBox *marginUnitComBox;
// 	QComboBox *sheetTypeComBox;
	
	QRadioButton *horizontalRadioBtn;
	QRadioButton *verticalRadioBtn;

	QLineEdit *rightEdit;
	QLineEdit *widthEdit;
	QLineEdit *heightEdit;
	QLineEdit *downEdit;
	QLineEdit *footerEdit;
	QLineEdit *upEdit;
	QLineEdit *headerEdit;
	QLineEdit *leftEdit;
	
	QPushButton *cancelBtn;
	QPushButton *confirmBtn;

};

#endif // PAGESETTINGDLG_H
