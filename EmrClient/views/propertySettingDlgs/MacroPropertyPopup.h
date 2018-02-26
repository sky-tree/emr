#ifndef MACROPROPERTYPOPUP_H
#define MACROPROPERTYPOPUP_H

#include <QDialog>
#include "customDelegate/MSC_VER.h"

class  QLabel;
class  QLineEdit;
class  QPushButton;
class  QComboBox;
class MacroPropertyPopup : public QDialog
{
    Q_OBJECT
public:
    explicit MacroPropertyPopup(QDialog *parent = 0);
    ~MacroPropertyPopup();

    void createUI();
    void initUI(); 
	void InitData(QMap<QString, QString> &map);
 
	static void getData(QString &name, QString &id, QString &defaultValue);
CREATE_UI_BEGIN
public:
    QLabel *defaultValueLabel;
    QLabel *connectIDLabel;
    QLabel *nameLabel;
    QLineEdit *defaultValueEdit;
    QComboBox *connectIDEdit;
    QLineEdit *nameEdit;
    QPushButton *cancelBtn;
    QPushButton *confirmBtn;
CREATE_UI_END

private slots:
	void onConfirmBtnClicked();
	void onCancelBtnClicked();
private:
	void resetMap();
	QMap<QString,QString> *m_map;
	void connectSlots();
};

#endif // MACROPROPERTYPOPUP_H