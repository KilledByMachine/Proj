#include "mainw.h"
#include "log.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Log w;
    w.show();

    return a.exec();
}
