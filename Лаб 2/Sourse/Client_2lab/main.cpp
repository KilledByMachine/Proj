#include "login.h"
#include "reg.h"
#include "winmain.h"

#include <QApplication>
#include <QObject>


int main(int argc, char *argv[])
{
    QApplication ap(argc, argv);
    Login a;
    //a.show();
    Reg b;
    WinMain c;
    QObject:: connect(&a,&Login::show_main,&c,&WinMain::show);
    QObject:: connect(&a,&Login::show_reg,&b,&Reg::show);
    QObject:: connect(&c,&WinMain::show_log,&a,&Login::show);
    QObject:: connect(&b,&Reg::show_main,&c,&WinMain::show);
    QObject:: connect(&b,&Reg::show_log,&a,&Login::show);

    b.show();

    return ap.exec();
}
