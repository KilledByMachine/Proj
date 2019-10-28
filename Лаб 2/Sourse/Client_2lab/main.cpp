#include "login.h"
#include "reg.h"
#include "winmain.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication ap(argc, argv);
    Login a;
    a.show();
    Reg b;
    b.show();
    WinMain c;
    c.show();

    return ap.exec();
}
