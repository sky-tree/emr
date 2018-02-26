/********************************************************************************
** Form generated from reading UI file 'continueprintdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTINUEPRINTDLG_H
#define UI_CONTINUEPRINTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ContinuePrintDlg
{
public:
    QLabel *startLabel;
    QLabel *endLabel;
    QLineEdit *startEdit;
    QLineEdit *endEdit;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn;
    QLabel *tipLabel;

    void setupUi(QDialog *ContinuePrintDlg)
    {
        if (ContinuePrintDlg->objectName().isEmpty())
            ContinuePrintDlg->setObjectName(QStringLiteral("ContinuePrintDlg"));
        ContinuePrintDlg->resize(241, 187);
        startLabel = new QLabel(ContinuePrintDlg);
        startLabel->setObjectName(QStringLiteral("startLabel"));
        startLabel->setGeometry(QRect(30, 20, 54, 16));
        endLabel = new QLabel(ContinuePrintDlg);
        endLabel->setObjectName(QStringLiteral("endLabel"));
        endLabel->setGeometry(QRect(30, 60, 54, 16));
        startEdit = new QLineEdit(ContinuePrintDlg);
        startEdit->setObjectName(QStringLiteral("startEdit"));
        startEdit->setGeometry(QRect(100, 20, 113, 20));
        endEdit = new QLineEdit(ContinuePrintDlg);
        endEdit->setObjectName(QStringLiteral("endEdit"));
        endEdit->setGeometry(QRect(100, 60, 113, 20));
        confirmBtn = new QPushButton(ContinuePrintDlg);
        confirmBtn->setObjectName(QStringLiteral("confirmBtn"));
        confirmBtn->setGeometry(QRect(30, 150, 75, 23));
        cancelBtn = new QPushButton(ContinuePrintDlg);
        cancelBtn->setObjectName(QStringLiteral("cancelBtn"));
        cancelBtn->setGeometry(QRect(140, 150, 75, 23));
        tipLabel = new QLabel(ContinuePrintDlg);
        tipLabel->setObjectName(QStringLiteral("tipLabel"));
        tipLabel->setGeometry(QRect(30, 90, 181, 41));
        tipLabel->setStyleSheet(QStringLiteral("QLabel{border-width: 1px;border-style: solid;border-color:gray;}"));

        retranslateUi(ContinuePrintDlg);

        QMetaObject::connectSlotsByName(ContinuePrintDlg);
    } // setupUi

    void retranslateUi(QDialog *ContinuePrintDlg)
    {
        ContinuePrintDlg->setWindowTitle(QApplication::translate("ContinuePrintDlg", "ContinuePrintDlg", 0));
        startLabel->setText(QApplication::translate("ContinuePrintDlg", "\350\265\267\345\247\213\346\256\265\350\220\275", 0));
        endLabel->setText(QApplication::translate("ContinuePrintDlg", "\347\273\223\346\235\237\346\256\265\350\220\275", 0));
        confirmBtn->setText(QApplication::translate("ContinuePrintDlg", "\347\241\256\345\256\232", 0));
        cancelBtn->setText(QApplication::translate("ContinuePrintDlg", "\345\217\226\346\266\210", 0));
        tipLabel->setText(QApplication::translate("ContinuePrintDlg", "\347\273\223\346\235\237\346\256\265\350\220\275\345\200\274\345\272\224\350\257\245\345\244\247\344\272\216\350\265\267\345\247\213\346\256\265\350\220\275\345\200\274", 0));
    } // retranslateUi

};

namespace Ui {
    class ContinuePrintDlg: public Ui_ContinuePrintDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTINUEPRINTDLG_H
