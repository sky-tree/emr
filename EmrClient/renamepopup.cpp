#include "renamepopup.h"
#include<MessageBox.h>
RenamePopup::RenamePopup(QString pOldName,QWidget *parent)
: QDialog(parent), oldName(pOldName)
{
	ui.setupUi(this);
	createConnect();
	ui.oldNameLineEdit->setText(pOldName);
}

RenamePopup::~RenamePopup()
{

}
QString RenamePopup::getData(QString pOldName)
{
	pOldName = pOldName.replace("*", "");
	RenamePopup dlg(pOldName);
	if (dlg.exec() == Accepted)
	{
		return dlg.getData();
	}
	return pOldName;
}

void RenamePopup::createConnect()
{
	connect(ui.btn_Cancel, SIGNAL(clicked()), this, SLOT(ClickResult()));
	connect(ui.btn_OK, SIGNAL(clicked()), this, SLOT(ClickResult()));
}

void RenamePopup::ClickResult()
{
	QObject *sender = this->sender();
	QString  text = ui.newNameLineEdit->text().trimmed();
	QString preName = ui.oldNameLineEdit->text();
	if (sender == ui.btn_OK)
	{
		if (text == preName)
		{
			MessageBox::showAboutMessageBox(QStringLiteral("提示"), QStringLiteral("新名称与原名称不能相同"));
			return;
		}
		if (text.length() == 0)
		{
			MessageBox::showAboutMessageBox(QStringLiteral("警告"), QStringLiteral("模板名称不能为空"));
			return;
		}
		if (text.contains("\\") || text.contains("/") || text.contains("*") || text.contains("?") || text.contains("<") || text.contains(">") || text.contains("\""))
		{
			MessageBox::showAboutMessageBox(QStringLiteral("警告"), QStringLiteral("模板名称不能包含\\ / * ? < > \""));
			return;
		}
		if (text.length() > 20)
		{
			MessageBox::showAboutMessageBox(QStringLiteral("警告"), QStringLiteral("模板名称不能超过20个字符!"));
			return;
		}
		oldName = text;
		emit QDialog::accept();
		
	}
	else
	{
		this->close();
	}

}
