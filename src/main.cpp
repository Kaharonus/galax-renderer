#include <QSurfaceFormat>
#include <QWidget>
#include <QApplication>
#include <iostream>
#include <ctime>

#include "views/windows/MainWindow.h"


int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setVersion(4, 6);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    MainWindow window(format);
    window.resize(1600, 900);
    window.show();
    return QApplication::exec();
}
