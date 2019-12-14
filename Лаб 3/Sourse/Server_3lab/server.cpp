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
    //спроба конекту
    //QSqlDatabase db = QSqlDatabase::addDatabase( "QODBC3" );

    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={SQL Server};SERVER=DESKTOP-RAPBD2P;Database=IPZ;");
    db.setUserName("admin");
    db.setPassword("admin");


    //db.setDatabaseName( "Driver={SQL Server};SERVERNODE=localhost;Database=Lab2;Trusted_Connection=yes;" );
    //"Server=MSSQL1;Database=AdventureWorks;Integrated Security=true;    DRIVER={SQL Native Client};"
    // QODBC QSQLITE
        if (!db.open())
        {
             qDebug()<<"!&737hhoijwefaf";
        }
        else{
            qDebug()<<"Con?";
            QSqlQuery query;
            query.exec("SELECT * FROM Records");
            //query.next();
            while (query.next()) {
                     int id = query.value(0).toInt();
                     QString name = query.value(1).toString();
                     QString lastname = query.value(2).toString();
                     QString l = query.value(3).toString();
                     for(int i =0;i<lastname.size();i++){
                         if(lastname[i]==',')
                            {
                             lastname[i]='.';
                             break;
                            }
                         //я напартачив з записами в самій бд, буду конвертити тут)
                     }
                     qDebug() << id << name<<lastname<<l<<lastname.toDouble()<<l.toDouble();
                     lastname.clear();
                 }

        }
        /*
        QString str = "/a/b/c/";
        QStringList parts;

        parts = str.split('/'); //розбивати строку ввот так, мб потом замінити цей метод для обробки запитів? auto
        //while (parts.size >1 <5
        parts.erase( parts.begin());

        QString temp;
        temp=parts[0];
        qDebug()<<parts<< temp;     parts.erase( parts.begin());
        qDebug()<<parts<<parts.size();
        parts.erase( parts.begin());
        qDebug()<<parts<<parts.size();
        parts.erase( parts.begin());
        qDebug()<<parts<<parts.size();
        parts.erase( parts.begin());

        //qDebug()<<parts[0];
        //parts.erase( parts.begin());
       // qDebug()<<parts[0];
       */
}

QList <Server:: Msok> Table; //табл дескрипт
//ліст законекчених юзерів їхні дексриптори, да ще одна табл бо вже тут будуть тимчасові налаштування, проміжні табл
// і можливо інша інфа.. пока в голову не приходить, жах, тре закривтаи інші дири




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
            QSqlQuery query;
            qDebug()<<"Start find login";
            if(db.isOpen())
            {
                qDebug()<<"select * from Users where Name='"+log+"'";
                query.exec("select * from Users where Name='"+log+"'");
                if(query.next()){
                    if(query.value(2).toString()==pass)
                    {
                        send<<"rfind:1:1;";
                        send<<flush;

                    }else{
                        send<<"rfind:1:0;";
                        send<<flush;
                    }
                }
                else{
                    send<<"rfind:0:0;";
                    send<<flush;
                }
            }
            else{
                qDebug()<<"cant find! db doesnt cennect";
                send<<"rfind:0:0;";
                send<<flush;

            }

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
            QSqlQuery query;
            qDebug()<<"Start find";
            if(db.isOpen())
            {
                qDebug()<<"select * from Users where Name='"+user+"'";
                query.exec("select * from Users where Name='"+user+"'");
                if(query.next()){
                    send<<"rfind:user:1;";
                    send<<flush;
                }
                else{
                    send<<"rfind:user:0;";
                    send<<flush;
                }
            }
            else{
                qDebug()<<"cant find! db doesnt cennect";
                send<<"rfind:user:1;";
                send<<flush;

            }
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
            QTextStream send(tmp);
            send<<"rlogin:1:333;";
            send<<flush;
            QSqlQuery query;
            qDebug()<<"Start find login";
            if(db.isOpen())
            {
                qDebug()<<"select * from Users where Name='"+login+"'";
                query.exec("select * from Users where Name='"+login+"'");
                if(query.next()){
                    if(query.value(2).toString()==pass)
                    {
                        send<<"rlogin:1:"+query.value(0).toString()+';';
                        send<<flush;
                        //залогінити, занести в список юзерів,
                    }
                    else
                    {
                        send<<"rfind:1:0;";
                        send<<flush;
                    }
                }
                else{
                    send<<"rfind:0:0;";
                    send<<flush;
                }
            }
            else{
                qDebug()<<"cant login! db doesnt cennect";
                send<<"rlogin:0:0;";
                send<<flush;

            }
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
            QTextStream send(tmp);
            send<<"rreg:1:666;";
            send<<flush;
            QSqlQuery query;
            int new_id;
            qDebug()<<"reg this user"+user;
            if(db.isOpen())
            {
                qDebug()<<"select * from Users where Name='"+user+"'";
                query.exec("select * from Users where Name='"+user+"'");
                if(query.next()){
                       send<<"rreg:0:0;";
                       send<<flush;
                }
                else{
                    //":67.345:89.567:3.45:4.48:2:1:A:8:8;"
                    query.exec("select * from Users");
                    if(query.last()){
                        new_id=query.value(0).toInt()+1; //VALUES (1001, 'Thad Beaumont', 65000)
                        QString temp; temp.setNum(new_id);
                        QString new_query="insert into Users values("+temp+",'"+user+"','"
                                +pass+"',':67.345:89.567:3.45:4.48:2:1:A:8:8;')";
                        query.exec(new_query);
                        send<<"rreg:1:";
                        send<<new_id;
                        send<<";";
                        send<<flush;

                    }
                    else{
                        qDebug()<<"empty Users!";
                    }
                }
            }
            else{
                qDebug()<<"db doesnt cennect";
                send<<"rreg:0:0;";
                send<<flush;

            }
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
            QString default_conf="putconf:"+key+":67.345:89.567:3.45:4.48:2:1:A:8:8;";
            QTcpSocket *tmp=nullptr;
            for(int i=0; i<Table.size();i++)
            {
                if(Table[i].desc==descriptor) {
                    tmp=Table[i].sok;
                    break;
                }
            }
            QTextStream send(tmp);
            QSqlQuery query;
            qDebug()<<"Send Conf";
            if(db.isOpen())
            {
                qDebug()<<"select * from Users where ID='"+key+"'";
                query.exec("select * from Users where ID='"+key+"'");
                if(query.next()){
                   send<<"putconf:"+key+query.value(3).toString();
                   send<<flush;
                }
                else{
                    send<<default_conf;
                    send<<flush;
                }
            }
            else{
                qDebug()<<"db doesnt cennect";
                send<<"putconf:0:00.999:00.999:0.99:0.99:0:0:0:0:0;";
                send<<flush;
            }
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
