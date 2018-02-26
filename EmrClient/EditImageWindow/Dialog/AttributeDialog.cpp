#include "AttributeDialog.h"
#include <QPushButton>

AttributeDialog::AttributeDialog(QWidget *parent)
	: QDialog(parent)
{
	mColorList = QColor::colorNames();
	ui.setupUi(this);
	fillColorList(ui.comboBox_2);
	fillColorList(ui.comboBox_4);

	ui.checkBox->setChecked(false);
	ui.comboBox_3->setEnabled(false);
	ui.comboBox_4->setEnabled(false);
	ui.checkBox_2->setChecked(false);

	ui.lineEdit->setPlaceholderText(QString::fromLocal8Bit("1-10"));
	QIntValidator *pIntValidator = new QIntValidator(this);
	pIntValidator->setRange(1, 10);
	ui.lineEdit->setValidator(pIntValidator);

	connect(ui.checkBox, SIGNAL(stateChanged(int)), this, SLOT(actionCheckBox(int)));
	connect(ui.comboBox_3, SIGNAL(activated(QString)), this, SLOT(actionComboBox_3(QString)));


	connect(ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok), SIGNAL(clicked()), this, SLOT(actionOK()));
	connect(ui.buttonBox->button(QDialogButtonBox::StandardButton::Cancel), SIGNAL(clicked()), this, SLOT(actionCancel()));
	this->setWindowTitle(QString::fromLocal8Bit("属性设置"));
	ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setText(QString::fromLocal8Bit("确定"));
	ui.buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->setText(QString::fromLocal8Bit("取消"));

}

AttributeDialog::~AttributeDialog()
{

}

void AttributeDialog::actionOK()
{
	QString txt = ui.lineEdit->text();
	txt = ui.comboBox->currentText();
	QColor color = mColorList[ui.comboBox_2->currentIndex()];
	if (ui.checkBox->isChecked() == true)
	{
		txt = ui.comboBox_3->currentText();
		if (txt.compare(QString::fromLocal8Bit("纯颜色填充")) == 0)
		{
			QColor color = mColorList[ui.comboBox_4->currentIndex()];
		}
	}
	bool b = ui.checkBox_2->isChecked();
}

void AttributeDialog::actionCancel()
{
}


void AttributeDialog::actionCheckBox(int state)
{
	if (state == Qt::Checked) // "选中"
	{
		ui.comboBox_3->setEnabled(true);
		if (QString::compare(ui.comboBox_3->currentText(), QString::fromLocal8Bit("纯颜色填充")) == 0)
		{
			ui.comboBox_4->setEnabled(true);
		}
		else
		{
			ui.comboBox_4->setEnabled(false);
		}
	}
	else // 未选中 - Qt::Unchecked
	{
		ui.comboBox_3->setEnabled(false);
		ui.comboBox_4->setEnabled(false);
	}
}

void AttributeDialog::actionComboBox_3(QString state)
{

	if (state.compare(QString::fromLocal8Bit("纯颜色填充")) == 0)
	{
		ui.comboBox_4->setEnabled(true);
	}
	else
	{
		ui.comboBox_4->setEnabled(false);
	}
}

void AttributeDialog::fillColorList(QComboBox *comboBox)
{
	QString color;

	foreach(color, mColorList)
	{
		QPixmap pix(QSize(140, 20));
		pix.fill(QColor(color));
		comboBox->addItem(QIcon(pix), NULL);
		comboBox->setIconSize(QSize(140, 20));
		comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	}
}