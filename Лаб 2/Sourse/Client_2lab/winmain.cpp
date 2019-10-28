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
    emit show_log();
    close();
    // hide();
}
