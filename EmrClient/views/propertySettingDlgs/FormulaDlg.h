#ifndef FORMULADLG_H
#define FORMULADLG_H

#include <QDialog>
#include "customDelegate/MSC_VER.h"
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class  QLineEdit;
class  QFrame;
class  QPushButton;

class FormulaDlg : public QDialog
{
    Q_OBJECT
public:
    explicit FormulaDlg(QDialog *parent = 0);
    ~FormulaDlg();

    void createUI();
    void initUI();
	void InitData(QMap<QString, QString> &map);
private:
	void resetMap(); 
private:
	QMap<QString, QString> *m_map;
CREATE_UI_BEGIN
public:
    QFrame *line;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit;
    QPushButton *cancelBtn;
    QPushButton *confirmBtn;
CREATE_UI_END

private slots:
	void onConfirmBtnClicked();
	void onCancelBtnClicked();
};

#endif // FORMULADLG_H