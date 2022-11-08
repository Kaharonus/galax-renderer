//
// Created by tomas on 8.11.22.
//

#include <orbital/Gravity.h>

using namespace Galax::Orbital;
using namespace Galax::Renderer;

Gravity::Gravity() {
    this->setDirection(rp3d::Vector3(0, -9.81, 0));
}


void Gravity::setCenter(std::shared_ptr<IUniform> center) {
    this->center = center;
}

void Gravity::setCenterMass(float centerMass) {
    this->centerMass = centerMass;
}

void Gravity::setPosition(std::shared_ptr<IUniform> position) {
    this->position = position;
}

void Gravity::setMass(float mass) {
    this->mass = mass;
}

rp3d::Vector3 Gravity::getDirection() {
    auto centerPos = std::get<glm::vec3>(center->getValue());
    auto pos = std::get<glm::vec3>(position->getValue());
    auto direction = centerPos - pos;
    auto distance = glm::length(direction);
    float force = G * centerMass * mass / (distance * distance);
    auto result = rp3d::Vector3(direction.x * force, direction.y * force, direction.z * force);
    return result;
}
