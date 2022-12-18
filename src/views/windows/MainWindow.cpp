//
// Created by tomas on 2.8.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved
#include <assets/AssetLoader.h>
#include <generators/SolarSystemGenerator.h>
#include <physics/PhysicsEngine.h>
#include <views/controls/QDraggableDoubleSpinBox.h>

#include "ui_MainWindow.h"
#include "MainWindow.h"
#include "Extensions.h"

#include <QGridLayout>


using namespace Galax::Renderer;
using namespace Galax::Generators;
using namespace Galax::Physics;


MainWindow::MainWindow(const QSurfaceFormat &format, QWidget *parent, Qt::WindowFlags flags) :
		QMainWindow(parent, flags), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	this->inputHandler = std::make_shared<InputHandler>();
	setupRenderer(format);
	setupPhysics();
	this->sceneLoader = std::make_shared<SceneLoader>("assets.data", "scenes");
	this->sceneLoader->registerGenerator(std::make_shared<SolarSystemGenerator>());

	refreshTimer = new QTimer(this);
	refreshTimer->setInterval(33);
	connect(refreshTimer, &QTimer::timeout, this, &MainWindow::uiRefresh);
	refreshTimer->start();

	loadScene("scenes/solar-system.json");
	ui->atmosphereWidget->setVisible(false);

	connect(this->ui->pauseAnimationsButton, &QPushButton::clicked, this, [&]{
		auto scene =this->renderer->getScene();
		if(!scene){
			return;
		}
		pauseAnimations(scene);
	});

}


void MainWindow::pauseNode(std::shared_ptr<IRenderNode> node){
	auto anim = node->getAnimations();
	for(auto animation: anim){
		if(pause){
			animation->stop();
		}else{
			animation->start();
		}
	}
	for(const auto& child: node->getChildren()){
		pauseNode(child);
	}
}

void MainWindow::pauseAnimations(std::shared_ptr<Scene> scene){
	for(const auto& model :scene->getModels()){
		pauseNode(model);
	}
	pause = !pause;;
}

void MainWindow::uiRefresh() {
	if (!this->selectedPlanet) {
		return;
	}
	auto planet = this->selectedPlanet;
	{ //Planet
		ui->planetType->setText(planet->getTypeName().c_str());
		if (!ui->seedInput->hasFocus()) {
			ui->seedInput->setValue(planet->getSeed());
		}
		if (abs(planet->getSeed() - this->ui->seedInput->value()) > 0.5) {
			planet->setSeed(ui->seedInput->value());
		}

		if (!ui->scaleInput->hasFocus()) {
			ui->scaleInput->setValue(planet->getSize());
		}
		planet->setSize(ui->scaleInput->value());

		if (!ui->speedInput->hasFocus()) {
			ui->speedInput->setValue(planet->getSpeed());
		}
		planet->setSpeed(ui->speedInput->value());
	}

	auto atmo = planet->getAtmosphere();
	ui->atmosphereWidget->setVisible(atmo != nullptr);
	if (atmo) {
		auto color = atmo->getColor();
		if (!ui->colorR->hasFocus()) {
			ui->colorR->setValue(color.r);
		}
		if (!ui->colorG->hasFocus()) {
			ui->colorG->setValue(color.g);
		}
		if (!ui->colorB->hasFocus()) {
			ui->colorB->setValue(color.b);
		}
		atmo->setColor(glm::vec3(ui->colorR->value(), ui->colorG->value(), ui->colorB->value()));

		if (!ui->atmoSizeInput->hasFocus()) {
			ui->atmoSizeInput->setValue(atmo->getRadius());
		}
		atmo->setRadius(ui->atmoSizeInput->value());

		if (!ui->densityInput->hasFocus()) {
			ui->densityInput->setValue(atmo->getDensity());
		}
		atmo->setDensity(ui->densityInput->value());
	}

	for (const auto &moon: this->moonControls) {
		moon->update();
	}


}


void MainWindow::setupRenderer(const QSurfaceFormat &format) {
	renderer = new QRenderer(inputHandler, format);
	//renderer->setVerticalSync(Galax::Renderer::QRenderer::Disabled);
	auto widget = QWidget::createWindowContainer(renderer);
	widget->setAutoFillBackground(false); // Important for overdraw, not occluding the scee.
	widget->setFocusPolicy(Qt::TabFocus);
	widget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
	auto layout = this->findChild<QGridLayout *>("layout");
	layout->addWidget(widget);
	show();
	renderOptionsWindow = new RenderOptions(this);
	renderOptionsWindow->show();

	this->ui->splitter->setSizes({60, 200});
}

void MainWindow::setupPhysics() {
	physicsEngine = new PhysicsEngine(inputHandler, 1 / 30.f, this);
}


void MainWindow::loadScene(const std::string &path) {

	auto sceneData = this->sceneLoader->loadScene(path);
	if (!sceneData) {
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

	inputHandler->registerMouseClickOnCallback([&](auto body, auto button) {
		//Check if it is a planet
		std::shared_ptr<IRenderNode> node;
		try {
			node = std::any_cast<std::shared_ptr<IRenderNode>>(body);
		} catch (...) {
			return;
		}
		auto planet = std::dynamic_pointer_cast<Planet>(node);
		this->selectedPlanet = planet;
		for (auto control: moonControls) {
			ui->moonList->removeWidget(control);
			control->deleteLater();
		}
		moonControls.clear();
		if (this->selectedPlanet) {
			int i = 0;
			for (const auto& moon: planet->getMoons()) {
				auto c = new MoonControl(moon, i,nullptr);
				moonControls.push_back(c);
				ui->moonList->addWidget(c);
				i++;
			}
		}

	});

	//FIX - unsafe
	physicsEngine->setCamera(scene->getModels()[0]->getCamera());

	auto physics = getPhysical(scene);
	for (auto &item: physics) {
		physicsEngine->addRigidBody(item);
	}
}

std::vector<std::shared_ptr<IRigidBody>> MainWindow::getPhysical(std::shared_ptr<Scene> scene) {
	auto physical = std::vector<std::shared_ptr<IRigidBody>>();
	for (auto &node: scene->getModels()) {
		auto p = getPhysical(node);
		if (auto planet = std::dynamic_pointer_cast<Planet>(node)) {
			p.push_back(planet);
		}
		physical.insert(physical.end(), p.begin(), p.end());
	}
	return physical;
}

std::vector<std::shared_ptr<IRigidBody>> MainWindow::getPhysical(const std::shared_ptr<IRenderNode> &node) {
	std::vector<std::shared_ptr<IRigidBody>> physical;
	for (const auto &child: node->getChildren()) {
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


