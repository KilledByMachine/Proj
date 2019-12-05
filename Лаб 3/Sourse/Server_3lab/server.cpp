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

QList <Server:: Msok> Table;
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
    str <<s;
    s="text";
    str <<s;
    str<<flush;
    qDebug() << "New Connect";    
    connect(NewUser.sok, SIGNAL(disconnected()), this, SLOT(slotClientDisconnected()));
    connect(NewUser.sok,SIGNAL(readyRead()),this,SLOT(get_data()));
}


void Server::slotClientDisconnected()
{
    int Num=-1;
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
    // організувати видалення тоги чи іншого клієнта +++++

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
    //qDebug()<<temp;
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
            //rfind:user

        }
        else{
               qDebug()<<part_heder<<"?";
        }
    }

    //qDebug()<<part_heder<<" "<<comand[pos]<<comand.size()<<" "<<comand[comand.size()-1];


}
