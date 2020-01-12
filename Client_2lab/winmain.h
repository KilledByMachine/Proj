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
struct table_records{
    //rate,rate2,course , year ,sem ,inst
    QString fname;
    QString lname;
    QString pname;
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

bool operator ==(const my_config& left, const my_config& right);

bool operator!=(const my_config& left, const my_config& right);


class WinMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit WinMain(QWidget *parent = nullptr);
    ~WinMain();
    void decoding(QString command);
    void change_conf();
    void config_change();
    QString convert_conf();
    void update_table();


signals:
    void show_log();
public slots:
    void get_keyID(int key);
    void start();

private slots:
    void get_from_serv();
    void on_pushButton_clicked();

    void on_prev_botton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_search_name_clicked();

    void on_action2015_changed();

    void on_action2016_changed();

    void on_action2017_changed();

    void on_action2018_changed();

    void on_sem1_changed();

    void on_sem2_changed();

    void on_igdg_changed();

    void on_igsn_changed();

    void on_ikni_changed();

    void on_ikta_changed();

    void on_course1_changed();

    void on_course2_changed();

    void on_course3_changed();

    void on_course4_changed();

    void on_doubleSpin_from_valueChanged(double arg1);

    void on_doubleSpin_to_valueChanged(double arg1);

    void on_doubleSpin_from_2_valueChanged(double arg1);

    void on_doubleSpin_to_2_valueChanged(double arg1);

    void on_next_button_clicked();

private:
    Ui::WinMain *ui;
    QTcpSocket * sok;
    int MyKey;
    my_config Config;
    my_config Sended_conf;
    bool config_change_enable=false;
    int Nres=0;
    int now=0;
    int next=0;
    int cadr=0;

};



#endif // WINMAIN_H
