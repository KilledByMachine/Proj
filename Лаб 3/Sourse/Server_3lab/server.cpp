#include "server.h"


Server::Server(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &Server::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 6000)){

        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }

    connect(mTcpServer,SIGNAL(newConnection()),this,SLOT(on_newUserConnected()));
}

void Server::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection();
    QTextStream str(mTcpSocket);
    str.setCodec("UTF-8");
    QString s="КУ я тут";
    str <<s;
    str<<flush;
    qDebug() << "New Connect";
    //connect(mTcpSocket, SIGNAL(disconnected()), this, SLOT(slotClientDisconnected()));
}

void Server::on_newUserConnected()
{
    qDebug() << "Server::on_newUserConnected(): new anonymous user connected";
    //ConnectedClients Cll;
    QMap<int, QTcpSocket*> C;
    C.insert(mTcpSocket->socketDescriptor(),mTcpSocket);
    qDebug()<<mTcpSocket->socketDescriptor();

    //connect(mTcpSocket, SIGNAL(readyRead()), this, SLOT(on_recievedData())   );
}
