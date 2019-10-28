#ifndef MAINW_H
#define MAINW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainW; }
QT_END_NAMESPACE

class MainW : public QMainWindow
{
    Q_OBJECT

public:
    MainW(QWidget *parent = nullptr);
    ~MainW();
signals:
    void hide_log();
    void send_int(QString sended);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainW *ui;
};
#endif // MAINW_H
