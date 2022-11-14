//
// Created by tomas on 9.11.22.
//

#include <orbital/OrbitAnimation.h>
#include <renderer/interfaces/IAnimation.h>
#include <iostream>

using namespace Galax::Orbital;
using namespace Galax::Renderer;


OrbitAnimation::OrbitAnimation() : IAnimation() {}

OrbitAnimation::OrbitAnimation(const std::string &name) : IAnimation(name) {}

uint OrbitAnimation::getId() {
    return this->getNameHash();
}

void OrbitAnimation::updateTime() {

    //Calculate the weight of the planet
    auto planetSize = this->planetSize->v3();
    auto weight = (pi_43 * planetSize.x * planetSize.y * planetSize.z) * this->massConstant;

    //Calculate the speed of the planet
    auto sunPosition = this->sunPosition->v3();
    auto distance = glm::distance(sunPosition, this->radius);
    auto speed = std::sqrt(((G * weight) / (pow(distance, 4))));

    animationLength = (animationSpeed / 1000.0) / speed;

    this->previousTime = Galax::Renderer::SceneObject::currentTime;
    this->animationTime += this->frameTime;
    if(animationTime > this->animationLength){
        animationTime = 0;
    }
    this->progress = animationTime / animationLength;
}

void OrbitAnimation::update() {

    if(Galax::Renderer::SceneObject::currentTime == this->previousTime){
        return; //Already calculated at this time, should be ok, but might not be.
    }
    assert(this->sunPosition != nullptr);

    updateTime();


    auto sunPos = this->sunPosition->v3();
    auto result = glm::vec3(0,0,0);
    auto angle = pi2 * progress;
    result.x = sunPos.x + (radius.x * glm::cos(angle + offset.x));
    result.y = sunPos.y + (radius.y * glm::sin(angle + offset.y));
    result.z = sunPos.z + (radius.z * glm::sin(angle + offset.z));

    currentPosition = result;
}

void OrbitAnimation::setPlanetRadius(std::shared_ptr<IUniform> planetRadius) {
    this->planetSize = planetRadius;
}

void OrbitAnimation::setTarget(Galax::Renderer::IAnimation::Target target) {
    // Unsupported, since this is animation for orbiting, which animates the position of the object
    assert(false);
}

IAnimation::Target OrbitAnimation::getTarget() const {
    return POSITION;
}

std::shared_ptr<IUniform> Galax::Orbital::OrbitAnimation::getCustomTarget() const {
    return nullptr;
}

glm::vec3 Galax::Orbital::OrbitAnimation::getValue() {
    return currentPosition;
}

void OrbitAnimation::setSunPosition(std::shared_ptr<IUniform> sunPosition) {
    this->sunPosition = sunPosition;
}

void OrbitAnimation::setInitialPosition(glm::vec3 initialPosition) {
    this->currentPosition = initialPosition;
    this->radius = initialPosition;
}

void OrbitAnimation::setOffset(glm::vec3 offset) {
    this->offset = offset;
}

void OrbitAnimation::start() {
    //Empty - this animation always runs
}

void OrbitAnimation::stop() {
    //Empty - this animation always runs

}








