//
// Created by tomas on 30.9.22.
//
#include <glbinding/gl/gl.h>
#include "SSBO.h"
#include "assert.h"

using namespace gl;
using namespace Galax::Renderer::SceneObjects;

SSBO::SSBO() : ISSBO(){ };

SSBO::SSBO(const std::string &name) : ISSBO(name) {

}

void SSBO::init() {
    glGenBuffers(1, &id);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
    glBufferData(GL_SHADER_STORAGE_BUFFER, data.size(), data.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}


void SSBO::setData(const std::vector<char> &data) {
    updated = true;
    this->data = data;
}



void SSBO::bind() {
    if(id == 0){
        init();
    }
    if(updated){
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
        glBufferData(GL_SHADER_STORAGE_BUFFER, data.size(), data.data(), GL_DYNAMIC_DRAW);
        updated = false;
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
}

void SSBO::unbind() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

template<typename T>
void SSBO::setDataAt(const T &data, unsigned int index) {
    auto sizeT = sizeof(T);
    if(index * sizeT > this->data.size()){
        this->data.resize(index * sizeT);
    }
    memcpy(&this->data[index * sizeT], &data, sizeT);
    updated = true;
}

template<typename T>
void SSBO::addData(const T &data) {
    auto sizeT = sizeof(T);
    this->data.resize(this->data.size() + sizeT);
    memcpy(&this->data[this->data.size() - sizeT], &data, sizeT);
    updated = true;
}

SSBO::~SSBO() {
    if(id != 0){
        glDeleteBuffers(1, &id);
    }

}

uint SSBO::getId() {
    return id;
}

