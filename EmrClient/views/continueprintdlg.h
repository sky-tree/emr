#ifndef CONTINUEPRINTDLG_H
#define CONTINUEPRINTDLG_H

#include <QDialog>
#include "ui_continueprintdlg.h"
#include "MSC_VER.h"
 
class ContinuePrintDlg : public QDialog
{
	Q_OBJECT

public:
	ContinuePrintDlg(QWidget *parent = 0);
	~ContinuePrintDlg();

	static bool getData(int &startValue, int & endValue);
	void initInputValidator(int maxValue);

	int getStartValue();
	int getEndValue();
private:
	Ui::ContinuePrintDlg ui;

	public slots:
	void onConfirmBtnClicked();
	void onCancelBtnClicked();
};

#endif // CONTINUEPRINTDLG_H
