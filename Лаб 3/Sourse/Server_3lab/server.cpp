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
Server:: Msok  Descriptor[100];
QList <Server:: Msok> Table;
int count =0;
void Server::slotNewConnection()
{

    mTcpSocket = mTcpServer->nextPendingConnection();
    QTextStream str(mTcpSocket);
    //str.setCodec("CP-1251");CP=Windows
    Msok NewUser;
    NewUser.desc=mTcpSocket->socketDescriptor();
    NewUser.sok=mTcpSocket;
    Table.insert(Table.end(),NewUser);
    Descriptor[count]=NewUser;
    count++;
    str.setCodec("UTF-8");
    QString s="Текст, text";
    str <<s;
    s="text";
    str <<s;
    str<<flush;
    qDebug() << "New Connect";
    qDebug() <<NewUser.desc;
    qDebug() <<count;
    connect(NewUser.sok, SIGNAL(disconnected()), this, SLOT(slotClientDisconnected()));
    connect(NewUser.sok,SIGNAL(readyRead()),this,SLOT(get_data()));
    /*
     * кароч табл вдалась.. тре зробити нормально (все в ліст) через інсерт, при відключенні знаходити
     * цей елемент (мб за номером) і видаляти.
    */


}


void Server::slotClientDisconnected()
{
    //QTcpSocket* clientSocket = ( QTcpSocket* )sender();
    int Num=-1;
    QTcpSocket* clientSocket = static_cast< QTcpSocket* >(sender());
    clientSocket->close();
    for(int i=0;i<count;i++)
    {
        if(Descriptor[i].sok->socketDescriptor()==-1) Num=i;
    }
    QString Disc="Disconected num: ";

    for(int i=0;i<count;i++)
    {
        QTextStream str(Descriptor[i].sok);
        str<<Disc;
        str<<Num;
        str<<flush;
    }
}

void Server:: get_data()
{
    QTcpSocket* serverSocket = static_cast< QTcpSocket* >(sender());
    QString temp = serverSocket->readAll();
    qDebug()<<temp;

}
