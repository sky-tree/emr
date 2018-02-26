#include "MacroPropertyPopup.h"
#include "globalvariable.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <qcursor.h>
#include<QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include<QFormLayout>
MacroPropertyPopup::MacroPropertyPopup(QDialog*parent) : QDialog(parent), m_map(NULL)
{
    defaultValueLabel = NULL;
    connectIDLabel = NULL;
    nameLabel = NULL;
    defaultValueEdit = NULL;
    connectIDEdit = NULL;
    nameEdit = NULL;
    cancelBtn = NULL;
    confirmBtn = NULL;
    createUI();
    initUI();
	connectSlots();
	this->setStyleSheet(kPushBtnStyle);
}

MacroPropertyPopup::~MacroPropertyPopup()
{

}

void MacroPropertyPopup::onConfirmBtnClicked()
{
	resetMap();
	this->close();
}

void MacroPropertyPopup::onCancelBtnClicked()
{
	this->close();
}
void MacroPropertyPopup::connectSlots()
{
	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));
}

void MacroPropertyPopup::createUI()
{
        if (this->objectName().isEmpty())
        this->resize(270, 160);
        nameLabel = new QLabel(this);
        nameEdit = new QLineEdit(this);

        connectIDLabel = new QLabel(this);
        connectIDEdit = new QComboBox(this);

        defaultValueLabel = new QLabel(this);
        defaultValueEdit = new QLineEdit(this);

		QFormLayout* flayout = new QFormLayout;
		flayout->addRow(nameLabel, nameEdit);
		flayout->addRow(connectIDLabel, connectIDEdit);
		flayout->addRow(defaultValueLabel, defaultValueEdit);

        confirmBtn = new QPushButton(this);
		confirmBtn->setFixedSize(81, 23);
        cancelBtn = new QPushButton(this);
		cancelBtn->setFixedSize(81, 23);

		QHBoxLayout* hlayout = new QHBoxLayout;
		hlayout->addStretch();
		hlayout->addWidget(confirmBtn);
		hlayout->addWidget(cancelBtn);

		QVBoxLayout* vlayout = new QVBoxLayout(this);
		vlayout->addLayout(flayout);
		vlayout->addLayout(hlayout);
		this->setLayout(vlayout);
}

void MacroPropertyPopup::initUI()
{
        this->setWindowTitle(tr("属性"));
        nameLabel->setText(tr("宏名称"));
        connectIDLabel->setText(tr("关联HIS数据ID"));
        defaultValueLabel->setText(tr("默认值"));
        confirmBtn->setText(tr("确定"));
        cancelBtn->setText(tr("取消"));
}

void MacroPropertyPopup::getData(QString &name, QString &id, QString &defaultValue)
{
	MacroPropertyPopup dialog;
	dialog.move(QCursor::pos());
	name = dialog.nameEdit->text();
	id = dialog.connectIDEdit->currentText();
	defaultValue = dialog.defaultValueEdit->text();
	if (dialog.exec() == QDialog::Accepted)
	{
		return;
	}
}
void MacroPropertyPopup::InitData(QMap<QString, QString> &map)
{
	m_map = &map;
	if (m_map == NULL)return;
	if (map.contains("value"))
	{
		defaultValueEdit->setText(map["value"]);
	}
	if (map.contains("name"))
	{
		nameEdit->setText(map["name"]);
	}
	if (map.contains("id"))
	{
		connectIDEdit->setCurrentText(map["id"]);
	}
}
void MacroPropertyPopup::resetMap()
{
	if (m_map == NULL)
	{
		m_map = new QMap<QString, QString>();
	}
	if (m_map->contains("dlgType")==false)
	{
		m_map->insert("dlgType", QString::number(macroPropertyPopupDlg));
	}
	QStringList strlist;
	strlist << "name" << "id" << "value";
	for (int i = 0; i < strlist.count();i++)
	{
		if (m_map->contains(strlist.at(i)))
		{
			m_map->remove(strlist.at(i));
		}
	}
	m_map->insert("name", nameEdit->text());
	m_map->insert("id", connectIDEdit->currentText());
	m_map->insert("value", defaultValueEdit->text());
}
