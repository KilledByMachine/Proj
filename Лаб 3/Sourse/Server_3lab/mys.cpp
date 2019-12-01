#include "mys.h"
#include "ui_mys.h"

MyS::MyS(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyS)
{
    ui->setupUi(this);
}

MyS::~MyS()
{
    delete ui;
}

