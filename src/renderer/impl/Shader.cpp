#include <glbinding/gl/gl.h>
#include "Shader.h"
#include <memory>
#include <stdexcept>
#include <iostream>

using namespace Galax::Renderer::SceneObjects;
using namespace gl;

Shader::Shader() : IShader() {
    this->type = Type::None;
}

Shader::~Shader() {
    if(this->id != 0) {
        glDeleteShader(this->id);
    }
}


Shader::Shader(const std::string& name, const std::string& source, Shader::Type type) : IShader(name) {
    this->type = type;
    this->code = source;
}

void Shader::setSource(const std::string& source) {
    this->code = source;
    this->updated = true;
}

void Shader::setType(Shader::Type type) {
    this->type = type;
    this->updated = true;
}

std::string Shader::getInfoLog() {
    return this->infoLog;
}

std::string& Shader::getSource() {
    return this->code;
}

Shader::Type Shader::getType() {
    return this->type;
}

std::string Shader::getTypeString() {
    switch (type) {
    case Type::None:
        return "None";
    case Type::VERTEX:
        return "VERTEX";
    case Type::FRAGMENT:
        return "FRAGMENT";
    case Type::GEOMETRY:
        return "GEOMETRY";
    case Type::TESSALATION_CTRL:
        return "TESSALATION_CTRL";
    case Type::TESSALATION_EVAL:
        return "TESSALATION_EVAL";
    case Type::COMPUTE:
        return "COMPUTE";
    default:
        return "";
    }
}

void Shader::recompile() {
    if (this->id != 0) {
        glDeleteShader(this->id);
    }
    this->id = glCreateShader((GLenum)this->type);

    const char* source = this->code.c_str();
    int length = this->code.size();
    glShaderSource(this->id, 1, &source, &length);
    glCompileShader(this->id);
    GLboolean status;
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &infoLogLength);
        char log[infoLogLength];
        glGetShaderInfoLog(this->id, infoLogLength, nullptr, log);
        this->infoLog = log;
    }else{
        this->infoLog = "";
    }
}

uint Shader::getId() {
    return this->id;
}

bool Shader::getUpdated() const {
    return updated;
}

void Shader::linkTo(uint programId) {
    updated = false;
    glAttachShader(programId, id);
}
