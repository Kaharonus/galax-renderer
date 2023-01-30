
#include <glbinding/gl/gl.h>
#include "RenderNode.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace Galax::Renderer::SceneObjects;
using namespace Galax::Renderer;
using namespace gl;

RenderNode::RenderNode() : IRenderNode() {
    init();
}

RenderNode::RenderNode(const std::string &name) : IRenderNode(name) {
    init();
}

void RenderNode::init() {
    modelMatrixUniform = std::make_shared<Uniform>("model", Uniform::Type::MAT4, glm::mat4(1.0));
    transposeInverseModelUniform = std::make_shared<Uniform>("transposeInverseModel", Uniform::Type::MAT4,
                                                             glm::mat4(1.0));
    this->positionUniform = std::make_shared<Uniform>("position", Uniform::Type::VEC3, glm::vec3(0.0f));
    this->scaleUniform = std::make_shared<Uniform>("scale", Uniform::Type::VEC3, glm::vec3(1.0f));
	this->rotationUniform = std::make_shared<Uniform>("rotation", Uniform::Type::VEC3, glm::vec3(0.0f));
    this->objectIdUniform = std::make_shared<Uniform>("objectId", Uniform::Type::INT, (int)this->getId());
	this->currentTimeUniform = std::make_shared<Uniform>("currentTime", Uniform::Type::FLOAT, 0.0f);
	this->frameTimeUniform = std::make_shared<Uniform>("frameTime", Uniform::Type::FLOAT, 0.0f);
    setPosition(glm::vec3(0.0f));
    setRotation(glm::vec3(0.0f));
    setScale(glm::vec3(1.0f));

}


RenderNode::~RenderNode() {
}

void RenderNode::addTexture(std::shared_ptr<ITexture> texture) {
    textures.push_back(texture);
}


void RenderNode::addChild(std::shared_ptr<IRenderNode> child) {
	child->setParent(this->shared_from_this());
    children.push_back(child);
}

void RenderNode::removeChild(std::shared_ptr<RenderNode> child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void RenderNode::removeAllChildren() {
    children.clear();
}



void RenderNode::setPosition(const glm::vec3 &position) {
    this->position = position;
    this->positionUniform->setValue(position);
    calculateModelMatrix();
}

void RenderNode::setRotation(const glm::vec3 &rotation) {
    this->rotation = rotation;
	this->rotationUniform->setValue(rotation);
    calculateModelMatrix();

}

void RenderNode::setScale(const glm::vec3 &scale) {
    this->scale = scale;
    this->scaleUniform->setValue(scale);
    calculateModelMatrix();
}

void RenderNode::setMesh(std::shared_ptr<IMesh> mesh) {
    this->addLodLevel(mesh, 0.0f);
    this->mesh = mesh;
}


void RenderNode::setProgram(std::shared_ptr<IProgram> program) {
    this->program = program;
}

void RenderNode::setCamera(std::shared_ptr<ICamera> camera) {
    this->camera = camera;
}

void RenderNode::addUniform(std::shared_ptr<IUniform> uniform) {
    this->uniforms.push_back(uniform);
}

glm::mat4 &RenderNode::getModelMatrix() {
    return modelMatrix;
}

glm::vec3 RenderNode::getPosition() {
	return parent ? parent->getPosition() + position : position;
}

glm::vec3 RenderNode::getRotation() {
	return parent ? rotation + parent->getRotation() : rotation;
}

glm::vec3 RenderNode::getScale() {
	return parent ? parent->getScale() * scale : scale;
}

std::vector<std::shared_ptr<IRenderNode>> RenderNode::getChildren() const {
    return children;
}

std::shared_ptr<IMesh> RenderNode::getMesh() const {
    return meshLODs.begin()->second;
}


std::shared_ptr<ICamera> RenderNode::getCamera() const {
    return camera;
}


uint RenderNode::getId() {
    return getNameHash();
}

void RenderNode::addLodLevel(std::shared_ptr<IMesh> lod, float distance) {
    meshLODs[distance] = lod;
}

void RenderNode::addAnimation(std::shared_ptr<IAnimation> animation) {
    animations.push_back(animation);
}

void RenderNode::calculateModelMatrix() {
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, scale);
	modelMatrixUniform->setValue(modelMatrix);
	transposeInverseModelUniform->setValue(glm::transpose(glm::inverse(modelMatrix)));
}

float RenderNode::getDistance() {
    auto cameraPos = camera->getPosition();
    auto center = meshLODs.begin()->second->getCenter() + position;
    auto len = glm::length(cameraPos - center);
    return len;

}

void RenderNode::selectLOD(float distance) {
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


void RenderNode::useDefaultUniforms() {
    if (camera == nullptr) {
        return;
    }
    setDrawSize(camera->getWidth(), camera->getHeight());
    program->setUniform(camera->getViewUniform());
    program->setUniform(camera->getProjectionUniform());
    program->setUniform(camera->getPositionUniform());
	program->setUniform(this->positionUniform);
	program->setUniform(this->scaleUniform);
	program->setUniform(this->rotationUniform);
    program->setUniform(camera->getRotationUniform());
	program->setUniform(camera->getResolutionUniform());
	program->setUniform(camera->getForwardUniform());
    program->setUniform(modelMatrixUniform);

    program->setUniform(transposeInverseModelUniform);
    program->setUniform(objectIdUniform);
	currentTimeUniform->setValue((float)SceneObject::getCurrentTime());
	program->setUniform(currentTimeUniform);
	frameTimeUniform->setValue((float)SceneObject::getFrameTime());
	program->setUniform(frameTimeUniform);
}


void RenderNode::updateAnimations() {
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

void RenderNode::draw(glm::mat4 parentModel) {

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



	this->currentDrawMatrix = parentModel * this->modelMatrix;
	this->modelMatrixUniform->setValue(currentDrawMatrix);
	this->transposeInverseModelUniform->setValue(glm::transpose(glm::inverse(this->modelMatrix)));
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

void RenderNode::setMeshWithLOD(std::map<float, std::shared_ptr<IMesh>> meshMap) {
    meshLODs = std::move(meshMap);
    auto mainLod = meshLODs.begin()->second;
    meshLODs.erase(meshLODs.begin());
    meshLODs[0] = mainLod;
    this->mesh = mainLod;
}

std::vector<std::shared_ptr<IUniform>> RenderNode::getUniforms() const {
    return uniforms;
}

std::vector<std::shared_ptr<ITexture>> RenderNode::getTextures() const {
    return textures;
}

void RenderNode::setDrawTarget(RenderNode::DrawTarget target) {
    drawTarget = target;
}

RenderNode::DrawTarget RenderNode::getDrawTarget() const {
    return drawTarget;
}

void RenderNode::setDrawTexture(std::shared_ptr<ITexture> texture) {
    assert(drawTarget == DrawTarget::TEXTURE);
    drawTexture = texture;
}

std::vector<std::shared_ptr<IAnimation>> RenderNode::getAnimations() const {
    return animations;
}

std::shared_ptr<IUniform> RenderNode::getPositionUniform() {
    return positionUniform;
}

std::vector<std::shared_ptr<IProgram>> RenderNode::getPrograms() const {
    return {program};
}

void RenderNode::drawAsWireframe(bool enabled) {
    wireframe = enabled;
}

std::shared_ptr<IUniform> RenderNode::getScaleUniform() {
    return scaleUniform;
}

void RenderNode::setTransparent(bool transparent) {
	this->transparent = transparent;
}

bool RenderNode::isTransparent() const {
	return transparent;
}

void RenderNode::setLightingModel(std::shared_ptr<LightingModel> lightingModel) {
	this->lightingModel = lightingModel;
}

void RenderNode::setParent(std::shared_ptr<IRenderNode> parent) {
	this->parent = parent;
}

std::shared_ptr<IRenderNode> RenderNode::getParent() const {
	return parent;
}

const glm::vec3 &RenderNode::getRelativePosition() {
	return position;
}

const glm::vec3 &RenderNode::getRelativeRotation() {
	return rotation;
}

const glm::vec3 &RenderNode::getRelativeScale() {
	return scale;
}

const glm::mat4 &RenderNode::getRenderModelMatrix() {
	return currentDrawMatrix;
}

fpgen::generator<std::shared_ptr<IRenderNode>> RenderNode::getAllChildren(std::shared_ptr<IRenderNode> node) {
	for(const auto& child: node->getChildren()){
		co_yield child;
		for(auto c: getAllChildren(child)){
			co_yield c;
		}
	}
}
