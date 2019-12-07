#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include <QMessageBox>
#include <QHostInfo>

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void chek_availible(QString temp, int num);
signals:
    void show_main();
    void show_reg();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

private:
    Ui::Login *ui;
    QTcpSocket * sok;
    bool is_available=false;
    bool login_av=false;
    bool reg_av=false;

};

#endif // LOGIN_H
