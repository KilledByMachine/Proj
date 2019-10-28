#ifndef WINMAIN_H
#define WINMAIN_H

#include <QMainWindow>

namespace Ui {
class WinMain;
}

class WinMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit WinMain(QWidget *parent = nullptr);
    ~WinMain();

private:
    Ui::WinMain *ui;
};

#endif // WINMAIN_H
