#include "Uniform.h"

using namespace Galax::Renderer::SceneObjects;


Uniform::Uniform(const std::string &name) : IUniform(name) {
}

Uniform::Uniform(const std::string &name, Type type, const UniformT &value) : IUniform(name) {
    this->value = value;
    this->type = type;
}

Uniform::~Uniform() {
}

void Uniform::setType(Type type) {
    this->type = type;
}


Uniform::Type Uniform::getType() const {
    return type;
}

uint Uniform::getId() {
    return getNameHash();
}

void Uniform::setValue(UniformT v) {
    this->value = v;
}

Uniform::UniformT Uniform::getValue() const {
    return value;
}

Uniform::operator UniformT() {
    return value;
}

glm::vec3 Uniform::v3() {
    return std::get<glm::vec3>(value);
}

float Uniform::f() {
    return std::get<float>(value);
}
