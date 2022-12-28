//
// Created by tomas on 2.8.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved
#include <assets/AssetLoader.h>
#include <generators/SolarSystemGenerator.h>
#include <physics/PhysicsEngine.h>
#include "views/controls/generic/QDraggableDoubleSpinBox.h"

#include "ui_QtRenderWindow.h"
#include "QtRenderWindow.h"
#include "Extensions.h"

#include <QGridLayout>


using namespace Galax::Renderer;
using namespace Galax::Generators;
using namespace Galax::Physics;


QtRenderWindow::QtRenderWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags), ui(new Ui::QtRenderWindow) {

	QSurfaceFormat format;
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setMajorVersion(4);
	format.setMinorVersion(6);
	format.setDepthBufferSize(16);

	ui->setupUi(this);



	this->inputHandler = std::make_shared<InputHandler>();
	setupRenderer(format);
	setupPhysics();
	this->sceneLoader = std::make_shared<SceneLoader>("assets.data", "scenes");
	this->sceneLoader->registerGenerator(std::make_shared<SolarSystemGenerator>());

	refreshTimer = new QTimer(this);
	refreshTimer->setInterval(33);
	refreshTimer->start();

	this->renderOptionsWindow = new MainWindow(this);


	loadScene("scenes/solar-system.json");
}



void QtRenderWindow::setupRenderer(const QSurfaceFormat &format) {
	SceneObject::addUniqueContext("QRender");

	renderer = new QRenderer(inputHandler, format);
	//renderer->setVerticalSync(Galax::Renderer::QRenderer::Disabled);
	auto widget = QWidget::createWindowContainer(renderer);
	widget->setAutoFillBackground(false); // Important for overdraw, not occluding the scee.
	widget->setFocusPolicy(Qt::TabFocus);
	widget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
	auto layout = this->findChild<QGridLayout *>("baseGridLayout");
	layout->addWidget(widget);
	show();
}

void QtRenderWindow::setupPhysics() {
	physicsEngine = new PhysicsEngine(inputHandler, 16);
	this->engineUpdate = new QTimer(this);
	this->engineUpdate->setInterval(16);
	connect(this->engineUpdate, &QTimer::timeout, this, [&](){
		physicsEngine->update();
	});
	this->engineUpdate->start();
}


void QtRenderWindow::loadScene(const std::string &path) {
	SceneObject::selectAndLockContext("QRender");
	auto sceneData = this->sceneLoader->loadScene(path);
	if (!sceneData) {
		SceneObject::unlockContext();
		return;
	}
	auto scene = sceneData->scene;
	auto effects = sceneData->postProcessEffects;

	renderer->setScene(scene);
	renderer->setLightingModel(scene->getLightingModel());
	for (auto &effect: sceneData->postProcessEffects) {
		renderer->addPostProcess(effect);
	}
	renderOptionsWindow->setScene(scene, scene->getLightingModel(), effects);
	physicsEngine->setScene(scene);
	SceneObject::unlockContext();
}




QtRenderWindow::~QtRenderWindow() {
	delete ui;
}


