#include <QSurfaceFormat>
#include <QWidget>
#include <QApplication>
#include <iostream>
#include <ctime>

#include "views/windows/QtRenderWindow.h"
#include "views/windows/DebugWindow.h"
#include "views/windows/SDLWindow.h"


int main(int argc, char *argv[]) {
	std::srand(std::time(nullptr));
	QApplication a(argc, argv);
    auto debug = a.arguments().contains("--with-debug");

    if(debug){
        auto qt = new QtRenderWindow();
        qt->resize(1600, 900);
        qt->setWindowTitle("Galax Qt Renderer");
        qt->show(false);
        return QApplication::exec();
    }else {
        auto sdl = new Galax::Windowing::SDLWindow(1600, 900, "SDL_Renderer");
        sdl->show(true);
        return 0;
    }
    return 420; //Should be impossible
}
