#include "StructuringPopup.h"
#include "globalvariable.h"
 
#include <QDateTimeEdit>
#include <QPushButton>
#include <QLineEdit>
#include "MessageBox.h"
#include <iostream>
using namespace std;

StructuringPopup::StructuringPopup()
{
    dateEdit = NULL;
    lineEdit = NULL;
    cancelBtn = NULL;
    confirmBtn = NULL;

	length = 0;
	decimalLen = 0;
	min = 0;
	max = 0;
	timeStr = "";
	currentStr = "";
	oldStr = "";

    createUI();
    initUI();

	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));

	this->setStyleSheet(kPushBtnStyle);
}

StructuringPopup::~StructuringPopup()
{

}

void StructuringPopup::createUI()
{
        if (this->objectName().isEmpty())
        this->resize(242, 73);
        dateEdit = new QDateTimeEdit(this);
        dateEdit->setGeometry(QRect(10, 10, 220, 22));
        dateEdit->setCalendarPopup(true);
        confirmBtn = new QPushButton(this);
        confirmBtn->setGeometry(QRect(70, 40, 75, 23));
        cancelBtn = new QPushButton(this);
        cancelBtn->setGeometry(QRect(150, 40, 75, 23));
        lineEdit = new QLineEdit(this);
        lineEdit->setGeometry(QRect(10, 10, 220, 20));
}

void StructuringPopup::initUI()
{
        this->setWindowTitle(tr("Dialog"));
        //dateEdit->setDisplayFormat(tr("yyyy-M-d"));
        confirmBtn->setText(tr("确定"));
        cancelBtn->setText(tr("取消"));
}

QString StructuringPopup::getInputNumber(QString data, int length, int decimalLen, double min, double max)
{
	StructuringPopup dlg;
	dlg.length = length;
	dlg.decimalLen = decimalLen;
	dlg.min = min;
	dlg.max = max;
	dlg.dateEdit->setVisible(false);
	dlg.revisePos();
	dlg.oldStr = data;
	dlg.lineEdit->setText(data);
	dlg.setWindowFlags(Qt::FramelessWindowHint);
	dlg.setWindowTitle(tr("输入数值"));

	//if (dlg.exec() == QDialog::Accepted)
	//{
	//	return dlg.getText();
	//}
	dlg.exec();

	return dlg.currentStr;
}

QString StructuringPopup::getDate(QString timeStr, QString format)
{
	StructuringPopup dlg;
	dlg.lineEdit->setVisible(false);
	dlg.timeStr = timeStr; 
	dlg.dateEdit->setDisplayFormat(format);
	dlg.dateEdit->setDateTime(QDateTime::fromString(timeStr, format));
	dlg.oldStr = timeStr;
	dlg.revisePos();
	dlg.setWindowFlags(/*Qt::Popup | */Qt::FramelessWindowHint);
	dlg.setWindowTitle(tr("输入日期"));

	//if (dlg.exec() == QDialog::Accepted)
	//{
	//	return dlg.getText();
	//}

	//return "";
	dlg.exec();

	return dlg.currentStr;
}

bool StructuringPopup::checkData()
{
	QString text = lineEdit->text();
	if (text.toDouble() > max || text.toDouble() < min)
	{
		MessageBox::showAboutMessageBox(QStringLiteral("提示"), QStringLiteral("超过有效值范围:%1 ~ %2").arg(min).arg(max));
		return false;
	}
	else if (text.length() > length)
	{
		MessageBox::showAboutMessageBox(QStringLiteral("提示"), QStringLiteral("超过最大输入长度: %1").arg(length));
		return false;
	}
	else if (text.indexOf(".") != -1)
	{
		if ((text.mid(text.lastIndexOf(".") + 1, text.length() - 1)).length() > decimalLen)
		{
			MessageBox::showAboutMessageBox(QStringLiteral("提示"), QStringLiteral("超过小数输入位数: %1").arg(decimalLen));
			return false;
		}
	}

	return true;
}

void StructuringPopup::revisePos()
{
	this->move(QCursor::pos());
}

QString StructuringPopup::getText()
{
	if (lineEdit->isVisible())
	{
		return lineEdit->text();
	}

	return dateEdit->text();
}

void StructuringPopup::onConfirmBtnClicked()
{
	if (lineEdit->isVisible())
	{
		if (!checkData())
		{
			//this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
			return;
		}

		double	value = lineEdit->text().toDouble();
		char rule[32];
		memset(rule, 0, sizeof(rule));
		sprintf(rule, "%%%d.%df", this->length, this->decimalLen);
		char result[256];
		memset(result, 0, sizeof(result));
		sprintf(result, rule, value);
		currentStr = QString(result).trimmed();
	}
	else
	{
		currentStr = dateEdit->text();
	}


	this->close();
}

void StructuringPopup::onCancelBtnClicked()
{
	currentStr = oldStr;
	this->close();
}
