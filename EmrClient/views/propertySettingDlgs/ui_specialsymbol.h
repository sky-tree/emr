/********************************************************************************
** Form generated from reading UI file 'specialsymbol.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPECIALSYMBOL_H
#define UI_SPECIALSYMBOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SpecialSymbol
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_medicalSymbol;
    QHBoxLayout *horizontalLayout_2;
    QTableView *tW_medicalSymbol;
    QWidget *tab_commonSymbol;
    QHBoxLayout *horizontalLayout_3;
    QTableView *tW_commonSymbol;
    QWidget *tab_numberSymbol;
    QHBoxLayout *horizontalLayout_4;
    QTableView *tW_numberSymbol;
    QWidget *tab_mathSymbol;
    QHBoxLayout *horizontalLayout_5;
    QTableView *tW_mathSymbol;
    QWidget *tab_specialSymbol;
    QHBoxLayout *horizontalLayout_6;
    QTableView *tW_specialSymbol;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_Ok;
    QPushButton *btn_Cancel;

    void setupUi(QWidget *SpecialSymbol)
    {
        if (SpecialSymbol->objectName().isEmpty())
            SpecialSymbol->setObjectName(QStringLiteral("SpecialSymbol"));
        SpecialSymbol->resize(585, 325);
        gridLayout = new QGridLayout(SpecialSymbol);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(SpecialSymbol);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_medicalSymbol = new QWidget();
        tab_medicalSymbol->setObjectName(QStringLiteral("tab_medicalSymbol"));
        horizontalLayout_2 = new QHBoxLayout(tab_medicalSymbol);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        tW_medicalSymbol = new QTableView(tab_medicalSymbol);
        tW_medicalSymbol->setObjectName(QStringLiteral("tW_medicalSymbol"));

        horizontalLayout_2->addWidget(tW_medicalSymbol);

        tabWidget->addTab(tab_medicalSymbol, QString());
        tab_commonSymbol = new QWidget();
        tab_commonSymbol->setObjectName(QStringLiteral("tab_commonSymbol"));
        horizontalLayout_3 = new QHBoxLayout(tab_commonSymbol);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        tW_commonSymbol = new QTableView(tab_commonSymbol);
        tW_commonSymbol->setObjectName(QStringLiteral("tW_commonSymbol"));

        horizontalLayout_3->addWidget(tW_commonSymbol);

        tabWidget->addTab(tab_commonSymbol, QString());
        tab_numberSymbol = new QWidget();
        tab_numberSymbol->setObjectName(QStringLiteral("tab_numberSymbol"));
        horizontalLayout_4 = new QHBoxLayout(tab_numberSymbol);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        tW_numberSymbol = new QTableView(tab_numberSymbol);
        tW_numberSymbol->setObjectName(QStringLiteral("tW_numberSymbol"));

        horizontalLayout_4->addWidget(tW_numberSymbol);

        tabWidget->addTab(tab_numberSymbol, QString());
        tab_mathSymbol = new QWidget();
        tab_mathSymbol->setObjectName(QStringLiteral("tab_mathSymbol"));
        horizontalLayout_5 = new QHBoxLayout(tab_mathSymbol);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        tW_mathSymbol = new QTableView(tab_mathSymbol);
        tW_mathSymbol->setObjectName(QStringLiteral("tW_mathSymbol"));

        horizontalLayout_5->addWidget(tW_mathSymbol);

        tabWidget->addTab(tab_mathSymbol, QString());
        tab_specialSymbol = new QWidget();
        tab_specialSymbol->setObjectName(QStringLiteral("tab_specialSymbol"));
        horizontalLayout_6 = new QHBoxLayout(tab_specialSymbol);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        tW_specialSymbol = new QTableView(tab_specialSymbol);
        tW_specialSymbol->setObjectName(QStringLiteral("tW_specialSymbol"));

        horizontalLayout_6->addWidget(tW_specialSymbol);

        tabWidget->addTab(tab_specialSymbol, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(368, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_Ok = new QPushButton(SpecialSymbol);
        btn_Ok->setObjectName(QStringLiteral("btn_Ok"));

        horizontalLayout->addWidget(btn_Ok);

        btn_Cancel = new QPushButton(SpecialSymbol);
        btn_Cancel->setObjectName(QStringLiteral("btn_Cancel"));

        horizontalLayout->addWidget(btn_Cancel);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(SpecialSymbol);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SpecialSymbol);
    } // setupUi

    void retranslateUi(QWidget *SpecialSymbol)
    {
        SpecialSymbol->setWindowTitle(QApplication::translate("SpecialSymbol", "\347\254\246\345\217\267", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_medicalSymbol), QApplication::translate("SpecialSymbol", "\345\214\273\345\255\246\347\254\246\345\217\267", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_commonSymbol), QApplication::translate("SpecialSymbol", "\345\270\270\347\224\250\347\254\246\345\217\267", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_numberSymbol), QApplication::translate("SpecialSymbol", "\346\225\260\345\255\227\347\254\246\345\217\267", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_mathSymbol), QApplication::translate("SpecialSymbol", "\346\225\260\345\255\246\347\254\246\345\217\267", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_specialSymbol), QApplication::translate("SpecialSymbol", "\347\211\271\346\256\212\347\254\246\345\217\267", 0));
        btn_Ok->setText(QApplication::translate("SpecialSymbol", "\347\241\256\345\256\232", 0));
        btn_Cancel->setText(QApplication::translate("SpecialSymbol", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class SpecialSymbol: public Ui_SpecialSymbol {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPECIALSYMBOL_H
