#include "specialsymbol.h"
#include "globalvariable.h"

#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QStandardItem>

#define  MAX_COLUMN 5

SpecialSymbol::SpecialSymbol(QDialog *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	medicalSelectionModel = NULL;
	commonSelectionModel = NULL;
	numberSelectionModel = NULL;
	mathSelectionModel = NULL;
	specialSelectionModel = NULL;
	resultSymbol.clear();
	setupModel();
	this->resize(568, 363);
	this->setMaximumSize(QSize(568, 363));
	connect(ui.tW_specialSymbol, SIGNAL(clicked(QModelIndex)), this, SLOT(sshow(QModelIndex)));
	connect(ui.tW_mathSymbol, SIGNAL(clicked(QModelIndex)), this, SLOT(sshow(QModelIndex)));
	connect(ui.tW_numberSymbol, SIGNAL(clicked(QModelIndex)), this, SLOT(sshow(QModelIndex)));
	connect(ui.tW_medicalSymbol, SIGNAL(clicked(QModelIndex)), this, SLOT(sshow(QModelIndex)));
	connect(ui.tW_commonSymbol, SIGNAL(clicked(QModelIndex)), this, SLOT(sshow(QModelIndex)));
	//connect(ui.btn_Ok, SIGNAL(clicked()), this, SLOT(on_btn_Ok_clicked()));

	ui.btn_Ok->setMinimumSize(ui.btn_Ok->size());
	ui.btn_Ok->setMaximumSize(ui.btn_Ok->size());
	ui.btn_Cancel->setMinimumSize(ui.btn_Ok->size());
	ui.btn_Cancel->setMaximumSize(ui.btn_Ok->size());
	this->setStyleSheet(kPushBtnStyle);
}

SpecialSymbol::~SpecialSymbol()
{

}

void SpecialSymbol::setupModel()
{
	QString medicalString = "g/L,U/L,mol/L,μmol/L,mmol/L,umol/L,IU/L,ng/dL,/μL,ng/ml,iu/ml,mmHg,mv,fl,pg,mm/1h,mIU/m,％ ,℃,﹩,‰,㎜,㎝,㎞,㎎,㎏";
	QString	commonString = "、,。,．,；,：,？,！,… ,‘ ’,“ ”,（ ）,《 》,｛ ｝,〖〗,【 】,～, –, —,﹏,〈    〉, 「  」, ﹁, ﹂,『  』,  ﹃ ,﹄";
	QString specialString = "@,#,&, ※ ,○,●,△ ,▲,◎,☆,★,◇,◆,□ ,■ ,▽,▼,㊣,＊,♀,♂,⊕,⊙,↑,↓,←,→,↖,↗,↙,↘,∥,/,\\,|";
	QString numberString = "Ⅰ Ⅱ Ⅲ Ⅳ Ⅴ Ⅵ Ⅶ Ⅷ Ⅸ Ⅹ Ⅺ Ⅻ ① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨ ⑩";
	QString mathString = "≈ ≠ ＝≤ ≥ ＜ ＞ ≮ ≯ ± ＋ － × ÷ ∫ ∮ ∝ ∞ ∧ ∨ ∑ ∪ ∩ ∈ ⊥ √ ≦ ≧ ∟ ㏒ ㏑* / ";
	
	initData(ui.tW_specialSymbol, specialSelectionModel, specialString.split(","));
	initData(ui.tW_commonSymbol, commonSelectionModel, commonString.split(","));
	initData(ui.tW_mathSymbol, mathSelectionModel, mathString.split(" "));
	initData(ui.tW_numberSymbol, numberSelectionModel, numberString.split(" "));
	initData(ui.tW_medicalSymbol, medicalSelectionModel, medicalString.split(","));
}

void SpecialSymbol::initData(QTableView * table, QItemSelectionModel * selectionModel, QStringList& data)
{
	int rowCount = ceil((double)data.length() / MAX_COLUMN);
	QStandardItemModel * model = new QStandardItemModel(rowCount, MAX_COLUMN);
	QStandardItem * parentItem = model->invisibleRootItem();

	for (int row = 0; row < rowCount; row++)
	{
		for (int col = 0; col < MAX_COLUMN; col++)
		{
			int index = row * MAX_COLUMN + col;
			if (index < data.length())
			{
				QStandardItem * item = new QStandardItem(data[index]);
				item->setTextAlignment(Qt::AlignCenter);
				item->setEditable(false);
				model->setItem(row, col, item);
			}
		}
	}
	
	selectionModel = new QItemSelectionModel(model);
	table->setModel(model);
	table->setSelectionModel(selectionModel);
	table->horizontalHeader()->setVisible(false);
	table->verticalHeader()->setVisible(false);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->setSelectionBehavior(QAbstractItemView::SelectItems);
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void SpecialSymbol::on_btn_Ok_clicked()
{
	QDialog::accept();
}
void SpecialSymbol::on_btn_Cancel_clicked()
{
	QDialog::reject();
}
void SpecialSymbol::sshow(QModelIndex index)
{
	resultSymbol = index.data().toString();
}