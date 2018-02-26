#ifndef PARAGRAPHSETTINGPOPUP_H
#define PARAGRAPHSETTINGPOPUP_H

#include <QDialog>
#include "customDelegate/MSC_VER.h"
#include "EMR/obj/temp/data/ParagraphData.h"
class  QLabel;
class  QLineEdit;
class  QComboBox;
class  QPushButton;
class ParagraphSettingPopup : public QDialog
{
    Q_OBJECT
public:
    explicit ParagraphSettingPopup(QDialog *parent = 0);
    ~ParagraphSettingPopup();
    void createUI();
    void initUI();

	void connectSlots();
	void resetMap();
	QMap<QString, QString> getData()const;
	QMap<QString, QString> *m_map;
	void InitData(QMap<QString, QString> &map);
	void setParagraphData(ParagraphData *pPara);
CREATE_UI_BEGIN
public:
    QComboBox *comboBox;
    QLabel *specialUnitLabel;
    QLabel *specialPatternLabel;
    QLabel *rightiIndentUnitLabel;
    QLabel *leftiIndentUnitLabel;
    QLabel *rightIndentLabel;
    QLabel *leftIndentLabel;
    QLineEdit *specialPatternEdit;
    QLineEdit *rightIndentEdit;
    QLineEdit *leftIndentEdit;
    QPushButton *cancelBtn;
    QPushButton *confirmBtn;
CREATE_UI_END
private slots:
	void onConfirmBtnClicked();
	void onCancelBtnClicked();
	void onChanged(int index);
private:
	ParagraphData *m_paragraphData;
public:
	int disL;
	int disR;
	char type;
	int indentLeft;

};

#endif // PARAGRAPHSETTINGPOPUP_H