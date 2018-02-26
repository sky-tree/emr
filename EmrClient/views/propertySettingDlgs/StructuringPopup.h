#ifndef STRUCTURINGPOPUP_H
#define STRUCTURINGPOPUP_H
 
#include <QDialog>
#include "customDelegate/MSC_VER.h"

class  QDateTimeEdit;
class  QPushButton;
class  QLineEdit;

class StructuringPopup : public QDialog
{
    Q_OBJECT
public:
    explicit StructuringPopup();
    ~StructuringPopup();

    void createUI();
    void initUI();

	bool checkData();
	void revisePos();
	QString getText();

	static QString getInputNumber(QString data, int length, int decimalLen, double min, double max);
	static QString getDate(QString timeStr, QString format = "yyyy-M-d");

CREATE_UI_BEGIN
public:
    QDateTimeEdit *dateEdit;
    QLineEdit *lineEdit;
    QPushButton *cancelBtn;
    QPushButton *confirmBtn;
CREATE_UI_END

public slots:
void onConfirmBtnClicked();
void onCancelBtnClicked();

public:
	int length;
	int decimalLen;
	double min;
	double max;
	QString timeStr;
	QString currentStr;
	QString oldStr;
};

#endif // STRUCTURINGPOPUP_H