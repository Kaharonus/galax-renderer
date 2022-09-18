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
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
}

void LightingModel::addTexture(std::shared_ptr<Texture> texture) {
    textures.push_back(texture);
}

void LightingModel::setLightningShader(std::shared_ptr<Shader> shader) {
    lightingShader = shader;
}

void LightingModel::init() {
    //prepare the vao
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    vertexShader = std::make_shared<Shader>("lightning quad shader", Shader::VERTEX);
    vertexShader->setSource(vertexShaderSource);

    lightingProgram = std::make_shared<Program>("Lighting program", vertexShader, lightingShader);
    lightingProgram->compile();

}

void LightingModel::addUniform(std::shared_ptr<Uniform> uniform) {
    uniforms.push_back(uniform);
}

void LightingModel::draw() {

    if(lightingProgram == nullptr) {
        init();
    }

    lightingProgram->use();
    for(auto uniform : uniforms) {
        lightingProgram->setUniform(uniform);
    }
    for(auto [i, texture] : enumerate(textures)) {
        lightingProgram->setTexture(texture, i);
    }

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);



}

void LightingModel::unbind() {

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
