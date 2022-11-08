//
// Created by tomas on 29.9.22.
//
#include <glbinding/gl/gl.h>

#include <iostream>
#include <effects/Bloom.h>
#include <renderer/impl/Shader.h>


using namespace gl;
using namespace Galax::Renderer;
using namespace Galax::Effects;
using namespace Galax::Renderer::SceneObjects;

Bloom::Bloom() : IPostProcessEffect("Bloom") {
}

Bloom::Bloom(const std::string& name) : IPostProcessEffect(name) {
}

void Bloom::init(){
    //Prep the temporary texture (used for blurring horizontally)
    auto tmpTexture = std::make_shared<Texture>(Texture::TYPE_2D, Texture::RGB, Texture::FLOAT, Texture::MIRRORED_REPEAT, Texture::Filtering::ANISOTROPIC);
    tmpTexture->setDimensions(1, 1);
    this->width = this->height = 1;
    textures[0] = tmpTexture;

    //The texture will be used by the first buffer as an output, and the second buffer as an input
    buffers[0] = std::make_shared<FrameBuffer>("Bloom buffer 0");
    buffers[0]->addOutputTexture(tmpTexture);
    buffers[0]->bind(true);

    buffers[1] = std::make_shared<FrameBuffer>("Bloom buffer 1");
    this->buffers[1]->clearOutputTextures();
    this->buffers[1]->addOutputTexture(textures[1]);
    buffers[1]->bind(true);
    buffers[1]->unbind();
    //Prep the quad that will be rendered to the screen
    quad = std::make_shared<Quad>("BloomQuad");
    quad->setFragShader(shader);
    program = quad->getProgram();

    this->directionUniform = std::make_shared<Uniform>("direction", Uniform::VEC2, glm::vec2(0,0));

}

void Bloom::render() {
    if(!program){ //This means that init() has not been called yet
        init();
    }
    glViewport(0, 0, width, height);
    program->bind();
    auto location = program->getTexturePosition(inputTexture);
    auto first = true;
    glActiveTexture(GL_TEXTURE0);


    for(int i = 0; i < this->passes; i++){
        auto horizontal = i % 2 == 0;
        buffers[horizontal]->bind(true);
        glBindTexture(GL_TEXTURE_2D, first ? inputTexture->getId() : textures[!horizontal]->getId());
        glUniform1i(location, 0);

        if(i % 2 == 0){
            directionUniform->setValue(glm::vec2(1,0));
        }else{
            directionUniform->setValue(glm::vec2(0,1));
        }

        program->setUniform(directionUniform);
        quad->draw();
        first = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Bloom::resize(int width, int height) {
    if(!program){ //This means that init() has not been called yet
        init();
    }
    width /= 4;
    height /= 4;
    buffers[0]->resize(width, height);
    buffers[1]->resize(width, height);
    this->width = width;
    this->height = height;
}

unsigned int Bloom::getId() {
    return this->getNameHash();
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
    textures[1] = texture;
}

void Bloom::setShader(std::shared_ptr<IShader> shader) {
    this->shader = shader;

}

void Bloom::setPasses(int passes) {
    this->passes = passes * 2; // Horizontal and vertical blur
}

int Bloom::getPasses() const {
    return this->passes / 2;
}

