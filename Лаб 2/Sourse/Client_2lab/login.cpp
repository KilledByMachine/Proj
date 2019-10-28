#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);

}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{

    close();
    emit show_main();

}

void Login::on_pushButton_2_clicked()
{

    close();
    emit show_reg();

}
