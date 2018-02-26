#include "structuringdlg.h"

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>
#include"qvalidator.h"
#include"views/globalvariable.h"
#include<QMessageBox>
#include "MessageBox.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
StructuringDlg::StructuringDlg(QWidget *parent) :
QDialog(parent), codeSysLabel(NULL), nameLabel(NULL), codeLabel(NULL), totalLenLabel(NULL), decimalLenLabel(NULL)
, formatLabel(NULL), currentContentLabel(NULL), boundValueLabel(NULL), nameEdit(NULL), codeSysComboBox(NULL), numValueRadioButton(NULL), dateRadioButton(NULL)
, codeEdit(NULL), totalLenEdit(NULL), decimalLenEdit(NULL), minEdit(NULL), maxEdit(NULL), currentContentEdit(NULL)
, /*scriptNameEdit(NULL), calculatePropertyCheckBox(NULL), */mustClickCheckBox(NULL), confirmBtn(NULL)
, cancelBtn(NULL), timeComboBox(NULL), m_map(NULL)
{
	createUI();
	initUI();
	connectSlots();
	this->setModal(true);
	//numValueRadioButton->setChecked(true);
	//setRadioBtnEnable(true);
	//重新定义stylesheet

}

StructuringDlg::~StructuringDlg()
{

}
void StructuringDlg::connectSlots()
{
	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));
	connect(numValueRadioButton, SIGNAL(toggled(bool)), this, SLOT(onRadioBtnClicked()));
	connect(dateRadioButton, SIGNAL(toggled(bool)), this, SLOT(onRadioBtnClicked()));
}
void StructuringDlg::setRadioBtnEnable(bool isEnable)
{
	if (isEnable == true)
	{
		totalLenEdit->setEnabled(isEnable);
		decimalLenEdit->text().clear();
		decimalLenEdit->setEnabled(isEnable);
		minEdit->setEnabled(isEnable);
		maxEdit->setEnabled(isEnable);

		/*formatEdit->setEnabled(false);*/
		timeComboBox->setEnabled(false);
		/*formatEdit->clear();*/
	}
	else
	{
		totalLenEdit->setEnabled(isEnable);
		decimalLenEdit->setEnabled(isEnable);
		minEdit->setEnabled(isEnable);
		maxEdit->setEnabled(isEnable);
		totalLenEdit->clear();
		decimalLenEdit->clear();
		minEdit->clear();
		maxEdit->clear();

		/*formatEdit->setEnabled(true);*/
		timeComboBox->setEnabled(true);
	}
}
void StructuringDlg::onRadioBtnClicked()
{
	QObject * sender = this->sender();
	if (sender == numValueRadioButton)
	{
		setRadioBtnEnable(true);
	}else if (sender==dateRadioButton)
	{	
		setRadioBtnEnable(false);
	}
}
void StructuringDlg::onConfirmBtnClicked()
{
	if (numValueRadioButton->isChecked())
	{
		if (!checkData()) return;
	}

	resetMap();
	//emit QDialog::accept();
	this->close();
}
void StructuringDlg::onCancelBtnClicked()
{
	this->close();
}
void StructuringDlg::createUI()
{
	this->resize(QSize(350, 350));
	this->setStyleSheet(kPushBtnStyle);

	nameLabel = new QLabel(this);
	nameEdit = new QLineEdit(this);

	codeSysLabel = new QLabel(this);
	codeSysComboBox = new QComboBox(this);

	codeLabel = new QLabel(this);
	codeEdit = new QLineEdit(this);

	QFormLayout* fLayout1 = new QFormLayout();
	fLayout1->addRow(nameLabel, nameEdit);
	fLayout1->addRow(codeSysLabel,codeSysComboBox);
	fLayout1->addRow(codeLabel, codeEdit);
	fLayout1->setSpacing(20);

	numValueRadioButton = new QRadioButton(this);
	dateRadioButton = new QRadioButton(this);
	mustClickCheckBox = new QCheckBox(this);

	QHBoxLayout* hLayout1 = new QHBoxLayout();
	hLayout1->addWidget(numValueRadioButton);
	hLayout1->addWidget(dateRadioButton);
	hLayout1->addStretch();
	hLayout1->addWidget(mustClickCheckBox);

	totalLenLabel = new QLabel(this);
	totalLenEdit = new QLineEdit(this);
	connect(totalLenEdit, SIGNAL(textChanged(QString)), this, SLOT(setNumRange()));
	decimalLenLabel = new QLabel(this);
	decimalLenEdit = new QLineEdit(this);
	connect(decimalLenEdit, SIGNAL(textChanged(QString)), this, SLOT(setNumRange()));

	boundValueLabel = new QLabel(this);
	minEdit = new QLineEdit(this);
	minEdit->setPlaceholderText("请输入最小值");
	maxEdit = new QLineEdit(this);
	maxEdit->setPlaceholderText("请输入最大值");

	QGridLayout* gLayout = new QGridLayout();
	gLayout->addWidget(totalLenLabel, 0, 0, 1, 1);
	gLayout->addWidget(totalLenEdit, 0, 1, 1, 1);
	gLayout->addWidget(decimalLenLabel, 0, 2, 1, 1);
	gLayout->addWidget(decimalLenEdit, 0, 3, 1, 1);
	gLayout->addWidget(boundValueLabel, 1, 0, 1, 1);
	gLayout->addWidget(minEdit, 1, 1, 1, 1);
	gLayout->addWidget(maxEdit, 1, 3, 1, 1);


	formatLabel = new QLabel(this);
	timeComboBox = new QComboBox(this);
	timeComboBox->setEditable(true);
	timeComboBox->lineEdit()->setPlaceholderText("请输入或选择时间格式");

	currentContentLabel = new QLabel(this);
	currentContentEdit = new QLineEdit(this);

	QFormLayout* fLayout2 = new QFormLayout();
	fLayout2->setSpacing(20);
	fLayout2->addRow(formatLabel, timeComboBox);
	fLayout2->addRow(currentContentLabel, currentContentEdit);

	confirmBtn = new QPushButton(this);
	confirmBtn->setFixedSize(81, 23);
	cancelBtn = new QPushButton(this);
	 cancelBtn->setFixedSize(81, 23);

	QHBoxLayout* hLayout4 = new QHBoxLayout();
	hLayout4->addStretch();
	hLayout4->addWidget(confirmBtn);
	hLayout4->addWidget(cancelBtn);

	QVBoxLayout* vLayout = new QVBoxLayout(this);
	vLayout->addLayout(fLayout1);
	vLayout->addLayout(hLayout1);
	vLayout->addLayout(gLayout);
	vLayout->addLayout(fLayout2);
	vLayout->addLayout(hLayout4);

	this->setLayout(vLayout);
// 	formatLabel_2 = new QLabel(this);
// 	formatLabel_2->setGeometry(QRect(40, 240, 261, 16));
	QValidator *totalLenEditValidator = new QIntValidator(0, 20, this);
	totalLenEdit->setValidator(totalLenEditValidator);

	QValidator *decimalLenEditValidator = new QIntValidator(0, 20, this);
	decimalLenEdit->setValidator(decimalLenEditValidator);
}

void StructuringDlg::initUI()
{
	this->setWindowTitle(tr("格式化元素属性设置"));
	codeSysLabel->setText(tr("编码系统"));
	codeSysComboBox->clear();
	codeSysComboBox->insertItems(0, QStringList()
	<< tr("UTF-8")
	<< tr("Unicode")
	<< tr("Unicode big endian")
	<< tr("ANSI")
	);
	nameLabel->setText(tr("名称"));
	codeLabel->setText(tr("编码"));
	numValueRadioButton->setText(tr("数值类型"));
	dateRadioButton->setText(tr("日期类型"));
	totalLenLabel->setText(tr("总长度"));
	decimalLenLabel->setText(tr("小数位数"));
	formatLabel->setText(tr("日期格式"));
	boundValueLabel->setText(tr("有效范围值"));
	/*dateFormatLabel->setText(tr("常用日期格式"));*/
	currentContentLabel->setText(tr("目前元素内容"));
//	scriptNameLabel->setText(tr("脚本元素名称"));
//	calculatePropertyCheckBox->setText(tr("计算属性"));*/张磊注释2017.8.1
	mustClickCheckBox->setText(tr("必点属性"));
	confirmBtn->setText(tr("确定"));
	cancelBtn->setText(tr("取消"));
/*	formatLabel_2->setText(tr("提示格式  yyyy年MM月dd日 hh小时mm分钟ss秒"));*/
	timeComboBox->clear();
	timeComboBox->insertItems(0, QStringList()
		<< tr("yyyy年MM月dd日 hh时mm分")
	<< tr("yyyy年MM月dd日 hh时mm分ss秒")
	<< tr("yyyy年MM月dd日")
	<< tr("yyyy-MM-dd hh:mm")
	<< tr("yyyy-MM-dd hh:mm:ss")
	<< tr("yyyy-MM-dd")
	);
}
void StructuringDlg::InitData(QMap<QString, QString> &map)
{
	m_map = &map;
	if (m_map == NULL)return;
	if (map.contains("codeSys")){ codeSysComboBox->setCurrentIndex(map["codeSys"].toInt()); }
	if (map.contains("code")){ codeEdit->setText(map["code"]); }
	if (map.contains("length")){ totalLenEdit->setText(map["length"]); }
	if (map.contains("decimalLen")){ decimalLenEdit->setText(map["decimalLen"]); }
	if (map.contains("decimalLen")){ decimalLenEdit->setText(map["decimalLen"]); }

	initTextEdit(map, codeEdit, "code");
	initTextEdit(map, totalLenEdit, "length");
	initTextEdit(map, decimalLenEdit, "decimalLen");
/*	initTextEdit(map, formatEdit, "format");*/
	initTextEdit(map, minEdit, "minValue");
	initTextEdit(map, maxEdit, "maxValue");
	initTextEdit(map, currentContentEdit, "content");
	initTextEdit(map, nameEdit, "name");
//	initTextEdit(map, scriptNameEdit, "scriptName");张磊注释2017.8.1
	timeComboBox->setCurrentText(map["dataTime"]);

	if (map.contains("valueType"))
	{
		if (map["valueType"] == "true")
		{ 
			numValueRadioButton->setChecked(true); 
		}
		else
		{
			dateRadioButton->setChecked(true);
		}
	}
// 	if (map.contains("calculateProperty"))
// 	{
// 		if (map["calculateProperty"] == "true"){ calculatePropertyCheckBox->setChecked(true); }
// 		else
// 		{
// 			calculatePropertyCheckBox->setChecked(false);
// 		}
// 	}张磊注释2017.8.1
	if (map.contains("mustClick"))
	{
		if (map["mustClick"] == "true"){ mustClickCheckBox->setChecked(true); }
		else
		{
			mustClickCheckBox->setChecked(false);
		}
	}
}
void StructuringDlg::initTextEdit(QMap<QString, QString> &qMap, QLineEdit *edit, QString str)
{
	if (qMap.contains(str))
	{
		edit->setText(qMap[str]);
	}
}
void StructuringDlg::va_listRemove(QMap<QString, QString> *qMap, int argc, ...)
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
void StructuringDlg::dealNumber()
{
	//int intLen = 0; 
	//int decimalLen = 0;
	//QString strLen = totalLenEdit->text();
	//QString decimalLen = decimalLenEdit->text();
	//if (strLen.length() <= 0)
	//{
	//	//QMessageBox::Abort(QMessageBox::NoIcon, "提示", "长度不能为空!");
	//}
	//else
	//{
	//	intLen = strLen.toInt();
	//}

	//if (decimalLen.length() >= 0)
	//{
	//	decimalLen = decimalLenEdit->text().toInt();
	//}
	//int numberLen = intLen - decimalLen.toInt() - 1;
 //   
	//QString minvalue = minEdit->text();
	//QString maxValue = maxEdit->text();
	//if (minvalue.length() == 0)
	//{

	//}
	//if (maxValue.length() == 0)
	//{

	//}



}
void StructuringDlg::resetMap()
{
	//dealNumber();
	if (m_map == NULL)
	{
		m_map =new  QMap<QString, QString>();
	}
	//m_map->clear();
	va_listRemove(m_map,15,"dlgType", "name", "codeSys", "code", "valueType",
		"length", "decimalLen","format","dataTime", "minValue", "maxValue", "content", "scriptName", "calculateProperty", "mustClick");
	m_map->insert("dlgType", QString::number(structuringDlg));
	m_map->insert("name", nameEdit->text());
	m_map->insert("codeSys", QString::number(codeSysComboBox->currentIndex()));
	m_map->insert("code", codeEdit->text());
	if (numValueRadioButton->isChecked())
	{
		m_map->insert("valueType", "true");
	}
	else
	{
		m_map->insert("valueType", "false");
	}
	m_map->insert("length", totalLenEdit->text());

	m_map->insert("decimalLen", decimalLenEdit->text());

/*	m_map->insert("format", formatEdit->text());*/
	m_map->insert("dataTime", timeComboBox->currentText());
	m_map->insert("minValue", QString::number(minEdit->text().toDouble()));
	m_map->insert("maxValue", maxEdit->text());

	if (maxEdit->text().toDouble() == 0)
	{
		double max = maxData();
		maxEdit->setText(QString::number(max));

		m_map->insert("maxValue", maxEdit->text());
		(*m_map)["maxValue"] = QString::number(max);
	}

	m_map->insert("content", currentContentEdit->text());
//	m_map->insert("scriptName", scriptNameEdit->text());张磊2017.8.1
// 	if (calculatePropertyCheckBox->isChecked())
// 	{
// 		m_map->insert("calculateProperty", "true");
// 	}张磊2017.8.1
// 	else
// 	{
// 		m_map->insert("calculateProperty", "false");
// 	}
	
	if (mustClickCheckBox->isChecked())
	{
		m_map->insert("mustClick", "true");
	}
	else
	{
		m_map->insert("mustClick", "false");
	}
}

bool StructuringDlg::checkData()
{
	if (totalLenEdit->text().toInt() <= 0)
	{
		MessageBox::showErrorMessageBox("提示", "数值总长度不能为0");
		return false;
	}
	else if (decimalLenEdit->text() != "" && totalLenEdit->text().toInt() < decimalLenEdit->text().toInt() + 2)
	{
		MessageBox::showErrorMessageBox("提示", "数值总长度至少比小数位数多两位");
		return false;
	}
	else if (maxEdit->text().toDouble() < minEdit->text().toDouble())
	{
		MessageBox::showErrorMessageBox("提示", "有效范围值后者应不小于前者");
		return false;
	}
	else if (maxEdit->text().length() > totalLenEdit->text().toInt() || minEdit->text().length() > totalLenEdit->text().toInt())
	{
		MessageBox::showErrorMessageBox("提示", "最大值或最小值长度应不大于数值总长度");
		return false;
	}
	else if (maxEdit->text().toDouble() != 0 )
	{
		double max = maxData();

		if (maxEdit->text().toDouble() > max)
		{
			char result[32];
			memset(result, 0, sizeof(result));
			sprintf(result, "%f", max);
			QString tip = "有效值范围的最大值应不大于数值总长度对应的最大值: " + QString(result);
			MessageBox::showErrorMessageBox("提示", tip);
			return false;
		}
	}
	
	return true;
}

double StructuringDlg::maxData()
{
	int len = 0;

	if (decimalLenEdit->text().toInt() > 0)
	{
		len = totalLenEdit->text().toInt() - decimalLenEdit->text().toInt() - 1;
	}
	else
	{
		len = totalLenEdit->text().toInt();
	}

	double max = 1;
	for (int i = 0; i < len; i++)
	{
		max *= 10;
	}

	int decimalLen = decimalLenEdit->text().toInt();
	if (decimalLen == 0)
	{
		max = max - 1;
	}
	else
	{
		double minuend = 1;
		for (int i = 0; i < decimalLen; i++)
		{
			minuend *= 0.1;
		}

		max = max - minuend;
	}

	return max;
}
void StructuringDlg::setNumRange()
{
	maxEdit->clear();

	int numLen = totalLenEdit->text().toInt();
	int numDecimal = decimalLenEdit->text().toInt();

	if ( (numLen-2) < numDecimal)
	{
		decimalLenEdit->clear();
		return;
	}
	numDecimal = decimalLenEdit->text().toInt();

	QString max = "";
	if ( numLen != 0 )
	{
		if ( numDecimal == 0 )
		{
			for (int i = 0; i < (numLen - numDecimal); i++)
			{
				max = max.append('9');
			}
		}
		else
		{
			for (int i = 0; i < (numLen - numDecimal-1); i++)
			{
				max = max.append('9');
			}
		}
		
	}
	if ( numDecimal != 0 )
	{
		max.append('.');
		for (int i = 0; i < numDecimal; i++)
		{
			max = max.append('9');
		}
	}
	minEdit->setText("0");
	maxEdit->setText(max);
}