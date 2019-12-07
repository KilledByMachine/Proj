#ifndef WINMAIN_H
#define WINMAIN_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>

namespace Ui {
class WinMain;
}

class WinMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit WinMain(QWidget *parent = nullptr);
    ~WinMain();
signals:
    void show_log();

private slots:
    void on_pushButton_clicked();

private:
    Ui::WinMain *ui;
};

#endif // WINMAIN_H
