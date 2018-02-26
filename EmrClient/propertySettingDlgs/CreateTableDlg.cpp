#include "CreateTableDlg.h"
//#include "globalvariable.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <qcursor.h>
#include <QValidator>
#include "globalvariable.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

CreateTableDlg::CreateTableDlg(QDialog*parent) : QDialog(parent), m_map(NULL)
{
    checkBox = NULL;
    nameLabel = NULL;
    connectIDLabel = NULL;
    nameEdit = NULL;
    connectIDEdit = NULL;
    cancelBtn = NULL;
    confirmBtn = NULL;

    createUI();
    initUI();

	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));

	//this->setStyleSheet(kPushBtnStyle);
}

CreateTableDlg::~CreateTableDlg()
{
	if (connectIDEdit)
	{
		delete connectIDEdit;
		connectIDEdit = NULL;
	}
	if (nameEdit)
	{
		delete nameEdit;
		nameEdit = NULL;
	}
	if (nameLabel)
	{
		delete nameLabel;
		nameLabel = NULL;
	}
	if (confirmBtn)
	{
		delete confirmBtn;
		confirmBtn = NULL;
	}
	if (cancelBtn)
	{
		delete cancelBtn;
		cancelBtn = NULL;
	}
	if (checkBox)
	{
		delete checkBox;
		checkBox = NULL;
	}
	if (m_map)
	{
		delete m_map;
		m_map = NULL;
	}
}

void CreateTableDlg::createUI()
{
        if (this->objectName().isEmpty())
        this->resize(200, 140);
        connectIDLabel = new QLabel(this);
		connectIDEdit = new QLineEdit(this);
		connectIDEdit->setText("2");

        nameLabel = new QLabel(this);
		nameEdit = new QLineEdit(this);
		nameEdit->setText("4");

		QFormLayout* flayout = new QFormLayout;
		flayout->addRow(connectIDLabel, connectIDEdit);
		flayout->addRow(nameLabel, nameEdit);

		checkBox = new QCheckBox(this);
		checkBox->setChecked(true);

		QHBoxLayout* hlayout1 = new QHBoxLayout;
		hlayout1->addStretch();
		hlayout1->addWidget(checkBox);
		hlayout1->addStretch();

        confirmBtn = new QPushButton(this);
		confirmBtn->setFixedSize(81, 23);
        cancelBtn = new QPushButton(this);
		cancelBtn->setFixedSize(81, 23);

		QHBoxLayout* hlayout2 = new QHBoxLayout;
		hlayout2->addWidget(confirmBtn);
		hlayout2->addWidget(cancelBtn);

		QVBoxLayout* layout = new QVBoxLayout(this);
		layout->addLayout(flayout);
		layout->addLayout(hlayout1);
		layout->addLayout(hlayout2);
		this->setLayout(layout);
       
        

		this->setStyleSheet(kPushBtnStyle);

		nameEdit->setValidator(new QIntValidator(1, 999, this));
		connectIDEdit->setValidator(new QIntValidator(1, 999, this));
		connect(nameEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
		connect(connectIDEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString )));
}

void CreateTableDlg::initUI()
{
        this->setWindowTitle(QStringLiteral("表格"));
        connectIDLabel->setText(QStringLiteral("列数"));
        nameLabel->setText(QStringLiteral("行数"));
        confirmBtn->setText(QStringLiteral("确定"));
        cancelBtn->setText(QStringLiteral("取消"));
        checkBox->setText(QStringLiteral("是否有边框"));
}

void CreateTableDlg::onConfirmBtnClicked()
{
	if (m_map == NULL)
	{
		m_map = new QMap<QString, QString>();
	}
	//m_map->clear();
	QStringList strlist;
	strlist<< "row" << "column" << "isFrame";
	for (int i = 0; i < strlist.count(); i++)
	{
		if (m_map->contains(strlist.at(i)))
		{
			m_map->remove(strlist.at(i));
		}
	}
	m_map->insert("row", nameEdit->text());
	m_map->insert("column", connectIDEdit->text());
	if (checkBox->isChecked())
	{
		m_map->insert("isFrame", "true");
	}
	else
	{
		m_map->insert("isFrame", "false");
	}
	emit QDialog::accept();
	this->close();
}

void CreateTableDlg::onCancelBtnClicked()
{
	this->close();
}
QMap<QString,QString> CreateTableDlg::getMap()const
{
	return *m_map;
}

void CreateTableDlg::onTextChanged(QString string)
{
	QObject * obj = this->sender();
	if (obj == nameEdit)
	{
		if (string.toInt() > 99)
		{
			nameEdit->setText("99");
		}
	}
	else
	{
		if (string.toInt() > 99)
		{
			connectIDEdit->setText("99"); 
		}
	}
}

