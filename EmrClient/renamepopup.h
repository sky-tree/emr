#ifndef RENAMEPOPUP_H
#define RENAMEPOPUP_H

#include <QDialog>
#include "ui_renamepopup.h"
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif

class RenamePopup : public QDialog
{
	Q_OBJECT

public:
	RenamePopup(QString pOldName,QWidget *parent = 0);
	~RenamePopup();
	static QString getData(QString oldName);

private:
	Ui::RenamePopup ui;
	QString oldName;
	QString getData()const
	{
		return oldName;
	}
	void createConnect();
	private slots:
	void ClickResult();
};

#endif // RENAMEPOPUP_H
