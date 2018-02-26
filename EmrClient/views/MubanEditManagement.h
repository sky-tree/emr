#ifndef MUBANEDITMANAGEMENT_H
#define MUBANEDITMANAGEMENT_H

#include <QDialog>
#include "customDelegate/MSC_VER.h"

class  QHBoxLayout;
class  QSplitter;
class  QTreeWidget;
class  QWidget;
class  QPushButton;
class  QTableWidget;
class QTreeWidgetItem;
class SimpleElementSettingWidget;
class MyAppEntrance;
class QAction;

#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif

enum NodeLevel
{
	LevelOne = 1,
	LevelTwo,
	LevelThree
};

enum NodeType
{
	ComplexElement,
	SimpleElement,
	LittleTemplate,
	DocOutline,
	HeaderAndFooter,
	MedicalFormula,
	MedicalDictionary,
	SpecialSymbols,
	Icd10Code
};

class MubanEditManagement : public QDialog
{
    Q_OBJECT
public:
    explicit MubanEditManagement(QDialog *parent = 0);
    ~MubanEditManagement();

    void createUI();
    void initUI();
	void createRootNodes();

	void reloadData();
	void setVisibleView(QWidget * param);
	void createActions();
	void addTreeNode(QTreeWidgetItem* item, int state = 0, QString str = "");// state 0 增加子节点 1 在前增加 2 在后增加
	QString getItemName(QTreeWidgetItem *item);
	void setSelectedItemEditable();
	void setNodeWithItem(QTreeWidgetItem * item);
	void setAllItemSelected(bool state = false);

CREATE_UI_BEGIN
public:
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *btnContainerHLayout;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *editBtn;
    QPushButton *deleteBtn;
    QPushButton *addBtn;
    QSplitter *settingSplitter;
    QSplitter *leftSplitter;
    QSplitter *mainSplitter;
    QTableWidget *placeholderWidget;
    QTableWidget *editTableWidget;
    QTreeWidget *treeWidget;
    QWidget *btnsContainerWidget;
	CREATE_UI_END
public:
	QTreeWidgetItem * complexElement;
	QTreeWidgetItem * simpleElement;
	QTreeWidgetItem * littleTemplate;
	QTreeWidgetItem * docOutline;
	QTreeWidgetItem * headerAndFooter;
	QTreeWidgetItem * medicalFormula;
	QTreeWidgetItem * medicalDictionary;
	QTreeWidgetItem * specialSymbols;
	QTreeWidgetItem * icd10Code;

	SimpleElementSettingWidget * simpleElementSettingWidget;
	MyAppEntrance * templateEditor;

	QAction * addNodeBefore;
	QAction * addChildNode;
	QAction * addNodeAfter;
	QAction * moveUp;
	QAction * moveDown;
	QAction * deleteNode;
	QAction * rename;
private slots:
	void treeItemDoubleClicked(QTreeWidgetItem *item, int column);
	void addRow();
	void delRow();

	void rightClickTemplate(const QPoint& pos);
	void addNodeBeforeTriggered();
	void addChildNodeTriggered();
	void addNodeAfterTriggered();
	void moveUpTriggered();
	void moveDownTriggered();
	void deleteNodeTriggered();
	void renameTriggered();
};

#endif // MUBANEDITMANAGEMENT_H