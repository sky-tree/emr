#ifndef INPUTPROMPTDLG_H
#define INPUTPROMPTDLG_H

#include <QWidget>
#include "customDelegate/MSC_VER.h"
#include"qdialog.h"
#include<QMap>
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QTextEdit;
 
class InputPromptDlg : public QDialog
{
    Q_OBJECT

public:
    explicit InputPromptDlg(QWidget *parent = 0);
    ~InputPromptDlg();

	void createUI();
	void initUI();
	void InitData(QMap<QString, QString> &map);
private:
	void connectSlots();
	void resetMap();
	void va_listRemove(QMap<QString, QString> *qMap, int argc, ...);
	private slots:
	void onConfirmBtnClicked();
	void onCancelBtnClicked();
private:
	QLineEdit *nameEdit;
	QLineEdit *codeEdit;
	QTextEdit *infoEdit;

	QComboBox *codeSysComboBox;
	
	QLabel *codeSysLabel;
	QLabel *codeLabel;
	QLabel *nameLabel;
	QLabel *inputInfoLabel;

	QPushButton *confirmBtn;
	QPushButton *cancelBtn;
	QMap<QString, QString> *m_map;
};

#endif // INPUTPROMPTDLG_H
