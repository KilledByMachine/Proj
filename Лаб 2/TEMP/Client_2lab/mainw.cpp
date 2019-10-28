#include "mainw.h"
#include "ui_mainw.h"
#include <QApplication>
#include "log.h"


MainW::MainW(QWidget *parent)

    : QMainWindow(parent)
    , ui(new Ui::MainW)
{
    ui->setupUi(this);
}

MainW::~MainW()
{
    delete ui;
}


void MainW::on_pushButton_clicked()
{

    close();
    emit hide_log();
    QString str=ui->lineEdit->text();
    emit send_int(str);

}
