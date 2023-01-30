//
// Created by tomas on 27.9.22.
//
#pragma once

#include <glm/vec3.hpp>
#include "../SceneObject.h"
#include "IUniform.h"

namespace Galax::Renderer {
    class ILight : public Galax::Renderer::SceneObject {
    public:
        struct LightData {
            glm::vec3 position;
            float intensity;
            glm::vec3 color;
            float falloff;
        };


        ILight() : SceneObject() {};
        ILight(const std::string &name) : SceneObject(name) {};
        virtual ~ILight() = default;
        virtual void setPos(glm::vec3 pos) = 0;
        virtual void setPositionUniform(std::shared_ptr<IUniform> uniform) = 0;
        virtual void setIntensity(float intensity) = 0;
        virtual void setIntensityUniform(std::shared_ptr<IUniform> uniform) = 0;
        virtual void setColor(glm::vec3 color) = 0;
        virtual void setFalloff(float falloff) = 0;
        virtual LightData getLightData() = 0;
        virtual bool isUpdated() = 0;
    };

}