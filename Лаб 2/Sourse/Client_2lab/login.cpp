#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    start();
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->label_3->hide();
    ui->label_4->hide();
    ui->pushButton->setDisabled(true);
    //login буде відправлятись з обрботчика після декодування rfind
    /* якщо в рез пошуку буде інфа що немає логіну, або пароль не дійсний, мес бокс з цим повідомленням
     * якщо все ок то відправити login: з параметрами з строк, по результату логіну сказти, вдала чи ні авторизація
     * і якщо вдала, відключитись від сер, показати головне вікно, передати йому данні, закрити це вікно
    */
}

Login::~Login()
{
    delete ui;
}
void Login:: start(){
    sok=new QTcpSocket(this);
    connect(sok,SIGNAL(readyRead()),this,SLOT(get_from_serv()));
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
        if (res == QMessageBox::Ok)
        {
               sok->connectToHost(QHostAddress::LocalHost,6000);
               if(!sok->waitForConnected(500))
               {
                   QMessageBox msg;
                   msg.setIcon(QMessageBox::Information);  //	Icon { NoIcon, Question, Information, Warning, Critical }
                   msg.setText("Нема чим зайнятись?!");
                   msg.setInformativeText("Опять 25! Не підключились!");
                   msg.exec();
               }

        }
    }

}

void Login::on_pushButton_clicked()
{
    // перевірка логіна + пароля
    // перевірка коректності вводу?
    // запит на сервер, якщо - то видати повідомлення про помилку (нема користувача\непра пароль)
    // інакше перкйти в головне вікно, записати собі ІД юзера (для запитів на табл)
    // перепистаи конфігурації данного користувача

    QString part="find:login:";
    if(sok->state()==QTcpSocket:: ConnectedState)
    {
        QTextStream sended(sok);
        sended<<part+ui->lineEdit->text()+':'+ui->lineEdit_2->text()+';';
        sended<<flush;
        //qDebug()<<"Md send";
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
    sok->disconnectFromHost();
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
void Login:: get_from_serv()
{

    QString s = sok->readAll();
    //QByteArray data = my->readAll();
    //login буде відправлятись з обрботчика після декодування rfind
    //qDebug() << s;
    decoding(s);
}

void Login:: decoding(QString command)
{
    QString part_heder;
    int pos=0;
    if(command[command.size()-1]!=';')      //некоректне закінчення команди
    {
        qDebug()<<"wrong_end_of_command";
        return;
    }
    for (int i=0;i<command.size();i++)
    {
        if(command[i]==':' || command[i]==';') break;
        else
           {
            part_heder.append(command[i]);
            pos=i;
        }
    }
    pos+=2; // пересунутись на початок 2-го слова, перескочити ':'
    if(pos>=command.size())     //хедер не може займати весь рядок неіснує нічого по типу find:
    {
        qDebug()<<"wrong_command";
        return;
    }
    if(part_heder=="rfind")
    {
        QString login_found,pass_found;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                login_found.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                pass_found.append(command[i]);
                pos=i;
            }
        }
        if(login_found.size()!=1 || pass_found.size()!=1)     //якщо парамтери пусті
        {
            qDebug()<<"wrong size of param";
            return;
        }
        else {
            //qDebug()<<login_found<<""<<pass_found;
            if(login_found[0]=='1' && pass_found[0]=='1'){
                QTextStream send_it(sok);
                send_it<<"login:"+ui->lineEdit->text()+':'+ui->lineEdit_2->text()+';';
                send_it<<flush;
            }
            else if(login_found[0]=='1' && pass_found[0]=='0'){
                QMessageBox:: information(this,"Помилка","Пароль невірний!");
            }
            else if(login_found[0]=='0'){
                QMessageBox:: information(this,"Помилка","Логін невірний!");
            }
        }

    }
    if(part_heder=="rlogin")
    {
        QString login_found,key;
        int keyID;
        bool convert_success;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                login_found.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                key.append(command[i]);
                pos=i;
            }
        }
        keyID=key.toInt(&convert_success,10); //спроба конвертувати
        if(login_found.size()!=1 || !convert_success)     //якщо парамтери пусті
        {
            qDebug()<<"wrong  param";
            return;
        }
        else {
            if(login_found[0]=='1')
            {
                sok->disconnectFromHost();
                emit send_ID(keyID);
                emit show_main();
                close();

            }
            else{
                QMessageBox:: information(this,"Трабл","Залогінитись не вдалось");
            }
        }


    }

}

