#include "reg.h"
#include "ui_reg.h"

Reg::Reg(QWidget *parent) :
    QDialog(parent),
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
   this->close();
   QString str=ui->lineEdit->text();
   emit send_from_reg(str);

}
