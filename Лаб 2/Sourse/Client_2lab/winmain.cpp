#include "winmain.h"
#include "ui_winmain.h"

WinMain::WinMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WinMain)
{
    ui->setupUi(this);
}

WinMain::~WinMain()
{
    delete ui;
}
bool operator ==(const my_config& left, const my_config& right)
{
    if(left.c1 == right.c1 && left.c2 == right.c2 && left.c3 == right.c3 && left.c4 == right.c4
            && left.low_r == right.low_r && left.hight_r == right.hight_r && left.low_rate == right.low_rate
            && left.hight_rate == right.hight_rate && left.sem1 == right.sem1 && left.sem2 == right.sem2
            && left.sorting_by == right.sorting_by && left.igdg == right.igdg && left.igsn == right.igsn
            && left.ikni == right.ikni && left.ikta == right.ikta
            && left._2018 == right._2018 && left._2017 == right._2017 && left._2016 == right._2016
            && left._2015 == right._2015)

    return true;
    else
        return false;
}
bool operator!=(const my_config& left, const my_config& right)
{
    return !(left==right);
}
void WinMain:: start(){
    sok=new QTcpSocket(this);
    qDebug()<<"Connected";

    sok->connectToHost(QHostAddress::LocalHost,6000);
    QMessageBox msg;
    msg.setIcon(QMessageBox::Information);  //	Icon { NoIcon, Question, Information, Warning, Critical }
    msg.setText("Проблеми!");
    msg.setInformativeText("Відсутній звязок з сервером!");
    if(!sok->waitForConnected(500))
    {
        qDebug()<<"WTF&";
       msg.exec();
    }
    else{
        connect(sok,SIGNAL(readyRead()),this,SLOT(get_from_serv()));
        QTextStream send(sok);
        QString s;
        s="getconf:";
        send<<s;
        send<<MyKey;
        send<<';';
        send<<flush;
    }

}

void WinMain::on_pushButton_clicked()
{
    //ui->doubleSpin_from_2->setValue(13.133);
    //ui->action1->setChecked(false); true\flase
    //qDebug()<<ui->action1->isChecked();
    sok->disconnectFromHost();
    emit show_log();
    close();
}

void WinMain:: get_keyID(int key){
    MyKey=key;
    qDebug()<<key;
}

void WinMain:: get_from_serv()
{

    QString s = sok->readAll();
    //QByteArray data = my->readAll();
    //login буде відправлятись з обрботчика після декодування rfind
    qDebug() << s;
    decoding(s);
}
void WinMain:: decoding(QString command)
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
    if(part_heder=="nres")
    {
        QString sended_num;
        int num; bool res_convert;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                sended_num.append(command[i]);
                pos=i;
            }
        }
        num=sended_num.toInt(&res_convert,10);
        if(!res_convert)     // невірні параметри
        {
            qDebug()<<"wrong  param";
            return;
        }
        else {
            qDebug()<<num<<" - кількість пришедших результатів";
        }
    }
    if(part_heder=="putconf")
    {
        QString keyid,low_r,hight_r,low_rate,hight_rate,sem,sorting_by,course,inst,year;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                keyid.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                low_r.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                hight_r.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                low_rate.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                hight_rate.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                sem.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                sorting_by.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                course.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                inst.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                year.append(command[i]);
                pos=i;
            }
        }
        // keyid,low_r,hight_r,low_rate,hight_rate,sem,sorting_by,course,inst,year
        if(keyid.size()==0 || low_r.size()==0 || hight_r.size()==0 ||low_rate.size()==0 ||hight_rate.size()==0
                || sem.size()==0 ||sorting_by.size()==0 || course.size()==0 || inst.size()==0 || year.size()==0)
            //якщо парамтери пусті
        {
            qDebug()<<"no match param";
            return;
        }
        else {
            if(sem.size()!=1 ||sorting_by.size()!=1 || course.size()!=1 || inst.size()!=1 || year.size()!=1)
            {
                qDebug()<<"bad size of param";
                return;
            }
            //дофіга перевірок і конвертів з строки в нужні числа
            qDebug()<<keyid<<low_r<<hight_r<<low_rate<<hight_rate<<sem<<sorting_by<<course<<inst<<year;
            bool convert; int to_convert=0;
            my_config temp;
            int Key = keyid.toInt(&convert,10);
            if(MyKey != Key) { qDebug()<<"Bad keyID"; return;}
            if(!convert){ qDebug()<<"Bad convert"; return;}
            temp.low_r=low_r.toDouble(&convert);
            if(!convert){ qDebug()<<"Bad convert"; return;}
            temp.hight_r=hight_r.toDouble(&convert);
            if(!convert){ qDebug()<<"Bad convert"; return;}
            temp.low_rate=low_rate.toDouble(&convert);
            if(!convert){ qDebug()<<"Bad convert"; return;}
            temp.hight_rate=hight_rate.toDouble(&convert);
            if(!convert){ qDebug()<<"Bad convert"; return;}
            to_convert=sem.toInt(&convert,16);
            switch(to_convert){
            case 0:
                    temp.sem1=false;
                    temp.sem2=false;
                break;
            case 1:
                temp.sem1=true;
                temp.sem2=false;
                break;
            case 2:
                temp.sem1=false;
                temp.sem2=true;
                break;
            case 3:
                temp.sem1=true;
                temp.sem2=true;
                break;
            default:
                convert=false;
                break;
            }
            if(!convert){ qDebug()<<"Bad convert"; return;}
            temp.sorting_by=sorting_by.toInt(&convert,10);
            if(!convert && temp.sorting_by<=2){ qDebug()<<"Bad convert"; return;}
            to_convert=course.toInt(&convert,16);
            temp.c4=to_convert/8; to_convert%=8;
            temp.c3=to_convert/4; to_convert%=4;
            temp.c2=to_convert/2; to_convert%=2;
            temp.c1=to_convert/1;
            if(!convert){ qDebug()<<"Bad convert"; return;}
            to_convert=inst.toInt(&convert,16);
            temp.igdg=to_convert/8; to_convert%=8;
            temp.igsn=to_convert/4; to_convert%=4;
            temp.ikni=to_convert/2; to_convert%=2;
            temp.ikta=to_convert/1;
            if(!convert){ qDebug()<<"Bad convert"; return;}
            to_convert=inst.toInt(&convert,16);
            temp._2018=to_convert/8; to_convert%=8;
            temp._2017=to_convert/4; to_convert%=4;
            temp._2016=to_convert/2; to_convert%=2;
            temp._2015=to_convert/1;
            if(!convert){ qDebug()<<"Bad convert"; return;}
            else
            {
                //перекидаємо нові налаштування, виклик фукнцію для налаштування по вікні
                Config = temp;


            }

        }
     }
    if(part_heder=="send")
    {
        QString fname,lname,pname,rate,rate2,course , year ,sem ,inst,number;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                fname.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                lname.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                pname.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                rate.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                rate2.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                course.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                year.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                sem.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                inst.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                number.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        // QString fname,lname,pname,rate,rate2,course , year ,sem ,inst,number;
        //занесення в табл, відображення і тд
        qDebug()<<"!&!&&@&!&&!&!";
        ui->textEdit->append(fname+lname+pname+rate+rate2+course+year+sem+inst+number);
        //ui->textEdit->setReadOnly(true);
        //ui->textEdit->setText("CHO BLAD");

    }
}

void WinMain::on_prev_botton_clicked()
{
    //ui->textEdit->setText("CHO BLAD");
    ui->comboBox->setCurrentIndex(2);
}
void WinMain::change_conf()
{
    //low_r,hight_r,low_rate,hight_rate,sem,sorting_by,course,inst,year;
    ui->doubleSpin_from->setValue(Config.low_r);
    ui->doubleSpin_to->setValue(Config.hight_r);
    ui->doubleSpin_from_2->setValue(Config.low_rate);
    ui->doubleSpin_to_2->setValue(Config.hight_rate);
    ui->sem1->setChecked(Config.sem1);
    ui->sem2->setChecked(Config.sem2);
    ui->comboBox->setCurrentIndex(Config.sorting_by);
    ui->course1->setChecked(Config.c1);
    ui->course2->setChecked(Config.c2);
    ui->course3->setChecked(Config.c3);
    ui->course4->setChecked(Config.c4);
    ui->igdg->setChecked(Config.igdg);
    ui->igsn->setChecked(Config.igsn);
    ui->ikni->setChecked(Config.ikni);
    ui->ikta->setChecked(Config.ikta);
    ui->action2015->setChecked(Config._2015);
    ui->action2016->setChecked(Config._2016);
    ui->action2017->setChecked(Config._2017);
    ui->action2018->setChecked(Config._2018);
}
void WinMain:: config_change()
{
    Config.low_r=ui->doubleSpin_from->value();
    Config.hight_r=ui->doubleSpin_to->value();
    Config.low_r=ui->doubleSpin_from_2->value();
    Config.hight_rate=ui->doubleSpin_to_2->value();
    Config.sem1=ui->sem1->isChecked();
    Config.sem2=ui->sem2->isChecked();
    Config.sorting_by=ui->comboBox->currentIndex();
    Config.c1=ui->course1->isChecked();
    Config.c2=ui->course2->isChecked();
    Config.c3=ui->course3->isChecked();
    Config.c4=ui->course4->isChecked();
    Config.igdg=ui->igdg->isChecked();
    Config.igsn=ui->igsn->isChecked();
    Config.ikni=ui->ikni->isChecked();
    Config.ikta=ui->ikta->isChecked();
    Config._2015=ui->action2015->isChecked();
    Config._2016=ui->action2016->isChecked();
    Config._2017=ui->action2017->isChecked();
    Config._2018=ui->action2018->isChecked();
}

void WinMain::on_comboBox_currentIndexChanged(int index)
{
    qDebug()<<index;
    //рейтинг - 0, бал - 1, ім - 2
}
