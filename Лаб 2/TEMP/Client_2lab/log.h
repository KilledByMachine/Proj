#ifndef LOG_H
#define LOG_H

#include <QDialog>
#include <QMessageBox>
#include <QString>

namespace Ui {
class Log;
}

class Log : public QDialog
{
    Q_OBJECT

public:
    explicit Log(QWidget *parent = nullptr);
    void change(QString temp);
    QString get_s();
    ~Log();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    //void hide_log();

    void on_pushButton_3_clicked();


private:
    Ui::Log *ui;
    QString temp;
};

#endif // LOG_H
