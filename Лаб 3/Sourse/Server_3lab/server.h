#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include "clients.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
signals:

public slots:
        void slotNewConnection();
        //void slotClientDisconnected();
        void on_newUserConnected();
private:
    QTcpServer * mTcpServer;
    QTcpSocket * mTcpSocket;



};

#endif // SERVER_H
