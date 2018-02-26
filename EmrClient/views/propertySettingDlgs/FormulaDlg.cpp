#include "FormulaDlg.h"

#include <QLineEdit>
#include <QFrame>
#include <QPushButton>
#include "globalvariable.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
FormulaDlg::FormulaDlg(QDialog*parent): QDialog(parent)
{
    line = NULL;
    lineEdit_2 = NULL;
    lineEdit = NULL;
    cancelBtn = NULL;
    confirmBtn = NULL;
	m_map = NULL;
    createUI();
    initUI();

	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));
}

FormulaDlg::~FormulaDlg()
{

}

void FormulaDlg::createUI()
{
        if (this->objectName().isEmpty()) 
        this->resize(170, 150);
        lineEdit = new QLineEdit(this);
        lineEdit_2 = new QLineEdit(this);
        line = new QFrame(this);
        line->setLineWidth(1);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        confirmBtn = new QPushButton(this);
		confirmBtn->setFixedSize(81, 23);
        cancelBtn = new QPushButton(this);
		cancelBtn->setFixedSize(81, 23);

		QHBoxLayout* hlayout = new QHBoxLayout;
		hlayout->addWidget(confirmBtn);
		hlayout->addWidget(cancelBtn);

		QVBoxLayout* vlayout = new QVBoxLayout(this);
		vlayout->addWidget(lineEdit);
		vlayout->addWidget(line);
		vlayout->addWidget(lineEdit_2);
		vlayout->addLayout(hlayout);
		this->setLayout(vlayout);
		this->setStyleSheet(kPushBtnStyle);
}

void FormulaDlg::initUI()
{
        this->setWindowTitle(tr("月经史公式"));
        confirmBtn->setText(tr("确定"));
        cancelBtn->setText(tr("取消"));
}

void FormulaDlg::onConfirmBtnClicked()
{
	resetMap();
	QDialog::accept();
}

void FormulaDlg::onCancelBtnClicked()
{
	this->close();
}
void FormulaDlg::resetMap()
{
	if (m_map == NULL)
	{
		m_map = new QMap<QString, QString>();
	}
	if (m_map->contains("topValue"))
	{
		m_map->remove("topValue");
	}
	if (m_map->contains("botValue"))
	{
		m_map->remove("botValue");
	}
	m_map->insert("topValue", lineEdit->text());
	m_map->insert("botValue", lineEdit_2->text());
}
void FormulaDlg::InitData(QMap<QString, QString> &map)
{
	m_map = &map;
	if (map.contains("topValue"))
	{
		lineEdit->setText(map["topValue"]);
	}
	if (map.contains("botValue"))
	{
		lineEdit_2->setText(map["botValue"]);
	}
}