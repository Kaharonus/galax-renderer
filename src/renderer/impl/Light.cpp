//
// Created by tomas on 14.9.22.
//

#include "Light.h"
#include "Uniform.h"

using namespace Galax::Renderer;
using namespace Galax::Renderer::SceneObjects;


Light::Light() {
    init();
}

Light::Light(const std::string &name) : ILight(name) {
    init();
}

void Light::init(){
    this->intensityUniform = std::make_shared<Uniform>(this->name + "_intensity", Uniform::FLOAT ,1.0f);
    this->positionUniform = std::make_shared<Uniform>(this->name + "_position", Uniform::VEC3, glm::vec3(0.0f));
}


ILight::LightData Galax::Renderer::SceneObjects::Light::getLightData() {
    auto position = std::get<glm::vec3>(this->positionUniform->getValue());
    auto intensity = std::get<float>(this->intensityUniform->getValue());
    return {position, intensity, this->color, this->falloff};
}

void Light::setFalloff(float falloff) {
    this->falloff = falloff;
    updated = true;
}

void Light::setColor(glm::vec3 color) {
    this->color = color;
    updated = true;
}

void Light::setIntensityUniform(std::shared_ptr<IUniform> uniform) {
    this->intensityUniform = uniform;
    updated = true;
}

void Light::setIntensity(float intensity) {
    this->intensityUniform->setValue(intensity);
    updated = true;
}

void Light::setPositionUniform(std::shared_ptr<IUniform> uniform) {
    this->positionUniform = uniform;
    updated = true;
}

void SceneObjects::Light::setPos(glm::vec3 pos) {
    this->positionUniform->setValue(pos);
    updated = true;
}

bool Light::isUpdated() {
    auto pos = std::get<glm::vec3>(this->positionUniform->getValue());
    auto in = std::get<float>(this->intensityUniform->getValue());
    auto positionUpdated = this->position != pos;
    auto intensityUpdated = this->intensity != in;
    this->position = pos;
    this->intensity = in;
    auto isUpdated = positionUpdated || intensityUpdated || updated;
    updated = false;
    return isUpdated;
}

