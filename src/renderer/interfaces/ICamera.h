//
// Created by tomas on 28.9.22.
//
#pragma once

#include <glm/vec3.hpp>
#include <tuple>
#include <glm/glm.hpp>
#include <memory>
#include "IUniform.h"
#include "../input/InputHandler.h"

namespace Galax::Renderer {
    class ICamera : public Galax::Renderer::SceneObject {
    public:
        ICamera() : SceneObject() {};

        ICamera(const std::string &name) : SceneObject(name) {};

        virtual ~ICamera() = default;

        virtual void update() = 0;

        virtual void setPosition(const glm::vec3 &position) = 0;

        virtual void setRotation(float yaw, float pitch) = 0;

        virtual void setMovementSpeed(float speed) = 0;

        virtual void setMouseSensitivity(float sensitivity) = 0;

        virtual void setFOV(float fov) = 0;

        virtual void setPerspective(float fov, float near, float far) = 0;

        virtual void setDimensions(float width, float height) = 0;

        virtual void setWorldUp(const glm::vec3 &worldUp) = 0;

        [[nodiscard]]virtual glm::mat4 getViewMatrix() const = 0;

        [[nodiscard]]virtual glm::mat4 getProjectionMatrix() const = 0;

        [[nodiscard]]virtual std::shared_ptr<IUniform> getViewUniform() const = 0;

        [[nodiscard]]virtual std::shared_ptr<IUniform> getProjectionUniform() const = 0;

        [[nodiscard]]virtual std::shared_ptr<IUniform> getPositionUniform() const = 0;

        [[nodiscard]]virtual std::shared_ptr<IUniform> getRotationUniform() const = 0;

        [[nodiscard]] virtual glm::vec3 getDirection() const = 0;

        [[nodiscard]]virtual glm::vec3 getPosition() const = 0;

        [[nodiscard]]virtual std::tuple<float, float> getRotation() const = 0;

        [[nodiscard]]virtual float getWidth() const = 0;

        [[nodiscard]]virtual float getHeight() const = 0;

        [[nodiscard]]virtual float getFOV() const = 0;

        [[nodiscard]]virtual float getMovementSpeed() const = 0;

        [[nodiscard]]virtual float getMouseSensitivity() const = 0;

        virtual void setInputHandler(std::shared_ptr<Galax::Renderer::InputHandler> input) = 0;

    };
}