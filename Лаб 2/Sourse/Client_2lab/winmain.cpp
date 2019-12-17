#include "winmain.h"
#include "ui_winmain.h"

WinMain::WinMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WinMain)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
    //table.setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    Sended_conf.sorting_by=13; // щоби налаштування не співпали

}

QList <table_records> My_Table;
void WinMain::on_pushButton_clicked()
{
    //ui->doubleSpin_from_2->setValue(13.133);
    //ui->action1->setChecked(false); true\flase
    //qDebug()<<ui->action1->isChecked();
    //ui->textEdit->clear();
    sok->disconnectFromHost();
    ui->tableWidget->clearContents();
    emit show_log();
    close();
}

void WinMain:: get_keyID(int key){
    MyKey=key;
    qDebug()<<key;
}

void WinMain:: update_table(){
        qDebug()<<My_Table.size()<<now<<next;
        if(My_Table.size()<=10){
            for(int i=0;i<My_Table.size();i++){

                    ui->tableWidget->setItem(i,0,new QTableWidgetItem(My_Table[i].fname));
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(My_Table[i].lname));
                    ui->tableWidget->setItem(i,2,new QTableWidgetItem(My_Table[i].pname));
                    ui->tableWidget->setItem(i,3,new QTableWidgetItem(My_Table[i].rate));
                    ui->tableWidget->setItem(i,4,new QTableWidgetItem(My_Table[i].rate2));
                    ui->tableWidget->setItem(i,5,new QTableWidgetItem(My_Table[i].course));
                    ui->tableWidget->setItem(i,6,new QTableWidgetItem(My_Table[i].year));
                    ui->tableWidget->setItem(i,7,new QTableWidgetItem(My_Table[i].sem));
                    ui->tableWidget->setItem(i,8,new QTableWidgetItem(My_Table[i].inst));

                    //0-імя 1-прізв 2 - по б 3-рейт 4-сер б 5-курс 6-рік 7-інст

            }
        }
        else {
            for(int i=0;i+now<My_Table.size() && i+now<=next;i++){

                    ui->tableWidget->setItem(i,0,new QTableWidgetItem(My_Table[now+i].fname));
                    ui->tableWidget->setItem(i,1,new QTableWidgetItem(My_Table[now+i].lname));
                    ui->tableWidget->setItem(i,2,new QTableWidgetItem(My_Table[now+i].pname));
                    ui->tableWidget->setItem(i,3,new QTableWidgetItem(My_Table[now+i].rate));
                    ui->tableWidget->setItem(i,4,new QTableWidgetItem(My_Table[now+i].rate2));
                    ui->tableWidget->setItem(i,5,new QTableWidgetItem(My_Table[now+i].course));
                    ui->tableWidget->setItem(i,6,new QTableWidgetItem(My_Table[now+i].year));
                    ui->tableWidget->setItem(i,7,new QTableWidgetItem(My_Table[now+i].sem));
                    ui->tableWidget->setItem(i,8,new QTableWidgetItem(My_Table[now+i].inst));

                    //0-імя 1-прізв 2 - по б 3-рейт 4-сер б 5-курс 6-рік 7-інст

            }


        }
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void WinMain:: get_from_serv()
{

    QString s = sok->readAll();     //або розбивати вручну по 1 команді і на декодування, або запустити цикл з сплітом
    QString part_decoding;
    part_decoding.clear();
    for(int i=0;i<s.size();i++){
        if(s[i]==';'){
            decoding(part_decoding+';');
            part_decoding.clear();
        }
        else{
            part_decoding.append(s[i]);
        }
    }
    //qDebug() << "FROM GET!!-----"<<part_decoding;
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
            Nres=num; //коли н дойде до 0, обновити табл
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
        if(keyid=="0"){
            qDebug()<<"Bad ID";
            return;

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
                config_change_enable=false;
                change_conf();
            }

        }
     }
    if(part_heder=="send")
    {
        QString name,rate,rate2,course , year ,sem ,inst,n1,n2,n3;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                name.append(command[i]);
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
                n1.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                n2.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                n3.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        table_records temporary;
        QStringList parts;
        parts = name.split(' ');
        temporary.fname=parts[0];
        temporary.lname=parts[1];
        temporary.pname=parts[2];
        temporary.rate=rate;
        temporary.rate2=rate2;
        temporary.course=course;
        temporary.year=year;
        temporary.sem=sem;
        temporary.inst=inst;
        temporary.s1=n1;
        temporary.s2=n2;
        temporary.s3=n3;
        if(Nres>0)
        {
           My_Table.insert(My_Table.size()-1, temporary);
        }
        Nres--;
        qDebug()<<Nres;
        if(Nres==0){
            ui->tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
            update_table();
        }
    }
}

void WinMain::on_prev_botton_clicked()
{
    if(now > 0){
        now-=10;
        next-=10;
        ui->tableWidget->clearContents();
        ui->tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
        qDebug()<<"Now= "<<now<<"Next= "<<next;
        update_table();
    }
}
void WinMain::on_next_button_clicked()
{

    if(next < My_Table.size()){
        now+=10;
        next+=10;
        ui->tableWidget->clearContents();
        ui->tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
        qDebug()<<"Now= "<<now<<"Next= "<<next;
        update_table();
    }

}

QString WinMain:: convert_conf(){
    QString result,temp; // course,inst,year
    temp.setNum(MyKey);
    result="changeconf:"+temp+':';
    temp.setNum(Config.low_r);
    result+=temp+':';
    temp.setNum(Config.hight_r);
    result+=temp+':';
    temp.setNum(Config.low_rate);
    result+=temp+':';
    temp.setNum(Config.hight_rate);
    result+=temp+':';
    if(!Config.sem1 && !Config.sem2) result+="0:";
    else if(Config.sem1 && !Config.sem2) result+="1:";
    else if(!Config.sem1 && Config.sem2) result+="2:";
    else if(Config.sem1 && Config.sem2) result+="3:";
    temp.setNum(Config.sorting_by);
    result+=temp+':';
    int to_convert=0x0;
    if(Config.c4) to_convert+=8;
    if(Config.c3) to_convert+=4;
    if(Config.c2) to_convert+=2;
    if(Config.c1) to_convert+=1;
    temp.setNum(to_convert,16);
    result+=temp+':';
    to_convert=0;
    if(Config.igdg) to_convert+=8;
    if(Config.igsn) to_convert+=4;
    if(Config.ikni) to_convert+=2;
    if(Config.ikta) to_convert+=1;
    temp.setNum(to_convert,16);
    result+=temp+':';
    to_convert=0;
    if(Config._2018) to_convert+=8;
    if(Config._2017) to_convert+=4;
    if(Config._2016) to_convert+=2;
    if(Config._2015) to_convert+=1;
    temp.setNum(to_convert,16);
    result+=temp+';';
    to_convert=0;
    return  result;
}

void WinMain::change_conf()
{
    //low_r,hight_r,low_rate,hight_rate,sem,sorting_by,course,inst,year;
    ui->doubleSpin_from->setValue(Config.low_r);
    ui->doubleSpin_to->setValue(Config.hight_r);
    ui->doubleSpin_from_2->setValue(Config.low_rate);
    ui->doubleSpin_to_2->setValue(Config.hight_rate);
    qDebug()<<"From Change conf"<<Config.low_r<<Config.hight_r<<Config.low_rate<<Config.hight_rate;
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
    config_change_enable=true;
}
void WinMain:: config_change()
{
    if(config_change_enable)
    {
    Config.low_r=ui->doubleSpin_from->value();
    Config.hight_r=ui->doubleSpin_to->value();
    Config.low_rate=ui->doubleSpin_from_2->value();
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
    Config._2018=ui->action2018->isChecked(); //можливо буде доповнення до цієї функції, мб для темп масиву значень.. мб
    }
}

void WinMain::on_comboBox_currentIndexChanged(int index)
{
    config_change();
    //рейтинг - 0, бал - 1, ім - 2
}

void WinMain::on_search_name_clicked()
{
    qDebug()<<"Size of Table="<<My_Table.size();
    ui->tableWidget->clearContents();
    now=0; next=9;
    My_Table.clear();
    QTextStream send(sok);
    send.setCodec("UTF-8");
    //QTextStream str(mTcpSocket);
    //str.setCodec("CP-1251");CP=Windows
    //str.setCodec("UTF-8");
    QString s;
    if(Config!=Sended_conf)
    {
        Sended_conf=Config;
        My_Table.clear();
        s=convert_conf();
        send<<s; send<<flush;
    }
    s.clear();
    //QString size_of_get,sort_num,name;
    //s="get:"+num_need_records+':'+last_elem_num_of_sort+':'+line_edit.text()+';';   #-якщо поле пусте
    qDebug()<<ui->lineEdit->text().size()<<"Size of name";
    s="get:10:";
    send<<s;
    if(Config.sorting_by==0 && My_Table.size()!=0)
    {
        send<<My_Table[My_Table.size()-1].s1+':';
    }
    else if(Config.sorting_by==1  && My_Table.size()!=0)
    {
        send<<My_Table[My_Table.size()-1].s2+':';
    }
    else  if(Config.sorting_by==2  && My_Table.size()!=0)
    {
        send<<My_Table[My_Table.size()-1].s3+':';
    }
    else {send<<"0:";}
    if(ui->lineEdit->text().size()>0)
            s=ui->lineEdit->text()+';';
    else
            s="#;";
    send<<s;
    send<<flush;
}

void WinMain::on_action2015_changed()
{
    config_change();
}

void WinMain::on_action2016_changed()
{
    config_change();
}

void WinMain::on_action2017_changed()
{
    config_change();
}

void WinMain::on_action2018_changed()
{
    config_change();
}

void WinMain::on_sem1_changed()
{
    config_change();
}

void WinMain::on_sem2_changed()
{
    config_change();
}

void WinMain::on_igdg_changed()
{
    config_change();
}

void WinMain::on_igsn_changed()
{
    config_change();
}

void WinMain::on_ikni_changed()
{
    config_change();
}

void WinMain::on_ikta_changed()
{
    config_change();
}

void WinMain::on_course1_changed()
{
    config_change();
}

void WinMain::on_course2_changed()
{
    config_change();
}

void WinMain::on_course3_changed()
{
    config_change();
}

void WinMain::on_course4_changed()
{
    config_change();
}

void WinMain::on_doubleSpin_from_valueChanged(double arg1)
{
    config_change();
}

void WinMain::on_doubleSpin_to_valueChanged(double arg1)
{
    config_change();
}

void WinMain::on_doubleSpin_from_2_valueChanged(double arg1)
{
    config_change();
}

void WinMain::on_doubleSpin_to_2_valueChanged(double arg1)
{
    config_change();
}


