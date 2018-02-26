#ifndef PHARMACYWITHDRAWALAPPLICATION_H
#define PHARMACYWITHDRAWALAPPLICATION_H
#include "qgroupbox.h"
#include"qpushbutton.h"
#include"qboxlayout.h"
#include "customDelegate/CustomTableView.h"
#include "qlabel.h"

// 代码一定要是: UTF-8(BOM)
//qt版本高于等于qt5，VS版本高于或等于VS2010
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
class PharmacyWithdrawalApplication:public QGroupBox 
{
    Q_OBJECT
public:
	explicit PharmacyWithdrawalApplication(QWidget *parent = 0);
    ~PharmacyWithdrawalApplication();
	QPushButton *addBtn;
	QPushButton *modifyBtn;
	QPushButton *deleteBtn;
	QPushButton *commitUpBtn;
	QPushButton *filterBtn;
	QPushButton *exportBtn;
	QPushButton *printBtn;
	QPushButton *exitBtn;

	QLabel  *libraryListLabel;
	QLabel *libraryListDetailLabel;
	CustomTableView *libraryListView;
	CustomTableView *libraryListDetailedView;
private:
	QHBoxLayout *topLayout;
	QVBoxLayout *libraryListLayout;
	QVBoxLayout *libraryListDetailedLayout;
	QVBoxLayout *AllLayout;

	QGroupBox *topGroupBox;
	QGroupBox *libraryListGroupBox;
	QGroupBox *libraryListDetailedGroupBox;
public:
	static PharmacyWithdrawalApplication *getInstance();
	void InitData();
protected:
	static PharmacyWithdrawalApplication *pInstance;

};
#endif //PHARMACYWITHDRAWALAPPLICATION_H
