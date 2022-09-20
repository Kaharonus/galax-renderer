
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>
#include "SceneObject.h"
#include <iostream>

using namespace Galax::Renderer;
using namespace gl;

SceneObject::SceneObject() {
    this->name = getNextDefaultName();
}

SceneObject::SceneObject(std::string name) {
    this->name = name;
}

SceneObject::~SceneObject() {
}

std::string SceneObject::getName() {
    return name;
}

void SceneObject::setName(std::string name) {
    this->name = name;
}

void SceneObject::setFrameTime(float frameTime) {
    SceneObject::frameTime = frameTime;
    SceneObject::currentTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string SceneObject::getNextDefaultName() {
    auto name = "SceneObject_" + std::to_string(SceneObject::defaultNameCounter++);
    return name;
}


uint SceneObject::getNameHash() {
    if (id != 0) {
        return id;
    }
    // Calculate hash of name and set it as id
    auto name = getName();
    id = std::hash<std::string>{}(name);
    return id;
}

void SceneObject::checkError(bool shouldThrow) {
    GLenum err;
    bool error = false;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL Error: (" << (int) err << ") " << matchError((int)err) << std::endl;
        error = true;
    }
    if (shouldThrow && error) {
        throw std::runtime_error("OpenGL Error");
    }
}

std::string SceneObject::matchError(int error) {
    auto err = (GLenum)error;
    switch (err) {
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_STACK_OVERFLOW:
            return "GL_STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW:
            return "GL_STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_CONTEXT_LOST:
            return "GL_CONTEXT_LOST";
        case GL_TABLE_TOO_LARGE:
            return "GL_TABLE_TOO_LARGE";
        default:
            return "Unknown error";
    }

}

void SceneObject::init() {
    if (initialized) {
        return;
    }
    initialized = true;
    startTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

unsigned int SceneObject::getLastFrameBuffer() {
    return lastBoundFrameBuffer;
}
