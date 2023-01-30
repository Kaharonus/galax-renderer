
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>
#include <renderer/SceneObject.h>
#include <iostream>

using namespace Galax::Renderer;
using namespace gl;

uint SceneObject::selectedContext = 0;
std::unordered_map<std::string, uint> SceneObject::contexts;
std::unordered_map<uint, unsigned int> SceneObject::lastBoundFrameBuffer;
std::unordered_map<uint, float> SceneObject::frameTime;
std::unordered_map<uint, float> SceneObject::currentTime;

std::unordered_map<uint, glm::vec2> SceneObject::screenSize;
std::unordered_map<uint,unsigned long> SceneObject::startTime;
std::unordered_map<uint,std::unordered_set<unsigned int>> SceneObject::usedNames;
std::mutex SceneObject::mutex;

SceneObject::SceneObject() {
    this->name = getNextDefaultName();
    checkName();
}

SceneObject::SceneObject(const std::string &name, bool enableUnsafeNaming) {
    this->name = name;
    if(!enableUnsafeNaming) {
        checkName();
    }
}

void SceneObject::checkName() {
    auto hash = getNameHash();

    if (usedNames[selectedContext].find(hash) != usedNames[selectedContext].end()) {
        throw std::runtime_error("Name " + name + " already in bind");
    }

    usedNames[selectedContext].insert(this->getNameHash());
}

SceneObject::~SceneObject() {
    usedNames[SceneObject::selectedContext].erase(this->getNameHash());
}

std::string SceneObject::getName() {
    return name;
}

void SceneObject::setName(std::string name) {
    this->name = name;
}

void SceneObject::setFrameTime(float frameTime) {
    SceneObject::frameTime[selectedContext] = frameTime;
	auto current = std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();
	SceneObject::currentTime[selectedContext] = (current - startTime[selectedContext]) / 1000000.0f;

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
        std::cout << "OpenGL Error: (" << (int) err << ") " << matchError((int) err) << std::endl;
        error = true;
    }
    if (shouldThrow && error) {
//        throw std::runtime_error("OpenGL Error");
    }
}

std::string SceneObject::matchError(int error) {
    auto err = (GLenum) error;
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
    startTime[selectedContext] = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}

unsigned int SceneObject::getLastFrameBuffer() {
    return lastBoundFrameBuffer[selectedContext];
}

void SceneObject::setDrawSize(int width, int height) {
    if (width != drawWidth || height != drawHeight) {
        glViewport(0, 0, width, height);
    }

}

float SceneObject::getFrameTime() {
    return frameTime[selectedContext];
}

void SceneObject::addUniqueContext(const std::string &name) {
	if (contexts.find(name) != contexts.end()) {
		return;
	}
	contexts[name] = std::hash<std::string>{}(name);
}

void SceneObject::selectAndLockContext(const std::string &name) {
	mutex.lock();
	auto contextId = contexts[name];
	selectedContext = contextId;
}

void SceneObject::unlockContext() {
	mutex.unlock();
}

float SceneObject::getCurrentTime() {
	return SceneObject::currentTime[selectedContext];
}

void SceneObject::setLastBoundFBO(uint fbo) {
	lastBoundFrameBuffer[selectedContext] = fbo;
}
