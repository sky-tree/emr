#include "CreateTableDlg.h"
#include "globalvariable.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <qcursor.h>
#include <QValidator>

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

	this->setStyleSheet(kPushBtnStyle);
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
        this->resize(245, 175);
        connectIDLabel = new QLabel(this);
        connectIDLabel->setGeometry(QRect(20, 50, 91, 21));
        nameLabel = new QLabel(this);
        nameLabel->setGeometry(QRect(20, 10, 54, 21));
        confirmBtn = new QPushButton(this);
        confirmBtn->setGeometry(QRect(20, 140, 81, 23));
        cancelBtn = new QPushButton(this);
        cancelBtn->setGeometry(QRect(140, 140, 81, 23));
        connectIDEdit = new QLineEdit(this);
        connectIDEdit->setGeometry(QRect(60, 50, 161, 20));
		connectIDEdit->setText("2");
        nameEdit = new QLineEdit(this);
        nameEdit->setGeometry(QRect(60, 10, 161, 20));
		nameEdit->setText("4");
        checkBox = new QCheckBox(this);
        checkBox->setGeometry(QRect(60, 100, 141, 16));
		checkBox->setChecked(true);

		nameEdit->setValidator(new QIntValidator(1, 999, nameEdit));
		connectIDEdit->setValidator(new QIntValidator(1, 50, connectIDEdit));
		connect(nameEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
		connect(connectIDEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString )));
}

void CreateTableDlg::initUI()
{
        this->setWindowTitle(tr("表格"));
        connectIDLabel->setText(tr("列数"));
        nameLabel->setText(tr("行数"));
        confirmBtn->setText(tr("确定"));
        cancelBtn->setText(tr("取消"));
        checkBox->setText(tr("是否有边框"));
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
		if (string.toInt() > 34)
		{
			nameEdit->setText("34");
		}
	}
	else
	{
		if (string.toInt() > 50)
		{
			connectIDEdit->setText("60"); 
		}
	}
}

