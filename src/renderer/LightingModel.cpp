//
// Created by tomas on 14.9.22.
//
#include <glbinding/gl/gl.h>
#include "LightingModel.h"
#include "../Extensions.h"


using namespace gl;
using namespace Galax::Renderer;

LightingModel::LightingModel() : SceneObject("Lighting model") {
    this->lightCountUniform = std::make_shared<Uniform>("lightCount", Uniform::INT, 0);

}

LightingModel::~LightingModel() {
}

void LightingModel::addTexture(std::shared_ptr<ITexture> texture) {
    textures.push_back(texture);
}

void LightingModel::setLightningShader(std::shared_ptr<IShader> shader) {
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

    this->lightSSBO = std::make_shared<SSBO>("lightData");
    for(auto &light: lights){
        this->lightSSBO->addData(light->getLightData());
    }
}

void LightingModel::addUniform(std::shared_ptr<IUniform> uniform) {
    uniforms.push_back(uniform);
}

void LightingModel::draw() {
    if(!quad){
        init();
    }

    outputFrameBuffer->bind(true);
    auto lightingProgram = quad->getProgram();
    if(!lightingProgram){
        return;
    }
    lightingProgram->bind();
    for(auto& uniform : uniforms) {
        lightingProgram->setUniform(uniform);
    }
    for(auto [i, texture] : enumerate(textures)) {
        lightingProgram->setTexture(texture, i);
    }
    for(auto [i,light]: enumerate(lights)){
        if(light->isUpdated()){
            lightSSBO->setDataAt(light->getLightData(), i);
        }
    }

    lightingProgram->setUniform(lightCountUniform);
    lightingProgram->setSSBO(lightSSBO);

    quad->draw();
    outputFrameBuffer->unbind();
}


uint LightingModel::getId() {
    return 0;
}

std::vector<std::shared_ptr<ITexture>> LightingModel::getTextures() {
    return outputTextures;
}

std::vector<std::shared_ptr<IUniform>> LightingModel::getUniforms() {
    return uniforms;
}

std::shared_ptr<IShader> LightingModel::getLightingShader() {
    return lightingShader;
}

void LightingModel::addOutputTexture(std::shared_ptr<ITexture> texture) {
    outputTextures.push_back(texture);
}

void LightingModel::resize(int width, int height) {
    this->width = width;
    this->height = height;
    if(outputFrameBuffer){
        outputFrameBuffer->resize(width, height);
    }
}

void LightingModel::addLight(std::shared_ptr<ILight> light) {
    lights.push_back(light);
    lightCountUniform->setValue(lightCountUniform->getValue<int>() + 1);
}
