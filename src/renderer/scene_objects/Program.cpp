
#include <glbinding/gl/gl.h>
#include "Program.h"
#include "../../Extensions.h"
#include <functional>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace Galax::Renderer::SceneObjects;
using namespace gl;

Program::Program() : SceneObject() {
    compiled = false;
}
Program::Program(const std::string& name) : SceneObject(name) {
    compiled = false;
}

Program::Program(const std::string& name, const std::vector<std::shared_ptr<Shader>>& shaders) : SceneObject(name) {
    compiled = false;
    for (auto shader : shaders) {
        addShader(shader);
    }
}

Program::Program(const std::string& name, const std::shared_ptr<Shader>& vertex, const std::shared_ptr<Shader>& fragment) : SceneObject(name) {
    if (vertex->getType() != Shader::Type::VERTEX) {
        throw std::runtime_error("Vertex shader is not of type VERTEX");
    }
    if (fragment->getType() != Shader::Type::FRAGMENT) {
        throw std::runtime_error("Fragment shader is not of type FRAGMENT");
    }
    this->vertexShader = vertex;
    this->fragmentShader = fragment;
    compiled = false;
}

Program::~Program() {
    if (id != 0) {
        glDeleteProgram(id);
    }
}

void Program::addShader(const std::shared_ptr<Shader>& shader) {
    switch (shader->getType()) {
    case Shader::None:
        throw std::runtime_error("Shader type is None");
        break;
    case Shader::VERTEX:
        this->vertexShader = shader;
        break;
    case Shader::FRAGMENT:
        this->fragmentShader = shader;
        break;
    case Shader::GEOMETRY:
        this->geometryShader = shader;
        break;
    case Shader::TESSALATION_CTRL:
        this->tessalationControlShader = shader;
        break;
    case Shader::TESSALATION_EVAL:
        this->tessalationEvaluationShader = shader;
        break;
    case Shader::COMPUTE:
        this->computeShader = shader;
        break;
    }
}

std::shared_ptr<Shader> Program::getShader(Shader::Type type) {
    switch (type) {
    case Shader::None:
        throw std::runtime_error("Shader type is None");
    case Shader::VERTEX:
        return this->vertexShader;
    case Shader::FRAGMENT:
        return this->fragmentShader;
    case Shader::GEOMETRY:
        return this->geometryShader;
    case Shader::TESSALATION_CTRL:
        return this->tessalationControlShader;
    case Shader::TESSALATION_EVAL:
        return this->tessalationEvaluationShader;
    case Shader::COMPUTE:
        return this->computeShader;
    }
    return nullptr;
}

bool Program::compile() {
    if(id != 0) {
        glDeleteProgram(id);
        id = 0;
    }

    if (this->vertexShader == nullptr) {
        throw std::runtime_error("Vertex shader is not set");
    }
    if (this->fragmentShader == nullptr) {
        throw std::runtime_error("Fragment shader is not set");
    }

    //Create the program
    id = glCreateProgram();
    if (id == 0) {
        throw std::runtime_error("Could not create program");
    }

    //Attach the shaders
    vertexShader->recompile();
    vertexShader->linkTo(id);
    fragmentShader->recompile();
    fragmentShader->linkTo(id);


    if (this->geometryShader != nullptr) {
        geometryShader->recompile();
        geometryShader->linkTo(id);
    }
    if (this->tessalationControlShader != nullptr) {
        tessalationControlShader->recompile();
        tessalationControlShader->linkTo(id);
    }
    if (this->tessalationEvaluationShader != nullptr) {
        tessalationEvaluationShader->recompile();
        tessalationEvaluationShader->linkTo(id);
    }
    if (this->computeShader != nullptr) {
        computeShader->recompile();
        computeShader->linkTo(id);
    }
    //Link the program
    glLinkProgram(id);

    //Check the link status
    GLboolean status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, ' ');
        glGetProgramInfoLog(id, length, &length, &log[0]);
        glDeleteProgram(id);
        id = 0;
        compiled = false;
        return false;
    }
    compiled = true;

    //Read the uniforms
    GLint count;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);
    this->uniformLocations.clear();
    for (GLint i = 0; i < count; i++) {
        GLint size;
        GLenum type;
        GLchar name[256];
        glGetActiveUniform(id, i, sizeof(name), nullptr, &size, &type, name);
        this->uniformLocations[std::hash<std::string>{}(name)] = {i};
        //uniforms.push_back(name);
    }
    return true;

}


void Program::setUniform(const std::shared_ptr<Uniform>& uniform) {
    auto uniformId = uniform->getId();
    auto location = this->uniformLocations[uniformId];
    if (location == -1) {
        //std::cerr << "Uniform " << uniform->getName() << " not found" << std::endl;
        return;
    }
    switch (uniform->getType()) {
    case Uniform::Type::BOOL:
        glProgramUniform1i(id, location, uniform->getValue<bool>());
        break;
    case Uniform::Type::INT:
        glProgramUniform1i(id, location, uniform->getValue<int>());
        break;
    case Uniform::Type::FLOAT:
        glProgramUniform1f(id, location, uniform->getValue<float>());
        break;
    case Uniform::Type::VEC2:
        glProgramUniform2fv(id, location, 1, glm::value_ptr(uniform->getValue<glm::vec2>()));
        break;
    case Uniform::Type::VEC3:
        glProgramUniform3fv(id, location, 1, glm::value_ptr(uniform->getValue<glm::vec3>()));
        break;
    case Uniform::Type::VEC4:
        glProgramUniform4fv(id, location, 1, glm::value_ptr(uniform->getValue<glm::vec4>()));
        break;
    case Uniform::Type::MAT3:
        glProgramUniformMatrix3fv(id, location, 1, GL_FALSE, glm::value_ptr(uniform->getValue<glm::mat3>()));
        break;
    case Uniform::Type::MAT4:
        glProgramUniformMatrix4fv(id, location, 1, GL_FALSE, glm::value_ptr(uniform->getValue<glm::mat4>()));
        break;
    }
}

void Program::setTexture(const std::shared_ptr<Texture>& texture, int unit) {
    auto id = texture->getNameHash();
    auto location = this->uniformLocations[id];
    if (location == -1) {
        std::cerr << "Texture " << texture->getName() << " not found" << std::endl;
        return;
    }
    texture->bind(unit);
    glUniform1i(location, unit);
    //checkError();
}

std::vector<std::shared_ptr<Shader>> Program::getShaders() {
    std::vector<std::shared_ptr<Shader>> shaders;
    if (this->vertexShader != nullptr) {
        shaders.push_back(this->vertexShader);
    }
    if (this->fragmentShader != nullptr) {
        shaders.push_back(this->fragmentShader);
    }
    if (this->geometryShader != nullptr) {
        shaders.push_back(this->geometryShader);
    }
    if (this->tessalationControlShader != nullptr) {
        shaders.push_back(this->tessalationControlShader);
    }
    if (this->tessalationEvaluationShader != nullptr) {
        shaders.push_back(this->tessalationEvaluationShader);
    }
    if (this->computeShader != nullptr) {
        shaders.push_back(this->computeShader);
    }
    return shaders;
}

void Program::addTexture(std::shared_ptr<Texture> texture) {
    auto hash = texture->getNameHash();
    this->additionalTextures[hash] = texture;
}

bool Program::shadersUpdated(){
    bool vShader = this->vertexShader->getUpdated();
    bool fShader = this->fragmentShader->getUpdated();
    bool gShader = this->geometryShader && this->geometryShader->getUpdated();
    bool tcShader = this->tessalationControlShader && this->tessalationControlShader->getUpdated();
    bool teShader = this->tessalationEvaluationShader && this->tessalationEvaluationShader->getUpdated();
    bool cShader = this->computeShader && this->computeShader->getUpdated();
    return vShader || fShader || gShader || tcShader || teShader || cShader;
}

void Program::use() {
    if (!compiled || this->shadersUpdated()) {
        auto success = compile();
    }
    //Use the program

    glUseProgram(this->id);

    for(auto [i, texture] : enumerate(this->additionalTextures)){
        setTexture(texture.second, i);
    }
}

uint Program::getTextureCount() {
    return this->additionalTextures.size();
}

uint Program::getId() {
    return id;
}

bool Program::hasTesslation() {
    return this->tessalationControlShader && this->tessalationEvaluationShader;
}

