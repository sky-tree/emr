#include "addmubandlg.h"
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QTextEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QDebug>
#include"globalvariable.h"
#include"dataModel\OperateData\DataBaseControl.h"
#include<QMessageBox>
#include"views/MainViewContainer.h"
#include "MessageBox.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#define MAX_DLG_SIZE 284, 366
#define DEPT_CODE_NAME "(0000)所有科室"
AddMuBanDlg::AddMuBanDlg(QWidget *parent) : QDialog(parent), m_DataBaseControl(NULL), model(NULL), m_DbManager(NULL)
{
	QString path_ = QCoreApplication::applicationDirPath();
	path_ += "/debug.log";
	m_pLogFile = new QFile(path_);
	if (!m_pLogFile->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		m_pLogFile = NULL;
	}
	else
	{
		m_pTxtOutput = new QTextStream(m_pLogFile);
	}

    this->setWindowTitle(tr("新增模板"));
    this->setModal(true);
	this->setFixedSize(280, 300);
	if (m_DataBaseControl == NULL)
	{
		m_DataBaseControl = new DataBaseControl(NULL);
	}
	map = new QMap<QString, QString>();
    createWidgets();
    loadComboxData();
	
    this->departmentComboBox->setEnabled(false);
    departmentLabel->setEnabled(false);
	this->setStyleSheet(kPushBtnStyle);
	this->setWindowIcon(QIcon(":/res/Res/MenuIcon/case.png"));

	setComboxValue(); 
	setRadioBtnEnable();
}
AddMuBanDlg::~AddMuBanDlg()
{
	if (m_DbManager)
	{
		delete m_DbManager;
		m_DbManager = NULL;
	}
	if (m_DataBaseControl)
	{
		delete m_DataBaseControl;
		m_DataBaseControl = NULL;
	}
	if (map)
	{
		delete map;
		map = NULL;
	}
	m_pLogFile->close();
}
void AddMuBanDlg::setRadioBtnEnable()
{
	publicRB->setEnabled(false); 
	departmentRB->setEnabled(false);
	personalRB->setEnabled(false);
	//operatorTemplateRight oTR = (*(DataBaseControl::m_operatorTemplate))[strMapType];
	 QList<QString> listStr=DataBaseControl::m_operatorTemplate->keys();
	 QListIterator<QString> listIterator(listStr);
	 while (listIterator.hasNext())
	 {
		 //1:个人，2:科室,3:全院
		 QString str = listIterator.next();
		 if (str == ""||str.length()==0){ continue; }
		 int type = str.toInt();
		 bool isAdd = (*(DataBaseControl::m_operatorTemplate))[str].addTemplate;
		 switch (type)
		 {
		 case 1: personalRB->setEnabled(isAdd); break;
		 case 2: departmentRB->setEnabled(isAdd);  break;
		 case 3: publicRB->setEnabled(isAdd); break;
		 default:
			 break;
		 }
	 }
	 if (departmentRB->isEnabled()){departmentRB->setChecked(true);}
	 else if (personalRB->isEnabled()){ personalRB->setChecked(true); }
	 else if (publicRB->isEnabled()){ publicRB->setChecked(true); }

	 if (DataBaseControl::m_docutorData->isSupperUser)
	 {
		 publicRB->setEnabled(true);
		 departmentRB->setEnabled(true);
		 personalRB->setEnabled(false);
	 }
}
void AddMuBanDlg::init(QMap<QString, QString> &map)
{
	QString businessName = map["businessName"];
	businessComboBox->setCurrentText(businessName);	
}
void AddMuBanDlg::onPushBtnClicked()
{
	debuglogs("AddMuBanDlg::onPushBtnClicked::enter");
        QObject * sender = this->sender();
		map->clear();
        if (sender == saveButton)
        {
			QString text = lineEdit->text();
			text = text.trimmed();
			//text = text.replace(" ", "");
			if (text.length()==0)
			{
				MessageBox::showErrorMessageBox("警告", "模板名称不能为空");
				return;
			}
			if (text.contains("\\") || text.contains("/") || text.contains("*") || text.contains("?") || text.contains("<") || text.contains(">") || text.contains("\""))
			{
				MessageBox::showErrorMessageBox("警告", "模板名称不能包含\\ / * ? < > \"");
				return;
			}
			if (text.length() > 20)
			{
				MessageBox::showErrorMessageBox("警告", "模板名称不能超过20个字符!");
				return;
			}
			map->insert("operateType", "Add");
			map->insert("templateName", text);
			
			map->insert("businessName", businessComboBox->currentText());
			//int type = getType();
			//map->insert("type", QString::number(type));
			map->insert("content", ""); 
			map->insert("isMore", "");
			map->insert("isMust", "");
			map->insert("templateType", QString::number(getTemplateType()));
			if (getTemplateType()== 1)
			{
				map->insert("departmentName", DEPT_CODE_NAME);
			}
			else
			{
				map->insert("departmentName", departmentComboBox->currentText());
			}
			emit QDialog::accept();
        }
        else if (sender == quitButton)
        {
            this->close();
        }
		debuglogs("AddMuBanDlg::onPushBtnClicked::exit");
}
QString AddMuBanDlg::getCheckBoxIsChecked(QCheckBox *checkBox)
{
	QString isCheck = "false";
	if (checkBox->isChecked())
	{
		isCheck = "true";
	}
	return isCheck;
}
int AddMuBanDlg::getTemplateType()
{
	int type = 1;
	if (departmentRB->isChecked())
	{
		type = 2;
	}
	else if (personalRB->isChecked())
	{
		type = 3;
	}
	return type;
}

void AddMuBanDlg::onSecondContainerBtnsClicked(bool checked)
{
	debuglogs("onSecondContainerBtnsClicked::enter");
    QObject * sender = this->sender();
    if (sender == departmentRB)
    {
		if (DataBaseControl::m_docutorData->isSupperUser || publicRB->isEnabled())
		{
			departmentComboBox->setEnabled(checked);
			departmentLabel->setEnabled(checked);
		}
		else
		{
			departmentComboBox->setEnabled(false);
			departmentLabel->setEnabled(false);
		}      
    }
	setComboxValue();
	debuglogs("onSecondContainerBtnsClicked::exit");
}
void AddMuBanDlg::setComboxValue()
{
	if (departmentRB->isChecked())
	{
		QString strDocutor = "(" + DataBaseControl::m_docutorData->dept_code + ")" + DataBaseControl::m_docutorData->dept_name;
		departmentComboBox->setCurrentText(strDocutor);
	}
	else if (publicRB->isChecked())
	{
		departmentComboBox->setCurrentText(DEPT_CODE_NAME);
	}
	else if (personalRB->isChecked())
	{
		departmentComboBox->setCurrentText(m_dctDepat);
	}
}
void AddMuBanDlg::createWidgets()
{
    QFont font;
    font.setBold(false);
    font.setWeight(50);
    this->setFont(font);


    nameLabel = new QLabel(this);
    nameLabel->setText(tr("模板名称"));
	lineEdit = new QLineEdit(this);

	businessLabel = new QLabel(this);
	businessLabel->setText(tr("所属业务"));
	businessComboBox = new QComboBox(this);

	QFormLayout* flayout1 = new QFormLayout;
	flayout1->addRow(nameLabel, lineEdit);
	flayout1->addRow(businessLabel, businessComboBox);

	publicRB = new QRadioButton();
	publicRB->setText(tr("全院模板"));
	publicRB->setChecked(true);
	connect(publicRB, SIGNAL(toggled(bool)), this, SLOT(onSecondContainerBtnsClicked(bool)));

	departmentRB = new QRadioButton();
	departmentRB->setText(tr("科室模板"));
	connect(departmentRB, SIGNAL(toggled(bool)), this, SLOT(onSecondContainerBtnsClicked(bool)));

	personalRB = new QRadioButton();
	personalRB->setText(tr("个人模板"));

	QHBoxLayout* hlayout1 = new QHBoxLayout;
	hlayout1->addWidget(publicRB);
	hlayout1->addWidget(departmentRB);
	hlayout1->addWidget(personalRB);

    departmentLabel = new QLabel(this);
    departmentLabel->setText(tr("科室名称"));
	departmentComboBox = new QComboBox(this);

	QFormLayout* flayout2 = new QFormLayout;
	flayout2->addRow(departmentLabel, departmentComboBox);

    saveButton = new QPushButton(this);
    saveButton->setText(tr("保存"));
	saveButton->setFixedSize(81, 23);
    connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(onPushBtnClicked()));
    quitButton = new QPushButton(this);
    quitButton->setText(tr("退出")); 
	quitButton->setFixedSize(81, 23);
    connect(quitButton, SIGNAL(clicked(bool)), this, SLOT(onPushBtnClicked()));

	QHBoxLayout* hlayout3 = new QHBoxLayout;
	hlayout3->addStretch();
	hlayout3->addWidget(saveButton);
	hlayout3->addWidget(quitButton);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addLayout(flayout1);
	layout->addLayout(hlayout1);
	layout->addLayout(flayout2);
	layout->addLayout(hlayout3);
	layout->setSpacing(20);
	setLayout(layout);

}

void AddMuBanDlg::loadComboxData()
{
    departmentComboBox->clear();
    //departmentComboBox->insertItems(0, QStringList()
    // << ""<< "神经科"<< "精神科"<< "外科");
	//科室加载
	model = NULL;
	if (m_DataBaseControl == NULL)return;
	model = &m_DataBaseControl->getQueryData("SELECT T1.* FROM DEPT_LIST T1\
		LEFT JOIN DEPT_PROPERTY_REL T2 ON T1.DEPT_CODE = T2.DEPT_CODE\
		WHERE T2.DEPT_PROPERTY_CODE = 39 order by T1.DEPT_CODE" );
	QStringList departList;
	for (int i = 0; i < model->rowCount();i++)
    {
		QSqlRecord deRecord = model->record(i);
		QString    code = deRecord.value("DEPT_CODE").toString();
		QString    deName = deRecord.value("DEPT_NAME").toString();
		QString    codeName = "(" + code + ")" + deName;
		departList.append(codeName);
    }
	QString useDepart = "(0000)所有科室";
	departList.append(DEPT_CODE_NAME);
	departmentComboBox->insertItems(0, departList);
	if (!DataBaseControl::m_docutorData->isSupperUser)
	{
		m_dctDepat = "(" + DataBaseControl::m_docutorData->dept_code + ")" + DataBaseControl::m_docutorData->dept_name;
		departmentComboBox->setCurrentText(m_dctDepat);
	}
    //所属节点加载
    businessComboBox->clear();
	delete model;
	model = NULL;
	model = &m_DataBaseControl->getQueryData("select * from MEDICALRECORDS_TYPE order by CLASSIFYCODE");
	QStringList list;
	for (int i = 0; i < model->rowCount(); i++)
	{
		QSqlRecord record = model->record(i);
		QString name = record.value("NAME").toString();
		QString classifyCode = record.value("CLASSIFYCODE").toString();
		QString nameClass = "(" + classifyCode + ")" + name;
		list.append(nameClass);
	}
	//list.append("其他");//测试使用
	businessComboBox->insertItems(0, list);

	/* businessComboBox->insertItems(0, QStringList()<<"住院病历"<<"病程记录"
		 << "手术记录"<< "护理记录"<<"知情同意书"<<"申请书"<<"诊疗报告"<<"其他报告"<< "医嘱");*/
	//by caokewen 2017/5/10 delete memory
	delete model;
	model = NULL;
}

void AddMuBanDlg::debuglogs(QString logs)
{
	*m_pTxtOutput << logs << endl;
}
