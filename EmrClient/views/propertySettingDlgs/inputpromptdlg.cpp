#include "inputpromptdlg.h"
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include"views/globalvariable.h"
InputPromptDlg::InputPromptDlg(QWidget *parent) :
QDialog(parent), nameEdit(NULL), codeEdit(NULL), infoEdit(NULL)
, codeSysComboBox(NULL), codeSysLabel(NULL), codeLabel(NULL), nameLabel(NULL), inputInfoLabel(NULL)
, confirmBtn(NULL), cancelBtn(NULL), m_map(NULL)
{
	createUI();
	initUI();
	connectSlots();
	this->setModal(true);
	this->setStyleSheet(kPushBtnStyle);
}

InputPromptDlg::~InputPromptDlg()
{
	
}
void InputPromptDlg::connectSlots()
{
	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));
}
void InputPromptDlg::onConfirmBtnClicked()
{
	resetMap();
	this->close();
}
void InputPromptDlg::onCancelBtnClicked()
{
	this->close();
}
void InputPromptDlg::createUI()
{
	this->setFixedSize(304, 380);
	nameLabel = new QLabel(this);
	nameEdit = new QLineEdit(this);

	codeSysLabel = new QLabel(this);
	codeSysComboBox = new QComboBox(this);

	codeLabel = new QLabel(this);
	codeEdit = new QLineEdit(this);

	QFormLayout* fLayout = new QFormLayout();
	fLayout->addRow(nameLabel, nameEdit);
	fLayout->addRow(codeSysLabel, codeSysComboBox);
	fLayout->addRow(codeLabel, codeEdit);

	
	inputInfoLabel = new QLabel(this);

	QHBoxLayout* hLayout1 = new QHBoxLayout();
	hLayout1->addWidget(inputInfoLabel);
	hLayout1->addStretch();

	infoEdit = new QTextEdit(this);
	infoEdit->resize(251, 131);

	confirmBtn = new QPushButton(this);
	confirmBtn->setFixedSize(81, 23);
	cancelBtn = new QPushButton(this);
	cancelBtn->setFixedSize(81, 23);

	QHBoxLayout* hLayout2 = new QHBoxLayout();
	hLayout2->addStretch();
	hLayout2->addWidget(confirmBtn); 
	hLayout2->addWidget(cancelBtn);

	QVBoxLayout* vLayout = new QVBoxLayout(this);
	vLayout->addLayout(fLayout);
	vLayout->addLayout(hLayout1);
	vLayout->addWidget(infoEdit);
	vLayout->addLayout(hLayout2);

	this->setLayout(vLayout);
}

void InputPromptDlg::initUI()
{
	this->setWindowTitle(tr("录入提示属性设置"));
	codeSysLabel->setText(tr("编码系统"));
	codeSysComboBox->clear();
	codeSysComboBox->insertItems(0, QStringList()
		<< tr("UTF-8")
		<< tr("Unicode")
		<< tr("Unicode big endian")
		<< tr("ANSI")
		);
	codeLabel->setText(tr("编    码"));
	nameLabel->setText(tr("名    称"));
	inputInfoLabel->setText(tr("录入提示输入信息"));
	confirmBtn->setText(tr("确定"));
	cancelBtn->setText(tr("取消"));
}
//初始化数据
void InputPromptDlg::InitData(QMap<QString, QString> &map)
{
	m_map = &map;
	if (m_map == NULL)return;
	if (map.contains("name")){ nameEdit->setText(map["name"]); }
	if (map.contains("codeSys")){ codeSysComboBox->setCurrentIndex(map["codeSys"].toInt()); }
	if (map.contains("code")){ codeEdit->setText(map["code"]); }
	if (map.contains("info")){ infoEdit->setText(map["info"]); }
}

void InputPromptDlg::resetMap()
{
	if (m_map == NULL)
	{
		m_map = new QMap<QString, QString>();
	}
	//m_map->clear();
	va_listRemove(m_map, 5, "dlgType","name","codeSys","code","info");
	m_map->insert("dlgType", QString::number(inputPromptDlg));
	m_map->insert("name", nameEdit->text());
	m_map->insert("codeSys", QString::number(codeSysComboBox->currentIndex()));
	m_map->insert("code", codeEdit->text());
	m_map->insert("info", infoEdit->toPlainText());
}
void InputPromptDlg::va_listRemove(QMap<QString, QString> *qMap, int argc, ...)
{
	va_list va;
	va_start(va, argc);
	for (int i = 0; i != argc; ++i)
	{
		const char * str = va_arg(va, const char*);
		QString str1(str);
		if (qMap->contains(str1))
		{
			qMap->remove(str1);
		}
	}
	va_end(va);
}