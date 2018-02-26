#ifndef CELLPROPERTYDLG_H
#define CELLPROPERTYDLG_H

#include <QWidget>
#include "customDelegate/MSC_VER.h"
#include<QMap>
#include"qdialog.h"
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

namespace Ui {
class CellPropertyDlg;
}

class CellPropertyDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CellPropertyDlg(QWidget *parent = 0);
    ~CellPropertyDlg();

	void createUI();
	void initUI();
	void InitData(QMap<QString, QString> &map);
private:
	void resetMap();
	void va_listRemove(QMap<QString,QString> *qMap, int argc,...);
	inline void getCheckBox(QMap<QString, QString>*map, QCheckBox *checkBox, QString str);
	inline void initCheckBox(QMap<QString, QString>&map, QCheckBox *checkBox, QString str);
public slots:
    void checkBtnChecked(bool state);
private slots:
void confirmBtnClicked();
void cancelBtnClicked();

private:
	void connectSlots();

private:
    QWidget * demonstrationWidget;
	QLineEdit *columnWidthEdit;
    QString demoMainStyle;

	QLabel *cellSetLabel;
	QLabel *columnWidthLabel;
	QLabel *unitLabel;
	QLabel *cellLineLabel;

	QCheckBox *topCheckBox;
	QCheckBox *leftCheckBox;
	QCheckBox *rightCheckBox;
	QCheckBox *bottomCheckBox;

	QPushButton *confirmBtn;
	QPushButton *cancelBtn;

	QMap<QString, QString> *m_map;
};

#endif // CELLPROPERTYDLG_H
