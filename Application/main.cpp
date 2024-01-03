#include <QApplication>
#include <QWidget>

///#include "MainWindow/MainWindows.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //st::MainWindow mainWindow;
    //mainWindow.show();
    QWidget w;
    w.show();

    return QApplication::exec();
}
