
#include <glbinding/gl/gl.h>
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <iostream>

using namespace Galax::Renderer::SceneObjects;
using namespace Galax::Renderer;
using namespace gl;

Node::Node() : INode() {
    init();
}

Node::Node(const std::string &name) : INode(name) {
    init();
}

void Node::init() {
    modelMatrixUniform = std::make_shared<Uniform>("model", Uniform::Type::MAT4, glm::mat4(1.0));
    transposeInverseModelUniform = std::make_shared<Uniform>("transposeInverseModel", Uniform::Type::MAT4,
                                                             glm::mat4(1.0));

    setPosition(glm::vec3(0.0f));
    setRotation(glm::vec3(0.0f));
    setScale(glm::vec3(1.0f));
}


Node::~Node() {
}

void Node::addTexture(std::shared_ptr<ITexture> texture) {
    textures.push_back(texture);
}


void Node::addChild(std::shared_ptr<INode> child) {
    children.push_back(child);
}

void Node::removeChild(std::shared_ptr<Node> child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void Node::removeAllChildren() {
    children.clear();
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

void Node::setMesh(std::shared_ptr<IMesh> mesh) {
    this->addLodLevel(mesh, 0.0f);
}


void Node::setProgram(std::shared_ptr<IProgram> program) {
    this->program = program;
}

void Node::setCamera(std::shared_ptr<ICamera> camera) {
    this->camera = camera;
}

void Node::addUniform(std::shared_ptr<IUniform> uniform) {
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

std::vector<std::shared_ptr<INode>> Node::getChildren() const {
    return children;
}

std::shared_ptr<IMesh> Node::getMesh() const {
    return meshLODs.begin()->second;
}


std::shared_ptr<IProgram> Node::getProgram() const {
    return program;
}

std::shared_ptr<ICamera> Node::getCamera() const {
    return camera;
}


uint Node::getId() {
    return getNameHash();
}

void Node::addLodLevel(std::shared_ptr<IMesh> lod, float distance) {
    meshLODs[distance] = lod;
}

void Node::addAnimation(std::shared_ptr<IAnimation> animation) {
    animations.push_back(animation);
}

void Node::calculateModelMatrix() {
    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrixUniform->setValue(modelMatrix);
    transposeInverseModelUniform->setValue(glm::transpose(glm::inverse(modelMatrix)));
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
    while (it != meshLODs.end()) {
        auto current = it;
        auto next = std::next(it);
        if (distance > current->first && distance < next->first) {
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
    program->setUniform(transposeInverseModelUniform);

}


void Node::updateAnimations() {
    for (const auto &anim: animations) {
        anim->update();
        switch (anim->getTarget()) {
            case IAnimation::POSITION:
                setPosition(anim->getValue());
                break;
            case IAnimation::ROTATION:
                setRotation(anim->getValue());
                break;
            case IAnimation::SCALE:
                setScale(anim->getValue());
                break;
            case IAnimation::CUSTOM:
                program->setUniform(anim->getCustomTarget());
                break;
        }
    }
}

void Node::draw() {

    if (meshLODs.empty()) { // There is no mesh to draw
        return;
    }
    if (program == nullptr) {
        return;
    }
    if (drawTarget == DrawTarget::TEXTURE) {
        if(!frameBuffer){
            assert(drawTexture != nullptr);
            frameBuffer = std::make_shared<FrameBuffer>("Node " + name + " FrameBuffer");
            frameBuffer->addOutputTexture(drawTexture);
            auto [w, h, _] = drawTexture->getDimensions();
            frameBuffer->resize(w, h);
        }
        frameBuffer->bind(true);
    }

    program->bind();

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
    auto drawMode = GL_TRIANGLES;

    if (program->hasTesslation()) {
        glPatchParameteri(GL_PATCH_VERTICES, 3);
        drawMode = GL_PATCHES;
    }
    glDrawElements(drawMode, (int) mesh->size(), GL_UNSIGNED_INT, 0);
    if(drawTarget == DrawTarget::TEXTURE){
        frameBuffer->unbind();
    }


}

void Node::setMeshWithLOD(std::map<float, std::shared_ptr<IMesh>> meshMap) {
    meshLODs = std::move(meshMap);
    auto mainLod = meshLODs.begin()->second;
    meshLODs.erase(meshLODs.begin());
    meshLODs[0] = mainLod;
    this->mesh = mainLod;
}

std::vector<std::shared_ptr<IUniform>> Node::getUniforms() const {
    return uniforms;
}

std::vector<std::shared_ptr<ITexture>> Node::getTextures() const {
    return textures;
}

void Node::setDrawTarget(Node::DrawTarget target) {
    drawTarget = target;
}

Node::DrawTarget Node::getDrawTarget() const {
    return drawTarget;
}

void Node::setDrawTexture(std::shared_ptr<ITexture> texture) {
    assert(drawTarget == DrawTarget::TEXTURE);
    drawTexture = texture;
}

std::vector<std::shared_ptr<IAnimation>> Node::getAnimations() const {
    return animations;
}
