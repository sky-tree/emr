#ifndef SIMPLEELEMENTSETTINGWIDGET_H
#define SIMPLEELEMENTSETTINGWIDGET_H

#include <QWidget>
#include "customDelegate/MSC_VER.h" 

class  QVBoxLayout;
class  QWidget;
class  QHBoxLayout;
class  QPushButton;
class  QTableWidget;
class  QComboBox;
class QSignalMapper;
class SimpleElementSettingWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SimpleElementSettingWidget(QWidget *parent = 0);
	~SimpleElementSettingWidget();

	enum ProjectType
	{
		NONE = 0,
		SingleSelection,
		MultiSelection,
		IsHaveSelection,
		Number,
		Macro
	};
	Q_DECLARE_FLAGS(ProjectTypes, ProjectType)

	struct ElementData : public QObjectUserData
	{
		ProjectType type;
		QString comboBoxText;
	};

	void createUI();
	void initUI();
	QComboBox * createPropertyCBox();
	CREATE_UI_BEGIN
public:

	QSignalMapper* signalMapper;
	QHBoxLayout *btnsContainerHLayout;
	QHBoxLayout *horizontalLayout_2;
	QPushButton *saveBtn;
	QPushButton *deleteBtn;
	QPushButton *addBtn;
	QTableWidget *settingTableWidget;
	QVBoxLayout *contentVLayout;
	QVBoxLayout *verticalLayout_2;
	QWidget *btnsContainerWidget;
	CREATE_UI_END
	
private slots:
	void addRow();
	void delRow();

	void onQComboBoxClicked(int i);
	void onCellBtnClicked(); 
};

#endif // SIMPLEELEMENTSETTINGWIDGET_H