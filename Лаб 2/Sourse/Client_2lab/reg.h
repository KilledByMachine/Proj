#ifndef REG_H
#define REG_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>

namespace Ui {
class Reg;
}

class Reg : public QMainWindow
{
    Q_OBJECT

public:    
    explicit Reg(QWidget *parent = nullptr);
    ~Reg();
signals:
    void show_log();
    void show_main();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Reg *ui;
};

#endif // REG_H
