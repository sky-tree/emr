#ifndef KEYWORDDLG_H
#define KEYWORDDLG_H

#include <QWidget>
#include "customDelegate/MSC_VER.h"
#include "qdialog.h"
#include<QMap>
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QTextEdit;
class QCheckBox;
class QPlainTextEdit;

class KeyWordDlg : public QDialog
{
    Q_OBJECT

public:
    explicit KeyWordDlg(QWidget *parent = 0);
    ~KeyWordDlg();

	void createUI();
	void initUI();
	void InitData(QMap<QString, QString> &map);
private:
	void connectSlots();
	void resetMap();
	void va_listRemove(QMap<QString, QString> *qMap, int argc, ...);
	inline void InitCombox(QMap<QString, QString> &pMap, QString str,QCheckBox *checkbox);
	private slots:
	void onConfirmBtnClicked();
	void onCancelBtnClicked();
private:
	QLabel *nameLabel;
	QLabel *codeSysLabel;
	QLabel *codeLabel;
	QLabel *levelNumLabel;
	QLabel *contentLabel;
//	QLabel *titleStyleLabel;//zl2017.8.1
//	QLabel *descriptionLabel;//zl2017.8.1

	QLineEdit *nameEdit;
	QLineEdit *codeEdit;
	QLineEdit *levelNumEdit;
	QPlainTextEdit *contentEdit;

	QComboBox *codeSysComboBox;
//	QComboBox *titleStyleComboBox;//zl2017.8.1
	
	QPushButton *confirmBtn;
	QPushButton *cancelBtn;

//	QCheckBox *visibilityCheckBox;//zl2017.8.1
//	QCheckBox *printCheckBox;//zl2017.8.1
//	QCheckBox *frameCheckBox;//zl2017.8.1
//	QCheckBox *locationCheckBox;//zl2017.8.1
//	QCheckBox *keyWordCheckBox;//zl2017.8.1

	QMap<QString, QString> *m_map;
};

#endif // KEYWORDDLG_H
