#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->label_3->hide();
    ui->label_4->hide();
    ui->pushButton->setDisabled(true);
    sok=new QTcpSocket(this);
    sok->connectToHost(QHostAddress::LocalHost,6000);
    QMessageBox msgBox;
    msgBox.setText("ТРАБЛ!");
    msgBox.setInformativeText("Проблеми з підключенням до серверу. \nСпробувати підключитись знову?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if(!sok->waitForConnected(500) )
    {
        int res = msgBox.exec();
        if (res == QMessageBox::Ok) //нажата кнопка Ok
               qDebug()<<"1"; // ще спроба конекту
        else
        {
            qDebug()<<"2";  //не робити нічого
            this->close();
        }
    }

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
    qDebug()<<sok->state();
    QString part="find:login:";
    if(sok->state()==QTcpSocket:: ConnectedState)
    {
        QTextStream sended(sok);
        sended<<part+ui->lineEdit->text()+':'+ui->lineEdit_2->text()+';';
        sended<<flush;
        qDebug()<<"Md send";
    }
    else
    {
        QMessageBox:: information(this,"Цей во.. ","Кеп в нас проблеми");
    }
    //close();
    //emit show_main();

}

void Login::on_pushButton_2_clicked()
{


    close();
    emit show_reg();

}
void Login::chek_availible(QString temp, int num)
{
    bool tmp = true;
    for(int i=0; i<temp.size(); i++)
    {
        if( (temp[i]<'a' || temp[i]>'z') && (temp[i]<'A' || temp[i]>'Z')  && (temp[i]<'0' || temp[i]>'9')  )
        {
            tmp=false;
            break;
        }
    }
    if(num==1)
    {   login_av=tmp;  }
    else {
        reg_av=tmp;
    }
    if(num==1)
    {
        if(login_av)
            ui->label_3->hide();
        else
            ui->label_3->show();
    }
    else
    {
        if(reg_av)
            ui->label_4->hide();
        else
            ui->label_4->show();
    }
    is_available=login_av && reg_av && temp.size()!=0;
    if(!is_available){
        ui->pushButton->setDisabled(true);
    }
    else{
        ui->pushButton->setEnabled(true);
    }



}

void Login::on_lineEdit_textChanged(const QString &arg1)
{
    chek_availible(arg1,1);

}

void Login::on_lineEdit_2_textChanged(const QString &arg1)
{
     chek_availible(arg1,2);
}
