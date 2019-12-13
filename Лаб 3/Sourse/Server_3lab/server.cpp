#include "server.h"



Server::Server(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &Server::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 6000))
    {
        qDebug() << "server is not started";
    } else
    {
        qDebug() << "server is started";
    }

}

QList <Server:: Msok> Table; //табл дескрипт
// створити окремий ліст, з огромними строками і своїми дескрипторами, через які вже конкретним користувачам будуть
// відправлятись їх результати (на той чи іншй запит)
void Server::slotNewConnection()
{

    mTcpSocket = mTcpServer->nextPendingConnection();
    QTextStream str(mTcpSocket);
    //str.setCodec("CP-1251");CP=Windows
    Msok NewUser;
    NewUser.desc=mTcpSocket->socketDescriptor();
    NewUser.sok=mTcpSocket;

    Table.insert(Table.end(),NewUser);
    str.setCodec("UTF-8");
    QString s="Текст, text";
    //QString s="send:First : Name : par :69.765:4.56:2:2017:I:IKNI:1234;";
    //s="text";
    str <<s;
    str<<flush;
    qDebug() << "New Connect";
    qDebug() <<mTcpSocket->socketDescriptor();
    connect(NewUser.sok, SIGNAL(disconnected()), this, SLOT(slotClientDisconnected()));
    connect(NewUser.sok,SIGNAL(readyRead()),this,SLOT(get_data()));
}


void Server::slotClientDisconnected()
{
    int Num=-1;
    qDebug()<<"DISC";
    QTcpSocket* clientSocket = static_cast< QTcpSocket* >(sender());
    clientSocket->close();
    for(int i=0;i<Table.size();i++)
    {
        if(Table[i].sok->socketDescriptor()==-1) Num=i;
    }
    QString Disc="Disconected num: ";
    Table.removeAt(Num);
    for(int i=0;i<Table.size();i++)
    {
        QTextStream str(Table[i].sok);
        str<<Disc;
        str<<Num;
        str<<flush;
    }
    //потом, організувати обробку запитів, і відправку зворотніх, мб прийдетсья запустити 2-3 клієнта щоби провірити
    //нормлаьно, а саме, щоби не приходила відповідь усім, а тільки нужному клієнту
    //псевдо клієнтам мб зробити нормлаьну форму запитів і тд. на зпити з бд, зробити заглушку
    //(буде посилати одне і те саме декілька раз, щоби нормлаьно пахало, нічо такого)
    //далі.. конект в бд, просто конект, ще її заповнити треба, спочатку вручну спробую, і налаштую запити до неї
    // потом спробую зробити рандомне заповнення

}

void Server:: get_data()
{
    QTcpSocket* serverSocket = static_cast< QTcpSocket* >(sender());
    QString temp = serverSocket->readAll();
    qDebug()<<temp;
    decoding(temp,serverSocket->socketDescriptor());

}

void Server:: decoding(QString command, int descriptor)
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
    if(part_heder=="find")
    {
        QString part2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                part2.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        if(pos>=command.size())     //я ще не все прочитав, кінця ще не було!!!
        {
            qDebug()<<"wrong_command";
            return;
        }
        if(part2=="login")
        {
            QString log,pass;  //тут збережуться лог, пароль
            for (int i=pos;i<command.size();i++)
            {
                if(command[i]==':' || command[i]==';') break;
                else
                   {
                    log.append(command[i]);
                    pos=i;
                }
            }
            pos+=2;
            if(pos>=command.size())     //я ще не все прочитав, кінця ще не було!!!
            {
                qDebug()<<"wrong_command";
                return;
            }

            for (int i=pos;i<command.size();i++)
            {
                if(command[i]==':' || command[i]==';') break;
                else
                   {
                    pass.append(command[i]);
                    //pos=i;  якщо це останій параметр команди, позиція вже не важлива, паролем буде все від : до :/;
                }
            }
            qDebug()<<log<<" "<<pass;            
            QTcpSocket *tmp=nullptr;
            for(int i=0; i<Table.size();i++)
            {
                if(Table[i].desc==descriptor) {
                    tmp=Table[i].sok;
                    break;
                }
            }
            QTextStream send(tmp);
            if(log=="admin" && pass=="admin")
            {   send<<"rfind:1:1;";
                send<<flush;
            }
            else if(log=="admin" && pass!="admin")
            {
                send<<"rfind:1:0;";
                send<<flush;
            }
            else if(log!="admin")
            {
                send<<"rfind:0:0;";
                send<<flush;
            }
            //rfind:login

        }
        else if (part2=="user")
        {
            QString user;
            for (int i=pos;i<command.size();i++)
            {
                if(command[i]==':' || command[i]==';') break;
                else
                   {
                    user.append(command[i]);
                    pos=i;
                }
            }
            qDebug()<<user;
            QTcpSocket *tmp=nullptr;
            for(int i=0; i<Table.size();i++)
            {
                if(Table[i].desc==descriptor) {
                    tmp=Table[i].sok;
                    break;
                }
            }
            QTextStream send(tmp);
            if(user=="admin" )
            {   send<<"rfind:user:1;";
                send<<flush;
            }
            else
            {
                send<<"rfind:user:0;";
                send<<flush;
            }
            //rfind:user

        }
        else{
               qDebug()<<part_heder<<"?";
        }
    }
    if(part_heder=="login")
    {
        QString login,pass;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                login.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                pass.append(command[i]);
                pos=i;
            }
        }
        if(login.size()==0 || pass.size()==0)     //якщо парамтери пусті
        {
            qDebug()<<"no match param";
            return;
        }
        else {
            qDebug()<<login<<""<<pass;
            QTcpSocket *tmp=nullptr;
            for(int i=0; i<Table.size();i++)
            {
                if(Table[i].desc==descriptor) {
                    tmp=Table[i].sok;
                    break;
                }
            }
            QTextStream sendit(tmp);
            sendit<<"rlogin:1:333;";
            sendit<<flush;
            //залогінити, нести в список юзерів,
            //также передати налаштування
        }


    }
    if(part_heder=="reg")
    {
        QString user,pass;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                user.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                pass.append(command[i]);
                pos=i;
            }
        }
        if(user.size()==0 || pass.size()==0)     //якщо парамтери пусті
        {
            qDebug()<<"no match param";
            return;
        }
        else {
            qDebug()<<user<<""<<pass;
            QTcpSocket *tmp=nullptr;
            for(int i=0; i<Table.size();i++)
            {
                if(Table[i].desc==descriptor) {
                    tmp=Table[i].sok;
                    break;
                }
            }
            QTextStream sendit(tmp);
            sendit<<"rreg:1:666;";
            sendit<<flush;
            //зареєструвати
            //также налаштування потрібні дати
        }


    }
    if(part_heder=="getconf")
    {
        QString key;                //за дескриптором створювати табл для юзера, на тимчасові записи
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                key.append(command[i]);
                pos=i;
            }
        }
        if(key.size()==0)
        {
            qDebug()<<"not enough";
        }
        else{
            qDebug()<<key<<" "<<key.toInt();
            //send config
            //QString keyid,low_r,hight_r,low_rate,hight_rate,sem,sorting_by,course,inst,year;
            //зробтити відправку дефолтних, тут щось придумаю, що попаде кароч
            //потім ці налашт будуть зберігатись в бд разом з іменем юзера в форматі строки, при конекті буде отправля
            //тись строка без перевірок
            QString s="putconf:"+key+":67.345:89.567:3.45:4.48:2:1:A:8:8;";
            QTcpSocket *tmp=nullptr;
            for(int i=0; i<Table.size();i++)
            {
                if(Table[i].desc==descriptor) {
                    tmp=Table[i].sok;
                    break;
                }
            }
            QTextStream send(tmp);
            send<<s;
            send<<flush;

        }

    }
    if(part_heder=="get")
    {
        QString size_of_get,sort_num,name;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                size_of_get.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                sort_num.append(command[i]);
                pos=i;
            }
        }
        pos+=2;
        for (int i=pos;i<command.size();i++)
        {
            if(command[i]==':' || command[i]==';') break;
            else
               {
                name.append(command[i]);
                pos=i;
            }
        }
        if(size_of_get.size()==0 || sort_num.size()==0 || name.size()==0)     //якщо парамтери пусті
        {
            qDebug()<<"no match param";
            return;
        }
        else {
            qDebug()<<size_of_get<<" "<<sort_num<<" "<<name;
            //передача цих даних в фкцію створення запиту в бд

        }
    //qDebug()<<part_heder<<" "<<comand[pos]<<comand.size()<<" "<<comand[comand.size()-1];

        }
    if(part_heder=="changeconf")
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
        qDebug()<<keyid<<low_r<<hight_r<<low_rate<<hight_rate<<sem<<sorting_by<<course<<inst<<year;
        // keyid,low_r,hight_r,low_rate,hight_rate,sem,sorting_by,course,inst,year
        if(keyid.size()==0 || low_r.size()==0 || hight_r.size()==0 ||low_rate.size()==0 ||hight_rate.size()==0
                || sem.size()==0 ||sorting_by.size()==0 || course.size()==0 || inst.size()==0 || year.size()==0)
            //якщо парамтери пусті
        {
            qDebug()<<"no match param";
            return;
        }
        else {
            //дофіга перевірок і конвертів з строки в нужні числа
            //передача параметрів в обробку бд, тобто конверт і в якусь set_conf
            qDebug()<<keyid<<low_r<<hight_r<<low_rate<<hight_rate<<sem<<sorting_by<<course<<inst<<year;

        }
    //qDebug()<<part_heder<<" "<<comand[pos]<<comand.size()<<" "<<comand[comand.size()-1];

     }
}
