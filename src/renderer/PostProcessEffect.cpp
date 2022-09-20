//
// Created by tomas on 20.9.22.
//
#include <glbinding/gl/gl.h>


#include "PostProcessEffect.h"
#include "../Extensions.h"

using namespace gl;
using namespace Galax::Renderer;


PostProcessEffect::PostProcessEffect() : SceneObject() {

}

PostProcessEffect::PostProcessEffect(const std::string &name) : SceneObject(name) {

}

void PostProcessEffect::init() {
    initialized = true;
    quad = std::make_shared<Quad>("PostProcessQuad (" + this->name + ")");
    quad->setFragShader(shader);

    if(outputTextures.empty()){
        return;
    }

    frameBuffer = std::make_shared<FrameBuffer>("PostProcessFrameBuffer (" + this->name + ")");
    for (auto &texture: outputTextures) {
        frameBuffer->addOutputTexture(texture);
    }

}

unsigned int PostProcessEffect::getId() {
    return this->getNameHash();
}

void PostProcessEffect::draw() {
    if (!initialized) {
        init();
    }
    if (frameBuffer) {
        frameBuffer->bind();
    }
    auto program = quad->getProgram();
    program->use();
    for (auto [i, texture]: enumerate(inputTextures)) {
        program->setTexture(texture, i);
    }
    quad->draw();
    if (frameBuffer) {
        frameBuffer->unbind();
    }
}


void PostProcessEffect::addInputTexture(std::shared_ptr<Texture> texture) {
    inputTextures.push_back(texture);
}

void PostProcessEffect::addOutputTexture(std::shared_ptr<Texture> texture) {
    outputTextures.push_back(texture);
}

void PostProcessEffect::resize(int width, int height) {
    if (!initialized) {
        init();
    }
    if (frameBuffer) {
        frameBuffer->resize(width, height);
    }
}

void PostProcessEffect::setShader(std::shared_ptr<Shader> shader) {
    this->shader = shader;
    if (initialized) {
        quad->setFragShader(shader);
    }

}

std::vector<std::shared_ptr<Texture>> PostProcessEffect::getOutputTextures() const {
    return outputTextures;
}

std::vector<std::shared_ptr<Texture>> PostProcessEffect::getInputTextures() const {
    return inputTextures;
}

std::shared_ptr<Shader> PostProcessEffect::getShader() const {
    return shader;
}

std::shared_ptr<FrameBuffer> PostProcessEffect::getFrameBuffer() const {
    return frameBuffer;
}
