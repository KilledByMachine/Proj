#ifndef REG_H
#define REG_H

#include <QDialog>
#include <QApplication>
#include <QString>


namespace Ui {
class Reg;
}

class Reg : public QDialog
{
    Q_OBJECT

public:
    explicit Reg(QWidget *parent = nullptr);
    ~Reg();
signals:
    void send_from_reg(QString sended);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Reg *ui;
};

#endif // REG_H
