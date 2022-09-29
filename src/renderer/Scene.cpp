#include <glbinding/gl/gl.h>

#include "Scene.h"

using namespace gl;
using namespace Galax::Renderer;

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::setInputHandler(std::shared_ptr<InputHandler> inputHandler) {
    this->inputHandler = inputHandler;
    for (auto c: this->cameras) {
        c->setInputHandler(inputHandler);
    }
}

void Scene::addMesh(std::shared_ptr<IMesh> mesh) {
    this->meshes.insert(mesh);
}

void Scene::addProgram(std::shared_ptr<IProgram> program) {
    this->programs.insert(program);
}

void Scene::addShader(std::shared_ptr<IShader> shader) {
    this->shaders.insert(shader);
}

void Scene::addTexture(std::shared_ptr<ITexture> texture) {
    this->textures.insert(texture);
}

void Scene::addCamera(std::shared_ptr<ICamera> camera) {
    this->cameras.insert(camera);
}

void Scene::setRoot(std::shared_ptr<INode> parent) {
    root = parent;
}

std::shared_ptr<INode> Scene::getRoot() {
    return root;
}

void Scene::buildNode(const INode &node) {
    if (node.getMesh() != nullptr) {
        addMesh(node.getMesh());
    }
    if (node.getProgram() != nullptr) {
        auto program = node.getProgram();
        for (auto& shader: program->getShaders()) {
            addShader(shader);
        }
        addProgram(program);
        addProgram(node.getProgram());
    }

    if (node.getCamera() != nullptr) {
        auto camera = node.getCamera();
        camera->setInputHandler(inputHandler);
        addCamera(camera);
    }
    for (auto& child: node.getChildren()) {
        buildNode(*child);
    }
}

void Scene::build() {
    buildNode(*root);
}

void Scene::drawNode(INode &node) {
    node.draw();
    for (auto& child: node.getChildren()) {
        drawNode(*child);
    }
}

void Scene::draw() {
    assert(gBuffer != nullptr);

    for (auto& camera: this->cameras) {
        camera->update();
    }

    gBuffer->bind();
    drawNode(*root);
    gBuffer->unbind();

}

void Scene::setDimensions(int w, int h) {
    this->width = w;
    this->height = h;
    for (const auto &camera: this->cameras) {
        camera->setDimensions(width, height);
    }
}

void Scene::setGBuffer(std::shared_ptr<GBuffer> buffer) {
    this->gBuffer = buffer;
}

