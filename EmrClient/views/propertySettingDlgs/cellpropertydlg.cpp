#include "cellpropertydlg.h"
#include <QDebug>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include"qvalidator.h"
#include"views/globalvariable.h"
CellPropertyDlg::CellPropertyDlg(QWidget *parent) :
QDialog(parent), demonstrationWidget(NULL), columnWidthEdit(NULL)
, cellSetLabel(NULL), columnWidthLabel(NULL), unitLabel(NULL), cellLineLabel(NULL)
, topCheckBox(NULL), leftCheckBox(NULL), rightCheckBox(NULL), bottomCheckBox(NULL)
, confirmBtn(NULL), cancelBtn(NULL), m_map(NULL)
{
	createUI();
	initUI();
	connectSlots();
	this->setModal(true);
}

CellPropertyDlg::~CellPropertyDlg()
{

}

void CellPropertyDlg::createUI()
{
	this->resize(463, 258);
	this->setMinimumSize(QSize(463, 258));
	this->setMaximumSize(QSize(463, 258));
	cellSetLabel = new QLabel(this);
	cellSetLabel->setGeometry(QRect(20, 10, 101, 16));
	columnWidthLabel = new QLabel(this);
	columnWidthLabel->setGeometry(QRect(20, 120, 91, 16));
	columnWidthEdit = new QLineEdit(this);
	columnWidthEdit->setGeometry(QRect(20, 150, 113, 20));

	QValidator *borderValidator = new QIntValidator(0, 999, this);
	columnWidthEdit->setValidator(borderValidator);

	unitLabel = new QLabel(this);
	unitLabel->setGeometry(QRect(140, 150, 91, 16));
	cellLineLabel = new QLabel(this);
	cellLineLabel->setGeometry(QRect(290, 10, 81, 16));
	demonstrationWidget = new QWidget(this);
	demonstrationWidget->setGeometry(QRect(260, 80, 120, 80));
	demonstrationWidget->setStyleSheet(QStringLiteral(""));
	topCheckBox = new QCheckBox(this);
	topCheckBox->setGeometry(QRect(290, 60, 71, 16));
	leftCheckBox = new QCheckBox(this);
	leftCheckBox->setGeometry(QRect(190, 110, 61, 16));
	rightCheckBox = new QCheckBox(this);
	rightCheckBox->setGeometry(QRect(390, 110, 71, 16));
	bottomCheckBox = new QCheckBox(this);
	bottomCheckBox->setGeometry(QRect(290, 170, 71, 16));
	confirmBtn = new QPushButton(this);
	confirmBtn->setGeometry(QRect(270, 220, 75, 23));
	cancelBtn = new QPushButton(this);
	cancelBtn->setGeometry(QRect(360, 220, 75, 23));
}

void CellPropertyDlg::initUI()
{
	connect(leftCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBtnChecked(bool)));
	connect(topCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBtnChecked(bool)));
	connect(rightCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBtnChecked(bool)));
	connect(bottomCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBtnChecked(bool)));

	demoMainStyle = "QWidget {border-width: 2px;border-style: dashed;border-color:gray}";
	demonstrationWidget->setStyleSheet(demoMainStyle);

	this->setWindowTitle(tr("单元格属性"));
	cellSetLabel->setText(tr("单元格列宽度调整"));
	columnWidthLabel->setText(tr("列表宽度设置"));
	columnWidthEdit->setPlaceholderText(tr("0"));
	unitLabel->setText(tr("毫米(最大为999)"));
	cellLineLabel->setText(tr("单元格边框线"));
	topCheckBox->setText(tr("上边框"));
	leftCheckBox->setText(tr("左边框"));
	rightCheckBox->setText(tr("右边框"));
	bottomCheckBox->setText(tr("下边框"));
	confirmBtn->setText(tr("确定"));
	cancelBtn->setText(tr("取消"));
}
//初始化数据
void CellPropertyDlg::InitData(QMap<QString, QString> &map)
{
	m_map = &map;
	if (map.contains("columnWidth"))
	{
		columnWidthEdit->setText(map["columnWidth"]);
	}
	initCheckBox(map, topCheckBox, "topCheck");
	initCheckBox(map, leftCheckBox, "leftCheck");
	initCheckBox(map, rightCheckBox, "rightCheck");
	initCheckBox(map, bottomCheckBox, "bottomCheck");
}
void CellPropertyDlg::resetMap()
{
	if (m_map == NULL)
	{
		m_map = new QMap<QString, QString>();
	}
	va_listRemove(m_map, 6,"dlgType","columnWidth","topCheck","leftCheck","rightCheck","bottomCheck");
	m_map->insert("dlgType", QString::number(cellPropertyDlg));
	m_map->insert("columnWidth", columnWidthEdit->text());
	getCheckBox(m_map, topCheckBox, "topCheck");
	getCheckBox(m_map, leftCheckBox, "leftCheck");
	getCheckBox(m_map, rightCheckBox, "rightCheck");
	getCheckBox(m_map, bottomCheckBox, "bottomCheck");
}
void CellPropertyDlg::va_listRemove(QMap<QString, QString> *qMap, int argc,...)
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
void CellPropertyDlg::getCheckBox(QMap<QString, QString>*map, QCheckBox *checkBox, QString str)
{
	if (checkBox->isChecked())
	{
		map->insert(str, "true");
	}
	else
	{
		map->insert(str, "false");
	}
}
void CellPropertyDlg::initCheckBox(QMap<QString, QString>&map, QCheckBox *checkBox, QString str)
{
	if (map.contains(str))
	{
		if (map[str] == "true")
		{
			checkBox->setChecked(true);
		}
		else
		{
			checkBox->setChecked(false);
		}
	}

}
void CellPropertyDlg::connectSlots()
{
	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(confirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(cancelBtnClicked()));
}
void CellPropertyDlg::confirmBtnClicked()
{
	resetMap();
	QDialog::accept();
//	this->close();
}
void CellPropertyDlg::cancelBtnClicked()
{
	this->close();
}
//#define APPEND_STYLE_SHEET(buttonX,color)\
//    if (ui->buttonX##CheckBox->isChecked()) \
//    {\
//        this->demoMainStyle += "QWidget {border-##buttonX##-color: ##color##;border-##buttonX##-style:solid;}";\
//    }

void CellPropertyDlg::checkBtnChecked(bool state)
{
	QString temDemoMainStyle = demoMainStyle;
	if (leftCheckBox->isChecked())
	{
		temDemoMainStyle += "QWidget{border-left-color: green;border-left-style:solid;}";
	}

	if (rightCheckBox->isChecked())
	{
		temDemoMainStyle += "QWidget{border-right-color: green;border-right-style:solid;}";
	}

	if (topCheckBox->isChecked())
	{
		temDemoMainStyle += "QWidget{border-top-color: green;border-top-style:solid;}";
	}

	if (bottomCheckBox->isChecked())
	{
		temDemoMainStyle += "QWidget{border-bottom-color: green;border-bottom-style:solid;}";
	}

	demonstrationWidget->setStyleSheet(temDemoMainStyle);
}
