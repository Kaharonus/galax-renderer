//
// Created by tomas on 3.9.22.
//

#include <renderer/impl/Animation.h>
#include <iostream>

using namespace Galax::Renderer::SceneObjects;

Animation::Animation() : IAnimation() {
}

Animation::Animation(const std::string& name) : IAnimation(name) {

}

Animation::Animation(const std::string& name, Target target, Repeat repeat) : IAnimation(name), target(target), repeat(repeat) {

}

void Animation::setLength(float length) {
    this->length = length;
}

void Animation::setStartType(StartType start) {
    this->startType = start;
}

void Animation::setStartTime(float startTime) {
    this->startTime = startTime;
}

void Animation::setTarget(Target target) {
    this->target = target;
}

void Animation::setCustomTarget(std::shared_ptr<Uniform> target) {
    assert(this->target == CUSTOM); // Well... obviously
    this->customTarget = target;
}

void Animation::setRepeat(Repeat repeat) {
    this->repeat = repeat;
}

void Animation::setEase(Ease ease) {
    this->ease = ease;
}

void Animation::addKeyFrame(float time, const IUniform::UniformT& value) {
    keyFrames[time] = value;
    keyFrameIterator = keyFrames.begin();
}

glm::vec3 Animation::getValue() {
    assert(target != CUSTOM); //When the custom target is used, the value is updated automagically on update
    return value;
}

void Animation::setUpdateFunction(AnimationUpdateFunction updateFunction) {
    this->updateFunction = std::move(updateFunction);
    this->updateFunctionSet = true;
}


void Animation::setCustomTargetValue(const IUniform::UniformT& u1, const IUniform::UniformT& u2, float t){
    IUniform::UniformT result;
    switch (customTarget->getType()) {
        case Uniform::BOOL:
            result = glm::mix(std::get<bool>(u1), std::get<bool>(u2), t);
            break;
        case Uniform::INT:
            result = glm::mix(std::get<int>(u1), std::get<int>(u2), t);
            break;
        case Uniform::Type::FLOAT:
            result = glm::mix(std::get<float>(u1), std::get<float>(u2), t);
            break;
        case Uniform::Type::VEC2:
            result = glm::mix(std::get<glm::vec2>(u1), std::get<glm::vec2>(u2), t);
            break;
        case Uniform::Type::VEC3:
            result = glm::mix(std::get<glm::vec3>(u1), std::get<glm::vec3>(u2), t);
            break;
        case Uniform::Type::VEC4:
            result = glm::mix(std::get<glm::vec4>(u1), std::get<glm::vec4>(u2), t);
            break;
        case Uniform::MAT3:
        case Uniform::Type::MAT4:
            assert(false); //Not implemented
            break;
    }
    if(this->updateFunctionSet){
        result =this->updateFunction(*this, result, t);
    }
    customTarget->setValue(result);
}

void Animation::setValue(const IUniform::UniformT& u1, const IUniform::UniformT& u2, float t){
    auto result = glm::mix(std::get<glm::vec3>(u1), std::get<glm::vec3>(u2), t);
    if(this->updateFunctionSet){
        result = std::get<glm::vec3>(this->updateFunction(*this, result, t));
    }
    value = result;

}


float Animation::easeFunction(float time){
    float t = time;
    switch (ease) {
        case LINEAR:
            break;
        case QUADRATIC_IN:
            t = t * t;
            break;
        case QUADRATIC_OUT:
            t = t * (2 - t);
            break;
        case QUADRATIC_IN_OUT:
            t = t < .5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
            break;
        case CUBIC_IN:
            t = t * t * t;
            break;
        case CUBIC_OUT:
            t = (--t) * t * t + 1;
            break;
        case CUBIC_IN_OUT:
            t = t < .5 ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
            break;
    }
    return t;
}


void Animation::updateTime(){

    if(this->startType == StartType::MANUAL && !shouldAnimate){
        return;
    }
    animationTime += frameTime;
    this->previousTime = currentTime;

    //Add handling of delayed start time

    shouldAnimate = true;
    if(animationTime > length){
        switch (repeat) {
            case ONCE:
                animationTime = length;
                shouldAnimate = false;
                break;
            case LOOP:
                animationTime = 0;
                keyFrameIterator = keyFrames.begin();
                break;
        }
    }

}


void Animation::update() {
    if(Galax::Renderer::SceneObjects::Animation::currentTime == this->previousTime){
        return; //Already calculated at this time.
    }
    updateTime();
    if(!shouldAnimate){
        return; //Probably before or after animation
    }


    auto currentKeyFrame = keyFrameIterator;
    auto nextKeyFrame = std::next(currentKeyFrame);

    auto keyFramePercentage = (animationTime - currentKeyFrame->first) / (nextKeyFrame->first - currentKeyFrame->first);
    if(keyFramePercentage > 1){
        keyFrameIterator++;
        currentKeyFrame = keyFrameIterator;
        nextKeyFrame = std::next(currentKeyFrame);
        if(nextKeyFrame == keyFrames.end()){
            return; //Animation is over
        }
    }
    if(keyFramePercentage < 0){
        keyFrameIterator = keyFrames.begin();
        currentKeyFrame = keyFrameIterator;
        nextKeyFrame = std::next(currentKeyFrame);
        return;
    }

    auto t = easeFunction(animationTime / length);

    if(target == CUSTOM){
        setCustomTargetValue(currentKeyFrame->second, nextKeyFrame->second, t);
    }else{
        setValue(currentKeyFrame->second, nextKeyFrame->second, t);
    }

}

void Animation::start() {
    animationTime = 0;
    keyFrameIterator = keyFrames.begin();
    shouldAnimate = true;
}

void Animation::stop() {
    shouldAnimate = false;
}



