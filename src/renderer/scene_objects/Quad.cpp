//
// Created by tomas on 20.9.22.
//

#include <glbinding/gl/gl.h>

#include "Quad.h"

using namespace gl;
using namespace Galax::Renderer::SceneObjects;

Quad::Quad() : SceneObject() {}

Quad::Quad(const std::string &name) : SceneObject(name) {
}

void Quad::setFragShader(std::shared_ptr<Shader> shader) {
    if (!initialized) {
        init();
    }
    assert(shader->getType() == Shader::FRAGMENT);
    this->fragShader = shader;
    this->program = std::make_shared<Program>("Quad Program (" + this->name + ")", vertexShader, shader);
}

std::shared_ptr<Program> Quad::getProgram() const {
    return program;
}

void Quad::draw() {
    if (!initialized) {
        init();
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Quad::init() {
    initialized = true;
    vertexShader = std::make_shared<Shader>("QuadVertexShader", vertexShaderSource, Shader::VERTEX);

    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

Quad::~Quad() {
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
}

unsigned int Quad::getId() {
    return this->getNameHash();
}
