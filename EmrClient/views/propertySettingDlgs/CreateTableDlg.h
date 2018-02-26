#ifndef CREATETABLEDLG_H
#define CREATETABLEDLG_H

#include <QDialog>
#include "customDelegate/MSC_VER.h"

class  QLabel;
class  QPushButton;
class  QLineEdit;
class  QCheckBox;

class CreateTableDlg : public QDialog
{
    Q_OBJECT
public:
    explicit CreateTableDlg(QDialog *parent = 0);
    ~CreateTableDlg();

    void createUI();
    void initUI();

CREATE_UI_BEGIN
public:
    QCheckBox *checkBox;
    QLabel *nameLabel;
    QLabel *connectIDLabel;
    QLineEdit *nameEdit;
    QLineEdit *connectIDEdit;
    QPushButton *cancelBtn;
    QPushButton *confirmBtn;
	QMap<QString, QString> *m_map;
CREATE_UI_END

private slots:
	void onConfirmBtnClicked();
	void onCancelBtnClicked();

	void onTextChanged(QString);
public:
	QMap<QString, QString> getMap()const;

};

#endif // CREATETABLEDLG_H