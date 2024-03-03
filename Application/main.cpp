#include "Ui/MainWindow.hpp"
#include <QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	st::ui::MainWindow mainWindow;
	mainWindow.show();

	return QApplication::exec();
}
