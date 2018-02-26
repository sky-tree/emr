#include "SelectionPopup.h"

#include "GlobalTemplate.h"

#include <QPushButton>
#include <QListWidget>
#include <QMouseEvent>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qdebug.h>
#include "globalvariable.h"
#include <qcursor.h>
#include <QApplication>
#include <QDesktopWidget>

SelectionPopup::SelectionPopup(QWidget*parent) : QDialog(parent)
{
	listWidget = NULL;
	cancelBtn = NULL;
	confirmBtn = NULL;
	createUI();
	initUI();

	this->setWindowFlags(Qt::FramelessWindowHint);

	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));

	this->setStyleSheet(kPushBtnStyle);
	//listWidget->setStyleSheet(kListViewStyle);
}

SelectionPopup::~SelectionPopup()
{

}

void SelectionPopup::createUI()
{
	if (this->objectName().isEmpty())
		this->resize(211, 333);
	confirmBtn = new QPushButton(this);
	confirmBtn->setGeometry(QRect(10, 300, 81, 23));
	cancelBtn = new QPushButton(this);
	cancelBtn->setGeometry(QRect(120, 300, 81, 23));
	listWidget = new QListWidget(this);
	listWidget->setGeometry(QRect(10, 10, 191, 271));
}

void SelectionPopup::initUI()
{
	confirmBtn->setText(tr("确定"));
	cancelBtn->setText(tr("取消"));
}

void SelectionPopup::onConfirmBtnClicked()
{
	this->close();
	QDialog::accept();
}

void SelectionPopup::onCancelBtnClicked()
{
	this->close();
}

void SelectionPopup::btnToggled(bool isChecked)
{
	int index = ((QCheckBox *)this->sender())->property("index").toInt();
	data[index]->isChecked = isChecked;

}

void SelectionPopup::initCheckBoxList()
{
	for (int i = 0; i < data.size(); i++)
	{
		QListWidgetItem * item = new QListWidgetItem(this->listWidget);
		QCheckBox * btn = new QCheckBox();
		btn->setText(data[i]->name);
		btn->setChecked(data[i]->isChecked);
		btn->setProperty("index", i);
		connect(btn, SIGNAL(toggled(bool)), this, SLOT(btnToggled(bool)));
		this->listWidget->setItemWidget(item, btn);
	}
}

void SelectionPopup::initRadioBtnList()
{
	for (int i = 0; i < data.size(); i++)
	{
		QListWidgetItem * item = new QListWidgetItem(this->listWidget);
		QRadioButton * btn = new QRadioButton();
		btn->setText(data[i]->name);
		btn->setChecked(data[i]->isChecked);
		btn->setProperty("index", i);
		connect(btn, SIGNAL(toggled(bool)), this, SLOT(btnToggled(bool)));
		this->listWidget->setItemWidget(item, btn);
	}
}

void SelectionPopup::loadListData(QString type, QList<DataType*>& data)
{
	this->data.clear();
	this->data = data;

	if (type == TempleteTypeEnumeration::RADIO)
	{
		this->setWindowTitle("单选");
		initRadioBtnList();
	}
	else if (type == TempleteTypeEnumeration::HAVE_CHOICE)
	{
		this->setWindowTitle("有无选择");
		initCheckBoxList();
	}
	else if (type == TempleteTypeEnumeration::MULTIPLE_CHOICE)
	{
		this->setWindowTitle("多选");
		initCheckBoxList();
	}
}

void SelectionPopup::getSelection(QString type, QList<DataType*> & data)
{
	SelectionPopup dialog;
	QList<DataType *> temList;
	temList.clear();
	for each (DataType* temData in data)
	{
		DataType * tem = new DataType(*temData);
		temList.push_back(tem);
	}


	dialog.loadListData(type, temList);
	dialog.resetPos();
	if (dialog.exec() == QDialog::Accepted)
	{
		deleteVectorContent<DataType>(&data);
		data = temList;
		return;
	}

	return;
}

void SelectionPopup::resetPos()
{
	QPoint cursorPoint = QCursor::pos();
	QSize currentSize = this->size();
	QSize screenSize = QApplication::desktop()->size();

	if (cursorPoint.y() + currentSize.height() + 40 > screenSize.height())
	{
		this->move(QPoint(cursorPoint.x(), cursorPoint.y() - currentSize.height()));
	}
	else
	{
		this->move(cursorPoint);
	}
}
