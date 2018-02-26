#include "MubanPropertyModifyDlg.h"

#include"dataModel\OperateData\DataBaseControl.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include "MessageBox.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "globalvariable.h"

MubanPropertyModifyDlg::MubanPropertyModifyDlg(QDialog*parent, QStringList & strList) : QDialog(parent)
, data(strList)
{
    writeLabel = NULL;
    timeLabel = NULL;
    departmentLabel = NULL;
    classifyCodeLabel = NULL;
    nameLabel = NULL;
    mubanCodeLabel = NULL;
    writeEdit = NULL;
    timeEdit = NULL;
    departmentEdit = NULL;
    classifyCodeEdit = NULL;
    nameEdit = NULL;
    mubanCodeEdit = NULL;
    cancelBtn = NULL;
    saveBtn = NULL;
	m_DataBaseControl = NULL;
	m_DbManager = NULL;

	//if (m_DbManager == NULL)
	//{
	//	m_DbManager = new DBManager();
	//	m_DbManager->connectOracle();
	//}
	if (m_DataBaseControl == NULL)
	{
		m_DataBaseControl = new DataBaseControl(NULL);
	}

    createUI();
    initUI();

	connect(saveBtn, SIGNAL(clicked()), this, SLOT(onSaveBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(onCancelBtnClicked()));

	timeEdit->setEnabled(false);
	writeEdit->setEnabled(false);
}

MubanPropertyModifyDlg::~MubanPropertyModifyDlg()
{
	if (m_DataBaseControl)
	{
		delete m_DataBaseControl;
		m_DataBaseControl = NULL;
	}
	if (m_DbManager)
	{
		delete m_DbManager;
		m_DbManager = NULL;
	}
}

void MubanPropertyModifyDlg::createUI()
{
	this->setStyleSheet(kPushBtnStyle);
	if (this->objectName().isEmpty())
		this->setFixedSize(230, 250);
        mubanCodeLabel = new QLabel(this);
		mubanCodeEdit = new QLineEdit(this);


		classifyCodeLabel = new QLabel(this);
		classifyCodeEdit = new QLineEdit(this);

        nameLabel = new QLabel(this);
		nameEdit = new QLineEdit(this);


        departmentLabel = new QLabel(this);
		departmentEdit = new QLineEdit(this);

        timeLabel = new QLabel(this);
		timeEdit = new QLineEdit(this);

        writeLabel = new QLabel(this);
        writeEdit = new QLineEdit(this);

		QFormLayout* fLayout = new QFormLayout();
		fLayout->addRow(mubanCodeLabel, mubanCodeEdit);
		fLayout->addRow(classifyCodeLabel, classifyCodeEdit);
		fLayout->addRow(nameLabel, nameEdit);
		fLayout->addRow(departmentLabel, departmentEdit);
		fLayout->addRow(timeLabel, timeEdit);
		fLayout->addRow(writeLabel, writeEdit);

        saveBtn = new QPushButton(this);
		saveBtn->setFixedSize(81, 23);
        cancelBtn = new QPushButton(this);
		 cancelBtn->setFixedSize(81, 23);

		QHBoxLayout* hLayout = new QHBoxLayout();
		hLayout->addStretch();
		hLayout->addWidget(saveBtn);
		hLayout->addWidget(cancelBtn);

		QVBoxLayout* vLayout = new QVBoxLayout(this);
		vLayout->addLayout(fLayout);
		vLayout->addLayout(hLayout);
		this->setLayout(vLayout);
		
}

void MubanPropertyModifyDlg::initUI()
{ 
	this->setWindowTitle(tr("模板属性修改"));
	mubanCodeLabel->setText(tr("模板代码"));
	nameLabel->setText(tr("模板名称"));
	classifyCodeLabel->setText(tr("分类代码"));
	departmentLabel->setText(tr("使用科室"));
	//if (!DataBaseControl::m_docutorData->isSupperUser)
	//{
	//	departmentLabel->setEnabled(false);
	//	departmentEdit->setEnabled(false);

	//}
	//else
	//{
	//	departmentLabel->setEnabled(true);
	//	departmentEdit->setEnabled(true);
	//}
	departmentLabel->setEnabled(false);
	departmentEdit->setEnabled(false);
	timeLabel->setText(tr("时间质控"));
	writeLabel->setText(tr("书写质控"));
	//departmentEdit->setText(QString());
	saveBtn->setText(tr("保存"));
	cancelBtn->setText(tr("取消"));
}

void MubanPropertyModifyDlg::changeData(QStringList & strList)
{
	MubanPropertyModifyDlg dialog(NULL, strList);
	dialog.classifyCodeEdit->setText(strList[0]);
	dialog.mubanCodeEdit->setText(strList[1]);
	dialog.nameEdit->setText(strList[2]);
	dialog.departmentName = strList[3];
	dialog.timeEdit->setText(strList[4]);
	dialog.writeEdit->setText(strList[5]);
	dialog.initDepartmentData();

	if (dialog.exec() == QDialog::Accepted)
	{
		return;
	}
}

void MubanPropertyModifyDlg::onSaveBtnClicked()
{

	QString text = nameEdit->text();
	text = text.trimmed();
	//text = text.replace(" ", "");
	if (text.length() == 0)
	{
		MessageBox::showAboutMessageBox(QStringLiteral("警告"), QStringLiteral("模板名称不能为空"));
		return;
	}
	if (text.length() > 20)
	{
		MessageBox::showAboutMessageBox(QStringLiteral("警告"), QStringLiteral("模板名称不能超过20个字符！"));
		return;
	}
	if (text.contains("\\") || text.contains("/") || text.contains("*") || text.contains("?") || text.contains("<") || text.contains(">") || text.contains("\""))
	{
		MessageBox::showAboutMessageBox(QStringLiteral("警告"), QStringLiteral("模板名称不能包含\\ / * ? < > \""));
		return;
	}

	
	data.clear();
	//QStringList stringList = departmentEdit->currentText().split(")");
	data << classifyCodeEdit->text() << mubanCodeEdit->text()
		<< nameEdit->text() /*<< stringList[1] << stringList[0].mid(1, stringList[0].length() - 1)*/;
		//<< timeEdit->text() << writeEdit->text();

	this->close();
}

void MubanPropertyModifyDlg::onCancelBtnClicked()
{
	this->close();
}

void MubanPropertyModifyDlg::initDepartmentData()
{
	departmentEdit->setText(departmentName);
}
