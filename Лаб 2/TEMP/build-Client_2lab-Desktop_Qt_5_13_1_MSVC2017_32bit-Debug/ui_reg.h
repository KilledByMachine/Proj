/********************************************************************************
** Form generated from reading UI file 'reg.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REG_H
#define UI_REG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Reg
{
public:
    QPushButton *pushButton;
    QLineEdit *lineEdit;

    void setupUi(QDialog *Reg)
    {
        if (Reg->objectName().isEmpty())
            Reg->setObjectName(QString::fromUtf8("Reg"));
        Reg->resize(400, 300);
        pushButton = new QPushButton(Reg);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(120, 130, 111, 23));
        lineEdit = new QLineEdit(Reg);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(140, 190, 113, 20));

        retranslateUi(Reg);

        QMetaObject::connectSlotsByName(Reg);
    } // setupUi

    void retranslateUi(QDialog *Reg)
    {
        Reg->setWindowTitle(QCoreApplication::translate("Reg", "Reg", nullptr));
        pushButton->setText(QCoreApplication::translate("Reg", "\320\235\320\260\320\267\320\260\320\264 \320\262 \320\273\320\276\320\263\321\226\320\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Reg: public Ui_Reg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REG_H
