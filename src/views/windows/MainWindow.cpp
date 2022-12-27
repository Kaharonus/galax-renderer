
//
// Created by tomas on 02.10.21.
//


#include <QCodeEditor>
#include <QEvent>
#include <QGLSLCompleter>
#include <QGLSLHighlighter>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLayout>
#include <QPushButton>
#include <QString>
#include <QSyntaxStyle>
#include <QWidget>
#include <QLabel>
#include <QStyleFactory>
#include <memory>
#include <cxxabi.h>
#include <sstream>

#include "ui_MainWindow.h"
#include "MainWindow.h"

#include "views/controls/generic/QDataTreeItem.h"
#include "views/controls/scene/CameraControl.h"
#include "views/controls/scene/NodeControl.h"
#include "views/controls/scene/TextureControl.h"
#include "views/controls/scene/CodeEditor.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	this->nodeView = ui->nodeView;
	connect(nodeView, &QTreeWidget::itemSelectionChanged, this, &MainWindow::onItemClicked);

	dataView = this->findChild<QTabWidget *>("dataView");
	dataView->setTabsClosable(true);
	dataView->setMovable(true);
	connect(dataView, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequest);
	auto defaultText = new QLabel("No item selected");
	defaultText->setAlignment(Qt::AlignCenter);
	defaultText->setFixedWidth(500);
	dataView->addTab(defaultText, "Welcome");
	tabs.emplace_back(nullptr, defaultText);
}


MainWindow::~MainWindow() {
	delete ui;
}


void MainWindow::addTab(QWidget *widget, std::shared_ptr<SceneObject> object){
	for(auto& [o, w] : tabs){
		if(o == object){
			dataView->setCurrentWidget(w);
			return;
		}
	}
	dataView->addTab(widget, object->getName().c_str());
	tabs.emplace_back(object, widget);
	dataView->setCurrentWidget(widget);
}

void MainWindow::onItemClicked() {

	auto shader = dynamic_cast<QDataTreeItem<IShader> *>(nodeView->currentItem());
	if (shader) {
		auto editor = new CodeEditor(shader->getData(), this);
		addTab(editor, shader->getData());
		return;
	}
	auto camera = dynamic_cast<QDataTreeItem<ICamera> *>(nodeView->currentItem());
	if (camera) {
		auto cam = new CameraControl(camera->getData(), this);
		addTab(cam, camera->getData());

		return;
	}
	auto node = dynamic_cast<QDataTreeItem<IRenderNode> *>(nodeView->currentItem());
	if (node) {
		auto nodeControl = new NodeControl(node->getData(), this);
		addTab(nodeControl, node->getData());
		return;
	}

	auto texture = dynamic_cast<QDataTreeItem<ITexture> *>(nodeView->currentItem());
	if (texture) {
		auto tex = new TextureControl(texture->getData(), this);
		addTab(tex, texture->getData());
		return;
	}
}

void MainWindow::addNode(std::shared_ptr<IRenderNode> node, QTreeWidgetItem *parent) {
	// TODO: Add more options to the debug window

	// Builds the base of the node
	auto root = parent ? new QDataTreeItem<IRenderNode>(parent) : new QDataTreeItem<IRenderNode>(nodeView);
	root->setText(0, type(*node).c_str());
	root->setText(1, node->getName().data());
	root->setData(node);
	// Adds program information
	for (auto program: node->getPrograms()) {
		if (program) {
			auto programNode = new QTreeWidgetItem(root);
			programNode->setText(0, type(*program).c_str());
			programNode->setText(1, program->getName().data());
			for (auto shader: program->getShaders()) {
				auto shaderNode = new QDataTreeItem<IShader>(programNode);
				shaderNode->setData(shader);
				shaderNode->setText(0, ("Shader (" + shader->getTypeString() + ")").data());
				shaderNode->setText(1, shader->getName().data());
			}
		}
	}

	//Adds the mesh
	auto mesh = node->getMesh();
	if (mesh) {
		auto meshNode = new QTreeWidgetItem(root);
		meshNode->setText(0, type(*mesh).c_str());
		meshNode->setText(1, mesh->getName().data());
	}


	//Add the camera
	auto camera = node->getCamera();
	if (camera) {
		auto cameraNode = new QDataTreeItem<ICamera>(root);
		cameraNode->setData(camera);
		cameraNode->setText(0, type(*camera).c_str());
		cameraNode->setText(1, camera->getName().data());
	}

	auto textures = node->getTextures();
	auto textureNode = new QTreeWidgetItem(root);
	textureNode->setText(0, "Textures");
	for (const auto &texture: textures) {
		auto textureItem = new QDataTreeItem<ITexture>(textureNode);
		textureItem->setData(texture);
		textureItem->setText(0, type(*texture).c_str());
		textureItem->setText(1, texture->getName().data());
	}

	// Adds the children of the node
	auto childrenNode = new QTreeWidgetItem(root);
	childrenNode->setText(0, "Children");

	for (auto n: node->getChildren()) {
		addNode(n, childrenNode);
	}
}

void MainWindow::setScene(std::shared_ptr<Galax::Renderer::Scene> scene,
							 std::shared_ptr<Galax::Renderer::LightingModel> lighting,
							 const std::vector<std::shared_ptr<Galax::Renderer::IPostProcessEffect>> &effects) {
	this->scene = scene;
	this->lighting = lighting;
	this->effects = effects;
	for(auto& model : scene->getModels()){
		addNode(model, nullptr);
	}
	addLighting(lighting);
	addEffects(effects);


}

void MainWindow::addLighting(std::shared_ptr<Galax::Renderer::LightingModel> model) {
	auto root = new QTreeWidgetItem(nodeView);
	root->setText(0, "Lighting");
	root->setText(1, model->getName().data());


	auto textures = model->getTextures();
	auto textureNode = new QTreeWidgetItem(root);
	textureNode->setText(0, "Textures");
	for (const auto &texture: textures) {
		auto textureItem = new QDataTreeItem<ITexture>(textureNode);
		textureItem->setData(texture);
		textureItem->setText(0, type(*texture).c_str());
		textureItem->setText(1, texture->getName().data());
	}

	auto shader = model->getLightingShader();
	auto shaderNode = new QDataTreeItem<IShader>(root);
	shaderNode->setData(shader);
	shaderNode->setText(0, ("Shader (" + shader->getTypeString() + ")").data());
	shaderNode->setText(1, shader->getName().data());
};

void MainWindow::addEffects(const std::vector<std::shared_ptr<Galax::Renderer::IPostProcessEffect>> &effects) {
	auto root = new QTreeWidgetItem(nodeView);
	root->setText(0, "Effects");
	root->setText(1, "Effects");

	for (const auto &effect: effects) {
		auto effectNode = new QTreeWidgetItem(root);
		effectNode->setText(0, type(*effect).c_str());
		effectNode->setText(1, effect->getName().data());

		auto inTextures = effect->getInputTextures();
		auto inTextureNode = new QTreeWidgetItem(effectNode);
		inTextureNode->setText(0, "Input textures");
		for (const auto &texture: inTextures) {
			auto textureItem = new QDataTreeItem<ITexture>(inTextureNode);
			textureItem->setData(texture);
			textureItem->setText(0, type(*texture).c_str());
			textureItem->setText(1, texture->getName().data());
		}


		auto outTextures = effect->getOutputTextures();
		auto outTextureNode = new QTreeWidgetItem(effectNode);
		outTextureNode->setText(0, "Output textures");
		for (const auto &texture: outTextures) {
			auto textureItem = new QDataTreeItem<ITexture>(outTextureNode);
			textureItem->setData(texture);
			textureItem->setText(0, type(*texture).c_str());
			textureItem->setText(1, texture->getName().data());
		}

		auto shader = effect->getShader();
		auto shaderNode = new QDataTreeItem<IShader>(effectNode);
		shaderNode->setData(shader);
		shaderNode->setText(0, ("Shader (" + shader->getTypeString() + ")").data());
		shaderNode->setText(1, shader->getName().data());
	}
}


std::string MainWindow::demangle(const char *name) {
	int status = -4; // some arbitrary value to eliminate the compiler warning
	std::unique_ptr<char, void (*)(void *)> res{abi::__cxa_demangle(name, NULL, NULL, &status), std::free};
	std::string result = (status == 0) ? res.get() : name;
	std::size_t found = result.find_last_of("::");
	if (found != std::string::npos) {
		result = result.substr(found + 1);
	}
	return result;
}

void MainWindow::onTabCloseRequest(int index) {
	ui->dataView->removeTab(index);
	auto [_, widget] = tabs.at(index);
	widget->deleteLater();
	tabs.erase(tabs.begin() + index);
}

