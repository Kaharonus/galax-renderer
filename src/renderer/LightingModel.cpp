//
// Created by tomas on 14.9.22.
//
#include <glbinding/gl/gl.h>
#include "LightingModel.h"
#include "../Extensions.h"


using namespace gl;
using namespace Galax::Renderer;

LightingModel::LightingModel() : SceneObject("Lighting model") {
}

LightingModel::~LightingModel() {

}

void LightingModel::addTexture(std::shared_ptr<Texture> texture) {
    textures.push_back(texture);
}

void LightingModel::setLightningShader(std::shared_ptr<Shader> shader) {
    lightingShader = shader;
    if(quad){
        quad->setFragShader(shader);
    }
}

void LightingModel::init() {
    this->quad = std::make_shared<Quad>("Lighting model quad");
    quad->setFragShader(lightingShader);

    //Check if we need to create a render buffer
    if(outputTextures.empty()){
        return;
    }
    this->outputFrameBuffer = std::make_shared<FrameBuffer>("Lighting model output");
    for(auto& texture : outputTextures){
        this->outputFrameBuffer->addOutputTexture(texture);
    }
    this->outputFrameBuffer->resize(width, height);
}

void LightingModel::addUniform(std::shared_ptr<Uniform> uniform) {
    uniforms.push_back(uniform);
}

void LightingModel::draw() {
    if(!quad){
        init();
    }

    outputFrameBuffer->bind();
    auto lightingProgram = quad->getProgram();
    if(!lightingProgram){
        return;
    }
    lightingProgram->use();
    for(auto& uniform : uniforms) {
        lightingProgram->setUniform(uniform);
    }
    for(auto [i, texture] : enumerate(textures)) {
        lightingProgram->setTexture(texture, i);
    }
    quad->draw();
    outputFrameBuffer->unbind();
}


uint LightingModel::getId() {
    return 0;
}

std::vector<std::shared_ptr<Texture>> LightingModel::getTextures() {
    return textures;
}

std::vector<std::shared_ptr<Uniform>> LightingModel::getUniforms() {
    return uniforms;
}

std::shared_ptr<Shader> LightingModel::getLightingShader() {
    return lightingShader;
}

void LightingModel::addOutputTexture(std::shared_ptr<Texture> texture) {
    outputTextures.push_back(texture);
}

void LightingModel::resize(int width, int height) {
    this->width = width;
    this->height = height;
    if(outputFrameBuffer){
        outputFrameBuffer->resize(width, height);
    }
}
