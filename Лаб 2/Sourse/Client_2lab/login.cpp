#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    //конект з сервером, мб виводити діалогове про конект з ним, і не убирати док не зєднає
    // в разі не підключення, повідомити про це, нікуди не пускати мб
    //
    //

}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    // перевірка логіна + пароля
    // перевірка коректності вводу?
    // запит на сервер, якщо - то видати повідомлення про помилку (нема користувача\непра пароль)
    // інакше перкйти в головне вікно, записати собі ІД юзера (для запитів на табл)
    // перепистаи конфігурації данного користувача
    close();
    emit show_main();

}

void Login::on_pushButton_2_clicked()
{


    close();
    emit show_reg();

}
