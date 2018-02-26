#include "inspection.h"
#include"dataModel\OperateData\DataBaseControl.h"
#include"dataModel/DBDataModel/DBManager.h"
#include<QCheckBox>
#include "TabWidgetCollection.h"
#include"MyAppEntrance.h"

#define EXMINEVIEWSTRING "检查所见:"
#define EXMINCLUSIONSTRING "检查结论:" 
#define INSPECTREFERENCEVALUE "参考值:"
#define RESULTDESCRIBLE "结果描述:"
Inspection::Inspection(QWidget *parent)
: QDialog(parent), returnData(""), m_DbManager(NULL),examineResultModel(NULL), inspectMicroResultModel(NULL), inspectResultModel(NULL)
{
	ui.setupUi(this);
	ui.splitter->setStretchFactor(0, 1);
	ui.splitter->setStretchFactor(1, 0);

	/*if (m_DbManager == NULL)
	{
		m_DbManager = new DBManager();
		m_DbManager->connectOracle();
	}*/
	m_DataBaseControl = new DataBaseControl(NULL);
	createConnect();
	initData();
	this->setMinimumSize(QSize(1000, 600));
}

Inspection::~Inspection()
{
	//if (examineResultModel != NULL)
	//{
	//	delete examineResultModel;
	//	examineResultModel = NULL;
	//}
	//if (inspectResultModel != NULL)
	//{
	//	delete inspectResultModel;
	//	inspectResultModel = NULL;
	//}
	//if (inspectMicroResultModel != NULL)
	//{
	//	delete inspectMicroResultModel;
	//	inspectMicroResultModel = NULL;
	//}
	if (m_DataBaseControl != NULL)
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
QString Inspection::getData()
{
	QWidget *widget = TabWidgetCollection::getInstance()->getCurrentWidget();
	if (widget == NULL) return "";
	Inspection dlg(widget);
	if (dlg.exec() == Accepted)
	{
		return dlg.getReturnData();
	}
	return "";
}

void Inspection::initData()
{
	initSql();
	if (!initModel())return;

	initTableView();

}

QString Inspection::getReturnData() const
{
	return returnData;
}

void Inspection::initSql()
{
	exmineResultSql = "select * from INP_CHECK_APPLY a where a.INP_NO ='" + DataBaseControl::m_patientData->inp_NO + "'";
	//exmineResultSql = "select * from INP_CHECK_APPLY";
	inspectResultSql = "select * from INP_TEST_RESULT a where a.INP_NO ='" + DataBaseControl::m_patientData->inp_NO + "'";
	//inspectResultSql = "select * from INP_TEST_RESULT" ;
	//inspectMicroResultSql = "select * from INP_TEST_RESULT_MICRO";
	inspectMicroResultSql = "select * from INP_TEST_RESULT_MICRO a where a.INP_NO ='" + DataBaseControl::m_patientData->inp_NO + "'";

	//test 
// 	exmineResultSql = "select * from INP_CHECK_APPLY a where a.INP_NO ='20170100069'";
// 	inspectResultSql = "select * from INP_TEST_RESULT a where a.INP_NO ='20170100069'";
// 	inspectMicroResultSql = "select * from INP_TEST_RESULT_MICRO a where a.INP_NO ='20170100069'";
}

bool Inspection::initModel()
{
	examineResultModel = &m_DataBaseControl->getQueryData(exmineResultSql);
	inspectResultModel = &m_DataBaseControl->getQueryData(inspectResultSql);
	inspectMicroResultModel = &m_DataBaseControl->getQueryData(inspectMicroResultSql);
	if (!examineResultModel || !inspectResultModel || !inspectMicroResultModel)
	{
		return false;
	}
	return true;
}

void Inspection::createConnect()
{
	connect(ui.btn_OK, SIGNAL(clicked()), this, SLOT(ClickResult()));
	connect(ui.btn_Cancel, SIGNAL(clicked()), this, SLOT(ClickResult()));
	connect(ui.tabWidget, SIGNAL(currentChanged(int index)), this, SLOT(tabChanged(int)));
}

void Inspection::ClickResult()
{
	QObject *sender = this->sender();
	if (sender == ui.btn_OK)
	{
		setReturnData();
		QDialog::accept();
	}
	else if (sender == ui.btn_Cancel)
	{
		QDialog::reject();
	}
}

void Inspection::tabChanged(int index)
{

}

void Inspection::initTableView()
{
	initInspectView();
	initExmineView();
	initInspectMicroView();
}

void Inspection::initInspectView()
{
	ui.tableWidget_inspect->setColumnCount(9);
	ui.tableWidget_inspect->setEditTriggers(QAbstractItemView::NoEditTriggers);
	setTableHeaderWidth(ui.tableWidget_inspect);
	//ui.tableWidget_inspect->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
	//导入 名称 英文名 结果 结果描述 参考值，状态，结果单位单位
	QStringList headers,inspectList;
	headers<< "导入" <<"检验单号"<< "检验名称" << "英文名" << "结果" << "结果描述" << "参考值" << "状态" << "单位";
	ui.tableWidget_inspect->setHorizontalHeaderLabels(headers);
	for (int i = 0; i < inspectResultModel->rowCount();i++)
	{
		QSqlRecord record = inspectResultModel->record(i);
		QString   inspectNo = record.value("TEST_NO").toString();
		QString   inspectName = record.value("ITEM_NAME").toString();
		QString   inspectEnglishName = record.value("ITEM_EN_NAME").toString();
		QString   result = record.value("RESULT").toString(); 
		QString   resultdescrible = record.value("RESULT_DESCR").toString();
		QString   range = record.value("RANGE").toString();
		QString   state = record.value("RESULT_STATUS").toString();
		QString   unit = record.value("RESULT_UNIT").toString();
		inspectList.clear();
		inspectList << inspectNo << inspectName << inspectEnglishName << result << resultdescrible << range << state << unit;
		constructWidgetData(inspectList, ui.tableWidget_inspect);
		
	}


}

void Inspection::initInspectMicroView()
{
	// 检验名称 阴性结果描述 微生物名 抗生素名 抗生素结果 mic描述 抗生素mic值 抗生素描述
	ui.tableWidget_inspectMicro->setColumnCount(11);
	//ui.tableWidget_inspectMicro->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget_inspectMicro->horizontalHeader()->setStretchLastSection(true);
	//setTableHeaderWidth(ui.tableWidget_inspectMicro);
	QStringList headers,dataList;
	headers << "导入" << "检验单号"<<"检验名称" << "阴性结果描述" << "微生物名" << "抗生素名" << "抗生素结果" << "mic描述" << "抗生素mic值" <<"mic参考值"<< "抗生素描述";
	ui.tableWidget_inspectMicro->setHorizontalHeaderLabels(headers);
	for (int i = 0; i < inspectMicroResultModel->rowCount();i++)
	{
		dataList.clear();
		QSqlRecord record = inspectMicroResultModel->record(i);
		QString  InspectNO = record.value("TEST_NO").toString();
		QString  InspectName = record.value("ITEM_NAME").toString();
		QString  InspectDescrible = record.value("RESULT_DESCR").toString();
		QString   InspectMicroName = record.value("MICRO_NAME").toString();
		QString  antibioticName = record.value("ANTIBIOTIC_NAME").toString();
		QString  antibioticResult = record.value("ANTIBIOTIC_RESULT").toString();
		QString  micDescrible = record.value("MIC_DESCR").toString();
		QString   antibioticValue = record.value("ANTIBIOTIC_MIC").toString();
		QString    antibioticMicRange = record.value("ANTIBIOTIC_MIC_RANGE").toString();
		QString   antibioticDescrible = record.value("ANTIBIOTIC_DESCR").toString();
		dataList << InspectNO << InspectName << InspectDescrible << InspectMicroName << antibioticName\
			<< antibioticResult << micDescrible << antibioticValue << antibioticMicRange << antibioticDescrible;
		constructWidgetData(dataList, ui.tableWidget_inspectMicro);
	}

}

void Inspection::initExmineView()
{
	ui.tableWidget_exmine->setColumnCount(5);
	ui.tableWidget_exmine->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//setTableHeaderWidth(ui.tableWidget_exmine);
	ui.tableWidget_exmine->horizontalHeader()->setStretchLastSection(true);
	QStringList headers;
	headers << "导入" << "检查名称" <<"检查时间"<< "检查所见" << "检查结论";
	ui.tableWidget_exmine->setHorizontalHeaderLabels(headers);


	for (int i = 0; i < examineResultModel->rowCount();i++)
	{
		QSqlRecord record = examineResultModel->record(i);
		QString  examineName = record.value("ITEM_NAME").toString();
		QString  examineTime = record.value("CHECK_TIME").toString();
		QString  examineView = record.value("FINDINGS").toString();
		QString  examineConclusion = record.value("CONCLUSION").toString();
		QStringList examineDataList;
		examineDataList << examineName << examineTime << examineView << examineConclusion;
		constructWidgetData(examineDataList, ui.tableWidget_exmine);
	}
	
}

void Inspection::setTableHeaderWidth(QTableWidget *widget)
{
	if (widget == NULL)return;
	for (int i = 0; i < widget->columnCount(); i++)
	{
		widget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
	}
}

QWidget* Inspection::createCheckBoxofWidget()
{
	QWidget *widget;
	QHBoxLayout *hLayout;
	QCheckBox *ckb;

	ckb = new QCheckBox;

	hLayout = new QHBoxLayout;
	//widget = new QWidget(tableWidget);
	widget = new QWidget;
	hLayout->addWidget(ckb);
	hLayout->setMargin(0);
	hLayout->setAlignment(ckb, Qt::AlignCenter);
	widget->setLayout(hLayout);
	return widget;
}

void Inspection::constructWidgetData(QStringList listData, QTableWidget *widget)
{
	if (widget == NULL || listData.length() == 0)return;
	int location = widget->rowCount();
	widget->insertRow(location);
	widget->setCellWidget(location, 0, createCheckBoxofWidget());
	for (int j = 1; j < widget->columnCount(); j++)
	{
		widget->setItem(location, j, new QTableWidgetItem(listData.at(j - 1)));
	}
}

void Inspection::setReturnData()
{
	returnData = "";
	//构造检查值
	//构造检验值
	//构造微生物检验值
	returnData = getInspectData() + getInspectMicroData() + getExmineData();
	
}

QString Inspection::getExmineData()
{
	QString data = "";
	for (int i = 0; i < ui.tableWidget_exmine->rowCount();i++)
	{
		QWidget *widget = ui.tableWidget_exmine->cellWidget(i, 0);
		QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
		if (ckb->isChecked())
		{
			QString exmineName = ui.tableWidget_exmine->item(i, 1)->text()+":";
			QString exmineView = ui.tableWidget_exmine->item(i, 3)->text() + ";";
			QString examineConclusion = ui.tableWidget_exmine->item(i, 4)->text();
			data += exmineName + EXMINEVIEWSTRING + exmineView + EXMINCLUSIONSTRING + examineConclusion+ ".\n";
		}
	}
	return data;

}

QString Inspection::getInspectData()
{
	QString data = "";

	for (int i = 0; i < ui.tableWidget_inspect->rowCount(); i++)
	{
		QWidget *widget = ui.tableWidget_inspect->cellWidget(i, 0);
		QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
		if (ckb->isChecked())
		{
			QString inspectName = ui.tableWidget_inspect->item(i, 2)->text();
			QString inspectEnglishName = ui.tableWidget_inspect->item(i, 3)->text();
			if (inspectEnglishName.length()>0)
			{
				inspectName = inspectName + "(" + inspectEnglishName + ")";
			}
			inspectName += ":";
			QString inspectConclusion= ui.tableWidget_inspect->item(i, 4)->text();

			QString inspectUnit = ui.tableWidget_inspect->item(i, 8)->text();
			QString inspectReferenceValue = "(";
			inspectReferenceValue = inspectReferenceValue + INSPECTREFERENCEVALUE + ui.tableWidget_inspect->item(i, 6)->text() + ");";
			QString resultDescrible = ui.tableWidget_inspect->item(i, 5)->text();
			resultDescrible = RESULTDESCRIBLE + resultDescrible;
			

			data += inspectName + inspectConclusion + inspectUnit + inspectReferenceValue + resultDescrible+".\n";
		}
	}


	return data;
}

QString Inspection::getInspectMicroData()
{
	QString data = "";
	for (int i = 0; i < ui.tableWidget_inspectMicro->rowCount(); i++)
	{
		QWidget *widget = ui.tableWidget_inspectMicro->cellWidget(i, 0);
		QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
		if (ckb->isChecked())
		{
			//"导入" << "检验单号" << "检验名称" << "阴性结果描述" << "微生物名" << "抗生素名" << "抗生素结果" << "mic描述" << "抗生素mic值" << "mic参考值" << "抗生素描述";
			QString inspectName = ui.tableWidget_inspectMicro->item(i, 2)->text()+":";
			QString resultDescrible = "阴性结果描述:"+ui.tableWidget_inspectMicro->item(i, 3)->text()+"  ";
			QString microName = "  微生物名:" + ui.tableWidget_inspectMicro->item(i, 4)->text()+"  ";
			QString antibioticName = "  抗生素名:" + ui.tableWidget_inspectMicro->item(i, 5)->text()+"  ";
			QString antibioticResult = "  抗生素结果:"+ui.tableWidget_inspectMicro->item(i, 6)->text()+"  ";
			QString micDescrible = " mic描述:"+ui.tableWidget_inspectMicro->item(i, 7)->text()+"  ";
			QString antibioticMicValue = " 抗生素mic值:"+ ui.tableWidget_inspectMicro->item(i, 8)->text();
			QString micReferenceValue = "(mic参考值:"+ui.tableWidget_inspectMicro->item(i, 9)->text()+") ";
			QString antibioticDescrible ="  抗生素描述:"+ ui.tableWidget_inspectMicro->item(i, 10)->text()+".\n";
			data += inspectName + resultDescrible + microName + antibioticName + antibioticResult + micDescrible + antibioticMicValue + micReferenceValue + antibioticDescrible;
		}
	}
	return data;
}
