#ifndef MUBANPROPERTYMODIFYDLG_H
#define MUBANPROPERTYMODIFYDLG_H

#include <QDialog>

#include "MSC_VER.h"
#include"dataModel/DBDataModel/DBManager.h"
class  QLabel;
class  QLineEdit;
class  QPushButton;
class QComboBox;
class DataBaseControl;

class MubanPropertyModifyDlg : public QDialog
{
    Q_OBJECT
public:
	explicit MubanPropertyModifyDlg(QDialog *parent, QStringList & strList);
    ~MubanPropertyModifyDlg();

	static void changeData(QStringList & strList);
	void initDepartmentData();

    void createUI();
    void initUI();

CREATE_UI_BEGIN
public:
    QLabel *writeLabel;
    QLabel *timeLabel;
    QLabel *departmentLabel;
    QLabel *classifyCodeLabel;
    QLabel *nameLabel;
    QLabel *mubanCodeLabel;
    QLineEdit *writeEdit;
    QLineEdit *timeEdit;
	QLineEdit *departmentEdit;
    QLineEdit *classifyCodeEdit;
    QLineEdit *nameEdit;
    QLineEdit *mubanCodeEdit;
    QPushButton *cancelBtn;
    QPushButton *saveBtn;
CREATE_UI_END

public:
	QStringList &data;
	DBManager *m_DbManager;
	DataBaseControl *m_DataBaseControl;
	QString departmentName;

	public slots:
	void onSaveBtnClicked();
	void onCancelBtnClicked();
};

#endif // MUBANPROPERTYMODIFYDLG_H