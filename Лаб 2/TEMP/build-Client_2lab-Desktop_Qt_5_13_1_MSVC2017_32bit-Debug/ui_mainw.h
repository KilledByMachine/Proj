/********************************************************************************
** Form generated from reading UI file 'mainw.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINW_H
#define UI_MAINW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainW
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainW)
    {
        if (MainW->objectName().isEmpty())
            MainW->setObjectName(QString::fromUtf8("MainW"));
        MainW->resize(513, 307);
        centralwidget = new QWidget(MainW);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(210, 120, 75, 23));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(200, 190, 113, 20));
        MainW->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainW);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 513, 21));
        MainW->setMenuBar(menubar);
        statusbar = new QStatusBar(MainW);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainW->setStatusBar(statusbar);

        retranslateUi(MainW);

        QMetaObject::connectSlotsByName(MainW);
    } // setupUi

    void retranslateUi(QMainWindow *MainW)
    {
        MainW->setWindowTitle(QCoreApplication::translate("MainW", "MainW", nullptr));
        pushButton->setText(QCoreApplication::translate("MainW", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainW: public Ui_MainW {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINW_H
