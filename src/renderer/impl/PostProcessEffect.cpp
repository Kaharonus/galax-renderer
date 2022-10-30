//
// Created by tomas on 20.9.22.
//
#include <glbinding/gl/gl.h>


#include "PostProcessEffect.h"
#include "../../Extensions.h"

using namespace gl;
using namespace Galax::Renderer;


PostProcessEffect::PostProcessEffect() : IPostProcessEffect() {

}

PostProcessEffect::PostProcessEffect(const std::string &name) : IPostProcessEffect(name) {

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
    for(auto &texture: outputTextures){
        frameBuffer->addOutputTexture(texture);
    }

}

unsigned int PostProcessEffect::getId() {
    return this->getNameHash();
}

void PostProcessEffect::render() {
    if (!initialized) {
        init();
    }
    setDrawSize(width, height);
    if (frameBuffer) {
        frameBuffer->bind(false);
    }
    auto program = quad->getProgram();
    program->bind();
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


void PostProcessEffect::addInputTexture(std::shared_ptr<ITexture> texture) {
    inputTextures.push_back(texture);
}

void PostProcessEffect::addOutputTexture(std::shared_ptr<ITexture> texture) {
    outputTextures.push_back(texture);
}

void PostProcessEffect::resize(int width, int height) {
    if (!initialized) {
        init();
    }
    if (frameBuffer) {
        frameBuffer->resize(width, height);
    }
    this->width = width;
    this->height = height;
}

void PostProcessEffect::setShader(std::shared_ptr<Shader> shader) {
    this->shader = shader;
    if (initialized) {
        quad->setFragShader(shader);
    }

}

std::vector<std::shared_ptr<ITexture>> PostProcessEffect::getOutputTextures() const {
    return outputTextures;
}

std::vector<std::shared_ptr<ITexture>> PostProcessEffect::getInputTextures() const {
    return inputTextures;
}

std::shared_ptr<IShader> PostProcessEffect::getShader() const {
    return shader;
}

std::shared_ptr<FrameBuffer> PostProcessEffect::getFrameBuffer() const {
    return frameBuffer;
}

void PostProcessEffect::addUniform(std::shared_ptr<Uniform> uniform) {
    uniforms.push_back(uniform);
}
