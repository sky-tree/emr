#include "tablepropertydlg.h"
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>
#include"qvalidator.h"
#include"views/globalvariable.h"
#include"QCheckBox"
TablePropertyDlg::TablePropertyDlg(QWidget *parent) :QDialog(parent), m_map(NULL)
, mCanDragLine(NULL)
{
	createUI();
	initUI();
	connectSlots();
	this->setModal(true);
}

TablePropertyDlg::~TablePropertyDlg()
{
    //delete ui;
	if (mCanDragLine)
	{
		delete mCanDragLine;
		mCanDragLine = NULL;
	}
}
void TablePropertyDlg::connectSlots()
{
	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));
	connect(noFrameRadioBtn, SIGNAL(toggled(bool)), this, SLOT(borderContainerBtnsClicked()));
	connect(haveFrameRadioBtn, SIGNAL(toggled(bool)), this, SLOT(borderContainerBtnsClicked()));
}
void TablePropertyDlg::borderContainerBtnsClicked()
{
	QObject * sender = this->sender();
	if (sender == noFrameRadioBtn)
	{
		borderWidthLabel->setEnabled(false);
		borderWidthEdit->setEnabled(false);
	}
	else if (sender == haveFrameRadioBtn)
	{
		borderWidthLabel->setEnabled(true);
		borderWidthEdit->setEnabled(true);
	}

}
void TablePropertyDlg::onConfirmBtnClicked()
{
	resetMap();
	 QDialog::accept();
	//this->close();
}
void TablePropertyDlg::onCancelBtnClicked()
{
	this->close();
}
void TablePropertyDlg::createUI()
{
	this->resize(433, 284);
	this->setMinimumSize(433, 284);
	this->setMaximumSize(433, 284);
	frameRadioBtnContainer = new QWidget(this);
	frameRadioBtnContainer->setObjectName(QStringLiteral("frameRadioBtnContainer"));
	frameRadioBtnContainer->setGeometry(QRect(0, 10, 181, 61));
	noFrameRadioBtn = new QRadioButton(frameRadioBtnContainer);
	noFrameRadioBtn->setObjectName(QStringLiteral("noFrameRadioBtn"));
	noFrameRadioBtn->setGeometry(QRect(20, 20, 89, 16));
	haveFrameRadioBtn = new QRadioButton(frameRadioBtnContainer);
	haveFrameRadioBtn->setObjectName(QStringLiteral("haveFrameRadioBtn"));
	haveFrameRadioBtn->setGeometry(QRect(110, 20, 89, 16));
	borderWidthLabel = new QLabel(this);
	borderWidthLabel->setObjectName(QStringLiteral("borderWidthLabel"));
	borderWidthLabel->setGeometry(QRect(190, 30, 130, 16));
	borderWidthLabel->setLayoutDirection(Qt::LeftToRight);
	borderWidthEdit = new QLineEdit(this);
	borderWidthEdit->setObjectName(QStringLiteral("borderWidthEdit"));
	borderWidthEdit->setGeometry(QRect(315, 30, 80, 20));
	borderWidthEdit->setInputMethodHints(Qt::ImhNone);
	borderWidthEdit->setEchoMode(QLineEdit::Normal);
	QValidator *borderValidator = new QIntValidator(0, 20, this);
	borderWidthEdit->setValidator(borderValidator);
	mmUnitLabel = new QLabel(this);
	mmUnitLabel->setObjectName(QStringLiteral("mmUnitLabel"));
	mmUnitLabel->setGeometry(QRect(380, 30, 31, 20));

	mCanDragLine = new QCheckBox(this);
	mCanDragLine->setGeometry(QRect(20, 80, 141, 16));
	mCanDragLine->setText(tr("是否能拖动表格线"));

	alignmentRadioBtnContainer = new QWidget(this);
	alignmentRadioBtnContainer->setObjectName(QStringLiteral("alignmentRadioBtnContainer"));
	alignmentRadioBtnContainer->setGeometry(QRect(0, 120, 261, 61));
	leftRadioBtn = new QRadioButton(alignmentRadioBtnContainer);
	leftRadioBtn->setObjectName(QStringLiteral("leftRadioBtn"));
	leftRadioBtn->setGeometry(QRect(20, 20, 89, 16));
	centerRadioBtn = new QRadioButton(alignmentRadioBtnContainer);
	centerRadioBtn->setObjectName(QStringLiteral("centerRadioBtn"));
	centerRadioBtn->setGeometry(QRect(110, 20, 89, 16));
	centerRadioBtn->setCheckable(true);
	centerRadioBtn->setChecked(false);
	rightRadioBtn = new QRadioButton(alignmentRadioBtnContainer);
	rightRadioBtn->setObjectName(QStringLiteral("rightRadioBtn"));
	rightRadioBtn->setGeometry(QRect(180, 20, 89, 16));
	rightRadioBtn->setLayoutDirection(Qt::LeftToRight);
	line = new QFrame(this);
	line->setObjectName(QStringLiteral("line"));
	line->setGeometry(QRect(20, 110, 381, 20));
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);
	desLabel = new QLabel(this);
	desLabel->setObjectName(QStringLiteral("desLabel"));
	desLabel->setGeometry(QRect(210, 190, 175, 20));
	cellMarginEdit = new QLineEdit(this);
	cellMarginEdit->setObjectName(QStringLiteral("cellMarginEdit"));
	cellMarginEdit->setGeometry(QRect(90, 190, 113, 20));
	QValidator *cellValidator = new QIntValidator(0, 20, this);
	cellMarginEdit->setValidator(cellValidator);

	cellMarginLabel = new QLabel(this);
	cellMarginLabel->setObjectName(QStringLiteral("cellMarginLabel"));
	cellMarginLabel->setGeometry(QRect(20, 190, 71, 16));
	cellMarginLabel->setLayoutDirection(Qt::LeftToRight);
	confirmBtn = new QPushButton(this);
	confirmBtn->setObjectName(QStringLiteral("confirmBtn"));
	confirmBtn->setGeometry(QRect(230, 240, 75, 23));
	cancelBtn = new QPushButton(this);
	cancelBtn->setObjectName(QStringLiteral("cancelBtn"));
	cancelBtn->setGeometry(QRect(330, 240, 75, 23));

}
void TablePropertyDlg::initUI()
{
	this->setWindowTitle("表格属性设置");
	noFrameRadioBtn->setText("无边框");
	haveFrameRadioBtn->setText("有边框");
	borderWidthLabel->setText("边框线宽度(最大为20)");
	leftRadioBtn->setText("左对齐");
	centerRadioBtn->setText("居中");
	rightRadioBtn->setText("右对齐");
	desLabel->setText("(单元格内部文字边距,最大为20)");
	cellMarginLabel->setText("单元格边距");
	confirmBtn->setText("确定");
	cancelBtn->setText("取消");
}
void TablePropertyDlg::InitData(QMap<QString, QString> &map)
{
	m_map = &map;
	if (map.contains("frame"))
	{
		if (map["frame"]=="false")
		{
			noFrameRadioBtn->setChecked(true);
		}
		else
		{
			haveFrameRadioBtn->setChecked(true);
			if (map.contains("frameValue"))
			{
				borderWidthEdit->setText(map["frameValue"]);
			}
		}
	}
	if (map.contains("canDrag"))
	{
		if (map["canDrag"] == "true")
		{
			mCanDragLine->setChecked(true);
		}
		else
		{
			mCanDragLine->setChecked(false);
		}
	}
	if (map.contains("Align"))
	{
		int type = map["Align"].toInt();
		switch (type)
		{
		case 1:leftRadioBtn->setChecked(true); break;
		case 2:centerRadioBtn->setChecked(true); break;
		case 3:rightRadioBtn->setChecked(true); break;
		default:
			break;
		}
	}
	if (map.contains("borderWidth")){  cellMarginEdit->setText(map["borderWidth"]); }
}
void TablePropertyDlg::resetMap()
{
	if (m_map == NULL)
	{
		m_map = new QMap<QString, QString>();
	}
	//m_map->clear();
	va_listRemove(m_map, 5, "dlgType", "frame", "frameValue", "Align", "borderWidth");
	m_map->insert("dlgType", QString::number(tablePropertyDlg));
	if (noFrameRadioBtn->isChecked())
	{
		m_map->insert("frame", "false");
		m_map->insert("frameValue", "");
	}
	else
	{
		m_map->insert("frame", "true");
		m_map->insert("frameValue", borderWidthEdit->text());
	}

	if (mCanDragLine->isChecked())
	{
		m_map->insert("canDrag", "true");
	}
	else
	{
		m_map->insert("canDrag", "false");
	}
	if (rightRadioBtn->isChecked())
	{
		m_map->insert("Align", "3");
	}
	else if (centerRadioBtn->isChecked())
	{
		m_map->insert("Align", "2");
	}
	else 
	{
		m_map->insert("Align", "1");
	}

	m_map->insert("borderWidth", cellMarginEdit->text());

}
void TablePropertyDlg::va_listRemove(QMap<QString, QString> *qMap, int argc, ...)
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