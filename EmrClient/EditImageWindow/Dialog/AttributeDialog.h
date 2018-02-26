#ifndef ATTRIBUTEDIALOG_H
#define ATTRIBUTEDIALOG_H

#include <QDialog>
#include "ui_AttributeDialog.h"

class AttributeDialog : public QDialog
{
	Q_OBJECT

public:
	AttributeDialog(QWidget *parent = 0);
	~AttributeDialog();
	void fillColorList(QComboBox *comboBox);
private:
	Ui::AttributeDialog ui;
	QStringList		mColorList;

signals:

	public slots :
		void actionOK();
		void actionCancel();
		void actionCheckBox(int state);
		void actionComboBox_3(QString state);
};

#endif // ATTRIBUTEDIALOG_H
