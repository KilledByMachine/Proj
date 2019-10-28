#include "log.h"
#include "ui_log.h"
#include "reg.h"
#include "mainw.h"



Log::Log(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Log)
{
    ui->setupUi(this);

}

void Log::change(QString a)
{
    temp =a;
}

QString Log:: get_s()
{
    return temp;
}

Log::~Log()
{
    delete ui;
}

void Log::on_pushButton_clicked()
{
    //Reg r;
    //r.setModal(false);
    Reg * r=new Reg(this);
    r->show();
    QObject:: connect(r, &Reg::send_from_reg, this, &Log::change);
    //r.exec();



}

void Log::on_pushButton_2_clicked()
{

    this->hide();
    QMessageBox::warning(this,"Провірочна","Помилка");

    QMessageBox:: information(this,"Увага!!!   ","12345678987654321");

    MainW * wind = new MainW(this);
    connect(wind, &MainW::hide_log, this, &Log::show);
    wind->show();
    connect(wind, &MainW::close, this, &Log::show);
    connect(wind, &MainW::send_int, this, &Log::change);


    //this->show();
}

void Log::on_pushButton_3_clicked()
{
    ui->lineEdit->insert(get_s());
}
