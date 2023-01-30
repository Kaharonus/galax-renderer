
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
using namespace Galax::Renderer;
using namespace gl;

Program::Program() : IProgram() {
    compiled = false;
}
Program::Program(const std::string& name) : IProgram(name) {

    compiled = false;
}

Program::Program(const std::string& name, const std::vector<std::shared_ptr<IShader>>& shaders) : IProgram(name) {
    compiled = false;
    for (auto shader : shaders) {
        addShader(shader);
    }
}

Program::Program(const std::string& name, const std::shared_ptr<IShader>& vertex, const std::shared_ptr<IShader>& fragment) : IProgram(name) {
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

void Program::addShader(std::shared_ptr<IShader> shader) {
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

std::shared_ptr<IShader> Program::getShader(IShader::Type type) {
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
    createProgram();
    compileAndLinkShaders();
    //Link the program
    glLinkProgram(id);
    compiled = checkStatus();
    if(!compiled){
        return false;
    }
    readUniforms();
    return true;
}


void Program::setUniform(const std::shared_ptr<IUniform>& uniform) {
    auto uniformId = uniform->getId();
    auto location = this->uniformLocations[uniformId];
    if (location == -1) {
        //std::cerr << "Uniform " << uniform->getName() << " not found" << std::endl;
        return;
    }
    switch (uniform->getType()) {
    case Uniform::Type::BOOL:
        glProgramUniform1i(id, location, std::get<bool>(uniform->getValue()));
        break;
    case Uniform::Type::INT:
        glProgramUniform1i(id, location, std::get<int>(uniform->getValue()));
        break;
    case Uniform::Type::FLOAT:
        glProgramUniform1f(id, location, std::get<float>(uniform->getValue()));
        break;
    case Uniform::Type::VEC2:
        glProgramUniform2fv(id, location, 1, glm::value_ptr(std::get<glm::vec2>(uniform->getValue())));
        break;
    case Uniform::Type::VEC3:
        glProgramUniform3fv(id, location, 1, glm::value_ptr(std::get<glm::vec3>(uniform->getValue())));
        break;
    case Uniform::Type::VEC4:
        glProgramUniform4fv(id, location, 1, glm::value_ptr(std::get<glm::vec4>(uniform->getValue())));
        break;
    case Uniform::Type::MAT3:
        glProgramUniformMatrix3fv(id, location, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat3>(uniform->getValue())));
        break;
    case Uniform::Type::MAT4:
        glProgramUniformMatrix4fv(id, location, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat4>(uniform->getValue())));
        break;
    }
}

void Program::setTexture(const std::shared_ptr<ITexture>& texture, int unit) {
    auto id = texture->getNameHash();
    auto location = this->uniformLocations[id];
    if (location == -1) {
        std::cerr << "Texture " << texture->getName() << " not found. (Raised by " << this->name <<")" << std::endl;
        return;
    }
    texture->bind(unit);
    glUniform1i(location, unit);
}

std::vector<std::shared_ptr<IShader>> Program::getShaders() {
    std::vector<std::shared_ptr<IShader>> shaders;
    if (this->vertexShader != nullptr) {
        shaders.push_back(this->vertexShader);
    }
    if (this->tessalationControlShader != nullptr) {
        shaders.push_back(this->tessalationControlShader);
    }
    if (this->tessalationEvaluationShader != nullptr) {
        shaders.push_back(this->tessalationEvaluationShader);
    }
    if (this->geometryShader != nullptr) {
        shaders.push_back(this->geometryShader);
    }
    if (this->fragmentShader != nullptr) {
        shaders.push_back(this->fragmentShader);
    }

    if (this->computeShader != nullptr) {
        shaders.push_back(this->computeShader);
    }
    return shaders;
}

void Program::addTexture(std::shared_ptr<ITexture> texture) {
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

void Program::bind() {
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



uint Program::getTexturePosition(const std::shared_ptr<ITexture>& texture) {
    auto id = texture->getNameHash();
    return this->uniformLocations[id];
}

void Program::unbind() {
    glUseProgram(0);
}

void Program::setSSBO(const std::shared_ptr<ISSBO> &ssbo) {
    auto id = ssbo->getNameHash();
    int location = this->ssboLocations[id];
    if (location == -1) {
        //Query the location

        GLuint loc = glGetProgramResourceIndex(this->id, GL_SHADER_STORAGE_BLOCK, ssbo->getName().c_str());
        if (loc == GL_INVALID_INDEX) {
            std::cerr << "SSBO " << ssbo->getName() << " not found. (Raised by " << this->name << ")" << std::endl;
            return;
        }
        this->ssboLocations[id] = {(int)loc};
        location = (int)loc;
    }

    ssbo->bind(location);

}

void Program::compileAndLinkShaders() {
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
}

bool Program::checkStatus() {
    GLboolean status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);

    if(status == GL_TRUE){
        return true;
    }
    GLint length;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
    std::string log(length, ' ');
    glGetProgramInfoLog(id, length, &length, &log[0]);
    glDeleteProgram(id);
    id = 0;
    return false;
}

void Program::readUniforms() {
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
    }
}

void Program::createProgram() {
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
       // throw std::runtime_error("Could not create program");
    }
}
