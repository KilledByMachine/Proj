#include "reg.h"
#include "ui_reg.h"
#include <QTcpSocket>

Reg::Reg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Reg)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->pushButton_2->setDisabled(true);
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
}

Reg::~Reg()
{
    delete ui;
}

void Reg:: start(){
    sok=new QTcpSocket(this);
    connect(sok,SIGNAL(readyRead()),this,SLOT(get_from_serv()));
    sok->connectToHost(QHostAddress::LocalHost,6000);
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);  //	Icon { NoIcon, Question, Information, Warning, Critical }
    msg.setText("Проблеми!");
    msg.setInformativeText("Відсутній звязок з сервером!");
    if(!sok->waitForConnected(500))
    {
       msg.exec();
    }

}

void Reg::on_pushButton_clicked()
{
    sok->disconnectFromHost();
    emit show_log();
    close();
}

void Reg::on_pushButton_2_clicked()
{
    // конект сервера
    //  перевірка наявності такого користувача, вивідо помилки або посил на Бд щоби записала
    // там (на бд) встановлення дефолтних конфігурацій, вони там будуть вписані, ці конф читаються разом з ІД
    // вивід повідомлення про вдалу реєсрацію .месбокс кароч, і привітання?
    QString part="find:user:";
    if(sok->state()==QTcpSocket:: ConnectedState)
    {
        QTextStream sended(sok);
        sended<<part+ui->lineEdit->text()+';';
        sended<<flush;
    }
    else
    {
        QMessageBox:: information(this,"ДОСІ","Досі немає зв'язку!");
    }
    //emit show_main();
    //close();
}
void Reg:: get_from_serv()
{

    QString s = sok->readAll();
    //QByteArray data = my->readAll();
    //login буде відправлятись з обрботчика після декодування rfind
    qDebug() << s;
    decoding(s);

}

void Reg::chek_availible(QString temp, int num)
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
    {   login_av=tmp;
    }
    if(num==2)
    {
        pass1_av=tmp;
    }
    if(num==3)
       {
        pass2_av=tmp;
        if(ui->lineEdit_2->text()!=ui->lineEdit_3->text())
            pass2_av=false;

       }
    if(num==1)
    {
        if(login_av)
            ui->label_4->hide();
        else
            ui->label_4->show();
    }
    if(num==2)
    {
        if(pass1_av)
            ui->label_5->hide();
        else
            ui->label_5->show();
    }
    if(num==3)
    {
        if(pass2_av)
            ui->label_6->hide();
        else
            ui->label_6->show();
    }
    is_available=login_av && pass1_av && pass2_av && temp.size()!=0;
    if(!is_available){
        ui->pushButton_2->setDisabled(true);
    }
    else{
        ui->pushButton_2->setEnabled(true);
    }

}

void Reg::on_lineEdit_textChanged(const QString &arg1)
{
    chek_availible(arg1,1);

}

void Reg::on_lineEdit_2_textChanged(const QString &arg1)
{
    chek_availible(arg1,2);
    chek_availible(ui->lineEdit_3->text(),3);
}

void Reg::on_lineEdit_3_textChanged(const QString &arg1)
{
    chek_availible(arg1,3);
}

void Reg:: decoding(QString command)
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
        QString user_header,user_found;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                user_header.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                user_found.append(command[i]);
                pos=i;
            }
        }
        if(user_header!="user" || user_found.size()!=1)     // невірні параметри
        {
            qDebug()<<"wrong  param";
            return;
        }
        else {
            if(user_header=="user" && user_found[0]=='0'){
                QTextStream send_it(sok);
                send_it<<"reg:"+ui->lineEdit->text()+':'+ui->lineEdit_2->text()+';';
                send_it<<flush;
            }
            else if(user_header=="user" && user_found[0]=='1'){
                QMessageBox:: information(this,"Помилка","Такий користувач уже існує!");
            }
        }
    }
    if(part_heder=="rreg")
    {
        QString reg_res,key;
        int keyID;
        bool convert_success;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                reg_res.append(command[i]);
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
        if(reg_res.size()!=1 || !convert_success)     //якщо парамтери пусті
        {
            qDebug()<<"wrong  param";
            return;
        }
        else {
            if(reg_res[0]=='1')
            {
                ui->lineEdit->clear();
                ui->lineEdit_2->clear();
                ui->lineEdit_3->clear();
                sok->disconnectFromHost();
                emit send_ID(keyID);
                emit show_main();                
                close();

            }
            else{
                QMessageBox:: information(this,"Трабл","Помилка реєстрації");
            }
        }


    }

}

