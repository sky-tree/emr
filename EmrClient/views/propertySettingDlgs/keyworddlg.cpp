#include "keyworddlg.h"

#include <QLayout>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include"views/globalvariable.h"
KeyWordDlg::KeyWordDlg(QWidget *parent) :
QDialog(parent), nameLabel(NULL), codeSysLabel(NULL), codeLabel(NULL), levelNumLabel(NULL), contentLabel(NULL), nameEdit(NULL), codeEdit(NULL), levelNumEdit(NULL), contentEdit(NULL)
, codeSysComboBox(NULL),  confirmBtn(NULL), cancelBtn(NULL)
,  m_map(NULL)
{
	createUI();
	initUI();
	connectSlots();
	this->setModal(true);
}

KeyWordDlg::~KeyWordDlg()
{
   
}
void KeyWordDlg::connectSlots()
{
	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));
}
void KeyWordDlg::onConfirmBtnClicked()
{
	resetMap();
	//emit QDialog::accept();
	this->close();
}
void KeyWordDlg::onCancelBtnClicked()
{
	this->close();
}
void KeyWordDlg::createUI()
{
	this->setFixedSize(QSize(323, 420));
	this->setStyleSheet(kPushBtnStyle);

	nameLabel = new QLabel(this);
	nameEdit = new QLineEdit(this);

	codeSysLabel = new QLabel(this);
	codeSysComboBox = new QComboBox(this);

	codeLabel = new QLabel(this);
	codeEdit = new QLineEdit(this);

	levelNumLabel = new QLabel(this);
	levelNumEdit = new QLineEdit(this);

	contentLabel = new QLabel(this);
	contentEdit = new QPlainTextEdit(this);

	QFormLayout* fLayout = new QFormLayout();
	fLayout->addRow(nameLabel,nameEdit);
	fLayout->addRow(codeSysLabel, codeSysComboBox);
	fLayout->addRow(codeLabel, codeEdit);
	fLayout->addRow(levelNumLabel, levelNumEdit);
	fLayout->addRow(contentLabel, contentEdit);
	fLayout->setSpacing(20);
//	titleStyleLabel = new QLabel(this);
//	titleStyleLabel->setGeometry(QRect(30, 220, 81, 16));
//	titleStyleComboBox = new QComboBox(this);//zl2017.8.1
//	titleStyleComboBox->setGeometry(QRect(110, 220, 91, 22));//zl2017.8.1
//	descriptionLabel = new QLabel(this);//zl2017.8.1
//	descriptionLabel->setGeometry(QRect(30, 290, 261, 16));//zl2017.8.1
	QHBoxLayout* hLayout = new QHBoxLayout();
	confirmBtn = new QPushButton(this);
	confirmBtn->setFixedSize(81, 23);
	cancelBtn = new QPushButton(this);
	 cancelBtn->setFixedSize(81, 23);

	hLayout->addStretch();
	hLayout->addWidget(confirmBtn); 
	hLayout->addWidget(cancelBtn);

	QVBoxLayout* vLayout = new QVBoxLayout(this);
	vLayout->addLayout(fLayout);
	vLayout->addLayout(hLayout);
	this->setLayout(vLayout);
//	visibilityCheckBox = new QCheckBox(this);//zl2017.8.1
//	visibilityCheckBox->setGeometry(QRect(220, 220, 71, 21));//zl2017.8.1
//	printCheckBox = new QCheckBox(this);//zl2017.8.1
//	printCheckBox->setGeometry(QRect(50, 250, 71, 21));//zl2017.8.1
//	frameCheckBox = new QCheckBox(this);//zl2017.8.1
//	frameCheckBox->setGeometry(QRect(170, 250, 71, 21));//zl2017.8.1
//	locationCheckBox = new QCheckBox(this);//zl2017.8.1
//	locationCheckBox->setGeometry(QRect(30, 320, 261, 21));//zl2017.8.1
//	keyWordCheckBox = new QCheckBox(this);//zl2017.8.1
//	keyWordCheckBox->setGeometry(QRect(30, 360, 271, 21));//zl2017.8.1
}

void KeyWordDlg::initUI()
{
	this->setWindowTitle(tr("固定文本"));
	nameLabel->setText(tr("名    称"));
	codeSysLabel->setText(tr("编码系统"));
	codeLabel->setText(tr("编   码"));
	levelNumLabel->setText(tr("层次序号"));
	contentLabel->setText(tr("内   容"));
//	titleStyleLabel->setText(tr("固定标题样式"));//zl2017.8.1
	codeSysComboBox->clear();
	codeSysComboBox->insertItems(0, QStringList()
		<< tr("UTF-8")
		<< tr("Unicode")
		<< tr("Unicode big endian")
		<< tr("ANSI")
		);
/***************************************************zl2017.8.1*****************************************/
// 	titleStyleComboBox->clear();
// 	titleStyleComboBox->insertItems(0, QStringList()
// 		<< tr("普通")
// 		<< tr("中级")
// 		<< tr("高级")
// 		); 
// 	descriptionLabel->setText(tr("层次序号与内容仅适用于固定标题和标题定位符"));
 	confirmBtn->setText(tr("确定"));
 	cancelBtn->setText(tr("取消"));

/***************************************************zl2017.8.1*****************************************/
// 	visibilityCheckBox->setText(tr("按钮可视"));
// 	printCheckBox->setText(tr("打印"));
// 	frameCheckBox->setText(tr("边框"));
// 	locationCheckBox->setText(tr("定位符标志（用于程序的内部精确查找定位）"));
// 	keyWordCheckBox->setText(tr("关键字的内容为其他内容的连接(对关键字有效)"));//zl2017.8.1
}
void KeyWordDlg::InitData(QMap<QString, QString> &map)
{
	m_map = &map;
	if (m_map == NULL)return;
	if (map.contains("name")){ nameEdit->setText(map["name"]); }
	if (map.contains("codeSys")){ codeSysComboBox->setCurrentIndex(map["codeSys"].toInt()); }
	if (map.contains("code")){ codeEdit->setText(map["code"]); }
	if (map.contains("levelNum")){ levelNumEdit->setText(map["levelNum"]); }
	if (map.contains("content")){ contentEdit->setPlainText(map["content"]); }

/***************************************************zl2017.8.1*****************************************/
// 	if (map.contains("titleStyle")){ titleStyleComboBox->setCurrentIndex(map["titleStyle"].toInt()); };
// 	InitCombox(map, "visible", visibilityCheckBox);
// 	InitCombox(map, "print", printCheckBox);
// 	InitCombox(map, "frame", frameCheckBox);
// 	InitCombox(map, "location", locationCheckBox);
// 	InitCombox(map, "keyWord", keyWordCheckBox);//zl2017.8.1
/********************************************************************************************/


	/*if (map.contains("visible"))
	{
		if (map["visible"] == "true"){ visibilityCheckBox->setChecked(true); }
		else
		{
			visibilityCheckBox->setChecked(false);
		}
	}
	
	if (map.contains("print"))
	{
		if (map["print"] == "true"){ printCheckBox->setChecked(true); }
		else
		{
			printCheckBox->setChecked(false);
		}
	}

	if (map.contains("frame"))
	{
		if (map["frame"] == "true"){ frameCheckBox->setChecked(true); }
		else
		{
			frameCheckBox->setChecked(false);
		}
	}

	if (map.contains("location"))
	{
		if (map["location"] == "true"){ locationCheckBox->setChecked(true); }
		else
		{
			locationCheckBox->setChecked(false);
		}
	}

	if (map.contains("keyWord"))
	{
		if (map["keyWord"] == "true"){ keyWordCheckBox->setChecked(true); }
		else
		{
			keyWordCheckBox->setChecked(false);
		}
	}*/


}
void KeyWordDlg::va_listRemove(QMap<QString, QString> *qMap, int argc, ...)
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
void KeyWordDlg::InitCombox(QMap<QString, QString> &pMap, QString str, QCheckBox *checkbox)
{
	if (pMap.contains(str))
	{
		if (pMap[str] == "true")
		{
			checkbox->setChecked(true);
		}
		else
		{
			checkbox->setChecked(false);
		}
	}
}
void KeyWordDlg::resetMap()
{
	if (m_map == NULL)
	{
		m_map = new QMap<QString, QString>();
	}   
	//m_map->clear();
	va_listRemove(m_map, 12,"dlgType","name","codeSys","code","levelNum",
		"content","titleStyle","visible","print","frame","location","keyWord");
	m_map->insert("dlgType", QString::number(keyWordDlg));
	m_map->insert("name", nameEdit->text());
	m_map->insert("codeSys", QString::number(codeSysComboBox->currentIndex()));
	m_map->insert("code", codeEdit->text());
	m_map->insert("levelNum", levelNumEdit->text());
	m_map->insert("content", contentEdit->toPlainText());

/***************************************************zl2017.8.1*****************************************/
// 	m_map->insert("titleStyle", QString::number(titleStyleComboBox->currentIndex()));
// 	if (visibilityCheckBox->isChecked()){ m_map->insert("visible", "true"); }
// 	else{ m_map->insert("visible", "true");}
// 
// 	if (printCheckBox->isChecked()){ m_map->insert("print", "true"); }
// 	else{ m_map->insert("print", "true"); }
// 
// 	if (frameCheckBox->isChecked()){ m_map->insert("frame", "true"); }
// 	else{ m_map->insert("frame", "true"); }
// 
// 	if (locationCheckBox->isChecked()){ m_map->insert("location", "true"); }
// 	else{ m_map->insert("location", "true"); }
// 
// 	if (keyWordCheckBox->isChecked()){ m_map->insert("keyWord", "true"); }
// 	else{ m_map->insert("keyWord", "true"); }//zl2017.8.1


}
