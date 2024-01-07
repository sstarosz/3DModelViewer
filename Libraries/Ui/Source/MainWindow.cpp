#include "MainWindow.hpp"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>

#include "Viewport.hpp"

namespace st::ui
{
    MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
    {
        setWindowTitle("StModelViewer");
        
        Viewport *viewport = new Viewport();
        QWidget *centralWidget = new QWidget(this);
        QHBoxLayout *layout = new QHBoxLayout(centralWidget);
        layout->addWidget(viewport);
        layout->addWidget(new QLabel("Hello World!"));

        setCentralWidget(centralWidget);
    }

}//namespace st::ui