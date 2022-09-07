//
// Created by tomas on 2.8.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved
#include "../../assets/AssetLoader.h"
#include "../../generators/SolarSystemLoader.h"
#include "ui_MainWindow.h"
#include "MainWindow.h"

#include <QGridLayout>


using namespace Galax::Renderer;
using namespace Galax::Generators;
//using namespace Galax::Renderer::SceneObjects;

MainWindow::MainWindow(const QSurfaceFormat &format, QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setupRenderer(format);
    loadScene();
}

void MainWindow::setupRenderer(const QSurfaceFormat &format){
    renderer = new QRenderer(format);
    QWidget * widget = QWidget::createWindowContainer(renderer);
    widget->setMinimumSize(1,1);
    widget->setAutoFillBackground(false); // Important for overdraw, not occluding the scene.
    widget->setFocusPolicy(Qt::TabFocus);
    auto layout = this->findChild<QGridLayout*>("baseGridLayout");
    layout->addWidget(widget);
    show();

    renderOptionsWindow = new RenderOptions(this);
    renderOptionsWindow->show();
}

void MainWindow::loadScene() {
    SolarSystemLoader systemLoader;

    auto system = systemLoader.generateSystem();

    renderer->setScene(system);
    renderOptionsWindow->setScene(system);
}

MainWindow::~MainWindow() {
    delete ui;
}


