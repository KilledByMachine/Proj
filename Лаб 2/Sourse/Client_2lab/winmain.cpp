#include "winmain.h"
#include "ui_winmain.h"

WinMain::WinMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WinMain)
{
    ui->setupUi(this);
}

WinMain::~WinMain()
{
    delete ui;
}

void WinMain::on_pushButton_clicked()
{
    //ui->doubleSpin_from_2->setValue(13.133);
    //ui->action1->setChecked(false); true\flase
    //qDebug()<<ui->action1->isChecked();
    emit show_log();
    close();
}
