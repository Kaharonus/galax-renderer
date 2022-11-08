//
// Created by tomas on 2.8.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved
#include <assets/AssetLoader.h>
#include <generators/SolarSystemLoader.h>
#include <physics/PhysicsEngine.h>

#include "ui_MainWindow.h"
#include "MainWindow.h"

#include <QGridLayout>


using namespace Galax::Renderer;
using namespace Galax::Generators;
using namespace Galax::Physics;
//using namespace Galax::Renderer::SceneObjects;

MainWindow::MainWindow(const QSurfaceFormat &format, QWidget *parent, Qt::WindowFlags flags) :
        QMainWindow(parent, flags), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setupRenderer(format);
    setupPhysics();
    loadScene();
}

void MainWindow::setupRenderer(const QSurfaceFormat &format){
    renderer = new QRenderer(format);
    renderer->setVerticalSync(Galax::Renderer::QRenderer::Disabled);
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

void MainWindow::setupPhysics(){
    physicsEngine = new PhysicsEngine(1/60.f,this);

}


void MainWindow::loadScene() {
    SolarSystemLoader systemLoader;

    auto [system, lighting, effects] = systemLoader.generateSystem();

    renderer->setScene(system);
    renderer->setLightingModel(lighting);
    for(auto& effect : effects){
        renderer->addPostProcess(effect);
    }
    renderOptionsWindow->setScene(system, lighting, effects);

    auto planets = system->getPlanets();
    for(auto& planet : planets){
        physicsEngine->addRigidBody(planet);
    }

}

MainWindow::~MainWindow() {
    delete ui;
}


