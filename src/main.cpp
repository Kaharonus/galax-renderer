#include <QSurfaceFormat>
#include <QWidget>
#include <QApplication>
#include <iostream>
#include <ctime>

#include "views/windows/QtRenderWindow.h"
#include "views/windows/MainWindow.h"
#include "views/windows/SDLWindow.h"


int main(int argc, char *argv[]) {
	std::srand(std::time(nullptr));
	QApplication a(argc, argv);

	SDLWindow sdl(1600, 900, "Render");
	sdl.show(false);

	return QApplication::exec();
}
