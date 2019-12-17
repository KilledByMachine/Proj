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
            query.exec("SELECT * FROM Records order by Name");
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
                     //qDebug() << id << name<<lastname<<l<<lastname.toDouble()<<l.toDouble();
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

QList <Server:: Msok> Table;        //табл дескрипт
QList <table_records> Temp_table;  //тимчасово зберіг запити
QList <user_list> Users_conf;     //актуальні налаштування клієнтів + дескриптори
//ліст законекчених юзерів їхні дексриптори, да ще одна табл бо вже тут будуть тимчасові налаштування, проміжні табл
// і можливо інша інфа.. пока в голову не приходить, жах, тре закривтаи інші дири



// створити окремий ліст, з огромними строками і своїми дескрипторами, через які вже конкретним користувачам будуть
// відправлятись їх результати (на той чи іншй запит)
void Server::slotNewConnection()
{

    mTcpSocket = mTcpServer->nextPendingConnection();
    QTextStream str(mTcpSocket);
    //str.setCodec("CP-1251");CP=Windows
    //str.setCodec("UTF-8");
    Msok NewUser;
    NewUser.desc=mTcpSocket->socketDescriptor();
    NewUser.sok=mTcpSocket;

    Table.insert(Table.begin(),NewUser);
    //str.setCodec("UTF-8");
    //QString s="Текст, text";
    //QString s="send:First : Name : par :69.765:4.56:2:2017:I:IKNI:1234;";
    //s="text";
    //str <<s;
    //str<<flush;
    qDebug() << "New Connect";
    qDebug() <<mTcpSocket->socketDescriptor();
    connect(NewUser.sok, SIGNAL(disconnected()), this, SLOT(slotClientDisconnected()));
    connect(NewUser.sok,SIGNAL(readyRead()),this,SLOT(get_data()));
}


void Server::slotClientDisconnected()
{
    int Num=-1,Desc=0;
    qDebug()<<"DISC";
    QTcpSocket* clientSocket = static_cast< QTcpSocket* >(sender());
    clientSocket->close();
    for(int i=0;i<Table.size() && !Table.isEmpty();i++)
    {
        qDebug()<<i<<"-- I"<<Table.size();
        qDebug()<<"Table desc: "<<Table[i].desc;
        if(Table[i].sok->socketDescriptor()==-1) {
            Num=i;
            Desc=Table[i].desc;
        }

    }
    if(!Table.isEmpty())
            {Table.removeAt(Num);}
    //bool conf_valid=false;
    qDebug()<<Desc;
    qDebug()<<Desc<<"--after cirle desc";
    for(int i=0;i<Users_conf.size()-1 && !Users_conf.isEmpty();i++)
    {
        qDebug()<<"User desc: "<<Users_conf[i].desc;
        if(Users_conf[i].desc==Desc)
        {
            qDebug()<<Users_conf[i].desc<<"--Desc";
            Num=i;

        }
    }
    if(db.isOpen() && !Users_conf.isEmpty()){
        //ТРАБЛ
        QString updated_conf =convert_conf(Num);
        QSqlQuery query;
        QString temp; temp.setNum(Users_conf[Num].Conf.user_id);
        qDebug()<<updated_conf<<temp<<"Try save conf";
        query.exec("UPDATE  Users SET Config='"+updated_conf+"' WHERE ID="+temp);
        qDebug()<<query.lastError();
    }
    else{
        qDebug()<<"DB doesnt avalibel, cant save config!";
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
    qDebug()<<"Server get:"<<temp;
    QString part_decoding;
    part_decoding.clear();
    for(int i=0;i<temp.size();i++){
        if(temp[i]==';'){
            qDebug()<<"To Decoding: "<<part_decoding+';';
            decoding(part_decoding+';',serverSocket->socketDescriptor());
            part_decoding.clear();
        }
        else{
            part_decoding.append(temp[i]);
        }
    }
    //decoding(temp,serverSocket->socketDescriptor());

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
                bool ok=query.exec("select * from Users where Name='"+log+"'");
                if(query.next() && ok){
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
            QSqlQuery query;
            qDebug()<<"Start find login";
            if(db.isOpen())
            {
                qDebug()<<"select * from Users where Name='"+login+"'";
                query.exec("select * from Users where Name='"+login+"'");
                if(query.next()){
                     if(query.value(2).toString()==pass)
                    {

                        qDebug()<<"Такий                 ";
                        QString keyid=query.value(0).toString();

                        //залогінити, занести в список юзерів,
                        user_list temp_user_list;
                        temp_user_list.desc=descriptor;
                        //якщо такий дексриптор активний, тобт є, то не робити інсерту
                        bool is_hear=false; int num=0;
                        for(int i=0;i<Users_conf.size() && !Users_conf.isEmpty();i++){
                            if(Users_conf[i].Conf.user_id==keyid)
                               {
                                is_hear=true;
                                num=i;
                               }
                        }
                        if(!is_hear) {
                            Users_conf.insert(Users_conf.begin(),temp_user_list);
                        }
                        else{
                            Users_conf[num].desc=descriptor;
                        }
                        //вносимо з пустими налаштуваннями, при першому запиті вони запишуться
                        QString low_r,hight_r,low_rate,hight_rate,sem,sorting_by,course,inst,year;
                        QString string_to_convert=query.value(3).toString();
                        qDebug()<<string_to_convert;
                        string_to_convert.chop(1);
                        QStringList parts=string_to_convert.split(':');
                        qDebug()<<"Size of parts in login: "<<parts.size();
                        qDebug()<<parts;
                        //ТРАБЛ
                        //keyid=parts[0];
                        low_r=parts[0];
                        hight_r=parts[1];
                        low_rate=parts[2];
                        hight_rate=parts[3];
                        sem=parts[4];
                        sorting_by=parts[5];
                        course=parts[6];
                        inst=parts[7];
                        year=parts[8];          //а тепер конверти

                        bool convert; int to_convert=0;
                        my_config temp;
                        temp.user_id=keyid.toInt();
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
                            for(int i=0; i<Users_conf.size() && !Users_conf.isEmpty();i++)
                            {
                                if(Users_conf[i].desc==descriptor) {
                                    Users_conf[i].Conf=temp;
                                    qDebug()<<"Config saved!!!";
                                    break;
                                }
                            }

                        }
                        qDebug()<<"sended login 1 1";
                        send<<"rlogin:1:"+query.value(0).toString()+';';
                        send<<flush;
                    }
                }
                else{
                    send<<"rlogin:0:0;";
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
                        QString temp_1; temp_1.setNum(new_id);
                        QString new_query="insert into Users values("+temp_1+",'"+user+"','"
                                +pass+"','67.345:89.567:3.45:4.48:2:1:A:8:8;')";
                        //ТРАБЛ поход мусорниий код папався під ногу
                        query.exec(new_query);
                        //send<<"rreg:1:";
                        //send<<new_id;
                        //send<<";";
                        //send<<flush;
                        //збереження налаштувань на сервері
                            //send<<"rlogin:1:"+query.value(0).toString()+';';
                            //send<<flush;
                            //залогінити, занести в список юзерів,
                            user_list temp_user_list;
                            temp_user_list.desc=descriptor;
                            qDebug()<<665<<Users_conf.size()<<Users_conf.isEmpty();
                            Users_conf.insert(Users_conf.end(),temp_user_list);
                            qDebug()<<665<<Users_conf.size()<<Users_conf.isEmpty();
                            //вносимо з пустими налаштуваннями, при першому запиті вони запишуться
                            QString low_r,hight_r,low_rate,hight_rate,sem,sorting_by,course,inst,year;
                            QString string_to_convert="67.345:89.567:3.45:4.48:2:1:A:8:8;";
                            string_to_convert.chop(1);
                            QStringList parts=string_to_convert.split(':');
                            //keyid=parts[0];
                            low_r=parts[0];
                            hight_r=parts[1];
                            low_rate=parts[2];
                            hight_rate=parts[3];
                            sem=parts[4];
                            sorting_by=parts[5];
                            course=parts[6];
                            inst=parts[7];
                            year=parts[8];          //а тепер конверти

                            bool convert; int to_convert=0;
                            my_config temp;
                            temp.user_id=new_id;
                            int Key =new_id;                       //keyid.toInt(&convert,10);
                            //if(!convert) { qDebug()<<"Bad keyID"; return;}
                            temp.user_id=Key;
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
                            temp.is_connected=true;
                            qDebug()<<665<<Users_conf.size()<<Users_conf.isEmpty();
                            if(!convert){ qDebug()<<"Bad convert"; return;}
                            else
                            {   //ТРАБЛ
                                qDebug()<<665<<Users_conf.size()<<Users_conf.isEmpty();
                                for(int i=0; i<Users_conf.size() && !Users_conf.isEmpty();i++)
                                {
                                    if(Users_conf[i].desc==descriptor) {
                                        Users_conf[i].Conf=temp;

                                        qDebug()<<"Config saved!!!";
                                        //на крайняк сюда
                                        break;
                                    }
                                }
                            }
                            send<<"rreg:1:";
                            send<<new_id;
                            send<<";";
                            send<<flush;
                            //сюда пока поставлю

                        }
                    else
                    {
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
            QString default_conf="putconf:"+key+":67.345:89.567:3.45:4.48:2:1:A:8:8;";
            QTcpSocket *tmp=nullptr;
            for(int i=0; i<Table.size();i++)
            {
                if(Table[i].desc==descriptor) {
                    tmp=Table[i].sok;
                    break;
                }
            }
            qDebug()<<"get con f"<<Users_conf.size()<<Users_conf.isEmpty();
            qDebug()<<"get con f"<<Users_conf[0].desc;
            for(int i=0; i<Users_conf.size() && !Users_conf.isEmpty();i++){
                if(Users_conf[i].Conf.user_id==key.toInt()) {
                    Users_conf[i].desc=descriptor;
                    qDebug()<<"get con f"<<Users_conf[0].desc;
                    break;
                }
            }
            QTextStream send(tmp);
            QSqlQuery query;
            qDebug()<<"Send Conf"<<"Desc ----"<<descriptor;
            if(db.isOpen())
            {
                qDebug()<<"select * from Users where ID='"+key+"'";
                query.exec("select * from Users where ID='"+key+"'");
                if(query.next()){
                   qDebug()<<"putconf:"+key+':'+query.value(3).toString();
                   send<<"putconf:"+key+':'+query.value(3).toString();
                   send<<flush;
                }
                else{
                    //send<<default_conf;
                    //send<<flush;
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
            qDebug()<<size_of_get.size()<<sort_num.size()<<name.size();
            return;
        }
        else {
            qDebug()<<"Its GET"<<size_of_get<<" "<<sort_num<<" "<<name;
            //передача цих даних в фкцію створення запиту в бд
            int num=size_of_get.toInt(),j;
            QTcpSocket *tmp=nullptr;
            for(int i=0; i<Table.size();i++)
            {
                if(Table[i].desc==descriptor) {
                    tmp=Table[i].sok;
                    break;
                }
            }
            my_config Conf;
            // ТРАБЛ Table-> Users-conf
            for(int i=0; i<Users_conf.size();i++)
            {
                if(Users_conf[i].desc==descriptor) {
                    Conf=Users_conf[i].Conf;
                    j=i;
                    break;
                }
            }

            QTextStream send(tmp);
            send.setCodec("UTF-8");
            QSqlQuery query;
            QString select, temp_select;
            select= "select * from Records where Rate_r > '";
            temp_select.setNum( Conf.low_r); select+=temp_select+"' and Rate_r < '";
            temp_select.setNum(Conf.hight_r-0.001); select+=temp_select+"' and Rate_b > '";
            temp_select.setNum(Conf.low_rate); select+=temp_select+"'  and Rate_b < '";
            temp_select.setNum(Conf.hight_rate-0.001); select+=temp_select+"' ";
            //хедер з діпазонами готов
            int temp_num=0;
            if(Conf.sem1 && Conf.sem2){
                temp_select="   ";
            }
            else if(Conf.sem1){
                temp_select=" and Semester='1'  ";
            }
            else if(Conf.sem2){
                temp_select=" and Semester='2'  ";
            }
            select+=temp_select;
            if(Conf.c1 && Conf.c2 && Conf.c3 && Conf.c4){
                temp_select="   ";
            }
            else {
                //c1-c4
                // щас тут будуть перебори через іф, 16 варіантів.. в 3х екземплярах
                if(!Conf.c1 && !Conf.c2 && !Conf.c3 && !Conf.c4)
                {
                    temp_select=" and Course='5' ";
                }
                if(!Conf.c1 && !Conf.c2 && !Conf.c3 && Conf.c4)
                {
                    temp_select=" and Course='4' ";
                }
                if(!Conf.c1 && !Conf.c2 && Conf.c3 && !Conf.c4)
                {
                    temp_select=" and Course='3' ";
                }
                if(!Conf.c1 && !Conf.c2 && Conf.c3 && Conf.c4)
                {
                    temp_select=" and (Course='3' or Course='3')  ";
                }
                if(!Conf.c1 && Conf.c2 && !Conf.c3 && !Conf.c4)
                {
                    temp_select=" and Course='2' ";
                }
                if(!Conf.c1 && Conf.c2 && !Conf.c3 && Conf.c4)
                {
                    temp_select=" and (Course='2' or Course='4') ";
                }
                if(!Conf.c1 && Conf.c2 && Conf.c3 && !Conf.c4)
                {
                    temp_select=" and (Course='2' or Course='3') ";
                }
                if(!Conf.c1 && Conf.c2 && Conf.c3 && Conf.c4)
                {
                    temp_select=" and ( Course='2' or Course='3' or Course='4') ";
                }
                if(Conf.c1 && !Conf.c2 && !Conf.c3 && !Conf.c4)
                {
                    temp_select=" and Course='1'  ";
                }
                if(Conf.c1 && !Conf.c2 && !Conf.c3 && Conf.c4)
                {
                    temp_select=" and (Course='1' or Course='4') ";
                }
                if(Conf.c1 && !Conf.c2 && Conf.c3 && !Conf.c4)
                {
                    temp_select=" and ( Course='1' or Course='3' ) ";
                }
                if(Conf.c1 && !Conf.c2 && Conf.c3 && Conf.c4)
                {
                    temp_select=" and ( Course='1' or Course='3' or Course='4') ";
                }
                if(Conf.c1 && Conf.c2 && !Conf.c3 && !Conf.c4)
                {
                    temp_select=" and ( Course='1' or Course='2' ) ";
                }
                if(Conf.c1 && Conf.c2 && !Conf.c3 && Conf.c4)
                {
                    temp_select=" and ( Course='1' or Course='2' or Course='4') ";
                }
                if(Conf.c1 && Conf.c2 && Conf.c3 && !Conf.c4)
                {
                    temp_select=" and ( Course='1' or Course='2' or Course='3') ";
                }

            }
            select+=temp_select;
            if(Conf.igdg && Conf.igsn && Conf.ikni && Conf.ikta){
                temp_select="   ";
            }
            else {
                if(!Conf.igdg && !Conf.igsn && !Conf.ikni && !Conf.ikta)
                {
                    temp_select=" and Inst='aaaaa' ";
                }
                if(!Conf.igdg && !Conf.igsn && !Conf.ikni && Conf.ikta)
                {
                    temp_select=" and Inst='ІКТА' ";
                }
                if(!Conf.igdg && !Conf.igsn && Conf.ikni && !Conf.ikta)
                {
                    temp_select=" and Inst='ІКНІ' ";
                }
                if(!Conf.igdg && !Conf.igsn && Conf.ikni && Conf.ikta)
                {
                    temp_select=" and (Inst='ІКНІ' or Inst='ІКТА')  ";
                }
                if(!Conf.igdg && Conf.igsn && !Conf.ikni && !Conf.ikta)
                {
                    temp_select=" and Inst='ІГСН' ";
                }
                if(!Conf.igdg && Conf.igsn && !Conf.ikni && Conf.ikta)
                {
                    temp_select=" and (Inst='ІГСН' or Inst='ІКТА') ";
                }
                if(!Conf.igdg && Conf.igsn && Conf.ikni && !Conf.ikta)
                {
                    temp_select=" and (Inst='ІГСН' or Inst='ІКНІ') ";
                }
                if(!Conf.igdg && Conf.igsn && Conf.ikni && Conf.ikta)
                {
                    temp_select=" and ( Inst='ІГСН' or Inst='ІКНІ' or Inst='ІКТА') ";
                }
                if(Conf.igdg && !Conf.igsn && !Conf.ikni && !Conf.ikta)
                {
                    temp_select=" and Inst='ІГДГ'  ";
                }
                if(Conf.igdg && !Conf.igsn && !Conf.ikni && Conf.ikta)
                {
                    temp_select=" and (Inst='ІГДГ' or Inst='ІКТА') ";
                }
                if(Conf.igdg && !Conf.igsn && Conf.ikni && !Conf.ikta)
                {
                    temp_select=" and ( Inst='ІГДГ' or Inst='ІКНІ' ) ";
                }
                if(Conf.igdg && !Conf.igsn && Conf.ikni && Conf.ikta)
                {
                    temp_select=" and ( Inst='ІГДГ' or Inst='ІКНІ' or Inst='ІКТА') ";
                }
                if(Conf.igdg && Conf.igsn && !Conf.ikni && !Conf.ikta)
                {
                    temp_select=" and ( Inst='ІГДГ' or Inst='ІГСН' ) ";
                }
                if(Conf.igdg && Conf.igsn && !Conf.ikni && Conf.ikta)
                {
                    temp_select=" and ( Inst='ІГДГ' or Inst='ІГСН' or Inst='ІКТА') ";
                }
                if(Conf.igdg && Conf.igsn && Conf.ikni && !Conf.ikta)
                {
                    temp_select=" and ( Inst='ІГДГ' or Inst='ІГСН' or Inst='ІКНІ') ";
                }

            }
            select+=temp_select;
            if(Conf._2018 && Conf._2017 && Conf._2016 && Conf._2015){
                temp_select="   ";
            }
            else {
                if(!Conf._2018 && !Conf._2017 && !Conf._2016 && !Conf._2015)
                {
                    temp_select=" and Year_='aaaaa' ";
                }
                if(!Conf._2018 && !Conf._2017 && !Conf._2016 && Conf._2015)
                {
                    temp_select=" and Year_='2015' ";
                }
                if(!Conf._2018 && !Conf._2017 && Conf._2016 && !Conf._2015)
                {
                    temp_select=" and Year_='2016' ";
                }
                if(!Conf._2018 && !Conf._2017 && Conf._2016 && Conf._2015)
                {
                    temp_select=" and (Year_='2016' or Year_='2015')  ";
                }
                if(!Conf._2018 && Conf._2017 && !Conf._2016 && !Conf._2015)
                {
                    temp_select=" and Year_='2017' ";
                }
                if(!Conf._2018 && Conf._2017 && !Conf._2016 && Conf._2015)
                {
                    temp_select=" and (Year_='2017' or Year_='2015') ";
                }
                if(!Conf._2018 && Conf._2017 && Conf._2016 && !Conf._2015)
                {
                    temp_select=" and (Year_='2017' or Year_='2016') ";
                }
                if(!Conf._2018 && Conf._2017 && Conf._2016 && Conf._2015)
                {
                    temp_select=" and ( Year_='2017' or Year_='2016' or Year_='2015') ";
                }
                if(Conf._2018 && !Conf._2017 && !Conf._2016 && !Conf._2015)
                {
                    temp_select=" and Year_='2018'  ";
                }
                if(Conf._2018 && !Conf._2017 && !Conf._2016 && Conf._2015)
                {
                    temp_select=" and (Year_='2018' or Year_='2015') ";
                }
                if(Conf._2018 && !Conf._2017 && Conf._2016 && !Conf._2015)
                {
                    temp_select=" and ( Year_='2018' or Year_='2016' ) ";
                }
                if(Conf._2018 && !Conf._2017 && Conf._2016 && Conf._2015)
                {
                    temp_select=" and ( Year_='2018' or Year_='2016' or Year_='2015') ";
                }
                if(Conf._2018 && Conf._2017 && !Conf._2016 && !Conf._2015)
                {
                    temp_select=" and ( Year_='2018' or Year_='2017' ) ";
                }
                if(Conf._2018 && Conf._2017 && !Conf._2016 && Conf._2015)
                {
                    temp_select=" and ( Year_='2018' or Year_='2017' or Year_='ІКТА') ";
                }
                if(Conf._2018 && Conf._2017 && Conf._2016 && !Conf._2015)
                {
                    temp_select=" and ( Year_='2018' or Inst='ІГСН' or Inst='2016') ";
                }


            }
            //рейтинг - 0, бал - 1, ім - 2
            select+=temp_select;
            if(name!="#"){
                temp_select="and Name LIKE '%"+name+"%'";
                select+=temp_select;
            }
            if(Conf.sorting_by==0){
                temp_select =" and Sort_by_r>"+sort_num;
                temp_select =" order by Sort_by_r ";
            }
            else if(Conf.sorting_by==1) {
                temp_select =" and Sort_by_b>"+sort_num;
                temp_select =" order by Sort_by_b ";
            }
            else if(Conf.sorting_by==2) {
                temp_select =" and Sort_by_name>"+sort_num;
                temp_select =" order by Sort_by_name ";
            }
            select+=temp_select;
            qDebug()<<select;
            query.exec(select);
            qDebug()<<query.lastError();
            //query.exec("SELECT * FROM Records");                //вні буде налаштовуватись в відповідності до конфіга
            //а тут будуть варіації запитів відносно налаштувань
            table_records Temp;
            while (query.next() ) {
                num--;
                Temp.name=query.value(1).toString()+':';
                Temp.rate=query.value(2).toString()+':';
                Temp.rate2=query.value(3).toString()+':';
                Temp.course=query.value(4).toString()+':';
                Temp.year=query.value(5).toString()+':';
                Temp.sem=query.value(6).toString()+':';
                Temp.inst=query.value(7).toString()+':';
                Temp.s1=query.value(8).toString()+':';
                Temp.s2=query.value(9).toString()+':';
                Temp.s3=query.value(10).toString()+';';
                Temp_table.insert(Temp_table.size(),Temp);
            }
            send<<"nres:";
            send<<Temp_table.size();
            send<<";";
            send<<flush;
            for(int i=0; i<Temp_table.size();i++){
                send<<"send:";
                send<<Temp_table[i].name;
                send<<Temp_table[i].rate;
                send<<Temp_table[i].rate2;
                send<<Temp_table[i].course;
                send<<Temp_table[i].year;
                send<<Temp_table[i].sem;
                send<<Temp_table[i].inst;
                send<<Temp_table[i].s1;
                send<<Temp_table[i].s2;
                send<<Temp_table[i].s3;
                send<<flush;
            }
            Temp_table.clear();

        }
    //qDebug()<<part_heder<<" "<<comand[pos]<<comand.size()<<" "<<comand[comand.size()-1];

        }
    if(part_heder=="changeconf")
    {
        qDebug()<<"Im hear!!!!!!!!!!!"<< descriptor;
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
        //qDebug()<<keyid<<low_r<<hight_r<<low_rate<<hight_rate<<sem<<sorting_by<<course<<inst<<year;
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
            //qDebug()<<keyid<<low_r<<hight_r<<low_rate<<hight_rate<<sem<<sorting_by<<course<<inst<<year;
            bool convert; int to_convert=0;
            my_config temp;
            int Key = keyid.toInt(&convert,10);
            if(!convert) { qDebug()<<"Bad keyID"; return;}
            temp.user_id=Key;
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
                for(int i=0; i<Users_conf.size();i++)
                {
                    if(Users_conf[i].desc==descriptor) {
                        Users_conf[i].Conf=temp;
                        qDebug()<<"Config saved!!!";
                        break;
                    }
                }

            }


        }
    //qDebug()<<part_heder<<" "<<comand[pos]<<comand.size()<<" "<<comand[comand.size()-1];

     }
}

QString Server:: convert_conf(int number_of_conf){
    QString result,temp; // course,inst,year
    result.clear();
    temp.setNum(Users_conf[number_of_conf].Conf.low_r);
    result+=temp+':';
    temp.setNum(Users_conf[number_of_conf].Conf.hight_r);
    result+=temp+':';
    temp.setNum(Users_conf[number_of_conf].Conf.low_rate);
    result+=temp+':';
    temp.setNum(Users_conf[number_of_conf].Conf.hight_rate);
    result+=temp+':';
    if(!Users_conf[number_of_conf].Conf.sem1 && !Users_conf[number_of_conf].Conf.sem2) result+="0:";
    else if(Users_conf[number_of_conf].Conf.sem1 && !Users_conf[number_of_conf].Conf.sem2) result+="1:";
    else if(!Users_conf[number_of_conf].Conf.sem1 && Users_conf[number_of_conf].Conf.sem2) result+="2:";
    else if(Users_conf[number_of_conf].Conf.sem1 && Users_conf[number_of_conf].Conf.sem2) result+="3:";
    temp.setNum(Users_conf[number_of_conf].Conf.sorting_by);
    result+=temp+':';
    int to_convert=0x0;
    if(Users_conf[number_of_conf].Conf.c4) to_convert+=8;
    if(Users_conf[number_of_conf].Conf.c3) to_convert+=4;
    if(Users_conf[number_of_conf].Conf.c2) to_convert+=2;
    if(Users_conf[number_of_conf].Conf.c1) to_convert+=1;
    temp.setNum(to_convert,16);
    result+=temp+':';
    to_convert=0;
    if(Users_conf[number_of_conf].Conf.igdg) to_convert+=8;
    if(Users_conf[number_of_conf].Conf.igsn) to_convert+=4;
    if(Users_conf[number_of_conf].Conf.ikni) to_convert+=2;
    if(Users_conf[number_of_conf].Conf.ikta) to_convert+=1;
    temp.setNum(to_convert,16);
    result+=temp+':';
    to_convert=0;
    if(Users_conf[number_of_conf].Conf._2018) to_convert+=8;
    if(Users_conf[number_of_conf].Conf._2017) to_convert+=4;
    if(Users_conf[number_of_conf].Conf._2016) to_convert+=2;
    if(Users_conf[number_of_conf].Conf._2015) to_convert+=1;
    temp.setNum(to_convert,16);
    result+=temp+';';
    to_convert=0;
    return  result;
}
