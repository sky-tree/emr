#ifndef SELECTIONPOPUP_H
#define SELECTIONPOPUP_H

#include <QDialog>
#include "MSC_VER.h"
#include "TempleteTypeEnumeration.h"

class  QPushButton;
class  QListWidget;

class SelectionPopup : public QDialog
{
    Q_OBJECT
public:
    explicit SelectionPopup(QWidget *parent = 0);
    ~SelectionPopup();

    void createUI();
    void initUI();

	static void getSelection(QString type, QList<DataType*> & data);

	void initRadioBtnList();
	void initCheckBoxList();
	void loadListData(QString type, QList<DataType*> & data);

	QList<DataType*> data;

	static const char RADIO = 0;//单选
	static const char HAVE_CHOICE = 1; //有无选择
	static const char MULTIPLE_CHOICE = 2;//多选

	//QString  QList<DataType> load
	// QList<DataType> return 
CREATE_UI_BEGIN
public:
    QListWidget *listWidget;
    QPushButton *cancelBtn;
    QPushButton *confirmBtn;
CREATE_UI_END

private slots:
	void onConfirmBtnClicked();
	void onCancelBtnClicked();

	void btnToggled(bool isChecked);

public:
	void resetPos();
};

#endif // SELECTIONPOPUP_H