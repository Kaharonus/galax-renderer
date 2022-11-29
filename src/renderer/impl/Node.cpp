
#include <glbinding/gl/gl.h>
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <iostream>

using namespace Galax::Renderer::SceneObjects;
using namespace Galax::Renderer;
using namespace gl;

Node::Node() : IRenderNode() {
    init();
}

Node::Node(const std::string &name) : IRenderNode(name) {
    init();
}

void Node::init() {
    modelMatrixUniform = std::make_shared<Uniform>("model", Uniform::Type::MAT4, glm::mat4(1.0));
    transposeInverseModelUniform = std::make_shared<Uniform>("transposeInverseModel", Uniform::Type::MAT4,
                                                             glm::mat4(1.0));
    this->positionUniform = std::make_shared<Uniform>("position", Uniform::Type::VEC3, glm::vec3(0.0f));
    this->scaleUniform = std::make_shared<Uniform>("scale", Uniform::Type::VEC3, glm::vec3(1.0f));
    this->objectIdUniform = std::make_shared<Uniform>("objectId", Uniform::Type::INT, (int)this->getId());
	this->currentTimeUniform = std::make_shared<Uniform>("currentTime", Uniform::Type::FLOAT, 0.0f);
	this->frameTimeUniform = std::make_shared<Uniform>("frameTime", Uniform::Type::FLOAT, 0.0f);
    setPosition(glm::vec3(0.0f));
    setRotation(glm::vec3(0.0f));
    setScale(glm::vec3(1.0f));

}


Node::~Node() {
}

void Node::addTexture(std::shared_ptr<ITexture> texture) {
    textures.push_back(texture);
}


void Node::addChild(std::shared_ptr<IRenderNode> child) {
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
    this->positionUniform->setValue(position);
    calculateModelMatrix();
}

void Node::setRotation(const glm::vec3 &rotation) {
    this->rotation = rotation;
    calculateModelMatrix();

}

void Node::setScale(const glm::vec3 &scale) {
    this->scale = scale;
    this->scaleUniform->setValue(scale);
    calculateModelMatrix();
}

void Node::setMesh(std::shared_ptr<IMesh> mesh) {
    this->addLodLevel(mesh, 0.0f);
    this->mesh = mesh;
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

std::vector<std::shared_ptr<IRenderNode>> Node::getChildren() const {
    return children;
}

std::shared_ptr<IMesh> Node::getMesh() const {
    return meshLODs.begin()->second;
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
    auto translationMatrix = glm::translate(glm::mat4(1.0f), position);
    auto rotationMatrix = glm::mat4_cast(glm::quat(glm::radians(rotation)));
    auto scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
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


void Node::useDefaultUniforms() {
    if (camera == nullptr) {
        return;
    }
    setDrawSize(camera->getWidth(), camera->getHeight());
    program->setUniform(camera->getViewUniform());
    program->setUniform(camera->getProjectionUniform());
    program->setUniform(camera->getPositionUniform());
    program->setUniform(camera->getRotationUniform());
	program->setUniform(camera->getResolutionUniform());
	program->setUniform(camera->getForwardUniform());
    program->setUniform(modelMatrixUniform);

    program->setUniform(transposeInverseModelUniform);
    program->setUniform(objectIdUniform);
	currentTimeUniform->setValue((float)currentTime);
	program->setUniform(currentTimeUniform);
	frameTimeUniform->setValue((float)frameTime);
	program->setUniform(frameTimeUniform);
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

void Node::draw(glm::mat4 parentModel) {

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



	auto currentMatrix = parentModel * this->modelMatrix;
	this->modelMatrixUniform->setValue(currentMatrix);
	this->transposeInverseModelUniform->setValue(glm::transpose(glm::inverse(currentMatrix)));
    useDefaultUniforms();
    auto i = program->getTextureCount();
    for (auto &texture: textures) {
        //texture->bind(i);
        program->setTexture(texture, i);
        i++;
    }

	if(this->isTransparent()){
		program->setUniform(this->lightingModel->getLightCountUniform());
		program->setSSBO(this->lightingModel->getLightSSBO());
	}

    auto distance = getDistance();
    selectLOD(distance);
    mesh->bind();
    auto drawMode = GL_TRIANGLES;

    if (program->hasTesslation()) {
        glPatchParameteri(GL_PATCH_VERTICES, 3);
        drawMode = GL_PATCHES;
    }
    if(wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    glDrawElements(drawMode, (int) mesh->size(), GL_UNSIGNED_INT, 0);

    if(wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
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

std::shared_ptr<IUniform> Node::getPositionUniform() {
    return positionUniform;
}

std::vector<std::shared_ptr<IProgram>> Node::getPrograms() const {
    return {program};
}

void Node::drawAsWireframe(bool enabled) {
    wireframe = enabled;
}

std::shared_ptr<IUniform> Node::getScaleUniform() {
    return scaleUniform;
}

void Node::setTransparent(bool transparent) {
	this->transparent = transparent;
}

bool Node::isTransparent() const {
	return transparent;
}

void Node::setLightingModel(std::shared_ptr<LightingModel> lightingModel) {
	this->lightingModel = lightingModel;
}
