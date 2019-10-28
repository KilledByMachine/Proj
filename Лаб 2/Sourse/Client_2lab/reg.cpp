#include "reg.h"
#include "ui_reg.h"

Reg::Reg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Reg)
{
    ui->setupUi(this);
}

Reg::~Reg()
{
    delete ui;
}

void Reg::on_pushButton_clicked()
{
    emit show_log();
    close();
}

void Reg::on_pushButton_2_clicked()
{
    emit show_main();
    close();
}
