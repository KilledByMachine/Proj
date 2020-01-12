#ifndef MYS_H
#define MYS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MyS; }
QT_END_NAMESPACE

class MyS : public QMainWindow
{
    Q_OBJECT

public:
    MyS(QWidget *parent = nullptr);
    ~MyS();

private:
    Ui::MyS *ui;
};
#endif // MYS_H
