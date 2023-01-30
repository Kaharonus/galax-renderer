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
    Galax::Windowing::IWindow* win;
    if(debug){
        auto renderWindow = new QtRenderWindow();
        renderWindow->resize(1600, 900);
        renderWindow->setWindowTitle("Galax Qt Renderer");
        win = renderWindow;

    }else{
        win = new Galax::Windowing::SDLWindow(1600, 900, "SDL_Renderer");
    }
    win->show(!debug);


	return QApplication::exec();
}
