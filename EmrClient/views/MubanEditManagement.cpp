#include "MubanEditManagement.h"
#include "views/SimpleElementSettingWidget.h"
#include "MyAppEntrance.h"
#include "BaseSystem.h"
#include "AuthorityConstants.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QTreeWidget>
#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include"qheaderview.h"
#include <qmenu.h>
#include "globalvariable.h"

#define CREATEACTION(objName, name, tip, signal, slot) \
	objName = new QAction(tr(name), this); \
	objName->setStatusTip(tr(tip)); \
	connect(objName, SIGNAL(signal), this, SLOT(slot));

/*创建树节点*/
#define createTreeNode(stringName,treeNode) \
	userModuleName.clear(); \
	userModuleName.append(tr(stringName)); \
	userModuleRoot = NULL; \
	userModuleRoot = new QTreeWidgetItem(treeNode, userModuleName);

MubanEditManagement::MubanEditManagement(QDialog*parent) : QDialog(parent)
{
	horizontalLayout = NULL;
	btnContainerHLayout = NULL;
	horizontalLayout_7 = NULL;
	horizontalLayout_2 = NULL;
	editBtn = NULL;
	deleteBtn = NULL;
	addBtn = NULL;
	settingSplitter = NULL;
	leftSplitter = NULL;
	mainSplitter = NULL;
	placeholderWidget = NULL;
	editTableWidget = NULL;
	treeWidget = NULL;
	btnsContainerWidget = NULL;

	complexElement = NULL;
	simpleElement = NULL;
	littleTemplate = NULL;
	docOutline = NULL;
	headerAndFooter = NULL;
	medicalFormula = NULL;
	medicalDictionary = NULL;
	specialSymbols = NULL;
	icd10Code = NULL;
	simpleElementSettingWidget = NULL;
	templateEditor = NULL;

	addNodeBefore = NULL;
	addChildNode = NULL;
	addNodeAfter = NULL;
	moveUp = NULL;
	moveDown = NULL;
	deleteNode = NULL;
	rename = NULL;

	createUI();
	initUI();
	createActions();
	placeholderWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	editTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	reloadData();
	treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(treeItemDoubleClicked(QTreeWidgetItem*, int)));
	connect(treeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(rightClickTemplate(const QPoint&)));
	connect(addBtn, SIGNAL(clicked(bool)), this, SLOT(addRow()));
	connect(deleteBtn, SIGNAL(clicked(bool)), this, SLOT(delRow()));

	this->setStyleSheet(kPushBtnStyle);
}

MubanEditManagement::~MubanEditManagement()
{

}

void MubanEditManagement::addRow()
{
	int totalRow = editTableWidget->rowCount();
	editTableWidget->setRowCount(totalRow + 1);
	editTableWidget->setCurrentCell(totalRow + 1, QItemSelectionModel::Select);
	editTableWidget->setFocus(Qt::MouseFocusReason);
}
void MubanEditManagement::delRow()
{
	int totalRow = editTableWidget->rowCount();
	if (totalRow > 0)
	{
		editTableWidget->removeRow(editTableWidget->currentRow());
	}
}

void MubanEditManagement::createUI()
{
	if (this->objectName().isEmpty())
		this->resize(1600, 870);
	this->setMaximumSize(QSize(1580, 940));
	horizontalLayout_2 = new QHBoxLayout(this);
	mainSplitter = new QSplitter(this);
	mainSplitter->setOrientation(Qt::Horizontal);
	leftSplitter = new QSplitter(mainSplitter);
	leftSplitter->setOrientation(Qt::Vertical);
	treeWidget = new QTreeWidget(leftSplitter);
	treeWidget->setMaximumWidth(382);
	
	leftSplitter->addWidget(treeWidget);
	settingSplitter = new QSplitter(leftSplitter);
	settingSplitter->setOrientation(Qt::Vertical);
	btnsContainerWidget = new QWidget(settingSplitter);
	btnsContainerWidget->setMaximumSize(QSize(382, 50));
	horizontalLayout_7 = new QHBoxLayout(btnsContainerWidget);
	btnContainerHLayout = new QHBoxLayout();
	addBtn = new QPushButton(btnsContainerWidget);
	addBtn->setMinimumHeight(31);

	btnContainerHLayout->addWidget(addBtn);

	deleteBtn = new QPushButton(btnsContainerWidget);
	deleteBtn->setMinimumHeight(31);

	btnContainerHLayout->addWidget(deleteBtn);

	editBtn = new QPushButton(btnsContainerWidget);
	editBtn->setMinimumHeight(31);

	btnContainerHLayout->addWidget(editBtn);


	horizontalLayout_7->addLayout(btnContainerHLayout);

	settingSplitter->addWidget(btnsContainerWidget);
	editTableWidget = new QTableWidget(settingSplitter);
	if (editTableWidget->columnCount() < 1)
		editTableWidget->setColumnCount(1);

	settingSplitter->addWidget(editTableWidget);
	leftSplitter->addWidget(settingSplitter);
	mainSplitter->addWidget(leftSplitter);
	placeholderWidget = new QTableWidget(mainSplitter);
	if (placeholderWidget->columnCount() < 9)
		placeholderWidget->setColumnCount(9);
	QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
	placeholderWidget->setHorizontalHeaderItem(0, __qtablewidgetitem17);
	QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
	placeholderWidget->setHorizontalHeaderItem(1, __qtablewidgetitem18);
	QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
	placeholderWidget->setHorizontalHeaderItem(2, __qtablewidgetitem19);
	QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
	placeholderWidget->setHorizontalHeaderItem(3, __qtablewidgetitem20);
	QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
	placeholderWidget->setHorizontalHeaderItem(4, __qtablewidgetitem21);
	QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
	placeholderWidget->setHorizontalHeaderItem(5, __qtablewidgetitem22);
	QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
	placeholderWidget->setHorizontalHeaderItem(6, __qtablewidgetitem23);
	QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
	placeholderWidget->setHorizontalHeaderItem(7, __qtablewidgetitem24);
	QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
	placeholderWidget->setHorizontalHeaderItem(8, __qtablewidgetitem25);
	if (placeholderWidget->rowCount() < 8)
		placeholderWidget->setRowCount(8);
	QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
	placeholderWidget->setVerticalHeaderItem(0, __qtablewidgetitem26);
	QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
	placeholderWidget->setVerticalHeaderItem(1, __qtablewidgetitem27);
	QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
	placeholderWidget->setVerticalHeaderItem(2, __qtablewidgetitem28);
	QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
	placeholderWidget->setVerticalHeaderItem(3, __qtablewidgetitem29);
	QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
	placeholderWidget->setVerticalHeaderItem(4, __qtablewidgetitem30);
	QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
	placeholderWidget->setVerticalHeaderItem(5, __qtablewidgetitem31);
	QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
	placeholderWidget->setVerticalHeaderItem(6, __qtablewidgetitem32);
	QTableWidgetItem *__qtablewidgetitem33 = new QTableWidgetItem();
	placeholderWidget->setVerticalHeaderItem(7, __qtablewidgetitem33);
	mainSplitter->addWidget(placeholderWidget);

	horizontalLayout_2->addWidget(mainSplitter);

	horizontalLayout = new QHBoxLayout();

	horizontalLayout_2->addLayout(horizontalLayout);

	createRootNodes();
}

void MubanEditManagement::createRootNodes()
{
	complexElement = new QTreeWidgetItem(treeWidget);
	simpleElement = new QTreeWidgetItem(treeWidget);
	littleTemplate = new QTreeWidgetItem(treeWidget);
	docOutline = new QTreeWidgetItem(treeWidget);
	headerAndFooter = new QTreeWidgetItem(treeWidget);
	medicalFormula = new QTreeWidgetItem(treeWidget);
	medicalDictionary = new QTreeWidgetItem(treeWidget);
	specialSymbols = new QTreeWidgetItem(treeWidget);
	icd10Code = new QTreeWidgetItem(treeWidget);

	treeWidget->headerItem()->setText(0, tr("电子病历目录"));
	const bool __sortingEnabled = treeWidget->isSortingEnabled();
	treeWidget->setSortingEnabled(false);
	complexElement->setText(0, tr("复杂元素"));
	simpleElement->setText(0, tr("简单元素"));
	littleTemplate->setText(0, tr("病历小模板"));
	docOutline->setText(0, tr("文档提纲"));
	headerAndFooter->setText(0, tr("页眉页脚"));
	medicalFormula->setText(0, tr("医学公式"));
	medicalDictionary->setText(0, tr("医学词典"));
	specialSymbols->setText(0, tr("特殊符号"));
	icd10Code->setText(0, tr("icd10疾病编码"));
	treeWidget->setSortingEnabled(__sortingEnabled);
}

void MubanEditManagement::initUI()
{
	this->setWindowTitle(tr("模板编辑管理"));

	addBtn->setText(tr("新增"));
	deleteBtn->setText(tr("删除"));
	editBtn->setText(tr("编辑"));
	editTableWidget->horizontalHeader()->setVisible(false);

	QTableWidgetItem *___qtablewidgetitem17 = placeholderWidget->horizontalHeaderItem(0);
	___qtablewidgetitem17->setText(tr("新建列"));
	QTableWidgetItem *___qtablewidgetitem18 = placeholderWidget->horizontalHeaderItem(1);
	___qtablewidgetitem18->setText(tr("1"));
	QTableWidgetItem *___qtablewidgetitem19 = placeholderWidget->horizontalHeaderItem(2);
	___qtablewidgetitem19->setText(tr("2"));
	QTableWidgetItem *___qtablewidgetitem20 = placeholderWidget->horizontalHeaderItem(3);
	___qtablewidgetitem20->setText(tr("3"));
	QTableWidgetItem *___qtablewidgetitem21 = placeholderWidget->horizontalHeaderItem(4);
	___qtablewidgetitem21->setText(tr("4"));
	QTableWidgetItem *___qtablewidgetitem22 = placeholderWidget->horizontalHeaderItem(5);
	___qtablewidgetitem22->setText(tr("5"));
	QTableWidgetItem *___qtablewidgetitem23 = placeholderWidget->horizontalHeaderItem(6);
	___qtablewidgetitem23->setText(tr("6"));
	QTableWidgetItem *___qtablewidgetitem24 = placeholderWidget->horizontalHeaderItem(7);
	___qtablewidgetitem24->setText(tr("7"));
	QTableWidgetItem *___qtablewidgetitem25 = placeholderWidget->horizontalHeaderItem(8);
	___qtablewidgetitem25->setText(tr("9"));
	QTableWidgetItem *___qtablewidgetitem26 = placeholderWidget->verticalHeaderItem(0);
	___qtablewidgetitem26->setText(tr("新建行"));
	QTableWidgetItem *___qtablewidgetitem27 = placeholderWidget->verticalHeaderItem(1);
	___qtablewidgetitem27->setText(tr("1"));
	QTableWidgetItem *___qtablewidgetitem28 = placeholderWidget->verticalHeaderItem(2);
	___qtablewidgetitem28->setText(tr("2"));
	QTableWidgetItem *___qtablewidgetitem29 = placeholderWidget->verticalHeaderItem(3);
	___qtablewidgetitem29->setText(tr("3"));
	QTableWidgetItem *___qtablewidgetitem30 = placeholderWidget->verticalHeaderItem(4);
	___qtablewidgetitem30->setText(tr("4"));
	QTableWidgetItem *___qtablewidgetitem31 = placeholderWidget->verticalHeaderItem(5);
	___qtablewidgetitem31->setText(tr("5"));
	QTableWidgetItem *___qtablewidgetitem32 = placeholderWidget->verticalHeaderItem(6);
	___qtablewidgetitem32->setText(tr("6"));
	QTableWidgetItem *___qtablewidgetitem33 = placeholderWidget->verticalHeaderItem(7);
	___qtablewidgetitem33->setText(tr("8"));

	btnsContainerWidget->setStyleSheet(".QWidget{border-color:gray;border-style:solid; border-width:1px;border-bottom-color:white; background-color:white;};");
	settingSplitter->setHandleWidth(0);
}

void MubanEditManagement::reloadData()
{
	placeholderWidget->setVisible(false);
	

	if (templateEditor == NULL)
	{
		templateEditor = new MyAppEntrance("", AuthorityConstants::FileType::MEDICAL_RECORDS, AuthorityConstants::Authority::WRITE_ALL);
		//QByteArray emptyArry;
		//emptyArry.resize(0);
		templateEditor->init(NULL,NULL);
		mainSplitter->addWidget(templateEditor);
	}

	if (simpleElementSettingWidget == NULL)
	{
		simpleElementSettingWidget = new SimpleElementSettingWidget();
		simpleElementSettingWidget->setVisible(false);
		simpleElementSettingWidget->setMinimumWidth(templateEditor->width());
		mainSplitter->addWidget(simpleElementSettingWidget);
	}
}

void MubanEditManagement::setVisibleView(QWidget * param)
{
	simpleElementSettingWidget->setVisible((simpleElementSettingWidget == param) ? true : false);
	templateEditor->setVisible((templateEditor == param) ? true : false);
	settingSplitter->setVisible(templateEditor->isVisible());
}

void MubanEditManagement::treeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
	if (item == complexElement)
	{
		setVisibleView(templateEditor);
	}
	else if(simpleElement == item)
	{
		setVisibleView(simpleElementSettingWidget);
	}
}

void MubanEditManagement::createActions()
{
	CREATEACTION(addNodeBefore, "在前增加节点", "在节点前面增加", triggered(bool), addNodeBeforeTriggered())
	CREATEACTION(addChildNode, "增加子节点", "新增一个子节点", triggered(bool), addChildNodeTriggered())
	CREATEACTION(addNodeAfter, "在后增加节点", "在节点后面增加", triggered(bool), addNodeAfterTriggered())
	CREATEACTION(moveUp, "节点上移", "将节点上移一个位置", triggered(bool), moveUpTriggered())
	CREATEACTION(moveDown, "节点下移", "在节点前面增加", triggered(bool), moveDownTriggered())
	CREATEACTION(deleteNode, "删除节点", "删除当前节点", triggered(bool), deleteNodeTriggered())
	CREATEACTION(rename, "重命名", "重命名当前节点", triggered(bool), renameTriggered())
}

void MubanEditManagement::addTreeNode(QTreeWidgetItem* item, int state, QString str)
{
	if (item != NULL/*&&item->parent() == NULL*/)
	{
		QStringList userModuleName;
		QString itemName = "";
		if (str.isEmpty() && str == "")
		{
			itemName = getItemName(item);
		}
		else
		{
			itemName = str;
		}
		userModuleName.append(itemName);

		QTreeWidgetItem *item1 = new QTreeWidgetItem(userModuleName);

		//前增后增设置
		switch (state)
		{
			case 0:
			{
				item->addChild(item1);
			}
				break;
			case 1:
			{
					  QTreeWidgetItem * currentNode = treeWidget->currentItem();
				item->insertChild(item->indexOfChild(currentNode), item1);
			}
				break;
			case 2:
			{
					  QTreeWidgetItem * currentNode = treeWidget->currentItem();
					  item->insertChild(item->indexOfChild(currentNode) + 1, item1);
			}
				break;
			default:
				break;
		}

		item1->setFlags(item->flags() | Qt::ItemIsEditable);
		item1->setSelected(true);
		item->setSelected(false);
	}
}

QString MubanEditManagement::getItemName(QTreeWidgetItem *item)
{
	int childCount = item->childCount();
	QString itemName = item->text(0);//获取节点的名字
	itemName = itemName + "_" + QString::number(childCount + 1);
	return itemName;
}

void MubanEditManagement::rightClickTemplate(const QPoint& pos)
{
	QTreeWidgetItem* curItem = treeWidget->itemAt(pos);  //获取当前被点击的节点
	if (curItem == NULL)return;           //这种情况是右键的位置不在treeItem的范围内，即在空白位置右击
	QVariant var = curItem->data(0, Qt::UserRole);
	QMenu *popMenu = new QMenu(this);//定义一个右键弹出菜单
	popMenu->addAction(addChildNode);//往菜单内添加QAction   该action在前面用设计器定义了
	popMenu->addAction(addNodeAfter);
	popMenu->addAction(addNodeBefore);
	popMenu->addAction(moveUp);
	popMenu->addAction(moveDown);
	popMenu->addAction(deleteNode);
	popMenu->addAction(rename);

	//根据不同情况进行变灰显示
	if (curItem->parent() == NULL)
	{
		addNodeBefore->setEnabled(false);
		addNodeAfter->setEnabled(false);
		moveUp->setEnabled(false);
		moveDown->setEnabled(false);
		deleteNode->setEnabled(false);
		rename->setEnabled(false);
	}
	else if (curItem->parent()->childCount() == 1)
	{
		addChildNode->setEnabled(true);
		addNodeBefore->setEnabled(true);
		addNodeAfter->setEnabled(true);
		moveUp->setEnabled(false);
		moveDown->setEnabled(false);
		deleteNode->setEnabled(true);
		rename->setEnabled(true);
	}
	else
	{
		addChildNode->setEnabled(true);
		addNodeBefore->setEnabled(true);
		addNodeAfter->setEnabled(true);
		moveUp->setEnabled(true);
		moveDown->setEnabled(true);
		deleteNode->setEnabled(true);
		rename->setEnabled(true);

		QTreeWidgetItem * currentItem = treeWidget->currentItem();
		int index = currentItem->parent()->indexOfChild(currentItem);
		moveUp->setEnabled((index == 0) ? false : true);
		moveDown->setEnabled((index == currentItem->parent()->childCount() - 1) ? false : true);
	}

	popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
}

void MubanEditManagement::setSelectedItemEditable()
{
	QList<QTreeWidgetItem *> list = treeWidget->selectedItems();
	treeWidget->editItem(list[list.size() - 1]);
}

void MubanEditManagement::setNodeWithItem(QTreeWidgetItem * item)
{
	if (item->parent() == NULL)
	{
		treeWidget->expandItem(item);
	}
	else
	{
		item->setExpanded(true);
	}

	setSelectedItemEditable();
}

void MubanEditManagement::addChildNodeTriggered()
{
	QTreeWidgetItem * item = treeWidget->currentItem();
	addTreeNode(item);

	setNodeWithItem(item);
}

void MubanEditManagement::addNodeAfterTriggered()
{
	QTreeWidgetItem * item = treeWidget->currentItem()->parent();
	addTreeNode(item, 2);

	setNodeWithItem(item);
}

void MubanEditManagement::addNodeBeforeTriggered()
{
	QTreeWidgetItem * item = treeWidget->currentItem()->parent();
	addTreeNode(item, 1);

	setNodeWithItem(item);
}

void MubanEditManagement::moveUpTriggered()
{
	//克隆之后变换位置
	QTreeWidgetItem * currentItem = treeWidget->currentItem();
	QTreeWidgetItem * parent = currentItem->parent();
	QTreeWidgetItem * newItem = currentItem->clone();
	int index = parent->indexOfChild(currentItem);
	parent->removeChild(currentItem);

	parent->insertChild( index - 1, newItem);
	setAllItemSelected(false);
	newItem->setSelected(true);
}

void MubanEditManagement::moveDownTriggered()
{
	//克隆之后变换位置
	QTreeWidgetItem * currentItem = treeWidget->currentItem();
	QTreeWidgetItem * parent = currentItem->parent();
	QTreeWidgetItem * newItem = currentItem->clone();
	int index = parent->indexOfChild(currentItem);
	parent->removeChild(currentItem);

	parent->insertChild(index + 1, newItem);
	setAllItemSelected(false);
	newItem->setSelected(true);
}

void MubanEditManagement::deleteNodeTriggered()
{
	QTreeWidgetItem * item = treeWidget->currentItem();
	if (item->parent() == NULL) return;
	delete item;
}

void MubanEditManagement::renameTriggered()
{
	QTreeWidgetItem * item = treeWidget->currentItem();
	if (item->parent() == NULL) return;
	treeWidget->editItem(item);
}

void MubanEditManagement::setAllItemSelected(bool state)
{
	QList<QTreeWidgetItem * > list = treeWidget->selectedItems();
	for (int i = 0; i < list.size(); i++)
	{
		list[i]->setSelected(state);
	}
}
