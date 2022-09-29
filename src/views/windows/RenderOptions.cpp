
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

#include "ui_RenderOptions.h"
#include "RenderOptions.h"

#include "../controls/QDataTreeItem.h"
#include "../controls/CameraControl.h"
#include "../controls/NodeControl.h"
#include "../controls/TextureControl.h"


RenderOptions::RenderOptions(QWidget* parent) : QMainWindow(parent), ui(new Ui::RenderOptions) {
    ui->setupUi(this);
    timer = new QTimer(this);
    this->nodeView = ui->nodeView;
    connect(nodeView, &QTreeWidget::itemSelectionChanged, this, &RenderOptions::onItemClicked);
    dataWidget = this->findChild<QVBoxLayout*>("dataWidget");
    auto defaultText = new QLabel("No item selected");
    defaultText->setAlignment(Qt::AlignCenter);
    defaultText->setFixedWidth(500);
    dataWidget->layout()->addWidget(defaultText);
}

void RenderOptions::clearLayout(QLayout *layout) {
    if (layout == NULL)
        return;
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}


void RenderOptions::prepareCodeEditor() {
    codeEditor = new QCodeEditor(this);
    dataWidget->layout()->addWidget(codeEditor);
    codeEditor->setAutoIndentation(true);
    codeEditor->setAutoParentheses(true);
    codeEditor->setCompleter(new QGLSLCompleter(this));
    codeEditor->setHighlighter(new QGLSLHighlighter());
    codeEditor->setTabReplaceSize(4);

    auto qStyleString = QString(styleString.data());

    auto style = new QSyntaxStyle(this);
    codeEditor->setSyntaxStyle(style->load(qStyleString) ? style : QSyntaxStyle::defaultStyle());

    errorLabel = new QLabel(this);
    errorLabel->setStyleSheet("QLabel { color : red; }");
    dataWidget->layout()->addWidget(errorLabel);

    timer = new QTimer(this);
    timer->setInterval(33);
    connect(timer, &QTimer::timeout, this, [this]() {
        if(errorLabel && currentShader){
            errorLabel->setText(currentShader->getInfoLog().c_str());
        }
    });
    timer->start();

    connect(codeEditor, &QCodeEditor::textChanged, this, &RenderOptions::codeChanged);
}

RenderOptions::~RenderOptions() {
    delete ui;
}

void RenderOptions::editShader(std::shared_ptr<IShader> shader) {
    prepareCodeEditor();
    currentShader = shader;
    codeEditor->setPlainText(shader->getSource().data());
}


void RenderOptions::editCamera(std::shared_ptr<ICamera> camera){
    auto cam = new CameraControl(camera , this);
    dataWidget->layout()->addWidget(cam);
}

void RenderOptions::editTexture(std::shared_ptr<ITexture> texture){
    auto tex = new TextureControl(texture, this);
    dataWidget->layout()->addWidget(tex);
}


void RenderOptions::editNode(std::shared_ptr<INode> node){
    auto nodeControl = new NodeControl(node, this);
    dataWidget->layout()->addWidget(nodeControl);
}

void RenderOptions::onItemClicked() {
    timer->stop();
    clearLayout(dataWidget->layout());

    auto shader = dynamic_cast<QDataTreeItem<IShader>*>(nodeView->currentItem());
    if (shader) {
        editShader(shader->getData());
        return;
    }
    auto camera = dynamic_cast<QDataTreeItem<ICamera>*>(nodeView->currentItem());
    if (camera) {
        editCamera(camera->getData());
        return;
    }
    auto node = dynamic_cast<QDataTreeItem<INode>*>(nodeView->currentItem());
    if (node) {
        editNode(node->getData());
        return;
    }

    auto texture = dynamic_cast<QDataTreeItem<Texture>*>(nodeView->currentItem());
    if (texture) {
        editTexture(texture->getData());
        return;
    }
}

void RenderOptions::codeChanged() {
    auto code = codeEditor->toPlainText().toStdString();
    if (currentShader == nullptr) {
        return;
    }
    currentShader->setSource(code);
    this->errorLabel->setText(QString(currentShader->getInfoLog().c_str()));
}

void RenderOptions::addNode(std::shared_ptr<INode> node, QTreeWidgetItem* parent) {
    // TODO: Add more options to the debug window

    // Builds the base of the node
    auto root = parent ? new QDataTreeItem<INode>(parent) : new QDataTreeItem<INode>(nodeView);
    root->setText(0, type(*node).c_str());
    root->setText(1, node->getName().data());
    root->setData(node);
    // Adds program information
    auto program = node->getProgram();
    if(program){
        auto programNode = new QTreeWidgetItem(root);
        programNode->setText(0, type(*program).c_str());
        programNode->setText(1, program->getName().data());
        for (auto shader : program->getShaders()) {
            auto shaderNode = new QDataTreeItem<IShader>(programNode);
            shaderNode->setData(shader);
            shaderNode->setText(0, ("Shader (" + shader->getTypeString() + ")").data());
            shaderNode->setText(1, shader->getName().data());
        }
    }


    //Adds the mesh
    auto mesh = node->getMesh();
    if(mesh){
        auto meshNode = new QTreeWidgetItem(root);
        meshNode->setText(0, type(*mesh).c_str());
        meshNode->setText(1, mesh->getName().data());
    }


    //Add the camera
    auto camera = node->getCamera();
    if(camera){
        auto cameraNode = new QDataTreeItem<ICamera>(root);
        cameraNode->setData(camera);
        cameraNode->setText(0, type(*camera).c_str());
        cameraNode->setText(1, camera->getName().data());
    }

    auto textures = node->getTextures();
    auto textureNode = new QTreeWidgetItem(root);
    textureNode->setText(0, "Textures");
    for(const auto& texture : textures){
        auto textureItem = new QDataTreeItem<ITexture>(textureNode);
        textureItem->setData(texture);
        textureItem->setText(0, type(*texture).c_str());
        textureItem->setText(1, texture->getName().data());
    }

    // Adds the children of the node
    auto childrenNode = new QTreeWidgetItem(root);
    childrenNode->setText(0, "Children");

    for (auto n : node->getChildren()) {
        addNode(n, childrenNode);
    }
}

void RenderOptions::setScene(std::shared_ptr<Galax::Renderer::Scene> scene,
                             std::shared_ptr<Galax::Renderer::LightingModel> lighting,
                             const std::vector<std::shared_ptr<Galax::Renderer::IPostProcessEffect>>& effects) {
    this->scene = scene;
    this->lighting = lighting;
    this->effects = effects;
    addNode(scene->getRoot(), nullptr);
    addLighting(lighting);
    addEffects(effects);
}

void RenderOptions::addLighting(std::shared_ptr<Galax::Renderer::LightingModel> model) {
    auto root = new QTreeWidgetItem(nodeView);
    root->setText(0, "Lighting");
    root->setText(1, model->getName().data());


    auto textures = model->getTextures();
    auto textureNode = new QTreeWidgetItem(root);
    textureNode->setText(0, "Textures");
    for(const auto& texture : textures){
        auto textureItem = new QDataTreeItem<Texture>(textureNode);
        textureItem->setData(texture);
        textureItem->setText(0, type(*texture).c_str());
        textureItem->setText(1, texture->getName().data());
    }

    auto shader = model->getLightingShader();
    auto shaderNode = new QDataTreeItem<Shader>(root);
    shaderNode->setData(shader);
    shaderNode->setText(0, ("Shader (" + shader->getTypeString() + ")").data());
    shaderNode->setText(1, shader->getName().data());
};

void RenderOptions::addEffects(const std::vector<std::shared_ptr<Galax::Renderer::IPostProcessEffect>>& effects){
    auto root = new QTreeWidgetItem(nodeView);
    root->setText(0, "Effects");
    root->setText(1, "Effects");

    for(const auto& effect : effects){
        auto effectNode = new QTreeWidgetItem(root);
        effectNode->setText(0, type(*effect).c_str());
        effectNode->setText(1, effect->getName().data());

        auto inTextures = effect->getInputTextures();
        auto inTextureNode = new QTreeWidgetItem(effectNode);
        inTextureNode->setText(0, "Input textures");
        for(const auto& texture : inTextures){
            auto textureItem = new QDataTreeItem<ITexture>(inTextureNode);
            textureItem->setData(texture);
            textureItem->setText(0, type(*texture).c_str());
            textureItem->setText(1, texture->getName().data());
        }


        auto outTextures = effect->getOutputTextures();
        auto outTextureNode = new QTreeWidgetItem(effectNode);
        outTextureNode->setText(0, "Output textures");
        for(const auto& texture : outTextures){
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


std::string RenderOptions::demangle(const char *name) {
    int status = -4; // some arbitrary value to eliminate the compiler warning
    std::unique_ptr<char, void (*)(void*)> res{abi::__cxa_demangle(name, NULL, NULL, &status),std::free};
    std::string result = (status == 0) ? res.get() : name;
    std::size_t found = result.find_last_of("::");
    if(found != std::string::npos){
        result = result.substr(found + 1);
    }
    return result;
}

