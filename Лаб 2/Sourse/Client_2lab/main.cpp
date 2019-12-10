#include "login.h"
#include "reg.h"
#include "winmain.h"

#include <QApplication>
#include <QObject>


int main(int argc, char *argv[])
{
    QApplication ap(argc, argv);
    //
    //конекти з сервером поход тут, бо з кожної форми, тупувато якось,
    // стоп.. а це буде один і той же конект, якщо 3 різні форми?
    // конекчусь ведь звідси або з логіна,
    // або зроблю звідси вікно, для конекта в сервер, воно вилазитиме першим, буде пробувати зєднатись
    // якщо не вийде, то воно запропонує вийти?

    //  відкривтаи і закривати сокети кожен раз?
    //  мб в кожній формі по нужді подключати, отключати? коли переходит ьв іншу форму, вона сама конектиться
    //  і в відовідній для неї формі веде діалог з сервером? нарпиклад реєстрація тік перевірка юзера, і занесення
    //  його в базу, а логіну перевірити правильність данних, взяти ід юзера (залогіненого) передати в мейн
    //  який вже сам законектиться щоби взяти параметри для такого і такого юзера?
    // бінго нахццй!
    Login a;
    Reg b;
    WinMain c;
    //b.setMaximumSize(300,200);
    /*
     * при відритті форми откриваєм сокет, питаєм що тре, закриваєм (разом з закритям по дефолту
     * відправить і на сервер) мб deleteLater()?
     * хоча навряд це щось дасть.. швидше вьсого сокет просто буде закриватись
     * створюю через new і видаляю після переходу в іншу
    */
    QObject:: connect(&a,&Login::show_main,&c,&WinMain::show);
    QObject:: connect(&a,&Login::show_reg,&b,&Reg::show);
    QObject:: connect(&c,&WinMain::show_log,&a,&Login::show);
    QObject:: connect(&b,&Reg::show_main,&c,&WinMain::show);
    QObject:: connect(&b,&Reg::show_log,&a,&Login::show);

    QObject:: connect(&a,&Login::show_reg,&b,&Reg::start);
    QObject:: connect(&b,&Reg::show_log,&a,&Login::start);
    QObject:: connect(&c,&WinMain::show_log,&a,&Login::start);
    QObject:: connect(&a,&Login::show_main,&c,&WinMain::start);
    QObject:: connect(&b,&Reg::show_main,&c,&WinMain::start);

    QObject:: connect(&a,&Login::send_ID,&c,&WinMain::get_keyID);
    QObject:: connect(&b,&Reg::send_ID,&c,&WinMain::get_keyID);


    a.show();

    return ap.exec();
}
