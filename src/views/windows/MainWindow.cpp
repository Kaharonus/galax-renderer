//
// Created by tomas on 2.8.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved
#include <assets/AssetLoader.h>
#include <generators/SolarSystemGenerator.h>
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

    this->inputHandler = std::make_shared<InputHandler>();
    setupRenderer(format);
    setupPhysics();
    this->sceneLoader = std::make_shared<SceneLoader>("assets.data", "scenes");
	this->sceneLoader->registerGenerator(std::make_shared<SolarSystemGenerator>());



	loadScene("scenes/solar-system.json");
}

void MainWindow::setupRenderer(const QSurfaceFormat &format){
    renderer = new QRenderer(inputHandler, format);
    //renderer->setVerticalSync(Galax::Renderer::QRenderer::Disabled);
    auto widget = QWidget::createWindowContainer(renderer);
    //widget->setMinimumSize(1,1);
    widget->setAutoFillBackground(false); // Important for overdraw, not occluding the scee.
    widget->setFocusPolicy(Qt::TabFocus);
	widget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    auto layout = this->findChild<QGridLayout*>("layout");
    layout->addWidget(widget);
    show();
    renderOptionsWindow = new RenderOptions(this);
    renderOptionsWindow->show();

	this->ui->splitter->setSizes({60, 200});
}

void MainWindow::setupPhysics(){
    physicsEngine = new PhysicsEngine(inputHandler, 1/60.f, this);
}




void MainWindow::loadScene(const std::string& path) {

	auto sceneData = this->sceneLoader->loadScene(path);
	if(!sceneData){
		return;
	}
	auto scene = sceneData->scene;
	auto effects = sceneData->postProcessEffects;

    renderer->setScene(scene);
    renderer->setLightingModel(scene->getLightingModel());
    for(auto& effect : sceneData->postProcessEffects){
        renderer->addPostProcess(effect);
    }
    renderOptionsWindow->setScene(scene, scene->getLightingModel(), effects);

	inputHandler->registerMouseClickOnCallback([](auto body, auto button){
		//Check if it is a planet
		std::shared_ptr<IRenderNode> node;
		try{
			node = std::any_cast<std::shared_ptr<IRenderNode>>(body);
		}catch(...){
			return;
		}
		auto planet = std::dynamic_pointer_cast<Planet>(node);
		if(!planet){
			return;
		}


	});

	//FIX - unsafe
    physicsEngine->setCamera(scene->getModels()[0]->getCamera());

    auto physics = getPhysical(scene);
    for(auto& item : physics){
        physicsEngine->addRigidBody(item);
    }
}

std::vector<std::shared_ptr<IRigidBody>> MainWindow::getPhysical(std::shared_ptr<Scene> scene) {
	auto physical = std::vector<std::shared_ptr<IRigidBody>>();
	for(auto& node : scene->getModels()) {
		auto p = getPhysical(node);
		if(auto planet = std::dynamic_pointer_cast<Planet>(node)){
			p.push_back(planet);
		}
		physical.insert(physical.end(), p.begin(), p.end());
	}
	return physical;
}

std::vector<std::shared_ptr<IRigidBody>> MainWindow::getPhysical(const std::shared_ptr<IRenderNode> &node) {
	std::vector<std::shared_ptr<IRigidBody>> physical;
	for (const auto &child : node->getChildren()) {
		if (auto planet = std::dynamic_pointer_cast<IRigidBody>(child)) {
			physical.push_back(planet);
		}
		auto children = getPhysical(child);
		physical.insert(physical.end(), children.begin(), children.end());
	}
	return physical;
}


MainWindow::~MainWindow() {
    delete ui;
}


