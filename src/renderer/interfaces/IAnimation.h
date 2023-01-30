//
// Created by tomas on 27.9.22.
//
#pragma once


#include <functional>
#include "IUniform.h"
namespace Galax::Renderer {
    class IAnimation : public Galax::Renderer::SceneObject {
    public:
        typedef std::function<IUniform::UniformT(const IAnimation &current, IUniform::UniformT interpolatedValue,
                                                 float percentDone)> AnimationUpdateFunction;

        enum Target {
            POSITION,
            ROTATION,
            SCALE,
            CUSTOM
        };


        IAnimation() : SceneObject() {};

        IAnimation(const std::string &name) : SceneObject(name, true) {};

        virtual ~IAnimation() = default;

        virtual void update() = 0;

        virtual void setTarget(Target target) = 0;

        virtual void start() = 0;

        virtual void stop() = 0;

        [[nodiscard]] virtual Target getTarget() const = 0;

        [[nodiscard]] virtual std::shared_ptr<IUniform> getCustomTarget() const = 0;

        virtual glm::vec3 getValue() = 0;

    };

}