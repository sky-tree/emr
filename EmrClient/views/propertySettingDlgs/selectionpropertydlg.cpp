#include "selectionpropertydlg.h"
#include "customDelegate/custompushbutton.h"

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include"qheaderview.h"
#include "MessageBox.h"
#include <qdebug.h>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
//#include"qstring.h"
/*******/
QWidget* comboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QComboBox *editor = new QComboBox(parent);
	editor->addItem("患者基本信息");
	editor->addItem("单选");
	editor->addItem("多选");
	editor->addItem("有无选择");
	return  editor;
}
void comboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString text = index.model()->data(index, Qt::EditRole).toString();
	QComboBox *comboBox = static_cast<QComboBox *>(editor);
	if (text.isEmpty() == false && text.isNull() == false)
	{
		int tindex = comboBox->findText(text);
		comboBox->setCurrentIndex(tindex);
	}	
}
void comboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QComboBox *combox = static_cast<QComboBox *>(editor);
	QString text = combox->currentText();
	model->setData(index, text, Qt::EditRole);
}
void comboxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

/*********/
SelectionPropertyDlg::SelectionPropertyDlg(QWidget *parent) :
QDialog(parent), codeEdit(NULL), nameEdit(NULL), codeSysComboBox(NULL), visualComboBox(NULL)
, codeSysLabel(NULL), codeLabel(NULL), nameLabel(NULL), selectItemLabel(NULL),selectItemRadioButton(NULL), tableWidget(NULL), confirmBtn(NULL), cancelBtn(NULL), addBtn(NULL), subBtn(NULL)
, forkBtn(NULL), flags(0), m_map(NULL), mPaddingLeftLabel(NULL), mPaddingLeftEdit(NULL), mHLayout1(NULL)
{
	selectedComboBox = NULL;
	notSelectedComboBox = NULL;
	notSelectedPrefix = NULL;
	selectedPrefix = NULL;

	
}

SelectionPropertyDlg::~SelectionPropertyDlg()
{
}
void SelectionPropertyDlg::connectSlots()
{
	connect(confirmBtn, SIGNAL(clicked(bool)), this, SLOT(onConfirmBtnClicked()));
	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtnClicked()));
	connect(addBtn, SIGNAL(clicked(bool)), this, SLOT(addRow()));
	connect(subBtn, SIGNAL(clicked(bool)), this, SLOT(delRow()));
	connect(forkBtn, SIGNAL(clicked(bool)), this, SLOT(delAllRow()));
	if (selectItemRadioButton)
	{
		connect(selectItemRadioButton, SIGNAL(toggled(bool)), this, SLOT(setTableWidgetEnable()));
	}


}
void SelectionPropertyDlg::setTableWidgetEnable()
{
	if (selectItemRadioButton&&selectItemRadioButton->isChecked() == true)
	{
// 		addBtn->setEnabled(false);
// 		subBtn->setEnabled(false);
// 		forkBtn->setEnabled(false);
// 		tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	}
	else
	{
// 		addBtn->setEnabled(true);
// 		subBtn->setEnabled(true);
// 		forkBtn->setEnabled(true);
// 		tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
	}
}
void SelectionPropertyDlg::addRow()
{
	int totalRow = tableWidget->rowCount();
	tableWidget->setRowCount(totalRow + 1);
	tableWidget->setCurrentCell(totalRow + 1, QItemSelectionModel::Select);
	tableWidget->setFocus(Qt::MouseFocusReason);
}
void SelectionPropertyDlg::delRow()
{
	int totalRow = tableWidget->rowCount();
	if (totalRow > 0)
	{
		tableWidget->removeRow(tableWidget->currentRow());
	}
}
void SelectionPropertyDlg::delAllRow()
{
	if (tableWidget->rowCount() == 0)return;
	
	if (MessageBox::showQueryMessageBox(QStringLiteral("删除"), QStringLiteral("确定全部删除?")) == QMessageBox::Yes)
	{
		while (tableWidget->rowCount() > 0)
		{
			tableWidget->removeRow(0);
		}
	}
}
void SelectionPropertyDlg::onConfirmBtnClicked()
{
	resetMap();
	//emit QDialog::accept();
	this->close();
}
void SelectionPropertyDlg::va_listRemove(QMap<QString, QString> *qMap, int argc, ...)
{
	va_list va;
	va_start(va, argc);
	for (int i = 0; i != argc; ++i)
	{
		const char * str = va_arg(va, const char*);
		QString str1(str);
		if (qMap->contains(str1))
		{
			qMap->remove(str1);
		}
	}
	va_end(va);
}
void SelectionPropertyDlg::resetMap()
{
	if (m_map == NULL)
	{ 
		m_map = new QMap<QString, QString>(); 
	}
	//m_map->clear();
	int type = -1;
	switch (flags)
	{
	case  selectionPropertyDlg:
		type = flags;
		break;

	case  selectionPropertyDlg_multity:
		type = flags;
		break;
	case  selectionPropertyDlg_bool:
		type = flags;
		break;
	case unfoldRedioDlg:
		type = flags;
		if (mPaddingLeftEdit)
		{
			m_map->insert("paddingleft", mPaddingLeftEdit->text());
		}
		break;
	case unfoldMultpleChoiceDlg:
		type = flags;
		if (mPaddingLeftEdit)
		{
			m_map->insert("paddingleft", mPaddingLeftEdit->text());
		}
		break;
	default:
		break;
	}
	if (type == -1)
	{
	//	m_map->clear();
		return;
	}
	va_listRemove(m_map, 9, "dlgType", "name", "visual", "codeSys", "codeName", "isEdit", "options","selectValue","notSelectValue");
	m_map->insert("dlgType", QString::number(type));
	m_map->insert("name", nameEdit->text());
	m_map->insert("visual", QString::number(visualComboBox->currentIndex()));
	m_map->insert("codeSys", QString::number(codeSysComboBox->currentIndex()));
	m_map->insert("codeName", codeEdit->text());
	if (selectedComboBox)
	{
		m_map->insert("selectValue", selectedComboBox->currentText());
	}
	if (notSelectedComboBox)
	{
		m_map->insert("notSelectValue", notSelectedComboBox->currentText());
	}

	if (selectItemRadioButton)
	{
		if (selectItemRadioButton->isChecked() == false)
		{
			m_map->insert("isEdit", "false");
		}
		else
		{
			m_map->insert("isEdit", "true");
		}
	}
	QString data = "";
	data.clear();
	QString qData = "";
	qData.clear();
	int count = tableWidget->rowCount();
	QTableWidgetItem* item = NULL;
	for (int i = 0; i < count; i++)
	{
		item=tableWidget->item(i, 0);
		if (item == NULL)continue;;
		qData = item->text();
		if (deBlank(qData))
		{
			data += qData + "[]";
		}
	}
	m_map->insert("options", data);
}
void SelectionPropertyDlg::onCancelBtnClicked()
{  
	this->close();
}
void SelectionPropertyDlg::createUI()
{
	this->setFixedSize(480, 500);
	this->setInputMethodHints(Qt::ImhNone);
	this->setStyleSheet(kPushBtnStyle);

	nameLabel = new QLabel(this);
	nameEdit = new QLineEdit(this);

	codeSysLabel = new QLabel(this);
	codeSysComboBox = new QComboBox(this);

	visualComboBox = new QComboBox(this);

	codeEdit = new QLineEdit(this);
	codeLabel = new QLabel(this);

	if (flags == selectionPropertyDlg_bool)
	{
		selectedPrefix = new QLabel(this);
		selectedComboBox = new QComboBox(this);
		selectedComboBox->setEditable(true);

		notSelectedPrefix = new QLabel(this);
		notSelectedComboBox = new QComboBox(this);
		notSelectedComboBox->setEditable(true);
	}
	

	QGridLayout* gLayout = new QGridLayout();
	gLayout->addWidget(nameLabel, 0, 0, 1, 1);
	gLayout->addWidget(nameEdit, 0, 1, 1, 1);
	gLayout->addWidget(codeSysLabel, 1, 0, 1, 1);
	gLayout->addWidget(codeSysComboBox, 1, 1, 1, 1);
	
	gLayout->addWidget(visualComboBox, 0, 4, 1, 1);
	gLayout->addWidget(codeLabel, 1, 3, 1, 1);
	gLayout->addWidget(codeEdit, 1, 4, 1, 1);
	
	if (flags == selectionPropertyDlg_bool)
	{
		gLayout->addWidget(selectedPrefix, 2, 0, 1, 1);
		gLayout->addWidget(selectedComboBox, 2, 1, 1, 1);
		gLayout->addWidget(notSelectedPrefix, 2, 3, 1, 1);
		gLayout->addWidget(notSelectedComboBox, 2, 4, 1, 1);
	}
	
	mHLayout1 = new QHBoxLayout();
	

	confirmBtn = new QPushButton(this);
	confirmBtn->setFixedSize(81, 23);
	cancelBtn = new QPushButton(this);
	 cancelBtn->setFixedSize(81, 23);

	QHBoxLayout* hLayout3 = new QHBoxLayout();
	hLayout3->addStretch();
	hLayout3->addWidget(confirmBtn); 
	hLayout3->addWidget(cancelBtn);

	/*******************************************暂时没用*************************************/
//  	seriesNumLabel = new QLabel(this);
// 		seriesNumLabel->setGeometry(QRect(30, 180, 54, 12));
//  	listNameLabel = new QLabel(this);
//  	listNameLabel->setGeometry(QRect(110, 180, 54, 12));
// 		generateBtn = new QPushButton(this);
// 		generateBtn->setGeometry(QRect(160, 590, 75, 23));
// 		generateBtn->setVisible(false);
	/**************************************************************************************/

	tableWidget = new QTableWidget(this);
	tableWidget->setGeometry(QRect(30, 210, 421, 192));

	QString btnStyle = "QPushButton{text-align:center;background-color:rgb(100,140,250);\
					   border:1px solid white;border-adius:4px}QPushButton:hover{background-color:rgb\
					   (154, 205, 252);}QPushButton:pressed{background-color:rgb(141, 184, 6, 200);}";
	addBtn = new QPushButton("添加一项");
	addBtn->setStyleSheet(btnStyle);
	addBtn->setToolTip("添加一项");
	subBtn = new QPushButton("删除一项");
	subBtn->setStyleSheet(btnStyle);
	subBtn->setToolTip("删除一项");
	forkBtn = new QPushButton("全部删除");
	forkBtn->setStyleSheet(btnStyle);
	forkBtn->setToolTip("全部删除");

	QHBoxLayout* hLayout2 = new QHBoxLayout();
	hLayout2->addStretch();
	hLayout2->addWidget(addBtn);
	hLayout2->addWidget(subBtn);
	hLayout2->addWidget(forkBtn);

	QVBoxLayout* vLayout = new QVBoxLayout(this);
	vLayout->addLayout(gLayout);
	vLayout->addLayout(mHLayout1);
	vLayout->addLayout(hLayout2);
	vLayout->addWidget(tableWidget);
	vLayout->addLayout(hLayout3);
	this->setLayout(vLayout);
}
//void SelectionPropertyDlg::setFlags(int type)
//{
//	flags = type;
//	switch (type)
//	{
//	case 1:
//		this->setWindowTitle(tr("单选属性设置")); break;
//	case 2:
//		this->setWindowTitle(tr("多选属性设置")); break;
//	case 3:
//		this->setWindowTitle(tr("有无选择属性设置")); break;
//	default:
//		break;
//	}
//}
void SelectionPropertyDlg::initUI()
{
	this->setWindowTitle(tr("属性设置"));
	codeLabel->setText(tr("       编  码"));
	codeLabel->setAlignment(Qt::AlignCenter);
	nameLabel->setText(tr("名    称"));
	nameLabel->setAlignment(Qt::AlignLeft);
	codeSysComboBox->clear();
	codeSysComboBox->insertItems(0, QStringList()
		<< "UTF-8"
		<< "Unicode"
		<< "Unicode big endian"
		<< "ANSI"
		);
	codeSysLabel->setText(tr("编码系统"));
	codeSysLabel->setAlignment(Qt::AlignLeft);
	visualComboBox->clear();
	visualComboBox->insertItems(0, QStringList()
		<< tr("隐藏")
		<< tr("居左")
		<< tr("居右")
		);
	visualComboBox->setCurrentIndex(0);
	
// 	seriesNumLabel->setText(tr("序    号"));
// 	listNameLabel->setText(tr("名    称"));
// 	seriesNumLabel->setVisible(false);
// 	listNameLabel->setVisible(false);

// 	addBtnLoc->setText("+");
// 	subBtnLoc->setText("_");
//	forkBtnLoc->setText("×");
//	contentEdit->setPlaceholderText(tr("注意每个内容之间必须用顿号分开确定后直接点击编译内容自动生成"));//zl2017.8.1
//	generateBtn->setText(tr("编译"));

	confirmBtn->setText(tr("确定"));
	cancelBtn->setText(tr("取消"));

	if (flags == selectionPropertyDlg_bool)
	{
		selectedPrefix->setText(tr("选中项目前缀"));
		selectedComboBox->clear();
		selectedComboBox->insertItems(0, QStringList()
			<< tr("有")
			<< tr("伴")
			<< tr("伴有")
			);
		notSelectedComboBox->clear();
		notSelectedComboBox->insertItems(0, QStringList()
			<< tr("无")
			);
		notSelectedPrefix->setText(tr("未选中项目前缀"));
	}
	

	tableWidget->setColumnCount(1);
	//comboxDelegate *testComboxDelegate=new comboxDelegate();
	//tableWidget->setItemDelegateForColumn(1, testComboxDelegate);
	QStringList headers;
	headers << "名称";
	tableWidget->setHorizontalHeaderLabels(headers);
	tableWidget->horizontalHeader()->setEnabled(false);
	//使行列头自适应宽度，最后一列将会填充空白部分  
	tableWidget->horizontalHeader()->setStretchLastSection(true);
	//使行列头自适应宽度，所有列平均分来填充空白部分              
	//tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}
void SelectionPropertyDlg::setComboxVisable(bool isVisable)
{	
	if (isVisable)
	{
		if (!selectItemLabel)
		{
			selectItemLabel = new QLabel(this);
			selectItemLabel->setText(tr("选择条目"));
		}
		if (!selectItemRadioButton)
		{
			selectItemRadioButton = new QRadioButton(this);
			selectItemRadioButton->setText(tr("该项目只能选择操作"));
			if (m_map->contains("isEdit") && m_map->value("isEdit") == "true")
			{
				if (selectItemRadioButton)
				{
					selectItemRadioButton->setChecked(true);
				}
			}
		}
		int index = mHLayout1->indexOf(selectItemLabel);
		if (index<0)
		{
			mHLayout1->addWidget(selectItemLabel);
		}
		index = mHLayout1->indexOf(selectItemRadioButton);
		if (index < 0)
		{
			mHLayout1->addWidget(selectItemRadioButton);
			mHLayout1->addStretch();
		}
	}
	else
	{
		int index = -1;
		if (selectItemLabel)
		{
			 index = mHLayout1->indexOf(selectItemLabel);
			if (index >= 0)
			{
				mHLayout1->removeWidget(selectItemLabel);
			}
		}
		if (selectItemLabel)
		{
			index = mHLayout1->indexOf(selectItemRadioButton);
			if (index >= 0)
			{
				mHLayout1->removeWidget(selectItemRadioButton);
			}
		}
		
	}
	//selectedComboBox->setVisible(isVisable);
	//notSelectedComboBox->setVisible(isVisable);
	//selectedPrefix->setVisible(isVisable);
	//notSelectedPrefix->setVisible(isVisable);

}
void SelectionPropertyDlg::setPaddingLeftVisable(bool isVisable)
{
	if (isVisable)
	{
		if (!mPaddingLeftLabel)
		{
			mPaddingLeftLabel = new QLabel(this);
			mPaddingLeftLabel->setText(tr("左边距"));
		
			
		}
		if (!mPaddingLeftEdit)
		{
			mPaddingLeftEdit = new QLineEdit(this);
			mPaddingLeftEdit->setValidator(new QIntValidator(0, 40, mPaddingLeftEdit));
			if (m_map->contains("paddingleft"))
			{
				if (mPaddingLeftEdit)
				{
					mPaddingLeftEdit->setText(m_map->value("paddingleft"));
				}
			}
		}
		int index = mHLayout1->indexOf(mPaddingLeftLabel);
		if (index<0)
		{
			mHLayout1->addWidget(mPaddingLeftLabel);
		}
		index = mHLayout1->indexOf(mPaddingLeftEdit);
		if (index < 0)
		{
			mHLayout1->addWidget(mPaddingLeftEdit);
			mHLayout1->addStretch();
		}
	}
	else
	{
		int index = -1;
		if (mPaddingLeftLabel)
		{
			index = mHLayout1->indexOf(mPaddingLeftLabel);
			if (index >= 0)
			{
				mHLayout1->removeWidget(mPaddingLeftLabel);
			}
		}
		if (mPaddingLeftLabel)
		{
			index = mHLayout1->indexOf(mPaddingLeftEdit);
			if (index >= 0)
			{
				mHLayout1->removeWidget(mPaddingLeftEdit);
			}
		}
	}
}
void SelectionPropertyDlg::InitData(QMap<QString, QString> &map)
{
	m_map = &map;
	if (map.contains("dlgType") == false)return;
	int type = 0;
	type = map["dlgType"].toInt();
	flags = type;
	createUI();
	initUI();
	connectSlots();
	this->setModal(true);
	
	switch (type)
	{
	case selectionPropertyDlg:
		this->setWindowTitle(tr("单选属性设置")); 
		setComboxVisable(true);
		setPaddingLeftVisable(false);
		break;
	case selectionPropertyDlg_multity:
		this->setWindowTitle(tr("多选属性设置")); 
		setComboxVisable(true);
		setPaddingLeftVisable(false);
		break;
	case selectionPropertyDlg_bool:
		this->setWindowTitle(tr("有无选择属性设置"));
		setComboxVisable(true);
		setPaddingLeftVisable(false);
		break;
	case unfoldRedioDlg:
		this->setWindowTitle(tr("单选属性设置"));
		setComboxVisable(false);
		setPaddingLeftVisable(true);
		break;
	case unfoldMultpleChoiceDlg:
		this->setWindowTitle(tr("多选属性设置"));
		setComboxVisable(false);
		setPaddingLeftVisable(true);
		break;
	default:
		break;
	}
	if (map.contains("name")){ nameEdit->setText(map["name"]); }
	if (map.contains("visual")){ visualComboBox->setCurrentIndex(map["visual"].toInt()); }
	if (map.contains("codeSys")){ codeSysComboBox->setCurrentIndex(map["codeSys"].toInt()); }
	if (map.contains("codeName")){ codeEdit->setText(map["codeName"]); }
	
	if (selectedComboBox)
	{
		if (map.contains("selectValue"))
		{
			QString str = map["selectValue"];
			if (str != "")
			{
				selectedComboBox->setCurrentText(str);
			}
		}
	}
	if (notSelectedComboBox)
	{
		if (map.contains("notSelectValue"))
		{
			QString str = map["notSelectValue"];
			if (str != "")
			{
				notSelectedComboBox->setCurrentText(str);
			}
		}
	}
	
	if (map.contains("options"))
	{		
		QString str = map["options"];
		QStringList strList = str.split("[]");
		int count = strList.count();
		if (count <=1)return;
		tableWidget->setRowCount(count-1);
		for (int i = 0; i < count-1; i++)
		{
			QTableWidgetItem *item = new QTableWidgetItem();
			item->setText(strList.at(i));
			tableWidget->setItem(i, 0, item);
		}
	}
}
bool SelectionPropertyDlg::deBlank(QString strs)
{
	int len = strs.length();
	for (int i = 0; i<len; i++)
	{
		if (strs.at(i).isSpace())
		{
			strs[i] = QChar(' ');
		}
	}
	if (strs.length()>0)
	{
		return true;
	}
	return false;
}
