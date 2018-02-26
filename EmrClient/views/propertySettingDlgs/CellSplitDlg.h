#ifndef CELLSPLITDLG_H
#define CELLSPLITDLG_H

#include <QDialog>

#include "customDelegate/MSC_VER.h"

class  QDialogButtonBox;
class  QLabel;
class  QLineEdit;

class CellSplitDlg : public QDialog
{
    Q_OBJECT
public:
    explicit CellSplitDlg(QDialog *parent = 0);
    ~CellSplitDlg();

    void createUI();
    void initUI();
public slots:
 virtual  void  accept();
   // void btn_Ok();
	//void close();
CREATE_UI_BEGIN
public:
    QDialogButtonBox *buttonBox;
    QLabel *columnLabel;
    QLabel *rowLabel;
    QLineEdit *columnEdit;
    QLineEdit *rowEdit;
	int  column;
	int  row;
CREATE_UI_END

};

#endif // CELLSPLITDLG_H