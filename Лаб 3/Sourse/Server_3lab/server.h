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
    double low_r;
    double hight_r;
    double low_rate;
    double hight_rate;
    bool sem1;
    bool sem2;
    int sorting_by;
    bool c1;
    bool c2;
    bool c3;
    bool c4;
    bool igdg;
    bool igsn;
    bool ikni;
    bool ikta;
    bool _2018;
    bool _2017;
    bool _2016;
    bool _2015;
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
