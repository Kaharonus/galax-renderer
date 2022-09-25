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


    // Get elements that are in both the input and the output
    // Since there are max 8 textures (by default) having an O(n^2) algorithm is fine
    std::vector<std::shared_ptr<Texture>> commonTextures;
    for (auto &inputTexture: inputTextures) {
        for (auto &outputTexture: outputTextures) {
            if(inputTexture != outputTexture){
                commonTextures.push_back(inputTexture);
            }
        }
    }
    frameBuffer = std::make_shared<FrameBuffer>("PostProcessFrameBuffer (" + this->name + ")");
    if(callCount == 1) {
        if(!commonTextures.empty()){
            //This would take too much unnecessary memory, so we don't allow it
            throw std::runtime_error("PostProcessEffect: Cannot have common input and output textures when callCount is 1 "
                                     "(the texture copy necessary is just a waste of resources)");
        }
        for (auto &texture: outputTextures) {
            frameBuffer->addOutputTexture(texture);
        }
        return;
    }



    // Create temporary textures
    for(auto &texture: commonTextures){
        auto temporaryTexture1 = std::make_shared<Texture>("1tmp_" + texture->getName(),
                                                          texture->getType(),
                                                          texture->getFormat(),
                                                          texture->getDataType(),
                                                          texture->getWrap(),
                                                          texture->getFiltering());
        temporaryTexture1->setDimensions(texture->getWidth(), texture->getHeight());

        auto temporaryTexture2 = std::make_shared<Texture>("2tmp_" + texture->getName(),
                                                           texture->getType(),
                                                           texture->getFormat(),
                                                           texture->getDataType(),
                                                           texture->getWrap(),
                                                           texture->getFiltering());
        temporaryTexture2->setDimensions(texture->getWidth(), texture->getHeight());

        temporaryTextures[texture] = {temporaryTexture1, temporaryTexture2};
    }
    secondFrameBuffer = std::make_shared<FrameBuffer>("PostProcessSecondaryFrameBuffer (" + this->name + ")");
    for (auto &texture: commonTextures) {
        auto [temporaryTexture1, temporaryTexture2] = temporaryTextures[texture];
        frameBuffer->addOutputTexture(temporaryTexture1);
        frameBuffer->addOutputTexture(temporaryTexture2);
        secondFrameBuffer->addOutputTexture(temporaryTexture1);
        secondFrameBuffer->addOutputTexture(temporaryTexture2);
    }
    for (auto &texture: outputTextures) {
        if(std::find(commonTextures.begin(), commonTextures.end(), texture) == commonTextures.end()){
            frameBuffer->addOutputTexture(texture); //Add only if it's not in the common textures
            secondFrameBuffer->addOutputTexture(texture);
        }
    }
}

unsigned int PostProcessEffect::getId() {
    return this->getNameHash();
}

void PostProcessEffect::drawSingle(){
    if (frameBuffer) {
        frameBuffer->bind(false);
    }
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
