
#include <glbinding/gl/gl.h>
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <iostream>

using namespace Galax::Renderer::SceneObjects;
using namespace gl;

Node::Node() : SceneObject() {
    init();
}

Node::Node(const std::string &name) : SceneObject(name) {
    init();
}

Node::Node(const std::string &name, std::shared_ptr<Node> parent) : SceneObject(name) {
    init();
    this->parent = parent;
}

void Node::init(){
    modelMatrixUniform = std::make_shared<Uniform>("model", Uniform::Type::MAT4, glm::mat4(1.0));

    setPosition(glm::vec3(0.0f));
    setRotation(glm::vec3(0.0f));
    setScale(glm::vec3(1.0f));
}


Node::~Node() {
}

void Node::addTexture(std::shared_ptr<Texture> texture) {
    textures.push_back(texture);
}


void Node::addChild(std::shared_ptr<Node> child) {
    children.push_back(child);
}

void Node::removeChild(std::shared_ptr<Node> child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void Node::removeAllChildren() {
    children.clear();
}

void Node::setParent(std::shared_ptr<Node> parent) {
    this->parent = parent;
}


void Node::setPosition(const glm::vec3 &position) {
    this->position = position;
    calculateModelMatrix();
}

void Node::setRotation(const glm::vec3 &rotation) {
    this->rotation = rotation;
    calculateModelMatrix();

}

void Node::setScale(const glm::vec3 &scale) {
    this->scale = scale;
    calculateModelMatrix();
}

void Node::setMesh(std::shared_ptr<Mesh> mesh) {
    this->addLodLevel(mesh, 0.0f);
}

void Node::setMaterial(std::shared_ptr<Material> material) {
    this->material = material;
}

void Node::setProgram(std::shared_ptr<Program> program) {
    this->program = program;
}

void Node::setCamera(std::shared_ptr<Camera> camera) {
    this->camera = camera;
}

void Node::addUniform(std::shared_ptr<Uniform> uniform) {
    this->uniforms.push_back(uniform);
}

glm::mat4 &Node::getModelMatrix() {
    return modelMatrix;
}

glm::vec3 &Node::getPosition() {
    return position;
}

glm::vec3 &Node::getRotation() {
    return rotation;
}

glm::vec3 &Node::getScale() {
    return scale;
}

std::shared_ptr<Node> Node::getParent() const {
    return parent;
}

std::vector<std::shared_ptr<Node>> Node::getChildren() const {
    return children;
}

std::shared_ptr<Mesh> Node::getMesh() const {
    return meshLODs.begin()->second;
}

std::shared_ptr<Material> Node::getMaterial() const {
    return material;
}

std::shared_ptr<Program> Node::getProgram() const {
    return program;
}

std::shared_ptr<Camera> Node::getCamera() const {
    return camera;
}


uint Node::getId() {
    return getNameHash();
}

void Node::addLodLevel(std::shared_ptr<Mesh> lod, float distance) {
    meshLODs[distance] = lod;
}

void Node::addAnimation(std::shared_ptr<Animation> animation) {
    animations.push_back(animation);
}

void Node::calculateModelMatrix() {
    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrixUniform->setValue(modelMatrix);
}

float Node::getDistance() {
    auto cameraPos = camera->getPosition();
    auto center = meshLODs.begin()->second->getCenter() + position;
    auto len = glm::length(cameraPos - center);
    return len;

}

void Node::selectLOD(float distance) {
    distance = std::abs(distance);
    auto it = meshLODs.begin();
    while(it != meshLODs.end()) {
        auto current = it;
        auto next = std::next(it);
        if(distance > current->first && distance < next->first) {
            mesh = current->second;
            return;
        }
        it++;
    }
    mesh = meshLODs.rbegin()->second;

}


void Node::useCamera() {
    if (camera == nullptr) {
        return;
    }
    glViewport(0, 0, camera->getWidth(), camera->getHeight());
    program->setUniform(camera->getViewUniform());
    program->setUniform(camera->getProjectionUniform());
    program->setUniform(camera->getPositionUniform());
    program->setUniform(camera->getRotationUniform());
    program->setUniform(modelMatrixUniform);

}


void Node::updateAnimations(){
    for(const auto& anim: animations){
        anim->update();
        switch(anim->getTarget()){
            case Animation::POSITION:
                setPosition(anim->getValue());
                break;
            case Animation::ROTATION:
                setRotation(anim->getValue());
                break;
            case Animation::SCALE:
                setScale(anim->getValue());
                break;
            case Animation::CUSTOM:
                program->setUniform(anim->getCustomTarget());
                break;
        }
    }
}

void Node::draw() {
    if (meshLODs.empty()) {
        return;
    }
    if (program == nullptr) {
        return;
    }

    program->use();

    updateAnimations();

    for (auto &uniform: uniforms) {
        program->setUniform(uniform);
    }

    useCamera();
    auto i = program->getTextureCount();
    for (auto &texture: textures) {
        //texture->bind(i);
        program->setTexture(texture, i);
        i++;
    }

    auto distance = getDistance();
    selectLOD(distance);
    mesh->bind();

    glDrawElements(GL_TRIANGLES, (int)mesh->size(), GL_UNSIGNED_INT, 0);

}

void Node::setMeshWithLOD(std::map<float, std::shared_ptr<Mesh>> meshMap) {
    meshLODs = std::move(meshMap);
    auto mainLod = meshLODs.begin()->second;
    meshLODs.erase(meshLODs.begin());
    meshLODs[0] = mainLod;
    this->mesh = mainLod;
}

std::vector<std::shared_ptr<Uniform>> Node::getUniforms() const {
    return uniforms;
}

std::vector<std::shared_ptr<Texture>> Node::getTextures() const {
    return textures;
}
