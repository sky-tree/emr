#include "continueprintdlg.h"
#include "globalvariable.h"
#include "MessageBox.h"


ContinuePrintDlg::ContinuePrintDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(ui.cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));

	this->setWindowTitle(tr("续打设置"));
	this->setStyleSheet(kPushBtnStyle);
}

ContinuePrintDlg::~ContinuePrintDlg()
{

}

void ContinuePrintDlg::onConfirmBtnClicked()
{
	if (ui.startEdit->text().toInt() > ui.endEdit->text().toInt())
	{
		MessageBox::showAboutMessageBox(QStringLiteral("提示"), QStringLiteral("结束段落值应不小于起始段落值"));
		return;
	}
	 
	QDialog::accept();
	//this->close();
}

void ContinuePrintDlg::onCancelBtnClicked()
{
	this->close();
}

bool ContinuePrintDlg::getData(int &startValue, int & endValue)
{
	ContinuePrintDlg dlg;

	if (dlg.exec() == QDialog::Accepted)
	{
		startValue = dlg.getStartValue();
		endValue = dlg.getEndValue();
		return true;
	}

	return false;
}

void ContinuePrintDlg::initInputValidator(int maxValue)
{
	ui.startEdit->setValidator(new QIntValidator(0, maxValue, this));
	ui.endEdit->setValidator(new QIntValidator(0, maxValue, this));
}

int ContinuePrintDlg::getStartValue()
{
	return ui.startEdit->text().toInt();
}

int ContinuePrintDlg::getEndValue()
{
	return ui.endEdit->text().toInt();
}
