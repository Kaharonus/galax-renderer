//
// Created by tomas on 29.9.22.
//

#include "Bloom.h"
#include "../renderer/impl/Shader.h"

using namespace Galax::Renderer;
using namespace Galax::Effects;
using namespace Galax::Renderer::SceneObjects;

Bloom::Bloom() : IPostProcessEffect("Bloom") {
    init();
}

Bloom::Bloom(const std::string& name) : IPostProcessEffect(name) {
    init();
}

void Bloom::init(){
    buffers[0] = std::make_shared<FrameBuffer>("Bloom buffer 0");
    buffers[1] = std::make_shared<FrameBuffer>("Bloom buffer 1");

}

void Bloom::render() {

}

void Bloom::resize(int width, int height) {
    buffers[0]->resize(width, height);
    buffers[1]->resize(width, height);
}

unsigned int Bloom::getId() {
    return 0;
}

std::vector<std::shared_ptr<ITexture>> Bloom::getInputTextures() const {
    return {inputTexture};
}

std::vector<std::shared_ptr<ITexture>> Bloom::getOutputTextures() const {
    return {outputTexture};
}

std::shared_ptr<IShader> Bloom::getShader() const {
    return shader;
}

void Bloom::setInputTexture(std::shared_ptr<ITexture> texture) {
    inputTexture = texture;
}

void Bloom::setOutputTexture(std::shared_ptr<ITexture> texture) {
    outputTexture = texture;
}

void Bloom::setShader(std::shared_ptr<IShader> shader) {
    this->shader = shader;
}

