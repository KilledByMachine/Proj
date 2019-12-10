#ifndef WINMAIN_H
#define WINMAIN_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include <QHostInfo>
#include <QMessageBox>

namespace Ui {
class WinMain;
}
//low_r,hight_r,low_rate,hight_rate,sem,sorting_by,course,inst,year;
struct my_config{
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
bool operator ==(const my_config& left, const my_config& right);

bool operator!=(const my_config& left, const my_config& right);


class WinMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit WinMain(QWidget *parent = nullptr);
    ~WinMain();
    void decoding(QString command);



signals:
    void show_log();
public slots:
    void get_keyID(int key);
    void start();

private slots:
    void get_from_serv();
    void on_pushButton_clicked();

    void on_prev_botton_clicked();

private:
    Ui::WinMain *ui;
    QTcpSocket * sok;
    int MyKey;

};



#endif // WINMAIN_H
