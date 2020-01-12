#ifndef CLIENTS_H
#define CLIENTS_H

#include <QObject>
#include <QTcpSocket>

class Clients
{
public:


    explicit Clients();
    ~Clients();

    QTcpSocket* socket;
    QString st;
    void set_soket(QTcpSocket* a);


private:

};

//typedef QMap<int, Clients> ConnectedClients;
//typedef QMap<int, Clients>::iterator ClientIterator;
//typedef Clients::iterator ClientsIterator;

#endif // CLIENTS_H
