//
// Created by tomas on 3.9.22.
//


#pragma once

#include <memory>
#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include "../SceneObject.h"
#include "Uniform.h"
#include <map>
#include <functional>

namespace Galax::Renderer::SceneObjects {

    class Animation : public SceneObject {
        typedef std::function<UniformT (const Animation& current, UniformT interpolatedValue, float percentDone)> AnimationUpdateFunction;

    public:
        enum Repeat {
            ONCE,
            LOOP,
        };

        enum Target {
            POSITION,
            ROTATION,
            SCALE,
            CUSTOM
        };

        enum StartType {
            IMMEDIATE,
            SCHEDULED
        };

        enum Ease {
            LINEAR,
            QUADRATIC_IN,
            QUADRATIC_OUT,
            QUADRATIC_IN_OUT,
            CUBIC_IN,
            CUBIC_OUT,
            CUBIC_IN_OUT,
        };

        Animation();

        explicit Animation(const std::string &name);

        Animation(const std::string &name, Target target, Repeat repeat);

        ~Animation() = default;

        /// <summary>
        /// Sets lenght of the aniamtion in miliseconds
        /// </summary>
        /// \param length
        void setLength(float length);

        void setStartType(StartType start);

        void setStartTime(float startTime);

        void setTarget(Target target);

        void setCustomTarget(std::shared_ptr<Uniform> target);

        void setRepeat(Repeat repeat);

        void setEase(Ease ease);

        void addKeyFrame(float time, const UniformT &value);

        void setUpdateFunction(AnimationUpdateFunction updateFunction);

        //UniformT getValue(float time);
        [[nodiscard]] float getLength() const {
            return length;
        };

        [[nodiscard]] StartType getStartType() const {
            return start;
        };

        [[nodiscard]]float getStartTime() const {
            return startTime;
        };

        [[nodiscard]]Target getTarget() const {
            return target;
        };

        [[nodiscard]]std::shared_ptr<Uniform> getCustomTarget() const {
            return customTarget;
        };

        [[nodiscard]]Repeat getRepeat() const {
            return repeat;
        };

        [[nodiscard]]Ease getEase() const {
            return ease;
        };

        [[nodiscard]] std::map<float, UniformT> getKeyFrames() const {
            return keyFrames;
        };

        uint getId() override {
            return -1;
        };

        void update();


        glm::vec3 getValue();


    private:
        float animationTime = 0;
        unsigned long previousTime = 0;
        float length;
        StartType start = IMMEDIATE;
        float startTimeDelta = 0;
        Target target;
        glm::vec3 value;
        std::shared_ptr<Uniform> customTarget;
        std::map<float, UniformT> keyFrames;
        Repeat repeat = ONCE;
        Ease ease = LINEAR;
        bool shouldAnimate = true;
        std::map<float, UniformT>::iterator keyFrameIterator;
        AnimationUpdateFunction updateFunction;
        bool updateFunctionSet = false;

        float easeFunction(float t);

        void updateTime();

        void setCustomTargetValue(const UniformT &u1, const UniformT &u2, float t);
        void setValue(const UniformT &u1, const UniformT &u2, float t);

    };
}

