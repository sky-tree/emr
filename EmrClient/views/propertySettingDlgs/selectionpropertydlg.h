#ifndef SELECTIONPROPERTYDLG_H
#define SELECTIONPROPERTYDLG_H
 
#include <QWidget>
#include<QMap>
#include"qdialog.h"
#include"qtablewidget.h"
#include"views/globalvariable.h"
class  QLineEdit;
class QTextEdit;
class QLabel;
class QPushButton;
class QRadioButton;
class QComboBox;
class QTableView;
class CustomPushButton;
class QHBoxLayout;
#include"qmap.h"
/*************************************************/
#include"qitemdelegate.h"
class comboxDelegate :public QItemDelegate
{
	Q_OBJECT
public:
	comboxDelegate(QObject *parent = 0) :QItemDelegate(parent)
	{
	}
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/******************************************/
class SelectionPropertyDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectionPropertyDlg(QWidget *parent = 0);
    ~SelectionPropertyDlg();

	void createUI();
	void initUI();
	void InitData(QMap<QString, QString> &map);
	//void setFlags(int type);
	//QMap<QString, QString> getData();
private:
	void setComboxVisable(bool isVisable);
	void setPaddingLeftVisable(bool isVisable);
	void connectSlots();
	void resetMap();
	void va_listRemove(QMap<QString, QString> *qMap, int argc, ...);
  bool deBlank(QString strs);
	private slots:
	void onConfirmBtnClicked();
	void onCancelBtnClicked();

	void addRow();
	void delRow();
	void delAllRow();
	void setTableWidgetEnable();
	
private:
	QLineEdit *codeEdit;
	QLineEdit *nameEdit;
//	QTextEdit *contentEdit;//zl2017.8.1

	QComboBox *codeSysComboBox;
	QComboBox *visualComboBox;
	QComboBox *selectedComboBox;
	QComboBox *notSelectedComboBox;
	
	QLabel *codeSysLabel;
	QLabel *codeLabel;
	QLabel *nameLabel;
	QHBoxLayout* mHLayout1;
	QLabel *selectItemLabel;
	QRadioButton *selectItemRadioButton;
// 	QLabel *seriesNumLabel;
// 	QLabel *listNameLabel;
// 	QLabel *addBtnLoc;
// 	QLabel *subBtnLoc;
// 	QLabel *forkBtnLoc;
	//勾选时
	QLabel *mPaddingLeftLabel;
	QLineEdit *mPaddingLeftEdit;

	QLabel *notSelectedPrefix;
	QLabel *selectedPrefix;

	QTableWidget *tableWidget;
	
//	QPushButton *generateBtn;
	QPushButton *confirmBtn;
	QPushButton *cancelBtn;
	QPushButton * addBtn;
	QPushButton * subBtn;
	QPushButton * forkBtn;

	int flags;
	QMap<QString, QString> *m_map;
};

#endif // SELECTIONPROPERTYDLG_H
