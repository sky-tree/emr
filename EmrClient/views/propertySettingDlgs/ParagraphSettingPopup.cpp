#include "ParagraphSettingPopup.h"
#include "globalvariable.h"

#include <QLabel>
#include <QValidator>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
ParagraphSettingPopup::ParagraphSettingPopup(QDialog*parent): QDialog(parent)
{
    comboBox = NULL;
    specialUnitLabel = NULL;
    specialPatternLabel = NULL;
    rightiIndentUnitLabel = NULL;
    leftiIndentUnitLabel = NULL;
    rightIndentLabel = NULL;
    leftIndentLabel = NULL;
    specialPatternEdit = NULL;
    rightIndentEdit = NULL;
    leftIndentEdit = NULL;
    cancelBtn = NULL;
    confirmBtn = NULL;
	m_map = NULL;
	m_paragraphData = NULL;
    createUI();
    initUI();

	connectSlots();
	this->setStyleSheet(kPushBtnStyle);
}

ParagraphSettingPopup::~ParagraphSettingPopup()
{

}
void ParagraphSettingPopup::setParagraphData(ParagraphData *pPara)
{
	m_paragraphData = pPara;
	if (m_paragraphData == NULL)return;
	leftIndentEdit->setText(QString::number(m_paragraphData->getInsideLeft()));
	rightIndentEdit->setText(QString::number(m_paragraphData->getInsideRight()));
	comboBox->setCurrentIndex(m_paragraphData->getIndentType());
	specialPatternEdit->setText(QString::number(m_paragraphData->getIndentValue()));
	disL = m_paragraphData->getInsideLeft();
	disR = m_paragraphData->getInsideRight();
	type = m_paragraphData->getIndentType();
	indentLeft = m_paragraphData->getIndentValue();
}
void ParagraphSettingPopup::connectSlots()
{
	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));
	connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onChanged(int)));
}

void ParagraphSettingPopup::onConfirmBtnClicked()
{
	resetMap();
	if (m_paragraphData == NULL)return;
	//m_paragraphData->setIndentParam(leftIndentEdit->text().toInt(), rightIndentEdit->text().toInt(), comboBox->currentIndex(), specialPatternEdit->text().toInt());
	disL = leftIndentEdit->text().toInt();
	disR = rightIndentEdit->text().toInt();
	type = comboBox->currentIndex();
	if (type==0)
	{
		specialPatternEdit->setText("0");
	}
	indentLeft = specialPatternEdit->text().toInt();
	this->accept();
}

void ParagraphSettingPopup::onCancelBtnClicked()
{
	this->reject();
}

void ParagraphSettingPopup::createUI()
{
        if (this->objectName().isEmpty())
        this->resize(269, 263);
		this->setFixedSize(269, 263);
        leftIndentLabel = new QLabel(this);
        leftIndentLabel->setGeometry(QRect(30, 20, 54, 21));
        leftIndentEdit = new QLineEdit(this);
        leftIndentEdit->setGeometry(QRect(80, 20, 131, 20));
        rightIndentEdit = new QLineEdit(this);
        rightIndentEdit->setGeometry(QRect(80, 70, 131, 20));
        rightIndentLabel = new QLabel(this);
        rightIndentLabel->setGeometry(QRect(30, 70, 54, 21));
        leftiIndentUnitLabel = new QLabel(this);
        leftiIndentUnitLabel->setGeometry(QRect(220, 20, 54, 21));
        rightiIndentUnitLabel = new QLabel(this);
        rightiIndentUnitLabel->setGeometry(QRect(220, 70, 54, 21));
        specialPatternLabel = new QLabel(this);
        specialPatternLabel->setGeometry(QRect(30, 120, 54, 21));
        comboBox = new QComboBox(this);
        comboBox->setGeometry(QRect(30, 150, 91, 22));
        specialPatternEdit = new QLineEdit(this);
        specialPatternEdit->setGeometry(QRect(140, 150, 71, 20));
        specialUnitLabel = new QLabel(this);
        specialUnitLabel->setGeometry(QRect(220, 150, 54, 21));
        confirmBtn = new QPushButton(this);
        confirmBtn->setGeometry(QRect(30, 220, 75, 23));
        cancelBtn = new QPushButton(this);
        cancelBtn->setGeometry(QRect(170, 220, 75, 23));

		leftIndentEdit->setValidator(new QIntValidator(0, 100, this));
		rightIndentEdit->setValidator(new QIntValidator(0, 100, this));
		specialPatternEdit->setValidator(new QIntValidator(0, 100, this));
}
 
void ParagraphSettingPopup::initUI()
{
        this->setWindowTitle(tr("段落属性"));
        leftIndentLabel->setText(tr("左缩进"));
        rightIndentLabel->setText(tr("右缩进"));
        leftiIndentUnitLabel->setText(tr("毫米"));
        rightiIndentUnitLabel->setText(tr("毫米"));
        specialPatternLabel->setText(tr("特殊模式"));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
		 << tr("不缩进")
         << tr("首行缩进")
         << tr("悬挂缩进")
        );
        specialUnitLabel->setText(tr("毫米"));
        confirmBtn->setText(tr("确定"));
        cancelBtn->setText(tr("取消"));
		comboBox->setCurrentIndex(0);
		specialPatternEdit->setEnabled(false);
}
void ParagraphSettingPopup::InitData(QMap<QString, QString> &map)
{
	m_map = &map;
	if (m_map == NULL)return;
	if (map.contains("left"))
	{
		leftIndentEdit->setText(map["left"]);
	}
	if (map.contains("right"))
	{
		rightIndentEdit->setText(map["right"]);
	}
	if (map.contains("special"))
	{
		comboBox->setCurrentText(map["special"]);
	}
	if (map.contains("specialValue"))
	{
		specialPatternEdit->setText(map["specialValue"]);
	}
}
QMap<QString,QString> ParagraphSettingPopup::getData()const
{
	return *m_map;
}
void ParagraphSettingPopup::resetMap()
{
	if (m_map == NULL)
	{
		m_map = new QMap<QString, QString>();
	}
	if (m_map->contains("dlgType") == false)
	{
		m_map->insert("dlgType", QString::number(paragraphSettingDlg));
	}
	QStringList strlist;
	// special 悬挂缩进 首行缩进
	strlist << "left" << "right" << "special"<< "specialValue";
	for (int i = 0; i < strlist.count(); i++)
	{
		if (m_map->contains(strlist.at(i)))
		{
			m_map->remove(strlist.at(i));
		}
	}
	m_map->insert("left", leftIndentEdit->text());
	m_map->insert("right", rightIndentEdit->text());
	m_map->insert("special", comboBox->currentText());
	m_map->insert("specialValue", specialPatternEdit->text()); 
}

void ParagraphSettingPopup::onChanged(int index)
{
	if (index == 0)
	{
		specialPatternEdit->setText("0");
		specialPatternEdit->setEnabled(false);
	}
	else
	{
		specialPatternEdit->setEnabled(true);
		int getIndex = (int)m_paragraphData->getIndentType();
		if (getIndex == index)
		{
			specialPatternEdit->setText(QString::number(m_paragraphData->getInsideLeft()));
		}
		else
		{
			specialPatternEdit->setText("0");
		}

	}
}

