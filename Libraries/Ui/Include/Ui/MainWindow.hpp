#ifndef ST_UI_MAINWINDOW_HPP
#define ST_UI_MAINWINDOW_HPP

#include <QMainWindow>


namespace st::ui
{


class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = {}); 
};



}//namespace st::ui

#endif //ST_UI_MAINWINDOW_HPP