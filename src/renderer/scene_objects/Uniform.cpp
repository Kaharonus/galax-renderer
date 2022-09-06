#include "Uniform.h"

using namespace Galax::Renderer::SceneObjects;


Uniform::Uniform(const std::string &name) : SceneObject(name) {
}

Uniform::Uniform(const std::string &name, Type type, const UniformT &value) : SceneObject(name) {
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

UniformT Uniform::getValue() const {
    return value;
}
