#ifndef REG_H
#define REG_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include <QMessageBox>
#include <QHostInfo>

namespace Ui {
class Reg;
}

class Reg : public QMainWindow
{
    Q_OBJECT

public:    
    explicit Reg(QWidget *parent = nullptr);
    ~Reg();
    void chek_availible(QString temp, int num);
    void decoding(QString command);

signals:
    void show_log();
    void show_main();
    void send_ID(int);
public slots:
        void start();

private slots:
    void get_from_serv();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

private:
    Ui::Reg *ui;
    QTcpSocket * sok;
    bool is_available=false;
    bool login_av=false;
    bool pass1_av=false;
    bool pass2_av=false;
};

#endif // REG_H
