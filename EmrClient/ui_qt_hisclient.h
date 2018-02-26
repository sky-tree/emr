/********************************************************************************
** Form generated from reading UI file 'qt_hisclient.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_HISCLIENT_H
#define UI_QT_HISCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Qt_HisClientClass
{
public:
    QWidget *centralWidget;
    QWidget *background;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *userNameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginBtn;
    QPushButton *cancelBtn;
    QLineEdit *ipAndPortEdit;
    QLabel *label;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Qt_HisClientClass)
    {
        if (Qt_HisClientClass->objectName().isEmpty())
            Qt_HisClientClass->setObjectName(QStringLiteral("Qt_HisClientClass"));
        Qt_HisClientClass->resize(610, 381);
        QFont font;
        font.setPointSize(12);
        Qt_HisClientClass->setFont(font);
        Qt_HisClientClass->setStyleSheet(QStringLiteral("#Qt_HisClientClass{background-color:rgb(226, 226, 226)}"));
        centralWidget = new QWidget(Qt_HisClientClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        background = new QWidget(centralWidget);
        background->setObjectName(QStringLiteral("background"));
        background->setGeometry(QRect(0, 90, 601, 241));
        background->setStyleSheet(QLatin1String("*[class~=\"QWidget\"]{image:url(:/Res/images/loginmain.png)}\n"
"       .QPushButton{\n"
"       background-color: rgb(137,203,235);\n"
"       border: 1px solid rgb(124, 124, 124);\n"
"       border-radius:1px;\n"
"       }\n"
"QWidget > QPushButton:hover{background-color:rgb(141, 184, 6)}\n"
"QWidget > QPushButton:pressed{background-color:rgb(141, 184, 6, 200)}\n"
"QWidget > QComboBox{background-color:rgb(137,203,235);}\n"
"QWidget > QLineEdit{background-color: rgb(137,203,235);}"));
        label_2 = new QLabel(background);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(310, 50, 61, 31));
        label_2->setFont(font);
        label_2->setStyleSheet(QStringLiteral(""));
        label_3 = new QLabel(background);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(310, 100, 61, 31));
        label_3->setFont(font);
        label_3->setStyleSheet(QStringLiteral(""));
        userNameEdit = new QLineEdit(background);
        userNameEdit->setObjectName(QStringLiteral("userNameEdit"));
        userNameEdit->setGeometry(QRect(370, 50, 181, 31));
        userNameEdit->setStyleSheet(QStringLiteral(""));
        passwordEdit = new QLineEdit(background);
        passwordEdit->setObjectName(QStringLiteral("passwordEdit"));
        passwordEdit->setGeometry(QRect(370, 100, 181, 31));
        passwordEdit->setStyleSheet(QStringLiteral(""));
        passwordEdit->setEchoMode(QLineEdit::Password);
        loginBtn = new QPushButton(background);
        loginBtn->setObjectName(QStringLiteral("loginBtn"));
        loginBtn->setGeometry(QRect(310, 160, 81, 31));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(false);
        font1.setWeight(50);
        loginBtn->setFont(font1);
        loginBtn->setStyleSheet(QStringLiteral(""));
        cancelBtn = new QPushButton(background);
        cancelBtn->setObjectName(QStringLiteral("cancelBtn"));
        cancelBtn->setGeometry(QRect(470, 160, 81, 31));
        cancelBtn->setFont(font);
        cancelBtn->setStyleSheet(QStringLiteral(""));
        ipAndPortEdit = new QLineEdit(background);
        ipAndPortEdit->setObjectName(QStringLiteral("ipAndPortEdit"));
        ipAndPortEdit->setGeometry(QRect(20, 210, 181, 31));
        ipAndPortEdit->setStyleSheet(QStringLiteral(""));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(210, 50, 161, 41));
        QFont font2;
        font2.setPointSize(18);
        font2.setBold(true);
        font2.setWeight(75);
        label->setFont(font2);
        label->setStyleSheet(QStringLiteral("color:rgb(23, 110, 187)"));
        Qt_HisClientClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Qt_HisClientClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 610, 23));
        Qt_HisClientClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(Qt_HisClientClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Qt_HisClientClass->setStatusBar(statusBar);

        retranslateUi(Qt_HisClientClass);

        QMetaObject::connectSlotsByName(Qt_HisClientClass);
    } // setupUi

    void retranslateUi(QMainWindow *Qt_HisClientClass)
    {
        Qt_HisClientClass->setWindowTitle(QApplication::translate("Qt_HisClientClass", "Qt_HisClient", 0));
        label_2->setText(QApplication::translate("Qt_HisClientClass", "\347\224\250\346\210\267\345\220\215\357\274\232", 0));
        label_3->setText(QApplication::translate("Qt_HisClientClass", "\345\257\206  \347\240\201\357\274\232", 0));
        loginBtn->setText(QApplication::translate("Qt_HisClientClass", "\347\231\273\345\275\225", 0));
        cancelBtn->setText(QApplication::translate("Qt_HisClientClass", "\345\217\226\346\266\210", 0));
        label->setText(QApplication::translate("Qt_HisClientClass", "\345\214\273\351\231\242\347\256\241\347\220\206\347\263\273\347\273\237", 0));
    } // retranslateUi

};

namespace Ui {
    class Qt_HisClientClass: public Ui_Qt_HisClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_HISCLIENT_H
