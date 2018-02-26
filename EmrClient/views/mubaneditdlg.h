#ifndef MUBANEDITDLG_H
#define MUBANEDITDLG_H
 
#include <QDialog>

#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif

class QPushButton;
class QTextEdit;
class QLabel;
class QTableView;
class QListView;
class QLineEdit;
class DataBaseControl;
class QSqlTableModel;
class QSqlQueryModel;
class QAction;
class QRadioButton;
class QHBoxLayout;
class QStringListModel;

#include"qmap.h"
#include "addmubandlg.h"
#include"qtablewidget.h"
#include"dataModel/DBDataModel/DBManager.h"
class MuBanEditDlg : public QDialog
{
    Q_OBJECT
public:
    explicit MuBanEditDlg(QWidget *parent = 0);
	inline	QMap<QString, QString> getData() const
	{
		return map;
	}
    virtual void closeEvent(QCloseEvent *e);
	void setItemEdit(bool isFlags=false);
	void setStartUpRight();
	void destory();
	bool getEditRight(int type);
	QString mappingType(QString type);
	~MuBanEditDlg();
private:
    QPushButton * addBtn;
    QPushButton * modifyBtn;
    QPushButton * deleteBtn;
    QPushButton * quitBtn;
    QPushButton * searchBtn;
	QPushButton * elementEditBtn;
	QPushButton *startBtn;
    QLabel * listLabel;
    QLabel * nameLabel;
    QLabel * mubanLabel;

	QLabel *typeCodeLabel;
	QLabel *typeNameLabel;
	QLabel *dapartmentLabel;
	QLabel *blockNameLabel;
	QLabel *writeBlockLabel;
	QLabel *timeBlockLabel;



    QListView *listView;
    QLineEdit *lineEdit;
	QLineEdit *typeCodeEdit;
	QLineEdit *typeNameEdit;
	QLineEdit *dapartmentEdit;
	QLineEdit *blockNameEdit;
	QLineEdit *writeBlockEdit;
	QLineEdit *timeBlockEdit;

    QTableView *tableView;
	QTableWidget *tableWidget;

	bool isEdit;//超级用户
	bool isEditRight;//个人，看是否配置了权限
	bool isAddTemplate;//是否有新增模板的权限
	bool isAllTempAdd;
	QAction * changeAction;
signals:
	
private slots:
    void onPushBtnClicked();
//doubleClicked
void Clicked(const QModelIndex &index);
void  rightClick(const QPoint& ps);
void onItemRightClicked();
void onLineEditClicked();
void tabWidgetDoubleClick(int row, int column);
void onRadioBtnToggled();

private:
	QRadioButton* getCheckedRadioButton();
    void createWidgets();
	bool modifyData();
	bool isSave();
    void loadListData();
    void reloadTableData();
	void InitAddTemplate(AddMuBanDlg* dlg);
	inline void setItemState(QTableWidgetItem *item, int row, int column);
	QMap<QString, QString> map;
	QMap<QString, QString> sqlMap;
	DBManager *m_DbManager;
	DataBaseControl *m_DataBaseControl;
	QSqlQueryModel* model;
	QSqlTableModel *templateModel;
protected:
	bool eventFilter(QObject *obj, QEvent *event);

public:
	QWidget		  * radioContainerWidget;
	QHBoxLayout  * radioHBoxLayout;
	QRadioButton * allBtn;
	QRadioButton * publicBtn;
	QRadioButton * departmentBtn;
	QRadioButton * privateBtn;
	QStringListModel * strModel;
};

#endif // MUBANEDITDLG_H
