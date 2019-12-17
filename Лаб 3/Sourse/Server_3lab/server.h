#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include "clients.h"
#include <QtSql>

struct table_records{
    //rate,rate2,course , year ,sem ,inst
    QString name;
    QString rate;
    QString rate2;
    QString course;
    QString year;
    QString sem;
    QString inst;
    QString s1;
    QString s2;
    QString s3;
};
struct my_config{
    int user_id;
    bool is_connected=false;
    double low_r=50.000;
    double hight_r=100.000;
    double low_rate=3.00;
    double hight_rate=5.00;
    bool sem1=true;
    bool sem2=true;
    int sorting_by=0;
    bool c1=true;
    bool c2=true;
    bool c3=true;
    bool c4=true;
    bool igdg=true;
    bool igsn=true;
    bool ikni=true;
    bool ikta=true;
    bool _2018=true;
    bool _2017=true;
    bool _2016=true;
    bool _2015 =true;
    //67.345:89.567:3.45:4.48:2:1:a:8:8;
};
struct user_list{
    my_config Conf;
    int desc;
};

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    struct Msok{
        int desc;
        QTcpSocket* sok;
    };
    void decoding(QString comand, int descriptor);
    QString convert_conf(int number_of_conf);

signals:

public slots:
        void slotNewConnection();
        void slotClientDisconnected();
        void get_data();

private:

    QTcpServer * mTcpServer;
    QTcpSocket * mTcpSocket;
    QSqlDatabase db;
};







#endif // SERVER_H
