#include "SimpleElementSettingWidget.h"

#include <QVBoxLayout>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <qheaderview.h>
#include <QComboBox>
#include <qdebug.h>
#include"qsignalmapper.h"
#include "propertySettingDlgs\selectionpropertydlg.h" 

SimpleElementSettingWidget::SimpleElementSettingWidget(QWidget*parent) : QWidget(parent)
{
	btnsContainerHLayout = NULL;
	horizontalLayout_2 = NULL;
	saveBtn = NULL;
	deleteBtn = NULL;
	addBtn = NULL;
	settingTableWidget = NULL;
	contentVLayout = NULL;
	verticalLayout_2 = NULL;
	btnsContainerWidget = NULL;
	createUI();
	initUI();
	signalMapper = new QSignalMapper(this);
	connect(addBtn, SIGNAL(clicked(bool)), this, SLOT(addRow()));
	connect(deleteBtn, SIGNAL(clicked(bool)), this, SLOT(delRow()));

	settingTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	QString comboBoxStyle = "QComboBox{border:solid;}";
	this->setStyleSheet(comboBoxStyle);
}

SimpleElementSettingWidget::~SimpleElementSettingWidget()
{

}

void SimpleElementSettingWidget::createUI()
{
	if (this->objectName().isEmpty())
		this->resize(631, 640);
	verticalLayout_2 = new QVBoxLayout(this);
	btnsContainerWidget = new QWidget(this);
	btnsContainerWidget->setMaximumSize(QSize(16777215, 45));
	horizontalLayout_2 = new QHBoxLayout(btnsContainerWidget);
	btnsContainerHLayout = new QHBoxLayout();
	addBtn = new QPushButton(btnsContainerWidget);
	addBtn->setMaximumSize(QSize(120, 16777215));
	addBtn->setMinimumHeight(31);

	btnsContainerHLayout->addWidget(addBtn);

	deleteBtn = new QPushButton(btnsContainerWidget);
	deleteBtn->setMaximumSize(QSize(120, 16777215));
	deleteBtn->setMinimumHeight(31);

	btnsContainerHLayout->addWidget(deleteBtn);

	saveBtn = new QPushButton(btnsContainerWidget);
	saveBtn->setMaximumSize(QSize(120, 16777215));
	saveBtn->setMinimumHeight(31);

	btnsContainerHLayout->addWidget(saveBtn);


	horizontalLayout_2->addLayout(btnsContainerHLayout);


	verticalLayout_2->addWidget(btnsContainerWidget);

	contentVLayout = new QVBoxLayout();
	settingTableWidget = new QTableWidget(this);
	if (settingTableWidget->columnCount() < 3)
		settingTableWidget->setColumnCount(3);
	QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
	settingTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
	QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
	settingTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
	QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
	settingTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);

	contentVLayout->addWidget(settingTableWidget);


	verticalLayout_2->addLayout(contentVLayout);

}

void SimpleElementSettingWidget::initUI()
{
	this->setWindowTitle(tr("Form"));
	addBtn->setText(tr("增加"));
	deleteBtn->setText(tr("删除"));
	saveBtn->setText(tr("保存"));
	QTableWidgetItem *___qtablewidgetitem = settingTableWidget->horizontalHeaderItem(0);
	___qtablewidgetitem->setText(tr("项目名称"));
	QTableWidgetItem *___qtablewidgetitem1 = settingTableWidget->horizontalHeaderItem(1);
	___qtablewidgetitem1->setText(tr("节点属性"));
	QTableWidgetItem *___qtablewidgetitem2 = settingTableWidget->horizontalHeaderItem(2);
	___qtablewidgetitem2->setText(tr("内容编辑"));
}

void SimpleElementSettingWidget::addRow()
{
	int totalRow = settingTableWidget->rowCount();
	settingTableWidget->setRowCount(totalRow + 1);
	settingTableWidget->setCurrentCell(totalRow + 1, QItemSelectionModel::Select);
	settingTableWidget->setFocus(Qt::MouseFocusReason);

	QComboBox * box = createPropertyCBox();
	settingTableWidget->setCellWidget(totalRow, 1, box);
	connect(box, SIGNAL(currentIndexChanged(int)), this, SLOT(onQComboBoxClicked(int)));
	box->setProperty("index", QVariant(totalRow));

	QPushButton * btn = new QPushButton();
	settingTableWidget->setCellWidget(totalRow, 2, btn);
	btn->setText(tr("编辑"));
	btn->setProperty("index", QVariant(totalRow));
	connect(btn, SIGNAL(clicked(bool)), this, SLOT(onCellBtnClicked()));

	ElementData * data = new ElementData();
	data->comboBoxText = tr("测试");
	data->type = SimpleElementSettingWidget::NONE;

	//set user data
	settingTableWidget->setUserData(totalRow, data);
}
void SimpleElementSettingWidget::onQComboBoxClicked(int i)
{
	QComboBox *sender = qobject_cast<QComboBox *>(this->sender());

	ElementData * elementData = (ElementData *)settingTableWidget->userData(sender->property("index").toInt());
	elementData->type = ProjectType(i);
	elementData->comboBoxText = sender->itemText(i);
}

void SimpleElementSettingWidget::onCellBtnClicked()
{
	QPushButton * btn = (QPushButton *)this->sender();
	ElementData * elementData = (ElementData *)settingTableWidget->userData(btn->property("index").toInt());

	QTableWidgetItem * item = settingTableWidget->item(btn->property("index").toInt(), 0);
	if (item != NULL)
	{
		item->setText(elementData->comboBoxText);
	}

	switch (elementData->type)
	{
	case NONE:
		break;
	case SingleSelection:
	{
							SelectionPropertyDlg dlg;
							dlg.exec();
	}
		break;
	case MultiSelection:
	{
						   SelectionPropertyDlg dlg;
						   dlg.exec();
	}
		break;
	case IsHaveSelection:
	{
							SelectionPropertyDlg dlg;
							dlg.exec();
	}
		break;
	case Number:
	{

	}
		break;
	case Macro:
	{

	}
		break;
	default:
		break;
	}
}

void SimpleElementSettingWidget::delRow()
{
	int totalRow = settingTableWidget->rowCount();
	if (totalRow > 0)
	{
		int row = settingTableWidget->currentRow();
		settingTableWidget->removeCellWidget(row, 2);
		settingTableWidget->removeCellWidget(row, 1);
		settingTableWidget->removeRow(row);
	}
}

QComboBox * SimpleElementSettingWidget::createPropertyCBox()
{
	QComboBox * box = new QComboBox();
	box->insertItems(0, QStringList()
		<< tr("无")
		<< tr("单选")
		<< tr("多选")
		<< tr("有无选择")
		<< tr("数字")
		<< tr("患者基本信息"));
	return box;
}
