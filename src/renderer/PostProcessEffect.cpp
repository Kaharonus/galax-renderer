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

    callCountUniform = std::make_shared<Uniform>("currentCall", Uniform::Type::INT, 1);

    if(outputTextures.empty()){
        return;
    }

    frameBuffer = std::make_shared<FrameBuffer>("PostProcessFrameBuffer (" + this->name + ")");
    for (auto &texture: outputTextures) {
        frameBuffer->addOutputTexture(texture);
    }

    if(this->callCount > 1){ // We need a secondary fbo to pingpong the data
        secondFrameBuffer = std::make_shared<FrameBuffer>("PostProcessSecondaryFrameBuffer (" + this->name + ")");
        for (auto &texture: outputTextures) {
            secondFrameBuffer->addOutputTexture(texture);
        }
    }

}

unsigned int PostProcessEffect::getId() {
    return this->getNameHash();
}

void PostProcessEffect::drawSingle(){
    auto program = quad->getProgram();
    program->use();
    for (auto [i, texture]: enumerate(inputTextures)) {
        program->setTexture(texture, i);
    }
    for (auto &uniform: uniforms) {
        program->setUniform(uniform);
    }
    quad->draw();
    if (frameBuffer) {
        frameBuffer->unbind();
    }
}

void PostProcessEffect::drawMultiple(){

}

void PostProcessEffect::draw() {
    if (!initialized) {
        init();
    }
    if (frameBuffer) {
        frameBuffer->bind(false);
    }
    if(callCount == 1){
        drawSingle();
    } else {
        drawMultiple();
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

void PostProcessEffect::addUniform(std::shared_ptr<Uniform> uniform) {
    uniforms.push_back(uniform);
}

void PostProcessEffect::setCallCount(int count) {
    callCount = count;
}

int PostProcessEffect::getCallCount() const {
    return callCount;
}
