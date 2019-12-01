#include "mys.h"
#include "server.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyS w;
    Server tryt;
    w.show();
    return a.exec();
}
