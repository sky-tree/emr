#include "pagesettingdlg.h"
#include "globalvariable.h"

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "MyAppEntrance.h"
#include "UIInputManager.h"
#include "BaseMath.h"
#include "UIConstants.h"
 
PageSettingDlg::PageSettingDlg(QWidget *parent) :
QDialog(parent), upLabel(NULL), downLabel(NULL), leftLabel(NULL),rightLabel(NULL), headerLabel(NULL), footerLabel(NULL),  rightEdit(NULL),downEdit(NULL), footerEdit(NULL),upEdit(NULL), headerEdit(NULL), leftEdit(NULL), cancelBtn(NULL), confirmBtn(NULL)
{
	createUI();
	initUI();
	connectSlots();
	InitData();
	this->setModal(true);
	this->setStyleSheet(kPushBtnStyle);
}

PageSettingDlg::~PageSettingDlg()
{

}
void PageSettingDlg::connectSlots()
{
	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));
	connect(pageUnitComBox, SIGNAL(activated(QString)), this, SLOT(onComboxActivated(QString)));
	connect(horizontalRadioBtn, SIGNAL(clicked(bool)), this, SLOT(onRadioBtnClicked()));
	connect(verticalRadioBtn, SIGNAL(clicked(bool)), this, SLOT(onRadioBtnClicked()));
}
void PageSettingDlg::onConfirmBtnClicked()
{
	bool isHorizontal = false;
	short paperType = 0;
	int pageInsideTop = upEdit->text().toInt();
	int pageInsideBottom = downEdit->text().toInt();
	int pageInsideLeft = leftEdit->text().toInt();
	int pageInsideRight = rightEdit->text().toInt();
	int titleMaxH = headerEdit->text().toInt();
	int bottomMaxH = footerEdit->text().toInt();

	if (horizontalRadioBtn->isChecked())
	{
		isHorizontal = true;
	}
	else if (horizontalRadioBtn->isCheckable())
	{
		isHorizontal = false;
	}
	int index_ = pageUnitComBox->currentIndex();
	switch (index_)
	{
	case 0:
		paperType = UIConstants::PaperTypeAndWH::A4;
		break;
	case 1:
		paperType = UIConstants::PaperTypeAndWH::B5;
		break;
	default:
		break;
	}

	MyAppEntrance* appEntrance = static_cast<MyAppEntrance*>(this->parent());
	if ( appEntrance )
	{
		UIInputManager * mgr = appEntrance->getUIInputMgr();
		mgr->setPageParams(true, isHorizontal, paperType, pageInsideTop, pageInsideBottom, pageInsideLeft, pageInsideRight, titleMaxH, bottomMaxH);
	}
	this->close();
	emit QDialog::accept();
}
void PageSettingDlg::onCancelBtnClicked()
{
	this->close();
}

void PageSettingDlg::onComboxActivated(QString name)
{
	int w, h;
	if (name == "A4")
	{
		BaseMath::refreshPageWHByCentimeter(UIConstants::PaperTypeAndWH::A4, w, h);
	}
	else if (name == "16开")
	{
		BaseMath::refreshPageWHByCentimeter(UIConstants::PaperTypeAndWH::B5, w, h);
	}
	if (horizontalRadioBtn->isChecked())
	{
		QString text_ = QString::number(h) + "mm";
		widthEdit->setText(text_);
		text_ = QString::number(w) + "mm";
		heightEdit->setText(text_);
	}
	else
	{
		QString text_ = QString::number(w) + "mm";
		widthEdit->setText(text_);
		text_ = QString::number(h) + "mm";
		heightEdit->setText(text_);
	}
	
}

void PageSettingDlg::onRadioBtnClicked()
{
	int w, h;
	QString name = pageUnitComBox->currentText();
	onComboxActivated(name);
}

void PageSettingDlg::createUI()
{
	this->setFixedSize(400,300);

	pageTypeLabel = new QLabel(this);
	pageUnitComBox = new QComboBox(this);

	heightLabel = new QLabel(this);
	heightEdit = new QLineEdit(this);
	heightEdit->setFocusPolicy(Qt::NoFocus);

	widthLabel = new QLabel(this);
	widthEdit = new QLineEdit(this);
	widthEdit->setFocusPolicy(Qt::NoFocus);

	directionLabel = new QLabel(this);

	upLabel = new QLabel(this);
	upEdit = new QLineEdit(this);

	downLabel = new QLabel(this);
	downEdit = new QLineEdit(this);

	leftLabel = new QLabel(this);
	leftEdit = new QLineEdit(this);

	rightEdit = new QLineEdit(this);
	rightLabel = new QLabel(this);

	headerLabel = new QLabel(this);
	headerEdit = new QLineEdit(this);

	footerEdit = new QLineEdit(this);
	footerLabel = new QLabel(this);

	horizontalRadioBtn = new QRadioButton(tr("横向"), this);
	verticalRadioBtn = new QRadioButton(tr("纵向"), this);

	confirmBtn = new QPushButton(this);
	confirmBtn->setFixedSize(81, 23);
	cancelBtn = new QPushButton(this);
	cancelBtn->setFixedSize(81, 23);

	QGridLayout * gridLayout = new QGridLayout();

	gridLayout->addWidget(pageTypeLabel, 0, 0);
	gridLayout->addWidget(pageUnitComBox, 0, 1);
	gridLayout->addWidget(upLabel, 0, 2);
	gridLayout->addWidget(upEdit, 0, 3);

	gridLayout->addWidget(heightLabel, 1, 0);
	gridLayout->addWidget(heightEdit, 1, 1);
	gridLayout->addWidget(downLabel, 1, 2);
	gridLayout->addWidget(downEdit, 1, 3);

	gridLayout->addWidget(widthLabel, 2, 0);
	gridLayout->addWidget(widthEdit, 2, 1);
	gridLayout->addWidget(leftLabel, 2, 2);
	gridLayout->addWidget(leftEdit, 2, 3);

	gridLayout->addWidget(directionLabel, 3, 0);
	gridLayout->addWidget(rightLabel, 3, 2);
	gridLayout->addWidget(rightEdit, 3, 3);

	gridLayout->addWidget(verticalRadioBtn, 4, 0);
	gridLayout->addWidget(headerLabel, 4, 2);
	gridLayout->addWidget(headerEdit, 4, 3);

	gridLayout->addWidget(horizontalRadioBtn, 5, 0);
	gridLayout->addWidget(footerLabel, 5, 2);
	gridLayout->addWidget(footerEdit, 5, 3);

	gridLayout->addWidget(confirmBtn, 7, 1);
	gridLayout->addWidget(cancelBtn, 7, 3);

	for (int i = 0; i < 8; i++)
	{
		gridLayout->setRowStretch(i, 1);
	}
	for (int i = 0; i < 5; i++)
	{
		gridLayout->setColumnStretch(1, 1);
	}

	this->setLayout(gridLayout);
	

	upEdit->setValidator(new QIntValidator(0, 100, this));
	leftEdit->setValidator(new QIntValidator(0, 100, this));
	downEdit->setValidator(new QIntValidator(0, 100, this));
	rightEdit->setValidator(new QIntValidator(0, 100, this));
	QRegExp rx("^(2\\d{2}|300)$");
	QRegExpValidator* validator = new QRegExpValidator(rx, this);
	footerEdit->setValidator(validator);
	headerEdit->setValidator(validator);
}

void PageSettingDlg::initUI()
{
	this->setWindowTitle(tr("页面属性设置"));

	pageTypeLabel->setText(tr("纸张类型"));
	pageUnitComBox->addItem(tr("A4"));
	pageUnitComBox->addItem(tr("16开"));

	heightLabel->setText(tr("高度"));
	widthLabel->setText(tr("宽度"));

	directionLabel->setText(tr("纸张方向"));

	upLabel->setText(tr("上边距"));

	leftLabel->setText(tr("左边距"));

	rightEdit->setPlaceholderText(tr("0"));
	downLabel->setText(tr("下边距"));

	downEdit->setPlaceholderText(tr("0"));

	rightLabel->setText(tr("右边距"));

	headerLabel->setText(tr("页眉"));

	footerEdit->setPlaceholderText(tr("200-300"));
	confirmBtn->setText(tr("确定"));

	upEdit->setPlaceholderText(tr("0"));

	footerLabel->setText(tr("页脚"));
	headerEdit->setPlaceholderText(tr("200-300"));
	cancelBtn->setText(tr("取消"));
	leftEdit->setPlaceholderText(tr("0"));

}
void PageSettingDlg::InitData()
{
	bool isHorizontal = false;
	short paperType = 0;
	int pageInsideTop = 0;
	int pageInsideBottom = 0;
	int pageInsideLeft = 0;
	int pageInsideRight = 0;
	int titleMaxH = 0;
	int bottomMaxH = 0;

	MyAppEntrance* appEntrance = static_cast<MyAppEntrance*>(this->parent());
	if (appEntrance)
	{
		UIInputManager * mgr = appEntrance->getUIInputMgr();
		mgr->getPageParams(isHorizontal, paperType, pageInsideTop, pageInsideBottom, pageInsideLeft, pageInsideRight, titleMaxH, bottomMaxH);
		
	}
	upEdit->setText(QString::number(pageInsideTop));
	downEdit->setText(QString::number(pageInsideBottom));
	leftEdit->setText(QString::number(pageInsideLeft));
	rightEdit->setText(QString::number(pageInsideRight));
	headerEdit->setText(QString::number(titleMaxH));
	footerEdit->setText(QString::number(bottomMaxH));
	if (isHorizontal)
	{
		horizontalRadioBtn->setChecked(true);
	}
	else
	{
		verticalRadioBtn->setChecked(true);
	}
	switch (paperType)
	{
	case UIConstants::PaperTypeAndWH::A4:
		pageUnitComBox->setCurrentIndex(0);
		onComboxActivated("A4");
		break;
	case UIConstants::PaperTypeAndWH::B5:
		pageUnitComBox->setCurrentIndex(1);
		onComboxActivated("16开");
		break;
	default:
		break;
	}
}