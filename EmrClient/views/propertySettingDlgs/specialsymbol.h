#ifndef SPECIALSYMBOL_H
#define SPECIALSYMBOL_H

#include <QWidget>
#include "ui_specialsymbol.h"
#include "customDelegate/MSC_VER.h"
#include <QDialog>
class QAbstractItemModel;
class QItemSelectionModel;
class QTableWidget;

class SpecialSymbol : public QDialog
{
	Q_OBJECT

public:
	SpecialSymbol(QDialog *parent = 0);
	~SpecialSymbol();

	void setupModel();
	void initData(QTableView * table, QItemSelectionModel * selectionModel, QStringList& data);

private slots:
void on_btn_Ok_clicked();
void on_btn_Cancel_clicked();
void sshow(QModelIndex index);
	

private:
	Ui::SpecialSymbol ui;

	QItemSelectionModel * medicalSelectionModel;
	QItemSelectionModel * commonSelectionModel;
	QItemSelectionModel * numberSelectionModel;
	QItemSelectionModel * mathSelectionModel;
	QItemSelectionModel * specialSelectionModel;
public :
	QString resultSymbol;
};

#endif // SPECIALSYMBOL_H
