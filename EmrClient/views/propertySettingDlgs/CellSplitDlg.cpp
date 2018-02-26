#include "CellSplitDlg.h"

#include "globalvariable.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include<QValidator>
CellSplitDlg::CellSplitDlg(QDialog*parent) : QDialog(parent)
{
	buttonBox = NULL;
	columnLabel = NULL;
	rowLabel = NULL;
	columnEdit = NULL;
	rowEdit = NULL;
	column = 1;
	row = 1;
	createUI();
	initUI();

	QPushButton * btnOk = buttonBox->button(QDialogButtonBox::Ok);
	btnOk->setText(tr("确定"));
	btnOk->setMinimumSize(QSize(74, 24));
	connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
	QPushButton * btnCancel = buttonBox->button(QDialogButtonBox::Cancel);
	btnCancel->setText(tr("取消"));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
	btnCancel->setMinimumSize(QSize(74, 24));
	this->setStyleSheet(kPushBtnStyle);
}

CellSplitDlg::~CellSplitDlg()
{

}

void CellSplitDlg::createUI()
{
	QValidator *editValidator = new QIntValidator(1, 999999, this);
	if (this->objectName().isEmpty())
		this->resize(234, 120);
	this->setMinimumSize(QSize(234, 120));
	this->setMaximumSize(QSize(234, 120));
	buttonBox = new QDialogButtonBox(this);
	buttonBox->setGeometry(QRect(-250, 80, 461, 32));
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
	rowLabel = new QLabel(this);
	rowLabel->setGeometry(QRect(20, 10, 54, 21));
	columnLabel = new QLabel(this);
	columnLabel->setGeometry(QRect(20, 50, 54, 21));
	rowEdit = new QLineEdit(this);
	rowEdit->setGeometry(QRect(70, 10, 141, 20));
	rowEdit->setValidator(editValidator);
	columnEdit = new QLineEdit(this);
	columnEdit->setGeometry(QRect(70, 50, 141, 20));
	columnEdit->setValidator(editValidator);
	rowEdit->setText("1");
	columnEdit->setText("2");
}

void CellSplitDlg::accept()
{
	if (columnEdit->text() != "")
	{
		column = columnEdit->text().toInt();
	}
	if (rowEdit->text() != "")
	{
		row = rowEdit->text().toInt();
	}
	QDialog::accept();
}

void CellSplitDlg::initUI()
{
	this->setWindowTitle(tr("拆分单元格"));
	rowLabel->setText(tr("行数:"));
	columnLabel->setText(tr("列数:"));
}
//void CellSplitDlg::close()
//{
//	this->close();
//}
