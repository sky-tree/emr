#include "views/pharmacyManagement/pharmacyWithdrawalApplication.h"

PharmacyWithdrawalApplication *PharmacyWithdrawalApplication::pInstance = NULL;
PharmacyWithdrawalApplication::PharmacyWithdrawalApplication(QWidget *parent) :QGroupBox(parent)
{
	this->setStyleSheet("QGroupBox{border:none;}");
	this->setStyleSheet("QLabel{border:none;}");
	this->setStyleSheet("QPushButton{background-color: rgb(113, 187, 250);border: 1px solid rgb(124, 124, 124);\
		border-radius:1px;}QWidget > QPushButton:hover{background-color:rgb(154, 205, 252)}");
	topLayout = new QHBoxLayout();
	libraryListLayout = new QVBoxLayout();
	libraryListDetailedLayout = new QVBoxLayout();
	AllLayout = new QVBoxLayout();
	topGroupBox = new QGroupBox();
	libraryListGroupBox = new QGroupBox();
	libraryListDetailedGroupBox = new QGroupBox();

	addBtn = new QPushButton(tr("新增(N)"));
	modifyBtn = new QPushButton(tr("修改(E)"));
	deleteBtn = new QPushButton(tr("删除(D)"));
	commitUpBtn = new QPushButton(tr("提交(A)"));
	filterBtn = new QPushButton(tr("筛选(Q)"));
	exportBtn = new QPushButton(tr("导出(X)"));
	printBtn = new QPushButton(tr("打印(P)"));
	exitBtn = new QPushButton(tr("退出(C)"));
	addBtn->setMinimumSize(80, 31);
	modifyBtn->setMinimumSize(80, 31);
	deleteBtn->setMinimumSize(80, 31);
	commitUpBtn->setMinimumSize(80, 31);
	filterBtn->setMinimumSize(80, 31);
	exportBtn->setMinimumSize(80, 31);
	printBtn->setMinimumSize(80, 31);
	exitBtn->setMinimumSize(80, 31);
	topLayout->setContentsMargins(0, 10, 0, 8);
	topLayout->addWidget(addBtn);
	topLayout->addWidget(modifyBtn);
	topLayout->addWidget(deleteBtn);
	topLayout->addWidget(commitUpBtn);
	topLayout->addWidget(filterBtn);
	topLayout->addWidget(exportBtn);
	topLayout->addWidget(printBtn);
	topLayout->addWidget(exitBtn);
	topLayout->setSpacing(10);
	topLayout->setAlignment(Qt::AlignLeft);
	topGroupBox->setAlignment(Qt::AlignLeft);
	topGroupBox->setLayout(topLayout);
	QFont ft;
	ft.setPointSize(12);
	libraryListLabel = new QLabel("药房出库单列表:");
	libraryListLabel->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,"
		"stop: 0 rgba(27, 132, 248, 100%),stop: 1 rgba(27, 165, 249, 100%));");
	libraryListLabel->setMinimumHeight(20);
	libraryListLabel->setFont(ft);
	libraryListView = new CustomTableView();
	libraryListLayout->setMargin(0);
	libraryListLayout->setSpacing(0);
	libraryListLayout->addWidget(libraryListLabel);
	libraryListLayout->addWidget(libraryListView);
	libraryListGroupBox->setLayout(libraryListLayout);

	
	libraryListDetailLabel = new QLabel("药房出库单药品明细列表:"); 
	libraryListDetailLabel->setMinimumHeight(20);
	libraryListDetailLabel->setFont(ft);
	libraryListDetailLabel->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,"
		"stop: 0 rgba(27, 132, 248, 100%),stop: 1 rgba(27, 165, 249, 100%));");
	libraryListDetailedView = new CustomTableView();
	libraryListDetailedLayout->setMargin(0);
	libraryListDetailedLayout->setSpacing(0);
	libraryListDetailedLayout->addWidget(libraryListDetailLabel);
	libraryListDetailedLayout->addWidget(libraryListDetailedView);
	libraryListDetailedGroupBox->setLayout(libraryListDetailedLayout);

	AllLayout->setMargin(0);
	AllLayout->addWidget(topGroupBox);
	AllLayout->addWidget(libraryListGroupBox);
	AllLayout->addWidget(libraryListDetailedGroupBox);
	this->setLayout(AllLayout);
}
PharmacyWithdrawalApplication *PharmacyWithdrawalApplication::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new PharmacyWithdrawalApplication();
	}
	return pInstance;
}
PharmacyWithdrawalApplication::~PharmacyWithdrawalApplication()
{
}
void PharmacyWithdrawalApplication::InitData()
{

}
